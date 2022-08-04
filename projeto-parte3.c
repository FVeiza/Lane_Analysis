/*
***************

LAED1 - Projeto (Parte III) - Proximidade dos obstáculos

Aluno: Fernando Veizaga

Data: 13/08/2021

***************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INICIOARRANJO   1
#define MAXTAM          1000

typedef int TipoChave;

typedef int TipoApontador;

typedef struct {
  TipoChave Chave;
  int resultado;
} TipoItem;

typedef struct {
  TipoItem Item[MAXTAM];
  TipoApontador Primeiro, Ultimo;
} TipoLista;

/* ========================================================================== */

void FLVazia(TipoLista *Lista)
{ Lista -> Primeiro = INICIOARRANJO;
  Lista -> Ultimo = Lista -> Primeiro;
}  /* FLVazia */

int Vazia(TipoLista Lista)
{ return (Lista.Primeiro == Lista.Ultimo);
}  /* Vazia */

void Insere(TipoItem x, TipoLista *Lista)
{ if (Lista -> Ultimo > MAXTAM) printf("Lista esta cheia\n");
  else { Lista -> Item[Lista -> Ultimo - 1] = x;
         Lista -> Ultimo++;
       }
}  /* Insere */

void Retira(TipoApontador p, TipoLista *Lista, TipoItem *Item)
{ int Aux;

  if (Vazia(*Lista) || p >= Lista -> Ultimo)
  { printf(" Erro   Posicao nao existe\n");
    return;
  }
  *Item = Lista -> Item[p - 1];
  Lista -> Ultimo--;
  for (Aux = p; Aux < Lista -> Ultimo; Aux++)
    Lista -> Item[Aux - 1] = Lista -> Item[Aux];
}  /* Retira */

void Imprime(TipoLista Lista)
{ int Aux;

  for (Aux = Lista.Primeiro - 1; Aux <= (Lista.Ultimo - 2); Aux++)
    printf("%d\n", Lista.Item[Aux].Chave);
}  /* Imprime */

/* ========================================================================== */

