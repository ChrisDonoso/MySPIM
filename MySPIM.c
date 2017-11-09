//CDA 3103C
//2 December 2016
//Group:
//Ramon Bautista 
//Christopher Donoso

#include "spimcore.h"

/* ALU */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    // Switch statement to control which ALU control gets executed
    switch (ALUControl)
    {
        // Addition: "Z = A + B"
        case 0x0:
            *ALUresult = A + B;
            break;

        // Subtraction: "Z = A - B"
        case 0x1:
            *ALUresult = A - B;
            break;

        // Set on less than (Signed): "If A < B, Z = 1; Otherwise, Z = 0"
        case 0x2:
            *ALUresult = ((int)A < (int)B) ? 1 : 0;
            break;

        // Set on less than (Unsigned): "If A < B, Z = 1; Otherwise, Z = 0"
        case 0x3:
            *ALUresult = (A < B) ? 1 : 0;
            break;

        // AND: "Z = A AND B"
        case 0x4:
            *ALUresult = A & B;
            break;

        // OR: "Z = A OR B"
        case 0x5:
            *ALUresult = A | B;
            break;

        // Left Shift: "Shift left B by 16 bits"
        case 0x6:
            *ALUresult = B << 16;
            break;

        // NOT: "Z = NOT A"
        case 0x7:
            *ALUresult = ~A;
            break;
    }

    // If ALUresult = 0, Zero = 1; otherwise Zero = 0;
    *Zero = (*ALUresult == 0) ? 1 : 0;
}

/* instruction fetch */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
	//Checks to see if PC is properly aligned.
	if(PC % 4 == 0)
	{
		*instruction = Mem[PC >> 2];

		return 0;
	}

	//If it is not aligned, a halt occurs.
	return 1;
}

/* instruction partition */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    //AND instructions and respective hexadecimal (right-shift if applicable)
	//in order to get the necessary bits of insutrction section.
	*op = (instruction & 0xFC000000) >> 26;
	*r1 = (instruction & 0x03E00000) >> 21;
	*r2 = (instruction & 0x001F0000) >> 16;
	*r3 = (instruction & 0x0000F800) >> 11;
	*funct = (instruction & 0x0000003F);
	*offset = (instruction & 0x000FFFF);
	*jsec = (instruction & 0x03FFFFFF);
}

/* instruction decode */
int instruction_decode(unsigned op,struct_controls *controls)
{
    // Decode the instruction using the opcode
    switch (op)
    {
        //R-Type
        case 0x0:
            controls->RegDst = 1;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0;
            controls->ALUSrc = 0;
            controls->RegWrite = 1;
            break;

        //Jump
        case 0x2:
            controls->RegDst = 2;
            controls->Jump = 1;
            controls->Branch = 2;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;
            break;

        //Beq
        case 0x4:
            controls->RegDst = 2;
            controls->Jump = 0;
            controls->Branch = 1;
            controls->MemRead = 0;
            controls->MemtoReg = 2;
            controls->ALUOp = 1;
            controls->MemWrite = 0;
            controls->ALUSrc = 2;
            controls->RegWrite = 0;
            break;

        //Addi
        case 0x8:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        //Slti
        case 0xA:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        //Sltiu
        case 0xB:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 3;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        //Lui
        case 0xF:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 6;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        //Lw
        case 0x23:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 1;
            controls->MemtoReg = 1;
            controls->ALUOp = 0;
            controls->MemWrite = 0;
            controls->ALUSrc = 1;
            controls->RegWrite = 1;
            break;

        //Sw
        case 0x2B:
            controls->RegDst = 0;
            controls->Jump = 0;
            controls->Branch = 0;
            controls->MemRead = 0;
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 1;
            controls->ALUSrc = 1;
            controls->RegWrite = 0;
            break;

        //Since none of the conditions were met, a halt occurs.
        default:
            return 1;
    }

    return 0;
}

/* Read Register */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	//Initializes data with respective addresses of r1 and r2.
	*data1 = Reg[r1];
	*data2 = Reg[r2];
}

/* Sign Extend */
void sign_extend(unsigned offset,unsigned *extended_value)
{
	//If offset ends with a negative bit, it will maintain it and initialize to 
	//extended_value. Else, it will be positive and initialize to extended_value.
	*extended_value = ((offset >> 15) == 1) ? (offset | 0xFFFF0000) : offset;
}

/* ALU operations */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    if(ALUSrc == 1)
        data2 = extended_value;

    if(ALUOp == 7)
    {
        switch(funct)
        {
            //Add
            case 0x20:
                ALUOp = 0;
                break;

            //Subtract
            case 0x22:
                ALUOp = 1;
                break;

            //Less than signed
            case 0x2A:
                ALUOp = 2;
                break;

            //Less than unsigned
            case 0x2B:
                ALUOp = 3;
                break;

            //And
            case 0x24:
                ALUOp = 4;
                break;

            //Or
            case 0x25:
                ALUOp = 5;
                break;

            //Halt since none of the conditions were met.
            default:
                return 1;
            }

            ALU(data1,data2, ALUOp,ALUresult,Zero);
        }

        else
        {
            ALU(data1,data2,ALUOp,ALUresult,Zero);
        }


    return 0;
}

/* Read / Write Memory */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	//Halt occurs if both MemWrite and MemRead are true and
    //sad address since the word is not aligned.
    if ((MemWrite || MemRead) && (((ALUresult % 4) != 0)))
        return 1;

	if(MemWrite == 1)
	{
		Mem[ALUresult >> 2] = data2;
	}

	if(MemRead == 1)
	{
		*memdata = Mem[ALUresult >> 2];
	}

	//If none of the conditions were met, a halt occurs.
	return 0;
}

/* Write Register */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	//Since RegWrite and MemtoReg are 1, the data will be coming from memory.
	if(RegWrite == 1 && MemtoReg == 1)
	{
		//If Regst is true, the memdata will go to Reg[r3], otherwhise,
		//it will go to Reg[r2]
		if(RegDst == 1)
			Reg[r3] = memdata;
		else
			Reg[r2] = memdata;
	}

	//Since only Regwrite is 1, the data is coming from ALUresult.
	else if(RegWrite == 1 && MemtoReg == 0)
	{
		//If Regst is true, the ALUresult will go to Reg[r3], otherwhise,
		//it will go to Reg[r2]
		if(RegDst == 1)
			Reg[r3] = ALUresult;
		else
			Reg[r2] = ALUresult;
	}
}

/* PC update */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
	*PC = *PC + 4;

	//Determines if the branch is taken, if so PC + offset.
	if(Branch == 1 && Zero == 1)
	{
		*PC = *PC + (extended_value << 2); 
	}

	//If Jump taken, it will left shift bits of jsec by 2 and use upper 4 bits of PC
	else if(Jump == 1)
	{
		*PC = (jsec << 2) + (*PC & 0xF0000000);
	}
}