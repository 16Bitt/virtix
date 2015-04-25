#ifndef SETJMP_H
#define SETJMP_H

typedef struct{
	void* ptr[5];
} jmp_buf_t;

typedef struct jmp_buf_t* jmp_buf;

/*
typedef struct{
	void* env[4];
} jmp_buf;
*/

#define setjmp(env) __builtin_setjmp((void*) (env))
#define longjmp(env, value) __builtin_longjmp((void*) (env), (value))

#endif
