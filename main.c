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

#include "funcao.h"





int menu(){
	int choice;

	printf("\nSelecione a opcao desejada: \n1 - Criar Diretorio\n2 - Criar Arquivo\n3-Remover Arquivo/Diretorio\n4-Lista Conteudo\n>> ");
	scanf("%d", &choice);
	while(choice!=1 && choice!=2 && choice!=3 && choice !=4){
		printf("\nEscolha invalida, selecione uma correta: ");
		scanf("%d", &choice);
	}
	return choice;

}

int main(){

	FILE* arq;
	blocos bloco[5];
	inode node[5];
	int end=1;
	int escolha;




	arq=fopen("server_arquivo", "r+b");

//////////////// **DEFININDO DIRETORIO RAIZ** //////////////////

	strcpy(node[0].nome, "root");
	node[0].bloco = 0;
	bloco[0].comeco = INI_CONTEUDO;

	fwrite(&node[0], sizeof(node[0]), 1, arq);
	contador_bloco=contador_bloco+1;
	contador_nodes=contador_nodes+1;
	fclose(arq);


///////////////////////////////////////////////////////////////

	while(end==1){
		escolha=menu();
		if(escolha==1)
			criar_diretorio_arquivo(1);
		if(escolha==2)
			criar_diretorio_arquivo(2);
		if(escolha==3)
			remover();
		if(escolha==4)
			listar();
		printf("\nDeseja realizar outra operacao?\n1-Sim\n2-Nao");
		scanf("%d",&end);
		Limpa_Tela;
		while(end!=1 && end!=2){
			printf("\nDigite valor valido: ");
			scanf("%d", &end);
		}
	}

	





}