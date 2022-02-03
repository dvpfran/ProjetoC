#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_MAX_ESCOLAS 5
#define NUM_MAX_UTILIZADORES 200
#define NUM_MAX_TRANSACOES 5000

#define TIPO_UTILIZADOR_ESTUDANTE 1
#define TIPO_UTILIZADOR_DOCENTE 2
#define TIPO_UTILIZADOR_FUNCIONARIO 3

#define OPCAO_MENU_SAIR 0
#define OPCAO_MENU_ESCOLAS_REGISTAR 1
#define OPCAO_MENU_ESCOLAS_CONSULTAR 2

#define OPCAO_MENU_UTILIZADORES_REGISTAR 3
#define OPCAO_MENU_UTILIZADORES_CONSULTAR 4

#define OPCAO_MENU_TRANSACOES_REGISTAR 5
#define OPCAO_MENU_TRANSACOES_CONSULTAR 6

#define OPCAO_MENU_ESTATISTICAS_TOTAL_FATURADO_ESCOLA 11
#define OPCAO_MENU_ESTATISTICAS_PERCENTAGEM_PAGAMENTOS 12
#define OPCAO_MENU_ESTATISTICAS_TOTAL_TRANSACOES_HORIZONTE_TEMPORAL 13

#define PATH_ESCOLAS "dados_escolas.bin"
#define PATH_UTILIZADORES "dados_utilizadores.bin"
#define PATH_TRANSACOES "dados_transacoes.bin"

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

int menu_opcoes();
void menu_escolas();
void menu_utilizadores();
void menu_transacoes();

void carregarTodosDados();
void guardarTodosDados();
void inicializarArrays();

// Todas as funções relacionadas com escolas.
void inicializarArrayEscolas();
Escola registarEscola(int proximoId);
void carregarEscolas();
int obterNumeroEscolasRegistadas(Escola escolas[]);
void mostrarEscolas(Escola escolas[]);

// Todas as funções relacionadas com utilizadores.
void inicializarArrayUtilizadores();
Utilizador registarUtilizador();
Utilizador ConsultarUtilizador();
int obterNumeroUtilizadoresRegistados(Utilizador utilizadores[]);
void mostrarEscolasUtilizador(Escola escolas[]);
void mostrarUtilizadores(Utilizador utilizadores[]);
char *buscarTipoUtilizador(int tipoUtilizador);
void mostrarTiposUtilizador();

// Todas as funções relacionadas com transações.
void inicializarArrayTransacoes();
Transacao registarTransacao(Transacao transacoes[]);
void consultarTransacao(Transacao transacoes[]);
void consultarTransacoes(Transacao transacoes[]);
int obterNumeroTransacoesRegistadas(Transacao transacoes[]);

void ConsultarTotalFaturaEscola(); // apresentado no menu principal
void PercentagemTransacoes();      // pagamentos por escola
void TotalTransacao();             // pagamentos entre duas datas por tipo de utilizador

void gravarFicheiro(void *buffer, int numCamposStruct, int tamanhoArray, char caminhoFicheiro[]);
void lerFicheiro(void *buffer, int numCamposStruct, int tamanhoArray, char caminhoFicheiro[]);