int verificaImpedimento(FILE *f){
    int resultado = 0;                                  //declaracao da variavel resultado, que armazenara os seguintes valores: 0, caso o padrao nao seja encontrado; 1, caso o padrao seja encontrado;
                                                        //2, caso o padrao seja encontrado mas existam impedimentos na pista (existem outros segmentos entre "1 3 2" e "2 3 1")

    int n;
    fscanf(f, "%d", &n);                                //leitura do numero n, correspondente ao numero de elementos

    int sequencia[5] = {1, 3, 2, 3, 1};                 //declaracao do vetor correspondete a sequencia que se deseja encontrar
    int sequenciapt1[3] = {1, 3, 2};
    int sequenciapt2[3] = {2, 3, 1};
    int *vetor = (int*)malloc(n*sizeof(int));           //declaracao do vetor que recebera os n elementos
    int tipos[3];                                      //declaracao do vetor que guardara um elemento de cada tipo

    for(int i = 0; i < 10; i++){                        //laco para preenchimento do vetor de tipos
        tipos[i] = -1;
    }

    for(int i = 0; i < n; i++){                         //laco para preenchimento do vetor de elementos
        if(i == n-1){
            fscanf(f, "%d", &vetor[i]);
        }
        else{
            fscanf(f, "%d ", &vetor[i]);
        }
    }

    int contador = 0, controle = 0;                     //declaracao de variaveis auxiliares

    for(int i = 0; i < n; i++){                         //laco para preenchimento do vetor de tipos
        controle = 0;
        for(int j = 0; j < 3; j++){                    //laco para comparacao do elemento atual do vetor de elementos com os elementos do vetor de tipos
            if(tipos[j] == vetor[i]){
                controle = 1;
                break;
            }
        }
        if(controle == 0){
            tipos[contador] = vetor[i];                 //se o elemento atual ainda nao estiver presente no vetor de tipos, o colocamos nesse vetor
            contador++;
        }
    }


    int aux;
    for(int i = 0; i < contador-1; i++){                //laco para ordenacao crescente do vetor de tipos
        if(tipos[i] > tipos[i+1]){
            aux = tipos[i];
            tipos[i] = tipos[i+1];
            tipos[i+1] = aux;
        }
        for(int j = i; j >= 0; j--){
            if(tipos[i] < tipos[j]){
                aux = tipos[i];
                tipos[i] = tipos[j];
                tipos[j] = aux;
            }
        }
    }


    int dados[2][20];                                   //declaracao da matriz que guardara uma linha com os tipos de cada segmento e uma linha como numero de elementos em cada segmento

    int contador1 = 0, c = 0;
    for(int i = 0; i < n; i++){                         //laco para preenchimento da matriz
        if(i == n-1){                                   //se o elemento atual do vetor for o ultimo, executamos lacos para garantir o preenchimento adequado da matriz
            if((vetor[i] != vetor[i-1])){               //verificacao de mudanca de tipo de segmento
                for(int j = 0; j < contador; j++){      //laco para indentificarmos o tipo do elemento anterior
                    if(vetor[i-1] == tipos[j]){
                        dados[0][c] = j+1;              //preenchimento do tipo do segmento anterior na matriz
                        dados[1][c] = contador1;        //preenchimento do numero de elementos do segmento anterior na matriz
                        c++;
                        contador1 = 1;
                        break;
                    }
                }
                for(int j = 0; j < contador; j++){      //laco para identificarmos o tipo do elemento atual
                    if(vetor[i] == tipos[j]){
                        dados[0][c] = j+1;              //preenchimento do tipo do segmento atual na matriz
                        dados[1][c] = contador1;        //preenchimento do numero de elementos do segmento atual na matriz
                        c++;
                        break;
                    }
                }
            }
            else{
                for(int j = 0; j < contador; j++){      //laco para identificarmos o tipo do elemento anterior
                    if(vetor[i-1] == tipos[j]){
                        dados[0][c] = j+1;              //preenchimento do tipo do segmento anterior na matriz
                        dados[1][c] = contador1+1;      //preenchimento do numero de elementos do segmento anterior na matriz
                        c++;
                        break;
                    }
                }
            }

        }
        else if((i > 0) && (vetor[i] != vetor[i-1])){   //verificacao de mudanca de segmento
            for(int j = 0; j < contador; j++){          //laco para identificarmos o tipo do elemento anterior
                if(vetor[i-1] == tipos[j]){
                    dados[0][c] = j+1;                  //preenchimento do tipo do segmento atual na matriz
                    dados[1][c] = contador1;            //preenchimento do numero de elementos do segmento anterior na matriz
                    c++;
                    contador1 = 0;
                    break;
                }
            }
        }

        contador1++;                                    //incremento do contador de numero de elementos do segmento atual
    }


    int controle1 = 0, encontrou = 0;                   //declaracao de variaveis auxiliares
    for(int i = 0; i < c-4; i++){                       //laco para verificacao de presenca da sequencia desejada na primeira linha da matriz
        controle1 = 0;
        if(dados[0][i] == 1){                           //se o elemento atual da primeira linha da matriz for igual ao primeiro elemento da sequencia, entramos no condicional
            for(int j = 0; j < 5; j++){                 //laco para verificacao de presencao da sequencia desejada na matriz, a partir do elemento atual
                if(dados[0][i+j] != sequencia[j]){
                    controle1 = 1;                      //se algum elemento seguinte for diferente da sequencia desejada, a variavel "controle1" recebe 1
                    break;
                }
            }
            if(controle1 == 0){
                encontrou = 1;                          //se a variavel "controle1" saiu do laco anterior sem alteracoes, a variavel "encontrou" recebe 1
                break;
            }
        }
        if(encontrou == 1){                             //se a variavel "encontrou" for igual a 1, interrompemos a execucao do laco
            break;
        }
    }

    int controle2 = 0, impedimento = 0, k;              //declaracao de variaveis auxiliares

    if(encontrou == 1){                                 //se a variavel "encontrou" sair do laco anterior com valor igual a 1, imprimimos a saida correspondente a ter encontrado a sequencia desejada
        resultado = 1;
    }
    else{                                               //se a variavel "encontrou" sair do laco anterior com valor igual a 0, verificamos se há algum impedimento na pista
        for(int i = 0; i < c-6; i++){                   //laco para verificacao de impedimentos na pista
            controle2 = 0;
            k = 0;
            if(dados[0][i] == 1){
                for(int j = 0; j < 3; j++){                     //laco para verificacao da existencia da primeira parte da sequencia "1 3 2" na matriz
                    if(dados[0][i+j] != sequenciapt1[j]){
                        controle2 = 1;                          //se algum elemento seguinte for diferente da sequencia desejada, a variavel "controle2" recebe 1 e a variavel auxiliar "k" e zerada
                        k = 0;
                        break;
                    }
                    else{
                        k = i+j;                                //se o elemento atual da matriz condizer com o elemento do trecho desejado, a variavel k recebe o valor da posicao desse elemento
                    }
                }
                if(controle2 == 0){                             //se confirmarmos a presenca do trecho "1 2 3" na matriz, entramos neste condicional para verificar a existencia de impedimentos
                    for(k++; k < c-2; k++){                     //laco para verificar se ha impedimentos ou se o padrao nao existe
                        controle2 = 0;
                        if(dados[0][k] == 2){
                            for(int a = 0; a < 3; a++){                 //laco para verificacao da existencia da segunda parte da sequencia "2 3 1" na matriz
                                if(dados[0][k+a] != sequenciapt2[a]){
                                    controle2 = 1;                      //se algum elemento seguinte for diferente da sequencia desejada, a variavel "controle2" recebe 1
                                    break;
                                }
                            }
                            if(controle2 == 0){
                                impedimento = 1;                    //se a variavel "controle2" saiu do laco anterior sem alteracoes, a variavel "impedimento" recebe 1
                            }
                        }
                        if(impedimento == 1){               //se a variavel "impedimento" for igual a 1, interrompemos a execucao do laco interno
                            break;
                        }
                    }
                }
            }
            if(impedimento == 1){                   //se a variavel "impedimento" for igual a 1, interrompemos a execucao do laco externo e atribuimos o valor 2 a variavel "resultado"
                resultado = 2;
                break;
            }
        }
    }

    return resultado;                               //retornamos o resultado da verificacao
}



