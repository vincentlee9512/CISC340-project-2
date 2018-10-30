## Updates
10/30/2018: all content has been implemented 

## How-to run
In the following examples, the assembly-language program to be
assembled is "program.as" and the machine-code file generated would be "program.mc":

  # input example
  $./sim-multi -i program.mc

The program should print the state of UST-3400 before each instruction is executed.
It will also print the number of cycle that program used.


## Files Used
**sim-multi.c** : takes machine code and simulates the behaviors of the instructions on the UST-3400 machine, recording the number of cycles for each instruction

**makefile** : compiles and cleans our simulator.c

### Test Suite
    testingAddAndNand: uses significant number of "add" and "nand" instructions.
    testingLwAndSw: uses significant number of "lw" and "sw" instructions.
    testingBeq: uses significant number of "beq" instructions.
    testingJalr: uses significant number of "jalr" instructions.
