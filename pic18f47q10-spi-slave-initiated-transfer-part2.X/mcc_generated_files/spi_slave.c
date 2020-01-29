/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifdef __XC
#include <xc.h>
#endif
#include <stdint.h>
#include "spi1.h"
#include "spi_slave.h"
#include "spi_slave_app.h"
#include "pin_manager.h"

#define DUMMY_DATA 0x33

void spi_slave_DataHandlerSLAVE0(void);

void spi_slave_init(void) {
    IOCAF4_SetInterruptHandler(spi_slave_SSHandlerSLAVE0);
    IOCAPbits.IOCAP4 = 1; /* enable positive interrupt for SLAVE0_SS */;
    IOCANbits.IOCAN4 = 1; /* enable positive interrupt for SLAVE0_SS */;
}

void spi_slave_SSHandlerSLAVE0(void) {
    if (0 == PORTAbits.RA4 /* get SLAVE0_SS value */) {
        if (SPI1_Open(SLAVE0_CONFIG)) { //open appropriate slave
            //only run the handler if we have the bus

            //No SPI Interrupt enabled. 
            //Call spi_slave_DataHandlerSLAVE0 from the main while(1) loop manually when SPI has received a byte

            // Stuff the first dummy byte here, if you need something else do it here
            //    Note: We are considering stuffing the dummy byte inside the handler instead,
            //          that means if you do not stuff it you get something random. 
            SPI1_WriteByte(DUMMY_DATA);
            if (slaveList[SLAVE0_CONFIG].SPI_beginHandler) {
                slaveList[SLAVE0_CONFIG].SPI_beginHandler();
            }
        }
    } else {
        if (slaveList[SLAVE0_CONFIG].SPI_endHandler) {
            slaveList[SLAVE0_CONFIG].SPI_endHandler();
        }
        SPI1_Close();
    }
}

void spi_slave_DataHandlerSLAVE0(void) {
    if (slaveList[SLAVE0_CONFIG].SPI_xchgHandler) {
        SPI1_WriteByte(slaveList[SLAVE0_CONFIG].SPI_xchgHandler(SPI1_ReadByte()));
    }
}

