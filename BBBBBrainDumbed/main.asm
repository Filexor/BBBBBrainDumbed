define sprdata 0xE040
macro VCPYNTSCSPR2
cli clj op1 A ldi.16 sprdata
op1 B ldi.16 0xE040
op1 D op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
op2 A ldri.16 op2 B stri.16
endmacro
macro VCPYNTSCALL2
cli clj op1 E clr
cli op1 A ldi.4 0 wait.4 nop
loop:
op1 A op2 E mov.16 cli op1 B
ldi.1 1 op1 A op2 B shr cli op1 B
ldi.4 7 op1 A shl op1 F 
ldi.16 pfdata op2 A clc adc.16 op1 B
ldi.16 0xE000 op1 D
op2 F ldri.16 op2 B stri.16
op2 F ldri.16 op2 B stri.16
op2 F ldri.16 op2 B stri.16
op2 F ldri.16 op2 B stri.16
VCPYNTSCSPR2
cli op1 A ldi.4 0 wait.4 nop
op1 B ldi.16 0xE000 op1 D 
op2 F ldri.16 op2 B stri.16
op2 F ldri.16 op2 B stri.16
op2 F ldri.16 op2 B stri.16
op2 F ldri.16 op2 B stri.16
inc.16 E op1 B ldi.16 242 clc 
op2 E sbb.16 op1 A ldi.16 loop
op1 B op2 A bz
cli op1 A ldi.4 0 wait.4 nop
endmacro
VCPYNTSCALL2
pfdata: