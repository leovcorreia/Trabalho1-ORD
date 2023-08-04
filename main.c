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
void inserir_espaco_na_led(int offset, int tamanho, FILE* arquivo_de_dados);

// Funções de registros
void inserir_registro(char* novo_registro, FILE* arquivo_de_dados);  // A implementar
void remover_registro(char* identificador, FILE* arquivo_de_dados);  // A implementar
void buscar_registro(char* identificador, FILE* arquivo_de_dados);  // A implementar

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
        fclose(arquivo_de_operacoes);

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

    fclose(arquivo_de_dados);
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
                remover_registro(parametro, arquivo_de_dados);
                break;  
            case 'i': 
                inserir_registro(parametro, arquivo_de_dados);
                break;
            case 'b': 
                buscar_registro(parametro, arquivo_de_dados);
                break;
            default:
                printf("\nA operacao '%c' nao e uma operacao valida", comando);
                break;
        }  
    }
}

// AINDA PRECISA SER TESTADA
void le_dados_led(int offset, FILE* arquivo_de_dados, int *tamanho, int *proximo_ponteiro)
{
    fseek(arquivo_de_dados, offset, SEEK_SET);
    fread(tamanho, sizeof(int), 1, arquivo_de_dados);
    fseek(arquivo_de_dados, 1, SEEK_CUR);
    fread(proximo_ponteiro, sizeof(int), 1, arquivo_de_dados);
}

// AINDA PRECISA SER TESTADA
void inserir_espaco_na_led(int offset, int tamanho, FILE* arquivo_de_dados)
{
    /*
    Insere um espaço vazio na LED.
    Coloca o tamanho e o asterisco e conecta na LED.
    'Tamanho' deve incluir o espaco para o '*' e o tamanho no inicio
    */
    fseek(arquivo_de_dados, offset, SEEK_SET);
    int tamanho_para_registro = tamanho - sizeof(int);
    fwrite(&tamanho_para_registro, sizeof(int), 1, arquivo_de_dados);
    fwrite("*", sizeof(char), 1, arquivo_de_dados);

    // Conectar na LED
    int tamanho_antigo = -1;
    int aponta_antigo = -1;
    
    int tamanho_atual = -1;
    int aponta_atual = -1;

    int tamanho_proximo = -1;
    int aponta_proximo = -1;

    fseek(arquivo_de_dados, 0, SEEK_SET); // Vai para o inicio do arquivo
    fread(&aponta_proximo, sizeof(int), 1, arquivo_de_dados);  // Lê o primeiro ponteiro da LED

    if (aponta_proximo == -1)
    {
        fseek(arquivo_de_dados, 0, SEEK_SET);
        fwrite(&offset, sizeof(int), 1, arquivo_de_dados);
        fseek(arquivo_de_dados, offset + sizeof(int) + 1, SEEK_SET);
        int ptr = -1;
        fwrite(&ptr, sizeof(int), 1, arquivo_de_dados);
        return;
    }

    aponta_atual = aponta_proximo;
    le_dados_led(aponta_atual, arquivo_de_dados, &tamanho_proximo, &aponta_proximo);
    
    while (aponta_proximo != -1 && tamanho_proximo > tamanho_para_registro) 
    {
        tamanho_antigo = tamanho_atual;
        tamanho_atual = tamanho_proximo;

        aponta_antigo = aponta_atual;
        aponta_atual = aponta_proximo;

        le_dados_led(aponta_atual, arquivo_de_dados, &tamanho_proximo, &aponta_proximo);
    }

    fseek(arquivo_de_dados, aponta_antigo + sizeof(int) + 1, SEEK_SET);
    fwrite(&offset, sizeof(int), 1, arquivo_de_dados);

    fseek(arquivo_de_dados, offset + sizeof(int) + 1, SEEK_SET);
    fwrite(&aponta_atual, sizeof(int), 1, arquivo_de_dados);
}

// AINDA PRECISA SER TESTADA
void inserir_registro(char* novo_registro, FILE* arquivo_de_dados)
{
    /*
    Insere um novo registro no arquivo
    */
    int tamanho_novo_registro = strlen(novo_registro);

    fseek(arquivo_de_dados, 0, SEEK_SET);  // Coloca ponteiro de leitura no inicio

    int offset_atual_led;
    int tamanho_atual_led;

    fread(&offset_atual_led, sizeof(int), 1, arquivo_de_dados);  // Lê o offset do primeiro elemento da LED

    if (offset_atual_led == -1)
    {
        // Inserção quando ainda não foi feita nenhuma remoção
        fseek(arquivo_de_dados, 0, SEEK_END);
        fwrite(novo_registro, tamanho_novo_registro, 1, arquivo_de_dados);
    }
    else
    {
        fseek(arquivo_de_dados, offset_atual_led, SEEK_SET);  // Vai até a posição de inserção
        fread(&tamanho_atual_led, sizeof(int), 1, arquivo_de_dados);

        if (tamanho_atual_led < tamanho_novo_registro + sizeof(int))  // Sem espaço vazio que caiba o elemento a ser adicionado
        {
            fseek(arquivo_de_dados, 0, SEEK_END);
            fwrite(novo_registro, tamanho_novo_registro, 1, arquivo_de_dados);
        }
        else
        {
            fseek(arquivo_de_dados, 1, SEEK_CUR); // Pulando o '*'
            int proximo_ponteiro_led;
            fread(&proximo_ponteiro_led, sizeof(int), 1, arquivo_de_dados); // Lendo o proximo ponteiro da led
            fseek(arquivo_de_dados, -(1 + sizeof(int)), SEEK_CUR); // Voltando para o íncio do local onde o registro deverá ser escrito

            fwrite(&tamanho_novo_registro, sizeof(int), 1, arquivo_de_dados);
            fwrite(novo_registro, tamanho_novo_registro, 1, arquivo_de_dados);

            fseek(arquivo_de_dados, 0, SEEK_SET); // Voltando para o início do arquivo
            fwrite(&proximo_ponteiro_led, sizeof(int), 1, arquivo_de_dados); // Conectando a ponta inicial da LED com a próxima

            fseek(arquivo_de_dados, offset_atual_led + tamanho_novo_registro + sizeof(int), SEEK_SET); // Voltando para onde o espaço vazio está

            if (tamanho_atual_led > tamanho_novo_registro + 2*sizeof(int) + 1 + TAM_MINIMO_SOBRA)
            {
                // Caso ainda haja espaço utilizável
                // Tamanho novo registro + espaços para guardar o tamanho (int) e o ponteiro da LED (int) + o '*' de removido

                inserir_espaco_na_led(offset_atual_led + tamanho_novo_registro + sizeof(int),
                tamanho_atual_led - tamanho_novo_registro, arquivo_de_dados);
            }
        }
    }
}

void remover_registro(char* identificador, FILE* arquivo_de_dados)
{
    /*
    Remove um registro do arquivo
    */

   /*
   percorre o arquivo ate achar o registro
   se n achar, de erro
   se achar, remova ele e coloque seu espaco na led
   */
}