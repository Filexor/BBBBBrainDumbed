macro testmacro
cli clj op1 A
ldi.4 0 ldi.4 0 ldi.4 0 ldi.4 8
op1 B
ldi.4 0 ldi.4 2 ldi.4 0 ldi.4 0xC
op1 E clr ldi.4 8 cli clc op1 D 
op2 A ldri.16 op2 D ldr.16
op2 B stri.16 
op2 A ldri.16 op2 D ldr.16
op2 B stri.16 
op2 A ldri.16 op2 D ldr.16
op2 B stri.16 
op2 A ldri.16 op2 D ldr.16
op2 B stri.16
op1 B ldi.4 8 ldi.4 2 cli op1 D 
op2 A ldri.16 op2 D ldr.16
op2 B stri.4 stri.4 
op1 B op2 E adc.16 op1 D
op2 A ldri.16 op2 D ldr.16
op2 B stri.4 stri.4 
op1 B op2 E adc.16 op1 D
op2 A ldri.16 op2 D ldr.16
op2 B stri.4 stri.4 
op1 B op2 E adc.16 op1 D
op2 A ldri.16 op2 D ldr.16
op2 B stri.4 stri.4 
op1 B op2 E adc.16 op1 D
op2 A ldri.16 op2 D ldr.16
op2 B stri.4 stri.1 stri.1
op2 A ldri.16 op2 D ldr.16
op2 B stri.4 stri.1 stri.1
op2 A ldri.16 op2 D ldr.16
op2 B stri.4 stri.1 stri.1
op2 A ldri.16 op2 D ldr.16
op2 B stri.4 stri.1 stri.1
nop
endmacro
testmacro