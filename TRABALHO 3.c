#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct lista{   //estrutura de uma c�lula de lista encadeada
	float x;
	int linha;
	int coluna;
	struct lista *direita;
	struct lista *abaixo;
};


struct matriz{  
	struct lista **colunas;    //Base de dados como uma lista encadeada que pode ser percorrida
	struct lista **linhas;     //pelas colunas ou pelas linhas por meio de um vetor de vetores
	int linha;
	int coluna;
};


struct matriz * CreateMatriz(int i, int j){ //Fun��o que cria os vetores das colunas e das linhas
	struct matriz *M = (struct matriz *)malloc(sizeof(struct matriz));
	int l,c;
	M->linha = i;
	M->coluna = j;
	M->linhas = (struct lista **)malloc(sizeof(struct lista *)*i);
	M->colunas = (struct lista **)malloc(sizeof(struct lista *)*j);
	for (l=0;l<i;l++){
		M->linhas[l] = (struct lista *)malloc(sizeof(struct lista));
		M->linhas[l]->abaixo = NULL;
		M->linhas[l]->direita = NULL;
	}
	for (c=0;c<j;c++){
		M->colunas[c] = (struct lista *)malloc(sizeof(struct lista));
		M->colunas[c]->abaixo = NULL;
		M->colunas[c]->direita = NULL;
	}
	return (M);
}

void Exclude(struct matriz *M){  //Fun��o que libera toda a m�moria relacionada a matriz, excluindo-a
	int i;
	for (i=0;i<M->linha;i++){
		struct lista *aux = M->linhas[i]->direita;
		while (aux != NULL){
			struct lista *Exc = aux;
			aux = aux->direita;
			free(Exc);
		}
		free(M->linhas[i]);
	}
	free(M->colunas);
	free(M->linhas);
	free(M);
	M = NULL;
}


float ConsultaValor(struct matriz *M, int i, int j){  //Fun��o que retorna determinado valor de determinada posi��o da matriz
	if(i<=M->linha&&j<=M->coluna){
		struct lista *aux = M->linhas[i-1];
		while(aux->direita!=NULL&&aux->direita->coluna<=j){
			aux = aux->direita;
		}
		if(aux->coluna==j){
			return(aux->x);
		}
	}
	return (0);
}


float ConsultaSomaLinha(struct matriz *M, int i){  //Fun��o que retorna a soma de determinada linha da matriz
	if(i<=M->linha){
		float s = 0;
		struct lista *aux = M->linhas[i-1];
		while(aux!=NULL){
			s = s + aux->x;
			aux = aux->direita;
		}
		return (s);
	}
	return (0);
}


float ConsultaSomaColuna(struct matriz *M, int j){  //Fun��o que retorna a soma de determinada coluna da matriz
	if(j<=M->coluna){
		float s = 0;
		struct lista *aux = M->colunas[j-1];
		while(aux!=NULL){
			s = s + aux->x;
			aux = aux->abaixo;
		}
		return (s);
	}
	return (0);
}


void New(struct matriz *M, int i, int j, float x){   //Fun��o que adiciona uma c�lula com um valor � matriz
	if(i<=M->linha&&j<=M->coluna){
		struct lista *aux = M->linhas[i-1];
		while(aux->direita!=NULL&&aux->direita->coluna<=j){
			aux = aux->direita;
		}
		if(aux->coluna==j){
			aux->x = x;
		}
		else{
			struct lista *n = (struct lista *)malloc(sizeof(struct lista));
			n->linha = i;
			n->coluna = j;
			n->x = x;
			n->direita = aux->direita;
			aux->direita = n;
			aux = M->colunas[j-1];
			while(aux->abaixo!=NULL&&aux->abaixo->linha<=i){
				aux = aux->abaixo;
			}
			n->abaixo = aux->abaixo;
			aux->abaixo = n;
		}
	}
}


float DetM(struct matriz *M, int i, int j){  //Fun��o que calcula o determinante de uma matriz nxn
	if(i<M->linha&&j<M->coluna){             //int i e int j s�o n�meros auxiliares na orgaiza��o da fun��o, eles representam a posi��o de inicio
		float Det = 0;
	    struct lista *aux = M->colunas[j]->abaixo;
	    while(aux!=NULL){
		    if(aux->linha!=i){
			    if((aux->coluna+aux->linha)%(2) == 0){
				    Det = Det + (aux->x)*DetM(M,aux->linha,aux->coluna);
			    }
			    else{
				    Det = Det - (aux->x)*DetM(M,aux->linha,aux->coluna);
			    }
		    }
		    aux = aux->abaixo;
	    }
	    return (Det);
	}
	else{
		return (1);  //retorna 1 quando a fun��o j� percorreu toda a matriz, fechando o ciclo
	}
}


void main(){
	int menu,i,j;
	struct matriz *M;
	float x;
	int exit = 0;
	while(exit == 0){
		printf("\n\nDigite:\n\n");
		printf("1 para criar a matriz\n\n"); 
		printf("2 para adicionar valor\n\n"); 
		printf("3 para consulta de valor\n\n"); 
		printf("4 para consulta de soma de uma coluna\n\n");
		printf("5 para consulta de soma de uma linha\n\n");
		printf("6 para excluir matriz\n\n");
		printf("7 para calcular o determinante da matriz\n\n");
		printf("8 para sair\n\n");
    	scanf("%d",&menu);
    	switch(menu){
    		case 1:
	            printf("Para criar a matriz, digite o tamanho dela:");
	            printf("\nNumero de linhas: ");
	            scanf("%d",&i);
	            printf("Numero de colunas: ");
	            scanf("%d",&j);
	            M = CreateMatriz(i,j);
	            printf("Matriz criada!");
	            break;
	        case 2:
	        	printf("Digite a posicao desejada:\n");
            	printf("\nlinha: ");
            	scanf("%d",&i);
            	printf("coluna: ");
            	scanf("%d",&j);
            	printf("Digite o valor desejado: ");
            	scanf("%f",&x);
            	New(M,i,j,x);
            	printf("Valor adicionado com sucesso!");
		        break;
            case 3:
            	printf("Digite a posicao desejada:\n");
            	printf("linha: ");
            	scanf("%d",&i);
            	printf("coluna: ");
            	scanf("%d",&j);
            	printf("O valor nesse posicao eh %f",ConsultaValor(M,i,j));
    	        break;
            case 4:
            	printf("Digite a coluna desejada: ");
            	scanf("%d",&j);
            	printf("O valor da soma eh %f",ConsultaSomaColuna(M,j));
				break;
			case 5:
            	printf("Digite a linha desejada: ");
            	scanf("%d",&i);
            	printf("O valor da soma eh %f",ConsultaSomaLinha(M,i));
				break;
			case 6:
				Exclude(M);
				printf("A matriz foi excluida com sucesso!");
				break;
			case 7:
				if (M->linha==M->coluna){
				    printf("O determinante eh igual a %f", DetM(M,0,0));	
				}
			    else{
			    	printf("Nao existe determinante!");
				}
				break;		
    	    case 8:
    	        exit = 1;
    	        break;
	        default:
    	        printf("Numero nao pertencente ao menu\n");
        }
	}
	printf("Missao cumprida!");
}

