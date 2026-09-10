# Renode Commands and Usage Quick Guide

> A practical command reference for Renode's Monitor/CLI, `.resc` scripts,
> debugging, networking, logging, state management, and automated tests.
>
> **Renode version note:** Renode is extensible, and the exact command set can
> vary by release and loaded plugins. The authoritative command list for the
> running instance is always `help`; use `help <command>` for syntax and
> `object`/`object method` for machine and peripheral APIs.

## Contents

- [Notation and command discovery](#notation-and-command-discovery)
- [Starting Renode](#starting-renode)
- [Monitor essentials](#monitor-essentials)
- [Machines and platform setup](#machines-and-platform-setup)
- [Loading software](#loading-software)
- [Execution control](#execution-control)
- [Inspecting and manipulating peripherals](#inspecting-and-manipulating-peripherals)
- [UART and analyzers](#uart-and-analyzers)
- [Logging and tracing](#logging-and-tracing)
- [Saving and restoring state](#saving-and-restoring-state)
- [Monitor variables and scripts](#monitor-variables-and-scripts)
- [Python integration](#python-integration)
- [GDB debugging](#gdb-debugging)
- [Networking](#networking)
- [Testing with Robot Framework](#testing-with-robot-framework)
- [Useful recipes](#useful-recipes)
- [Common mistakes](#common-mistakes)
- [Official references](#official-references)

---

## Notation and command discovery

### Prompt contexts

Renode displays the current context in the prompt:

```text
(monitor)       # global Monitor context
(machine-0)     # selected machine context
```

Most commands are entered one per line. Arguments are separated by spaces.
Strings containing spaces must be quoted.

### Built-in command discovery

```text
help
help <command>
<partial-command><Tab>
<command><Tab><Tab>
```

Examples:

```text
help start
help mach
help analyzers
```

Use `Ctrl+R` for command-history search, the up/down arrows for history, and
`Ctrl+C` to clear the current input. Many common commands have short aliases,
for example `s` for `start`, `q` for `quit`, and `i` for `include`.

### Object and peripheral discovery

An emulation object exposes its methods, properties, fields, and indexers when
its name is entered by itself:

```text
sysbus
sysbus.uart
sysbus.uart WriteByte
```

Typical output includes the accepted parameter types and usage. This is the
complete API for the specific peripheral model loaded in the current machine.

### Common value syntax

| Value | Example | Meaning |
|---|---|---|
| Decimal integer | `42` | Integer |
| Hexadecimal integer | `0x40000000` | Integer in hexadecimal |
| Float | `1.5` | Floating-point value |
| Boolean | `true`, `false` | Logical value |
| String | `"hello"` | Quoted string |
| File path | `@firmware.elf` | Path with completion |
| Range | `<0x80000000 0x1000>` | Base address and length |
| Index | `[0]` | Indexer argument |
| Variable | `$bin` | Monitor variable or macro |
| Comment | `# comment` or `: comment` | Rest of line ignored |
| Command substitution | `` `cpu PC` `` | Run inner command and use its output |

Use `@` for file-path completion. Relative paths are resolved using Renode's
configured path and current working directory. In scripts, `$ORIGIN` refers to
the directory of the current `.resc` file; in the interactive Monitor,
`$CWD` refers to the current working directory.

---

## Starting Renode

Run the graphical application:

```bash
renode
```

Run a script immediately:

```bash
renode my-platform.resc
renode @my-platform.resc
```

Load a saved state directly:

```bash
renode statefile.save
renode statefile.save.gz
```

Run the Monitor in the same terminal:

```bash
renode --console
```

Run without GUI, with Monitor available on port `1234` by default:

```bash
renode --disable-gui
```

Run Monitor over telnet on a chosen port:

```bash
renode -P 1234
telnet 127.0.0.1 1234
```

Combine console and headless modes:

```bash
renode --console --disable-gui
```

Quit from the Monitor:

```text
quit
```

---

## Monitor essentials

| Command | Usage | Purpose |
|---|---|---|
| `help` | `help` | List built-in commands |
| `help` | `help <command>` | Show detailed help |
| `quit` | `quit` / `q` | Exit Renode |
| `log` | `log "message"` | Write a message to the Renode log |
| `include` | `include @file.resc` | Load a script without starting execution |
| `start` | `start` / `s` | Start emulation; `start @file.resc` can load and start a script |
| `pause` | `pause` | Pause emulation |
| `clear` | `Clear` | Remove all machines, peripherals, and loaded state |
| `using` | `using sysbus` | Set a default object prefix |
| `peripherals` | `peripherals [filter]` | List or filter machine peripherals |
| `path` | `path` / `path <value>` | Inspect or configure path lookup |
| `currentTime` | `currentTime` | Print virtual and host time |
| `commandFromHistory` | `commandFromHistory <index>` | Execute a command from history |

Command names are generally case-insensitive in normal Monitor use, but object
members and script examples should be copied exactly when a model requires a
specific spelling.

---

## Machines and platform setup

### Create and select machines

```text
mach create
mach create "my-machine"
mach set "machine-0"
mach set 0
help mach
```

`mach create` creates an empty machine and switches the Monitor to it. A
machine initially contains only `sysbus`.

### Load a platform description

```text
machine LoadPlatformDescription @platforms/cpus/nrf52840.repl
```

The `.repl` file defines CPUs, memories, buses, and peripherals. After loading:

```text
peripherals
peripherals uart
peripherals CoreTimer
peripherals <0x70000000 0x7fffffff>
```

### Access the system bus

The system bus is normally the root of the peripheral hierarchy:

```text
sysbus
sysbus.cpu
sysbus.uart
sysbus.timer
```

Shorten repeated paths:

```text
using sysbus
cpu
uart
```

### Typical platform bootstrap

```text
mach create "board"
machine LoadPlatformDescription @platforms/cpus/my-board.repl
peripherals
using sysbus
sysbus LoadELF @build/firmware.elf
showAnalyzer uart
start
```

---

## Loading software

### ELF

```text
sysbus LoadELF @firmware.elf
sysbus LoadELF @https://example.com/firmware.elf
```

Loading multiple ELF files uses the lowest loaded section of the **last** ELF
to determine the initial program counter (or Cortex-M vector-table offset).
Override it explicitly when necessary:

```text
sysbus.cpu PC 0x08000100
sysbus.cpu VectorTableOffset 0x08000000
```

### Other formats

```text
sysbus LoadBinary @firmware.bin
sysbus LoadUImage @image.uimage
sysbus LoadHEX @firmware.hex
```

The exact parameters for these methods can be checked with:

```text
sysbus LoadBinary
sysbus LoadUImage
sysbus LoadHEX
```

---

## Execution control

### Start, pause, and reset

```text
start
pause
machine Reset
```

If a `$reset` macro exists, `machine Reset` invokes it as part of the reset
flow.

### Inspect the current instruction

```text
sysbus.cpu PC
sysbus FindSymbolAt `sysbus.cpu PC`
```

The second command resolves the current program counter to a symbol when debug
symbols are available.

### Single-step execution

Blocking single-step:

```text
sysbus.cpu ExecutionMode SingleStepBlocking
sysbus.cpu Step
```

Non-blocking single-step:

```text
sysbus.cpu ExecutionMode SingleStepNonBlocking
sysbus.cpu Step false
```

Return to continuous execution:

```text
sysbus.cpu ExecutionMode Continuous
```

Blocking mode stops virtual-time progress between instructions. Non-blocking
mode is more suitable for multicore stepping, but virtual time can progress
between steps.

---

## Inspecting and manipulating peripherals

### Read and write properties

```text
sysbus.ddr Size
sysbus.cpu CyclesPerInstruction
sysbus.cpu CyclesPerInstruction 0x2
```

No value means “get”; appending a value means “set”.

### Invoke methods

```text
sysbus.ddr ZeroAll
sysbus.uart DumpHistoryBuffer
```

Always inspect a model before using an unfamiliar method:

```text
sysbus.uart
sysbus.uart DumpHistoryBuffer
```

### Read and write memory/peripheral registers

```text
sysbus.uart ReadByte 0x0
sysbus.uart WriteByte 0x0 0x41
```

The available read/write method names depend on the object and bus width. Common
forms include `ReadByte`, `ReadWord`, `ReadDoubleWord`, `WriteByte`,
`WriteWord`, and `WriteDoubleWord`.

### Enumerable objects

Apply a property or method to every item:

```text
cluster0 ForEach IsHalted true
```

Transform every item and return the results:

```text
cluster0 Select GetRegister "X0"
```

---

## UART and analyzers

Show a graphical or configured analyzer:

```text
showAnalyzer uart
showAnalyzer sysbus.uart0
```

Discover analyzer IDs:

```text
analyzers
analyzers sysbus.uart0
analyzers default sysbus.uart0
```

Connect the Monitor directly to a UART:

```text
uart_connect sysbus.uart0
```

Input is redirected to the UART until `Esc` is pressed.

Typical `.resc` UART setup:

```text
showAnalyzer uart
start
```

For file-backed UART output, inspect the UART model first; many UART models
provide methods such as:

```text
sysbus.uart CreateFileBackend @uart.log
sysbus.uart CloseFileBackend @uart.log
```

---

## Logging and tracing

### Log levels

Levels, from most to least verbose:

```text
NOISY    = -1
DEBUG    = 0
INFO     = 1
WARNING  = 2
ERROR    = 3
```

Use the global logger:

```text
logLevel
logLevel -1
```

Configure a specific peripheral:

```text
logLevel -1 sysbus.uart
```

Configure a backend separately:

```text
logFile @renode.log
logLevel 2 file
logLevel 1 file sysbus.uart
```

### Peripheral access logging

```text
sysbus LogPeripheralAccess sysbus.uart
sysbus LogAllPeripheralsAccess true
```

Disable the global access logger:

```text
sysbus LogAllPeripheralsAccess false
```

### CPU execution tracing

Function-name tracing:

```text
sysbus.cpu LogFunctionNames true
sysbus.cpu LogFunctionNames true ["dev kobject"]
sysbus.cpu LogFunctionNames true ["dev kobject"] true
```

The final `true` suppresses subsequent duplicate function names. A lighter,
less exact alternative is:

```text
sysbus.cpu LogTranslationBlockFetch true
```

### Silence known unhandled ranges

```text
sysbus SilenceRange <0x80000 0x1000>
```

Use this only for ranges that are intentionally unmapped or irrelevant to the
simulation; unhandled-access warnings can reveal missing platform models.

---

## Saving and restoring state

Save the complete emulation:

```text
Save @statefile.save
```

Restore it:

```text
Load @statefile.save
Load @statefile.save.gz
```

Loading a state clears the current emulation first. After loading, select the
machine and reopen analyzers:

```text
mach set 0
showAnalyzer sysbus.uart
```

Enable periodic autosaving using a virtual-time interval:

```text
autoSave true "0.2"
```

Disable it:

```text
autoSave false
```

State files can be version-sensitive; a save created by one Renode version may
not load in another.

---

## Monitor variables and scripts

### Variables

```text
$bin=@build/firmware.elf
$bin?=@build/firmware.elf
set greeting "hello"
```

`$var=` sets a value. `$var?=` sets a default only when the variable is not
already defined. `set` is the long-form assignment command.

Multiline value:

```text
set message
"""
"line one"
"line two"
"""
```

Special variables:

```text
$ORIGIN    # current .resc script directory; scripts only
$CWD       # current working directory; useful interactively
```

Global variables use the `global` scope:

```text
$global.CWD
```

### Macros

Define and run a reusable command sequence:

```text
macro load_firmware
"""
    sysbus LoadELF $bin
    showAnalyzer uart
"""

runMacro $load_firmware
```

### `.resc` scripts

Load a script:

```text
include @scripts/my-board.resc
```

Start immediately after loading:

```text
start @scripts/my-board.resc
```

A minimal script:

```text
:name: My board
:description: Load and run firmware on a board.

using sysbus
mach create "my-board"
machine LoadPlatformDescription @platforms/cpus/my-board.repl

$bin?=$ORIGIN/../../build/firmware.elf

macro reset
"""
    sysbus LoadELF $bin
"""

showAnalyzer uart
runMacro $reset
start
```

Scripts can include other scripts:

```text
include $ORIGIN/common.resc
```

---

## Python integration

Renode supports built-in IronPython inside the Monitor, plus external Python
integrations. The built-in Monitor integration is commonly used for hooks and
custom commands.

Execute Python:

```text
python "print 'Hello from Renode'"
```

Load a Python file:

```text
include @scripts/my-monitor.py
```

Multiline Python:

```text
set py
"""
print "Hello"
"""
python $py
```

Define a Monitor command by prefixing a Python function with `mc_`:

```python
def mc_sleep(time):
    sleep(float(time))
```

After loading the definition, call it from the Monitor without the prefix:

```text
sleep 5
```

Useful Python context variables include `monitor`, `self`, `externals`, and
`variables`. For example, `monitor.Machine['sysbus.uart0']` accesses an object
in the selected machine.

---

## GDB debugging

Start a GDB remote server:

```text
machine StartGdbServer 3333
```

Connect with the target toolchain:

```bash
arm-none-eabi-gdb build/firmware.elf
```

```gdb
target remote :3333
continue
```

Start emulation automatically when GDB connects:

```text
machine StartGdbServer 3333 true
```

For a selected CPU or cluster:

```text
machine StartGdbServer 3333 true cpu=sysbus.cpu
machine StartGdbServer 3333 true cpuCluster="cortex-r5f"
machine StartGdbServer 3333 true cpuCluster="all"
```

Add CPUs to an existing server or create another server:

```text
machine StartGdbServer 3333 true sysbus.cpu1
machine StartGdbServer 3334 true sysbus.cpu2
```

You can pass Monitor commands from GDB:

```gdb
monitor start
continue
```

Enable reverse execution snapshots:

```text
reverseExecMode true
```

Then use GDB:

```gdb
reverse-step
reverse-stepi
reverse-continue
rs
rsi
rc
rsi 5
```

Reverse execution is intended for single-core emulations and is based on
periodic or manually created snapshots.

---

## Networking

### Wired switch topology

Create a switch globally:

```text
emulation CreateSwitch "switch1"
```

Connect an interface from the selected machine:

```text
connector Connect sysbus.ethernet switch1
```

Disconnect one interface:

```text
connector Disconnect sysbus.ethernet switch1
```

Disconnect it from every switch:

```text
connector DisconnectFromAll sysbus.ethernet
```

Start a switch:

```text
switch1 Start
```

Enable or disable promiscuous mode:

```text
switch1 EnablePromiscuousMode sysbus.ethernet
switch1 DisablePromiscuousMode sysbus.ethernet
```

Complete two-machine example:

```text
mach create "node-a"
machine LoadPlatformDescription @platforms/cpus/node-a.repl
connector Connect sysbus.ethernet switch1

mach create "node-b"
machine LoadPlatformDescription @platforms/cpus/node-b.repl
connector Connect sysbus.ethernet switch1

emulation CreateSwitch "switch1"
switch1 Start
start
```

Create the switch before connecting in scripts when ordering matters:

```text
emulation CreateSwitch "switch1"
```

For host networking, wireless networks, Wireshark, and machine-to-machine
connections, consult the official networking pages listed below because those
features use additional connector and plugin-specific commands.

---

## Testing with Robot Framework

Run one test file:

```bash
renode-test my_test.robot
```

Run multiple files:

```bash
renode-test my_tests.robot additional_tests.robot
```

Run a YAML test list:

```bash
renode-test -t my_tests.yaml
```

Run tests in parallel:

```bash
renode-test -j12 -t my_tests.yaml
renode-test -j3 one.robot two.robot three.robot
```

Stop on the first error:

```bash
renode-test --stop-on-error my_test.robot
```

Repeat tests:

```bash
renode-test -n 10 my_test.robot
```

Run only matching test cases:

```bash
renode-test -f "*GDB*" my_tests.robot
```

Show Renode logs:

```bash
renode-test --show-log my_test.robot
```

Enable interactive GUI/analyzers:

```bash
renode-test --enable-xwt my_test.robot
```

Stop for interactive inspection on failure:

```bash
renode-test --debug-on-error my_test.robot
```

Use a different Robot Framework communication port:

```bash
renode-test -P 9997 my_test.robot
```

Save snapshots of failed tests:

```bash
RENODE_CI_MODE=YES renode-test my_test.robot
```

Common Renode Robot keywords include:

| Keyword | Purpose |
|---|---|
| `Start Emulation` | Start virtual execution |
| `Reset Emulation` | Clear/reset the emulation |
| `Execute Command` | Run a Monitor command |
| `Allocate Temporary File` | Allocate a Renode temporary file |
| `Download File` | Download into the Renode temporary folder |
| `Send Key To Uart` | Send a key |
| `Write Char On Uart` | Write one character |
| `Write Line To Uart` | Write a line |
| `Wait For Line On Uart` | Wait for expected UART output |
| `Wait For Prompt On Uart` | Wait for a prompt |
| `Wait For Next Line On Uart` | Wait for any next line |
| `Test If Uart Is Idle` | Assert that UART has no output |
| `Wait For Outgoing Packet` | Wait for a network packet |
| `Wait For Outgoing Packet With Bytes At Index` | Match packet bytes |
| `Provides` | Save a named test snapshot |
| `Requires` | Restore a named test snapshot |

Minimal Robot test:

```robot
*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Teardown   Test Teardown
Resource        ${RENODEKEYWORDS}

*** Test Cases ***
Should Print Help
    ${x}=    Execute Command    help
    Should Contain    ${x}    Available commands:
```

---

## Useful recipes

### Start a board and firmware

```text
mach create
machine LoadPlatformDescription @platforms/cpus/my-board.repl
using sysbus
sysbus LoadELF @build/firmware.elf
showAnalyzer uart
start
```

### Find a peripheral by name or address

```text
peripherals uart
peripherals <0x40000000 0x10000>
```

### Inspect a CPU register and symbol

```text
sysbus.cpu PC
sysbus FindSymbolAt `sysbus.cpu PC`
```

### Inspect an unknown peripheral API

```text
sysbus.somePeripheral
sysbus.somePeripheral SomeMethod
```

### Set a CPU property

```text
sysbus.cpu CyclesPerInstruction
sysbus.cpu CyclesPerInstruction 2
```

### Reset and reload firmware

```text
pause
machine Reset
sysbus LoadELF @build/firmware.elf
start
```

### Save a booted state for repeated debugging

```text
Save @booted.save
```

Later:

```bash
renode booted.save
```

### Trace only selected function prefixes

```text
sysbus.cpu LogFunctionNames true ["main uart"]
```

### Log all peripheral accesses temporarily

```text
sysbus LogAllPeripheralsAccess true
logLevel -1
```

Disable afterward:

```text
sysbus LogAllPeripheralsAccess false
logLevel 1
```

### Run a UART shell interactively

```text
uart_connect sysbus.uart
```

Press `Esc` to return to the Monitor.

### Build a reusable reset macro

```text
macro reset_firmware
"""
    pause
    machine Reset
    sysbus LoadELF $bin
"""
runMacro $reset_firmware
```

---

## Common mistakes

| Problem | Fix |
|---|---|
| `sysbus.cpu` is not found | Create a machine and load a `.repl` platform first |
| A command is unknown | Run `help`; plugin/model commands may not be loaded |
| A peripheral command is unknown | Enter the object name by itself to inspect its API |
| A file cannot be found | Use `@path`, `$CWD/path`, or `$ORIGIN/path` as appropriate |
| The wrong firmware starts | The last loaded ELF sets the initial entry location; set `PC` or `VectorTableOffset` explicitly |
| UART output is invisible | Run `showAnalyzer <uart>` or use `uart_connect <uart>` |
| A loaded state appears empty | Run `mach set 0` and reopen the analyzer |
| A switch does not forward traffic | Connect interfaces, then start the switch or the whole emulation |
| Stepping hangs other cores | Use `SingleStepNonBlocking` for multicore scenarios |
| Logs are too noisy | Increase `logLevel`, configure a backend, or use `SilenceRange` narrowly |
| GDB connects but execution does not run | Start Renode or use `monitor start`, then `continue` in GDB |

---

## Official references

These pages are the authoritative source for the current release:

- [Using Renode](https://renode.readthedocs.io/en/latest/introduction/using.html)
- [Working with machines](https://renode.readthedocs.io/en/latest/basic/machines.html)
- [Monitor and script syntax](https://renode.readthedocs.io/en/latest/basic/monitor-syntax.html)
- [Basic execution control](https://renode.readthedocs.io/en/latest/basic/control.html)
- [Using the logger](https://renode.readthedocs.io/en/latest/basic/logger.html)
- [State saving and loading](https://renode.readthedocs.io/en/latest/basic/saving.html)
- [Running Renode in different modes](https://renode.readthedocs.io/en/latest/basic/running.html)
- [Using Python in Renode](https://renode.readthedocs.io/en/latest/basic/using-python.html)
- [Debugging with GDB](https://renode.readthedocs.io/en/latest/debugging/gdb.html)
- [Setting up a wired network](https://renode.readthedocs.io/en/latest/networking/wired.html)
- [Testing with Renode](https://renode.readthedocs.io/en/latest/introduction/testing.html)
- [Renode documentation home](https://renode.readthedocs.io/en/latest/)

For the exact command set in your installation, use:

```text
help
help <command>
```

