/*
 * File Name : 
 *				LCD.c
 * 
 * File Description :
 *				LCD Source file Driver.
 *
 * Created: 15/01/2015 
 * Author: MiKeL
 *
 * LCD is connected to PORTA as follow:
									RS		>> PORTA.0
									R/W		>> PORTA.1
									E		>> PORTA.2
									D4-D7	>> PORTA.4 - PORTA.7
 *
 *	FUNCTIONS : 1- void_LCD_init()	>> LCD initialization.
				2- void_LCD_print() >> Print a string on LCD.
				3- void_LCD_goto()	>> Goto a specific position on LCD screen.
				4- void_LCD_clear()	>> Clear LCD screen.
				5- LCD_data()		>> Write data to LCD data register.
				6- LCD_cmnd()		>> Write command to LCD command register.
 */ 

#include <avr/io.h>
#ifndef F_CPU
	#define F_CPU 8000000UL
#endif
#include <util/delay.h>

// Definitions.
#define LCD_PORT		PORTA
#define LCD_PORT_DIREC	DDRA
#define RS	0
#define RW	1
#define E	2
	
unsigned char col_pos, row_pos;
static	unsigned char cntr;

/********************************
----------- LCD DATA ------------
********************************/
void LCD_data(unsigned char data)
{
	LCD_PORT = (LCD_PORT & 0x0F) | (data & 0xF0);		// Sending higher nibble of 'data'.
	LCD_PORT |= (1<<RS);								// Setting RS pin.
	LCD_PORT &= ~ (1<<RW);								// Clearing R/W pin.
	LCD_PORT |= (1<<E);									// Setting E pin.
	_delay_ms(1);										// Delay 1 us.
	LCD_PORT &= ~ (1<<E);								// Clearing E pin.
	
	LCD_PORT = (LCD_PORT & 0x0F) | (data<<4);			// Sending lower nibble of 'data'.
	LCD_PORT |= (1<<E);									// Setting E pin.
	_delay_ms(1);										// Delay 1 us.
	LCD_PORT &= ~ (1<<E);								// Clearing E pin.
}

/********************************
--------- LCD COMMAND -----------
********************************/
void LCD_cmnd(unsigned char cmnd)
{
	LCD_PORT = (LCD_PORT & 0x0F) | (cmnd & 0xF0);		// Sending higher nibble of 'cmnd'.
	LCD_PORT &= ~ (1<<RS);								// Clearing RS pin.
	LCD_PORT &= ~ (1<<RW);								// Clearing R/W pin.
	LCD_PORT |= (1<<E);									// Setting E pin.
	_delay_ms(1);										// Delay 1 us.
	LCD_PORT &= ~ (1<<E);								// Clearing E pin.
	
	_delay_ms(2);	
	
	LCD_PORT = (LCD_PORT & 0x0F) | (cmnd<<4);			// Sending lower nibble of 'cmnd'.
	LCD_PORT |= (1<<E);									// Setting E pin.
	_delay_ms(1);										// Delay 1 us.
	LCD_PORT &= ~ (1<<E);								// Clearing E pin.
}

/********************************
------ LCD INITIALIZATION -------
********************************/
extern void void_LCD_init()
{
	col_pos = 1;	row_pos =1;
	LCD_PORT_DIREC = 0xFF;		// LCD uses PORTA.
	
	LCD_PORT &= ~ (1<<E);		// Clearing E pin.
	_delay_ms(1);			// Delay 2 ms for startup.
	
	// LCD initialization for 4-bit ( 5x7 matrix ).
	LCD_cmnd(0x33);		_delay_ms(1);	
	LCD_cmnd(0x32);		_delay_ms(1);
	LCD_cmnd(0x28);		_delay_ms(1);
	LCD_cmnd(0x0E);		_delay_ms(1);
	LCD_cmnd(0x01);
	_delay_ms(10);
	LCD_cmnd(0x06);		
	_delay_ms(1);
	LCD_cmnd(0x0C);
	_delay_ms(1);
	
}

/********************************
---------- LCD PRINT ------------
********************************/
extern void void_LCD_print(unsigned char * strng)
{
	cntr=0;
	while(strng[cntr] != 0)
	{
		LCD_data(strng[cntr++]);
		
		// Increment cursor position.
		col_pos++;
		if(col_pos == 17)
		{
			col_pos = 1;
			row_pos = 2;
			void_LCD_goto(row_pos,col_pos);
		}
	}
}

/********************************
----------- LCD GOTO ------------
********************************/
extern void void_LCD_goto(unsigned char row, unsigned char col)
{
	unsigned char LCD_first_pos_addr[] = {0x80, 0xC0, 0x94, 0xD4};	// LCD addresses of first character of each row.
		
	LCD_cmnd( LCD_first_pos_addr[row-1] + (col-1) );
	_delay_ms(100);
	
	// Monitor cursor position.
	col_pos = col;
	row_pos = row;
}

/********************************
----------- LCD CLEAR -----------
********************************/
extern void void_LCD_clear()
{
	LCD_cmnd(0x01);
	_delay_ms(200);
	row_pos = 1;	col_pos = 1;
	void_LCD_goto(row_pos,col_pos);
}