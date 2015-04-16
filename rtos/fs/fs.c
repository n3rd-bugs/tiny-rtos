/*
 * fs.c
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

#ifdef CONFIG_FS
#include <string.h>
#include <sll.h>
#include <path.h>
#include <sleep.h>
#include <fs.h>
#include <console.h>
#include <pipe.h>

/* Global variables. */
static FS_DATA file_data;

/* Internal function prototypes. */
static uint8_t fd_do_resume(void *, void *);

/*
 * fs_init
 * This function will initialize file system layer. This function must be
 * called before using any other APIs.
 */
void fs_init()
{
    /* Clear the global file system data. */
    memset(&file_data, 0, sizeof(FS_DATA));

#ifdef CONFIG_SEMAPHORE
    /* Create a semaphore to protect global file system data. */
    semaphore_create(&file_data.lock, 1, 1, SEMAPHORE_PRIORITY);
#endif

#ifdef FS_CONSOLE
    /* Initialize console. */
    console_init();
#endif

#ifdef FS_PIPE
    /* Initialize PIPE file system. */
    pipe_init();
#endif

} /* fs_init */

/*
 * fs_register
 * @file_system: File system data to be registered.
 * This function registers a file system, this is called by lower layer to
 * register a new file system.
 */
void fs_register(FS *file_system)
{
#ifndef CONFIG_SEMAPHORE
    /* Lock the scheduler. */
    scheduler_lock();
#else
    /* Obtain the global data lock. */
    OS_ASSERT(semaphore_obtain(&file_data.lock, MAX_WAIT) != SUCCESS);
#endif

    /* Just push this file system in the list. */
    sll_push(&file_data.list, file_system, OFFSETOF(FS, next));

#ifdef CONFIG_SEMAPHORE
    /* Release the global data lock. */
    semaphore_release(&file_data.lock);
#else
    /* Enable scheduling. */
    scheduler_unlock();
#endif

} /* fs_register */

/*
 * fs_unregister
 * @file_system: File system data needed to unregistered.
 * This function will unregister a file system. Must be called by the
 * component that has registered the file system.
 * Note that the invalidating a file descriptor is needed to be maintained by
 * underlying layer.
 */
void fs_unregister(FS *file_system)
{
#ifndef CONFIG_SEMAPHORE
    /* Lock the scheduler. */
    scheduler_lock();
#else
    /* Obtain the global data lock. */
    OS_ASSERT(semaphore_obtain(&file_data.lock, MAX_WAIT) != SUCCESS);
#endif

    /* This could be a file descriptor chain, so destroy it. */
    fs_destroy_chain((FD)file_system);

    /* Just remove this file system from the list. */
    OS_ASSERT(sll_remove(&file_data.list, file_system, OFFSETOF(FS, next)) != file_system);

#ifdef CONFIG_SEMAPHORE
    /* Release the global data lock. */
    semaphore_release(&file_data.lock);
#else
    /* Enable scheduling. */
    scheduler_unlock();
#endif

} /* fs_unregister */

/*
 * fs_data_watcher_set
 * @fd: File descriptor for which data is needed to be monitored.
 * @watcher: Data watcher needed to be registered.
 * This function will add a data watcher for the given file system.
 */
void fs_data_watcher_set(FD fd, FS_DATA_WATCHER *watcher)
{
    FS *fs = (FS *)fd;

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd) != SUCCESS);

    /* Add this watcher is the watcher list. */
    sll_append(&fs->data_watcher_list, watcher, OFFSETOF(FS_DATA_WATCHER, next));

    /* Release lock for this file descriptor. */
    fd_release_lock(fd);

} /* fs_data_watcher_set */

/*
 * fs_connection_watcher_set
 * @fd: File descriptor for which connection is needed to be monitored.
 * @watcher: Connection watcher needed to be registered.
 * This function will add a connection watcher for given file system.
 */
