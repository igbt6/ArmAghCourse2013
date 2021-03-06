#include<LPC21xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "spi.h"
#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"
#include "dacMcp4921.h"
#include "mcp23S09.h"
#include "uart.h"
#include "command_decoder.h"
#include "string.h"


unsigned char ucTokenNr;
struct Token asToken[MAX_TOKEN_NR];


int main(void)
{
	MCP23S09_Init();
	char uartReceiverBuffer[RECEIVER_SIZE];
  UART_InitWithInt(9600);
	Led_Init(); //DEBUG 
  char cDataStr[15]= {"portget "}; 
	
		while(1)
    {
   
 			if(eReciever_GetStatus()==READY)
        {
            Reciever_GetStringCopy(uartReceiverBuffer);
            DecodeMsg(uartReceiverBuffer);
            if((ucTokenNr!=0)&&(asToken[0].eType==KEYWORD)) 
            {
							switch(asToken[0].uValue.eKeyword)
                {
                case DACSET:
                     if((asToken[1].eType==NUMBER)&&(asToken[1].uValue.uiNumber<0x0FFF))
												DAC_MCP4921_Set_Adv(asToken[1].uValue.uiNumber);
                    break;

								case PORTSET:
                     if((asToken[1].eType==NUMBER)&&(asToken[1].uValue.uiNumber<=0x00FF))
												Port_MCP23S09_Set(asToken[1].uValue.uiNumber);
                    break;
										 
							 case MCP_PORT_RD:
                     //if((asToken[1].eType==STRING)&&(asToken[1].uValue.uiNumber<0x0FFF))
												//DAC_MCP4921_Set_Adv(asToken[1].uValue.uiNumber);
												AppendUIntToString(Port_MCP23S09_Get(),cDataStr);
												Transmiter_SendString((unsigned char*)cDataStr);
                    break;
                default:
                    break;
                }
            }
        }
			 
				//if(Transmiter_GetStatus()==FREE){
					//	displayWatchValue();
				//}
    }
		return 0;
}

////////////////////////////////////////////////////
