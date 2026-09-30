# LDR(immediate) vs MOV(immediate)
If you are reading a couple of assembly code, you might notice this:
```
	ldr	r7, =0x7fff
```

Why use `ldr` (load) instruction instead of `mov`? The answer is, if a const is to large it won't fit `mov` instruction encoding. Lets look into `mov` instruction encoding:
```text
``movw Rd, #imm16
   31        26 25             16 15       12 11            0
   +-----------+-----------------+-----------+---------------+
   | opcode    |       Rd        |  some bits|     imm16     |
   +-----------+-----------------+-----------+---------------+`
```

The encoding of `mov` instruction can only fit 16-bit unsigned integer,. Integers above 0xFFFF will not be encodable with `mov` instruction so `mov r7, =0x7fff` will cause an error.

The solution to this is to use `ldr, Rd, =<const>`. `ldr` in this case is a pseudo-op that the *assembler is aware of*. The assembler will actually generate something like this:
```asm
ldr, r7, [pc + #4]
word_0:
  .word 0x7fff
```

`word_0` is a label, the `=word_0` would mean *get the data from memory location* `pc + offset` where offset will point to location of label `word_0`, in this case since the current `pc` is in instruct line `ldr, r7, [pc + #4]`, the `word_0` is the next instruction so its `+4` relative to current 'pc'.
