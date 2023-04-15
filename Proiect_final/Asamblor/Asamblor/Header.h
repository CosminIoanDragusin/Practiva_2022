#pragma once
#pragma once
#include <stdio.h>
#include <stdlib.h>



namespace Asamblor
{
	using SByte = char;
	using Byte = unsigned char;
	using Word = unsigned short;

	using u32 = unsigned int;
	using s32 = signed int;

	struct Mem;
	struct CPU;
	struct StatusFlags;
}

struct Asamblor::Mem
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	void Initialise()
	{
		for (u32 i = 0; i < MAX_MEM; i++)
		{
			Data[i] = 0;
		}
	}

	/** read 1 byte */
	Byte operator[](u32 Address) const
	{
		// assert here Address is < MAX_MEM
		return Data[Address];
	}

	/** write 1 byte */
	Byte& operator[](u32 Address)
	{
		// assert here Address is < MAX_MEM
		return Data[Address];
	}
};

struct Asamblor::StatusFlags
{
	Byte C : 1;	//0: Carry Flag	
	Byte Z : 1;	//1: Zero Flag
	Byte S : 1; //3: Clear Flag
	Byte V : 1; //6: Overflow
	Byte N : 1; //7: Negative
};

struct Asamblor::CPU
{
	Word PC;		//program counter
	Byte SP;		//stack pointer

	Byte R1, R2, R3, R4, R5, R6; //registers

	union // processor status
	{
		Byte PS;
		StatusFlags Flag;
	};

	void Reset(Mem& memory)
	{
		Reset(0xFFFC, memory);
	}

	void Reset(Word ResetVector, Mem& memory)
	{
		PC = ResetVector;
		SP = 0xFF;
		Flag.C = Flag.Z = Flag.V = 0;
		R1 = R2 = R3 = R4 = R5 = R6 = 0;
		memory.Initialise();
	}

	Byte FetchByte(s32& Ticks, const Mem& memory)
	{
		Byte Data = memory[PC];
		PC++;
		Ticks--;
		return Data;
	}

	SByte FetchSByte(s32& Ticks, const Mem& memory)
	{
		return FetchByte(Ticks, memory);
	}

	Word FetchWord(s32& Ticks, const Mem& memory)
	{
		Word Data = memory[PC];
		PC++;

		Data |= (memory[PC] << 8);
		PC++;

		Ticks -= 2;
		return Data;
	}

	Byte ReadByte(
		s32& Ticks,
		Word Address,
		const Mem& memory)
	{
		Byte Data = memory[Address];
		Ticks--;
		return Data;
	}

	Word ReadWord(
		s32& Ticks,
		Word Address,
		const Mem& memory)
	{
		Byte LoByte = ReadByte(Ticks, Address, memory);
		Byte HiByte = ReadByte(Ticks, Address + 1, memory);
		return LoByte | (HiByte << 8);
	}

	/** write 1 byte to memory */
	void WriteByte(Byte Value, s32& Ticks, Word Address, Mem& memory)
	{
		memory[Address] = Value;
		Ticks--;
	}

	/** write 2 bytes to memory */
	void WriteWord(Word Value, s32& Ticks, Word Address, Mem& memory)
	{
		memory[Address] = Value & 0xFF;
		memory[Address + 1] = (Value >> 8);
		Ticks -= 2;
	}

	/** @return the stack pointer as a full 16-bit address (in the 1st page) */
	Word SPToAddress() const
	{
		return 0x100 | SP;
	}

	void PushWordToStack(s32& Ticks, Mem& memory, Word Value)
	{
		WriteByte(Value >> 8, Ticks, SPToAddress(), memory);
		SP--;
		WriteByte(Value & 0xFF, Ticks, SPToAddress(), memory);
		SP--;
	}

	/** Push the PC-1 onto the stack */
	void PushPCMinusOneToStack(s32& Ticks, Mem& memory)
	{
		PushWordToStack(Ticks, memory, PC - 1);
	}

	/** Push the PC+1 onto the stack */
	void PushPCPlusOneToStack(s32& Ticks, Mem& memory)
	{
		PushWordToStack(Ticks, memory, PC + 1);
	}

	/** Push the PC onto the stack */
	void PushPCToStack(s32& Ticks, Mem& memory)
	{
		PushWordToStack(Ticks, memory, PC);
	}

	void PushByteOntoStack(s32& Ticks, Byte Value, Mem& memory)
	{
		const Word SPWord = SPToAddress();
		memory[SPWord] = Value;
		Ticks--;
		SP--;
		Ticks--;
	}

	Byte PopByteFromStack(s32& Ticks, Mem& memory)
	{
		SP++;
		Ticks--;
		const Word SPWord = SPToAddress();
		Byte Value = memory[SPWord];
		Ticks--;
		return Value;
	}

