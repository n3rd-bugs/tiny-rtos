/*
 * condition.c
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

#include <os.h>
#include <condition.h>
#include <sll.h>

#ifdef CONFIG_SLEEP
#include <sleep.h>
#endif

/* Internal function prototypes. */
static uint8_t suspend_sreach_task(void *, void *);

/*
 * suspend_sreach_task
 * @node: An task waiting on this condition.
 * @param: Resumption criteria.
 * @return: TRUE if we need to resume this task, FALSE if we cannot resume
 *  this task.
 * This is a search function to search a task that satisfy the suspension
 * criteria.
 */
static uint8_t suspend_sreach_task(void *node, void *param)
{
    RESUME *resume = (RESUME *)param;
    uint8_t match = TRUE;

    /* Check if we can resume this task. */
    match = resume->do_resume(resume->param, ((SUSPEND *)((TASK *)node)->suspend_data)->param);

    /* Return if we need to stop the search or need to process more. */
    return (match);

} /* suspend_sreach_task */

/*
 * suspend_condition
 * @condition: Condition for which we need to suspend this task.
 * @suspend: Suspend data.
 * @return: SUCCESS if criteria was successfully achieved,
 *  SUSPEND_PRIORITY will be returned if a timeout was occurred while waiting
 *  for the condition.
 * This function will suspend the caller task to wait for a criteria.
 */
int32_t suspend_condition(CONDITION *condition, SUSPEND *suspend)
{
#ifdef CONFIG_SLEEP
    uint64_t last_tick = current_system_tick();
#endif
    TASK *tcb = get_current_task();
    int32_t status = SUCCESS;

    /* Current task should not be null. */
    OS_ASSERT(tcb == NULL);

    /* There is never a surety that if a condition is satisfied for a task when
     * it is resumed, as some other high priority task may again trigger in
     * and avail that condition. */
    /* This is not a bug and happen in a RTOS where different types of
     * schedulers are present. */

    /* Check if we need to suspend on this condition. */
    while (suspend->do_suspend(condition->data, suspend->param))
    {
#ifdef CONFIG_SLEEP
        /* Check if we need to wait for a finite time. */
        if (suspend->timeout != (uint32_t)(MAX_WAIT))
        {
            /* If called again compensate for the time we have already waited. */
            suspend->timeout -= (uint32_t)(current_system_tick() - last_tick);

            /* Save when we suspended last time. */
            last_tick = current_system_tick();

            /* Add the current task to the sleep list, if not available in
             * the allowed time the task will be resumed. */
            sleep_add_to_list(tcb, suspend->timeout - current_system_tick());
        }
#endif /* CONFIG_SLEEP */

        /* If we need to sort the list on priority. */
        if (suspend->flags & CONDITION_PRIORITY)
        {
            /* Add this task on the task list. */
            sll_insert(&condition->task_list, tcb, &task_priority_sort, OFFSETOF(TASK, next));
        }

        else
        {
            /* Add this task at the end of task list. */
            sll_append(&condition->task_list, tcb, OFFSETOF(TASK, next));
        }

        /* Task is being suspended. */
        tcb->status = TASK_SUSPENDED;

        /* Assign the suspension data to the task. */
        tcb->suspend_data = (void *)suspend;

        /* If we have a pre-suspend for this condition. */
        if (condition->pre_suspend)
        {
            /* Call pre-suspend for this condition. */
            condition->pre_suspend(condition->data);
        }

        /* Wait for either being resumed by some data or timeout. */
        task_waiting();

        /* If we have a post-resume for this condition. */
        if (condition->post_resume)
        {
            /* Call post_resume for this condition. */
            condition->post_resume(condition->data, status);
        }

        /* Check if we are resumed due to a timeout. */
        if (tcb->status == TASK_RESUME_SLEEP)
        {
            /* Pick the condition for which this task is being resumed. */
            condition = tcb->suspend_data;

            /* Return an error we failed to achieve the condition. */
            status = CONDITION_TIMEOUT;

            /* Remove this task from the task list. */
            OS_ASSERT(sll_remove(&condition->task_list, tcb, OFFSETOF(TASK, next)) != tcb);

            /* Break out of the loop. */
            break;
        }

        /* If we did not resume normally. */
        else if (tcb->status != TASK_RESUME)
        {
            /* Return the error returned by the task. */
            status = tcb->status;

            /* Break out of the loop. */
            break;
        }
    }

    /* Return status to the caller. */
    return (status);

} /* suspend_condition */

/*
 * resume_condition
 * @condition: Condition for which we need to resume tasks.
 * @resume: Resume data.
 * @return: SUCCESS if criteria was successfully achieved,
 *  SUSPEND_PRIORITY will be returned if a timeout was occurred while waiting
 *  for the condition.
 * This function will suspend the caller task to wait for a criteria.
 */
void resume_condition(CONDITION *condition, RESUME *resume)
{
    TASK *tcb;

    /* Resume all the tasks waiting on this condition. */
    do
    {
        /* If a parameter was given. */
        if (resume->param != NULL)
        {
            /* Should never happen. */
            OS_ASSERT(resume->do_resume == NULL);

            /* Search for a task that can be resumed. */
            tcb = (TASK *)sll_search_pop(&condition->task_list, &suspend_sreach_task, resume, OFFSETOF(TASK, next));
        }

        else
        {
            /* Get the first task that can be executed. */
            tcb = (TASK *)sll_pop(&condition->task_list, OFFSETOF(TASK, next));
        }

        /* If we have a task to resume. */
        if (tcb)
        {
            /* Set the task status as required by resume. */
            tcb->status = resume->status;

            /* Save the condition for which this task is resuming. */
            tcb->suspend_data = condition;

#ifdef CONFIG_SLEEP
            /* Remove this task from sleeping tasks. */
            sleep_remove_from_list(tcb);
#endif /* CONFIG_SLEEP */

            /* Try to reschedule this task. */
            ((SCHEDULER *)(tcb->scheduler))->yield(tcb, YIELD_SYSTEM);

            /* Try to yield the current task. */
            task_yield();
        }
        else
        {
            /* No more tasks left break out of this loop. */
            break;
        }

    } while (tcb != NULL);

} /* resume_condition */
