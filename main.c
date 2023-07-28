// Arquivo principal do trabalho

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAM_MINIMO_SOBRA 10  // Tamanho mínimo que a sobra de um registro deve possuir para ser reaproveitada

typedef enum {False, True} booleano;

void impressao_da_led(FILE* arquivo_de_dados);
void fazer_operacoes(FILE* arquivo_de_dados, FILE* arquivo_de_operacoes);

int main(int argc, char *argv[]) {

    if (argc == 3 && strcmp(argv[1], "-e") == 0) {

        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);
        // chamada da funcao que executa o arquivo de operacoes
        // o nome do arquivo de operacoes estara armazenado na variavel argv[2]
        // executa_operacoes(argv[2])

    } else if (argc == 2 && strcmp(argv[1], "-p") == 0) {

        printf("Modo de impressao da LED ativado ...\n");
        // chamada da funcao que imprime as informacoes da led
        // imprime_led();

    } else {

        fprintf(stderr, "Argumentos incorretos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s -e nome_arquivo\n", argv[0]);
        fprintf(stderr, "$ %s -p\n", argv[0]);
        exit(EXIT_FAILURE);

    }

    return 0;
}
    // Proposta de heurística
    /*
    MAIN
    1- Pegar o modo de operação dos argcs e argvs
    2- Verificar e abrir o arquivo 'dados.dat' (caso n exista, dar erro)
    3- Se o modo de operação for '-e'
    4- -- fazer_operacoes()
    5- Senão, Se o modo de operação for '-p'
    6- -- impressao_da_led()
    7- Senão, dar um erro

    FAZER_OPERACOES
    1- para cada linha do arquivo de operações:
    2- -- se a linha comeca com b, fazer_busca()
    3- -- se a linha comeca com i, fazer_insercao()
    4- -- se a linha comeca com r, fazer_remocao()
    5- -- se a linha comeca com outra coisa (e nao for fim do arquivo), exibir erro

    IMPRESSAO_DA_LED
    1- se o offset for '-1'
    2- -- imprima "offset: -1"
    3- caso contrario:
    4- -- imprima o offset e o tamanho do registro
    5- -- IMPRESSAO_DA_LED(novo_offset_lido)

    FAZER_BUSCA
    (A implementar) É uma busca sequencial simples

    FAZER_INSERCAO
    (A implementar) Verificar primeiro elemento da LED -> se couber, blz! (e checar tamanho restante) -> senão, coloque no final

    FAZER_REMOCAO
    (A implementar) Remover registro e colocar seu offset na LED (que está decrescente) na posição correta
    */

    // Pro windows não fechar o CMD imediatamente 
