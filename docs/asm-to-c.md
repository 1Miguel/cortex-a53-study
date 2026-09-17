# ASM to C
Here's a quick lookup on how simple C control structure look like in asm:

``` C
void func(void) {

// this vars are stored in RAM
int a = 20;
int b = 10;
int c = 0;

  if (a < b) {
    c = a;
  } else {
    c = b;
  }
}
```

```asm

func:
  // since a, b, and c are stored in RAM we will load the
  // address (pointer) to r1-r3 via the "=var" syntax
  ldr r0, =a
  ldr r1, =b
  ldr r2, =c
  // load values of a and b, in C this is equivalent to:
  //  r3 = *(ptr_a);
  //  r4 = *(ptr_b);
  ldr r3, [r0]
  ldr r4, [r1]
  // compare r3 and r4
  cmp r3, r4
  beq 2
1:

2:
  str r2, 
```
