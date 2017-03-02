/*
 * fs_target.h
 *
 * Copyright (c) 2017 Usama Masood <mirzaon@gmail.com>
 *
 * Standard MIT License apply on this source code, with the inclusion of below
 * clause.
 *
 * This source is for educational purpose only, and should never be used for
 * any other purpose. If this source is used for other than educational purpose
 * (in any form) the author will not be liable for any legal charges.
 */

#ifndef _FS_TARGET_H_
#define _FS_TARGET_H_

#ifdef CONFIG_FS
#include <fs_avr.h>

/* Hook-up FS OS stack. */
#define FS_TGT_INIT()           fs_avr_init()

#endif /* CONFIG_FS */

#endif /* _FS_TARGET_H_ */
