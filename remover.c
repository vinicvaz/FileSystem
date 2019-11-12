#include "funcao.h"
#include <stdio.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////////
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
//////////////////////////////////////////////////////////////////////////////
#include <sys/un.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>					
#include <sys/stat.h>
#include <dirent.h>
#include <dirent.h>
#include <sys/wait.h>

void remover(){
	char buffer[10];
	FILE* arq;
	int teste=0;
	int i=0;
	int valor=-1;
	char zera_inode[14] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
	int valor_bloco;
	char zera_bloco[44];
	char zera_nome[10] = "\0\0\0\0\0\0\0\0\0\0";

	for(int m=0;m<44;m++){
		zera_bloco[m]='\0';
	}

	printf("\nDigite o nome a ser removido: ");
	scanf(" %[^\n]s", buffer);

	while(i<5 && teste==0){
		//printf("\ncomparacao %d, inode %d\n string %s e name %s", i, i,buffer,node[i].nome);
		if((strcmp(buffer,node[i].nome))==0){
			printf("\nINODE que tem o nome e %d",i);
			valor = i;
			teste=1;
		}
		i++;
	}
		
	if(valor==-1){
		printf("\nArquivo nao encontrado.");
	}else{

		////////remover do arquivo o inode//////
		arq=fopen("server_arquivo", "r+b");
		fseek(arq,(valor*TAMANHO_INODE)-1,SEEK_SET);
		printf("\nestou na posicao %ld pronto para ser removido", ftell(arq));
		fwrite(zera_inode,TAMANHO_INODE,1,arq);
		fclose(arq);
		/////////remover do arquivo conteudo do bloco que ele aponta///
		arq=fopen("server_arquivo","r+b");
		valor_bloco=node[valor].bloco;
		printf("\nVALOR BLOCO INODE: %d", valor_bloco);
		fseek(arq,300+(valor_bloco*TAMANHO_BLOCO)-1,SEEK_SET);
		printf("\nEstou na posicao %ld", ftell(arq));
		fwrite(zera_bloco,TAMANHO_BLOCO,1,arq);
		fclose(arq);


		//////////remover da struct correspondente/////////
		strcpy(node[valor].nome,zera_nome);
		node[valor].bloco = 0;

		contador_livre=contador_livre+1;

		printf("\nAgora voce tem %d inodes livres, total de %d bytes para inodes", contador_livre, TAMANHO_INODE*contador_livre);

		

	}
 
 
}

void listar(){
	int i;

	printf("\nO conteudo e:\n");
	for(i=1;i<contador_nodes;i++){
		printf("%s\n", node[i].nome);
	}
}