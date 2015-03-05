/*
 * os_arm_gcc.h
 *
 * Copyright (c) 2014 Usama Masood <mirzaon@gmail.com>
 *
 * Standard MIT License apply on this source code, with the inclusion of below
 * clause.
 *
 * This source is for educational purpose only, and should never be used for
 * any other purpose. If this source is used for other than educational purpose
 * (in any form) the author will not be liable for any legal charges.
 */

/* This macro is used to tell compiler to not manage stack for it. */

#ifndef OS_AVR_GCC_H
#define OS_AVR_GCC_H

#define STACK_LESS          __attribute__ (( naked ))

#define ISR_FUN             void __attribute__ ((interrupt))
#define NAKED_ISR_FUN       void __attribute__ ((interrupt, naked))
#define NAKED_FUN           void __attribute__ ((naked))

#endif /* OS_AVR_GCC_H */