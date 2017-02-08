/*
 Trabalho 1 - Sistemas Operacionais 2016/2
 Dupla: Rodrigo Leitzke, matricula: 13103629
        Thiago Bubolz, matricula: 14200897
        Versão Sequencial;
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "calcula.h"


const char *ler_linha(FILE *arq){

    int cont = 0;
    int tam_linha = 255;

    const char *ret;
  
    char *linha_buffer = malloc(sizeof(char) * tam_linha);
    char ch = getc(arq);

    while ((ch != '\n') && (ch != EOF)) {
        if (cont == tam_linha) {
            tam_linha += 255;
            linha_buffer = realloc(linha_buffer, tam_linha);
        }
            linha_buffer[cont] = ch;
            cont++;
            ch = getc(arq);
    }

    linha_buffer[cont] = '\0'; //ultimo caractere;
    char *linha = malloc(sizeof(char)*(cont+1));
    strncpy(linha, linha_buffer, (cont + 1));
    free(linha_buffer);
    ret = linha;
    return ret;

}


unsigned int ocorrencias(const char *arquivo, const char *substring, int numthreads){

    int cont = 0;
    FILE *arq;

    arq = fopen(arquivo, "r");
    const char *linha;

    int len_sub = strlen(substring);
    

    while(!feof(arq)){
        linha = ler_linha(arq);
        for (linha = strstr(linha, substring); linha; linha = strstr(linha + len_sub, substring))  ++cont; 
        linha++;
        
    }

    fclose(arq);   

    return cont;
}
    
