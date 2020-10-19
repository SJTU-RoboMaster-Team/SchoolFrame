/**
  ******************************************************************************
  * @FileName			    CANTask.h
  * @Description            CAN communication
  * @author                 Chang Liu
  * @note
  ******************************************************************************
  *
  * Copyright (c) 2021 Team JiaoLong-ShanghaiJiaoTong University
  * All rights reserved.
  *
  ******************************************************************************
**/

#ifndef __CANTASK_H__
#define __CANTASK_H__

#include "includes.h"

#define CanRxGetU16(canRxMsg, num) (((uint16_t)canRxMsg.Data[num * 2] << 8) | (uint16_t)canRxMsg.Data[num * 2 + 1])

typedef enum {
    CAN_TYPE_1 = 1,
    CAN_TYPE_2
} CanType_e;

class CAN {
public:
    static CAN can1, can2;

    /**
     * @brief reset can communication
     */
    void Reset(CanType_e type, uint8_t motorPart1, uint8_t motorPart2, uint8_t doubleBoard);

    /**
     * @brief process received data
     */
    void RxHandle();

    /**
     * @brief data transmition handler
     */
    void TxHandle();

    /**
     * @brief get state of tx done flag
     */
    inline uint8_t GetTxDone() { return TxDone; }

    /**
     * @brief set state of tx done flag
     */
    inline void SetTxDone(uint8_t value) { TxDone = value; }

    /**
     * @brief get the can type
     */
    inline CAN_HandleTypeDef *GetHcan() { return hcan; }

private:
    /**
    * @brief transmit data to motors
    */
    void TxHandleMotor(uint8_t part);

    /**
    * @brief transmit data to another board
    */
    void TxHandleDoubleBoard();

    /**
     * @brief define CAN object is used or not
     * @param [0] first 4 motors
     * @param [1] last 4 motors
     * @param [2] doubl board
     */
    uint8_t sendData[3];
    uint8_t currentSendData;
    uint8_t everRx;
    uint8_t TxDone;
    CAN_HandleTypeDef *hcan;
    CanRxMsgTypeDef RxMsg;
    CanTxMsgTypeDef TxMsg;
};

#endif
