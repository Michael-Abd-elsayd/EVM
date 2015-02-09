/*
 * File Name : 
 *				LCD.h
 * 
 * File Description :
 *				LCD Header file.
 *
 * Created: 15/01/2015 
 * Author: MiKeL
 */ 

#ifndef LCD_H_
#define LCD_H_

void LCD_data(unsigned char data);
void LCD_cmnd(unsigned char cmnd);

extern void void_LCD_init();
extern void void_LCD_print(unsigned char * strng);
extern void void_LCD_goto(unsigned char row, unsigned char col);
extern void void_LCD_clear();

#endif /* LCD_H_ */