Use the INSTALL file to get started. This code should run on the STM32MP157F-DK2 board in a linux development environment.

Use the "make" command in the project root directory to create the binaries. 

M4 binary is located in the bin/m4/ directory

A7 uimg is located in build/corea7/ directory

SD card should be formatted according to instructions found here: https://github.com/4ms/stm32mp1-baremetal

SD card should contain both the M4 binary and the A7 uimg files.


Booting the board and loading the m4 binary into memory:

  Connect the board with using the ST-Link cable and use PuTTY (or similar) to communicate with the board (115200 serial 8 bits no parity 1 stop)
  
  Interrupt bootup with key press and enter these commands in order to load m4 binary into memory
  
    "rproc init"
    
    "fatload mmc 0:4 0xC2000000 main.elf"
    
    "rproc load 0 0xC2000000 ${filesize}"
    
    "run bootcmd"
    
    
 Both A7 and M4 binaries should now be running on their respective cores.
