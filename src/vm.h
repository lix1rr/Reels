#ifndef VM_H
#define VM_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>


enum {
	VM_MEM_SIZE = (256 * 256)
};

typedef uint32_t word_t;

// Should be heap allocated
struct VM {
	int alts;

	word_t a;
	word_t b;
	word_t c;
			 
	word_t x;
	word_t y;
	word_t z;

	char* pc;
	word_t sp;

	word_t stack[VM_MEM_SIZE];
	
	// The code section in memory
	// code_end refers to one past the code section
	char* code_begin;
	char* code_end;
};

struct StdFunction {
	const char* name;
	const char* desc;
	void(*func)(struct VM* vm);
};

// Initialises registers of VM
void VM_init(struct VM*);

// Prints out registers
void VM_show_debug_info(const struct VM* vm);

// Loads null terminated string pointed to by `str` into memory address `offset`
void VM_load_string(struct VM* vm, word_t offset, const char* str);

// Runs the VM
bool VM_interpret(struct VM* vm);

#endif