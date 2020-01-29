/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.78
        Device            :  PIC18F47Q10
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"

//#define __DEBUG__

#define NO_ACTION                      (0)
#define SW_RELEASED                    (1)
#define SW_PRESSED                     (2)

#define SPI_TX_BLOCK_SIZE              (16)
#define SPI_RX_BLOCK_SIZE              (SPI_TX_BLOCK_SIZE*2)

#define SPI_SLAVE_REQ_INITIATE()       SREQ_SetHigh()
#define SPI_SLAVE_REQ_TERMINATE()      SREQ_SetLow()
#define SPI_SLAVE_REQ_PIN_VALUE()      SREQ_GetValue() 
#define SLAVE_SELECT_PIN_VALUE()       SLAVE0_SS_GetValue()

#define SLAVE_SYNC_DELAY               (1) // delay count in mS
#define SLAVE_SYNC_DELAY_ms(mS)        __delay_ms(mS) //custom name for delay in milliseconds


volatile uint8_t txBlock[SPI_TX_BLOCK_SIZE] = "I_AM_SLAVE"; // Transmitter buffer for SPI transmission
volatile uint8_t rxBlock[SPI_RX_BLOCK_SIZE] = {0};      // Receiver buffer for SPI transmission
uint8_t exchangeByteCount = 0;                          // counter for exchanged bytes

volatile uint8_t SW0 = 0;
volatile uint8_t slaveSelect = 0;
volatile uint8_t slaveRelease = 0;
void IOCBF4_UserInterruptHandler(void);
void IOCAF4_UserInterruptHandler(void);
/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    printf("\r\n\n\nSPI_Slave: System Started \r\n");
    IOCBF4_SetInterruptHandler(IOCBF4_UserInterruptHandler);
    printf("SPI_Slave: Registered User interrupt Handler for SW0 \r\n");
    SPI_SLAVE_REQ_TERMINATE();
#ifdef  __DEBUG__
    printf("SPI_Slave: S_INT Pin Value: %d \r\n",SINT_GetValue());
    printf("SPI_Slave: SS Pin Value: %d \r\n",SLAVE15_SS_GetValue());
#endif
    while (1)
    {
        // Add your application code
        if(SW0 == SW_RELEASED)
        {
            // reset all parameters & SPI
            SW0 = NO_ACTION;
            slaveSelect = 0;
            slaveRelease = 0;
            if(SPI_SLAVE_REQ_PIN_VALUE() == HIGH)
            {
              SPI_SLAVE_REQ_TERMINATE(); 
            __delay_ms(1);// wait for resetting Master & Slave  
            }
            printf("\r\nSPI_Slave: Requesting Master Master to Start Data Exchange \r\n");  
#ifdef  __DEBUG__
            printf("\r\nSPI_Slave: Slave has Data to exchange \r\n");         
            printf("SPI_Slave: S_INT Pin Value: %d \r\n",SINT_GetValue());
#endif
            
            SPI_SLAVE_REQ_INITIATE();  // Sending slave request 
            
            if(SPI_SLAVE_REQ_PIN_VALUE() == HIGH)
            {
                IOCAF4_SetInterruptHandler(IOCAF4_UserInterruptHandler);  // Registered User interrupt Handler for SS
                LED0_Toggle();
                SPI1_Close(); // Closing SPI whether it is opened or not...
#ifdef  __DEBUG__               
                printf("SPI_Slave: Registered User interrupt Handler for SS \r\n");
                printf("SPI_Slave: Interrupting Master... \r\n");
                printf("SPI_Slave: Closing SPI whether it is opened or not... \r\n");
#endif                
            }
            
        }
        if(slaveSelect == 1)// Master Slave Select PIN Interrupt
        {
            printf("\rSPI_Slave: Slave is selected by Master \r\n");  
            slaveSelect = 0;
#ifdef  __DEBUG__  
            printf("SPI_Slave: SS Pin Value: %d \r\n",SLAVE15_SS_GetValue());        
#endif
            if (SPI1_Open(SLAVE0_CONFIG))
            { 
                printf("\rSPI_Slave: SPI Open \r\n");  
                printf("\rSPI_Slave: Starting data exchange \r\n"); 
                while((LOW == SLAVE_SELECT_PIN_VALUE()) && (exchangeByteCount < SPI_TX_BLOCK_SIZE))
                {
                    rxBlock[exchangeByteCount] = SPI1_ExchangeByte(txBlock[exchangeByteCount]);
                    exchangeByteCount++;
                }
                SPI_SLAVE_REQ_TERMINATE();
                printf("\rSPI_Slave: Data exchange completed \r\n");
                printf("\rSPI_Slave: Slave request Terminated \r\n"); 
#ifdef  __DEBUG__ 
                printf("SPI_Slave: SS Pin Value: %d \r\n",SLAVE15_SS_GetValue());
                printf("SPI_Slave: S_INT Pin Value: %d \r\n",SINT_GetValue());
#endif
                printf("SPI_Slave: Sent Data            : %s \r\n",txBlock);
                printf("SPI_Slave: Received Data        : %s \r\n",rxBlock);
                printf("SPI_Slave: No of Bytes Exchanged: %d \r\n",exchangeByteCount);
                exchangeByteCount = 0;
                rxBlock[exchangeByteCount] = 0;
            }
            else
            {
                SPI1_Close();
                SPI_SLAVE_REQ_TERMINATE();
#ifdef  __DEBUG__ 
                printf("SPI_Slave: S_INT Pin Value: %d \r\n",SINT_GetValue());
#endif
                printf("SPI_Slave: SPI Opening Error... Closing SPI \r\n\r\n");
                
            }
        }
        if(slaveRelease == 1)// Master Slave Release PIN Interrupt
        {
            slaveRelease = 0;
            SPI1_Close();
            SPI_SLAVE_REQ_TERMINATE();
#ifdef  __DEBUG__ 
            printf("SPI_Slave: S_INT Pin Value: %d \r\n",SINT_GetValue());
            printf("SPI_Slave: SS Pin Value: %d \r\n",SLAVE15_SS_GetValue());
#endif
            printf("SPI_Slave: Master Terminated Communication \r\n");
            printf("SPI_Slave: SPI Closed \r\n\r\n");
        }       
    }
}

/**
  User interrupt handler for IOCBF4 (SW0 press)
*/
void IOCBF4_UserInterruptHandler(void)
{
    SW0 = SW_RELEASED;
}

void IOCAF4_UserInterruptHandler(void)
{
    if (LOW == SLAVE_SELECT_PIN_VALUE()) 
    {
        slaveSelect = 1;      
    } 
    else if (HIGH == SLAVE_SELECT_PIN_VALUE()) 
    {
        slaveRelease = 1;
    }
}


/**
 End of File
*/