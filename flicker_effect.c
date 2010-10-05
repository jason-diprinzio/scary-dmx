/*
 *  flicker_effect.c
 *  Effects Controller
 *
 *  Created by Jason DiPrinzio on 9/18/08.
 *  Copyright 2008 Inspirotech Inc. All rights reserved.
 *
 */

#include "flicker_effect.h"
#include "dmx_controller.h"
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//
static pthread_t flicker_pt = 0;
static pthread_mutex_t flicker_mutex = PTHREAD_MUTEX_INITIALIZER;
//
static int flickering = 0;

/*
    Stop the flicker effect thread.
*/
void stop_flicker()
{
    pthread_mutex_lock(&flicker_mutex);
        flickering = 0;
    pthread_mutex_unlock(&flicker_mutex);
    if(flicker_pt)
        pthread_join(flicker_pt, NULL);
    flicker_pt = 0;
}

/*
	This thread will update channels like a chaser that 
    creates a flicker effect.
*/
void *Flicker(void *channel_number)
{
    int channel = *((int*)channel_number);
	free(channel_number);
    //Effect speed.
	useconds_t seconds = 10000;
    
	register int i=0;
	while(flickering){
        //Hardcoded sequence of values for this effect on a dimmer pack.
	    for(i=65; i<155; i+=2){ 
            update_channel(channel, i); 
            usleep(seconds);
        }
        
        if(!flickering) break;
	    
        for(i=155; i>100; i-=2){ 
            update_channel(channel, i); 
            if(!flickering) break;
            usleep(seconds);
        }

        if(!flickering) break;
	    
		for(i=100; i<125; i+=2){ 
            update_channel(channel, i); 
            if(!flickering) break;
            usleep(seconds);
        }

        if(!flickering) break;
	    
		for(i=125; i>75; i-=2){ 
            update_channel(channel, i); 
            if(!flickering) break;
            usleep(seconds);
        }

        if(!flickering) break;
	    
		for(i=75; i<125; i+=2){ 
            update_channel(channel, i); 
            if(!flickering) break;
            usleep(seconds);
        }

        if(!flickering) break;
	    
		for(i=125; i>65; i-=2){ 
            update_channel(channel, i); 
            if(!flickering) break;
            usleep(seconds);
        }
	}
    
cleanup:
    //Reset
    i=0;
	update_channel(channel, i);    
	pthread_exit(NULL);
}

/*
    Start the flicker effect thread on the specified channel.
*/
int start_flicker(int channel){

    int status = FLICKER_OK;
    pthread_mutex_lock(&flicker_mutex);
        if(flickering){
            pthread_mutex_unlock(&flicker_mutex);
            return FLICKER_IN_PROGRESS;
        }else{
            flickering = 1;
            int *lightFlickerChannel = malloc(sizeof(int));
            *lightFlickerChannel = channel;
            
            if(0 > lightFlickerChannel < DMX_CHANNELS ){
                 pthread_create(&flicker_pt, NULL, Flicker, (void *)(lightFlickerChannel));
            } else {
                status = FLICKER_BAD_CHANNEL;
                flickering = 0;
                free(lightFlickerChannel);
            }
        }
    pthread_mutex_unlock(&flicker_mutex);
    return status;
}
