#ifndef COMMANDTABLE_H
#define COMMANDTABLE_H

#define SENSOR_DISTANCE1 0X01
#define SENSOR_DISTANCE2 0X02
#define SENSOR_DISTANCE3 0X03
#define SENSOR_DISTANCE4 0X04
#define SENSOR_DISTANCE5 0X05
#define SENSOR_DISTANCE6 0X06
#define SENSOR_DISTANCE7 0X07
#define SENSOR_LINE_DEVIATION 0x08
#define SENSOR_ANGLE 0x09
#define SENSOR_LINE_LEFT 0x0A
#define SENSOR_LINE_RIGHT 0x0B
#define SENSOR_LINE_STRAIGHT 0x0C
#define SENSOR_LINE_GOAL 0x0D
#define SENSOR_LINE_STOP 0x0E
#define SENSOR_DATA_ALL 0x0F

#define CONTROL_SIGNALS 0x20
#define STEER_STRAIGHT 0x21
#define STEER_STRAIGHT_LEFT 0x22
#define STEER_STRAIGHT_RIGHT 0x23
#define STEER_BACK 0x24
#define STEER_STOP 0x25
#define STEER_ROTATE_LEFT 0x26
#define STEER_ROTATE_RIGHT 0x27
#define CALIBRATE_LINE_SENSOR 0x28
#define CLAW_OPEN 0x29
#define CLAW_CLOSE 0x2A
#define CONTROL_THROTTLE 0x2B

#define SEND_STRING 0x30
#define BT_CONNECT 0x31
#define BT_DISCONNECT 0x32

#define FLAG_AUTO 0x41
#define FLAG_MAN 0x42

#define PARA_KP_RIGHT 0x61
#define PARA_KD_RIGHT 0x62
#define PARA_KP_LEFT 0x63
#define PARA_KD_LEFT 0x64

#define ERROR_SPI 0xE0
#define ERROR_UNKNOWN_COMMAND 0xE1
#define ERROR_FATAL 0xEE

#endif // COMMANDTABLE_H
