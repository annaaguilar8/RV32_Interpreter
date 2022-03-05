//Anna Aguilar 80669897

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // malloc & free
#include <stdint.h> // use guaranteed 64-bit integers
#include "tokenizer.h" // Create header file and reference that
#include "memory.h" // built-in functions to read and write to a specific file
#include <math.h> // pow function for converting hex to decimal

int32_t* reg; // Array of 32 32-bit registers

void init_regs();
bool interpret(char* instr);
void write_read_demo();

/**
 * Initialize register array for usage.
 * Malloc space for each 32-bit register then initializes each register to 0.
 * Do not alter this function!
 */
void init_regs(){
	int reg_amount = 32;
	reg = malloc(reg_amount * sizeof(int32_t)); // 32 * 4 bytes
	for(int i = 0; i < 32; i++)
		reg[i] = i;
}

//calling fuction to avoid "error: implicit declaration of function 'print_all_tokens' is invalid in C99"
void print_all_tokens(char** tokens);

// function to comapre two strings without using strcmp
bool stringCompare(char* userIn, char* instruction){
	int i = 0;
	// iterate through each letter in word and if something doesnt match return false
	if (userIn[i] != instruction[i]){
		return false;
	}
   	while (userIn[i] == instruction[i]){
	   i++;
		if (userIn[i] != instruction[i]){
			return false;
		}else if((userIn[i] == '\0') || (instruction[i] == '\0')){
			break;
		}
	}	 
	return true;
}

/**
 * Fill out this function and use it to read interpret user input to execute RV64 instructions.
 * You may expect that a single, properly formatted RISC-V instruction string will be passed
 * as a parameter to this function.
 */
bool interpret(char* instr){
	char delim = ' ';
	char** tokens = tokenize(instr,delim);
	// Created strings and pointers for each intruction to help with my stringCompare function
	char add[5] = "ADD";
	char *pointerAdd = add; 
	char addi[5] = "ADDI";
	char *pointerAddi = addi;
	char lw[5] = "LW";
	char *pointerLw = lw; 
	char sw[5] = "SW";
	char *pointerSw = sw;
	char eof[5] = "EOF";
	char *pointerEof = eof;
	// Extra credit
	char and[5] = "AND";
	char *pointerAnd = and; 
	char or[5] = "OR";
	char *pointerOr = or;
	char xor[5] = "XOR";
	char *pointerXor = xor;

	// If statements for each intruction
	if(stringCompare(tokens[0],pointerAdd)){
		// Create pointers for each value of each token
		char *rd = tokens[1];
		char *rs1 = tokens[2];
		char *rs2 = tokens[3];
		// Use atoi to convert to int 
		// I added 1 to the pointers to skip over the 'X'
		int rdInt = atoi(rd+1);
		int rs1Int = atoi(rs1+1);
		int rs2Int = atoi(rs2+1);

		reg[rdInt] = reg[rs1Int] + reg[rs2Int];
		
	}else if(stringCompare(tokens[0],pointerAddi)){
		// Create pointers for each value of each token
		char *rd = tokens[1];
		char *rs1 = tokens[2];
		char *imm = tokens[3];
		// Use atoi to convert to int 
		// I added 1 to the pointers to skip over the 'X'
		int rdInt = atoi(rd+1);
		int rs1Int = atoi(rs1+1);
		int immInt = atoi(imm);

		reg[rdInt] = reg[rs1Int] + immInt;

	}else if(stringCompare(tokens[0],pointerLw)){
		// We have to tokenize 2 more time to get rid of the open and closing parethesis
		char openDelim = '(';
		char** tokensLW = tokenize(tokens[2],openDelim);
		char closeDelim = ')';
		char** rs1Token = tokenize(tokensLW[1],closeDelim);
		// Create pointers for each value of each token
		char *rd = tokens[1];
		char *imm = tokensLW[0];
		char *rs1 = rs1Token[0];
		// Use atoi to convert to int 
		int rdInt = atoi(rd+1);
		int rs1Int = atoi(rs1+1);
		int immInt = atoi(imm);

		// To get our address location we must use memory[rs1 + imm]
		// and since our memory addresses are going by 2, we need to make sure its an even number
		// If not, we go to the next number up. 
		int memAddress = rs1Int + immInt;
		if(memAddress % 2 != 0){
			memAddress += 1;
		}

		// store (memory[rs1 + imm]) in rd
		int32_t read = read_address(memAddress, "mem.txt");
		reg[rdInt] = read;

	}else if(stringCompare(tokens[0],pointerSw)){
		char openDelim = '(';
		char** tokensLW = tokenize(tokens[2],openDelim);
		char closeDelim = ')';
		char** rs1Token = tokenize(tokensLW[1],closeDelim);
		// Create pointers for each value of each token
		char *rs2 = tokens[1];
		char *imm = tokensLW[0];
		char *rs1 = rs1Token[0];
		// Use atoi to convert to int 
		int rs2Int = atoi(rs2+1);
		int rs1Int = atoi(rs1+1);
		int immInt = atoi(imm);

		// Store rs2 in memory[rs1 + offset]
		int rs2Val = reg[rs2Int];

		// To get our address location we must use memory[rs1 + imm]
		// and since our memory addresses are going by 2, we need to make sure its an even number
		// If not, we go to the next number up. 
		int memAddress = rs1Int + immInt;
		if(memAddress % 2 != 0){
			memAddress += 1;
		}	
		// Write new address
		int32_t write = write_address(rs2Val, memAddress, "mem.txt");
		if(write == (int32_t) NULL){
			printf("ERROR: Unsucessful write to address %0d\n", memAddress);
		}
	}else if(stringCompare(tokens[0],pointerAnd)){
		// Create pointers for each value of each token
		char *rd = tokens[1];
		char *rs1 = tokens[2];
		char *rs2 = tokens[3];
		// Use atoi to convert to int 
		// I added 1 to the pointers to skip over the 'X'
		int rdInt = atoi(rd+1);
		int rs1Int = atoi(rs1+1);
		int rs2Int = atoi(rs2+1);

		// Using & to preform bitwise and on rs1 and rs2
		// If rs1 = 010011 and rs2 = 011001, the reult of AND would be dr = 010001
		reg[rdInt] = reg[rs1Int] & reg[rs2Int];

	}else if(stringCompare(tokens[0],pointerOr)){
		// Create pointers for each value of each token
		char *rd = tokens[1];
		char *rs1 = tokens[2];
		char *rs2 = tokens[3];
		// Use atoi to convert to int 
		// I added 1 to the pointers to skip over the 'X'
		int rdInt = atoi(rd+1);
		int rs1Int = atoi(rs1+1);
		int rs2Int = atoi(rs2+1);

		// Using | to preform bitwise or on rs1 and rs2
		// If rs1 = 000100 and rs2 = 000001, the reult of OR would be dr = 000101
		reg[rdInt] = reg[rs1Int] | reg[rs2Int];
	
	}else if(stringCompare(tokens[0],pointerXor)){
		// Create pointers for each value of each token
		char *rd = tokens[1];
		char *rs1 = tokens[2];
		char *rs2 = tokens[3];
		// Use atoi to convert to int 
		// I added 1 to the pointers to skip over the 'X'
		int rdInt = atoi(rd+1);
		int rs1Int = atoi(rs1+1);
		int rs2Int = atoi(rs2+1);

		// Using ^ to preform bitwise xor on rs1 and rs2
		// xor puts 0's when bits are the same and 1's when they are different
		reg[rdInt] = reg[rs1Int] ^ reg[rs2Int];

	}else{
		printf("Sorry! Invalid instruction..\nPlease try again or enter 'EOF' character to end program.\n");
		return false;
	}
	return true;
}


