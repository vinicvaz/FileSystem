#ifndef FUNCAO_H_INCLUDED
#define FUNCAO_H_INCLUDED


#define INI_CONTEUDO 300
#define FIM_CONTEUDO 500

#define INI_INODES 1
#define FIM_INODES 200

#define INI_GERENCIA 200 
#define FIM_GERENCIA 300
#define Limpa_Tela system("/usr/bin/clear") //PARA LIMPAR A TELA
#define TAMANHO_INODE 14
#define TAMANHO_BLOCO 44

int contador_nodes;
int contador_bloco;
int contador_conteudo_dir;
int contador_nodes_root;
int contador_livre;



struct blocos
{
	int conteudo[10];
	int comeco;

};
typedef struct  blocos blocos;



struct inode{

	char nome[10];
	//char data;
	int bloco;

};
typedef struct inode inode;

void inserir_conteudo(int bloco_escrita);
void criar_arquivo_diretorio(int ref);
void criar_diretorio_arquivo(int choice);
void remover();
void listar();

inode node[5];
blocos bloco[5];

#endif