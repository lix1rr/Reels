#include <stdbool.h>
#include "vm.h"
#include "stdfn.h"

struct StdFunction stdfns[3] = {
	/*
	template:
	args_count excludes varargs, and is negative if the last paramter is varargs
	{
	  "function_name",
	  "desciption",
	  args_count, permission_level, returns, fn_ptr
	},

	*/
	{
	  "void print(size, ... string)",
	  "Prints `string` to stdout",
	  stdfn_print
	},
	{
	  "void eprint(size, ... string)",
	  "Prints `string` to stdout",
	  stdfn_eprint
	},
	{
	  "bytesRead read(size, ... string)",
	  "Write `size` characters from stdout into `string`. If there is an EOF, it will return bytes read",
	  stdfn_read
	}
	
};

void print_to_file(struct VM* vm, FILE* file) {
	if (vm->sp + 1 >= VM_MEM_SIZE) {
		printf("Argument 1 to `print` is out of bounds");
		return;
	}
	word_t size = vm->stack[vm->sp + 1];
	if (vm->sp + 1 + size >= VM_MEM_SIZE) {
		printf("The string in the varargs of `print` is out of bounds");
		return;
	}
	for (int i = 0; i < size; i++) {
		word_t word = vm->stack[i + 2 + vm->sp];
		char c = (char) (word & 0xFF);
		fputc(c, file);
	}
}

void stdfn_print(struct VM* vm) {
	print_to_file(vm, stdout);
}

void stdfn_eprint(struct VM* vm) {
	print_to_file(vm, stderr);
}

void stdfn_read(struct VM* vm) {

}