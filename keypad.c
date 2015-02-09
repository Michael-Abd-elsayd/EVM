/*
 * File Name :
 *			keypad.c
 *
 * File Description:
 *			Keypad Driver Source file
 *
 *  Created: 15/01/2015 05:03:06 م
 *  Author: MiKeL
 *
 *	N.B: Cols >> Pins(0,1,2,3)
 *		 Rows >> Pins(4,5,6,7)
 *
 * FUNCTIONS : 1- void_keypad_init()	>> Initialize Keypad (Determines Keypad Port).
			   2- u_char_keypad_read()	>> Reading the pressed key.
 */
 
 #include <avr/io.h>
 #ifndef F_CPU
	#define F_CPU 8000000UL
#endif
 #include <util/delay.h>
 
 // Definitions.
 #define KEYPAD_PORT		PORTC
 #define KEYPAD_PIN			PINC
 #define KEYPAD_PORT_DDR	DDRC
 
 unsigned char colloc, rowloc;
 
 unsigned char keypad[4][4] = { '0','1','2','3',
								'4','5','6','7',
								'8','9','A','B',
								'C','D','E','F' };
/***************************************
-------- KEYPAD INITIALIZATION ---------
***************************************/ 
extern void void_keypad_init()
{
	KEYPAD_PORT_DDR = 0xF0;
}
 
 /***************************************
 ------------- KEYPAD READ --------------
 ***************************************/
extern unsigned char u_char_keypad_read()
{	
	
		// Ensure all keys are released.
		do
		{
			KEYPAD_PORT = 0x0F;					// Ground all Rows.
			colloc = (KEYPAD_PIN & 0x0F);
		} while (colloc != 0x0F);
		
		
		do
		{
			// Monitor the keys (cols).
			do 
			{
				_delay_ms(20);							// Wait for debounce.
				colloc = (KEYPAD_PIN & 0x0F);
			} while (colloc == 0x0F);
			
			_delay_ms(20);							// Wait for debounce.
			colloc = (KEYPAD_PIN & 0x0F);
		} while(colloc == 0x0F);
		
						
		// Determines the col & row.
		while(1)
		{
			// Ground First Row and check cols.
			KEYPAD_PORT = 0xEF;
			colloc = (KEYPAD_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 0;
				break;
			}
			
			// Ground Second Row and check cols.
			KEYPAD_PORT = 0xDF;
			colloc = (KEYPAD_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 1;
				break;
			}
			
			// Ground Third Row and check cols.
			KEYPAD_PORT = 0xBF;
			colloc = (KEYPAD_PIN & 0x0F);
			if(colloc != 0x0F)
			{
				rowloc = 2;
				break;
			}

			KEYPAD_PORT = 0x7F;
			colloc = (KEYPAD_PIN & 0x0F);
			rowloc = 3;
			break;
		}			
		if(colloc == 0x0E)
			return keypad[rowloc][0];
		else if(colloc == 0x0D)
			return keypad[rowloc][1];
		else if(colloc == 0x0B)
			return keypad[rowloc][2];
		else
			return keypad[rowloc][3];
	
}