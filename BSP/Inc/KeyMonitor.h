/**
 ***********************************(C) COPYRIGHT 2020-2030 JiaoDragon************************************
 * @file       KeyMonitor.h
 * @brief      An auto monitor of Key values or switch values
 * @note
 * @Version    V1.0.0
 * @Date       September-5-2020
 ***********************************(C) COPYRIGHT 2020-2030 JiaoDragon************************************
 */

#ifndef __KEYMONITOR_H
#define __KEYMONITOR_H
#include <stdint.h>

#include "gpio.h"

	/***	MAP	***/

	/***	CONSTANT VALUE	***/
	#define PUSH_TIME_MAX 19		//MAXIMUM Push Time

	/***	TYPE DEFINE	***/
	typedef enum{
		KEY_RESET = 0,
		KEY_PUSH,
		KEY_LOCK,
	} E_key_state;
	
	//struct T_key_obj;
	
	struct T_key_obj{
		GPIO_PinState trig_condition;		//Basic Information
		E_key_state e_key_state;
		E_key_state e_key_last;
		
		GPIO_TypeDef* GPIO_key;		//GPIO	Information
		uint16_t GPIO_PIN_KEY;
		
		uint8_t push_time;		//Push Time Detection
		uint8_t detect_buf;
		
		E_key_state* (*State_judge_function)(struct T_key_obj* key_obj, GPIO_PinState scan_result);		//Value Judeg Function
		void (*Key_value_measurement_function)(struct T_key_obj* key_obj);
	};
	
	/***	EXTERNAL VARIABLES	***/
	extern struct T_key_obj *list_t_key[];
	extern uint8_t key_total_num;

	/***	APIs	***/
	//E_key_state* general_key_value_judge(struct T_key_obj* key_obj, GPIO_PinState scan_result);		//General Value Judeg Function
	void refresh_key_state(void);
	
	/***	SPECIFIC INIT CONFIGURATION	***/
	#define __HIGH_TRIG_INIT(_GPIO_PORT, _GPIO_PIN, _FUNC) { \
		GPIO_PIN_SET, KEY_RESET, KEY_RESET, \
		_GPIO_PORT, _GPIO_PIN, \
		0, 0, \
		&general_key_value_judge, \
		_FUNC \
	}
	
	#define __LOW_TRIG_INIT(_GPIO_PORT, _GPIO_PIN, _FUNC) { \
		GPIO_PIN_SET, KEY_RESET, KEY_RESET, \
		_GPIO_PORT, _GPIO_PIN, \
		0, 0, \
		&general_key_value_judge, \
		_FUNC \
	}
	
#endif
