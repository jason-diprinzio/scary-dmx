//
//  DMXOutputDisplayController.m
//  Scary DMX
//
//  Created by Jason Diprinzio on 11/14/11.
//  Copyright 2011 Inspirotech Consulting, Inc. All rights reserved.
//

#import "DMXOutputDisplayController.h"
#include "dmx_controller.h"


@implementation DMXOutputDisplayController


- (void)awakeFromNib
{
    [analyzer start];
    timer = [NSTimer scheduledTimerWithTimeInterval:0.01 target:self selector:@selector(updateBuffer) userInfo:nil repeats:YES];
    [timer retain];
}

- (void)updateBuffer
{
    dmx_value_t vdmx[16];
    get_channel_buffer(vdmx, 0, 16);
    int i=0;
    for(i=0; i<16; i++){
        buffer[i] =  (float)(vdmx[i]/255.0f);
    }
    [analyzer update:16 :buffer];   
}

- (void)windowWillClose:(NSNotification *)notification
{
    [timer invalidate];
}

-(void)windowDidResize:(NSNotification *)notification
{
    NSRect rect = [[window contentView] bounds];
    [analyzer setFrame:rect];
    [analyzer drawRect:rect];
}

-(void) dealloc
{
    [super dealloc];
    [timer release];
}

@end