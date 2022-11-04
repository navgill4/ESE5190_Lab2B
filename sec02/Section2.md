REPL to read and write RP2040 registers from a console (in progress)

So far I have the sec01 code running on the Qt Py 2040 https://github.com/navgill4/ESE5190_Lab2B/blob/main/sec01/flashlight_01.c
The python script connects to the serial console and takes an input from the user so if i send 'r', 'g', or 'b' the LED turns red, green and blue accordingly.

I am still working on the register read/write functionality.

My plan for that is to input the register address/value from the user in the python script.

On the c side I plan to get one character at a time and cast to an unsigned int 32 bit and then the C code will read/write to the registers
