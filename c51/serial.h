#ifndef __SERIAL_H__
#define __SERIAL_H__
//
void serial_init();
void serial_send_ch(unsigned char send_dat);
void serial_send_da(unsigned char send_dat[]);
unsigned char serial_recv_da();
void serial_mpu6050(int dat);
//
#endif