void main()
{
    Escola escolas[NUM_MAX_ESCOLAS];
    Utilizador utilizadores[NUM_MAX_UTILIZADORES];
    Transacao transacoes[NUM_MAX_TRANSACOES];

    inicializarArrays(escolas, utilizadores, transacoes);
    carregarTodosDados(escolas, utilizadores, transacoes);

    int numEscolasRegistadas = obterNumeroEscolasRegistadas(escolas);
    int numUtilizadoresRegistados = obterNumeroUtilizadoresRegistados(utilizadores);
    int numTransacoesRegistadas = obterNumeroUtilizadoresRegistados(utilizadores);

    int opcaoMenu = 0;
    
    do {
        opcaoMenu = menu_opcoes();

        switch (opcaoMenu) {
            case OPCAO_MENU_ESCOLAS_REGISTAR:
                escolas[numEscolasRegistadas] = registarEscola(numEscolasRegistadas);
                break;

            case OPCAO_MENU_ESCOLAS_CONSULTAR:
                mostrarEscolas(escolas);
                break;

            case OPCAO_MENU_UTILIZADORES_REGISTAR:
                utilizadores[numUtilizadoresRegistados] = registarUtilizador(numUtilizadoresRegistados, escolas);
                break;

            case OPCAO_MENU_UTILIZADORES_CONSULTAR:
                mostrarUtilizadores(utilizadores);
                break;

            case OPCAO_MENU_TRANSACOES_REGISTAR:
                registarTransacao(transacoes);
                break;

            case OPCAO_MENU_TRANSACOES_CONSULTAR:
                consultarTransacoes(transacoes);
                break;

            case OPCAO_MENU_SAIR:
            default:
                break;
        }
    } while (opcaoMenu != OPCAO_MENU_SAIR);

    guardarTodosDados(escolas, utilizadores, transacoes);
}

// Vai inicializar o array das escolas e garantir que todos os id's comecem a 0.
// Se o id tiver a 0 significa que a posição do array ainda não foi preenchida.
void inicializarArrays(Escola escolas[], Utilizador utilizadores[], Transacao transacoes[]) {
    inicializarArrayEscolas(escolas);
    inicializarArrayUtilizadores(utilizadores);
    inicializarArrayTransacoes(transacoes);
}

void inicializarArrayEscolas(Escola escolas[]) {
    for (int index = 0; index < NUM_MAX_ESCOLAS; index++) {
        escolas[index].id = 0;
        strcpy(escolas[index].nome, "");
        strcpy(escolas[index].abreviacao, "");
        strcpy(escolas[index].localidade, "");
        escolas[index].campus = 0;
    }
}

void inicializarArrayUtilizadores(Utilizador utilizadores[]) {
    for (int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        utilizadores[index].id = 0;
        utilizadores[index].idEscola = 0;
        utilizadores[index].nif = 0;
        utilizadores[index].tipoUtilizador = 0;
        strcpy(utilizadores[index].email, "");
        strcpy(utilizadores[index].nome, "");
        utilizadores[index].saldo = 0;
    }
}

void inicializarArrayTransacoes(Transacao transacoes[]) {
    for (int index = 0; index < NUM_MAX_TRANSACOES; index++) {
        transacoes[index].id = 0;
        transacoes[index].idUtilizador = 0;
        transacoes[index].valorTransacao = 0;
        strcpy(transacoes[index].dataHora, "");
    }
}

int menu_opcoes() {
    int menuSelecionado = 0;

    printf("******************************** MENU PRINCIPAL ********************************\n");
    menu_escolas();
    menu_utilizadores();
    menu_transacoes();
    printf("*******\n");
    printf("* [%d] * Sair               *\n", OPCAO_MENU_SAIR);
    printf("* Selecionar menu: ");
    scanf("%d", &menuSelecionado);
    printf("\n********************************************************************************\n");
    return menuSelecionado;
}

void menu_escolas() {
    printf("******* Escolas: ********\n");
    printf("* [%d] * Registar Escolas   *\n", OPCAO_MENU_ESCOLAS_REGISTAR);
    printf("* [%d] * Consultar Escolas  *\n", OPCAO_MENU_ESCOLAS_CONSULTAR);
}

void menu_utilizadores() {
    printf("******* Utilizadores: ********\n");
    printf("* [%d] * Registar Utilizador   *\n", OPCAO_MENU_UTILIZADORES_REGISTAR);
    printf("* [%d] * Consultar Utilizador   *\n", OPCAO_MENU_UTILIZADORES_CONSULTAR);
}

void menu_transacoes() {
    printf("******* Transacoes: ********\n");
    printf("* [%d] * Registar Transacao   *\n", OPCAO_MENU_TRANSACOES_REGISTAR);
    printf("* [%d] * Consultar Transacao   *\n", OPCAO_MENU_TRANSACOES_CONSULTAR);
}

