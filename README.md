# MySPIM
A C programming based mini processor simulator called MySPIM.

Purpose
=======
MySPIM will demonstrate some functions of MIPS processor as well as the principle of the datapath and the control 
signals of MIPS processor. The MySPIM simulator should read in a file containing MIPS machine codes and simulate 
what the MIPS does cycle-by-cycle. 

Memory Usage
============
•	The size of memory of MySPIM is 64kB (Address 0x0000 to 0xFFFF).
•	The system assumes that all program starts at memory location 0x4000.
•	All instructions are word-aligned in the memory, i.e., the addresses of all instructions are multiple of 4.
•	The simulator (and the MIPS processor itself) treats the memory as one segment. (The division of memory into text, data, and stack segments is only done by the compiler/assembler.)
•	At the start of the program, all memory are initialized to zero, except those specified in the “-asc” file, as shown in the provided codes.
•	The memory is in big-endian byte order.
•	The memory is in the following format: e.g. Store a 32-bit number 0xaabbccdd in memory address 0x0 – 0x3. 

Conditions that the MySPIM Should Halt
=====================================
If one of the following situations is encountered, the global flag Halt is set to 1, and hence the simulation halts.
•	An illegal instruction is encountered.
•	Jumping to an address that is not word-aligned (being multiple of 4).
•	The address of lw or sw is not word-aligned.
•	Accessing data or jump to address that is beyond the memory.
Note: The instructions beyond the list of instructions in Figure 1 are illegal.


Compile/Run
===========
1. Open your terminal and go to the directory of the file.

2. Compile: 
	$ gcc -o spimcore spimcore.c project.c

3. After compilation, to use MySPIM, you would type the following command in UNIX:
	$ ./spimcore <filename>.asc

4. Command prompt should appear.

5. Spimcore works like a simple debugger with the following commands:

	r	Dump registers contents
	m	Dump memory contents (in Hexadecimal format)
	s[n]	Step n instructions (simulate the next n instruction). If n is not typed, 1 is assumed
	c	Continue (carry on the simulation until the program halts (with illegal instruction))
	H	Check if the program has halted
	d	ads1 ads2 Hexadecimal dump from address ads1 to ads2
	I	Inquire memory size
	P	Print the input file
	g	Display all control signals
	X, X, q, Q	Quit