void fs_connection_watcher_set(FD fd, FS_CONNECTION_WATCHER *watcher)
{
    FS *fs = (FS *)fd;

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd) != SUCCESS);

    /* Add this watcher is the watcher list. */
    sll_append(&fs->connection_watcher_list, watcher, OFFSETOF(FS_CONNECTION_WATCHER, next));

    /* Release lock for this file descriptor. */
    fd_release_lock(fd);

} /* fs_connection_watcher_set */

/*
 * fs_connected
 * @fd: File descriptor for which connection was established.
 * This function will be called by driver when this console is connected.
 */
void fs_connected(FD fd)
{
    FS *fs = (FS *)fd;
    FS_CONNECTION_WATCHER *watcher;

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd) != SUCCESS);

    /* Pick the first watcher data. */
    watcher = fs->connection_watcher_list.head;

    /* While we have a watcher to process. */
    while (watcher != NULL)
    {
        /* If we have a connection established watcher. */
        if (watcher->connected != NULL)
        {
            /* Call the watcher function. */
            watcher->connected(fd, watcher->data);
        }

        /* Pick the next watcher. */
        watcher = watcher->next;
    }

    /* Release lock for this file descriptor. */
    fd_release_lock(fd);

} /* console_connected */

/*
 * fs_disconnected
 * @fd: File descriptor for which connection was terminated.
 * This function will be called by driver when this console is disconnected.
 */
void fs_disconnected(FD fd)
{
    FS *fs = (FS *)fd;
    FS_CONNECTION_WATCHER *watcher;

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd) != SUCCESS);

    /* Pick the first watcher data. */
    watcher = fs->connection_watcher_list.head;

    /* While we have a watcher to process. */
    while (watcher != NULL)
    {
        /* If we have a connection terminated watcher. */
        if (watcher->disconnected != NULL)
        {
            /* Call the watcher function. */
            watcher->disconnected(fd, watcher->data);
        }

        /* Pick the next watcher. */
        watcher = watcher->next;
    }

    /* Release lock for this file descriptor. */
    fd_release_lock(fd);

} /* console_disconnected */

/*
 * fs_connect
 * @fd: File descriptor needed to be connected.
 * @fd_head: Descriptor that will be defined as a head file descriptor.
 * This function will connect a file descriptor to another file descriptor that
 * will act as chain's head.
 */
void fs_connect(FD fd, FD fd_head)
{
    FS *fs = (FS *)fd;

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd) != SUCCESS);

    /* Given file descriptor should not be a chain head. */
    OS_ASSERT(fs->flags & FS_CHAIN_HEAD);

    /* Given file descriptor should not be a part of a chain. */
    OS_ASSERT(fs->fd_chain.fd_node.head != 0);

    /* Save the list head. */
    fs->fd_chain.fd_node.head = fd_head;

    /* Release lock for this file descriptor. */
    fd_release_lock(fd);

    /* Get lock for head file descriptor. */
    OS_ASSERT(fd_get_lock(fd_head) != SUCCESS);

    /* If head file descriptor is not a chain head. */
    if (!(((FS *)fd_head)->flags & FS_CHAIN_HEAD))
    {
        /* Initialize chain head. */
        ((FS *)fd_head)->flags |= FS_CHAIN_HEAD;
    }

    /* Push this node on the list head. */
    sll_push(&((FS *)fd_head)->fd_chain.fd_list, fd, OFFSETOF(FS, fd_chain.fd_node.next));

    /* Release lock for head file descriptor. */
    fd_release_lock(fd_head);

} /* fs_connect */

/*
 * fs_destroy_chain
 * @fd_head: File descriptor chain needed to be destroyed.
 * This function will destroy a descriptor chain. All file descriptors are
 * capable of becoming a chain head so this function should be called when a
 * file descriptor is being destroyed.
 */
void fs_destroy_chain(FD fd_head)
{
    FS *fs, *fs_head = (FS *)fd_head;

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd_head) != SUCCESS);

    /* Get a file descriptor from list. */
    fs = (FS *)sll_pop(&fs_head->fd_chain.fd_list, OFFSETOF(FS, fd_chain.fd_node.next));

    /* While we have file descriptor in the chain. */
    while (fs != NULL)
    {
        /* Get lock for head file system. */
        OS_ASSERT(fd_get_lock(fs) != SUCCESS);

        /* Remove this file descriptor from the list. */
        fs->fd_chain.fd_node.head = NULL;

        /* Release lock for head file descriptor. */
        fd_release_lock(fs);

        /* Get next file descriptor. */
        fs = (FS *)sll_pop(&fs_head->fd_chain.fd_list, OFFSETOF(FS, fd_chain.fd_node.next));
    }

    /* Release lock for this file descriptor. */
    fd_release_lock(fd_head);

} /* fs_destroy_chain */

