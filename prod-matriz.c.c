/*************************************************************************************************/
/***************************** Trabalho de Sistemas Operacionais *********************************/
/*************************************************************************************************/
/********* Nome: Pedro Daniel Camargos Soares 			Matricula: 0020640	**********************/
/*************************************************************************************************/
/********* Nome: Lucas Gabriel de Almeida			 	Matricula: 0035333	**********************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Para compilar o codigo usando Thread usase o comando 		**********************************/
/* 					gcc -pthread 'Nome_arquivo.c' -o 'Nome_Arquivo_saida'
/* Para Executar o programa compilado usase o comando 			**********************************/
/*					./main matriz1.txt matriz2.txt													


/************************************* Importa as bibliotecas ************************************/
#include<stdlib.h> //Biblioteca basica C
#include<stdio.h> //Biblioteca basica C
#include<pthread.h> //Biblioteca de threads do linux, necessaria para as chamadas de sistema com threads
/*************************************************************************************************/

float **matriz1,**matriz2,**matriz3; // Tres matrizes dinamicas,
// A matriz 1 e 2 receberao os numeros reais armazenados dentro dos dois
// arquivos que serao passadas por parametro, e a matriz 3 sera responsavel por armazenar o 
// resultado da multiplicacao entra as matrizes 1 e 2.

//Variaveis responsaveis por ler os dados armazenados na matriz
int linha_matriz1, coluna_matriz1, linha_matriz2, coluna_matriz2, linha_matriz3, coluna_matriz3;

typedef struct{ // Estrutura de dados usada na multiplicacao das matrizes
		int linha; //Recebera a linha
		int coluna; //Recebera a coluna
	} argumentos_thread;


void* calcular_cij(void *argumentos){ //Funcao que ira multiplar os numeros das matrizes

	argumentos_thread * argu = (argumentos_thread *) argumentos; //Recebe as Threads
	
	matriz3[argu->linha][argu->coluna] = 0; //Define como 0 o valor do numero na posicao linha coluna
	
	int i; //Contador que sera usado para realizar a multiplicacao
	for(i=0; i<coluna_matriz1; i++){
		
		matriz3[argu->linha][argu->coluna] += matriz1[argu->linha][i] * matriz2[i][argu->coluna];
		// A matriz3 (resultante) na posicao Linha/Coluna (passados por parametro pela funcao pthread_create();
		// Recebe o valor da multiplicacao da Matriz1 pela matriz2.
		// Ou seja, CMN = AMK × BKN.		
	}
}

int ler_arquivo(int argc, char** argv){ //Funcao que ira ler os arquivos de entrada (matrizes)
	FILE *Entrada1,*Entrada2; //Cria um ponteiro para o arquivo de entrada
	Entrada1 = fopen(argv[1],"r"); //Abre o arquivo
	Entrada2 = fopen(argv[2],"r"); //Abre o arquivo

	if (Entrada1 == NULL){ //Verifica se o arquivo foi aberto com sucesso
		printf("O arquivo nao pode ser aberto!\n");
		exit(EXIT_FAILURE);
	}
	if (Entrada2 == NULL){ //Verifica se o arquivo foi aberto com sucesso
		printf("O arquivo nao pode ser aberto!\n");
		exit(EXIT_FAILURE);
	}
	
	fscanf(Entrada1,"%d", &linha_matriz1); //Le o numero de linhas da matriz 1
	fscanf(Entrada1,"%d", &coluna_matriz1); //Le o numero de colunas da matriz 1
	
	fscanf(Entrada2,"%d", &linha_matriz2); //Le o numero de linhas da matriz 2
	fscanf(Entrada2,"%d", &coluna_matriz2); //Le o numero de colunas da matriz 2
	
	if(coluna_matriz1 != linha_matriz2){ //Verifica se a propriedade de multiplicacao de matrizes e possivel
		printf("Matrizes nao podem ser multiplicadas!\n");
		exit(EXIT_FAILURE);
	}
	
	//Seguindo a propriedade de multiplicacao de matrizes,
	//a matriz resultante recebera:
	linha_matriz3 = linha_matriz1; // o numero de linhas da matriz 1,
	coluna_matriz3 = coluna_matriz2;// e o numero de colunas da matriz 2
	
	//Aloca dinamicamente as matrizes
	matriz1 = malloc(linha_matriz1*sizeof(float*)); 
	matriz2 = malloc(linha_matriz2*sizeof(float*));
	matriz3 = malloc(linha_matriz3*sizeof(float*));
	
	int i,j; //Contadores, usados para percorrer as linhas e colunas da matrizes
	
	for (i = 0; i < linha_matriz1; i++){ //Varre a primeira matriz
	
    	matriz1[i] = malloc(coluna_matriz1 * sizeof(float)); //aloca a primeira matriz
    	
		for (j = 0; j < coluna_matriz1; j++){ 
		
			fscanf(Entrada1,"%f", &matriz1[i][j]); //Le os dados da matriz 1
			
		}
    }
    
	printf("\n\n");
	fclose(Entrada1); //Fecha o arquivo 1
	
	for (i = 0; i < linha_matriz2; i++){ //Varre a segunda matriz
	
    	matriz2[i] = malloc(coluna_matriz2 * sizeof(float)); //Aloca a segunda matriz
    	
		for (j = 0; j < coluna_matriz2; j++){ 
		
			fscanf(Entrada2,"%f", &matriz2[i][j]); //Le os dados da matriz 2

		}
    }
    
	fclose(Entrada2); //Fecha o arquivo
	
    for (i = 0; i < linha_matriz3; i++){ //Laço de repeticao que ira alocar a matriz resultante
    
    	matriz3[i] = malloc(coluna_matriz3 * sizeof(float));
    	
    }
	
}

