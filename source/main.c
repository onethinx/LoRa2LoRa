/********************************************************************************
 *    ___             _   _     _			
 *   / _ \ _ __   ___| |_| |__ (_)_ __ __  __
 *  | | | | '_ \ / _ \ __| '_ \| | '_ \\ \/ /
 *  | |_| | | | |  __/ |_| | | | | | | |>  < 
 *   \___/|_| |_|\___|\__|_| |_|_|_| |_/_/\_\
 *
 ********************************************************************************
 *
 * Copyright (c) 2020 Onethinx BV <info@onethinx.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ********************************************************************************
 *
 * Created by: Rolf Nooteboom
 *
 * Date: 03-10-2020
 * 
 * LoRa to LoRa example
 * 
 * This example demonstrates the use of LoRa to LoRa communication
 * 
 * The LoRa parameters are stored in LoRaParams
 ********************************************************************************/

#include "OnethinxCore01.h"
#include "OnethinxExt01.h"
#include "cy_gpio.h"

/* The Module definition: set to 1 for the transmitting module, to 0 for the receiving module */
#define MODULE_TX 1

/* The Payload Size we use for this example */
#define PAYLOADSIZE 12

/* Let the M0p go into deepsleep when waiting for the CM4, we don't use the BLE ECO and we like to debug this code */
coreConfiguration_t	coreConfig = {
	.System.Idle.Mode = 		M0_DeepSleep,
	.System.Idle.BleEcoON = 	false,
	.System.Idle.DebugON =  	true,
};

/* The LoRa Radio parameters are defined below. Change the frequency according your region */
LoRaParams_t LoRaParams =
{
	.Frequency = 8681000,		// Frequency in 100Hz steps
	.TXpower = PWR_MAX,
	.Modulation.SF = LORA_SF10,
	.Modulation.BW = LORA_BW_250,
	.Modulation.CR = LORA_CR_4_5,
	.Modulation.LowDataRateOptimize = LORA_LOWDATARATEOPTIMIZE_OFF,
	.Packet.PreambleLength = 8,
	.Packet.HeaderType = LORA_PACKET_VARIABLE_LENGTH,
	.Packet.PayloadSize = PAYLOADSIZE,
	.Packet.CRCmode = LORA_CRC_ON,
	.Packet.IQmode = LORA_IQ_NORMAL,
	.Packet.SyncWord = LORA_MAC_PUBLIC_SYNCWORD
};

/* Declare the TX and RX buffers */
uint8_t LoRaTXbuffer[PAYLOADSIZE] = "Hello LED x";
uint8_t LoRaRXbuffer[PAYLOADSIZE];

/* The GPIO definition for the Onethinx Development Kit */
#define LED_BLUE_PORT           P12_4_PORT
#define LED_RED_PORT            P12_4_PORT
#define LED_BLUE_PIN            P12_4_PIN
#define LED_RED_PIN             P12_5_PIN
#define LED_B_SET(value)		Cy_GPIO_Write(LED_BLUE_PORT, LED_BLUE_PIN, value)
#define LED_R_SET(value)		Cy_GPIO_Write(LED_RED_PORT, LED_RED_PIN, value)

#define BUTTON_PORT            P0_4_PORT
#define BUTTON_PIN             P0_4_PIN

/* Declare the RadioStatus and CoreStatus globally for debugging purposes */
RadioStatus_t RadioStatus;
coreStatus_t CoreStatus;

int main(void)
{
	CyDelay(1000);	// Use only for debugging otherwise testmode acquire might not work. Testmode acquire (ENABLE_ACQUIRE 1) is the preferred method. 
					// To acquire in non-testmode use 'ENABLE_ACQUIRE 0' in the OpenOCD settings (launch.json)

	/* Initialize the GPIO for the blue LED */
	Cy_GPIO_Pin_FastInit(LED_BLUE_PORT, LED_BLUE_PIN, CY_GPIO_DM_STRONG, 0UL, HSIOM_SEL_GPIO);
	/* Initialize the GPIO for the red LED */
	Cy_GPIO_Pin_FastInit(LED_RED_PORT, LED_RED_PIN, CY_GPIO_DM_STRONG, 0UL, HSIOM_SEL_GPIO);
	/* Initialize the GPIO for the button */
	Cy_GPIO_Pin_FastInit(BUTTON_PORT, BUTTON_PIN, CY_GPIO_DM_HIGHZ, 1UL, HSIOM_SEL_GPIO);

	/* Enable global interrupts */
	__enable_irq();

	// Set the initial LED state
	bool LED_B_nR = true;

	/* Initialize the LoRaWAN Core */
	CoreStatus = LoRaWAN_Init(&coreConfig);

	if (CoreStatus.system.version < 0x000000BC) while(1)
	{
		/* Check the stack version: the minimum stack version is 0x000000BC */
		/* The code will hang here and flash the LEDs if you need a stack upgrade */
		/* Contact Onethinx for requesting a stack upgrade */
		LED_B_SET(LED_B_nR); 
		LED_R_SET(!LED_B_nR);
		CyDelay(200);
		LED_B_nR = !LED_B_nR;
	}

	/* Unlock the LoRaWAN Core for the use of non-LoRaWAN related functionality such as bare LoRa Communication */
	/* Unlocking the Core may void LoRa Alliance Certification and/or CE Certification */
	/* It is the user's responsibility to observe compliance with applicable (local) regulatories (allowed frequencies, output power, duty cycle etc.) */
	LoRaWAN_Unlock();



	/* The Transmitter */
	while(MODULE_TX)
	{
		/* Wait for the button to get pressed */
		while (Cy_GPIO_Read(BUTTON_PORT, BUTTON_PIN) == 0) {};

		/* Set the LEDs */
		LED_B_SET(LED_B_nR); 
		LED_R_SET(!LED_B_nR);

		/* Modify the TX buffer according the LED state */
		LoRaTXbuffer[10] = LED_B_nR? 'B' : 'R';

		/* Send the data over LoRa, use a timeout of 3000ms in case the transmitting takes too long (for example if the user sends too much data on a low datarate) */
		LoRa_TX(&LoRaParams, &RadioStatus, (uint8_t *) &LoRaTXbuffer, sizeof(LoRaTXbuffer), 3000);
		
		/* Debounce: wait for the button to get released and add a small delay */
		while (Cy_GPIO_Read(BUTTON_PORT, BUTTON_PIN) == 1) {};
		CyDelay(50);
		/* Toggle the LED state */
		LED_B_nR = !LED_B_nR;
	}

	/* The Receiver */
	while(1)
	{
		/* Activate the receiver for 3000ms max. */
		/* The LoRA_RX function will immidiately return upon reception of data  */
		LoRa_RX(&LoRaParams, &RadioStatus, (uint8_t *) &LoRaRXbuffer, sizeof(LoRaRXbuffer), 30000);

		/* Check for valid data */
		if ((RadioStatus.IRQstatus.IRQ_RX_DONE) && (!RadioStatus.IRQstatus.IRQ_CRC_ERROR))
		{
			/* Set the LEDs */
			LED_B_SET(LoRaRXbuffer[10] == 'B'); 
			LED_R_SET(LoRaRXbuffer[10] == 'R'); 
		}
	}
}
/* [] END OF FILE */