/*
 * fs_disconnect
 * @fd: File descriptor needed to be disconnected.
 * This function will disconnect a file descriptor from it's head.
 */
void fs_disconnect(FD fd)
{
    FS *fs = (FS *)fd;

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd) != SUCCESS);

    /* Given file descriptor should not be a chain head. */
    OS_ASSERT(fs->flags & FS_CHAIN_HEAD);

    /* If we are part of a file descriptor chain. */
    if (fs->fd_chain.fd_node.head != NULL)
    {
        /* Get lock for head file descriptor. */
        OS_ASSERT(fd_get_lock(fs->fd_chain.fd_node.head) != SUCCESS);

        /* We must have removed this file descriptor from it's list. */
        OS_ASSERT(sll_remove(&((FS *)fs->fd_chain.fd_node.head)->fd_chain.fd_list, fd, OFFSETOF(FS, fd_chain.fd_node.next)) != fd);

        /* Release lock for head file descriptor. */
        fd_release_lock(fs->fd_chain.fd_node.head);
    }

    /* Release lock for this file descriptor. */
    fd_release_lock(fd);

} /* fs_disconnect */

/*
 * fs_sreach_directory
 * @node: An existing file system in the list.
 * @param: Search parameter that will be updated.
 * @return: FALSE.
 * This is a search function to search a file system that should be used
 * to process a given node.
 */
uint8_t fs_sreach_directory(void *node, void *param)
{
    /* Save the required path. */
    char *path = ((DIR_PARAM *)param)->name;
    uint8_t match = FALSE;

    /* Match the file system path. */
    if (util_path_match(((FS *)node)->name, &path) == TRUE)
    {
        /* If given path was is a directory. */
        if (*path == '\\')
        {
            /* Move past the delimiter. */
            path++;
        }

        /* If this was an exact match. */
        if (*path == '\0')
        {
            /* Got an exact match. */
            match = TRUE;
        }

        /* Update the pointer until this path was matched. */
        ((DIR_PARAM *)param)->matched = path;

        /* Save this node. */
        ((DIR_PARAM *)param)->priv = node;
    }

    /* Return if we need to stop the search or need to process more. */
    return (match);

} /* fs_sreach_directory */

/*
 * fs_sreach_node
 * @node: An existing file system in the list.
 * @param: Search parameter that will be updated.
 * @return: FALSE.
 * This is a search function to search a file system that should be used
 * to process a given node.
 */
uint8_t fs_sreach_node(void *node, void *param)
{
    /* Save the required path. */
    char *path = ((NODE_PARAM *)param)->name;
    uint8_t match = FALSE;

    /* Match the file system path and this is a exact match. */
    if ( (util_path_match(((FS *)node)->name, &path) == TRUE) && (*path == '\0') )
    {
        /* Return this node. */
        ((NODE_PARAM *)param)->priv = node;

        /* Got an match. */
        match = TRUE;
    }

    /* Return if we need to stop the search or need to process more. */
    return (match);

} /* fs_sreach_node */

/*
 * fs_open
 * @name: File name to open.
 * @flags: Open flags.
 * This function opens a named node with given flags. The name should not
 * end with a '\\'.
 */
