#ifndef PC88_KEYCODES_H
#define PC88_KEYCODES_H

#define KEY88_NONE			0xFF
#define KEY88_EXT			0x08

#define KEY88_RETURN		0x17
#define KEY88_AT			0x20
#define KEY88_A				0x21
#define KEY88_B				0x22
#define KEY88_C				0x23
#define KEY88_D				0x24
#define KEY88_E				0x25
#define KEY88_F				0x26
#define KEY88_G				0x27
#define KEY88_H				0x30
#define KEY88_I				0x31
#define KEY88_J				0x32
#define KEY88_K				0x33
#define KEY88_L				0x34
#define KEY88_M				0x35
#define KEY88_N				0x36
#define KEY88_O				0x37
#define KEY88_P				0x40
#define KEY88_Q				0x41
#define KEY88_R				0x42
#define KEY88_S				0x43
#define KEY88_T				0x44
#define KEY88_U				0x45
#define KEY88_V				0x46
#define KEY88_W				0x47
#define KEY88_X				0x50
#define KEY88_Y				0x51
#define KEY88_Z				0x52
#define KEY88_BRL			0x53
#define KEY88_BACKSLASH		0x54
#define KEY88_BRR			0x55
#define KEY88_TILDE			0x56
#define KEY88_MINUS			0x57
#define KEY88_0				0x60
#define KEY88_1				0x61
#define KEY88_2				0x62
#define KEY88_3				0x63
#define KEY88_4				0x64
#define KEY88_5				0x65
#define KEY88_6				0x66
#define KEY88_7				0x67
#define KEY88_8				0x70
#define KEY88_9				0x71
#define KEY88_COLON			0x72
#define KEY88_SEMICOLON		0x73
#define KEY88_COMMA			0x74
#define KEY88_PERIOD		0x75
#define KEY88_SLASH			0x76
#define KEY88_UNDERSCORE	0x77
#define KEY88_HOMECLR		0x80
#define KEY88_UP			0x81
#define KEY88_RIGHT			0x82
#define KEY88_INSDEL		0x83
#define KEY88_GRPH			0x84
#define KEY88_KANA			0x85
#define KEY88_SHIFT			0x86
#define KEY88_CTRL			0x87
#define KEY88_STOP			0x90
#define KEY88_F1			0x91
#define KEY88_F2			0x92
#define KEY88_F3			0x93
#define KEY88_F4			0x94
#define KEY88_F5			0x95
#define KEY88_SPACE			0x96
#define KEY88_ESC			0x97
#define KEY88_TAB			0xA0
#define KEY88_DOWN			0xA1
#define KEY88_LEFT			0xA2
#define KEY88_HELP			0xA3
#define KEY88_COPY			0xA4
#define KEY88_CAPS			0xA7
#define KEY88_ROLLUP		0xB0
#define KEY88_ROLLDOWN		0xB1

#define KEY88_KP_0			0x00
#define KEY88_KP_1			0x01
#define KEY88_KP_2			0x02
#define KEY88_KP_3			0x03
#define KEY88_KP_4			0x04
#define KEY88_KP_5			0x05
#define KEY88_KP_6			0x06
#define KEY88_KP_7			0x07
#define KEY88_KP_8			0x10
#define KEY88_KP_9			0x11
#define KEY88_KP_MUL		0x12
#define KEY88_KP_PLUS		0x13
#define KEY88_KP_EQUAL		0x14
#define KEY88_KP_COMMA		0x15
#define KEY88_KP_PERIOD		0x16
#define KEY88_KP_MINUS		0xA5
#define KEY88_KP_DIV		0xA6

#define KEY88_F6			(0xC0 | KEY88_EXT)
#define KEY88_F7			(0xC1 | KEY88_EXT)
#define KEY88_F8			(0xC2 | KEY88_EXT)
#define KEY88_F9			(0xC3 | KEY88_EXT)
#define KEY88_F10			(0xC4 | KEY88_EXT)
#define KEY88_BACKSPACE		(0xC5 | KEY88_EXT)
#define KEY88_INS			(0xC6 | KEY88_EXT)
#define KEY88_DEL			(0xC7 | KEY88_EXT)
#define KEY88_HENKAN		(0xD0 | KEY88_EXT)
#define KEY88_KETTEI		(0xD1 | KEY88_EXT)
#define KEY88_PC			0xD2
#define KEY88_ZENKAKU		0xD3	// half-width
#define KEY88_RETURNL		(0xE0 | KEY88_EXT)
#define KEY88_RETURNR		(0xE1 | KEY88_EXT)
#define KEY88_SHIFTL		(0xE2 | KEY88_EXT)
#define KEY88_SHIFTR		(0xE3 | KEY88_EXT)

#endif // PC88_KEYCODES_H
