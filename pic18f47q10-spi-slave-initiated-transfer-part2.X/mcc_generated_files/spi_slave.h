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

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SPI_SLAVE_H
#define	SPI_SLAVE_H

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    This function sets up the SPI Slave Select interrupt on change pins if SS is located
    on an IOC capable pin. It should be called during device initialization.
 * @Example
    spi_slave_init();
 */
void spi_slave_init(void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    This function is responsible for handling incoming messages for the SLAVE0 Slave Configuration.
    This handler will be called automatically if Interrupt on Change is enabled for the 
    Slave Select pin. Otherwise it should be called from the main while(1) loop.
 * @Example
    spi_slave_SSHandlerSLAVE0();
 */
void spi_slave_SSHandlerSLAVE0(void);


#endif	/* XC_HEADER_TEMPLATE_H */