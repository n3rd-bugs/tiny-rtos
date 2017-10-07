/*
 * atmegaxx4.h
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
#ifndef _ATMEGAXX4_H_
#define _ATMEGAXX4_H_

#ifdef CONFIG_BOOTLOAD
#include <bootload_avr.h>
#endif /* CONFIG_BOOTLOAD */

#ifdef CONFIG_SERIAL
#include <usart_avr.h>
#endif /* CONFIG_SERIAL */

#ifdef CONFIG_FS
#include <fs_target.h>
#endif /* CONFIG_FS */

/* Any other configuration is being managed by Eclipse plugin for avr-gcc. */

#endif /* _ATMEGAXX4_H_ */