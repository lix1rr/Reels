# Reels
Reels is a simple to use [estoric programming language](https://en.wikipedia.org/wiki/Esoteric_programming_language) written in C. It loads programs into memory 

Registers: a, b, c, x, y, z, all 32 bits

```
 >    | increments stack pointer
      | alt: increments by *stack_pointer
 <    | decremenets stack pointer
      | alt: decrements by *stack_pointer
 ~    | does nothing
 ]    | if (a > 0), go to the next ~
 [     | if (a > 0), go to the previous ~
 -    | a = b - c
 /    | a = b / c
 !    | a = !b
 &    | a = a & b
 %    | aborts the program
      | alt: quits the program with exit code *stack_pointer
 $    | the next instruction is in alt mode
 #    | call system function

  where `a` is a lowercase letter of a register, and `A` is a capital letter of a register:
 a    | *stack_pointer = a
 A    | a = *stack_pointer

 (number) | *stack_pointer = number
          | number is decimal, and is smaller than the word size
          | alt: floating point number

 ```