# Valida Compiler

### Overview
This repository contains the compiler for the [Valida zkVM](https://github.com/valida-xyz/valida), a STARK-based VM designed to efficiently prove the execution of programs written in conventional languages. A work-in-progress spec for the VM ISA is described in the GitHub issue [here](https://github.com/valida-xyz/valida-compiler/issues/2). The code implementing the compiler can be found in the `llvm/lib/Target/Valida` directory.

### Building the compiler
These are the steps to build the Valida backend compiler (`llc`), which translates LLVM IR to Valida Assembly:

1. `mkdir build`
2. `cd build`
3. `cmake -G "Ninja" -DLLVM_TARGETS_TO_BUILD="" -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="Valida" ../llvm`
4. `ninja llc`

### Compilation example

In this section we'll show how to generate Valida Assembly from the following example C program:

```c
// fib.c

int fib(int n) {
    int a = 0;
    int b = 1;
    for (int i = 0; i != n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return a;
}

int main() {
    int n = 10;
    int result = fib(n);
    return result;
}
```

1. Compile the program to LLVM IR using Clang, with a generic 32-bit backend as our target:

`clang fib.c --target=riscv32 -S -emit-llvm`

2. Generate the assembly:

`./build/bin/llc fib.ll --march=valida --global-isel`

```asm
; fib.s

...
fib:                                    ; @fib
; %bb.0:
	sw	-4(fp), 12(fp)
	imm32	-8(fp), 0, 0, 0, 0
	imm32	-12(fp), 0, 0, 0, 1
	imm32	-16(fp), 0, 0, 0, 0
	beq	.LBB0_1, 0(fp), 0(fp)
.LBB0_1:
	bne	.LBB0_2, -16(fp), -4(fp)
	beq	.LBB0_4, 0(fp), 0(fp)
; %bb.2:
	add	-20(fp), -8(fp), -12(fp)
	sw	-8(fp), -12(fp)
	sw	-12(fp), -20(fp)
	beq	.LBB0_3, 0(fp), 0(fp)
; %bb.3:
	addi	-16(fp), -16(fp), 1
	beq	.LBB0_1, 0(fp), 0(fp)
.LBB0_4:
	sw	4(fp), -8(fp)
	jalv	-4(fp), 0(fp), 8(fp)
...
main:                                   ; @main
; %bb.0:
	imm32	-4(fp), 0, 0, 0, 0
	imm32	-8(fp), 0, 0, 0, 10
	sw	-16(fp), -8(fp)
	imm32	-20(fp), 0, 0, 0, 28
	jal	-28(fp), fib, -28
	sw	-12(fp), -24(fp)
	sw	4(fp), -12(fp)
	jalv	-4(fp), 0(fp), 8(fp)
...
```
