#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_ESCOLAS 5
#define NUM_UTilizadores 200
#define NUM_Transacoes 5000

typedef struct {
    int id;
    char nome[100];
    char abreviacao[10];
    char localidade[20];
    int campus;
} Escola;

typedef struct {
    int id;
    int idEscola;
    char nome[100];
    int nif;
    int tipoUtilizador; // Estudante Docente Funcionário
    char email[100];
    float saldo;
} Utilizador;

typedef struct {
    int id;
    int idUtilizador;
    int tipoTransacao; // Carregamento Pagamento
    float valorTransacao;
    char dataHora[50];
} Transacao;

Escola registarEscola(int proximoId);
void CarregarEscolas();
char *converterEscolasParaChar(Escola escolas[]);

Utilizador registarUtilizador();
Utilizador ConsultarUtilizador();

Transacao RegistarTransacao();
Transacao ConsultarTransacao();

void ConsultarTotalFaturaEscola(); // apresentado no menu principal
void PercentagemTransacoes(); // pagamentos por escola
void TotalTransacao(); // pagamentos entre duas datas por tipo de utilizador

void LerDadosFicheiro();
void guardarDadosFicheiro(); // guardar em binário

void main() {
    int escolasRegistadas = 0;
    Escola escolas[NUM_ESCOLAS];
;
    
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

void CarregarEscolas(Escola escolas[]) {
}

char *converterEscolasParaChar(Escola escolas[])
{
    char *charEscolas = malloc(1024);
    for (int index = 0; index < NUM_ESCOLAS; index++)
    {
        char charEscola[200];
        // Junta todas as variáveis da escolas[index] na variável charEscola.
        sprintf(charEscola, "[%d;%s;%s;%d;%s]\n", escolas[index].id, escolas[index].nome, escolas[index].abreviacao, escolas[index].campus, escolas[index].localidade);

        // Se o strcat for usado com o array vazio vai
        // vai adicionar símbolos estranho no início do index 0.
        if (index > 0) {
            // Vai concatenar o charEscola com o charEscolas.
            strcat(charEscolas, charEscola);
        }
        else {
            // Copia o charEscola para o charEscolas.
            strcpy(charEscolas, charEscola);
        }
    }
    return charEscolas;
}

void guardarDadosFicheiro(char dados[], char caminhoFicheiro[])
{
    FILE *dadosFicheiro;
    dadosFicheiro = fopen(caminhoFicheiro, "w");
    fprintf(dadosFicheiro, "%s", dados);
    fclose(dadosFicheiro);
}