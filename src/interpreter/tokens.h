#ifndef NESJS_INTERPRETER_TOKENS_H
#define NESJS_INTERPRETER_TOKENS_H

#define T_EOF 0
#define T_NL 1
#define T_NUM 0x80
#define T_STR 0x81
#define T_ID 0x82
#define T_EQ 0x83
#define T_NE 0x84
#define T_LE 0x85
#define T_GE 0x86
#define T_LAND 0x87
#define T_LOR 0x88
#define T_INC 0x89
#define T_DEC 0x8A
#define T_ADDEQ 0x8B
#define T_SUBEQ 0x8C
#define T_MULEQ 0x8D
#define T_DIVEQ 0x8E
#define T_MODEQ 0x8F
#define T_SHL 0x90
#define T_SHR 0x91
#define T_LET 0xA0
#define T_IF 0xA1
#define T_ELSE 0xA2
#define T_WHILE 0xA3
#define T_FOR 0xA4
#define T_FUNC 0xA5
#define T_RETURN 0xA6
#define T_BREAK 0xA7
#define T_CONT 0xA8
#define T_TRUE 0xA9
#define T_FALSE 0xAA
#define T_NULL 0xAB
#define T_OF 0xAC
#define T_LENGTH 0xB0
#define T_PUSH 0xB1
#define T_POP 0xB2

#define V_NUM 0
#define V_STR 1
#define V_BOOL 2
#define V_UNDEF 3
#define V_ARR 4
#define V_NONE 5

#define C_NONE 0
#define C_BREAK 1
#define C_CONT 2
#define C_RET 3

#endif
