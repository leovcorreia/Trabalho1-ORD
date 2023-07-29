/*
    Trabalho 1 - Organização e recuperação de dados
    Professora: Valeria Delisandra Feltrim
    Alunos: 
        Leonardo Venâncio Correia (RA: 129266)
        Murilo Luis Calvo Neves (RA: 129037)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define TAM_MINIMO_SOBRA 10  // Tamanho mínimo que a sobra de um registro deve possuir para ser reaproveitada
#define NOME_ARQUIVO_DADOS "dados.dat"

// Se acabar não sendo usado, é bom remover
typedef enum {False, True} booleano;  // Enum booleano para deixar o código mais limpo caso seja necessário

// Funções auxiliares
// -- A criar

// Funções de registros
void inserir_registro(char* novo_registro);  // A implementar
void remover_registro(char* identificador);  // A implementar
void buscar_registro(char* identificador);  // A implementar

// Funções de modos de operações
void impressao_da_led(FILE* arquivo_de_dados);  // A implementar
void fazer_operacoes(FILE* arquivo_de_dados, FILE* arquivo_de_operacoes);  // A implementar

int main(int argc, char *argv[]) {
    /*
        Ponto de entrada principal do programa
    */

    FILE *arquivo_de_dados = fopen(NOME_ARQUIVO_DADOS, "rb+");

    if (arquivo_de_dados == NULL)
    {
        fprintf(stderr, "\nNao foi encontrado o arquivo de dados para leitura");
        exit(EXIT_FAILURE);
    }
    
    if (argc == 3 && strcmp(argv[1], "-e") == 0) {

        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);
        FILE *arquivo_de_operacoes = fopen(argv[2], "rb");  // Como dito no enunciado, assume-se que esse arquivo sempre é aberto corretamente
        fazer_operacoes(arquivo_de_dados, arquivo_de_operacoes);

    } else if (argc == 2 && strcmp(argv[1], "-p") == 0) {

        printf("Modo de impressao da LED ativado ...\n");
        imprime_led(arquivo_de_dados);

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
    5- -- se a linha comeca com outra coisa (e nao for fim do arquivo), exibir erro [ou fazer nada]

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