	/** Pop a 16-bit value from the stack */
	Word PopWordFromStack(s32& Ticks, Mem& memory)
	{
		Word ValueFromStack = ReadWord(Ticks, SPToAddress() + 1, memory);
		SP += 2;
		Ticks--;
		return ValueFromStack;
	}

	// Process status bits
	static constexpr Byte
		NegativeFlagBit = 0b10000000,
		OverflowFlagBit = 0b01000000,
		BreakFlagBit = 0b000010000,
		UnusedFlagBit = 0b000100000,
		InterruptDisableFlagBit = 0b000000100,
		ZeroBit = 0b00000001;

	// opcodes
	static constexpr Byte
		LDA_IMEDIAT = 0xA9,//
		LDA_ZP = 0xA5,//
		MOV = 0x08,
		ADD = 0x09,//
		SUB = 0x10,//
		CMP = 0x11, //
		OR = 0x12,//
		AND = 0x13,//
		XOR = 0x14,
		//alt format
		CLR = 0x300,
		NEG = 0x301,
		INC = 0x302, //
		DEC = 0x303, //
		ASR = 0x304,
		ASL_ins = 0x305,//
		LSR_ins = 0x306,//
		ROL_ins = 0x307,//
		//INS_ROR = 0x308,//
		INS_ROR = 0x30E,//
		RLC = 0x309,
		RRC = 0x30A,
		JMP = 0x30B,//
		PUSH = 0x30C,
		POP = 0x30D,
		//alt format
		BR = 0xE0,//
		BNE = 0xE1,//
		BEQ = 0xE2,//
		BPL = 0xE3,//
		BMI = 0xE4,//
		BCS = 0xE5,//
		BCC = 0xE6,//
		BVS = 0xE7,//
		BVC = 0xE8,//
		//alt format
		CLC = 0x1F88,//
		CLV = 0x1F89,//
		CLZ = 0x1F8A,//
		CLS = 0x1F8B,//
		CCC = 0x1F8C, //
		SEC = 0x1F8D,//
		SEV = 0x1F8E,//
		SEZ = 0x1F8F,//
		SES = 0x1F90,//
		SCC = 0x1F91,//
		NOP = 0x1F92,//
		RET = 0x1F93,
		RETI = 0x1F94,
		HALT = 0x1F95,
		WAIT = 0x1F96,
		PUSH_PC = 0x1F97,
		POP_PC = 0x1F98,
		PUSH_FLAG = 0x1F99,
		POP_FLAG = 0x1F9A;

	/** Sets the correct Process status after a load register instruction
	*	- LDA, LDX, LDY
	*	@Register The A,X or Y Register */
	void SetZeroAndNegativeFlags(Byte Register)
	{
		Flag.Z = (Register == 0);
		Flag.N = (Register & NegativeFlagBit) > 0;
	}

	/** @return the address that the program was loading into, or 0 if no program */
	Word LoadPrg(const Byte* Program, u32 NumBytes, Mem& memory) const;

	/** printf the registers, program counter etc */
	void PrintStatus() const;

	/** @return the number of cycles that were used */
	s32 Execute(s32 Cycles, Mem& memory);

	/** Addressing mode - Zero page */
	Word AddrZeroPage(s32& Cycles, const Mem& memory);

	/** Addressing mode - Zero page with X offset */
	Word AddrZeroPageX(s32& Cycles, const Mem& memory);

	/** Addressing mode - Zero page with Y offset */
	Word AddrZeroPageY(s32& Cycles, const Mem& memory);

	/** Addressing mode - Absolute */
	Word AddrAbsolute(s32& Cycles, const Mem& memory);

	/** Addressing mode - Absolute with X offset */
	Word AddrAbsoluteX(s32& Cycles, const Mem& memory);

	/** Addressing mode - Absolute with X offset
	*	- Always takes a cycle for the X page boundary)
	*	- See "STA Absolute,X" */
	Word AddrAbsoluteX_5(s32& Cycles, const Mem& memory);

	/** Addressing mode - Absolute with Y offset */
	Word AddrAbsoluteY(s32& Cycles, const Mem& memory);

	/** Addressing mode - Absolute with Y offset
	*	- Always takes a cycle for the Y page boundary)
	*	- See "STA Absolute,Y" */
	Word AddrAbsoluteY_5(s32& Cycles, const Mem& memory);

	/** Addressing mode - Indirect X | Indexed Indirect */
	Word AddrIndirectX(s32& Cycles, const Mem& memory);

	/** Addressing mode - Indirect Y | Indirect Indexed */
	Word AddrIndirectY(s32& Cycles, const Mem& memory);

	/** Addressing mode - Indirect Y | Indirect Indexed
	*	- Always takes a cycle for the Y page boundary)
	*	- See "STA (Indirect,Y) */
	Word AddrIndirectY_6(s32& Cycles, const Mem& memory);
};