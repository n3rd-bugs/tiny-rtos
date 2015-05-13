/*
 * enc28j60_target.h
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

#ifndef _ENC28J60_TARGET_H_
#define _ENC28J60_TARGET_H_

#include <os.h>
#include <ethernet.h>

#ifdef ETHERNET_ENC28J60
#include <enc28j60_stm32f407.h>

/* Hook-up enc28j60 driver. */
#define ENC28J60_ENABLE_INT     enc28j60_stm32f407_enable_interrupt
#define ENC28J60_DISABLE_INT    enc28j60_stm32f407_disable_interrupt

#endif /* ETHERNET_ENC28J60 */

#endif /* _ENC28J60_TARGET_H_ */