FD fs_open(char *name, uint32_t flags)
{
    DIR_PARAM param;
    FS *fs = NULL;

#ifdef CONFIG_SEMAPHORE
    /* Obtain the global data lock. */
    OS_ASSERT(semaphore_obtain(&file_data.lock, MAX_WAIT) != SUCCESS);
#endif

    /* Initialize a search parameter. */
    param.name = name;
    param.matched = NULL;
    param.priv = (void *)fs;

    /* First find a file system to which this call can be forwarded. */
    sll_search(&file_data.list, NULL, fs_sreach_directory, &param, OFFSETOF(FS, next));

    /* If a node was found. */
    if (param.priv)
    {
        /* Update the name to the resolved. */
        name = param.matched;

        /* Use this FD, we will update it if required. */
        fs = (FS *)param.priv;
    }

#ifdef CONFIG_SEMAPHORE
    /* Release the global data lock. */
    semaphore_release(&file_data.lock);
#endif

    if (fs != NULL)
    {
        /* Check if we need to call the underlying function to get a new file
         * descriptor. */
        if (fs->open != NULL)
        {
            /* Call the underlying API to get the file descriptor. */
            fs = (FD)fs->open(name, flags);
        }
    }

    /* Return the created file descriptor. */
    return ((FD)fs);

} /* fs_open */

/*
 * fs_close
 * @fd: Pointer to file descriptor.
 * This function will close a file descriptor.
 */
void fs_close(FD *fd)
{
    /* Check if a close function was registered with this descriptor. */
    if (((FS *)(*fd))->close != NULL)
    {
        /* Transfer call to underlying API. */
        ((FS *)(*fd))->close((void **)fd);
    }

    /* Clear the file descriptor. */
    *fd = (FD)NULL;

} /* fs_close */

/*
 * fd_get_lock
 * @fd: File descriptor from which lock is needed.
 * @return: A success status will be returned if file descriptor lock was
 *  successfully acquired.
 * This function will acquire lock for given file descriptor.
 */
int32_t fd_get_lock(FD fd)
{
    int32_t status = SUCCESS;
    FS *fs = (FS *)fd;

    /* If this file descriptor does have a acquire lock function. */
    if (fs->get_lock)
    {
        /* Get lock for this file descriptor. */
        status = fs->get_lock((void *)fd);
    }

    /* Return status to the caller. */
    return (status);

} /* fd_get_lock */

/*
 * fd_release_lock
 * @fd: File descriptor from which lock will be released.
 * This function will release lock for given file descriptor.
 */
void fd_release_lock(FD fd)
{
    FS *fs = (FS *)fd;

    /* If this file descriptor do have a lock release function. */
    if (fs->release_lock)
    {
        /* Release lock for this file descriptor. */
        fs->release_lock((void *)fd);
    }

} /* fd_release_lock */

/*
 * fs_read
 * @fd: File descriptor from which data is needed to be read.
 * @buffer: Buffer in which data is needed to be read.
 * @nbytes: Number of bytes that can be read in the provided buffer.
 * @return: >0 number of bytes actually read in the given buffer,
 *  FS_NODE_DELETED if file node was deleted during waiting for read,
 *  FS_READ_TIMEOUT if no data was received before the given timeout.
 * This function will read data from a file descriptor.
 */
