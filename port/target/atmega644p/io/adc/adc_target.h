/*
 * adc_target.h
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

#ifndef _ADC_TARGET_H_
#define _ADC_TARGET_H_

#include <os.h>

#ifdef CONFIG_ADC
#include <adc_atmega644p.h>

/* Hook-up ADC OS stack. */
#define ADC_TGT_INIT                adc_atmega644_init
#define ADC_TGT_CHANNEL_SELECT      adc_atmega644_channel_select
#define ADC_TGT_CHANNEL_UNSELECT    adc_atmega644_channel_unselect
#define ADC_TGT_READ                adc_atmega644_read

#endif /* CONFIG_ADC */

#endif /* _ADC_TARGET_H_ */