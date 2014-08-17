#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <os.h>

/* These defines different scheduler classes. */
#define TASK_APERIODIC   0x01
#define TASK_PERIODIC    0x02
#define TASK_IDLE        0x03

/* Some task resume status. */
#define TASK_SUSPENDED          -100
#define TASK_RESUME_NORMAL      -101
#define TASK_RESUME_SLEEP       -102
#define TASK_RESUME_SEMAPHORE   -103

/* Scheduler class definition. */
typedef struct _scheduler SCHEDULER;
struct _scheduler
{
    /* This is scheduler list member. */
    SCHEDULER   *next;

    /* These are scheduler specific functions. */
    TASK        *(*get_task) (void);
    void        (*yield) (TASK *, uint8_t);

    /* Some internal members. */
    TASK_LIST    tasks;
    TASK_LIST    ready_tasks;

    /* Priority of this scheduling class. */
    uint8_t     priority;

    /* Scheduler class identifier. */
    uint8_t     class;
};

/* Scheduler list structure. */
typedef struct _scheduler_list
{
    SCHEDULER   *head;
    SCHEDULER   *tail;
} SCHEDULER_LIST;

/* Global task list. */
extern TASK_LIST sch_task_list;

/* Function prototypes. */
void scheduler_init();
TASK *scheduler_get_next_task();
void scheduler_task_add(TASK *tcb, uint8_t class, uint32_t priority, uint64_t param, uint8_t flags);

#endif /* _SCHEDULER_H_ */
