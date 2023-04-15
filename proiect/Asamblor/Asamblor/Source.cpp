#include "Header.h"

//#define ASSERT( Condition, Text ) { if ( !Condition ) { throw -1; } }

Asamblor::s32 Asamblor::CPU::Execute(s32 Cycles, Mem& memory)
{
	/** Load a Register with the value from the memory address */
	auto LoadRegister =
		[&Cycles, &memory, this]
	(Word Address, Byte& Register)
	{
		Register = ReadByte(Cycles, Address, memory);
		SetZeroAndNegativeFlags(Register);
	};

	/** And the A Register with the value from the memory address */
	auto And =
		[&Cycles, &memory, this]
	(Word Address)
	{
		R1 &= ReadByte(Cycles, Address, memory);
		SetZeroAndNegativeFlags(R1);
	};

	/** Or the A Register with the value from the memory address */
	auto Ora =
		[&Cycles, &memory, this]
	(Word Address)
	{
		R1 |= ReadByte(Cycles, Address, memory);
		SetZeroAndNegativeFlags(R1);
	};

	/** Eor the A Register with the value from the memory address */
	auto Eor =
		[&Cycles, &memory, this]
	(Word Address)
	{
		R1 ^= ReadByte(Cycles, Address, memory);
		SetZeroAndNegativeFlags(R1);
	};

	/* Conditional branch */
	auto BranchIf = [&Cycles, &memory, this]
	(bool Test, bool Expected)
	{
		SByte Offset = FetchSByte(Cycles, memory);
		if (Test == Expected)
		{
			const Word PCOld = PC;
			PC += Offset;
			Cycles--;

			const bool PageChanged = (PC >> 8) != (PCOld >> 8);
			if (PageChanged)
			{
				Cycles--;
			}
		}
	};

	/** Do add with carry given the the operand */
	auto ADC = [&Cycles, &memory, this]
	(Byte Operand)
	{
		
		const bool AreSignBitsTheSame =
			!((R1 ^ Operand) & NegativeFlagBit);
		Word Sum = R1;
		Sum += Operand;
		Sum += Flag.C;
		R1 = (Sum & 0xFF);
		SetZeroAndNegativeFlags(R1);
		Flag.C = Sum > 0xFF;
		Flag.V = AreSignBitsTheSame &&
			((R1 ^ Operand) & NegativeFlagBit);
	};

	/** Do subtract with carry given the the operand */
	auto SBC = [&ADC](Byte Operand)
	{
		ADC(~Operand);
	};

	/** Sets the processor status for a CMP/CPX/CPY instruction */
	auto RegisterCompare = [&Cycles, &memory, this]
	(Byte Operand, Byte RegisterValue)
	{
		Byte Temp = RegisterValue - Operand;
		Flag.N = (Temp & NegativeFlagBit) > 0;
		Flag.Z = RegisterValue == Operand;
		Flag.C = RegisterValue >= Operand;
	};

	/** Arithmetic shift left */
	auto ASL = [&Cycles, this](Byte Operand) -> Byte
	{
		Flag.C = (Operand & NegativeFlagBit) > 0;
		Byte Result = Operand << 1;
		SetZeroAndNegativeFlags(Result);
		Cycles--;
		return Result;
	};

	/** Logical shift right */
	auto LSR = [&Cycles, this](Byte Operand) -> Byte
	{
		Flag.C = (Operand & ZeroBit) > 0;
		Byte Result = Operand >> 1;
		SetZeroAndNegativeFlags(Result);
		Cycles--;
		return Result;
	};

	/** Rotate left */
	auto ROL = [&Cycles, this](Byte Operand) -> Byte
	{
		Byte NewBit0 = Flag.C ? ZeroBit : 0;
		Flag.C = (Operand & NegativeFlagBit) > 0;
		Operand = Operand << 1;
		Operand |= NewBit0;
		SetZeroAndNegativeFlags(Operand);
		Cycles--;
		return Operand;
	};

	/** Rotate right */
	auto ROR = [&Cycles, this](Byte Operand) -> Byte
	{
		bool OldBit0 = (Operand & ZeroBit) > 0;
		Operand = Operand >> 1;
		if (Flag.C)
		{
			Operand |= NegativeFlagBit;
		}
		Cycles--;
		Flag.C = OldBit0;
		SetZeroAndNegativeFlags(Operand);
		return Operand;
	};

	/** Push Processor status onto the stack
	*	Setting bits 4 & 5 on the stack */
	auto PushPSToStack = [&Cycles, &memory, this]()
	{
		Byte PSStack = PS | BreakFlagBit | UnusedFlagBit;
		PushByteOntoStack(Cycles, PSStack, memory);
	};

	

	const s32 CyclesRequested = Cycles;
	while (Cycles > 0)
	{
		Byte Ins = FetchByte(Cycles, memory);
		switch (Ins)
		{
		case AND:
		{
			R1 &= FetchByte(Cycles, memory);
			SetZeroAndNegativeFlags(R1);
		} break;
		case OR: {
			R1 |= FetchByte(Cycles, memory);
			SetZeroAndNegativeFlags(R1);
		}
		case ADD:
		{
			R1 += FetchByte(Cycles, memory);
			SetZeroAndNegativeFlags(R1);
		} break;
		case SUB:
		{
			R1 -= FetchByte(Cycles, memory);
			SetZeroAndNegativeFlags(R1);
		} 
		break;
		case MOV:
		{
			R1 += FetchByte(Cycles, memory);
			SetZeroAndNegativeFlags(R1);
		} break;
		
		case LDA_IMEDIAT:
		{
			R1 = FetchByte(Cycles, memory);
			SetZeroAndNegativeFlags(R1);
		} break;
		
		case LDA_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			LoadRegister(Address, R1);
		} break;
		
		case JMP:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			PC = Address;
		} break;
		
		case DEC:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			Value--;
			Cycles--;
			WriteByte(Value, Cycles, Address, memory);
			SetZeroAndNegativeFlags(Value);
		} break;
		
		case INC:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			Value++;
			Cycles--;
			WriteByte(Value, Cycles, Address, memory);
			SetZeroAndNegativeFlags(Value);
		} break;
		case BR:
		{
			BranchIf(Flag.Z || Flag.C || Flag.V || Flag.S, true);
		} break;
		case BEQ:
		{
			BranchIf(Flag.Z, true);
		} break;
		case BNE:
		{
			BranchIf(Flag.Z, false);
		} break;
		case BCS:
		{
			BranchIf(Flag.C, true);
		} break;
		case BCC:
		{
			BranchIf(Flag.C, false);
		} break;
		case BMI:
		{
			BranchIf(Flag.N, true);
		} break;
		case BPL:
		{
			BranchIf(Flag.N, false);
		} break;
		case BVC:
		{
			BranchIf(Flag.V, false);
		} break;
		case BVS:
		{
			BranchIf(Flag.V, true);
		} break;
		case CLC:
		{
			Flag.C = false;
			Cycles--;
		} break;
		case SEC:
		{
			Flag.C = true;
			Cycles--;
		} break;
		case CLZ:
		{
			Flag.Z = false;
		} break;
		case CLS:
		{
			Flag.S = false;
			Cycles--;
		} break;
		case CCC:
		{
			Flag.Z = false;
			Flag.V = false;
			Flag.S = false;
			Flag.C = false;
			Cycles--;
		} break;
		
		case CLV:
		{
			Flag.V = false;
			Cycles--;
		} break;
		case NOP:
		{
			Cycles--;
		} break;
		case SEV:
		{
			Flag.V = true;
		}
		case SEZ:
		{
			Flag.Z = true;
		}
		case SES:
		{
			Flag.S = true;
		}
		case SCC:
		{
			Flag.C = true;
		}
		
		case CMP:
		{
			Byte Operand = FetchByte(Cycles, memory);
			RegisterCompare(Operand, R1);
		} break;
		
		case ASL_ins:
		{
			R1 = ASL(R1);
		} break;
		
		case LSR_ins:
		{
			R1 = LSR(R1);
		} break;
		
		case ROL_ins:
		{
			R1 = ROL(R1);
		} break;
		
		case INS_ROR: {
			R1 = ROR(R1);
		}break;
		
		default:
		{
			printf("Instruction %d not handled\n", Ins);
			throw - 1;
		} break;
		}
	}

	const s32 NumCyclesUsed = CyclesRequested - Cycles;
	return NumCyclesUsed;
}


