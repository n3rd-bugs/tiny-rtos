/*
 * ethernet_target.h
 *
 * Copyright (c) 2015 Usama Masood <mirzaon@gmail.com>
 *
 * Standard MIT License apply on this source code, with the inclusion of below
 * clause.
 *
 * This source is for educational purpose only, and should never be used for
 * any other purpose. If this source is used for other than educational purpose
 * (in any form, direct or indirect) the author will not be liable for any
 * outcome.
 */
#ifndef _ETHERNET_TARGET_H_
#define _ETHERNET_TARGET_H_
#include <kernel.h>

#ifdef CONFIG_ETHERNET
#include <ethernet_avr.h>

/* Hook-up ethernet OS stack. */
#define ETHERENET_TGT_INIT  ethernet_avr_init

#endif /* CONFIG_ETHERNET */
#endif /* _ETHERNET_TARGET_H_ */