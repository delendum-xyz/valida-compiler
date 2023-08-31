include "ValidaInstrFormats.td"

//===----------------------------------------------------------------------===//
// Nodes
//===----------------------------------------------------------------------===//

def SDT_DCallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i32> ]>;

def SDT_DCallSeqEnd : SDCallSeqEnd<[
  SDTCisVT<0, i32>, SDTCisVT<1, i32>
]>;

def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_DCallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END",   SDT_DCallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

//===----------------------------------------------------------------------===//
// Operand Types
//===----------------------------------------------------------------------===//

def FPOp32 : Operand<i32> {
  let PrintMethod = "printFPOp32";
  let DecoderMethod = "DecodeFPOp32";
}

def ImmOp32 : Operand<i32> {
  let PrintMethod = "printImmOp32";
  let DecoderMethod = "DecodeImmOp32";
}

// Node immediate fits as 32-bit sign extended on target immediate.
def ImmSExt32  : ImmLeaf<i32, [{ return isInt<32>(Imm); }]>;

//===----------------------------------------------------------------------===//
// Patterns
//===----------------------------------------------------------------------===//

class AlignedLoad<PatFrag Node> :
  PatFrag<(ops node:$ptr), (Node node:$ptr), [{
  LoadSDNode *LD = cast<LoadSDNode>(N);
  return LD->getMemoryVT().getSizeInBits()/8 <= 4;
}]>;

class AlignedStore<PatFrag Node> :
  PatFrag<(ops node:$val, node:$ptr), (Node node:$val, node:$ptr), [{
  StoreSDNode *SD = cast<StoreSDNode>(N);
  return SD->getMemoryVT().getSizeInBits()/8 <= 4;
}]>;

//===----------------------------------------------------------------------===//
// Instruction Class Templates
//===----------------------------------------------------------------------===//

class ALU_rr<bits<32> op, string opcodestr>
  : DInst<op, (outs), (ins FPOp32:$c, FPOp32:$a, FPOp32:$b),
         opcodestr#"\t$c, $a, $b"> {
  let Inst = (ascend op,
    (operand "$c", 32),
    (operand "$a", 32),
    (operand "$b", 32));
}

class ALU_ri<bits<32> op, string opcodestr>
  : DInst<op, (outs), (ins FPOp32:$c, FPOp32:$a, ImmOp32:$b),
         opcodestr#"\t$c, $a, $b"> {
  let Inst = (ascend op,
    (operand "$c", 32),
    (operand "$a", 32),
    (operand "$b", 32),
    0b0,
    0b1);
}

class LOAD<bits<32> op, string opcodestr>
  : DInst<op, (outs), (ins FPOp32:$c, FPOp32:$a),
         opcodestr#"\t$c, $a"> {
  let Inst = (ascend op,
    (operand "$c", 32),
    (operand "$a", 32));
}

class STORE<bits<32> op, string opcodestr>
  : DInst<op, (outs), (ins FPOp32:$a, FPOp32:$b),
         opcodestr#"\t$a, $b"> {
  let Inst = (ascend op,
    (operand "$a", 32),
    (operand "$b", 32));
}

class WRITE_IMM<bits<32> op, string opcodestr>
  : DInst<op, (outs), (ins FPOp32:$c, ImmOp32:$a, 
                           ImmOp32:$b, ImmOp32:$d,
                           ImmOp32:$e),
         opcodestr#"\t$c, $a, $b, $d, $e"> {
  let Inst = (ascend op,
    (operand "$c", 32),
    (operand "$a", 32),
    (operand "$b", 32),
    (operand "$d", 32),
    (operand "$e", 32));
}

class JUMP1<bits<32> op, string opcodestr>
  : DInst<op, (outs), (ins FPOp32:$c, FPOp32:$a, FPOp32:$b),
         opcodestr#"\t$c, $a, $b"> {
  let Inst = (ascend op,
    (operand "$c", 32),
    (operand "$a", 32),
    (operand "$b", 32));
  let isCall = 1;
}

class JUMP2<bits<32> op, string opcodestr>
  : DInst<op, (outs), (ins FPOp32:$c, ImmOp32:$a, ImmOp32:$b),
         opcodestr#"\t$c, $a, $b"> {
  let Inst = (ascend op,
    (operand "$c", 32),
    (operand "$a", 32),
    (operand "$b", 32));
  let isCall = 1;
}

class JUMP3<bits<32> op, string opcodestr>
  : DInst<op, (outs), (ins ImmOp32:$c, FPOp32:$a, FPOp32:$b),
         opcodestr#"\t$c, $a, $b"> {
  let Inst = (ascend op,
    (operand "$c", 32),
    (operand "$a", 32),
    (operand "$b", 32));
}

//===----------------------------------------------------------------------===//
// Instructions
//===----------------------------------------------------------------------===//

// Load and store instructions
def LOAD32 : LOAD<1, "lw">;
def STORE32 : STORE<2, "sw">;

// Immediate value writes
def IMM32 : WRITE_IMM<3, "imm32">;

// Control flow
def JAL  : JUMP2<4, "jal">;
def JALV : JUMP1<5, "jalv">;
def BEQ  : JUMP3<6, "beq">;
def BNE  : JUMP3<7, "bne">;

// Arithmetic (32-bit)
def ADD : ALU_rr<8, "add">;
def SUB : ALU_rr<9, "sub">;
def MUL : ALU_rr<10, "mul">;

// Arithmetic (32-bit) immediate
def ADDi : ALU_ri<11, "addi">;
def SUBi : ALU_ri<12, "subi">;
def MULi : ALU_ri<13, "muli">;

def RET : DInst<14, (outs), (ins), "ret">;

//===----------------------------------------------------------------------===//
// Pseudo Instructions
//===----------------------------------------------------------------------===//

// Pseudoinstruction for JAL
def CALL : DPseudoInst<15,
    (outs), 
    (ins ImmOp32:$a, ImmOp32:$b, ImmOp32:$c),
    "call", []>;

def ADJCALLSTACKDOWN : DPseudoInst<1, (outs), (ins ImmOp32:$amt1, ImmOp32:$amt2),
                                   "# ADJCALLSTACKDOWN $amt1",
                                   [(callseq_start timm:$amt1, timm:$amt2)]>;
def ADJCALLSTACKUP   : DPseudoInst<2, (outs), (ins ImmOp32:$amt1, ImmOp32:$amt2),
                                   "# ADJCALLSTACKUP $amt1",
                                   [(callseq_end timm:$amt1, timm:$amt2)]>;
