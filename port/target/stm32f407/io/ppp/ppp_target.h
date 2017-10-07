/*
 * ppp_target.h
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
#ifndef _PPP_TARGET_H_
#define _PPP_TARGET_H_
#include <kernel.h>

#ifdef CONFIG_PPP
#include <ppp_stm32f407.h>

/* Hook-up PPP OS stack. */
#define PPP_TGT_INIT  ppp_stm32f407_init

#endif /* CONFIG_PPP */
#endif /* _PPP_TARGET_H_ */