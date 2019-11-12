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

int contador_livre=5;

/////////FUNCAO QUE INSERE CONTEUDO EM UM ARQUIVO/////////
void inserir_conteudo(int bloco_escrita){
	char buffer[40];
	int conversao;
	FILE* arq;
	int inicio;
	blocos bloco[5];

	for (int m = 0; m < 40; ++m)
		{
			buffer[m]='\0';
		}
	fflush(stdout);
	setbuf(stdin,NULL);

	//printf("\nEstou no bloco: %d e o conteudo sera salvo aqui", contador_bloco-1);
	printf("\n\nDigite o conteudo a ser inserido no seu arquivo: ");
	scanf(" %[^\n]s", buffer);

	////// converter conteudo char para int/////
	conversao = atoi(buffer);
	///////inserir conteudo na struct correspondente///
	bloco[contador_bloco-1].conteudo[0]=conversao;

	/////// escrever no bloco respectivo no arquivo /////
	arq=fopen("server_arquivo", "r+b");
	fseek(arq, 300+((contador_bloco-1)*sizeof(blocos)),SEEK_SET);
	inicio=ftell(arq);
	printf("\nSou o bloco %d e minha posicao inicial e %d\nO conteudo de seu arquivo esta aqui", contador_bloco-1, inicio);
	fwrite(buffer, sizeof(buffer), 1,arq);
	fclose(arq);


}


////FUNCAO QUE ADD UM ARQUIVO DENTRO DO DIRETORIO - APENAS UM /////
void criar_arquivo_diretorio(int ref){
	FILE* arq;
	int inicio;
	char buffer[200];
	int i=0;
	int n0=0;
	int test=0;
	int tamanho;

	printf("\nDigite o nome do arquivo: ");
	scanf(" %[^\n]s", node[contador_nodes].nome);
	tamanho = strlen(node[contador_nodes].nome);

	node[contador_nodes].bloco=contador_bloco;

//////// ACHA O INICIO DO BLOCO RESPECTIVO AO INODE////
	arq=fopen("server_arquivo", "r+b");
	fseek(arq, 300+(contador_bloco*sizeof(blocos)),SEEK_SET);
	inicio=ftell(arq);
	bloco[contador_bloco].comeco = inicio;
	fclose(arq);

//////////ESCREVENDO ARQUIVO NA REGIAO DE INODES//////
	arq=fopen("server_arquivo","r+b");
	fread(buffer,INI_INODES,FIM_INODES,arq);
	i=0;
	n0=0;
	test=0;

	node[contador_nodes].bloco = contador_bloco;

	while(i<200 && test==0){

		if(buffer[i]==0){
				n0=n0+1;
			}
			if(n0>tamanho){
				fseek(arq, (i-tamanho)+1, SEEK_SET);
				fwrite(&node[contador_nodes], sizeof(node[contador_nodes]), 1,arq);
				fclose(arq);
				test=1;
			}
			if(buffer[i]!=0){
				n0=0;
			}
			i++;

	}


	printf("\n\nO arquivo criado aponta o bloco %d que comeca em %d",node[contador_nodes].bloco, bloco[contador_bloco].comeco);
	printf("\n\nEsse arquivo tem como valor de inode %d", contador_nodes);

	///////////escrevendo o valor do inode criado no bloco da root//////////
	bloco[contador_bloco-1].conteudo[0] = contador_nodes;
	/////////////////ESCREVENDO ISSO NO ARQUIVO BIN/////////////
	arq=fopen("server_arquivo", "r+b");
	fseek(arq,INI_CONTEUDO,SEEK_SET);
	fwrite(&bloco[contador_bloco-1].conteudo[contador_nodes-1],sizeof(bloco[contador_bloco-1]),1,arq);
	fclose(arq);

	contador_conteudo_dir=contador_conteudo_dir+1;

	printf("\n\nO bloco %d contem o inode: ",contador_bloco-1);
	for(int i=0;i<contador_conteudo_dir;i++){
		printf("%d ", bloco[contador_bloco-1].conteudo[i]);
	}

	////////// AUMENTANDO VALOR DAS VARIAVEIS GLOBAIS /////////
	contador_nodes=contador_nodes+1;
	contador_bloco=contador_bloco+1;
	contador_livre=contador_livre-1;
	///////////////////////////////////////
	printf("\nAgora voce tem %d inodes livres, total de %d bytes para inodes", contador_livre, TAMANHO_INODE*contador_livre);

	int escolha;
	printf("\n\nDeseja adicionar conteudo no arquivo? \n1-Sim\n2-Nao");
	scanf("%d", &escolha);
	while(escolha!=1 && escolha!=2){
		printf("\nValor invalido, digite outro: ");
		scanf("%d", &escolha);
	}
	if(escolha==1){
		inserir_conteudo(node[contador_nodes-1].bloco);
	}


}