Escola registarEscola(int proximoId)
{
    Escola escola;
    escola.id = proximoId + 1;
    printf("* Registo de nova escola\n");
    printf("* Nome: ");
    scanf("%s", &escola.nome);
    printf("* Abreviacao: ");
    scanf("%s", &escola.abreviacao);
    printf("* Localidade: ");
    scanf("%s", &escola.localidade);
    printf("* Campus: ");
    scanf("%d", &escola.campus);
    
    system("cls");

    return escola;
}

void carregarEscolas(Escola escolas[]) {
    lerFicheiro(escolas, sizeof(Escola), NUM_MAX_ESCOLAS, PATH_ESCOLAS);
}

void mostrarEscolas(Escola escolas[])
{
    system("cls");
    for (int index = 0; index < NUM_MAX_ESCOLAS; index++)
    {
        if (escolas[index].id > 0) {
            printf("********************************************************************************\n");
            printf("* Id: %d\n", escolas[index].id);
            printf("* Abreviacao: %s\n", escolas[index].abreviacao);
            printf("* Nome: %s\n", escolas[index].nome);
            printf("* Campus: %d\n", escolas[index].campus);
            printf("* Localidade: %s\n", escolas[index].localidade);
        }

        if (index + 1 == NUM_MAX_ESCOLAS)
        {
            printf("********************************************************************************\n");
        }
    }
}

int obterNumeroEscolasRegistadas(Escola escolas[]) {
    int contador = 0;
    for (int index = 0; index < NUM_MAX_ESCOLAS; index++) {
        if (escolas[index].id > 0) {
            contador++;
        }
    }
    return contador;
}

Utilizador registarUtilizador(int proximoId, Escola escolas[]) {
    system("cls");
    Utilizador utilizador;
    utilizador.id = proximoId + 1;
    printf("****** Registo de Novo Utilizador:\n");
    mostrarEscolasUtilizador(escolas);
    printf("* Id Escola: "); 
    scanf("%d", &utilizador.idEscola);
   
    printf("* Nome: ");
    scanf("%s", &utilizador.nome);
    mostrarTiposUtilizador();   

    printf("* Utilizador: ");
    scanf("%d", &utilizador.tipoUtilizador);
    printf("* Email: ");
    scanf("%s", &utilizador.email);
    
    printf("* NIF: ");
    scanf("%d", &utilizador.nif);

    utilizador.saldo = 0;
    
    system("cls");

    return utilizador;
}

void mostrarEscolasUtilizador(Escola escolas[]) {
    printf("* Lista de escolas:\n");
    for (int index = 0; index < NUM_MAX_ESCOLAS; index++) {
        if (escolas[index].id > 0) {
            printf("** Id: [%d] - Nome: %s\n", escolas[index].id, escolas[index].nome);
        }
    }
}

void mostrarUtilizadores(Utilizador utilizadores[]) {
    system("cls");
    for (int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizadores[index].id > 0) {
            printf("* Id: %d\n", utilizadores[index].id);
            printf("* Id Escola: %d\n", utilizadores[index].idEscola);
            printf("* Nif: %d\n", utilizadores[index].nif);
            printf("* Tipo Utilizador: %d\n", utilizadores[index].tipoUtilizador);
            printf("* Email: %s\n", utilizadores[index].email);
            printf("* Nome: %s\n", utilizadores[index].nome);
            printf("* Saldo: %.2f\n", utilizadores[index].saldo);
        }
    }
}

void mostrarTiposUtilizador() {
    printf("* Tipo utilizador: \n");
    printf("** [%d] - %s\n", TIPO_UTILIZADOR_ESTUDANTE, buscarTipoUtilizador(TIPO_UTILIZADOR_ESTUDANTE));
    printf("** [%d] - %s\n", TIPO_UTILIZADOR_DOCENTE, buscarTipoUtilizador(TIPO_UTILIZADOR_DOCENTE));
    printf("** [%d] - %s\n", TIPO_UTILIZADOR_FUNCIONARIO, buscarTipoUtilizador(TIPO_UTILIZADOR_FUNCIONARIO));
}

int obterNumeroUtilizadoresRegistados(Utilizador utilizadores[]) {
    int contador = 0;
    for (int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizadores[index].id > 0) {
            contador++;
        }
    }
    return contador;
}

