//
// Created by admin on 2020/9/4.
//

#ifndef WINDMILL_LIB_DANCES_H
#define WINDMILL_LIB_DANCES_H

#include "DanceWithBGM.h"
#include "Lib_songs.h"

void Dance_chassis_left(void);
void Dance_chassis_right(void);
void Dance_chassis_forward(void);
void Dance_chassis_backward(void);
void Dance_chassis_clockwise(void);
void Dance_chassis_counterclockwise(void);
void Dance_chassis_stop(void);
void Dance_next_song(void);

/*** SCRIPT ***/
inline constexpr T_Motion Astronomia_m[] = {
        {(MotionFunc)0, 4800},
        {&Dance_chassis_left, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_right, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_left, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_right, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_forward, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_backward, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_forward, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_backward, 200}, {&Dance_chassis_stop, 400},

		{&Dance_chassis_right, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_left, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_right, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_left, 200}, {&Dance_chassis_stop, 400},
		{&Dance_chassis_backward, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_forward, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_backward, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_forward, 200}, {&Dance_chassis_stop, 400},

        {&Dance_chassis_left, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_right, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_forward, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_backward, 200}, {&Dance_chassis_stop, 400},
        {&Dance_chassis_clockwise, 200}, {&Dance_chassis_stop, 7200},

		//{&Dance_chassis_stop, 5000},
};

inline constexpr T_Motion Happybirthday_m[] = {
        {&Dance_chassis_clockwise, 300}, {&Dance_chassis_stop, 300},
		{&Dance_chassis_clockwise, 300}, {&Dance_chassis_stop, 300},
		{&Dance_chassis_clockwise, 300}, {&Dance_chassis_stop, 300},
		{&Dance_chassis_clockwise, 300}, {&Dance_chassis_stop, 300},
		{&Dance_chassis_clockwise, 300}, {&Dance_chassis_stop, 300},
		{&Dance_chassis_clockwise, 300}, {&Dance_chassis_stop, 300}, {&Dance_chassis_stop, 300},

        {&Dance_chassis_counterclockwise, 3600}, {&Dance_chassis_stop, 300},

        {&Dance_chassis_clockwise, 1500}, {&Dance_chassis_stop, 150},
        {&Dance_chassis_counterclockwise, 1500}, {&Dance_chassis_stop, 150},

        {&Dance_chassis_left, 3600}, {&Dance_chassis_stop, 300},
        {&Dance_chassis_right, 3600}, {&Dance_chassis_stop, 300},

        {&Dance_chassis_left, 2100}, {&Dance_chassis_stop, 150},
        {&Dance_chassis_right, 2100}, {&Dance_chassis_stop, 150},
};

inline constexpr T_Motion Senbonzakura_m[] = {
        {&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},				
        {&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_clockwise, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_counterclockwise, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_counterclockwise, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_clockwise, 266}, {&Dance_chassis_stop, 534},		
				
		{&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_clockwise, 133},
		{&Dance_chassis_clockwise, 133}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_counterclockwise, 133},
		{&Dance_chassis_counterclockwise, 133}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_counterclockwise, 133},
		{&Dance_chassis_counterclockwise, 133}, {&Dance_chassis_stop, 534},
    	{&Dance_chassis_clockwise, 133},
		{&Dance_chassis_clockwise, 133}, {&Dance_chassis_stop, 534},
				
        {&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},
	    {&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_clockwise, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_counterclockwise, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_counterclockwise, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_clockwise, 266}, {&Dance_chassis_stop, 534},		
				
        {&Dance_chassis_left, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_right, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_forward, 266}, {&Dance_chassis_stop, 534},
        {&Dance_chassis_backward, 266}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_clockwise, 133},
		{&Dance_chassis_clockwise, 133}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_counterclockwise, 133},
		{&Dance_chassis_counterclockwise, 133}, {&Dance_chassis_stop, 534},
		{&Dance_chassis_counterclockwise, 133},
		{&Dance_chassis_counterclockwise, 133}, {&Dance_chassis_stop, 534},
    	{&Dance_chassis_clockwise, 133},
		{&Dance_chassis_clockwise, 133}, {&Dance_chassis_stop, 534},
		//{(MotionFunc)0, 5000},{&Dance_next_song, 1}, 
	};

inline constexpr T_Dance Astronomia_dance = {Astronomia_m, 1, sizeof(Astronomia_m)/sizeof(T_Motion), 0, &Astronomia, (task_s**)0, 0};
inline constexpr T_Dance Happybirthday_dance = {Happybirthday_m, 1, sizeof(Happybirthday_m)/sizeof(T_Motion), 0, &Happybirthday, (task_s**)0, 0};
inline constexpr T_Dance Senbonzakura_dance = {Senbonzakura_m, 1, sizeof(Senbonzakura_m)/sizeof(T_Motion), 0, &Senbonzakura, (task_s**)0, 0};

#endif //WINDMILL_LIB_DANCES_H
