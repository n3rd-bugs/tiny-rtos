#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "os.h"

/* Some status definitions. */
#define SEMAPHORE_TIMEOUT   -700
#define SEMAPHORE_BUSY      -701

/* Semaphore type definitions. */
#define SEMAPHORE_FIFO      0x01
#define SEMAPHORE_PRIORITY  0x02

typedef struct _semaphore
{
    /* Lists of tasks waiting for this semaphore. */
    TASK_LIST   tasks;

    /* Current semaphore count. */
    uint8_t     count;

    /* Maximum semaphore count there can be. */
    uint8_t     max_count;

    /* Type of this semaphore. */
    uint8_t     type;

} SEMAPHORE;

/* Function prototypes. */
void semaphore_create(SEMAPHORE *semaphore, uint8_t count, uint8_t max_count, uint8_t type);
uint32_t semaphore_obtain(SEMAPHORE *semaphore, uint32_t wait);
void semaphore_release(SEMAPHORE *semaphore);

#endif /* _SEMAPHORE_H_ */
