/**
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 * @file       bsp_imu.h
 * @brief      this file contains the common defines and functions prototypes for
 *             the bsp_imu.c driver
 * @note
 * @Version    V1.0.0
 * @Date       Jan-30-2018
 ***************************************(C) COPYRIGHT 2018 DJI***************************************
 */

#ifndef __IMU_H__
#define __IMU_H__
#include "mytype.h"
#include "includes.h"
#define MPU_DELAY(x) HAL_Delay(x)
#define VAL_MIN(a, b) ((a) < (b) ? (a) : (b))
#define VAL_MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct
{
	int16_t ax;
	int16_t ay;
	int16_t az;

	int16_t mx;
	int16_t my;
	int16_t mz;

	int16_t temp;

	int16_t gx;
	int16_t gy;
	int16_t gz;

	int16_t ax_offset;
	int16_t ay_offset;
	int16_t az_offset;

	int16_t gx_offset;
	int16_t gy_offset;
	int16_t gz_offset;
} mpu_data_t;

typedef struct
{
	int16_t ax;
	int16_t ay;
	int16_t az;

	int16_t mx;
	int16_t my;
	int16_t mz;

	float temp;

	float wx; /*!< omiga, +- 2000dps => +-32768  so gx/16.384/57.3 =	rad/s */
	float wy;
	float wz;

	float vx;
	float vy;
	float vz;

	float rol;
	float pit;
	float yaw;

	uint8_t FirstEnter;
	uint16_t InitCount;
	uint8_t InitFinish;
} imu_t;

class IMU_SPI {
	private:
		uint8_t tx, rx;
		uint8_t tx_buff[14];
		uint8_t ist_buff[6];                           /* buffer to save IST8310 raw data */

	protected:
		volatile uint32_t last_update, now_update;               /* Sampling cycle count, ubit ms */
		uint8_t mpu_buff[14];                          /* buffer to save imu raw data */
		mpu_data_t mpu_data;
		imu_t imu;
		float inv_sqrt(float x);
		uint8_t mpu_write_byte(uint8_t const reg, uint8_t const data);
		uint8_t mpu_read_byte(uint8_t const reg);
		uint8_t mpu_read_bytes(uint8_t const regAddr, uint8_t* pData, uint8_t len);
		void ist_reg_write_by_mpu(uint8_t addr, uint8_t data);
		uint8_t ist_reg_read_by_mpu(uint8_t addr);
		void mpu_master_i2c_auto_read_config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num);
		uint8_t ist8310_init();
		void ist8310_get_data(uint8_t* buff);
		uint8_t mpu_set_gyro_fsr(uint8_t fsr);
		uint8_t mpu_set_accel_fsr(uint8_t fsr);

	public:
		IMU_SPI() {
			for(int i = 0; i < 14; i++) tx_buff[i] = 0xFF;
		}
		void mpu_get_data();
};

class IMU : public IMU_SPI {
	private:
		volatile float        q0;
		volatile float        q1;
		volatile float        q2;
		volatile float        q3;
		volatile float        exInt, eyInt, ezInt;                   /* error integral */
		float gx, gy, gz, ax, ay, az, mx, my, mz;
		uint8_t id;

		uint8_t mpu_device_init();
		void init_quaternion();
		void mpu_offset_call();
		void imu_ahrs_update();
		void imu_attitude_update();

	public:
        static IMU BSP_IMU;
		IMU() {
			IMU_SPI();
			q0 = 1.0f;
			q1 = 0.0f;
			q2 = 0.0f;
			q3 = 0.0f;
		}
		void Reset() {
			mpu_device_init();
			init_quaternion();
		}
		void Handle() {
			mpu_get_data();
			imu_ahrs_update();
			imu_attitude_update();
		}
		inline const imu_t& GetData() {return imu;};
};

#endif