int32_t fs_read(FD fd, char *buffer, int32_t nbytes)
{
#ifdef CONFIG_SLEEP
    uint64_t last_tick = current_system_tick();
#endif
    FS *fs = (FS *)fd;
    FS_PARAM param;
    SUSPEND suspend;
    int32_t read = 0, status = SUCCESS;
    uint32_t interrupt_level = GET_INTERRUPT_LEVEL();

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd) != SUCCESS);

    /* If lock was successfully obtained. */
    if (status == SUCCESS)
    {
        /* Check if a read function was registered with this descriptor */
        if ((fs->read != NULL))
        {
            /* Check if we need to block on read for this FS and there is no
             * data on the descriptor and we are in a task. */
            if ((!(fs->flags & FS_DATA_AVAILABLE)) &&
                (fs->flags & FS_BLOCK) &&
                (get_current_task() != NULL))
            {
#ifdef CONFIG_SLEEP
                /* Save the time out for which we might sleep. */
                suspend.timeout = fs->timeout;
#endif

                /* There is never a surety that if some data is available and
                 * picked up by a waiting task as scheduler might decide to run
                 * some other higher/same priority task and data can get consumed
                 * by it before this waiting task can get it. */
                /* This is not a bug and happen in a RTOS where different types of
                 * schedulers are present. */
                do
                {
#ifdef CONFIG_SLEEP
                    /* If we don't need to wait indefinitely. */
                    if (fs->timeout != (uint32_t)MAX_WAIT)
                    {
                        /* If called again compensate for the time we have already waited. */
                        suspend.timeout -= (uint32_t)(current_system_tick() - last_tick);

                        /* Save when we suspended last time. */
                        last_tick = current_system_tick();
                    }
#endif
                    /* Initialize file system parameter. */
                    param.flag = FS_BLOCK_READ;

                    /* Initialize suspend criteria. */
                    suspend.param = &param;
                    suspend.flags = (fs->flags & FS_PRIORITY_SORT ? CONDITION_PRIORITY : 0);

                    /* We need to suspend so disable preemption and release the lock
                     * this way releasing the lock will not pass the control to
                     * next task. */

                    /* Disable preemption. */
                    scheduler_lock();

                    /* Disable interrupts. */
                    DISABLE_INTERRUPTS();

                    /* Release lock for this file descriptor. */
                    fd_release_lock(fd);

                    /* Wait for data on this file descriptor. */
                    status = suspend_condition(&fs->condition, &suspend);

                    /* Restore old interrupt level. */
                    SET_INTERRUPT_LEVEL(interrupt_level);

                    /* Enable preemption. */
                    scheduler_unlock();

                    /* If the file node has been deleted then we should not try to get the
                     * lock. */
                    if (status != FS_NODE_DELETED)
                    {
                        /* Get lock for this file descriptor. */
                        OS_ASSERT(fd_get_lock(fd) != SUCCESS);
                    }

                    /* If an error has occurred. */
                    if (status != SUCCESS)
                    {
                        /* Break out of this loop. */
                        break;
                    }

                } while (!(fs->flags & FS_DATA_AVAILABLE));
            }

            /* Check if some data is available. */
            if ((status == SUCCESS) && (fs->flags & FS_DATA_AVAILABLE))
            {
                /* Transfer call to underlying API. */
                read = fs->read((void *)fd, buffer, nbytes);

                /* Some data is still available. */
                if (fs->flags & FS_DATA_AVAILABLE)
                {
                    /* Resume any task waiting on this file descriptor. */
                    fd_data_available(fd);
                }

                /* If there is some space on this file descriptor. */
                if (fs->flags & FS_SPACE_AVAILABLE)
                {
                    /* Resume any tasks waiting for space on this file descriptor. */
                    fd_space_available(fd);
                }
            }
        }

        /* Release lock for this file descriptor. */
        fd_release_lock(fd);
    }

    /* Return number of bytes read. */
    return (read);

} /* fs_read */

/*
 * fs_write
 * @fd: File descriptor on which data is needed to be written.
 * @buffer: Data buffer needed to be sent.
 * @nbytes: Number of bytes to write. If -1 the we will use assume that given
 *  data is a null terminated string and it's length will be calculated locally.
 * @return: Returns number of bytes written, if this descriptor is part of a
 * chain average number of bytes written will be returned.
 * This function will write data on a file descriptor.
 */
