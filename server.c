#include<stdio_ext.h>
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
//////////////////////////////////////////////////////////////////////////////
#define LIMITE_SIMULTANEO 5 // LIMITE DE CONEXOES 
#define Limpa_Tela system("/usr/bin/clear") //PARA LIMPAR A TELA
//////////////////////////////////////////////////////////////////////////////


pthread_mutex_t mutex[3] = PTHREAD_MUTEX_INITIALIZER;

int i=0;
//////////////////////////////////////////////////////////////////////////////
void copiar(FILE* arquivo, FILE* arquivo1){ // USADO NA FUNCAO COPY PARA COPIAR O CONTEUDO DE UM ARQUIVO PARA OUTRO
	char leitor[1000];
	
	while(fgets(leitor,1000,arquivo) != NULL)

		fputs(leitor,arquivo1);
		

}
//////////////////////////////////////////////////////////////////////////////
int copy(char* nome, char* nome2){ // VERIFICA SE O ARQUIVO EXISTE, LE ELE, CRIA O ARQUIVO DESTINO E CHAMA FUNCAO COPIAR
	
	
	FILE * arquivo = fopen(nome, "r");

	if(arquivo==NULL){
		printf("Errro na copia");
		return -1;
	}

	FILE* arquivo1=fopen(nome2, "w");
	copiar(arquivo,arquivo1);

	fclose(arquivo);
	fclose(arquivo1);
	return 0;


}
//////////////////////////////////////////////////////////////////////////////
void * conection(void *arg){ // LIDA COM A THREAD DE FORMA A FAZER CADA OPERACAO DESEJADA 
	int client = *(int*)arg;

	char buff[128];
	char nome[100];
	char nome2[100];
	int escolha;
	char menu[]="MENU\n\n 1-Criar Arquivo\n 2-Criar Diretorio\n 3-Remover Arquivo\n 4-Remover Diretorio\n 5-Copiar dados\n 6-Listar conteudo\n Digite o valor desejado: ";
	char arq[]="Digite o nome do arquivo: ";
	char dir[]="Digite o nome do diretorio: ";
	char ctrlc[]="Digite o nome do arquivo a ser copiado: ";
	char ctrlv[]="Digite o nome do arquivo destino: ";
	int v;
	int sair=1;

	int x;

	printf("Client numero: %d\n", client);

	while(sair==1){
	send(client,menu,sizeof(menu),0);
	recv(client,buff,sizeof(buff),0);
	escolha=atoi(buff);
	printf("Escolha: %d\n",escolha);
	while(escolha!=1 && escolha!=2 && escolha!=3 && escolha!=4 && escolha!=5 && escolha!=6 ){
		send(client,"Valor invalido, digite outro: ", sizeof("Valor invalido, digite outro: "),0);
		recv(client,buff,sizeof(buff),0);
		escolha=atoi(buff);
		memset(buff, 0, sizeof(buff));
	}
	fflush(stdout);
//////////////////////////////////////////////////////////////////////////////
	if(escolha==1 || escolha==3){ // CRIAR E DELETAR ARQUIVOS
		pthread_mutex_lock(&mutex[0]); // MUTEX LOCK < --------
		send(client,arq,sizeof(arq),0);
		recv(client,nome,sizeof(nome),0);
		fflush(stdout);
		printf("Nome arq escolhido: %s\n", nome);
		FILE* arq;

		if(escolha==1){ 
			arq=fopen(nome,"w");
			send(client,"Arquivo criado", sizeof("Arquivo criado"),0);
		}
		else{
			v=remove(nome);
			if(v==0){
				send(client,"Arquivo removido",sizeof("Arquivo removido"),0);
			}else{
				send(client,"Erro ao remover, arquivo inexistente",sizeof("Erro ao remover, arquivo inexistente"),0);

			}
		}
		memset(nome, 0, sizeof(nome));
		__fpurge(stdin);
		fflush(stdout);
		pthread_mutex_unlock(&mutex[0]); // MUTEX UNLOCK < -----
	}
//////////////////////////////////////////////////////////////////////////////
	if(escolha==2 || escolha==4){ // CRIAR E DELETAR DIRETORIOS 
		pthread_mutex_lock(&mutex[1]); // MUTEX LOCK < -----
		send(client,dir,sizeof(dir),0);
		recv(client,nome,sizeof(nome),0);
		fflush(stdout);
		printf("Nome dir escolhido: %s\n", nome);
		if(escolha==2){
			FILE* arq;
			mkdir(nome,  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			send(client, "Diretorio criado", sizeof("Diretorio criado"),0);
		}else{
			v=rmdir(nome);
			if(v==0){
				send(client,"Diretorio removido",sizeof("Diretorio removido"),0);
			}else{
				send(client,"Erro ao remover, diretorio inexistente ou com conteudo", sizeof("Erro ao remover, diretorio inexistente ou com conteudo"),0);
			}
		}
		memset(nome, 0, sizeof(nome));
		pthread_mutex_unlock(&mutex[1]); // MUTEX UNLOCK <-----
	}
//////////////////////////////////////////////////////////////////////////////
	if(escolha==5){ //COPIAR CONTEUDO 
		pthread_mutex_lock(&mutex[2]); //MUTEX LOCK < ----
		send(client,ctrlc,sizeof(ctrlc),0);
		recv(client,nome,sizeof(nome),0);
		fflush(stdout);
		printf("Arquivo escolhido para copia: %s\n",nome);
		send(client,ctrlv,sizeof(ctrlv),0);
		recv(client,nome2,sizeof(nome2),0);
		fflush(stdout);
		printf("Arquivo destino: %s\n", nome2);
		v=copy(nome,nome2);
		if(v==0){
			send(client,"Conteudo copiado",sizeof("Conteudo copiad"),0);
		}else{
			send(client,"Erro ao copiar",sizeof("Erro ao copiar"),0);
		}

		memset(nome, 0, sizeof(nome));
		memset(nome2, 0, sizeof(nome2));

		pthread_mutex_unlock(&mutex[2]); // MUTEX UNLOCK < ---
//////////////////////////////////////////////////////////////////////////////
	}
	if(escolha==6){ // LISTAR CONTEUDO 
		DIR *d;
		struct dirent *dir;
		d = opendir(".");
		fflush(stdout);
		fflush(stdin);
		char *aux;
 		if (d) {
			while ((dir = readdir(d)) != NULL) {
		    	printf("%s\n", dir->d_name);
		    	aux = dir->d_name;
		    	send(client,"\n",sizeof("\n"),0);
		    	send(client,aux,sizeof(aux),0);
		    	

		    }
		    closedir(d);
		  }

	    printf("Conteudo listado");
	}
//////////////////////////////////////////////////////////////////////////////
	send(client, "\nDeseja realizar outra operacao? 1-Sim, 2-Nao: ", sizeof("Deseja realizar outra operacao? 1-Sim, 2-Nao: "),0);
	recv(client, buff, sizeof(buff),0);
	sair=atoi(buff);
	while(sair!=1 && sair!=2){
		send(client, "\nValor invalido, digite outro: ", sizeof("Valor invalido, digite outro: "),0);
		recv(client,buff,sizeof(buff),0);
		sair=atoi(buff);
	}
	send(client,"\e[2J\e[H",sizeof("\e[2J\e[H"),0);
	fflush(stdout);
	memset(buff, 0, sizeof(buff));
}
	

pthread_exit(NULL);
		





}
//////////////////////////////////////////////////////////////////////////////
int main()
{
//////////////////////////////////////////////////////////////////////////////
	
	struct sockaddr_in client_addr ;  
	struct sockaddr_in server_addr =
	{
		.sin_family = AF_INET,
		.sin_addr.s_addr = htonl(INADDR_ANY),
		.sin_port = htons(8080)
	};
//////////////////////////////////////////////////////////////////////////////
	int server = socket(AF_INET,SOCK_STREAM,0);
	bind(server,(struct sockaddr*)&server_addr,sizeof(server_addr));

	int client;
	int csize = sizeof(client_addr);
//////////////////////////////////////////////////////////////////////////////

	listen(server,LIMITE_SIMULTANEO );
//////////////////////////////////////////////////////////////////////////////
	pthread_t thread;
	int choice;
//////////////////////////////////////////////////////////////////////////////
	for (;;){
	while((client = accept(server,(struct sockaddr *)&client_addr,&csize))!=-1){
		pthread_create(&thread,NULL,conection,(void*)&client);
		pthread_join(thread,NULL);

		//i++;


		}
		close(client);
	}
	return 0;
}
//////////////////////////////////////////////////////////////////////////////