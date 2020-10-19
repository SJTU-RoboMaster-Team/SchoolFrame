/**
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 * @file       mid_softTimers.h
 * @brief      A soft timer for task scheduling
 * @note
 * @Version    V1.1.0
 * @Date       August-29-2020
 * @author     Tang Xinyang
 ***********************************(C) COPYRIGHT 2020 JiaoDragon************************************
 */
	
#ifndef __MID_SOFTTIMER_H_INCLUDED
#define __MID_SOFTTIMER_H_INCLUDED

#include "stm32f4xx.h"

/**
 * @note 使用方法：定义一个soft_timer_s的全局变量，如：
soft_timer_s timer1; 初始化该软定时器：
soft_timer_init(&timer1, tick_interval); 在硬件定时器回调函数中调用：
soft_time_scheduler(&timer1);
 */

#ifndef MAX_TASK_AMOUNT
#define MAX_TASK_AMOUNT (32) //单个定时器最大任务数
#endif
struct task_s;

typedef void(*func_t)(struct task_s *, void *);

typedef struct task_s {
    /**
     * @brief 回调的任务函数
     */
    func_t func;
    /**
     * @brief 任务函数自定义参数的地址
     */
    void *args;
    int16_t interval; // 任务函数的调用间隔 (ms)
    int16_t delay;    // 当前距离下次调用的时间 (ms)
    uint8_t enable;    // 标志当前任务是否启用
} task_s;

typedef struct soft_timer_s {
    task_s tasks[MAX_TASK_AMOUNT];  // 任务列表
    uint16_t tick_interval;         // 该软定时器的调用间隔 (ms)
} soft_timer_s;

/**
 * @brief 初始化一个软件定时器
 * @param soft_timer 指向一个软定时器实例的指针
 * @param tick_interval 该软定时器的调用间隔 (ms)
 */
void soft_timer_init(soft_timer_s *soft_timer, uint16_t tick_interval);

/**
 * @brief 软件定时器的调度执行函数
 * @param soft_timer 指向一个软定时器实例的指针
 */
void soft_timer_schedule(soft_timer_s *soft_timer);

// 向软件定时器中添加一个任务
// 参数：
//     soft_timer: 指向一个软定时器实例的指针
//     func: 任务的工作函数
//     args: 工作函数用户自定义参数的地址
//     interval: 任务的执行间隔(ms)
//     new_task: 用于接收新任务地址的二级指针
// 返回值：
//     0: 任务添加失败，即任务列表已满
//     1: 任务添加成功
uint8_t soft_timer_add_task(soft_timer_s *soft_timer, func_t func, void *args, uint16_t interval, task_s **new_task);

// 删除一个任务
// 参数：
//     task: 指向任务的指针
void soft_timer_delete_task(task_s *task);

// 重置一个任务的执行间隔
// 参数：
//     task: 指向任务的指针
//     interval: 任务的执行间隔(ms)
void soft_timer_reset_interval(task_s *task, uint16_t interval);


// 一个软件定时器的实例声明
//extern soft_timer_s timer_tim7_1ms;

#endif
