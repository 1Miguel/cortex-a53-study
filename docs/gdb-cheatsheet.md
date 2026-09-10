# GDB Breakpoint Cheat Sheet

## Quick reference


| Goal | Command |
|---|---|
| Connect to a target server | `target remote <port i.e :3333>` |
| Current breakpoint location | `where` |
| Display the current stackframe | `f` or 'frame' |
| Show the current line of code | `l` or 'list' |
| Show if the program is halted | 'info program' |
| Break at a function | `b function_name` |
| Break at `main` | `b main` |
| Break at a source line | `b file.c:line` |
| Break at an address | `b *0xADDRESS` |
| Stop at first instruction | `starti` |
| List breakpoints | `info break` |
| Delete breakpoints, num can be found using info break | `delete <breakpoint_num>` |
| Delete all breakpoints | `delete` |
| Continue | `c` or `continue`|
| One source line | `n` or `s` |
| One machine instruction | `si` |
| Show current instruction | `x/i $pc` |
| Show registers | `info registers` |
| Backtrace | `bt` |
| Send monitor commands to renode | `monitor <renode_cmds>` |

## Start GDB

```bash
gdb ./program
```

Useful startup options:

```bash
gdb -q ./program          # Start without the startup banner
gdb --args ./program arg1 # Provide command-line arguments
```

## Set breakpoints

### Break at a function

```gdb
break function_name
b function_name           # Short form
```

Example:

```gdb
b main
```

This stops when execution reaches `main`.

### Break at `main`

```gdb
break main
run
```

If the program is already running:

```gdb
continue
```

### Break at an address

```gdb
break *0xADDRESS
```

The `*` means that the location is an instruction address rather than a
function or source-line name.

To find the address of a symbol:

```gdb
info address main
disassemble main
```

### Break at the first instruction

For a program that has not started yet, use:

```gdb
starti
```

`starti` starts the program and stops at its first instruction. It is the
most convenient way to inspect the entry point before `main`.

To stop at the executable entry point without running first:

```gdb
info files
break *ENTRY_POINT_ADDRESS
run
```

The entry point is shown by `info files`. For a dynamically linked program,
the first instruction may be in the dynamic loader rather than in the
program's own `_start`.

To inspect the program's startup code:

```gdb
disassemble /m _start
break _start
run
```

On targets where the entry symbol is available, this is equivalent to:

```gdb
break *&_start
```

## Manage breakpoints

```gdb
info breakpoints        # List breakpoints
info break              # Short form
disable 1               # Disable breakpoint 1
enable 1                # Enable breakpoint 1
delete 1                # Delete breakpoint 1
delete                  # Delete all breakpoints (asks for confirmation)
clear main              # Remove breakpoints at main
```

Create a temporary breakpoint that is removed after it is hit:

```gdb
tbreak function_name
tbreak *0xADDRESS
```

## Run and step

```gdb
run                     # Start or restart the program
continue                # Continue until the next stop
step                    # Execute source line, stepping into calls
next                    # Execute source line, stepping over calls
stepi                   # Execute one machine instruction
nexti                   # Execute one instruction, stepping over calls
finish                  # Run until the current function returns
until LOCATION          # Run until LOCATION or a later source line
```

Pass arguments after starting GDB:

```gdb
set args arg1 arg2
run
```

## Inspect the current instruction

```gdb
disassemble              # Disassemble the current function
x/i $pc                  # Examine the instruction at the program counter
x/10i $pc                # Show the next 10 instructions
display/i $pc            # Show the current instruction after each stop
```

Common program-counter names:

```gdb
$pc                      # Generic program counter
$rip                     # x86-64 program counter
$eip                     # 32-bit x86 program counter
$x30                     # AArch64 link register; use $pc for the PC
```

## Inspect registers and memory

```gdb
info registers           # Display all registers
p/x $pc                  # Print the PC in hexadecimal
p/x $sp                  # Print the stack pointer
x/16gx $sp               # Examine 16 giant words from the stack
x/16wx ADDRESS           # Examine 16 words at ADDRESS
x/s STRING_ADDRESS       # Examine a NUL-terminated string
```

Common `x` format combinations:

| Command | Meaning |
|---|---|
| `x/i ADDRESS` | Instruction |
| `x/10i ADDRESS` | 10 instructions |
| `x/x ADDRESS` | Hexadecimal value |
| `x/16gx ADDRESS` | 16 8-byte hexadecimal values |
| `x/s ADDRESS` | String |
| `x/16b ADDRESS` | 16 bytes |

## Conditional and selective breakpoints

```gdb
break file.c:42
break function if condition
condition 1 variable == 0
ignore 1 5                # Ignore breakpoint 1 five times
```

Examples:

```gdb
b main.c:25
b process_packet if length >  MaxPacketSize
condition 2 count == 10
```

## Breakpoint commands

Run commands automatically whenever a breakpoint is hit:

```gdb
commands 1
silent
printf "PC = %p\n", $pc
bt
continue
end
```

Remove the `continue` line if execution should stop after printing.

## Useful stop-state commands

```gdb
bt                       # Backtrace
frame 0                  # Select the current frame
info locals              # Show local variables
list                     # Show source around the current line
print variable           # Print a variable
print/x expression       # Print an expression in hexadecimal
```

## Typical breakpoint workflow

```bash
gdb ./program
```

```gdb
# Stop at the first instruction
starti

# Inspect startup instructions and registers
x/10i $pc
info registers

# Continue to main
b main
continue

# Step one machine instruction at a time
stepi

# Inspect active breakpoints
info breakpoints
```

