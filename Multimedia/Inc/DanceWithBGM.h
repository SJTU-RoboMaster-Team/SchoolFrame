//
// Created by admin on 2020/9/4.
//

/**
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 * @file       DanceWithBGM.h
 * @brief      I'm totally insane
 * @note
 * @Version    V1.0.0
 * @Date       September-4-2020
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 */

#ifndef __DANCEWITHBGM_H
#define __DANCEWITHBGM_H

#include <stdint.h>
#include "SoftTimer.h"
#include "MusicAutoPlay.h"

    /***	MAP	***/

    /***	CONSTANT VALUE	***/

    /***	TYPE DEFINE	***/

    typedef void(*MotionFunc)(void);

    typedef struct {
        MotionFunc motion;
        uint16_t Time;
    } T_Motion;

    typedef struct {
        T_Motion* t_motion_script;
        uint8_t first_enter;
        uint16_t motion_num;
        uint16_t motion_index;
        T_Song* BGM;
        task_s** task_address;
		uint16_t motion_interval;
    } T_Dance;

    /***	EXTERNAL VARIABLES	***/

    /***	APIs	***/
	void Dance_auto_insane(task_s* auto_play_task, void* args);
	uint8_t Dance_init(soft_timer_s* timer_1ms, T_Dance* t_dance, task_s** task_address);
	void Dance_deinit(task_s* auto_play_task, T_Dance* t_dance);
	
#endif //__DANCEWITHBGM_H