Asamblor::Word Asamblor::CPU::AddrZeroPage(s32& Cycles, const Mem& memory)
{
	Byte ZeroPageAddr = FetchByte(Cycles, memory);
	return ZeroPageAddr;
}

Asamblor::Word Asamblor::CPU::AddrZeroPageX(s32& Cycles, const Mem& memory)
{
	Byte ZeroPageAddr = FetchByte(Cycles, memory);
	ZeroPageAddr += R2;
	Cycles--;
	return ZeroPageAddr;
}

Asamblor::Word Asamblor::CPU::AddrZeroPageY(s32& Cycles, const Mem& memory)
{
	Byte ZeroPageAddr = FetchByte(Cycles, memory);
	ZeroPageAddr += R3;
	Cycles--;
	return ZeroPageAddr;
}

Asamblor::Word Asamblor::CPU::AddrAbsolute(s32& Cycles, const Mem& memory)
{
	Word AbsAddress = FetchWord(Cycles, memory);
	return AbsAddress;
}

Asamblor::Word Asamblor::CPU::AddrAbsoluteX(s32& Cycles, const Mem& memory)
{
	Word AbsAddress = FetchWord(Cycles, memory);
	Word AbsAddressX = AbsAddress + R2;
	const bool CrossedPageBoundary = (AbsAddress ^ AbsAddressX) >> 8;
	if (CrossedPageBoundary)
	{
		Cycles--;
	}

	return AbsAddressX;
}

