/**
  ******************************************************************************
  * @FileName			    LEDTask.cpp
  * @Description            An auto monitor of Key values or switch values, along with onepush function
  * @author                 Wang Xi
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#define __KEYMONITOR_C

#include "includes.h"

/***	Key value judgment	***/
/***	Don't change unless necessary	***/

/**
 * @brief Key value judgment for general keys.
 * @param key_obj The selected key.
 * @param scan_result The state of the key's GPIO, can be HAL_GPIO_ReadPin() for general cases.
 * @return The state of the key.
 */
static E_key_state* general_key_value_judge(struct T_key_obj* key_obj, GPIO_PinState scan_result){
	if(key_obj->trig_condition == scan_result){		//If the key is trigged
		(key_obj->detect_buf)++;
		
		if(key_obj->detect_buf > 7){
			key_obj->e_key_state = KEY_PUSH;
			key_obj->detect_buf = 0;		//Reset Buffer
			
			if((key_obj->push_time) < PUSH_TIME_MAX) (key_obj->push_time)++;		//Refresh Push Time
		}
	}
	
	else{		//RESET the key
		key_obj->e_key_state = KEY_RESET;		//Reset everything
		key_obj->push_time = 0;
		key_obj->detect_buf = 0;
	}
	
	return &(key_obj->e_key_state);
}





/***	USER CODE 1 : What you'll do when you exactly know the value of all the key_obj	***/
static void key_value_measurement_function_for_motor1(struct T_key_obj* key_obj){

}

static void key_no_action(struct T_key_obj* key_obj){
	
}
/***	USER CODE 1 END 	***/





/***	USER CODE 2 : What to do after a scan loop 	***/

/**
 * @brief The actions after key scannings and processings. One possible solution is to fill the function with functions that needs multiple keys' state.
 */
void Key_final_action(){
	
}
/***	USER CODE 2 END 	***/





/***	USER CODE 3 : Define all the keys and create the index list	***/
struct T_key_obj t_key_blue = __LOW_TRIG_INIT(GPIOA, GPIO_PIN_3, &key_value_measurement_function_for_motor1);

	// An index list of all keys for automation
struct T_key_obj *list_t_key[] = {&t_key_blue,};

/***	USER CODE 3 END	***/

/***	Automatically used in callback function	***/
uint8_t key_total_num = (sizeof(list_t_key)) / (sizeof(struct T_key_obj*));





/**
 * @brief Automatic key state process.
 * @note Should be called in callback function HAL_TIM_PeriodElapse_Callback().
 */
void  Key_refresh_key_state(){
	for(int i = 0; i < key_total_num; i++){
		list_t_key[i]->State_judge_function(list_t_key[i], HAL_GPIO_ReadPin(list_t_key[i]->GPIO_key, list_t_key[i]->GPIO_PIN_KEY)); //Judge the key values
	}
	
	for(int i = 0; i < key_total_num; i++){
		list_t_key[i]->Key_value_measurement_function(list_t_key[i]);		//And reactions to follow
	}
	
	Key_final_action();
}
/***	API 1 END	***/
