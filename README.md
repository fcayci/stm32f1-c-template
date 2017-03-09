# stm32f1-c-template

C template for an STM32F107-based board. (Cortex-M3)

Tested on EasyMx Pro v7 board with STM32F107 chip.

# Install
* Toolchain - [GNU ARM Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* (Windows only) - [MinGW and MSYS ](http://www.mingw.org/)
* Programmer - [STLink](https://github.com/texane/stlink)

# Compile
* Browse into the directory and run `make` to compile.
```
Cleaning...
Building template.c
   text	   data	    bss	    dec	    hex	filename
    130	      0	      0	    130	     82	template.elf
Successfully finished...
```

# Program
* Run `make burn` to program the chip.
```
...
...
Flash written and verified! jolly good!
```
* Sometimes you might need to burn twice to get it working or reset after the first one.
