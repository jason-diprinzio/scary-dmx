/*
 *  dmx_controller.h
 *  Effects Controller
 *
 *  Created by Jason DiPrinzio on 9/13/08.
 *  Copyright 2008 Inspirotech Inc. All rights reserved.
 *
 */

#define CHANNEL_RESET           0
#define DMX_CHANNELS            513
 
enum { 
    DMX_INIT_OK,       
    DMX_INIT_FAIL,
    DMX_INIT_NO_DEVICES,
    DMX_INIT_OPEN_FAIL,
    DMX_INIT_SET_BAUD_FAIL,
    DMX_INIT_SET_DATA_FLOW_FAIL    
};

//initialize dmx output
int init_dmx();

//stop threads
void stop_dmx();

//kill everything
void destroy_dmx();

//start threads;
void start_dmx();

//update one channel with a new value
void update_channel(int, short);

//update all channels at once
void bulk_update(unsigned char*);
