#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMMEMORY 65536 /* maximum number of data words in memory */
#define NUMREGS 8 /* number of machine registers */

#define ADD 0
#define NAND 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5
#define HALT 6
#define NOOP 7

#define NOOPINSTRUCTION 0x1c00000

typedef struct stateStruct {
    int pc;
	int mem[NUMMEMORY];
	int reg[NUMREGS];
	int numMemory;
} stateType;

int field0(int instruction){
    return( (instruction>>19) & 0x7);
}

int field1(int instruction){
    return( (instruction>>16) & 0x7);
}

int field2(int instruction){
    return(instruction & 0xFFFF);
}

int opcode(int instruction){
    return(instruction>>22);
}

void printInstruction(int instr){
    char opcodeString[10];
    if (opcode(instr) == ADD) {
	strcpy(opcodeString, "add");
    } else if (opcode(instr) == NAND) {
	strcpy(opcodeString, "nand");
    } else if (opcode(instr) == LW) {
	strcpy(opcodeString, "lw");
    } else if (opcode(instr) == SW) {
	strcpy(opcodeString, "sw");
    } else if (opcode(instr) == BEQ) {
	strcpy(opcodeString, "beq");
    } else if (opcode(instr) == JALR) {
	strcpy(opcodeString, "jalr");
    } else if (opcode(instr) == HALT) {
	strcpy(opcodeString, "halt");
    } else if (opcode(instr) == NOOP) {
	strcpy(opcodeString, "noop");
    } else {
	strcpy(opcodeString, "data");
    }

    printf("%s %d %d %d\n", opcodeString, field0(instr), field1(instr),
	field2(instr));
}

void printState(stateType *statePtr){
    int i;
	printf("\n@@@\nstate:\n");
	printf("\tpc %d\n", statePtr->pc);
	printf("\tmemory:\n");
	for(i = 0; i < statePtr->numMemory; i++){
		printf("\t\tmem[%d]=%d\n", i, statePtr->mem[i]);
	}	
	printf("\tregisters:\n");
	for(i = 0; i < NUMREGS; i++){
		printf("\t\treg[%d]=%d\n", i, statePtr->reg[i]);
	}
	printf("end state\n");
}

int signExtend(int num){
	// convert a 16-bit number into a 32-bit integer
	if (num & (1<<15) ) {
		num -= (1<<16);
	}
	return num;
}

void print_stats(int n_instrs, int cycles){
	printf("INSTRUCTIONS: %d\n", n_instrs);
	printf("CYCLES: %d\n", cycles);
}


void run(stateType* state){

	// Reused variables;
	int instr = 0;
    int regA = 0;
	int regB = 0;
	int offset = 0;
	int branchTarget = 0;
	int aluResult = 0;
	int total_cycles = 0;
	int total_instrs = 0;

	// Primary loop
    while(1){
		total_instrs++;

		printState(state);
		
		// Instruction Fetch
		instr = state->mem[state->pc];

		/* check for halt */
		if (opcode(instr) == HALT) {
		    printf("machine halted\n");
			total_cycles += 1;
			break;
		}

		// Increment the PC
		state->pc = state->pc+1;
		
		// Set reg A and B
		regA = state->reg[field0(instr)];
		regB = state->reg[field1(instr)];

		// Set sign extended offset
		offset = signExtend(field2(instr));

		// Branch target gets set regardless of instruction
		branchTarget = state->pc + offset;

		/**
		 *
		 * Action depends on instruction
		 *
		 **/
		// ADD
		if(opcode(instr) == ADD){
			// Add
			aluResult = regA + regB;
			// Save result
			state->reg[field2(instr)] = aluResult;
			
			total_cycles += 2;
		}
		// NAND
		else if(opcode(instr) == NAND){
			// NAND
			aluResult = ~(regA & regB);
			// Save result
			state->reg[field2(instr)] = aluResult;
			
			total_cycles += 2;
		}
		// LW or SW
		else if(opcode(instr) == LW || opcode(instr) == SW){
			// Calculate memory address
			aluResult = regB + offset;
			if(opcode(instr) == LW){
				// Load
				state->reg[field0(instr)] = state->mem[aluResult];
			}else if(opcode(instr) == SW){
				// Store
				state->mem[aluResult] = regA;
			}
			total_cycles += 5;
		}
		// JALR
		else if(opcode(instr) == JALR){
			// rA != rB for JALR to work
			if(field0(instr) != field1(instr)){
				// Save pc+1 in regA
				state->reg[field0(instr)] = state->pc;
				//Jump to the address in regB;
				state->pc = state->reg[field1(instr)];
			}

			total_cycles += 1;
		}
		// BEQ
		else if(opcode(instr) == BEQ){
			// Calculate condition
			aluResult = (regA == regB);

			// ZD
			if(aluResult){
				// branch
				state->pc = branchTarget;
			}

			total_cycles += 3;
		}

		// NOOP
		else if(opcode(instr) == NOOP){
			total_cycles += 1;
		}	
    } // While
	print_stats(total_instrs, total_cycles);
}

int main(int argc, char** argv){

	/** Get command line arguments **/
    char* fname;

	opterr = 0;

	int cin = 0;

	while((cin = getopt(argc, argv, "i:")) != -1){
		switch(cin)
		{
			case 'i':
				fname=(char*)malloc(strlen(optarg));
				fname[0] = '\0';

				strncpy(fname, optarg, strlen(optarg)+1);
				printf("FILE: %s\n", fname);
				break;
			case '?':
				if(optopt == 'i'){
					printf("Option -%c requires an argument.\n", optopt);
				}
				else if(isprint(optopt)){
					printf("Unknown option `-%c'.\n", optopt);
				}
				else{
					printf("Unknown option character `\\x%x'.\n", optopt);
					return 1;
				}
				break;
			default:
				abort();
		}
	}

	/*
	if(argc == 1){
		fname = (char*)malloc(sizeof(char)*100);
		printf("Enter the name of the machine code file to simulate: ");
		fgets(fname, 100, stdin);
		fname[strlen(fname)-1] = '\0'; // gobble up the \n with a \0
	}
	else if (argc == 2){
        
	    int strsize = strlen(argv[1]);

		fname = (char*)malloc(strsize);
		fname[0] = '\0';

		strcat(fname, argv[1]);
	}else{
		printf("Please run this program correctly\n");
		exit(-1);
	}
	*/

	FILE *fp = fopen(fname, "r");
	if (fp == NULL) {
		printf("Cannot open file '%s' : %s\n", fname, strerror(errno));
		return -1;
	}
	
	/* count the number of lines by counting newline characters */
	int line_count = 0;
	int c;
    while (EOF != (c=getc(fp))) {
        if ( c == '\n' ){
            line_count++;
		}
    }
	// reset fp to the beginning of the file
	rewind(fp);

	stateType* state = (stateType*)malloc(sizeof(stateType));
	
	state->pc = 0;
	memset(state->mem, 0, NUMMEMORY*sizeof(int));
	memset(state->reg, 0, NUMREGS*sizeof(int));

	state->numMemory = line_count;

	char line[256];
	
	int i = 0;
	while (fgets(line, sizeof(line), fp)) {
        /* note that fgets doesn't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
		state->mem[i] = atoi(line);
		i++;
    }
    fclose(fp);
	
	/** Run the simulation **/
	run(state);

	free(state);
	free(fname);

}
