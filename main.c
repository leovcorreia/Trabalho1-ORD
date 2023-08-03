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
        //imprime_led(arquivo_de_dados);

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

void fazer_operacoes(FILE* arquivo_de_dados, FILE* arquivo_de_operacoes)
{
    /*
    Lê e interpreta cada linha de comando descrita no arquivo de operações.
    */
    char comando;
    char parametro[1024];

    while (comando = fgetc(arquivo_de_operacoes), comando != EOF)
    {
        fseek(arquivo_de_operacoes, 1, SEEK_CUR);
        fgets(parametro, 1024, arquivo_de_operacoes);
        int tamanho_parametro = strlen(parametro);
        
        if (parametro[tamanho_parametro - 1] == '\n')
        {
            parametro[tamanho_parametro - 1] = '\0';
        }

        switch (comando)
        {
            case 'r':
                remover_registro(parametro);
                break;  
            case 'i': 
                inserir_registro(parametro);
                break;
            case 'b': 
                buscar_registro(parametro);
                break;
            default:
                printf("\nA operacao '%c' nao e uma operacao valida", comando);
                break;
        }  
    }
}