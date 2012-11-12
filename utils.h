/*
 *  utils.h
 *  Scary DMX
 *
 *  Created by Jason DiPrinzio on 12/27/09.
 *  Copyright 2009 Inspirotech Inc. All rights reserved.
 *
 */
#ifndef UTILS_H
#define UTILS_H

#include <libkern/OSAtomic.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#ifndef SPIN_LOCK_UNLOCKED
#define SPIN_LOCK_UNLOCKED 0
#endif

/* Logging */
#ifdef _LOG

#include <libgen.h>

OSSpinLock _loglock_;

#define log_info(format, ...) \
OSSpinLockLock(&_loglock_); \
fprintf (stdout, "[INFO]  "); \
fprintf (stdout, format, ##__VA_ARGS__); \
OSSpinLockUnlock(&_loglock_);

#define log_debug(format, ...) \
OSSpinLockLock(&_loglock_); \
fprintf (stderr, "[DEBUG] [%s:%d] ", basename(__FILE__), __LINE__); \
fprintf (stderr, format, ##__VA_ARGS__); \
OSSpinLockUnlock(&_loglock_);

#define log_warn(format, ...) \
OSSpinLockLock(&_loglock_); \
fprintf (stderr, "[WARN]  " ); \
fprintf (stderr, format, ##__VA_ARGS__); \
OSSpinLockUnlock(&_loglock_);

#define log_error(format, ...) \
OSSpinLockLock(&_loglock_); \
fprintf (stderr, "[ERROR] [%s:%d] ", basename(__FILE__), __LINE__ ); \
fprintf (stderr, format, ##__VA_ARGS__); \
OSSpinLockUnlock(&_loglock_);

#else
#define log_info(format, ...)
#define log_debug(format, ...)
#define log_warn(format, ...)
#define log_error(format, ...)
#endif


/* DMX Utilities */

#define DMX_CHANNELS            513

typedef unsigned char dmx_value_t;
typedef unsigned int dmx_channel_t;

enum channel_ret {
    CHANNEL_LIST_OK,
    CHANNEL_LIST_BAD_CHANNEL
};

struct channel_list {
    dmx_channel_t *channels;
    unsigned int length;
};

typedef struct channel_list *channel_list_t;

/*
 Print a list of cue channels from a show to a file.
 */
void printCueChannels(unsigned char *, FILE *);

/*
 Print a list of channels for any effect that supports channes lists.
 */
void printChannelList(channel_list_t, FILE *);

channel_list_t new_channel_list(const unsigned int);
channel_list_t channel_list_from_data(const unsigned int, const unsigned int *);
channel_list_t copy_channel_list(const channel_list_t);
void delete_channel_list(const channel_list_t);
#define DELETE_CHANNEL_LIST(in) \
    if(in){ \
        delete_channel_list(in); \
    } \
    in = 0;

int validate_channel_list(const channel_list_t, const unsigned int);


/* Thread Utilities */
#define THREAD_FINISHED 0
static const unsigned int _THREAD_FINISHED = 11544216;

#define EXIT_THREAD() \
    pthread_exit((void*)&_THREAD_FINISHED); 

int spawn_joinable_pthread(pthread_t *thread, void*(*func)(void*), void *data);
int cancel_join_pthread(const pthread_t*);

#ifndef __APPLE__
#define PTHREAD_SETNAME(name) \
    pthread_setname_np(pthread_self(), name) 
#else
#define PTHREAD_SETNAME(name) \
    pthread_setname_np(name)
#endif

#endif

