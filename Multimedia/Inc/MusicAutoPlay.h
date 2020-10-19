/**
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 * @file       MusicAutoPlay.c
 * @brief      I'm insane 
 * @note       
 * @Version    V1.0.0
 * @Date       July-13-2020      
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 */

#ifndef __MUSICAUTOPLAY_H
#define __MUSICAUTOPLAY_H
 
	#include <stdint.h>
	#include "SoftTimer.h"
	
	/***	MAP	***/

	/***	CONSTANT VALUE	***/
	//音高定义
	#define L1	262
	#define L1U	277
	#define L2	294
	#define L2U	311
	#define L3	330
	#define L4	349
	#define L4U	370
	#define L5	392
	#define L5U	415
	#define L6	440
	#define L6U	466
	#define L7	494

	#define M1	523
	#define M1U	554
	#define M2	587
	#define M2U	622
	#define M3	659
	#define M4	698
	#define M4U	740
	#define M5	784
	#define M5U	831
	#define M6	880
	#define M6U	932
	#define M7	988

	#define H1	1046
	#define H1U	1109
	#define H2	1175
	#define H2U	1245
	#define H3	1318
	#define H4	1397
	#define H4U	1480
	#define H5	1568
	#define H5U	1661
	#define H6	1760
	#define H6U	1865
	#define H7	1976

	/***	TYPE DEFINE	***/
	typedef struct {
		uint16_t Note;
		uint16_t Time;
	} T_MusicNote;
	
	typedef struct {
		T_MusicNote* t_script;
		char name[19];
		uint8_t first_enter;
		uint16_t note_num;
		uint16_t note_index;
	} T_Song;
		
	/***	EXTERNAL VARIABLES	***/
    extern soft_timer_s timer_song_1ms;
	
	/***	APIs	***/
	void Music_auto_play(task_s* auto_play_task, void* args);
	uint8_t Music_init(soft_timer_s* timer_1ms, T_Song* t_song, task_s** task_address);
    void Music_deinit(task_s* auto_play_task, T_Song* t_song);
	
	/***	SPECIFIC INIT CONFIGURATION	***/
	
#endif //__MUSICAUTOPLAY_H
