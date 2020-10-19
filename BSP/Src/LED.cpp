/**
  ******************************************************************************
  * @FileName			    LEDTask.cpp
  * @Description            LED
  * @author                 Wang Xi
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

#include "BSP_INC.h"

static uint8_t led_num = 0;

void Led_on(uint8_t id){
    HAL_GPIO_WritePin(GPIO_PORT_LED, 1<<id, GPIO_PIN_SET);
}

void Led_off(uint8_t id){
    HAL_GPIO_WritePin(GPIO_PORT_LED, 1<<id, GPIO_PIN_RESET);
}

/**
 * @author Xi Wang
 * @brief Turn on LEDs
 * @param num How many led to turn on
 * @retval
 *      @arg 1-8 The total num of LED that has been turned on
 *      @arg -1 The number of LED has reached 8 and no more available LED
 */
int8_t Led_add_n(uint8_t num){
    while(num){
        if(led_num > 7) return -1;
        Led_on(++led_num);
        num--;
    }

    return led_num;
}

/**
 * @author Xi Wang
 * @brief Turn on an LED
 * @retval
 *      @arg 1-8 The total num of LED that has been turned on
 *      @arg -1 No more available LED
 */
int8_t Led_add(){
    return Led_add_n(1);
}

/**
 * @author Xi Wang
 * @brief Turn off LEDs
 * @param num How many led to turn off
 * @retval
 *      @arg 0-7 The total num of LED that has been turned on
 *      @arg -1 The number of LED has reached 0 and no more available LED
 */
int8_t Led_delete_n(uint8_t num){
    while(num){
        if(led_num < 1) return -1;
        Led_off(led_num--);
        num--;
    }

    return led_num;
}

/**
 * @author Xi Wang
 * @brief Turn off an LED
 * @retval
 *      @arg 0-7 The total num of LED that has been turned on
 *      @arg -1 No more available LED
 */
int8_t Led_delete(){
    return Led_delete_n(1);
}

/**
 * @author Xi Wang
 * @brief Get the number of LED to a specific value
 * @param num Expected num of LED
 */
void Led_operate(uint8_t num){
    if(led_num > num) Led_delete_n(led_num - num);
    else Led_add_n(num - led_num);
}
