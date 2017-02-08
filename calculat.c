/*
 Trabalho 1 - Sistemas Operacionais 2016/2
 Dupla: Rodrigo Leitzke, matricula: 13103629
        Thiago Bubolz, matricula: 14200897
        Versão Concorrente;
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calculat.h"
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t lock;
pthread_mutex_t lock_cont;

sem_t empty;

unsigned int cont = 0;
unsigned int finaldearquivo = 0;

void inicializaLista(Cabeca *head);
const char *ler_linha(FILE *arq);
void insereLista(Cabeca *head, const char *linha);
Buffer* removeLista(Cabeca *head);
void* remover(void *arg);
void* insere(void *arg);

/*
Função usada para iniciar variaveis, mutexes, semaforos, e threads, que retorna o numero de ocorrencias
das substrings
*/
unsigned int ocorrencias(const char *arquivo, const char *substring, int numthreads){

    cont = 0;
    finaldearquivo = 0;

    pthread_mutex_init(&lock, NULL);
    pthread_mutex_init(&lock_cont, NULL);

    //Declaração das threads
    pthread_t thread_leitura;
    pthread_t thread_contagem[numthreads];

    //Inicia o semaforo para a verificação da lista vazia;
    sem_init(&empty, 0, 0);

    //Declaração e abertura do arquivo
    FILE *arq;
    arq = fopen(arquivo, "r");

    //Criação e inicialização da cabeça da lista
    Cabeca *head = (Cabeca *) malloc(sizeof(Cabeca));
    inicializaLista(head);

    //Param1 passado para a thred que le as linhas do arquivo e adiciona na lista;
    Param1 *param1 = (Param1 *) malloc(sizeof(Param1));
    param1->arq = arq;
    param1->head = head;
    param1->numthreads = numthreads;

    //Param2 passado para a thred que retira as linhas da lista e faz a contagem da substring;
    Param2 *param2 = (Param2 *) malloc(sizeof(Param2));
    param2->substring = (char *) malloc(sizeof(char)*255);
    strcpy(param2->substring, substring);
    param2->head = head;

    //Criação da thread de leitura dos dados do arquivo, e inserção na lista
    pthread_create(&thread_leitura, NULL, insere, (void *) param1);

    //Laço que cria as threads de acordo com o numero de cores do usuário, 
    //todas essas threads são de remoção de linhas da lista, que retiram a linha e contam 
    //o numero de ocorrencia da substring
    for(int i=0; i<numthreads; i++){
        pthread_create(&thread_contagem[i], NULL, remover, (void *) param2);
    }

    
    //Da join em todas as threads
    pthread_join(thread_leitura, NULL);
    for(int i=0; i<numthreads; i++){
        pthread_join(thread_contagem[i], NULL); 
    }

    
    //Destroi os mutex e o semaforo
    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&lock_cont);
    sem_destroy(&empty);  

    free(head);
    free(param1);
    free(param2);  

    return cont;
}

//Função utilizada para inicializar a cabeça da lista
void inicializaLista(Cabeca *head){
    head->primeiro = NULL;
    head->ultimo = NULL;
}

/*
Função void passada por parametro na criação da thread, ela passa por todo o arquivo
e a cada linha insere essa linha em uma lista encadeada
*/
void* insere(void *arg){
    Param1 *param1 = (Param1 *) arg;
    //
    const char *linha;
    int value;
    sem_getvalue(&empty, &value);

    while(!feof(param1->arq)){        
        linha = ler_linha(param1->arq);
        pthread_mutex_lock(&lock);
        insereLista(param1->head, linha);
        pthread_mutex_unlock(&lock);
        sem_post(&empty);
        linha++;
        sem_getvalue(&empty, &value);
    }
    for (int i = 0; i < param1->numthreads; ++i)
    {
        sem_post(&empty);
    }
    fclose(param1->arq);
    pthread_mutex_lock(&lock);
    finaldearquivo = 1;
    pthread_mutex_unlock(&lock);
    return NULL;
}


/*
Função que recebe a cabeça da lista como parametro, e uma linha do arquivo e adiciona no inicio
de uma lista encadeada circular
*/
void insereLista(Cabeca *head, const char *linha){
    //
    Buffer *novo = (Buffer *) malloc (sizeof (Buffer));
    novo->linha = (char *) malloc (255 * sizeof (char));
    strcpy(novo->linha,linha);
    if(head->primeiro == NULL && head->ultimo == NULL){
        head->primeiro = novo;
        head->ultimo = novo;
        novo->prox = novo;
        novo->ant = novo;
    }else{
        novo->prox = head->primeiro;
        novo->ant = head->ultimo;
        head->primeiro->ant = novo;
        head->ultimo->prox = novo;
        head->primeiro = novo;
    }
}

/*
Função void usada para ser passada como parametro para uma thread, ela chama a função removeLista
e conta as ocorrencias da substring em uma linha do texto
*/
void* remover(void *arg){
    //
    Param2 *param2 = (Param2 *) arg;
    int len_sub = strlen(param2->substring);
    int value;
    Buffer *aux;

    pthread_mutex_lock(&lock_cont);
    sem_getvalue(&empty, &value);
    pthread_mutex_unlock(&lock_cont);
    while(!finaldearquivo || value){
        sem_wait(&empty);
        pthread_mutex_lock(&lock);
        aux = removeLista(param2->head);
        pthread_mutex_unlock(&lock);
        if(aux != NULL){
            for (aux->linha = strstr(aux->linha, param2->substring); aux->linha; aux->linha = strstr(aux->linha + len_sub, param2->substring)){
                pthread_mutex_lock(&lock_cont);
                ++cont;
                sem_getvalue(&empty, &value);
                pthread_mutex_unlock(&lock_cont); 
            } 
        }else{
            
            return NULL;
        }
        pthread_mutex_lock(&lock_cont);
        sem_getvalue(&empty, &value);
        pthread_mutex_unlock(&lock_cont);
        
        free(aux->linha);
        free(aux);
    }

    return NULL;
}

/*
Função que recebe a cabeça de uma lista como parametro e remove o ultimo elemento dessa lista
retornando-o
*/
Buffer* removeLista(Cabeca *head){
    //
    Buffer *nodo_removido;
    
    if(head->primeiro == NULL || head->ultimo == NULL){
        return NULL;
    }else if(head->primeiro == head->ultimo){
        nodo_removido = head->primeiro;
        head->primeiro = NULL;
        head->ultimo = NULL;
        return nodo_removido;   
    }else{
        nodo_removido = head->ultimo; 
        head->ultimo = nodo_removido->ant;
        head->ultimo->prox = head->primeiro;
        head->primeiro->ant = head->ultimo;
        return nodo_removido;
    }
    return NULL;
}

/*
Função que le uma linha do arquivo, adiciona em um cont har e retorna esse valor
*/
const char *ler_linha(FILE *arq){

    int inc = 0;
    int tam_linha = 255;

    const char *ret;
  
    char *linha_buffer = malloc(sizeof(char) * tam_linha);
    char ch = getc(arq);

    while ((ch != '\n') && (ch != EOF)) {
        if (inc == tam_linha) {
            tam_linha += 255;
            linha_buffer = realloc(linha_buffer, tam_linha);
        }
            linha_buffer[inc] = ch;
            inc++;
            ch = getc(arq);
    }

    linha_buffer[inc] = '\0'; //ultimo caractere;
    char *linha = malloc(sizeof(char)*(inc+1));
    strncpy(linha, linha_buffer, (inc + 1));
    free(linha_buffer);
    ret = linha;
    return ret;

}