///////////FUNCAO QUE CRIA DIRETORIOS E ARQUIVOS NO ROOT//////////
void criar_diretorio_arquivo(int choice){


	FILE* arq;
	int inicio;
	char buffer[200];
	int i=0;
	int n0=0;
	int test=0;
	int tamanho;

	////LIMPA BUFFER//
	for (int m = 0; m < 145; ++m)
	{
			buffer[m]='\0';
	}
	

	fflush(stdout);
	setbuf(stdin,NULL);

	
	if(choice==1)
		printf("\nDigite o nome do diretorio: ");
	else
		printf("\nDigite o nome do arquivo: ");
	scanf(" %[^\n]s", node[contador_nodes].nome);
	tamanho = strlen(node[contador_nodes].nome);



	node[contador_nodes].bloco = contador_bloco;

	arq=fopen("server_arquivo", "r+b");
	//////// ACHA O INICIO DO BLOCO RESPECTIVO AO INODE////
	fseek(arq, 300+(contador_bloco*sizeof(blocos)),SEEK_SET);
	inicio=ftell(arq);
	bloco[contador_bloco].comeco = inicio;
	fclose(arq);


	///////escrever diretorio na regiao de inodes////
	arq=fopen("server_arquivo","r+b");
	fread(buffer,INI_INODES,FIM_INODES,arq);
	i=0;
	n0=0;
	test=0;
	while(i<200 && test==0){

		if(buffer[i]==0){
				n0=n0+1;
			}
			if(n0>tamanho){
				fseek(arq, (i-tamanho)+1, SEEK_SET);
				fwrite(&node[contador_nodes], sizeof(node[contador_nodes]), 1,arq);
				fclose(arq);
				test=1;
			}
			if(buffer[i]!=0){
				n0=0;
			}
			i++;

	}


	contador_nodes_root=contador_nodes_root+1;
	///////////escrevendo o valor do inode criado no bloco da root//////////
	//bloco[0].conteudo[contador_nodes-1] = contador_nodes;
	bloco[0].conteudo[contador_nodes_root-1]=node[contador_nodes].bloco;
	/////////////////ESCREVENDO ISSO NO ARQUIVO BIN/////////////
	arq=fopen("server_arquivo", "r+b");
	fseek(arq,INI_CONTEUDO,SEEK_SET);
	fwrite(&bloco[0].conteudo[contador_nodes-1],sizeof(bloco[0]),1,arq);
	fclose(arq);

	printf("\n\nBLOCO DO ROOT CONTEM NODES:");
	for(int i=0;i<contador_nodes_root;i++){
		printf("%d ", bloco[0].conteudo[i]);
	}

	if(choice==1)
		printf("\n\nO diretorio criado aponta o bloco %d que comeca em %d",contador_bloco, bloco[contador_bloco].comeco);
	else
		printf("\n\nO Arquivo criado aponta o bloco %d que comeca em %d",contador_bloco, bloco[contador_bloco].comeco);

	////////////modificando variaveis//////
	contador_livre=contador_livre-1;
	contador_bloco=contador_bloco+1;
	contador_nodes=contador_nodes+1;
	printf("\nAgora voce tem %d inodes livres, total de %d bytes para inodes", contador_livre, TAMANHO_INODE*contador_livre);

	int escolha;
	if(choice==1){
		printf("\nDeseja inserir um arquivo nesse diretorio? \n1-Sim\n2-Nao");
		scanf("%d",&escolha);
		while(escolha!=1 && escolha !=2){
			printf("\nValor invalido, digite outro: ");
			scanf("%d", &escolha);
		}
		if(escolha==1){
			criar_arquivo_diretorio(node[contador_nodes-1].bloco);
		}
	}else{
		printf("\nDeseja inserir conteudo no arquivo? \n1-Sim\n2-Nao");
		scanf("%d", &escolha);
		while(escolha!=1 && escolha !=2){
			printf("\nValor invalido, digite outro ");
			scanf("%d", &escolha);
		}
		if(escolha==1){
			inserir_conteudo(node[contador_nodes-1].bloco);
		}
	}


}