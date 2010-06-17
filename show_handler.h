/*
 *  showhandler.h
 *  Scary DMX
 *
 *  Created by Jason DiPrinzio on 9/21/08.
 *  Copyright 2008 Inspirotech Inc. All rights reserved.
 *
 */
#include "oscillator_effect.h"
#include "flicker_effect.h"
#include "sound_analyzer.h"
#include "timed_effect.h"
#include "dmx_controller.h"
#include "utils.h"

#include <stdio.h>

#ifndef SHOW_HANLDER_H
#define SHOW_HANDLER_H

enum {
    DMX_SHOW_OK,
    DMX_SHOW_NOT_LOADED
};

//
typedef struct _cue_t{
    int empty;
    int flickerChannel;
    timed_effect_data_t* timer;
    oscillator_data_t* oData;
    analyzer_data_t* aData;
    int stepDuration;              // this is the duration in seconds of the cue if not using the length of the movie.
    unsigned char* channelValues;  
} cue_t;
//
typedef struct _cue_node_t{
    cue_t* cue;
    struct _cue_node_t* nextCue;
    struct _cue_node_t* previousCue;
} cue_node_t;
//
typedef struct _dmx_show_t{
    char* showName;
    int   cueCount;
    struct _cue_node_t* currentCue;
} dmx_show_t;
//

/*
    Start the show sequence.
 */
int start_show();

/*
    Stop the show sequence.
 */
void end_show();

/*
    Allocate and create a cue node.
 */
int create_cue_node(cue_node_t**);

/* 
    Allocate memory and initialize a show
    with its first cue.
 */
int init_show(dmx_show_t**);

/*
    Reset the cue pointer in the show
    to point to the first cue.
 */
void _rewind_show(dmx_show_t*);
void rewind_show();

/*
    Release all the memory allocated
    for this show structure.
 */
int free_show(dmx_show_t*);

/*
    Free up any allocations related to loading shows.  
    Useful for during application shutdown.
 */
int free_all_show();

/*
    Add a cue to this show at the 
    end of the cue list.
 */
int add_cue(dmx_show_t*);

// Show creation functions
void set_channel_value_for_current_cue(dmx_show_t*, int, int);
//
void set_step_duration_for_current_cue(dmx_show_t*, int);
//
void set_flicker_channel_for_current_cue(dmx_show_t*, int);
//
void set_oscillator_data_for_current_cue(dmx_show_t*, oscillator_data_t*);
//
void set_timer_data_for_current_cue(dmx_show_t*, analyzer_data_t*);
//
void setTimerDataForCurrentCue(dmx_show_t*, timed_effect_data_t*);
//
void printShow(dmx_show_t*, FILE*);
//
// Call back registration functions
void register_show_ended(void *callRef, void(*showEnded)(void*));
//
void register_show_next_step(void *callRef, void(*show_nex_step)(void*, cue_node_t*));

#endif

