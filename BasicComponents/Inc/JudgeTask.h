/**
  ******************************************************************************
  * @FileName			    JudgeTask.h
  * @Description            Receive the data from the Judge and transmmit
  * @author                 Yunhao Wang
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#ifndef __JUDGETASK_H
#define __JUDGETASK_H

#include "includes.h"

typedef enum
{
	ONLINE = 0,
	OFFLINE = 1,
}JudgeState_e;

//比赛状态数据：0x0001。发送频率：1Hz
typedef struct
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
} __packed ext_game_status_t;

//比赛结果数据：0x0002。发送频率：比赛结束后发送（不需要使用）
typedef struct
{
	uint8_t winner;
} __packed ext_game_result_t;

//机器人存活数据：0x0003。发送频率：1Hz
typedef struct
{
 uint16_t red_1_robot_HP;
 uint16_t red_2_robot_HP;
 uint16_t red_3_robot_HP;
 uint16_t red_4_robot_HP;
 uint16_t red_5_robot_HP;
 uint16_t red_7_robot_HP;
 uint16_t red_outpost_HP;
 uint16_t red_base_HP;
 uint16_t blue_1_robot_HP;
 uint16_t blue_2_robot_HP;
 uint16_t blue_3_robot_HP;
 uint16_t blue_4_robot_HP;
 uint16_t blue_5_robot_HP;
 uint16_t blue_7_robot_HP;
 uint16_t blue_outpost_HP;
 uint16_t blue_base_HP;
} __packed ext_game_robot_HP_t;

//新增：飞镖发射状态：0x0004。发送频率：飞镖发射后发送
typedef struct
{
  uint8_t dart_belong;
  uint16_t stage_remaining_time;
} __packed ext_dart_status_t;

//人工智能挑战赛加成与惩罚区状态：0x0005。发送频率：1Hz 周期发送 用不到
typedef struct
{
 uint8_t F1_zone_status:1;
 uint8_t F1_zone_buff_debuff_status:3;
 uint8_t F2_zone_status:1;
 uint8_t F2_zone_buff_debuff_status:3;
 uint8_t F3_zone_status:1;
 uint8_t F3_zone_buff_debuff_status:3;
 uint8_t F4_zone_status:1;
 uint8_t F4_zone_buff_debuff_status:3;
 uint8_t F5_zone_status:1;
 uint8_t F5_zone_buff_debuff_status:3;
 uint8_t F6_zone_status:1;
 uint8_t F6_zone_buff_debuff_status:3;
} __packed ext_ICRA_buff_debuff_zone_status_t;


//场地事件数据：0x0101。发送频率：事件改变后发送
typedef struct
{
	uint32_t event_type;
} __packed ext_event_data_t;

//补给站动作标识：0x0102。发送频率：动作改变后发送（不需要使用）
typedef struct
{
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
} __packed ext_supply_projectile_action_t;

//补给站预约子弹：cmd_id (0x0103)。发送频率：上限10Hz。RM对抗赛尚未开放
typedef struct
{
	uint8_t supply_projectile_id;
	uint8_t supply_num;
} __packed ext_supply_projectile_booking_t;

//裁判警告信息：cmd_id (0x0104)。发送频率：警告发生后发送
typedef struct
{
	uint8_t level;
	uint8_t foul_robot_id;
} __packed ext_referee_warning_t;

//新增：飞镖发射口倒计时：cmd_id (0x0105)。发送频率：1Hz 周期发送
typedef struct
{
  uint8_t dart_remaining_time;
} __packed ext_dart_remaining_time_t;


//比赛机器人状态：0x0201。发送频率：10Hz
typedef struct
{
 uint8_t robot_id;
 uint8_t robot_level;
 uint16_t remain_HP;
 uint16_t max_HP;
 uint16_t shooter_heat0_cooling_rate;
 uint16_t shooter_heat0_cooling_limit;
 uint16_t shooter_heat1_cooling_rate;
 uint16_t shooter_heat1_cooling_limit;
 uint8_t shooter_heat0_speed_limit;
 uint8_t shooter_heat1_speed_limit;
 uint8_t max_chassis_power;
 uint8_t mains_power_gimbal_output : 1;
 uint8_t mains_power_chassis_output : 1;
 uint8_t mains_power_shooter_output : 1;
} __packed ext_game_robot_status_t;

//实时功率热量数据：0x0202。发送频率：50Hz
typedef struct
{
 uint16_t chassis_volt;
 uint16_t chassis_current;
 float chassis_power;
 uint16_t chassis_power_buffer;
 uint16_t shooter_heat0;
 uint16_t shooter_heat1;
 uint16_t mobile_shooter_heat2;
} __packed ext_power_heat_data_t;

//机器人位置：0x0203。发送频率：10Hz
typedef struct
{
	float x;
	float y;
	float z;
	float yaw;
} __packed ext_game_robot_pos_t;

//机器人增益：0x0204。发送频率：状态改变后发送
typedef struct
{
	uint8_t power_rune_buff;
} __packed ext_buff_t;

//空中机器人能量状态：0x0205。发送频率：10Hz
typedef struct
{
	uint8_t energy_point;
	uint8_t attack_time;
} __packed ext_aerial_robot_energy_t;

//伤害状态：0x0206。发送频率：伤害发生后发送
typedef struct
{
	uint8_t armor_id : 4;
	uint8_t hurt_type : 4;
} __packed ext_robot_hurt_t;

//实时射击信息：0x0207。发送频率：射击后发送
typedef struct
{
	uint8_t bullet_type;
	uint8_t bullet_freq;
	float bulletSpeed;
} __packed ext_shoot_data_t;

//弹丸剩余发射数：0x0208。发送频率：1Hz周期发送，空中机器人以及哨兵机器人主控发送
typedef struct
{
	uint16_t bullet_remaining_num;
} __packed ext_bullet_remaining_t;

//新增：机器人 RFID 状态：0x0209。发送频率：1Hz
typedef struct
{
 uint32_t rfid_status;
} __packed ext_rfid_status_t;

//新增：飞镖机器人客户端指令数据：0x020A。发送频率：10Hz
typedef struct
{
 uint8_t dart_launch_opening_status;
 uint8_t dart_attack_target;
 uint16_t target_change_time;
 uint8_t first_dart_speed;
 uint8_t second_dart_speed;
 uint8_t third_dart_speed;
 uint8_t fourth_dart_speed;
 uint16_t last_dart_launch_time;
 uint16_t operate_launch_cmd_time;
} __packed ext_dart_client_cmd_t;

//交互数据接收信息：0x0301。发送频率：上限 10Hz
typedef struct
{
	uint16_t data_cmd_id;
	uint16_t send_ID;
	uint16_t receiver_ID;
}__packed ext_student_interactive_header_data_t;

//客户端自定义数据：cmd_id:0x0301。内容 ID:0xD180。发送频率：上限 10Hz  2020年已废弃
typedef struct
{
	float data1;
	float data2;
	float data3;
	uint8_t masks;
} __packed client_custom_data_t;

//学生机器人间通信 cmd_id 0x0301，内容ID:0x0200~0x02FF
typedef struct
{
	uint8_t* data;
} __packed Robot_interactive_data_t;

//学生机器人间通信（敌方机器人坐标） cmd_id 0x0301，内容ID:0x0200~0x02FF
typedef struct
{
	uint16_t x1,y1,x2,y2,x3,y3,x4,y4,x5,y5;  //x1,y1 英雄 x2,y2 工程 x3,y3 步兵1 x4,y4 步兵2 x5,y5 步兵3？
} __packed Robot_Warps_data_t;

//客户端自定义图形（删除图层） 机器人间通信：0x0301。内容 ID:0x0100。发送频率：上限10Hz
typedef struct
{
 uint8_t operate_type;
 uint8_t layer;
} __packed ext_client_custom_graphic_delete_t;

//客户端自定义图形（图形数据） 机器人间通信：0x0301。内容 ID:0x0100。发送频率：上限10Hz
typedef struct
{
 uint8_t graphic_name[3];
 uint32_t operate_type:3;
 uint32_t graphic_type:3;
 uint32_t layer:4;
 uint32_t color:4;
 uint32_t start_angle:9;
 uint32_t end_angle:9;
 uint32_t width:10;
 uint32_t start_x:11;
 uint32_t start_y:11;
 uint32_t radius:10;
 uint32_t end_x:11;
 uint32_t end_y:11;
} __packed graphic_data_struct_t;

typedef struct {
   uint32_t radius:10;
   uint32_t end_x:11;
   uint32_t end_y:11;
} Pack;//输出float变量用的中转结构

//绘制一个图形
typedef struct
{
 graphic_data_struct_t grapic_data_struct;
} __packed ext_client_custom_graphic_single_t;

//绘制两个图形
typedef struct
{
 graphic_data_struct_t grapic_data_struct[2];
} __packed ext_client_custom_graphic_double_t;

//绘制五个图形
typedef struct
{
graphic_data_struct_t grapic_data_struct[5];
} __packed ext_client_custom_graphic_five_t;

//绘制七个图形
typedef struct
{
graphic_data_struct_t grapic_data_struct[7];
} __packed ext_client_custom_graphic_seven_t;

//绘制字符
extern char Referee_Transmit_Char[30];
typedef struct
{
graphic_data_struct_t grapic_data_struct;
uint8_t data[30];
} __packed ext_client_custom_character_t;


typedef struct
{
	float power_limit;
	float speed_limit0;
	float speed_limit1;
} __packed robot_status_t;

typedef struct
{
	ext_game_status_t GameStatus;
	ext_game_robot_HP_t RobotHP;
	ext_dart_status_t DartStatus;
	ext_event_data_t EventData;
	ext_referee_warning_t refereeWarning;
	ext_dart_remaining_time_t DartRemainingTime;
	ext_game_robot_status_t GameRobotState;
	ext_power_heat_data_t PowerHeat;
	ext_game_robot_pos_t RobotPos;
	ext_buff_t Buff;
	ext_aerial_robot_energy_t AreialRobotEnergy;
	ext_robot_hurt_t RobotHurt;
	ext_shoot_data_t ShootData;
	ext_bullet_remaining_t bulletRemaining;
	ext_rfid_status_t RfidStatus;
	ext_dart_client_cmd_t DartCmd;
} __packed referee_data_t;

#define jointbyte(x,y) ((uint16_t)(x << 8) | y)//连接2个byte

extern referee_data_t RefereeData;
extern JudgeState_e JUDGE_State;
extern robot_status_t cur_robot_status;
extern int8_t client_graphic_steps;
extern int32_t lockCounter;

void sendNumber(uint8_t);
void dealWithVisualResult();
void initRecvUart();
void JudgeUartRxCpltCallback();
void InitJudgeUart();
void RecvRxCpltCallback();
void getJudgeState();
void Referee_Transmit();
void fakeHeatCalc();
void Client_Graph_Start();
void Client_Graph_Clear();
#endif /*__ JUDGETASK_H */