int32_t fs_write(FD fd, char *buffer, int32_t nbytes)
{
#ifdef CONFIG_SLEEP
    uint64_t last_tick = current_system_tick();
#endif
    FS *fs = (FS *)fd;
    FS_PARAM param;
    SUSPEND suspend;
    int32_t status = SUCCESS, written = 0, n_fd = 0, nbytes_fd;
    uint32_t interrupt_level = GET_INTERRUPT_LEVEL();
    char *buffer_start;
    FD next_fd = NULL;
    uint8_t is_list = FALSE;

    /* If this is a null terminated string. */
    if (nbytes == -1)
    {
        /* Compute the string length. */
        nbytes = (int32_t)strlen(buffer);
    }

    /* Save buffer data. */
    buffer_start = buffer;
    nbytes_fd = nbytes;

    do
    {
        /* Initialize loop variables. */
        nbytes = nbytes_fd;
        buffer = buffer_start;

        /* Get lock for this file descriptor. */
        status = fd_get_lock(fd);

        /* If lock was successfully obtained. */
        if (status == SUCCESS)
        {
            /* Check if a write function was registered with this descriptor. */
            if (fs->write != NULL)
            {
                /* If configured try to write on the descriptor until all the
                 * data is sent. */
                do
                {
                    /* If we are in a task. */
                    if ((get_current_task() != NULL) &&

                        /* Check if we can block on write for this FD and there
                         * is no space. */
                        ((fs->flags & FS_BLOCK) &&
                         (!(fs->flags & FS_SPACE_AVAILABLE))))
                    {
#ifdef CONFIG_SLEEP
                        /* Save the time out for which we might sleep. */
                        suspend.timeout = fs->timeout;
#endif

                        /* There is never a surety that if some space is available and
                         * consumed up by a waiting task as scheduler might decide to run
                         * some other higher/same priority task and spaced can get consumed
                         * by it before this waiting task can get it. */
                        /* This is not a bug and happen in a RTOS where different types of
                         * schedulers are present. */
                        do
                        {
#ifdef CONFIG_SLEEP
                            /* If we don't need to wait indefinitely. */
                            if (fs->timeout != (uint32_t)MAX_WAIT)
                            {
                                /* If called again compensate for the time we have already waited. */
                                suspend.timeout -= (uint32_t)(current_system_tick() - last_tick);

                                /* Save when we suspended last time. */
                                last_tick = current_system_tick();
                            }
#endif
                            /* Initialize suspend criteria. */
                            param.flag = FS_BLOCK_WRITE;

                            /* Initialize suspend criteria. */
                            suspend.param = &param;
                            suspend.flags = (fs->flags & FS_PRIORITY_SORT ? CONDITION_PRIORITY : 0);

                            /* We need to suspend so disable preemption and release the lock
                             * this way releasing the lock will not pass the control to
                             * next task. */

                            /* Disable preemption. */
                            scheduler_lock();

                            /* Disable interrupts. */
                            DISABLE_INTERRUPTS();

                            /* Release lock for this file descriptor. */
                            fd_release_lock(fd);

                            /* Wait for space on this file descriptor. */
                            status = suspend_condition(&fs->condition, &suspend);

                            /* Restore old interrupt level. */
                            SET_INTERRUPT_LEVEL(interrupt_level);

                            /* Enable preemption. */
                            scheduler_unlock();

                            /* If the file node has been deleted then we should not try to get the
                             * lock. */
                            if (status != FS_NODE_DELETED)
                            {
                                /* Get lock for this file descriptor. */
                                OS_ASSERT(fd_get_lock(fd) != SUCCESS);
                            }

                            /* If an error has occurred. */
                            if (status != SUCCESS)
                            {
                                /* Break out of this loop. */
                                break;
                            }

                        } while (!(fs->flags & FS_SPACE_AVAILABLE));
                    }

                    /* Check if some space is available. */
                    if ((status == SUCCESS) && (fs->flags & FS_SPACE_AVAILABLE))
                    {
                        /* Transfer call to underlying API. */
                        status = fs->write((void *)fd, buffer, nbytes);

                        if (status <= 0)
                        {
                            break;
                        }

                        /* Decrement number of bytes remaining. */
                        nbytes -= status;
                        buffer += status;
                        written += status;
                    }

                    /* If an error has occurred. */
                    else if (status != SUCCESS)
                    {
                        /* Break out of this loop. */
                        break;
                    }

                } while ((fs->flags & FS_FLUSH_WRITE) && (nbytes > 0));

                /* Some data is available. */
                if (fs->flags & FS_DATA_AVAILABLE)
                {
                    /* Resume any task waiting on this file descriptor. */
                    fd_data_available(fd);
                }

                /* Some space is still available. */
                if (fs->flags & FS_SPACE_AVAILABLE)
                {
                    /* Resume any tasks waiting for space on this file descriptor. */
                    fd_space_available(fd);
                }
            }

            /* If call was made on list head. */
            if (fs->flags & FS_CHAIN_HEAD)
            {
                /* We should not be in a list. */
                OS_ASSERT(is_list == TRUE);

                /* We are in a list. */
                is_list = TRUE;

                /* Pick-up the list head. */
                next_fd = fs->fd_chain.fd_list.head;
            }

            else if (is_list == TRUE)
            {
                /* Pick-up the next file descriptor. */
                next_fd = fs->fd_chain.fd_node.next;
            }

            /* Release lock for this file descriptor. */
            fd_release_lock(fd);
        }
        else
        {
            /* Unable to obtain semaphore. */
            break;
        }

        /* Check if we need to process a file descriptor in the chain. */
        if (next_fd != NULL)
        {
            /* Pick the next file descriptor. */
            fd = next_fd;

            /* Increment number of file descriptor processed. */
            n_fd++;
        }

    } while (next_fd != NULL);

    /* If we have written on more than one file descriptor. */
    if (n_fd > 1)
    {
        /* Calculate average number of bytes written. */
        written /= n_fd;
    }

    /* Return total number of bytes written. */
    return (written);

} /* fs_write */

