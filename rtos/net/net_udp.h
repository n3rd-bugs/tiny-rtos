/*
 * net_udp.h
 *
 * Copyright (c) 2015 Usama Masood <mirzaon@gmail.com>
 *
 * Standard MIT License apply on this source code, with the inclusion of below
 * clause.
 *
 * This source is for educational purpose only, and should never be used for
 * any other purpose. If this source is used for other than educational purpose
 * (in any form) the author will not be liable for any outcome from its direct
 * or indirect use.
 */
#ifndef _NET_UDP_H_
#define _NET_UDP_H_
#include <kernel.h>

#ifdef CONFIG_NET
#include <net.h>

#ifdef NET_UDP
#include <console.h>

/* UDP stack configuration. */
#define UDP_CSUM

/* UDP header definitions. */
#define UDP_HRD_LENGTH              (8)
#define UDP_HRD_SRC_PORT_OFFSET     (0)
#define UDP_HRD_DST_PORT_OFFSET     (2)
#define UDP_HRD_LEN_OFFSET          (4)
#define UDP_HRD_CSUM_OFFSET         (6)

/* UDP port flags. */
#define UDP_FLAG_THR_BUFFERS        (0x01)

/* UDP port structure. */
typedef struct _udp_port UDP_PORT;
struct _udp_port
{
    /* Console structure for this UDP port. */
    CONSOLE         console;

    /* UDP buffer lists. */
    struct _udp_port_buffer_list
    {
        FS_BUFFER       *head;
        FS_BUFFER       *tail;
    } buffer_list;

    /* UDP port list member. */
    UDP_PORT        *next;

    /* UDP socket address. */
    SOCKET_ADDRESS  socket_address;

    /* Destination address. */
    SOCKET_ADDRESS  destination_address;

    /* Last datagram address. */
    SOCKET_ADDRESS  last_datagram_address;

    /* UDP port flags. */
    uint8_t         flags;

    /* Padding variable. */
    uint8_t         pad[7];

};

/* UDP global data. */
typedef struct _udp_data
{
    /* Port list. */
    struct _udp_data_port_list
    {
        UDP_PORT    *head;
        UDP_PORT    *tail;
    } port_list;

#ifdef CONFIG_SEMAPHORE
    /* Data lock to protect global UDP data. */
    SEMAPHORE   lock;
#endif

} UDP_DATA;

/* UDP port search parameter. */
typedef struct _udp_port_param
{
    /* Resolved UDP port. */
    UDP_PORT        *port;

    /* Socket search data. */
    SOCKET_ADDRESS  socket_address;
} UDP_PORT_PARAM;

/* Function prototypes. */
void udp_initialize();
void udp_register(UDP_PORT *, char *, SOCKET_ADDRESS *);
void udp_unregister(UDP_PORT *);
int32_t net_process_udp(FS_BUFFER *, uint32_t, uint32_t, uint32_t, uint32_t);
int32_t udp_header_add(FS_BUFFER *, SOCKET_ADDRESS *, uint8_t);

#endif /* NET_UDP */
#endif /* CONFIG_NET */
#endif /* _NET_UDP_H_ */
