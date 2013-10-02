/*
 *  dmx_controller.h
 *  Effects Controller
 *
 *  Created by Jason DiPrinzio on 9/13/08.
 *  Copyright 2008 Inspirotech Inc. All rights reserved.
 *
 */

#ifndef Scary_DMX_DMX_CONTROLLER_H
#define Scary_DMX_DMX_CONTROLLER_H

#include "utils.h"

#define CHANNEL_RESET           0


enum { 
    DMX_INIT_OK,       
    DMX_INIT_FAIL,
    DMX_INIT_NO_DEVICES,
    DMX_INIT_OPEN_FAIL,
    DMX_INIT_SET_BAUD_FAIL,
    DMX_INIT_SET_DATA_FLOW_FAIL    
};

/*initialize dmx output*/
int init_dmx();

/* stop threads */
void stop_dmx();

/*kill everything */
void destroy_dmx();

/* start threads */
void start_dmx();

/* update one channel with a new value */
void update_channel(dmx_channel_t, dmx_value_t);

void update_channels(channel_list_t channelList, dmx_value_t val);

/* update all channels at once */
void bulk_update(unsigned char*);

/* get a copy of the channel buffer for a universe */
void get_channel_buffer(dmx_value_t *buf, int offset, int num_channels);

/* get the value for a given channel */
dmx_value_t get_channel_value(dmx_channel_t ch);

#endif
