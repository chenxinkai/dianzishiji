#ifndef __DISPLAY1602_H__
#define __DISPLAY1602_H__
//
void write_data(unsigned char dat);
void write_com(unsigned char com);
void init_1602();
void display_1602_one(unsigned char x,y,cha);
void display_1602_multi(unsigned char x,y,unsigned char temp_display[]);
void format_display_1602(unsigned char x,y,int dat);
//
#endif