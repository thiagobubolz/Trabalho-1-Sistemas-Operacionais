/*
 Trabalho 1 - Sistemas Operacionais 2016/2
 Dupla: Rodrigo Leitzke, matricula: 13103629
        Thiago Bubolz, matricula: 14200897
        Arquivo com as chamadas de funções e criação de estruturas;
*/
typedef struct buffer{

	char *linha;
	struct buffer *ant;
	struct buffer *prox;
}Buffer;

typedef struct cabeca{
	Buffer *primeiro;
	Buffer *ultimo;
}Cabeca;

typedef struct parametro1{
	FILE *arq;
	Cabeca *head;
	int numthreads;
}Param1;

typedef struct parametro2{
	Cabeca *head;
	char *substring;

}Param2;

unsigned int ocorrencias(const char *arquivo, const char *substring, int numthreads);