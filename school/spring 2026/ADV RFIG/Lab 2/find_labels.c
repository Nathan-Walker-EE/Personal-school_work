#include "structs.h"

label* find_labels(char* dlx_file){
	label* labels;
	
	FILE* fp = fopen(dlx_file, "r");
	if(fp == NULL){
		printf("failed to open file\n");
		exit(1);
	}
	
	char line[256];
	int text_label = 0;
	
	while(fgets(line,sizeof(line),fp)){
		char *comment = strchr(line, ';');
		
		char* token = strtok(line, " ,\t\n\r");
		
		while(token != NULL && comment == 0){
			if(strcmp(token, ".text") == 0){break; text_label=1;}
			token = strtok(NULL," ,\t\n\r");
		}
		if(text_label)break;
	}
	
	int opcode_flag = 0;
	int addr = 0;
	while(fgets(line,sizeof(line), fp){
		char* comment = strchr(line,';');
		char* token = strtok(line, " ,\t\n\r");
		
		for(int i = 0; i < 49 && comment == 0; i++){
			if(strcmp(token, opcodes[i].name) == 0){opcode_flag=1;}
		}
		if(!opcode_flag){
			labels = (label*)realloc(labels, sizeof(label)*(addr+1));
			labels[addr].label = strdup(token);
			labels[addr].address = addr++;
		}
	}
}