%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "show_handler.h"
 
dmx_show_t *resultShow;

void yyerror(const char *str)
{
    fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
    return 1;
} 

int parse_show_file(const char *filename, dmx_show_t **show)
{
    FILE *showFile = fopen(filename, "r");
    if(!showFile){
        return -1;
    }
    //
    extern FILE *yyin;
    yyin = showFile;
    //
    int i = init_show(&resultShow); 
    if(i) return i;
    //
    yyrestart(showFile);
    i = yyparse();
    if(i) return i;
    //
    _rewind_show(resultShow);   
    fclose(showFile);
    //
    *show = resultShow;
    //
    return 0; 
}

#ifdef _EXT_PARSER
int main(int argc, char **argv)
{
    extern FILE *yyin;
    yyin = fopen(argv[1], "r");
    //
    int i = init_show(&resultShow);
    if(i){
        fprintf(stderr, "Failed to initialize show.");
    }
    i = yyparse();
    _rewind_show(resultShow);    
    //
    FILE *outFile = fopen("./outshow.shw", "w+");
    if(!outFile){
        printf("Could not open out file for show output.");
    }
    printShow(resultShow, outFile);
    fclose(outFile);
    //
    free_show(resultShow);
    return i;
}
#endif

%} 

%union {
    int     val;
    double  dval;
    char    *text;
    struct {
        int count;
        int channels[512];
    } chan_list;
}

%type <val>         value
%type <dval>        float_value
%type <chan_list>   channel_list
%type <text>        file_spec
%type <val>         analyzer_type
%type <dval>        threshold
%type <val>         threshold_value
%type <val>         bands
%type <val>         freq
%type <val>         chan
%type <val>         low_value
%type <val>         high_value
%type <val>         speed_value
%type <val>         dmx_value
%type <val>         ontime_value
%type <val>         offtime_value

%token <val>        VALUE
%token <val>        CHANNEL
%token <chan_list>  CHANNEL_LIST
%token <dval>       FLOAT_VALUE
%token <text>       FILE_SPEC

%token CUE CHAN FLICKER OSCILLATOR ANALYZER 
%token TIMER SPEED LOW HIGH FILENAME TYPE FREQ THRESHOLD BANDS
%token THRESHOLD_VALUE DMX_VALUE ONTIME OFFTIME 
%token LPAREN RPAREN LBRACE RBRACE SEMICOLON DASH

%token <text> UNKNOWN

%%

show: 
cues
{
#ifdef _TRACE_PARSER
    printf("Show def.\n");
#endif
}
;

cues: 
cue
|
cues cue
;

cue:
CUE LBRACE RBRACE
{
#ifdef _TRACE_PARSER
    printf("Empty Cue def.\n");
#endif
}
|
CUE LBRACE settings RBRACE   
{
#ifdef _TRACE_PARSER
    printf("Cue def.\n");
#endif
//
    set_step_duration_for_current_cue(resultShow, 0);
    add_cue(resultShow);
}
|
CUE LPAREN VALUE RPAREN LBRACE settings RBRACE
{
#ifdef _TRACE_PARSER
    printf("Cue def with duration time: %d\n", $3);
#endif
//
    set_step_duration_for_current_cue(resultShow, $3);
    add_cue(resultShow);
}
;

settings: 
setting
|
settings setting
;

setting:
channel_setting 
|
flicker_setting
|
analyzer_setting
|
oscillator_setting
|
timer_setting
;

channel_setting: 
CHANNEL  value 
{
#ifdef _TRACE_PARSER
    printf("set channel %d : %d\n", $1, $2);
#endif
    set_channel_value_for_current_cue(resultShow, $1, $2);
}
;

flicker_setting:
FLICKER LBRACE CHAN value RBRACE
{
#ifdef _TRACE_PARSER
    printf("flicker setting: %d\n", $4);
#endif
    set_flicker_channel_for_current_cue(resultShow, $4);
}
;

