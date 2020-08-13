[![MCHP](images/microchip.png)](https://www.microchip.com)

# SPI Slave Initiated transfer Part-2

# Introduction
This demo illustrates the SPI communication between two PIC18F47Q10 MCUs. The Master Synchronous Serial Port (MSSP) module of PIC18F47Q10 can operate as SPI.

One Q10 MCU is configured as an SPI Master and the other as an SPI Slave. The demo uses interrupt-based communication initiation from the slave device. In SPI communication master has control over the clock, thus master initiates the communication by generating the clock.

This example demonstrates, how the slave can request the master to start SPI communication, so the slave can send data to the master.

**This part of the demo illustrates the implementation of only SPI salve. It is recommended to refer the first part "[pic18f47q10-spi-slave-initiated-transfer-part1](https://github.com/microchip-pic-avr-examples/pic18f47q10-spi-slave-initiated-transfer-part1)" of the demo before proceeding further for the SPI Master configuration.**

# Useful Links
- [PIC18-Q10 Product Family Page](https://www.microchip.com/design-centers/8-bit/pic-mcus/device-selection/pic18f-q10-product-family)
- [PIC18F47Q10 Data Sheet](http://ww1.microchip.com/downloads/en/DeviceDoc/40002043D.pdf)
- [PIC18F47Q10 Code Examples on GitHub](https://github.com/microchip-pic-avr-examples?q=pic18f47q10)


# Demo Description
 ![block_diagram](images/block_diagram.jpg)

The demo consists of two PIC18F47Q10 Curiosity nano evaluation boards for demonstrating SPI Master and SPI Slave. SPI peripheral's port pins from the master to the slave (SDI, SDO, SCK, and SS) are connected using jumper wires. To enable communication initiation request from the slave, one I/O pin is connected to the master from slave. For initiation of communication, the slave device generates interrupt on GPIO to master device, which notifies the master that the slave has data to send and master can start SPI communication.
In this demo application, switch press event is used as a trigger to exchange data. After pressing the switch on the slave device, it sends an interrupt to the master; then the master selects the slave and, the master and the slave exchanges the data.

This demo also uses UART to send the data serially to the terminal window. Thus, the exchanged data can be observed using a terminal emulator.
Please refer to the timing diagram for details.

##  Timing Diagrams
* Case 1:  Data transferred from the slave to the master = 16 bytes.

 ![timing_diagram_16bytes](images/timing_diagram_16bytes.png)
**Note**: *This code example transfers 16 bytes of data.*

* Case 2: Data transferred from slave to master < 16 bytes.  

 ![timing_diagram_less_than_16bytes](images/timing_diagram_less_than_16bytes.png)
**Note**: *To transfer less than 16 bytes of data, configure the Macro **SPITXBLOCKSIZE** in **main.c** in the slave firmware.*

## Firmware Flowchart

![flowchart](images/flowchart.jpg)


# Hardware Used
* [PIC18F47Q10 Curiosity nano evaluation board](https://www.microchip.com/Developmenttools/ProductDetails/DM182029)

With full program and debug capabilities, the PIC18F47Q10 Curiosity Nano evaluation kit offers complete support for the new design. With the award-winning MPLAB X integrated development platform and MPLAB Code Configurator (MCC), the kit provides access to the Intelligent analog and Core Independent Peripherals on the PIC18F47Q10. Figure 2 shows PIC18F47Q10 Curiosity Nano board.
![curiosity_nano](images/curiosity_nano.jpg)

# Software tools
Microchip’s free MPLAB X IDE, compiler and MPLAB Code Configurator (MCC) graphical code generator are used throughout the application firmware development to provide easy and hassle-free user experience. Following are the tool versions used for this demo application:

* [MPLAB® X IDE v5.30](http://www.microchip.com/mplab/mplab-x-ide)
* [XC8 Compiler v2.10](http://www.microchip.com/mplab/compilers)
* [MPLAB® Code Configurator (MCC) v3.85.1](https://www.microchip.com/mplab/mplab-code-configurator)
* [MCC Device Libraries PIC10 / PIC12 / PIC16 / PIC18  MCUs v1.78.0](microchip.com/mplab/mplab-code-configurator)
* [Microchip PIC18F-Q Series Device Support 1.3.89](https://packs.download.microchip.com/)

*Note: For running the demo, the installed tool versions should be same or later. This example is not tested with previous versions.*
# Demo realization
## 1. Creating a new Project

* Open MPLAB X IDE latest version.
* Go to File > New Project. (click)
* Select (Categories) Microchip Embedded > (Projects) Standalone Project. (Press Next)
* (Select Device) > Enter Device name (Microcontroller Part Number: in this project enter  **PIC18F47Q10**). (Press Next)    
* Select Tool is optional. (Press Next)
* Select Latest XC8 Compiler. (Press Next)
* Enter Project Name and File location. (Press Finish)
* Click on MCC symbol to configure peripherals.

 ![new_project](images/new_project.png)

## 2. Configure the hardware peripherals
### Configure System Module.
1.	In Resource Management tab > (Under Project Resources) Select System Module >Select Easy Setup.
2.	Select Oscillator (**HFINTOSC**).
3.	Select HF Internal Clock (**8_MHz**).
4.	Select Clock Divider (**1**).
5.	Disable Watchdog timer (**WDT Disabled**).
6.	Enable Low-voltage Programming.

**Note:**  *SPI clock frequency depends on this main system clock frequency*

![system_configuration](images/system_configuration.png)

### Configure SPI Slave
1.	In **Resource Management** tab > (Under Device Resources) go to **Libraries** > **Foundation Service**s > **SPISLAVE**. Add SPISLAVE by clicking on (+) button.
The SPISLAVE library should show up in Project Resources under Peripherals.

 ![SPISLAVE](images/SPISLAVE.png)

2.	Now in **SPISLAVE** easy setup tab and add a driver using the "+" sign at the bottom left corner of the window.
3.	This automatically adds **MSSP1** as it supports SPI slave communication.
4.	Now go back to SPISLAVE easy setup tab.  Configure parameters of SPI as desired. This example uses default configuration.
5.	Under **Pin Manager: Grid View**, Initially RC4 pin is selected for SDO1 and SDI1, Uncheck RC4 of SDO1 and then Check RC5 for SDO1.

 ![MSSP1](images/MSSP1.png)

### Add and Configure the EUSART Peripheral.
1.	Under **Device Resources**-> **Peripherals** -> **EUSART** -> **EUSART2**.
2.	Click on (+) symbol to add **ESURT2**.
3.	Select **EUSART2** and under Easy Setup, Configure UART.
4.	Disable Receive.
5.	Enable **Redirect STDIO to USART**. (It enables the printf function)
6.	Under **Pin Manager: Grid View**, Uncheck the RB7 Pin (RX2) and RB6 Pin (TX2). And check the RD0 Pin (TX2) to direct ESUART to STDIO.

 ![eusart](images/eusart.png)

### Configure the pins used on the device (Pin Manager: Grid View)
1.	Open Pin Manager Grid view.
2.	Verify MSSP1 Module Pins:
   - Port C and Pin 3 for SCK1
   - Port C and Pin 4 for SDI1
   - Port C and Pin 5 for SDO1
3.	Verify ESUART2 Pins:
   - Port D and Pin 0 for TX
4. Set the following pins:
 - Port E and Pin 0 as output for LED0
 - Port E and Pin 2 as input for SW0
 - Port B and Pin 4 as input for SW0.
 -	Port A and Pin 4 as input for Slave Select from Master.
 -	Port B and Pin 3 as output of Slave for requesting Master to initiate the SPI communication.

**Note:** *As PE2 pin does not support I/O interrupt, PE2 is connected to PB4 using jumper wire. PB4 is used for switch press interrupt detection.*

 ![pin_manager](images/pin_manager.png)

### Pin Module Configuration
1.	Under Project Resources, Click Pin Module.
2.	Change default Names under **Custom Name** as shown in figure below.
3.	Uncheck the **Analog** checkboxes for all the pins to configure them as digital I/Os.
4.	Check the **Start High** checkbox for pins RA4, RB4, RE2 and RE0. So that, on the power on LED (PE0) stays in off state and falling edge can be detected on the other port pins for switch press event (RE2 and RB4) and slave select pin (RA4).
5.	Check **WPU** checkbox for the input pins RA4, RB4 and RE2.
6.	Select **any** in **IOC** setting of RA4 and **positive** of RB4.
![pin_module](images/pin_module.jpg)
## 3.	Generate the Project files.
Click On save all button and click the **Generate** button to generate firmware. Verify the Output message.

 ![generate_project](images/generate_project.png)


## 4.	Hardware connection

| MASTER     | SLAVE     | Details     |
| -------- | -------- | -------- |
|RC3     | RC3   |CLOCK|
|RC4 |RC5 |MOSI|
| RC5| RC4|MISO|
|RB3 |RA4 |SLAVE SELECT|
| RB4|RB3| SLAVE REQUEST|
|GND|GND|GROUND|
|NA|RE2-RB4|SW0 (NANO BOARD)|


## 5.	Configure Terminal Emulator
Configure any Terminal Emulator to view UART data, set baud rate as configured for UART peripheral. (In this case 9600).

## 6.	Program the Board
* Program the MCU by clicking “Make and Program Device” icon on MPLAB XPRESS IDE as shown in below figure.
* IDE generates the .hex file, save the file in local PC  
* Drag the “.hex file” from the downloads section and drop the file on to the Curiosity drive. This should program the device

 ![programming](images/programming.png)

# Demo Operation
This example demonstrates the SPI communication between two PIC18F47Q10 microcontrollers. In which one acts as Master and other acts as a Slave. In this communication, Slave sends a request to Master to start SPI communication so slave can send data to Master. Then Master selects the Slave and starts SPI communication.
![demo_image](images/demo_image.jpg)

![connection_image](images/connection_image.jpg)

To operate this demo, followings are the steps:
1.	Program the SPI Master and the SPI Slave boards.
2.	Make connections between the master and the slave boards.
3.	Connect both the boards to any terminal application over PC via USB cable.
4.	Press SW0 switch present on PIC18F47Q10 nano board of the SPI Slave device.
5.	Observe the LED0 toggle on the slave side.
6.	Simultaneously observe LED0 toggle on the SPI Master side also.
7.	Observe the messages over terminal applications. Both the master and the slave will display the communication messages received.


**Note:** *Refer to "[pic18f47q10-spi-slave-initiated-transfer-part1](https://github.com/microchip-pic-avr-examples/pic18f47q10-spi-slave-initiated-transfer-part1)" for SPI Master program*
