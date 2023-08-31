class DInst<bits<32> op, dag outs, dag ins, string asmstr>
    : Instruction {
  bits<32> Opcode = op;

  let Namespace = "DL";
  let DecoderNamespace = "Valida";

  // First word contains the opcode
  dag Inst = (ascend);

  dag OutOperandList = outs;
  dag InOperandList = ins;

  let AsmString = asmstr;
}

class DPseudoInst<bits<32> op, dag outs, dag ins, string asmstr, list<dag> pattern>
    : DInst<op, outs, ins, asmstr> {
  let isPseudo = 1;
  let Pattern = pattern;
}