int main(){
    FILE *f;
    char arq[20];
    printf("Digite o nome do arquivo: ");
    scanf("%s", arq);
    f = fopen(arq, "r");

    TipoLista lista;                                        //declaracao de uma struct TipoLista
    TipoItem item;                                          //declaracao de uma struct TipoItem
    FLVazia(&lista);                                        //utilizacao da funcao "FLVazia()" para esvaziar a lista criada

    int l;
    fscanf(f, "%d", &l);                                    //leitura do numero de perfis no arquivo texto

    for(int i = 0; i < l; i++){                             //laco para preenchimento de "lista" com itens
        item.Chave = i;                                     //atribuicao de valores para a variavel "Chave" de item
        item.resultado = verificaImpedimento(f);            //atribuicao de valores para a variavel "resultado" de item atraves da funcao "verificaImpedimento()"
        Insere(item, &lista);                               //utilizacao da funcao "Insere()" para inserir os itens em "lista"
    }

    int impedimento = 0, proximidade = 0;
    for(int i = lista.Primeiro - 1; i <= (lista.Ultimo - 2); i++){          //laco para verificacao da ocorrencia ou nao de impedimentos nas verificacoes
        if(lista.Item[i].resultado == 2){                                   //condicional verifica os valores da variavel "resultado" em cada struct do tipo "Item" na lista
            impedimento = 1;                                                //se o valor de alguma variavel "resultado" tiver valor igual a 2, a variavel "impedimento" recebe valor 1
            proximidade = i;                                            //a variavel "proximidade" guarda a posicao do perfil mais proximo do robo que contem impedimento
        }
    }

    if(impedimento == 0){                                       //se a variavel "impedimento" tiver valor igual a 0, significa que nao ha impedimentos na pista
        printf("Resultado: Pista sem impedimento.\n");
    }
    else if((impedimento == 1) && (proximidade >= 0 && proximidade <= 4)){      //se a variavel "impedimento" tiver valor diferente de 0 e "proximidade" estiver entre 1 e 5, significa que ha um impedimento distante do robo na pista
        printf("Resultado: Pista com impedimento distante.\n");
    }
    else if((impedimento == 1) && (proximidade >= 5 && proximidade <= 14)){     //se a variavel "impedimento" tiver valor diferente de 0 e "proximidade" estiver entre 6 e 15, significa que ha um impedimento proximo do robo na pista
        printf("Resultado: Pista com impedimento proximo.\n");
    }

    fclose(f);

    return 0;
}

