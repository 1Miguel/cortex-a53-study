# ARM Architecture

ARM has a load/store architecture. All computation takes place in the ARM's registers. Application use variables to hold data stored in main memory. ARM core operates on this data via loading this data from main memory to internal registers, then store it back from registers back to main memory.

The generic syntax for load(**ld**) and store(**str**) 
```asm
ldr{size} reg, mem
str{size} reg, mem
```

