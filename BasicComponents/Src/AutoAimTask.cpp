/**
  ******************************************************************************
  * @FileName			    AutoAimTask.cpp
  * @Description            Commmunicate with MiniPC and control the gimbal
  *                         and shoot automatically
  * @author                 Steve Young
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#include <AutoAimTask.h>

#include "includes.h"

void AutoAimRecv::RecvInit() {
    if (HAL_UART_Receive_DMA(&AUTOAIM_UART, &recvTemp, 1) != HAL_OK) {
        Error_Handler();
    }
}

void AutoAimRecv::UartRxCpltCallback() {
    if (recvStart) {
        if (bufferCnt == length - 1 && recvTemp == 'e') {
            LRC_Check = 0;
            for (int i = 1; i <= length - 3; i++) {
                LRC_Check += buffer[i];
            }
            if (LRC_Check == buffer[length - 2]) {
                receivingCnt = 20;
                //find_enemy = 1;
                finish = 0;
                receiveCnt++;
                data.yaw = *(float *) (buffer + 1);
                data.pitch = *(float *) (buffer + 5);
                data.yaw_v = *(float *) (buffer + 9);
                data.pitch_v = *(float *) (buffer + 13);
                if (isnan(data.yaw) || isnan(data.pitch)) {
                    goto end;
                }
                data.flag = buffer[length - 3];
            }
            bufferCnt = 0;
            recvStart = 0;
        } else if (bufferCnt > length) {
            bufferCnt = 0;
            recvStart = 0;
        } else {
            buffer[bufferCnt] = recvTemp;
            bufferCnt++;
        }
    } else {
        if (recvTemp == 's') {
            recvStart = 1;
            bufferCnt = 0;
            buffer[0] = recvTemp;
            bufferCnt++;
        }
    }
    end:
    if (HAL_UART_Receive_DMA(&AUTOAIM_UART, &recvTemp, 1) != HAL_OK) Error_Handler();
}

AutoAim::AutoAim() {
    autoShoot = 0;
    mode = manual;
    gimbalFreeze = 0;
    chassisFreeze = 0;
    V_Predict = true;
    K_V_Predict = 1;
    adjust.pitch = -1;
    adjust.yaw = -0.5;
}

void AutoAim::Reset() {
    RecvInit();
}

/*
uint8_t AutoAim::JudgeBuffModeAccTime() {
	uint8_t buff_mode = 0;
	if(RefereeData.GameStatus.stage_remain_time < SMALL_BUFF_TIME 
		&& RefereeData.GameStatus.stage_remain_time > BIG_BUFF_TIME) {
		buff_mode = SMALL_BUFF_MODE;
	}
	else if(RefereeData.GameStatus.stage_remain_time < BIG_BUFF_TIME) {
		buff_mode = BIG_BUFF_MODE;
	}
	return buff_mode;
}
*/

void AutoAim::DataProcess() {
    if (!finish) {
        output.yaw = data.yaw + adjust.yaw;
        output.pitch = data.pitch + adjust.pitch;
        //autoShoot = (data.flag & 0x01) == 0x01;
        //mode = ((data.flag & 0x02) == 0x02) ? energy : predict;
        gimbalFreeze = (data.flag & 0x04) == 0x04;
        chassisFreeze = (data.flag & 0x08) == 0x08;

        worldYaw = Gimbal::gimbal.GMRealAngle(GMYaw) + output.yaw;
        interpolation = 0;
        interpolationCnt = 0;
        //chassis_lock = chassisFreeze;
        //aim_output.yaw = PID_PROCESS(&AutoAim_Yaw_pid, 0, aim.yaw);
        //aim_output.pitch = PID_PROCESS(&AutoAim_Pitch_pid, 0, aim.pitch);// * 0.1f + adjust.pitch
    } else if (interpolationCnt < AIM_INTERPOLATION_CNT) {
        interpolationCnt++;
        output.yaw =
                worldYaw - data.yaw_v * (float) interpolationCnt / 1000.f - Gimbal::gimbal.GMRealAngle(GMYaw);
        autoShoot = (output.yaw < 0.25f) && (output.pitch < 1.5f);
        interpolation = 1;
    } else {

    }
    MINMAX(output.yaw, -25.0f, 25.0f);
    MINMAX(output.pitch, -20.0f, 20.0f);
}

void AutoAim::Control() {
    //chassis_lock = (aim_mode == BUFF_MODE) ? 1 : 0;
    if (!finish && mode != manual) {
        Gimbal::gimbal.AutoAimSetPosition(output.pitch, output.yaw);
        finish = true;
    }
}

void AutoAim::UartTxInfo() {
    sendData.start = 's';
    sendData.bulletSpeed = RefereeData.GameRobotState.shooter_heat0_speed_limit - 1;
    sendData.enemyColor = RefereeData.GameRobotState.robot_id < 10 ? 0 : 1;
    //sendData.energy_mode = JudgeBuffModeAccTime();
    sendData.program_mode = 0;
    sendData.crc = 0;
    for (uint8_t *ptr = (uint8_t *) &sendData.bulletSpeed; ptr < (uint8_t *) &sendData.crc; ptr++) {
        sendData.crc += *ptr;
    }
    sendData.end = 'e';
//    freq_div(if (HAL_UART_Transmit_IT(&AUTOAIM_UART, (uint8_t *) &sendData, sizeof(sendData)) !=
//                 HAL_OK) { Error_Handler(); }, 100);
}

void AutoAim::Handle() {
    DataProcess();
    Control();
    UartTxInfo();
}

void AutoAim::AutoShoot() const {
    if (autoShoot) {
        Shoot::shoot.ShootOneBullet(0);
    }
}

AutoAim AutoAim::autoAim;
