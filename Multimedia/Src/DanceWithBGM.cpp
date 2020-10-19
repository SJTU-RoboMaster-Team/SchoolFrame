/**
  ******************************************************************************
  * @FileName			    DanceWithBGM.cpp
  * @Description            I'm totally insane
  * @author                 Wang Xi
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include "includes.h"
//#include "Lib_dances.h"

void Dance_auto_insane(task_s* auto_play_task, void* args);

/**
 * @brief Initialize a dance task and initialize its soft timer.
 * @param timer_1ms The soft timer for dance task, defined in SoftTimer.c.
 * @param t_song Script of the motion.
 * @param task_address The pointer of the pointer to the dance task. Can be [(task_s**)0] under cases where the function is called by user or there's no need to know which task music auto-play is.
 * @return The state of the initialize procedure.
 *      @arg 0 Fail
 *      @arg 1 Succeed
 */
uint8_t Dance_init(soft_timer_s* timer_1ms, T_Dance* t_dance, task_s** task_address){
    if(t_dance->first_enter){
        Music_init(timer_1ms, t_dance->BGM, t_dance->task_address);
        t_dance->first_enter = 0;

        return soft_timer_add_task(timer_1ms, &Dance_auto_insane, (void*)(t_dance), 1, task_address);
    }	//Init soft_timer

    return 0;
}

/**
 * @brief Cancel a dance task.
 * @param auto_play_task The task pointer of dance.
 * @param t_dance Script of the motion.
 * @return none
 */
void Dance_deinit(task_s* auto_play_task, T_Dance* t_dance){
    t_dance->motion_index = 0;
    t_dance->first_enter = 1;

    //Music_deinit(*(t_dance->task_address), t_dance->BGM);
    soft_timer_delete_task(auto_play_task);
}

/**
 * @brief Dance auto-insane task to be added into soft timer.
 * @param auto_play_task The task pointer of dance auto-insane.
 * @param args The pointer to arg(s). In this situation, it should be transformed into T_dance*, representing the motion script.
 */
void Dance_auto_insane(task_s* auto_play_task, void* args){
    //static uint16_t motion_interval = 0;
    T_Dance* t_dance = (T_Dance*)args;

    if(0 == t_dance->motion_interval){ //load a new motion
        MotionFunc motion = t_dance->t_motion_script[t_dance->motion_index].motion;
        t_dance->motion_interval = t_dance->t_motion_script[t_dance->motion_index].Time;

        // Motion enforcement
        if((MotionFunc)0 == motion){

        } else {
            motion();
        }

        // Dance final action
        if(++(t_dance->motion_index) >= t_dance->motion_num){
            Dance_deinit(auto_play_task, t_dance);
            t_dance->motion_interval = 0;
        }
    }

    else{
        t_dance->motion_interval--;
    }
}
