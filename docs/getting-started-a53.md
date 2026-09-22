
# Getting Started Renode w/ A53
## Run using the example
1. Run `renode` to open the `renode` monitor
```bash
> renode &
```
2. Run and load the built-in script
```bash
(monitor) include @scripts/single-node/cortex-a53.resc
```
3. type start in the monitor to begin the execution
```bash
ARM Cortex-A53) start
```

## Running with no-gui
1. Run renode with --disable-gui option
```bash
renode --disable-gui
```

2. This will create a telnet session which will allow you to connect to it in a different terminal, this is useful in automation
```bash
telnet localhost 1234
```

3. Then, include the machine file and start gdb server
```bash
(monitor) include @scripts/single-node/cortex-a53.resc
(ARM Cortex-A53) machine StartGdbServer 3333
```

4. Once GDB server is started, all machine/monitor commands can be sent over gdb, at this point you can start the uart console session via
```bash
uart_connect sysbus.uart0
```