analyzer_setting:
ANALYZER LBRACE file_spec channel_list threshold threshold_value 
bands freq analyzer_type RBRACE
{
#ifdef _TRACE_PARSER
    printf("analyzer setting: %s ,", $3);
    printf("channel value: ");
    printf(" %d channel(s)", $4.count);
    int i=0;
    for(i=0; i<$4.count; i++){
        printf(" %d, ", $4.channels[i]);
    }
    printf("freq value: %d ,", $8);
    printf("threshold: %5.3f ,", $5);
    printf("threshold value: %d ,", $6);
    printf("bands: %d\n", $7);
#endif
    analyzer_data_t *aData = NEW_ANALYZER_DATA_T(aData);
    aData->movieFile = $3;
    aData->dmxChannelList = COPY_CHANNEL_LIST(aData->dmxChannelList, $4.channels, $4.count);
    aData->threshold = $5;
    aData->dmxValue = $6;
    aData->numberOfBandLevels = $7;
    aData->frequency = $8;
    aData->flags = $9;
    set_timer_data_for_current_cue(resultShow, aData);
}
|
ANALYZER LBRACE file_spec chan threshold threshold_value 
bands freq analyzer_type RBRACE
{
#ifdef _TRACE_PARSER
    printf("analyzer setting: %s ,", $3);
    printf("channel value: %d ,", $4);
    printf("freq value: %d ,", $8);
    printf("threshold: %5.3f ,", $5);
    printf("threshold value: %d ,", $6);
    printf("bands: %d\n", $7);
#endif
    analyzer_data_t *aData = NEW_ANALYZER_DATA_T(aData);
    aData->movieFile = $3;
    int length = sizeof(int) * ( 1 + 1);
    aData->dmxChannelList = malloc(length);
    aData->dmxChannelList[1] = 0;
    aData->dmxChannelList[0] = $4;
    aData->threshold = $5;
    aData->dmxValue = $6;
    aData->numberOfBandLevels = $7;
    aData->frequency = $8;
    aData->flags = $9;
    set_timer_data_for_current_cue(resultShow, aData);
}           
;

oscillator_setting:
OSCILLATOR LBRACE chan low_value high_value speed_value RBRACE
{
#ifdef _TRACE_PARSER
    printf("Oscillator setting: ch-- %d, low-- %d, high-- %d, speed-- %d\n", $3, $4, $5, $6 );
#endif
    oscillator_data_t* oData = NEW_OSCILLATOR_DATA_T(oData);
    oData->channel = $3;
    oData->lowThreshold = $4;
    oData->highThreshold = $5;
    oData->speed = $6;
    set_oscillator_data_for_current_cue(resultShow, oData);
}
;

timer_setting:
TIMER LBRACE chan dmx_value ontime_value offtime_value RBRACE
{
#ifdef _TRACE_PARSER
    printf("Timer setting channel-- %d, on-- %d, off-- %d\n",$3, $5, $6);
#endif
    timed_effect_data_t* timer = malloc(sizeof(timed_effect_data_t));
    memset(timer, 0, sizeof(timed_effect_data_t));
    timer->channel = $3;
    timer->value = $4;
    timer->on_time = $5;
    timer->off_time = $6;
    timer->timer_handle = 0;
    setTimerDataForCurrentCue(resultShow, timer);
}
;

analyzer_type:
TYPE value
{
    $$ = $2;
}
;

freq:
FREQ value
{
    $$ = $2;
}
;

chan:           CHAN value
{
    $$ = $2;
}
;

threshold:      THRESHOLD float_value
{
    $$ = $2;
}
;

threshold_value: THRESHOLD_VALUE value
{
    $$ = $2;
}
;

bands:          BANDS value
{
    $$ = $2;
}
;

dmx_value:      DMX_VALUE value
{
    $$ = $2;
}
;

low_value:      LOW value
{
    $$ = $2;
}
;

high_value:     HIGH value
{
    $$ = $2;
}
;

speed_value:    SPEED value
{
    $$ = $2;
}
;

ontime_value:   ONTIME value
{
    $$ = $2;
}
;

offtime_value:  OFFTIME value
{
    $$ = $2;
}
;

value:          VALUE SEMICOLON
{
    $$ = $1;
}
;

float_value:    FLOAT_VALUE SEMICOLON
{   
    $$ = $1;
}
;

file_spec:      FILENAME FILE_SPEC SEMICOLON
{
    $$ = $2;
}
;

channel_list:   CHAN CHANNEL_LIST SEMICOLON
{
    $$ = $2;
}

%%

