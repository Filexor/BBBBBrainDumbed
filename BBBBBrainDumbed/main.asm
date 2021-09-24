cli op1 A
ldi.4 0 ldi.4 0 ldi.4 0 ldi.4 8
op1 B
ldi.4 0 ldi.4 0 ldi.4 0 ldi.4 0xc
op1 D
ldi.4 0 ldi.4 2 ldi.4 0xf ldi.4 8
op1 F
ldi.16 loop op1 E
loop:
op2 A ldri.16 op2 B str.16
op2 A mov.16 op2 D clc sbb.16
op2 F bn