/*
 * fs_ioctl
 * @fd: File descriptor.
 * @cmd: IOCTL command needed to be executed.
 * @param: IOCTL command parameter if any.
 * This function will execute a command on a file descriptor.
 */
int32_t fs_ioctl(FD fd, uint32_t cmd, void *param)
{
    int32_t status = SUCCESS;
    FS *fs = (FS *)fd;

    /* Get lock for this file descriptor. */
    OS_ASSERT(fd_get_lock(fd) != SUCCESS);

    /* If lock was successfully obtained. */
    if (status == SUCCESS)
    {
        /* Check if an IOCTL function was registered with this descriptor. */
        if (fs->ioctl != NULL)
        {
            /* Transfer call to underlying API. */
            status = fs->ioctl((void *)fd, cmd, param);
        }

        /* Release lock for this file descriptor. */
        fd_release_lock(fd);
    }

    /* Return status to caller. */
    return (status);

} /* fs_ioctl */

/*
 * fd_data_available
 * @fd: File descriptor on which some data is available to read.
 * This function will be called be underlying file system to tell that there
 * is some data available to be read on this file descriptor.
 */
void fd_data_available(void *fd)
{
    FS *fs = (FS *)fd;
    FS_PARAM fs_param;
    FS_DATA_WATCHER *watcher = fs->data_watcher_list.head;

    /* Set flag that some data is available. */
    fs->flags |= FS_DATA_AVAILABLE;

    /* Call the consumer, this can be called from an interrupt so locks
     * must not be used here, also if called from user space appropriate
     * locks are already acquired. */

    /* While we have a watcher to process. */
    while ( (watcher != NULL) &&

            /* While we still have some data. */
            (fs->flags & FS_DATA_AVAILABLE))
    {
        /* If we have a watcher for received data. */
        if (watcher->data_available != NULL)
        {
            /* Call the watcher function. */
            watcher->data_available(fd, watcher->data);
        }

        /* Pick the next watcher. */
        watcher = watcher->next;
    }

    /* If we still have some data available, resume any tasks waiting on it. */
    if (fs->flags & FS_DATA_AVAILABLE)
    {
        /* Initialize criteria. */
        fs_param.flag = FS_BLOCK_READ;

        /* Resume a task if any waiting on read on this file descriptor. */
        fd_handle_criteria(fd, &fs_param, TASK_RESUME);
    }

} /* fd_data_available */

/*
 * fd_data_flushed
 * @fs: File descriptor for which data has been flushed.
 * This function will clear the data available flag for a given file
 * descriptor. Caller must have the lock for FS before calling this routine.
 */
void fd_data_flushed(void *fd)
{
    FS *fs = (FS *)fd;

    /* Clear the data available flag. */
    fs->flags &= (uint32_t)~(FS_DATA_AVAILABLE);

} /* fd_data_flushed */

/*
 * fd_space_available
 * @fs: File descriptor for which there is data space is available.
 * This function will clear the FD flag that is there is some space available
 * that can now be used, and resume any tasks waiting on this.
 */
