#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct
{
    char label[16];
    int linha;
} Label;

void erroCompila(char *msg, FILE *saida, int linha, char *erro) // funcao para imprimir erros de compilacao
{
    fclose(saida);
    // limpar o arquivo txt.
    FILE *arq = fopen("saida.txt", "w");
    fprintf(saida, "Erro de compilacao na linha %d, perto de: %s\nError:\n\t%s\n", linha, msg, erro);
    fclose(arq);
    exit(1);
}

void conversor(char *translate, FILE *saida, int linha)
{
    if (strcasecmp(translate, "add") == 0)
    {
        fprintf(saida, "0010");
    }
    else if (strcasecmp(translate, "sub") == 0)
    {
        fprintf(saida, "0011");
    }
    else if (strcasecmp(translate, "addi") == 0)
    {
        fprintf(saida, "1000");
    }
    else if (strcasecmp(translate, "sft") == 0)
    {
        fprintf(saida, "1001");
    }
    else if (strcasecmp(translate, "and") == 0)
    {
        fprintf(saida, "0100");
    }
    else if (strcasecmp(translate, "or") == 0)
    {
        fprintf(saida, "0101");
    }
    else if (strcasecmp(translate, "not") == 0)
    {
        fprintf(saida, "1010");
    }
    else if (strcasecmp(translate, "xor") == 0)
    {
        fprintf(saida, "0110");
    }
    else if (strcasecmp(translate, "slt") == 0)
    {
        fprintf(saida, "0111");
    }
    else if (strcasecmp(translate, "lw") == 0)
    {
        fprintf(saida, "0000");
    }
    else if (strcasecmp(translate, "sw") == 0)
    {
        fprintf(saida, "0001");
    }
    else if (strcasecmp(translate, "lui") == 0)
    {
        fprintf(saida, "1011");
    }
    else if (strcasecmp(translate, "beq") == 0)
    {
        fprintf(saida, "1100");
    }
    else if (strcasecmp(translate, "blt") == 0)
    {
        fprintf(saida, "1101");
    }
    else if (strcasecmp(translate, "j") == 0)
    {
        fprintf(saida, "1110");
    }
    else if (strcasecmp(translate, "jal") == 0)
    {
        fprintf(saida, "1111");
    }
    else if (strcasecmp(translate, "$zero") == 0)
    {
        fprintf(saida, "0000");
    }
    else if (strcasecmp(translate, "$t0") == 0)
    {
        fprintf(saida, "0001");
    }
    else if (strcasecmp(translate, "$t1") == 0)
    {
        fprintf(saida, "0010");
    }
    else if (strcasecmp(translate, "$t2") == 0)
    {
        fprintf(saida, "0011");
    }
    else if (strcasecmp(translate, "$a0") == 0)
    {
        fprintf(saida, "0100");
    }
    else if (strcasecmp(translate, "$a1") == 0)
    {
        fprintf(saida, "0101");
    }
    else if (strcasecmp(translate, "$a2") == 0)
    {
        fprintf(saida, "0110");
    }
    else if (strcasecmp(translate, "$s0") == 0)
    {
        fprintf(saida, "0111");
    }
    else if (strcasecmp(translate, "$s1") == 0)
    {
        fprintf(saida, "1000");
    }
    else if (strcasecmp(translate, "$s2") == 0)
    {
        fprintf(saida, "1001");
    }
    else if (strcasecmp(translate, "$s3") == 0)
    {
        fprintf(saida, "1010");
    }
    else if (strcasecmp(translate, "$s4") == 0)
    {
        fprintf(saida, "1011");
    }
    else if (strcasecmp(translate, "$gp") == 0)
    {
        fprintf(saida, "1100");
    }
    else if (strcasecmp(translate, "$sp") == 0)
    {
        fprintf(saida, "1101");
    }
    else if (strcasecmp(translate, "$pc") == 0)
    {
        fprintf(saida, "1110");
    }
    else if (strcasecmp(translate, "$ra") == 0)
    {
        fprintf(saida, "1111");
    }
    else
    {
        linha++;
        erroCompila(translate, saida, linha, "Erro de sintaxe, esperando uma operacao ou um registrador");
    }
}

void conversorA4(char *string, FILE *saida) // 4 bits
{
    int num = atoi(string);
    if (num > 15)
    {
        erroCompila(string, saida, 0, "Erro de sintaxe, esperado numero em 4 bits");
    }
    int vet_bin[4] = {0, 0, 0, 0};
    int i = 0, j;
    int aux_num = num;

    while (num > 0)
    {
        vet_bin[i] = num % 2;
        i++;
        num = num / 2;
    }

    for (j = 3; j >= 0; j--)
    {
        fprintf(saida, "%d", vet_bin[j]);
    }
}

