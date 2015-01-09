/*
 * mem.h
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

#ifndef MEM_H
#define MEM_H

#include <config.h>

#ifdef CONFIG_MEMGR

/* Memory manager configuration. */
#define MEMGR_STATIC
#define MEMGR_DYNAMIC
#define MEMGR_STATS

#ifdef MEMGR_STATIC
#include <mem_static.h>
#endif

#ifdef MEMGR_DYNAMIC
#include <mem_dynamic.h>
#endif

#ifdef MEMGR_STATS
#include <mem_stats.h>
#endif

/* Exported variable definitions. */
#ifdef MEMGR_STATIC
extern MEM_STATIC mem_static;
#endif

#ifdef MEMGR_DYNAMIC
extern MEM_DYNAMIC mem_dynamic;
#endif

/* Function prototypes. */
void mem_init();

#ifdef MEMGR_STATIC
#define mem_static_alloc(size)      mem_static_alloc_region(&mem_static, size)
#define mem_static_dealloc(mem)     mem_static_dealloc_region((char *)mem)
#endif

#ifdef MEMGR_DYNAMIC
#define mem_dynamic_alloc(size)     mem_dynamic_alloc_region(&mem_dynamic, size)
#define mem_dynamic_dealloc(mem)    mem_dynamic_dealloc_region((char *)mem)
#endif

#endif /* CONFIG_MEMGR */

#endif /* MEM_H */