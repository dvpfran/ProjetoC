#include <stdio.h>
#include <string.h>

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

Escola RegistarEscola(char nome[], char abreviacao[], int campus, char localidade[]);
void CarregarEscolas();
char *converterEscolasParaChar(Escola escolas[]);

Utilizador RegistarUtilizador();
Utilizador ConsultarUtilizador();

Transacao RegistarTransacao();
Transacao ConsultarTransacao();

void ConsultarTotalFaturaEscola(); // apresentado no menu principal
void PercentagemTransacoes(); // pagamentos por escola
void TotalTransacao(); // pagamentos entre duas datas por tipo de utilizador

void LerDadosFicheiro();
void guardarDadosFicheiro(); // guardar em binário

void main() {
    Escola escolas[NUM_ESCOLAS];
    CarregarEscolas(escolas);

    // CHAMAR FUNÇÃO
}

Escola RegistarEscola(char nome[], char abreviacao[], int campus, char localidade[]) {
    Escola escola;//GUIA!!!
    strcpy(escola.nome, nome);
    strcpy(escola.abreviacao, abreviacao);
    strcpy(escola.localidade, localidade);
    escola.campus = campus;
    return escola;
}

void CarregarEscolas(Escola escolas[]) {
    escolas[0] = RegistarEscola("Escola Superior de Educação e Ciências Sociais", "ESECS", 1, "Leiria");
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