# stm32f1-c-template

C template for an STM32F107-based board. (Cortex-M3)

Tested on EasyMx Pro v7 board with STM32F107 chip.

# Install
* Toolchain - [GNU ARM Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* (Windows only) - [MinGW and MSYS ](http://www.mingw.org/)
* Programmer & Debugger - [STLink](https://github.com/texane/stlink)

# Compile
* Browse into the directory and run `make` to compile.
```
Cleaning...
Building template.c
   text	   data	    bss	    dec	    hex	filename
    216	      0	      0	    216	     d8	template.elf
Successfully finished...
```

# Program
* Run `make burn` to program the chip.
```
...
Flash written and verified! jolly good!
```

# Debug
* Run `st-util` from one terminal
* Run `make debug` from a second terminal to debug the program.
* You can turn off tui layout with `tui disable` if needed
