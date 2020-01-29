/* If filename is nmea_parser.c than it is generated from nmea_parser.rl.
 * please see README.md for more details.
 * Do not edit! */
/*
 * nmea_parser.rl
 *
 * Copyright (c) 2020 Usama Masood <mirzaon@gmail.com> All rights reserved.
 *
 * This file is part of a non-commercial software. For more details please
 * refer to the license agreement that comes with this software.
 *
 * If you have not received a license file please contact:
 *  Usama Masood <mirzaon@gmail.com>
 *
 */
#include <kernel.h>

#ifdef CONFIG_NMEA
#include <nmea.h>
#include <fs.h>
#include <fs_buffer.h>

%%{
    # Define machine.
    machine nmea;

    action csum_reset
    {
        /* Reset message checksum. */
        csum = 0;
    }
    action csum_set
    {
        /* Save message checksum */
        csum_got = (uint8_t)(csum_got << (8 * index));
        csum_got |= (uint8_t)(((*p > '9') ? 'A':'0') - *p);
    }
    action index_reset
    {
        /* Reset index. */
        index = 0;
        have_dot = FALSE;
    }
    action talker_set
    {
        /* Save the talker ID. */
        msg->talker_id[index != 0] = *p;
        index++;
    }
    action utc_set
    {
        /* Update UTC */
        nmea_parser_set_value(&msg->data.gaa.utc, &index, &have_dot, *p, 3);
    }
    action lat_set
    {
        /* Update Latitude */
        nmea_parser_set_value(&msg->data.gaa.latitude, &index, &have_dot, *p, 5);
    }
    action lat_ns_set
    {
        msg->data.gaa.latitude_ns = *p;
    }
    action lon_set
    {
        /* Update Longitude */
        nmea_parser_set_value(&msg->data.gaa.longitude, &index, &have_dot, *p, 5);
    }
    action lon_ew_set
    {
        msg->data.gaa.longitude_ew = *p;
    }
    action fix_set
    {
        msg->data.gaa.fix = (uint8_t)(*p - '0');
    }
    action sat_used_set
    {
        msg->data.gaa.used = (uint8_t)(msg->data.gaa.used * 10);
        msg->data.gaa.used = (uint8_t)(*p - '0' + msg->data.gaa.used);
    }
    action hdop_set
    {
        /* Update HDOP. */
        nmea_parser_set_value(&msg->data.gaa.hdop, &index, &have_dot, *p, 3);
    }
    action alt_set
    {
        /* Update altitude. */
        nmea_parser_set_value(&msg->data.gaa.altitude, &index, &have_dot, *p, 3);
    }
    action alt_unit_set
    {
        /* Save the altitude units. */
        msg->data.gaa.alt_unit = *p;
    }
    action geoid_neg_set
    {
        /* Set the GEOID as negative. */
        msg->data.gaa.geoid_neg = TRUE;
    }
    action geoid_set
    {
        /* Update GEOID. */
        nmea_parser_set_value(&msg->data.gaa.geoid_sep, &index, &have_dot, *p, 3);
    }
    action geoid_unit_set
    {
        /* Save the GEOID units. */
        msg->data.gaa.geoid_unit = *p;
    }

    # Start of a message.
    start = (alnum|[,.*\r\n])* '$'{1} @csum_reset;

    # Match and save talker.
    talker = ((upper)@talker_set){2};

    # Match GGA message.
    gga = 'G''G''A'
          ','@index_reset(((digit|[.])@utc_set)+){,1}       # UTC
          ','@index_reset(((digit|[.])@lat_set)+){,1}       # Latitude
          ','([NS]@lat_ns_set){,1}                          # N/S
          ','@index_reset(((digit|[.])@lon_set)+){,1}       # Longitude
          ',' ([EW]@lon_ew_set){,1}                         # E/W
          ','(digit@fix_set){,1}                            # Fix Indicator
          ','@index_reset((digit@sat_used_set){1,2}){,1}    # Setallites used
          ','@index_reset(((digit|[.])@hdop_set)+){,1}      # HDOP
          ','@index_reset(((digit|[.])@alt_set)+){,1}       # MSL Altitude
          ','(alpha@alt_unit_set){,1}                       # MSL units
          ','(('-')@geoid_neg_set){,1}                      # GEOID +/-
             @index_reset(((digit|[.])@geoid_set)+){,1}     # GEOID seperation
          ','(alpha@geoid_unit_set){,1}                     # GEOID units
          ','((digit|[.])+){,1}                             # Age of Diff. Corr.
          ','((digit|[.])+){,1};                            # Diff. Ref. Station ID

    # Machine entry.
    main := start@index_reset talker gga '*'((digit|/[A-F]/)@csum_set){2}>index_reset'\r''\n';
}%%

