/*
 * serial.c
 *
 * Copyright (c) 2017 Usama Masood <mirzaon@gmail.com>
 *
 * Standard MIT License apply on this source code, with the inclusion of below
 * clause.
 *
 * This source is for educational purpose only, and should never be used for
 * any other purpose. If this source is used for other than educational purpose
 * (in any form, direct or indirect) the author will not be liable for any
 * outcome.
 */
#include <kernel.h>

#ifdef CONFIG_SERIAL
#include <stdio.h>
#include <serial.h>
#include <serial_target.h>
#include <net.h>
#include <lcd_an.h>
#include <string.h>

#ifdef FS_CONSOLE
/* Debug file descriptor. */
FD debug_fd = NULL;
#else
SERIAL *debug_serial = NULL;
#endif

#ifdef FS_CONSOLE
/* Internal function prototypes. */
static int32_t serial_write(void *, const uint8_t *, int32_t);
static int32_t serial_read(void *, uint8_t *, int32_t);
#endif

/*
 * serial_init
 * This will initialize serial sub system.
 */
void serial_init()
{
#ifdef SERIAL_TGT_INIT
    /* Initialize serial target. */
    SERIAL_TGT_INIT();
#endif
} /* serial_init */

/*
 * serial_register
 * @serial: Serial port needed to be registered.
 * @name: Name of serial port device.
 * This will register a serial port.
 */
void serial_register(SERIAL *serial, const char *name, void *buffer, uint32_t flags)
{
#ifdef FS_CONSOLE
    FS_BUFFER_DATA *buffer_data = (FS_BUFFER_DATA *)buffer;
    char fs_name[64];
#else

    /* Remove some compiler warnings. */
    UNUSED_PARAM(name);
    UNUSED_PARAM(buffer);
#endif

    /* Initialize serial target device. */
    if (serial->device.init(serial->device.data) == SUCCESS)
    {
        /* Save the serial flags. */
        serial->flags = flags;

#ifdef FS_CONSOLE
        /* Initialize console data. */
        serial->console.fs.name = name;
        serial->console.fs.write = &serial_write;
        serial->console.fs.read = &serial_read;

        /* There is space available to be written. */
        serial->console.fs.flags |= (FS_SPACE_AVAILABLE);

        /* If we want to use interrupt mode. */
        if (flags & SERIAL_INT)
        {
            /* Enable blocking mode. */
            serial->console.fs.flags |= FS_BLOCK;
        }
        else
        {
            /* There is always data available on the port. */
            serial->console.fs.flags |= FS_DATA_AVAILABLE;
        }

        /* Register serial port with console. */
        console_register(&serial->console);
#endif

        /* If this is to be used as debug console. */
        if (flags & SERIAL_DEBUG)
        {
#ifdef FS_CONSOLE
            /* Set debug file descriptor. */
            snprintf(fs_name, 64, "\\console\\%s", name);
            debug_fd = fs_open(fs_name, 0);
#else
            /* Set the debug serial port. */
            debug_serial = serial;
#endif
        }

#ifdef FS_CONSOLE
        /* If this we need to use interrupts to transfer data on this
         * port. */
        if (flags & SERIAL_INT)
        {
            /* User should have provided the buffer data. */
            ASSERT(buffer_data == NULL);

            /* Initialize buffer data. */
            fs_buffer_dataset(serial, buffer_data);

            /* This is a buffered descriptor. */
            serial->console.fs.flags |= FS_BUFFERED;

            /* Assign interrupt data. */
            semaphore_set_interrupt_data(&serial->console.lock, serial->device.data, serial->device.int_lock, serial->device.int_unlock);

            /* Unlock serial interrupts. */
            serial->device.int_unlock(serial->device.data);
        }
#endif
    }

} /* serial_register */

#ifdef FS_CONSOLE
/*
 * serial_write
 * @fs: Serial data for which this was called.
 * @buf: String needed to be printed.
 * @nbytes: Number of bytes to be printed from the string.
 * @return: Number of bytes will be returned if write was successful.
 * This function will send a given buffer on the serial port.
 */
static int32_t serial_write(void *fs, const uint8_t *buf, int32_t nbytes)
{
    SERIAL *serial = (SERIAL *)fs;
    FS_BUFFER *buffer = (FS_BUFFER *)buf;
    int32_t n;

    /* If this is a buffered serial port. */
    if ((serial != NULL) && (serial->flags & SERIAL_INT))
    {
        /* Add a buffer to be transmitted on the transmission list. */
        fs_buffer_add(serial, buffer, FS_BUFFER_TX, FS_BUFFER_ACTIVE);

        /* Start TX on the serial port. */
        serial->device.puts(fs, serial->device.data, NULL, 0, SERIAL_INT);

        /* Return number of bytes provided. */
        n = nbytes;
    }
    else
    {
        /* Transmit required buffer on the serial. */
        n = serial->device.puts(fs, serial->device.data, (uint8_t *)buf, nbytes, 0);
    }

    /* Return number of bytes transmitted. */
    return (n);

} /* serial_write */