void conversorA8(char *string, FILE *saida) // 8 bits
{
    int num = atoi(string);
    int vet_bin[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int i = 0, j;
    int aux_num = num;

    while (num > 0)
    {
        vet_bin[i] = num % 2;
        i++;
        num = num / 2;
    }

    for (j = 7; j >= 0; j--)
    {
        fprintf(saida, "%d", vet_bin[j]);
    }
}

int main()
{
    FILE *arq = fopen("s1.txt", "r");
    FILE *saida = fopen("saida.txt", "w");
    char c;
    char operation[16] = "";
    int reg = 0; // 0 - nao // 1 - sim
    int i = 0;
    int contador = 0; // contador de registros
    int linha = 0;
    int not = 0;    // variavel que controla o operador NOT
    int digito = 0; // variavel que controla se o operador é um digito
    int desvio = 0; // variavel que controla se o operador é um desvio

    // int auxLabel = 0; // verifica se existe um label no /n
    int qtdLabel = 0; // quantidade de label
    int limiteVetor = 1;
    Label *vetorLabel = malloc(limiteVetor * sizeof(Label));
    char buffer[16];

    while (1)
    {
        c = fgetc(arq);

        if (reg)
        {
            if (c == '$')
            {
                contador++;
            }
            else if (c == EOF)
            {
                linha++;
                erroCompila(operation, saida, linha, "Erro de sintaxe");
            }
        }
        reg = 0;
        if (c == EOF)
        {
            // verificar se operation está vazia, se estiver nao precisa fazer nada
            if (i != 0)
            {
                operation[i + 1] = '\0';
                // se operation for um numero, chama funcao conversorA8
                for (int j = 0; j < strlen(operation); j++)
                {
                    if (isdigit(operation[j]))
                    {
                        digito++;
                    }
                }
                if (strlen(operation) == digito)
                {
                    // converter string operation para numero
                    if (atoi(operation) > 15)
                    {
                        conversorA8(operation, saida);
                    }
                    else
                    {
                        conversorA4(operation, saida);
                    }
                }
                else
                {
                    conversor(operation, saida, linha);
                }
            }
            break;
        }

        if (c == ':')
        {
            if (i != 15)
            {
                operation[i + 1] = '\0';
            }
            c = fgetc(arq);
            linha++;
            if (c == '\n')
            {
                if(qtdLabel == limiteVetor){
                    limiteVetor = limiteVetor + 1;
                    vetorLabel = realloc(vetorLabel, limiteVetor * sizeof(Label));
                }
                strcpy(vetorLabel[qtdLabel].label, operation);
                vetorLabel[qtdLabel].linha = linha + 1;
                qtdLabel++;
                
                i = 0;
                memset(operation, 0, 16);
                contador = 0;
                not = 0;
                digito = 0;
                desvio = 0;
            }
            else
            {
                erroCompila(operation, saida, linha, "Erro de sintaxe, Label incorreta, depois de : não está com o \n");
            }
        }
        else if (c == ' ')
        {
            if (i != 15)
            {
                operation[i + 1] = '\0';
            }

            // c = ' ' // significa que preciso resetar os dados e passar a instrucao para o arquivo
            conversor(operation, saida, linha);
            if (strcasecmp(operation, "not") == 0 )
            {
                not = 1;
            }

            if (strcasecmp(operation, "beq") == 0  || strcasecmp(operation, "blt") == 0  || strcasecmp(operation, "j") == 0  || strcasecmp(operation, "jal") == 0)
            {
                desvio = 1;
            }

            i = 0;
            memset(operation, 0, 16);
            reg = 1;
        }
        else if (c == ',')
        {
            if (i != 15)
            {
                operation[i + 1] = '\0';
            }

            conversor(operation, saida, linha);
            i = 0;
            memset(operation, 0, 16);
            reg = 1;
        }
        else if (c == '\n')
        {
            if (contador != 0)
            {
                linha++;
            }
            if (i != 15)
            {
                operation[i + 1] = '\0';
            }

            if (contador == 1)
            {
                // 8 bits
                // verificr se opertion é um numero
                for (int j = 0; j < strlen(operation); j++)
                {
                    if (isdigit(operation[j]))
                    {
                        digito++;
                    }
                }
                if (strlen(operation) == digito)
                {
                    conversorA8(operation, saida);
                } else if(desvio)// verificar se operation é j e jal
                {
                    //verificar nas labels pra ver se alguma encaixa
                    // se encaixar, transformar a linha delas +1 em binario
                    // se nao encaixar, erro
                    for(int j = 0; j < qtdLabel; j++){
                        if(strcasecmp(operation, vetorLabel[j].label) == 0){
                            sprintf(buffer, "%d", vetorLabel[j].linha);
                            conversorA8(buffer, saida);
                            break;
                        } else if(j == qtdLabel-1){
                            erroCompila(operation, saida, linha, "Erro de sintaxe, label não encontrada");
                        }
                    }
                }
                else
                {
                    // erro;
                    if (not )
                    {
                        conversor(operation, saida, linha);
                        conversorA8("0", saida);
                    }
                    else
                    {
                        erroCompila(operation, saida, linha, "Erro de sintaxe, esperando numeroConstante");
                    }
                }
            }
            else if (contador == 2)
            {
                // 4 bits
                // verificar se operation é um numero
                for (int j = 0; j < strlen(operation); j++)
                {
                    if (isdigit(operation[j]))
                    {
                        digito++;
                    }
                }
                if (strlen(operation) == digito)
                {
                    conversorA4(operation, saida);
                }else if(desvio)
                {
                    for(int j = 0; j < qtdLabel; j++){
                        if(strcasecmp(operation, vetorLabel[j].label) == 0){
                            sprintf(buffer, "%d", vetorLabel[j].linha);
                            conversorA4(buffer, saida);
                            break;
                        } else if(j == qtdLabel-1){
                            erroCompila(operation, saida, linha, "Erro de sintaxe, label não encontrada");
                        }
                    }
                }
                else
                {
                    // erro;
                    erroCompila(operation, saida, linha, "Erro de sintaxe, esperando 3 registradores");
                }
            }
            else
            {
                conversor(operation, saida, linha);
            }

            // resetar variaveis
            i = 0;
            memset(operation, 0, 16);
            contador = 0;
            fprintf(saida, "\n"); // tirar depois de pronto
            not = 0;
            digito = 0;
            desvio = 0;
        }
        else
        {
            operation[i] = c;
            i++;
        }
    }

    fclose(arq);
    fclose(saida);
    free(vetorLabel);
    return 0;
}
