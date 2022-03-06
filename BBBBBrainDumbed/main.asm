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
VCPYNTSCSPR2 nop