char *buscarTipoUtilizador(int tipoUtilizador) {
    char *descricaoTipoUtilizador;
    switch (tipoUtilizador) {
        case TIPO_UTILIZADOR_ESTUDANTE:
            descricaoTipoUtilizador = "Estudante";
            break;
        case TIPO_UTILIZADOR_DOCENTE:
            descricaoTipoUtilizador = "Docente";
            break;
        case TIPO_UTILIZADOR_FUNCIONARIO:
            descricaoTipoUtilizador = "Funcionario";
            break;
    }
    return descricaoTipoUtilizador;
}

void carregarUtilizadores(Utilizador utilizadores[]) {
    lerFicheiro(utilizadores, sizeof(Utilizador), NUM_MAX_UTILIZADORES, PATH_UTILIZADORES);
}

Transacao registarTransacao(Transacao transacoes[]) {

}

void consultarTransacoes(Transacao transacoes[]) {

}

int obterNumeroTransacoesRegistadas(Transacao transacoes[]) {
    int contador = 0;
    for (int index = 0; index < NUM_MAX_TRANSACOES; index++) {
        if (transacoes[index].id > 0) {
            contador++;
        }
    }
    return contador;
}

void carregarTransacoes(Transacao transacoes[]) {
    lerFicheiro(transacoes, sizeof(Transacao), NUM_MAX_TRANSACOES, PATH_TRANSACOES);
}

// Função para carregar todos os dados quando o programa é aberto
// Escolas - Utilizadores - Transações - Etc
void carregarTodosDados(Escola escolas[], Utilizador utilizadores[], Transacao transacoes[]) {
    carregarEscolas(escolas);
    carregarUtilizadores(utilizadores);
    carregarTransacoes(transacoes);
}

// Função para guardar todos os dados
// Escolas - Utilizadores - Transações
void guardarTodosDados(Escola escolas[], Utilizador utilizadores[], Transacao transacoes[]) {
    // Escolas
    gravarFicheiro(escolas, sizeof(Escola), NUM_MAX_ESCOLAS, PATH_ESCOLAS);
    gravarFicheiro(utilizadores, sizeof(Utilizador), NUM_MAX_UTILIZADORES, PATH_UTILIZADORES);
    gravarFicheiro(transacoes, sizeof(Transacao), NUM_MAX_TRANSACOES, PATH_TRANSACOES);
}

void lerFicheiro(void *buffer, int numCamposStruct, int tamanhoArray, char caminhoFicheiro[])
{
    FILE *ficheiro = fopen(caminhoFicheiro, "rb");
    int num_elementos_lidos_sucesso = 0;

    if(ficheiro == NULL)
    {
        printf("\nNao foi possivel abrir o ficheiro indicado.\n");
    }
    else
    {
        num_elementos_lidos_sucesso = fread(buffer, numCamposStruct, tamanhoArray, ficheiro);
        if(num_elementos_lidos_sucesso == 0 )
        {
            printf("Nao foi possivel ler o ficheiro: %s\n", caminhoFicheiro);
        }
        else
        {
            printf("Dados carregados do ficheiro: %s.\n", caminhoFicheiro);
        }
    }
    fclose(ficheiro);
}

void gravarFicheiro(void *buffer, int numCamposStruct, int tamanhoArray, char caminhoFicheiro[])
{
    FILE *ficheiro = fopen(caminhoFicheiro, "wb");
    int num_elementos_escritos_sucesso = 0;

    if(ficheiro == NULL)
    {
        printf("\nNao foi possivel abrir o ficheiro indicado \n");
    }
    else
    {
        num_elementos_escritos_sucesso = fwrite(buffer, numCamposStruct, tamanhoArray, ficheiro);
        if(num_elementos_escritos_sucesso ==0 )
        {
            printf("Nao foi possivel escrever para o ficheiro indicado: %s \n", caminhoFicheiro);
        }
        else
        {
            printf("Escrita bem sucedida para o ficheiro: %s \n", caminhoFicheiro);
        }
    }
    fclose(ficheiro);
}