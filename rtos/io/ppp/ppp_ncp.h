/*
 * ppp_ncp.h
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
#ifndef _PPP_NCP_H_
#define _PPP_NCP_H_

#include <os.h>

#ifdef CONFIG_PPP

/* Exported variables. */
extern PPP_PROTO ppp_proto_ncp;

/* Function prototypes. */
uint8_t ppp_ncp_option_negotiable(PPP *, PPP_PKT_OPT *);
int32_t ppp_ncp_option_pocess(PPP *, PPP_PKT_OPT *, PPP_PKT *);
uint8_t ppp_ncp_option_length_valid(PPP *, PPP_PKT_OPT *);
int32_t ppp_ncp_update(void *, PPP *, PPP_PKT *, PPP_PKT *);

#endif /* CONFIG_PPP */

#endif /* _PPP_NCP_H_ */
