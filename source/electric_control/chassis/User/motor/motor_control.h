#ifndef __MOTOR_CONTORL_H
#define __MOTOR_CONTORL_H

#include <stdint.h>
#include "main.h" // stm32 hal
#include "ris_protocol.h"
#pragma pack(1)
typedef union
{
    int32_t     L;
    uint8_t     u8[4];
    uint16_t    u16[2];
    uint32_t    u32;
    float       F;
} COMData32;

typedef struct
{
    unsigned char start[2]; 
    unsigned char motorID; 
    unsigned char reserved;
} COMHead;

typedef struct
{
    uint8_t mode;       
    uint8_t ReadBit;  
    int8_t Temp;        
    uint8_t MError;    

    COMData32 Read;    
    int16_t T;        

    int16_t W;         
    float LW;          
    int16_t W2;         
    float LW2;          

    int16_t Acc;        
    int16_t OutAcc;    
    int32_t Pos;        
    int32_t Pos2;      

    int16_t gyro[3];    
    int16_t acc[3];

    int16_t Fgyro[3];
    int16_t Facc[3];
    int16_t Fmag[3];
    uint8_t Ftemp;      

    int16_t Force16;   
    int8_t Force8;     

    uint8_t FError;    

    int8_t Res[1];      

} ServoComdV3; 

typedef struct
{
    uint8_t head[2];    
    RIS_Mode_t mode;  
    RIS_Fbk_t   fbk;    
    uint16_t  CRC16;    
} MotorData_t; 

typedef struct
{
    uint8_t none[8];            

} LowHzMotorCmd;

typedef struct
{                                                      
    uint8_t mode;               
    uint8_t ModifyBit;          
    uint8_t ReadBit;            
    uint8_t reserved;

    COMData32 Modify;           

    // K_P*delta_Pos + K_W*delta_W + T
    int16_t T;                 
    int16_t W;                 
    int32_t Pos;               

    int16_t K_P;                
    int16_t K_W;               

    uint8_t LowHzMotorCmdIndex; 
    uint8_t LowHzMotorCmdByte; 

    COMData32 Res[1];          

} MasterComdV3; 

typedef struct
{
    uint8_t head[2];    
    RIS_Mode_t mode;   
    RIS_Comd_t comd;   
    uint16_t   CRC16;   
} ControlData_t;    

#pragma pack()

typedef struct
{
    ControlData_t motor_send_data; 
    int hex_len;                      
    long long send_time;               
    
    unsigned short id;                 
    unsigned short mode;                

    // K_P*delta_Pos + K_W*delta_W + T
    float T;                            
    float W;                            
    float Pos;                        
    float K_P;                         
    float K_W;                        
    COMData32 Res;                   
} MOTOR_send;

typedef struct
{
    
    MotorData_t motor_recv_data;   
    int hex_len;                       
    long long resv_time;               
    int correct;                      
    
    unsigned char motor_id;             
    unsigned char mode;                
    int Temp;                        
    unsigned char MError;          
    float T;                            
		float W;						
    float Pos;                         
	float footForce;											

} MOTOR_recv;

#define SET_485_DE_UP() HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET)
#define SET_485_DE_DOWN() HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET)

#define SET_485_RE_UP() HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET)
#define SET_485_RE_DOWN() HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_RESET)

uint32_t crc32_core(uint32_t *ptr, uint32_t len);
int modify_data(MOTOR_send *motor_s);
int extract_data(MOTOR_recv *motor_r);
HAL_StatusTypeDef SERVO_Send_recv(MOTOR_send *pData, MOTOR_recv *rData);

#endif
