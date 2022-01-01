#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_ESCOLAS 5
#define NUM_UTilizadores 200
#define NUM_Transacoes 5000

#define PATH_ESCOLAS "dados_escolas.txt"

typedef struct
{
    int id;
    char nome[100];
    char abreviacao[10];
    char localidade[20];
    int campus;
} Escola;

// Atualizar este valor quando é adicionado ou removido algum campo da struct Escola.
#define NUM_CAMPOS_STRUCT_ESCOLA 5

typedef struct
{
    int id;
    int idEscola;
    char nome[100];
    int nif;
    int tipoUtilizador; // Estudante Docente Funcionário
    char email[100];
    float saldo;
} Utilizador;

typedef struct
{
    int id;
    int idUtilizador;
    int tipoTransacao; // Carregamento Pagamento
    float valorTransacao;
    char dataHora[50];
} Transacao;

Escola registarEscola(int proximoId);
void CarregarEscolas();
char *converterEscolasParaChar(Escola escolas[], int escolasRegistadas);
int converterCharParaEscolas(char charEscolas[], Escola escolas[]);
void converterCharParaCampoEscola(int contadorCamposEscola, int contadorEscolas, Escola escolas[], char splitEscolas[]);

Utilizador registarUtilizador();
Utilizador ConsultarUtilizador();

Transacao RegistarTransacao();
Transacao ConsultarTransacao();

void ConsultarTotalFaturaEscola(); // apresentado no menu principal
void PercentagemTransacoes();      // pagamentos por escola
void TotalTransacao();             // pagamentos entre duas datas por tipo de utilizador

char *lerDadosFicheiro(char caminhoFicheiro[]);
void guardarDadosFicheiro(); // guardar em binário

void main()
{
    int escolasRegistadas = 0;
    Escola escolas[NUM_ESCOLAS];
}

Escola registarEscola(int proximoId)
{
    Escola escola;
    escola.id = proximoId;
    printf("* Registo de nova escola\n");
    printf("* Nome: ");
    scanf("%s", &escola.nome);
    printf("* Abreviacao: ");
    scanf("%s", &escola.abreviacao);
    printf("* Localidade: ");
    scanf("%s", &escola.localidade);
    printf("* Campus: ");
    scanf("%d", &escola.campus);
    return escola;
}

void CarregarEscolas(Escola escolas[])
{
}

char *converterEscolasParaChar(Escola escolas[], int escolasRegistadas)
{
    char *charEscolas = malloc(1024);
    for (int index = 0; index < escolasRegistadas; index++)
    {
        char charEscola[200];
        // Junta todas as variáveis da escolas[index] na variável charEscola.
        sprintf(charEscola, "[%d;%s;%s;%d;%s]\n", escolas[index].id, escolas[index].nome, escolas[index].abreviacao, escolas[index].campus, escolas[index].localidade);

        // Se o strcat for usado com o array vazio vai
        // vai adicionar símbolos estranho no início do index 0.
        if (index > 0)
        {
            // Vai concatenar o charEscola com o charEscolas.
            strcat(charEscolas, charEscola);
        }
        else
        {
            // Copia o charEscola para o charEscolas.
            strcpy(charEscolas, charEscola);
        }
    }
    return charEscolas;
}

// Vai retornar o número de escolas existentes.
int converterCharParaEscolas(char charEscolas[], Escola escolas[]) {
    int contadorEscolas = 0, contadorCamposEscola = 1;
    // Vai dividir os valores a cada ; que encontrar.
    char *splitEscolas = strtok(charEscolas, ";");

    while (splitEscolas != NULL) { 
        // Vai atualizar o array escolas conforme o contadorCamposEscola e o contadorEscolas.    
        converterCharParaCampoEscola(contadorCamposEscola, contadorEscolas, escolas, splitEscolas);
        contadorCamposEscola++;

        // Assim que o contadorCamposEscola ficar maior signfica
        // que o próximo registo já faz parte de outra escola.
        if (contadorCamposEscola > NUM_CAMPOS_STRUCT_ESCOLA) {
            contadorCamposEscola = 1;
            contadorEscolas++;
        }
		splitEscolas = strtok(NULL, ";");
	}

    return contadorEscolas;
}

void converterCharParaCampoEscola(int contadorCamposEscola, int contadorEscolas, Escola escolas[], char splitEscolas[]) {
     switch (contadorCamposEscola) {
        case 1:
            escolas[contadorEscolas].id = (atoi)(splitEscolas);
            break;
        case 2:
            strcpy(escolas[contadorEscolas].nome, splitEscolas);
            break;
        case 3:
            strcpy(escolas[contadorEscolas].abreviacao, splitEscolas);
            break;
        case 4:
            escolas[contadorEscolas].campus = atoi(splitEscolas);
            break;
        case 5:
            strcpy(escolas[contadorEscolas].localidade, splitEscolas);
            break;
    }
}

char *lerDadosFicheiro(char caminhoFicheiro[])
{
    FILE *dadosFicheiro;
    dadosFicheiro = fopen(caminhoFicheiro, "r");

    if (dadosFicheiro == NULL)
    {
        printf("O ficheiro nao existe.\n");
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(1024);

    fgets(buffer, 1024, dadosFicheiro);
    fclose(dadosFicheiro);
    return buffer;
}

void guardarDadosFicheiro(char dados[], char caminhoFicheiro[])
{
    FILE *dadosFicheiro;
    dadosFicheiro = fopen(caminhoFicheiro, "w");
    fprintf(dadosFicheiro, "%s", dados);
    fclose(dadosFicheiro);
}
