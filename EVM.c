/*
 * PROJECT : 
 *		Electronic Voting Machine
 *
 *  Created: 23/01/2015 
 *  Author: Michael
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>
#include "LCD.h"
#include "keypad.h"

// Global Area.
void initializatoin();
void print(char msg[11]);
void leds(unsigned char led_color);
void control(unsigned char input);
unsigned char get();

unsigned char _1, _2, _3;

// Main Function.
int main(void)
{
	initializatoin();		// System Initialization.
	print("welcome");		// Welcome Message.
	leds('R');				// RED LED ON.
	
	while(1)
	{
	control(get());
	}	
}

// USER-FUNCTION AREA.
// Initialization Function.
void initializatoin()
{
	DDRD = 0x03;			// PD.0 & PD.1 Output.
	void_LCD_init();		
	void_keypad_init();
}

// PRINT Function.
void print(char msg[11])
{
	unsigned char *text;
	unsigned char content[20];

	if( !strcmp(msg,"welcome") ) 
	{
			void_LCD_clear();
			text = "Welcome to EVM  0:start C:result";
			void_LCD_print(text);
	}			
	else if( !strcmp(msg,"choose") )
	{
		void_LCD_clear();
		text = "Choose 1, 2 or 3Any key:Cancel";
		void_LCD_print(text);
	}	
	else if( !strcmp(msg,"confirm") )
	{
			void_LCD_clear();
			text = "Confirm ?   F:OkAny key:Cancel";
			void_LCD_print(text);
	}
	else if( !strcmp(msg,"done") )
	{
			void_LCD_clear();
			text = "Thnx for voting Any key:Home";
			void_LCD_print(text);
	}			
	else if( !strcmp(msg,"result") )
	{
			void_LCD_clear();
			
			text = "1 = ";
			void_LCD_print(text);		
			itoa(eeprom_read_byte((unsigned char*)1) ,content,10);
			void_LCD_print( content );
			
			void_LCD_goto(1,7);
			text = "   2 = ";
			void_LCD_print(text);
			itoa(eeprom_read_byte((unsigned char*)2) ,content,10);
			void_LCD_print( content );
			
			void_LCD_goto(2,0);
			text = " 3 = ";
			void_LCD_print(text);
			itoa(eeprom_read_byte((unsigned char*)3) ,content,10);
			void_LCD_print( content );
	}
	
}

// LEDS Function.
void leds(unsigned char led_color)
{
	switch (led_color)
	{
		case 'R':
			PORTD = 0x01;
			break;
		case 'G':
			PORTD = 0x02;
			break;
	}
}

// GET Function.
unsigned char get()
{
	return u_char_keypad_read();
}

// CONTROL Function.
void control(unsigned char input)
{
	switch(input)
	{
		// 0 Means start voting process.
		case '0':
			// Choose msg is displayed and Green Led is ON.
			print("choose");
			leds('G');
			// Get the next input.
			input = get();
			// Check the new input.
			switch (input)
			{
				// '1' has been selected.
				case '1':
					// Display Confirm msg and get confirmation input.
					print("confirm");
					input = get();
					// Check the confirmation input.
					switch (input)
					{
						// 'F' Means OK.
						case 'F':
							_1 = eeprom_read_byte((unsigned char*)1);						
							++_1;
							eeprom_write_byte((unsigned char*)1,_1); 
							print("done");				
							break;
						// Any key Means cancel the process.
						default:
							print("welcome");
							leds('R');
					}
					break;
				// '2' has been selected.
				case '2':
					// Display Confirm msg and get confirmation input.
					print("confirm");
					input = get();
					// Check the confirmation input.
					switch (input)
					{
						case 'F':
							_2 = eeprom_read_byte((unsigned char*)2);
							++_2;
							eeprom_write_byte((unsigned char)2,_2);
							print("done");
							break;
						// Any key Means cancel the process.
						default:
							print("welcome");
							leds('R');
					}
					break;
				// '3' has been selected.
				case '3':
					// Display Confirm msg and get confirmation input.
					print("confirm");
					input = get();
					// Check the confirmation input.
					switch (input)
					{
						case 'F':
							_3 = eeprom_read_byte((unsigned char*)3);
							++_3;
							eeprom_write_byte((unsigned char)3,_3);
							print("done");
							break;
						// Any key Means cancel the process.
						default:
							print("welcome");
							leds('R');
					}
					break;
				// Any other key Means cancel the process and Red Led is ON.
				default:
					print("welcome");
					leds('R');
			}
			break;
		// Printing voting results.	
		case 'C':
			print("result");
			leds('R');
			break;
		// Any key except '0' has no effect.
		default:
			print("welcome");
			leds('R');
	}
}