void fd_space_available(void *fd)
{
    FS *fs = (FS *)fd;
    FS_PARAM fs_param;
    FS_DATA_WATCHER *watcher = fs->data_watcher_list.head;

    /* Set flag that some data is available. */
    fs->flags |= FS_SPACE_AVAILABLE;

    /* Call the consumer, this can be called from an interrupt so locks
     * must not be used here, also if called from user space appropriate
     * locks are already acquired. */

    /* While we have a watcher to process. */
    while ( (watcher != NULL) &&

            /* While we still have some data. */
            (fs->flags & FS_SPACE_AVAILABLE))
    {
        /* If we have somebody waiting for space on this file descriptor. */
        if (watcher->space_available != NULL)
        {
            /* Call the watcher function. */
            watcher->space_available(fd, watcher->data);
        }

        /* Pick the next watcher. */
        watcher = watcher->next;
    }

    /* If there is still some space available. */
    if (fs->flags & FS_SPACE_AVAILABLE)
    {
        /* Initialize criteria. */
        fs_param.flag = FS_BLOCK_WRITE;

        /* Resume a task if any waiting on write for this file descriptor. */
        fd_handle_criteria(fd, &fs_param, TASK_RESUME);
    }

} /* fd_space_available */

/*
 * fd_space_consumed
 * @fs: File descriptor for which there is no more space.
 * This function will set the no space available flag for a given file
 * descriptor. Caller must have the lock for FS before calling this routine.
 */
void fd_space_consumed(void *fd)
{
    FS *fs = (FS *)fd;

    /* Set the flag that there is no more space in this FD. */
    fs->flags &= (uint32_t)(~FS_SPACE_AVAILABLE);

} /* fd_space_consumed */

/*
 * fd_do_resume
 * @param_resume: Parameter for which we need to resume a task.
 * @param_suspend: Parameter for which a task was suspended.
 * @return: TRUE if we need to resume this task, FALSE if we cannot resume
 *  this task.
 * This is callback to see if we can resume a task suspended on a file
 * descriptor for a given condition.
 */
static uint8_t fd_do_resume(void *param_resume, void *param_suspend)
{
    FS_PARAM *fs_resume = (FS_PARAM *)param_resume;
    FS_PARAM *fs_suspend = (FS_PARAM *)param_suspend;
    uint8_t resume = FALSE;

    /* Check if the waiting task fulfills our criteria. */
    if (fs_resume->flag & fs_suspend->flag)
    {
        /* Resume this task. */
        resume = TRUE;
    }

    /* Return if we can resume this task. */
    return (resume);

} /* fd_do_resume */

/*
 * fd_handle_criteria
 * @fd: File descriptor for which a criteria is needed to be handled.
 * @param: Criteria needed to be handled.
 * @status: Status needed to be returned to the task.
 * This function will update and handle criteria for an FD, also resumes a
 * task waiting for given criteria.
 */
void fd_handle_criteria(void *fd, FS_PARAM *param, int32_t status)
{
    FS *fs = (FS *)fd;
    RESUME resume;

    /* Initialize resume criteria. */
    resume.do_resume = &fd_do_resume;
    resume.param = param;
    resume.status = status;

    /* Resume tasks waiting for this condition. */
    resume_condition(&fs->condition, &resume);

} /* fd_handle_criteria */

/*
 * fs_memcpy_r
 * @dst: Destination buffer on which data is needed to be pushed.
 * @src: Source buffer from which data is needed to be copied.
 * @len: Number of bytes to copy.
 * This function will copy n bytes from source to the destination buffer last
 * byte first.
 */
void fs_memcpy_r(void *dst, void *src, uint32_t n)
{
    uint8_t *dst_ptr = (uint8_t *)dst;
    uint8_t *src_ptr = (uint8_t *)src;

    /* Go to the end of destination buffer. */
    dst_ptr = dst_ptr + n;

    /* While we have a byte to copy. */
    while (n --)
    {
        /* Copy a byte from source to the buffer. */
        *(--dst_ptr) = *(src_ptr++);
    }

} /* fs_memcpy_r */

#endif /* CONFIG_FS */
