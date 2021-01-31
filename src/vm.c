#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include <signal.h>
#include "vm.h"
#include "stdfn.h"
	
void VM_init(struct VM* vm) {
	vm->a = 0;
	vm->b = 0;
	vm->c = 0;
	vm->x = 0;
	vm->y = 0;
	vm->z = 0;
	vm->pc = 0;
	vm->alts = 0;
	vm->sp = VM_MEM_SIZE - 1;
	vm->code_begin = 0;
	vm->code_end = 0;
}

void VM_show_debug_info(const struct VM* vm) {
	return;
	printf("\n"
		"a  = dec:%03i hex:%02x | x  = dec:%03i hex:%02x\n"
		"b  = dec:%03i hex:%02x | y  = dec:%03i hex:%02x\n"
		"c  = dec:%03i hex:%02x | z  = dec:%03i hex:%02x\n"
		"pc = dec:%03i hex:%02x | sp = dec:%03i hex:%02x\n",
		(int) vm->a, (int) vm->a, (int) vm->x, (int) vm->x,
		(int) vm->b, (int) vm->b, (int) vm->y, (int) vm->y,
		(int) vm->c, (int) vm->c, (int) vm->z, (int) vm->z,
		(int) (vm->pc - vm->code_begin), (int) (vm->pc - vm->code_begin), (int) vm->sp, (int)vm->sp
	);
	for (word_t i = 0; i < VM_MEM_SIZE - vm->sp; i++) {
		printf("%i: %i\n", (int) i, (int) vm->stack[VM_MEM_SIZE - 1 - i]);
	}
}


bool VM_interpret(struct VM* vm) {
	assert(vm != NULL);
	assert(vm->sp != 0);
	vm->pc = vm->code_begin;
	while (vm->pc < vm->code_end) {
		char op = *vm->pc;
		if (op >= '0' && op <= '9') {
			vm->a = op - '0';
		} else {
			switch (op) {
			case '(': {
				if (vm->alts > 0) {
					printf("Error, floating point literals not implemented\n");
					return false;
				}
				else {
					word_t num = 0;
					while (true) {
						vm->pc++;
						op = *vm->pc;
						if (vm->pc >= vm->code_end) {
							printf("Error while parsing number, reached end of code without terminating )\n");
							return false;
						}
						if (op == ')')
							break;
						else if (op >= '0' && op <= '9') {
							num *= 10;
							num += op - '0';
						} else {
							printf("Error while parsing number, illegal character '%c' (%i)\n", op, (int)op);
							return false;
						}
					}
					vm->stack[vm->sp] = num;
				}
				break;
			case '\'':
				if (vm->alts > 0) {
					// Byte literal
					if (vm->pc + 2 >= vm->code_end) {
						printf("No terminating ' found in byte literal\n");
						return false;
					}
					if (vm->pc[2] != '\'') {
						printf("The second letter after a byte literal must be a '\n");
						return false;
					}
					vm->stack[vm->sp] = vm->pc[1];
					vm->pc += 2;
				}
				else {
					vm->pc++;
					char* start = vm->pc;
					while (*vm->pc != '\'') {
						if (vm->pc >= vm->code_end) {
							printf("No terminating ' found in string literal\n");
							return false;
						}
						vm->pc++;
					}
					for (char* i = vm->pc; i >= start; i--) {
						vm->stack[--vm->sp] = *i;
					}
					vm->stack[--vm->sp] = vm->pc - start;
				}
				break;
			}
			case '>':
				vm->sp -= vm->alts > 0 ? vm->stack[vm->sp] : 1;
				if (vm->sp >= VM_MEM_SIZE) {
					printf("The stack pointer is out of bounds\n");
				}
				break;
			case '<':
				vm->sp += vm->alts > 0 ? vm->stack[vm->sp] : 1;
				if (vm->sp >= VM_MEM_SIZE) {
					printf("The stack pointer is out of bounds\n");
				}
				break;
			case ']':
				if (vm->a > 0) {
					do {
						vm->pc++;
					} while (*vm->pc != '~');
				}
				break;
			case '[':
				if (vm->a > 0) {
					do {
						vm->pc--;
					} while (*vm->pc != '~');
				}
				break;
			case '#': {
				word_t idx = vm->stack[vm->sp];
				if (idx > sizeof(stdfns) / sizeof(stdfns[0])) {
					printf("Trying to reference an unknown standard function index %i\n", (int)idx);
					return false;
				}
				stdfns[idx].func(vm);
			}
				break;
			case '$':
				vm->alts += 2;
				break;
			case '+':
				if (vm->alts > 0) {
					*((float*)&vm->a) = *((float*)&vm->b) + *((float*)&vm->c);
				}
				else {
					vm->a = vm->b + vm->c;
				}
				break;
			case '-':
				if (vm->alts > 0) {
					*((float*)&vm->a) = *((float*)&vm->b) + *((float*)&vm->c);
				} else {
					vm->a = vm->b - vm->c;
				}
				break;
			case '*':
				if (vm->alts > 0) {
					*((float*)&vm->a) = *((float*)&vm->b) + *((float*)&vm->c);
				} else {
					vm->a = vm->b * vm->c;
				}
				break;
			case '/':
				if (vm->alts > 0) {
					*((float*)&vm->a) = *((float*)&vm->b) + *((float*)&vm->c);
				} else {
					vm->a = vm->b / vm->c;
				}
				break;
			case '!':
				vm->a = !vm->b;
				break;
			case '&':
				vm->a = vm->b & vm->c;
				break;
			case '%':
				return true;
			case '~':
				break;
			case 'a':
				vm->stack[vm->sp] = vm->a;
				break;
			case 'b':
				vm->stack[vm->sp] = vm->b;
				break;
			case 'c':
				vm->stack[vm->sp] = vm->c;
				break;
			case 'x':
				vm->stack[vm->sp] = vm->x;
				break;
			case 'y':
				vm->stack[vm->sp] = vm->y;
				break;
			case 'z':
				vm->stack[vm->sp] = vm->z;
				break;
			case 'A':
				vm->a = vm->stack[vm->sp];
				break;
			case 'B':
				vm->b = vm->stack[vm->sp];
				break;
			case 'C':
				vm->c = vm->stack[vm->sp];
				break;
			case 'X':
				vm->x = vm->stack[vm->sp];
				break;
			case 'Y':
				vm->y = vm->stack[vm->sp];
				break;
			case 'Z':
				vm->z = vm->stack[vm->sp];
				break;
			default:
				printf("Illegal character '%c' (%i)\n", op, (int)op);
				return false;
			}
		}
		vm->pc++;
		if (vm->alts > 0) vm->alts--;
	}
	return true;
}