/* Machine definitions. */
%% write data;

/*
 * nmea_parse_message
 * @nmea: NMEA instance.
 * @msg: Parsed message will be returned here.
 * This function will return a parsed reading from a NMEA bus/device.
 */
int32_t nmea_parse_message(NMEA *nmea, NMEA_MSG *msg)
{
    int32_t status = SUCCESS;
    uint8_t chr[2];
    uint8_t *p = &chr[0], *pe = &chr[1];
    uint8_t index, have_dot;
    uint8_t csum, csum_got = 0;
    char cs = nmea_start;
    FS *fs = (FS *)nmea->fd;
    FS_BUFFER_LIST *buffer = NULL;

    /* Remove some compiler warning. */
    UNUSED_PARAM(nmea_en_main);

    for (;;)
    {
        /* If this is a buffer file descriptor. */
        if (fs->flags & FS_BUFFERED)
        {
            /* If we don't have any data to process. */
            if ((buffer == NULL) || (buffer->total_length == 0))
            {
                /* If we have a last buffer. */
                if (buffer != NULL)
                {
                    /* Lock the file descriptor. */
                    fd_get_lock(nmea->fd);

                    /* Free the last buffer. */
                    fs_buffer_add(nmea->fd, buffer, FS_LIST_FREE, FS_BUFFER_ACTIVE);

                    /* Release file descriptor lock. */
                    fd_release_lock(nmea->fd);
                }

                /* Read a byte from the stream. */
                status = fs_read(nmea->fd, (void *)&buffer, sizeof(buffer));
            }

            /* If we do have a valid buffer. */
            if ((status == sizeof(buffer)) && (buffer != NULL) && (buffer->total_length > 0))
            {
                /* Lock the file descriptor. */
                fd_get_lock(nmea->fd);

                /* Pull a byte from the buffer. */
                (void)fs_buffer_list_pull(buffer, chr, 1, FS_BUFFER_HEAD);

                /* Release file descriptor lock. */
                fd_release_lock(nmea->fd);
            }
            else
            {
                /* If we did not read expected data. */
                if (status >= 0)
                {
                    /* Return error to the caller. */
                    status = NMEA_READ_ERROR;
                }

                break;
            }
        }
        else
        {
            /* Read a byte from the stream. */
            status = fs_read(nmea->fd, chr, 1);

            if (status != 1)
            {
                /* Return error to the caller. */
                status = NMEA_READ_ERROR;

                break;
            }
        }

        /* Reset the read pointer. */
        p = chr;

        /* Update the checksum. */
        csum ^= *p;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough="
#pragma GCC diagnostic ignored "-Wchar-subscripts"
#pragma GCC diagnostic ignored "-Wsign-conversion"
        %% write exec;
#pragma GCC diagnostic pop

        /* Check if machine is now in finished state. */
        if ((cs == nmea_first_final) || (cs == nmea_error))
        {
            /* State machine error. */
            if (cs == nmea_error)
            {
                /* Return error that invalid sequence was read. */
                status = NMEA_SEQUENCE_ERROR;
            }
            break;
        }
    }

    /* Return status to the caller. */
    return (status);

} /* nmea_parse_message */

#endif /* CONFIG_NMEA */
