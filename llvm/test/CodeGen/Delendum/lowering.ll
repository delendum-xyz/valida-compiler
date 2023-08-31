: NOTE: Why is the test failing? The RET instruction is present in the output 
; RUN: llc --global-isel --march=valida --stop-after=irtranslator < %s | FileCheck %s

; CHECK: name: f
; CHECK: RET
define void @f() {
  ret void
}

; NOTE: What's the proper way to test argument lowering into the stack?
; CHECK: RET
define void @f(i32 %a) {
  ret void
}
