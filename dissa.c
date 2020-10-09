#include <stdio.h>
#include <string.h>

void error();

int main(int argc, char* argv[]){

	FILE *fd = fopen(argv[1], "r");
	if(fd == NULL){
		printf("File not found\n");
		return 0;
	}
	int count = 0;
	int val = 0;
	int sections = 0;
	int i;
	//check the four magic numbers
	fread(&val, 1, 1, fd);
	if(val != 76){
		error();
		return 0;
	}

	fread(&val, 1, 1, fd);
	if(val != 97){
		error();
		return 0;
	}

	fread(&val, 1, 1, fd);
	if(val != 88){
		error();
		return 0;
	}

	fread(&val, 1, 1, fd);
	if(val != 0){
		error();
		return 0;
	}
	//check the version number
	fread(&val, 2, 1, fd);
	if(val != 5){
		error();
		return 0;
	}

	//get the number of sections in the file
	fread(&sections, 1, 1, fd);
	//fprintf(stderr, "%d", sections);	
	for(i = 0; i < sections; i++){
		//get the type of data to read in
		unsigned int type = 0;
		fread(&type, 1, 1, fd);
		
		if(type == 7){
		printf(".text ");

		//get the adress for the type of data
		int addr = 0;
		fread(&addr, 4, 1, fd);
		printf("0x%08x\n", addr);
		
		//get the size of the section
		unsigned int size = 0;
		fread(&size, 4, 1, fd);
		//loop over the section while there are bytes to read
		while(count < size){
			addr+=4;
			unsigned int value = 0;
			unsigned int opcode = 0;
			unsigned int func = 0;
			int logicalstmt = 0;
			int syscall = 0;
			int jr = 0;
			//read in the 32 bit instrucition
			fread(&value, 4, 1, fd);
			//get the opcode
			opcode = value >> 26;
			//if its an R type instruction
			if(opcode == 0){
				//fprintf(stderr, "R type\n");
				//get the function from the instruction
				func = value << 26;
				func = func >> 26;

				//statements for r type instructions
				if(func == 0x20){
					printf("add ");
				} else if (func == 0x21){
					printf("addu ");
				} else if (func == 0x24){
					printf("and ");
				} else if (func == 0x27){
					printf("nor ");
				} else if (func == 0x25){
					printf("or ");
				} else if (func == 0x2a){
					printf("slt ");
				} else if (func == 0x2b){
					printf("sltu ");
				} else if (func == 0x00){
					printf("sll ");
					logicalstmt = 1;
				} else if (func == 0x03){
					printf("sra ");
					logicalstmt = 1;
				} else if (func == 0x02){
					printf("srl ");
					logicalstmt = 1;
				} else if (func == 0x22){
					printf("sub ");
				} else if (func == 0x23){
					printf("subu ");
				} else if (func == 0x26){
					printf("xor ");
				} else if (func == 0x08){
					printf("jr ");
					jr = 1;
				} else if (func == 0x0c){
					printf("syscall\n");
					syscall = 1;
				
				} else {
					printf("R type instruction not found");
				}

				//check if syscall has all 0 values after the opcode
				if(syscall == 1 && (value >> 6 != 0)){
					printf("Incorrect registers for syscall");
			       		return 0;
				}

				if(syscall != 1){
					//get rd register
					unsigned int rd = value << 16;
					rd = rd >> 27;
					//get rs register
					unsigned int rs = value >> 21;
					//get rt
					unsigned int rt = value << 11;
					rt = rt >> 27;
					//get the shift amount
					unsigned int shift = value << 21;
					shift = shift >> 27;

					unsigned int regs[3] = {rd, rs, rt};
					int j = 0;
					//check which each register is
					for(j = 0; j < 3; j++){
						if(func == 0x08){
							j = 1;
						}
						int temp = 0;
						if(logicalstmt == 1 && j == 1){
							j = 2;
							temp = 1;
						}else if(j == 2 && logicalstmt == 1){
							printf("%d\n", shift);
							break;
						}
						//print out the type of register
						 if(regs[j] == 0){
							printf("$zero");
						} else if (regs[j] == 1){
							printf("$at");
						} else if (regs[j] == 2){
							printf("$v0");
						} else if (regs[j] == 3){
							printf("$v1");
						} else if (regs[j] == 4){
							printf("$a0");
						} else if (regs[j] == 5){
							printf("$a1");
						} else if (regs[j] == 6){
							printf("$a2");
						} else if (regs[j] == 7){
							printf("$a3");
						} else if (regs[j] == 8){
							printf("$t0");
						} else if (regs[j] == 9){
							printf("$t1");
						} else if (regs[j] == 0x0a){
							printf("$t2");
						} else if (regs[j] == 0x0b){
							printf("$t3");
						} else if (regs[j] == 0x0c){
							printf("$t4");
						} else if (regs[j] == 0x0d){
							printf("$t5");
						} else if (regs[j] == 0x0e){
							printf("$t6");
						} else if (regs[j] == 0x0f){
							printf("$t7");
						} else if (regs[j] == 0x10){
							printf("$s0");
						} else if (regs[j] == 0x11){
							printf("$s1");
						} else if (regs[j] == 0x12){
							printf("$s2");
						} else if (regs[j] == 0x13){
							printf("$s3");
						} else if (regs[j] == 0x14){
							printf("$s4");
						} else if (regs[j] == 0x15){
							printf("$s5");
						} else if (regs[j] == 0x16){
							printf("$s6");
						} else if (regs[j] == 0x17){
							printf("$s7");
						} else if (regs[j] == 0x18){
							printf("$t8");
						} else if (regs[j] == 0x19){
							printf("$t9");
						} else if (regs[j] == 0x1a){
							printf("$k0");
						} else if (regs[j] == 0x1b){
							printf("$k1");
						} else if (regs[j] == 0x1c){
							printf("$gp");
						} else if (regs[j] == 0x1d){
							printf("$sp");
						} else if (regs[j] == 0x1e){
							printf("$fp");
						} else if (regs[j] == 0x1f){
							printf("$ra");
						} else {
							printf("\nRegister address not found");
							return 0;
						}

						if(func == 0x08){
							printf("\n");
							break;
						}

						if(temp == 1){
							j = 1;
						}

						if(j != 2){
							printf(", ");
						} else {
							printf("\n");
						}
					}
				}
			//jump instruction
			} else if(opcode == 0x02) {
				unsigned int addr = value << 6;
				addr = addr >> 4;

				printf("j 0x%08x\n", addr);
			//jump and link instruction	
			} else if (opcode == 0x03){
				unsigned int addr = value << 6;
				addr = addr >> 4;

				printf("jal 0x%08x\n", addr);
			//opcodes for all I type instructions
			} else {
				int immediate = 0;
				int beq = 0;
				int lui = 0;
				int lw = 0;
				int signedd = 0;
				//print out the type of instruction
				if(opcode == 0x0a){
					printf("slti ");
					signedd = 1;
					immediate = 1;
				} else if (opcode == 0x0b){
					printf("sltiu ");
					signedd = 1;
					immediate = 1;
				} else if (opcode == 0x04){
					printf("beq ");
					beq = 1;
					signedd = 1;
				} else if (opcode == 0x05){
					printf("bne ");
					beq = 1;
					signedd = 1;
				} else if (opcode == 0x0d){
					printf("ori ");
					signedd = 1;
				} else if (opcode == 0x08){
					printf("addi ");
					signedd = 1;
				} else if (opcode == 0x09){
					printf("addiu ");
					signedd = 1;
				} else if (opcode == 0x0c){
					printf("andi ");
					signedd = 1;
				} else if (opcode == 0x0f){
					printf("lui ");
					lui = 1;
					signedd = 1;
				} else if (opcode == 0x23){
					printf("lw ");
					lw = 1;
					signedd = 1;
				} else if (opcode == 0x24){
					printf("lbu ");
					lw = 1;
					signedd = 1;
				} else if (opcode == 0x25){
					printf("lhu ");
					lw = 1;
					signedd = 1;
				} else if (opcode == 0x28){
					printf("sb ");
					lw = 1;
					signedd = 1;
				} else if (opcode == 0x29){
					printf("sh ");
					lw = 1;
					signedd = 1;
				} else if (opcode == 0x2b){
					printf("sw ");
					lw = 1;
					signedd = 1;
				}else {
					printf("Opcode not found");
					return 0;
				}

				int j = 0;
				unsigned int rs = value << 6;
				rs = rs >> 27;
				
				unsigned int rt = value << 11;
				rt = rt >> 27;

				unsigned int imm = value << 16;
				imm = imm >> 16;
				unsigned int signimm = 0;
				unsigned int temp = imm >> 15;
			
				//check if the immediate is a negative number
				if (signedd == 1 && temp == 1){
					signimm = ~imm + (unsigned)0x1;
					signimm = signimm - 0xffff0000;
				} else {
					signimm = imm;
				}
				int signedimm = 0;
				//get the address for the beq instruction
				if(beq == 1){
					if((imm >> 15) == 1){
						signedimm = value << 16;
						signedimm = signedimm >> 16;
					}
				}
				//put registers and immidiate into int array
				unsigned int regs[3] = {rt, rs, signimm};
			
				//switch the immidiate and rs if we have a lw or sw instruction
				if(lw == 1){
					unsigned int t = regs[1];
					regs[1] = regs[2];
					regs[2] = t;	
					
				}
				//loop over the array and print the registers/immidiate
				for(j = 0; j < 3; j++){
					//print negative if value was negative
					if(((imm >> 15) == 1) && lw == 0 &&  j == 2){
						printf("-");
					}
					//if there is a lw/sw, check if value was negative
					if(lw == 1 && j == 1){
						if(temp == 1){
							printf("-%d(", signimm);
							continue;
						}
						printf("0x%04x(", imm);
						continue;
					}
					//if lui instruction, only print the immidate
					if(lui == 1 && j == 1){
						printf("0x%04x\n", imm);
						break;
					}
					//if we have a beq instruciton
					if(j == 2 && beq == 1){
						int temp = imm >> 15;
						if(temp == 1){
							signedimm = addr + (4*signedimm);
							
							printf("0x%08x\n", signedimm);
							break;
						} else{
							imm = addr + (4*imm);
						}
						printf("0x%08x\n", imm);
						break;
					}
					//check if immidate is negative for every other instruction
					if(j == 2 && lw == 0){
						if(temp == 1 && signedd == 1){
							printf("%d\n", signimm);		
							break;
						}
						printf("0x%04x\n", signimm);
						break;	
					}else if(regs[j] == 0){
						printf("$zero");
					} else if (regs[j] == 1){
						printf("$at");
					} else if (regs[j] == 2){
						printf("$v0");
					} else if (regs[j] == 3){
						printf("$v1");
					} else if (regs[j] == 4){
						printf("$a0");
					} else if (regs[j] == 5){
						printf("$a1");
					} else if (regs[j] == 6){
						printf("$a2");
					} else if (regs[j] == 7){
						printf("$a3");
					} else if (regs[j] == 8){
						printf("$t0");
					} else if (regs[j] == 9){
						printf("$t1");
					} else if (regs[j] == 0x0a){
						printf("$t2");
					} else if (regs[j] == 0x0b){
						printf("$t3");
					} else if (regs[j] == 0x0c){
						printf("$t4");
					} else if (regs[j] == 0x0d){
						printf("$t5");
					} else if (regs[j] == 0x0e){
						printf("$t6");
					} else if (regs[j] == 0x0f){
						printf("$t7");
					} else if (regs[j] == 0x10){
						printf("$s0");
					} else if (regs[j] == 0x11){
						printf("$s1");
					} else if (regs[j] == 0x12){
						printf("$s2");
					} else if (regs[j] == 0x13){
						printf("$s3");
					} else if (regs[j] == 0x14){
						printf("$s4");
					} else if (regs[j] == 0x15){
						printf("$s5");
					} else if (regs[j] == 0x16){
						printf("$s6");
					} else if (regs[j] == 0x17){
						printf("$s7");
					} else if (regs[j] == 0x18){
						printf("$t8");
					} else if (regs[j] == 0x19){
						printf("$t9");
					} else if (regs[j] == 0x1a){
						printf("$k0");
					} else if (regs[j] == 0x1b){
						printf("$k1");
					} else if (regs[j] == 0x1c){
						printf("$gp");
					} else if (regs[j] == 0x1d){
						printf("$sp");
					} else if (regs[j] == 0x1e){
						printf("$fp");
					} else if (regs[j] == 0x1f){
						printf("$ra");
					} else {
						printf("\nRegister address not found");
						return 0;
					}

					if( j == 2 && lw == 1){
						printf(")\n");
						break;
					}

					if(j <= 1){
						printf(", ");
					} else {
						printf("\n");
					}

				}				
			}

 			count+=4;	
		}
		} else if (type == 8){
			printf(".data ");
		
			//get the address for the type
			unsigned int addr = 0;
			fread(&addr, 4, 1, fd);
			printf("0x%08x\n", addr);

			//get the size for the data
			unsigned int size = 0;
			fread(&size, 4, 1, fd);
			//read in a byte and print it out 
			while(count < size){
				unsigned int  byte = 0;
				fread(&byte, 1, 1, fd);
				printf(".byte 0x%02x\n", byte);
				count++;
			}
			
		} else {
			error();
			return 0;
		}
		count = 0;
	}
	
	return 0;
}

void error(){
	printf("Incorrect file format\n");
}