/*
 * serial_read
 * @priv_data: Serial data for which this was called.
 * @buf: Buffer in which data is needed to be read.
 * @nbytes: At most number of bytes to be read.
 * @return: Number of bytes read will be returned here.
 * This function will read data from given serial port.
 */
static int32_t serial_read(void *fs, uint8_t *buf, int32_t nbytes)
{
    SERIAL *serial = (SERIAL *)fs;
    FS_BUFFER *buffer;
    int32_t n = nbytes;

    /* If this is a buffered serial port. */
    if ((serial != NULL) && (serial->flags & SERIAL_INT))
    {
        /* See if we have a buffer to read from. */
        buffer = fs_buffer_get(serial, FS_BUFFER_RX, 0);

        /* If we have a buffer and we are in TX. */
        if (buffer != NULL)
        {
            /* Return a buffer from the RX list. */
            (*((FS_BUFFER **)buf)) = buffer;
        }
    }
    else
    {
        /* Read some data in the given buffer. */
        n = serial->device.gets(fs, serial->device.data, buf, nbytes, 0);
    }

    /* Return number of bytes read from the serial port. */
    return (n);

} /* serial_read */
#endif

/*
 * serial_assert_puts
 * @buffer: Buffer to be printed on the debug serial.
 * @nbytes: Number of bytes to be printed.
 * This function prints a giver buffer on the debug serial, this is usually
 * called from the context of the assert.
 */
void serial_assert_puts(uint8_t *buffer, int32_t nbytes)
{
#ifdef FS_CONSOLE
    SERIAL *serial = (SERIAL *)debug_fd;
#else
    SERIAL *serial = debug_serial;
#endif

    /* Assert if debug serial is not yet initialized. */
    ASSERT(serial == NULL);

    /* Transmit required buffer on the serial. */
    serial->device.puts(serial, serial->device.data, buffer, ((nbytes == 0) ? (int32_t)strlen((char *)buffer) : nbytes), 0);

} /* serial_assert_puts */

/*
 * serial_puts
 * @buf: Buffer to be printed on the serial port.
 * @n: Number of bytes to print from the buffer.
 * This function prints a formated string on the serial debug port.
 */
static int32_t serial_puts(uint8_t *buf, int32_t n)
{
#ifdef FS_CONSOLE
    SERIAL *serial = (SERIAL *)debug_fd;
#else
    SERIAL *serial = debug_serial;
#endif

    /* If we do have a serial port. */
    if (serial != NULL)
    {
#ifdef FS_CONSOLE
        /* Write given string on the serial port. */
        n = fs_puts(serial, buf, n);
#else
        /* Print the result on the serial. */
        n = debug_serial->device.puts(serial, serial->device.data, buf, n, 0);
#endif /* FS_CONSOLE */

#ifdef LCD_AN_DEBUG
        if ((n > 0) && (lcd_an_fd != NULL))
        {
            /* Print this string on the LCD-AN. */
            n = fs_puts(lcd_an_fd, buf, n);
        }
#endif
    }

    /* Print number of bytes printed. */
    return (n);

} /* serial_puts */

/*
 * serial_printf
 * @format: Formated string to be printed on serial debug port.
 * This function prints a formated string on the serial debug port.
 */
int32_t serial_printf(char *format, ...)
{
    uint8_t print_buffer[SERIAL_PRINTF_BUFFER_SIZE], *buf = print_buffer;
    int32_t n = 0;
    va_list vl;

    /* Arguments start from the format. */
    va_start(vl, format);

    /* Process the given string and save the result in a temporary buffer. */
    n = vsnprintf((char *)buf, SERIAL_PRINTF_BUFFER_SIZE, format, vl);

    if (n > 0)
    {
        /* Put this buffer on the serial port. */
        n = serial_puts(buf, n);
    }

    /* Destroy the argument list. */
    va_end(vl);

    /* Return number of bytes printed on serial port. */
    return (n);

} /* serial_printf */

/*
 * serial_vprintf
 * @format: Formated string to be printed on serial debug port.
 * This function prints a formated message on the serial debug port.
 */
int32_t serial_vprintf(const char *format, va_list vl)
{
    int32_t n;
    uint8_t buf[SERIAL_PRINTF_BUFFER_SIZE];

    /* Process the given string and save the result in a temporary buffer. */
    n = vsnprintf((char *)buf, SERIAL_PRINTF_BUFFER_SIZE, format, vl);

    if (n > 0)
    {
        /* Put this buffer on the serial port. */
        n = serial_puts(buf, n);
    }

    /* Return number of bytes printed on UART. */
    return (n);

} /* serial_vprintf */
#endif /* FS_CONSOLE */
