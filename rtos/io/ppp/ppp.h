/*
 * ppp.h
 *
 * Copyright (c) 2015 Usama Masood <mirzaon@gmail.com>
 *
 * Standard MIT License apply on this source code, with the inclusion of below
 * clause.
 *
 * This source is for educational purpose only, and should never be used for
 * any other purpose. If this source is used for other than educational purpose
 * (in any form) the author will not be liable for any legal charges.
 */
#ifndef _PPP_H_
#define _PPP_H_

#include <os.h>

#ifdef CONFIG_PPP
#include <ppp_fcs.h>
#include <ppp_packet.h>
#include <ppp_hdlc.h>

/* PPP configuration. */
#define PPP_MODEM_CHAT

/* Status codes. */
#define PPP_INVALID_HEADER          -900
#define PPP_NOT_SUPPORTED           -901
#define PPP_VALUE_NOT_VALID         -902
#define PPP_NO_NEXT_OPTION          -903
#define PPP_NO_BUFFERS              -904
#define PPP_NO_SPACE                -905
#define PPP_INTERNAL_ERROR          -906

/* PPP instance states. */
#define PPP_STATE_CONNECTED         1
#define PPP_STATE_LCP               2
#define PPP_STATE_IPCP              3
#define PPP_STATE_NETWORK           4
#define PPP_STATE_DISCONNECTED      5

/* PPP configuration flags. */
#define PPP_FALG_ACFC               0x01
#define PPP_FLAG_PFC                0x02

/* ACFC and PFC helper macros. */
#define PPP_IS_ACFC_VALID(ppp)      ((ppp)->lcp_flags & PPP_FALG_ACFC)
#define PPP_IS_PFC_VALID(ppp)       ((ppp)->lcp_flags & PPP_FLAG_PFC)

/* LCP code definitions. */
#define PPP_CONFIG_NONE             0
#define PPP_CONFIG_REQ              1
#define PPP_CONFIG_ACK              2
#define PPP_CONFIG_NAK              3
#define PPP_CONFIG_REJECT           4
#define PPP_TREM_REQ                5
#define PPP_TREM_ACK                6
#define PPP_CODE_REJECT             7
#define PPP_PROTO_REJECT            8
#define PPP_ECHO_REQ                9
#define PPP_ECHO_REP                10
#define PPP_DIS_REQ                 11

/* LCP option definitions. */
#define PPP_LCP_OPT_MRU             1
#define PPP_LCP_OPT_ACCM            2
#define PPP_LCP_OPT_AUTH_PROTO      3
#define PPP_LCP_OPT_QUAL_PROTO      4
#define PPP_LCP_OPT_MAGIC           5
#define PPP_LCP_OPT_PFC             7
#define PPP_LCP_OPT_ACFC            8

/* IPCP option definitions. */
#define PPP_IPCP_OPT_ADDRESSES      1
#define PPP_IPCP_OPT_COMP           2
#define PPP_IPCP_OPT_ADDRESS        3

/* PPP protocol definitions. */
#define PPP_FLAG                    (0x7E)
#define PPP_ADDRESS                 (0xFF)
#define PPP_CONTROL                 (0x03)

/* PPP packet type definitions. */
#define PPP_PROTO_LCP               (0xC021)
#define PPP_PROTO_IPCP              (0x8021)

/* PPP authentication protocol definitions. */
#define PPP_AUTH_PAP                (0xC023)
#define PPP_AUTH_CHAP               (0xC223)

/* PPP instance data. */
typedef struct _ppp_data
{
#ifdef CONFIG_SEMAPHORE
    SEMAPHORE   lock;
#endif

    /* File system watchers. */
    FS_DATA_WATCHER         data_watcher;
    FS_CONNECTION_WATCHER   connection_watcher;

    uint8_t     magic_number[4];

    /* PPP instance state. */
    uint32_t    state;

    /* PPP LCP configuration flags. */
    uint32_t    lcp_flags;

    /* ACCM definitions. */
    uint32_t    tx_accm[8];
    uint32_t    rx_accm;

    /* MRU value. */
    uint32_t    mru;

    /* IP address configured. */
    uint32_t    ip_address;

    union _ppp_state_data
    {
        /* ID used in last LCP request. */
        uint8_t     lcp_id;

        /* ID used in last IPCP request. */
        uint8_t     ipcp_id;
    } state_data;

    /* Structure padding. */
    uint8_t     pad[3];
} PPP;

/* PPP protocol definition. */
typedef struct _ppp_proto
{
    uint8_t (*negotiable)(PPP *, PPP_PKT_OPT *);
    uint8_t (*length_valid)(PPP *, PPP_PKT_OPT *);
    int32_t (*process)(PPP *, PPP_PKT_OPT *, PPP_PKT *);
    int32_t (*update)(void *, PPP *, PPP_PKT *, PPP_PKT *);

    uint16_t protocol;
    uint8_t pad[2];
} PPP_PROTO;

/* Function prototypes. */
void ppp_register_fd(PPP *, FD fd);
void ppp_connection_established(void *, void *);
void ppp_connection_terminated(void *, void *);
void ppp_rx_watcher(void *, void *);
void ppp_tx_watcher(void *, void *);

/* PPP internal APIs. */
uint32_t ppp_get_buffer_head_room(PPP *);
void ppp_process_modem_chat(void *, PPP *);
void ppp_process_frame(void *, PPP *);
void ppp_configuration_process(void *, PPP *, FS_BUFFER *, PPP_PROTO *);

#ifdef PPP_MODEM_CHAT
#include <modem_chat.h>
#endif
#include <ppp_lcp.h>
#include <ppp_ipcp.h>

#endif /* CONFIG_PPP */

#endif /* _PPP_H_ */