## ASMULATOR
This challenge embeds the [Unicorn Emulator framework](https://www.unicorn-engine.org/docs/) within a x64 bit binary. The competitor must reverse engineer the challenge to identify the ARM based emulator and write
custom shellcode payload to be able to write arbitrary bytes to a file for execution.

A successful competitor will reverse engineer the custom format, append their payload and achieve exeuction.
Note, this binary should be provided *with symbols*.

### Custom Payload Format

* The custom payload format consists of 4 ASCII bytes to indicate it should route to the shel.
* The next 20 bytes  are for 32-bit ARM instructions to be emulated.
* The final (up to 100MB is allocated) are arbitrary 
```
  4 ascii-bytes  20 ARM-bytes    N number of bytes to get dumped to /tmp/debug.elf
        |               |             |
    [PREAMBLE]     [SHELLCODE]    [PAYLOAD]
    
    
    
Example payload: shel\xde\x00\xa0\xe3\xad\x10\xa0\xe3\xbe\x20\xa0\xe3\xef\x30\xa0\xe3\x7f\x45\x4c\x46\x90\x90\x90\xa0

Preamble                           ARM Shellcode                             Competitor's payload
  |                                     |                                               |
[shel] [\xde\x00\xa0\xe3\xad\x10\xa0\xe3\xbe\x20\xa0\xe3\xef\x30\xa0\xe3\] [x7f\x45\x4c\x46\x90\x90\x90\xa0]
```

### Testing
The Makefile generates a payload and writes the data to ```/tmp/shellcode.raw```.
This can then me thrown at the challenge via the following command:
* ```nc localhost 8080 < /tmp/shellcode.raw```
