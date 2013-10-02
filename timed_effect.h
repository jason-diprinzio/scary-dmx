/*
 *  timed_effect.h
 *  Scary DMX
 *
 *  Created by Jason DiPrinzio on 10/11/08.
 *  Copyright 2008 Inspirotech Inc. All rights reserved.
 *
 */

#ifndef Scary_DMX__TIMED_EFFECT_H
#define Scary_DMX__TIMED_EFFECT_H

#include "utils.h"

typedef struct timed_effect_t *timed_effect_handle;

typedef struct _timed_effect_data_t {
    channel_list_t channels;
    dmx_value_t on_value;
    dmx_value_t off_value;
    dmx_time_t on_time;
    dmx_time_t off_time;
    timed_effect_handle *timer_handle;
    struct _timed_effect_data_t *nextTimer;
} timed_effect_data_t;

#define NEW_TIMED_EFFECT(timer) \
    malloc(sizeof(timed_effect_data_t)); \
    memset(timer, 0, sizeof(timed_effect_data_t));

enum {
    TIMED_EFFECT_OK,
    TIMED_EFFECT_FAIL,
    TIMED_EFFECT_IN_PROGRESS
};

/*
 Print a timer setting to a show file.
 */
void print_timer_data(timed_effect_data_t *, FILE *);
int timed_effects_init();
int create_timed_effect_handle(timed_effect_handle **handle);
int cue_timed_effect(timed_effect_data_t *timer_data);
int start_timed_effects();
void stop_timed_effects();

#define FREE_TIMED_EFFECTS(data) \
if(data) { \
    free_timed_effects(data); \
    data = 0; \
}

void free_timed_effects(timed_effect_data_t *);

#endif
