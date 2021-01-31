#include "vm.h"
#include "signal.h"
#include "resources.h"

struct String {
	char* ptr;
	size_t capacity;
	size_t size;
};

// Not buffered :(
// Reads until a newline or EOF, and returns if it succeeded
// gotta free .ptr if it aint null
bool getLine(struct String* str) {
	if (str->capacity == 0) {
		str->capacity = 64;
		str->ptr = (char*)malloc(str->capacity);
		str->size = 0;
		if (str->ptr == NULL) {
			return str;
		}
	}
	
	while (true) {
		int c = fgetc(stdin);
		if (c == EOF) {
			return false;
		}
		else if (c == '\n') {
			return true;
		}
		if (str->size == str->capacity) {
			str->capacity *= 2;
			str->ptr = (char*) realloc((void*) str->ptr, str->capacity);
			if (str->ptr == NULL) {
				str->size = 0;
				return str;
			}
		}
		str->ptr[str->size] = (char) c;
		str->size++;
	}
}

void repl() {
	struct VM* vm = (struct VM*)malloc(sizeof(struct VM));

	struct String line;
	line.ptr = (char*)malloc(4);
	line.capacity = 4;
	line.ptr[0] = '~';
	line.size = 1;
	printf(" :: ");
	while (getLine(&line)) {
		if (line.size > 0) {
			VM_init(vm);
			if (line.size + 2 > line.capacity) {
				line.capacity *= 2;
				line.ptr = (char*)realloc((void*)line.ptr, line.capacity);
			}
			line.ptr[line.size++] = '~';
			line.ptr[line.size++] = '%';
			vm->code_begin = line.ptr;
			vm->code_end = line.ptr + line.size;
			if (VM_interpret(vm)) {
				VM_show_debug_info(vm);
			};
		}
		line.ptr[0] = '~';
		line.size = 1;
		printf("\n :: ");
	}
	free(vm);
}

int main(int argc, char** argv) {
	repl();
	const char* programName = argv[0];
	for (char* i = programName; *i != 0; i++) {
		if (*i == '\\' || *i == '/') {
			programName = i + 1;
		}
	}
	if (programName[0] == 0) {
		programName = "<program name>";
	}
	
	if (argc == 1) {
		printf(RESOURCE_DATA_help_txt, programName);
		printf("At least one parameter is needed, use \"%s help\" for more info\n", programName);
		return EXIT_FAILURE;
	}
	
	if (strcmp("help", argv[1]) == 0) {
		printf(RESOURCE_DATA_help_txt, programName);
		return EXIT_SUCCESS;
	}
	if (strcmp("repl", argv[1]) == 0) {
		repl();
		return EXIT_SUCCESS;
	}
	if (strcmp("moo", argv[1]) == 0) {
		fwrite(RESOURCE_DATA_woof_txt, 1, RESOURCE_SIZE_woof_txt, stdout);
		return EXIT_SUCCESS;
	}
	if (strcmp("run", argv[1]) == 0) {

	}
		
}