Asamblor::Word Asamblor::CPU::AddrAbsoluteX_5(s32& Cycles, const Mem& memory)
{
	Word AbsAddress = FetchWord(Cycles, memory);
	Word AbsAddressX = AbsAddress + R2;
	Cycles--;
	return AbsAddressX;
}

Asamblor::Word Asamblor::CPU::AddrAbsoluteY(s32& Cycles, const Mem& memory)
{
	Word AbsAddress = FetchWord(Cycles, memory);
	Word AbsAddressY = AbsAddress + R3;
	const bool CrossedPageBoundary = (AbsAddress ^ AbsAddressY) >> 8;
	if (CrossedPageBoundary)
	{
		Cycles--;
	}

	return AbsAddressY;
}

Asamblor::Word Asamblor::CPU::AddrAbsoluteY_5(s32& Cycles, const Mem& memory)
{
	Word AbsAddress = FetchWord(Cycles, memory);
	Word AbsAddressY = AbsAddress + R3;
	Cycles--;
	return AbsAddressY;
}

Asamblor::Word Asamblor::CPU::AddrIndirectX(s32& Cycles, const Mem& memory)
{
	Byte ZPAddress = FetchByte(Cycles, memory);
	ZPAddress += R2;
	Cycles--;
	Word EffectiveAddr = ReadWord(Cycles, ZPAddress, memory);
	return EffectiveAddr;
}

Asamblor::Word Asamblor::CPU::AddrIndirectY(s32& Cycles, const Mem& memory)
{
	Byte ZPAddress = FetchByte(Cycles, memory);
	Word EffectiveAddr = ReadWord(Cycles, ZPAddress, memory);
	Word EffectiveAddrY = EffectiveAddr + R3;
	const bool CrossedPageBoundary = (EffectiveAddr ^ EffectiveAddrY) >> 8;
	if (CrossedPageBoundary)
	{
		Cycles--;
	}
	return EffectiveAddrY;
}

Asamblor::Word Asamblor::CPU::AddrIndirectY_6(s32& Cycles, const Mem& memory)
{
	Byte ZPAddress = FetchByte(Cycles, memory);
	Word EffectiveAddr = ReadWord(Cycles, ZPAddress, memory);
	Word EffectiveAddrY = EffectiveAddr + R3;
	Cycles--;
	return EffectiveAddrY;
}


Asamblor::Word Asamblor::CPU::LoadPrg(const Byte* Program, u32 NumBytes, Mem& memory) const
{
	Word LoadAddress = 0;
	if (Program && NumBytes > 2)
	{
		u32 At = 0;
		const Word Lo = Program[At++];
		const Word Hi = Program[At++] << 8;
		LoadAddress = Lo | Hi;
		for (Word i = LoadAddress; i < LoadAddress + NumBytes - 2; i++)
		{
			//TODO: mem copy?
			memory[i] = Program[At++];
		}
	}

	return LoadAddress;
}

void Asamblor::CPU::PrintStatus() const
{
	printf("R1: %d R2: %d R3: %d R4: %d R5: %d R6: %d\n", R1, R2, R3, R4 ,R5 , R6);
	printf("PC: %d SP: %d\n", PC, SP);
	printf("PS: %d\n", PS);
}