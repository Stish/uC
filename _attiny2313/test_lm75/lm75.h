#ifndef LM75_H
#define LM75_H

//definition of the adress
#define LM75_0	0x90
#define LM75_1	0x92
#define LM75_2	0x94
#define LM75_3	0x96
#define LM75_4	0x98
#define LM75_5	0x9A
#define LM75_6	0x9C
#define LM75_7	0x9F

//definition of the pointer register
#define Temp_Reg	0x0
#define Conf_Reg	0x1
#define Thyst		0x2
#define	Tos			0x3

//definition of the configuration register
#define shutdown	0
#define Comparator  1
#define Interrupt   1
#define os			2
#define fault0		3
#define fault1		4

#endif