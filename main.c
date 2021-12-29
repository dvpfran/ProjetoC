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

Utilizador RegistarUtilizador();
Utilizador ConsultarUtilizador();

Transacao RegistarTransacao();
Transacao ConsultarTransacao();

void ConsultarTotalFaturaEscola(); // apresentado no menu principal
void PercentagemTransacoes(); // pagamentos por escola
void TotalTransacao(); // pagamentos entre duas datas por tipo de utilizador

void LerDadosFicheiro();
void GuardarDadosFicheiro(); // guardar em binário

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
void ConsultarUtilizador(Utilizador utilizadores[])
{
    utilizadores[0] = RegistarUtilizador("")
}
{
    printf("Insira o seu ID -  ");
    scanf("%d", &id);
    printf("Insira o seu ID escolar -  ");
    scanf("%d", &idEscola);
    printf("Insira o seu nome completo -  ");
    scanf("%c", &nome);
    printf("Insira o seu nif -  ");
    scanf("%d", &nif);
    printf("Insira o seu e-mail -  ");
    scanf("%c", &email);
}