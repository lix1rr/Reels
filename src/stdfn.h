#ifndef STDFN_H
#define STDFN_H
#include "vm.h"
void stdfn_print(struct VM* vm);
void stdfn_eprint(struct VM* vm);
void stdfn_read(struct VM* vm);
extern struct StdFunction stdfns[3];
#endif