/**
 * Simple demo program to show the usage of read_address() and write_address() found in memory.c
 * Before and after running this program, look at mem.txt to see how the values change.
 * Feel free to change "data_to_write" and "address" variables to see how these affect mem.txt
 * Use 0x before an int in C to hardcode it as text, but you may enter base 10 as you see fit.
 */
void write_read_demo(){
	int32_t data_to_write = 0xFFF; // equal to 4095
	int32_t address = 0x98; // equal to 152
	char* mem_file = "mem.txt";

	// Write 4095 (or "0000000 00000FFF") in the 20th address (address 152 == 0x98)
	int32_t write = write_address(data_to_write, address, mem_file);
	if(write == (int32_t) NULL)
		printf("ERROR: Unsucessful write to address %0X\n", 0x40);
	int32_t read = read_address(address, mem_file);

	printf("Read address %d (0x%d): %d (0x%lX)\n", address, address, read, read); // %lu -> format as an long-unsigned
}

/**
 * Prints all 32 registers in column-format
 */
void print_regs(){
	int col_size = 10;
	for(int i = 0; i < 8; i++){
		printf("X%02i:%.*lld", i, col_size, (long long int) reg[i]);
		printf(" X%02i:%.*lld", i+8, col_size, (long long int) reg[i+8]);
		printf(" X%02i:%.*lld", i+16, col_size, (long long int) reg[i+16]);
		printf(" X%02i:%.*lld\n", i+24, col_size, (long long int) reg[i+24]);
	}
}


int main(){
	// Do not write any code between init_regs
	init_regs(); // DO NOT REMOVE THIS LINE

	print_regs();

	// Below is a sample program to a write-read. Overwrite this with your own code.
	// write_read_demo();

	printf(" RV32 Interpreter.\nType RV32 instructions. Use upper-case letters and space as a delimiter.\nEnter 'EOF' character to end program\n");


	char* instruction = malloc(1000 * sizeof(char));
	bool is_null = false;
	// fgets() returns null if EOF is reached.
	is_null = fgets(instruction, 1000, stdin) == NULL;
	while(!is_null){
		interpret(instruction);
		printf("\n");
		print_regs();
		printf("\n");

		is_null = fgets(instruction, 1000, stdin) == NULL;
	}

	printf("Good bye!\n");

	return 0;
}
