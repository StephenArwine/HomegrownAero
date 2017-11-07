#include <util.h>


#define PWR_MGMT_1 0x6B
#define ACCEL_XOUT_H 0xBA
#define ACCEL_XOUT_L 0xBB
#define ACCEL_YOUT_H 0XBC
#define ACCEL_YOUT_L 0XBD
#define ACCEL_ZOUT_H 0xBE
#define ACCEL_ZOUT_L 0xC0
#define GYRO_XOUT_H 0xC3
#define GYRO_XOUT_L 0xC4
#define GYRO_YOUT_H 0xC5
#define GYRO_YOUT_L 0xC6
#define GYRO_ZOUT_H 0xC7
#define GYRO_ZOUT_L 0xC8

#define XG_OFFSET_H_R 0x93
#define XG_OFFSET_L_R 0x94
#define YG_OFFSET_H_R 0x95
#define YG_OFFSET_L_R 0x96
#define ZG_OFFSET_H_R 0x97
#define ZG_OFFSET_L_R 0x98

#define XG_OFFSET_H 0x13
#define XG_OFFSET_L 0x14
#define YG_OFFSET_H 0x15
#define YG_OFFSET_L 0x16
#define ZG_OFFSET_H 0x17
#define ZG_OFFSET_L 0x18

#define GYRO_CONFIG 0X1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_CONFIG2 0x1D