#ifndef __IIC_H__
#define __IIC_H__
//
void iic_start();
void iic_init();
void iic_stop();
void iic_response();
void iic_write_byte(unsigned char  dat);
unsigned char iic_read_byte();

void iic_write_address(unsigned char SlaveAddress,address,dat);
unsigned char iic_read_address(unsigned char writeAddress,unsigned char controlByte,unsigned char readAddress);
//
#endif