int main (int argc, char** argv) {
	
	int i, j, k; //Variaveis utilizadas no laço de repeticao
		
	//entrada de dados
	ler_arquivo(argc,argv); //Chama a funcao para ler os arquivos passados por paremetro 
	
	
	pthread_t treads[linha_matriz3*coluna_matriz3]; //Crias as threads
	//A funcao pthread_t ira criar X threads com base nas dimensoes da matriz resultante.

	// O comando a seguir cria um vetor argu, de tamanho baseado na dimensao da matriz
	// Ele sera responsavel por ajudar a criar as threads
	// Lembrando que a estrutura argumentos_thread armazena as linha e coluna  que cada thread sera responsavel
	argumentos_thread argu[linha_matriz3*coluna_matriz3]; 
	
	int x = 0;
	
	//Cria as threads
	for(i=0; i < linha_matriz3; i++){
		
		for(j=0; j < coluna_matriz3; j++){
			
			//Define as coordenadas que a thread sera responsavel por multiplicar
			argu[x].linha = i; 
			argu[x].coluna = j;
			
			pthread_create(&treads[x], NULL, calcular_cij, (void*)&argu[x]); //Funcao responsavel por criar as threads
			
			/*
				O primeiro parametro (&treads[x]) ira armazenar a informacao sobre a thread criada
				O segundo parametro especifica algumas propriedades para a thread, no caso, passado como NULL para nao altera-la
				O terceiro chama a funcao cuja a thread sera responsavel por executar, no caso a multiplicacao das matrizes
				O quarto passa os argumentos para a funcao, no caso a linha e a coluna a serem multiplicadas
			*/
			
			x++; //Atualiza o contador para usar outra thread
			
		}
	}
	
	// O laco a seguir faz com que as threads que ja tenham terminado sua execucao
	// Esperem por aquelas que ainda nao terminaram
	for(x=0; x < linha_matriz3*coluna_matriz3; x++){
		
		pthread_join(treads[x], NULL); //Indica qual thread devera entrar em modo de espera
		
	}
	
	FILE *saida; //Cria um ponteiro para o arquivo de saida
	saida = fopen("resultado.txt","w"); //Cria o arquivo de saida (matriz resultante)
	fprintf(saida,"%d\n",linha_matriz3); //Escreve o numero de linhas da matriz resultante
	fprintf(saida,"%d\n",coluna_matriz3); //Escreve o numero de colunas da matriz resultante
	
	//Escreve todos os valores LinhaXColuna da matriz resultante
	for (i = 0; i < linha_matriz3; i++){
		
    	for (j = 0; j < coluna_matriz3; j++){
    		
			fprintf(saida,"%f ",matriz3[i][j]); //Escreve no arquivo
    		
       }
       
	fprintf(saida,"\n");
	
	}
	
	fclose(saida); //Fecha o arquivo de saida
	
	//Mostra o resultado na tela
	printf("%d\n",linha_matriz3); //Exibe o numero de linhas da matriz resultante
	printf("%d\n",coluna_matriz3); //Exibe o numero de colunas da matriz resultante
	
	//Exibe todos os valores LinhaXColuna da matriz resultante
	for (i = 0; i < linha_matriz3; i++){
		
    	for (j = 0; j < coluna_matriz3; j++){
    		
			printf("%f ",matriz3[i][j]); //Exibe na tela
    		
       }
       
	printf("\n");
	}
	
	//Libera a memoria alocada pelas matrizes
	for (i = 0; i < linha_matriz1; i++){ //Libera cada elemento da matriz 1
		
    	free(matriz1[i]);
    	
		}
		
    free(matriz1); //Libera a matriz 1
	
	for (i = 0; i < linha_matriz2; i++){ //Libera cada elemento da matriz 2
	
    	free(matriz2[i]);
    	
    }
	free(matriz2); //Libera a matriz 2
	
    for (i = 0; i < linha_matriz3; i++){ //Libera cada elemento da matriz 3
    
    	free(matriz3[i]);
    	
    }
	free(matriz3); //Libera a matriz 3
	
	exit(EXIT_FAILURE); //Termina o programa
	
}
