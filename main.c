#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define NUM_MAX_ESCOLAS 5
#define NUM_MAX_UTILIZADORES 200
#define NUM_MAX_TRANSACOES 5000

#define TIPO_UTILIZADOR_ESTUDANTE 1
#define TIPO_UTILIZADOR_DOCENTE 2
#define TIPO_UTILIZADOR_FUNCIONARIO 3

#define TIPO_TRANSACAO_PAGAMENTO 1
#define TIPO_TRANSACAO_CARREGAMENTO 2

#define OPCAO_MENU_SAIR 0
#define OPCAO_MENU_ESCOLAS_REGISTAR 1
#define OPCAO_MENU_ESCOLAS_CONSULTAR 2

#define OPCAO_MENU_UTILIZADORES_REGISTAR 3
#define OPCAO_MENU_UTILIZADORES_CONSULTAR 4

#define OPCAO_MENU_TRANSACOES_REGISTAR 5
#define OPCAO_MENU_TRANSACOES_CONSULTAR 6

#define OPCAO_MENU_ESTATISTICAS_PERCENTAGEM_PAGAMENTOS 7
#define OPCAO_MENU_ESTATISTICAS_TOTAL_TRANSACOES_HORIZONTE_TEMPORAL 8

#define OPCAO_MENU_GUARDAR_DADOS 10

#define PATH_ESCOLAS "dados_escolas.bin"
#define PATH_UTILIZADORES "dados_utilizadores.bin"
#define PATH_TRANSACOES "dados_transacoes.bin"

// ################# CÓDIGO DA SEGUNDA FASE #################

#define PATH_ESCOLAS_TXT "dados_escolas.txt"
#define PATH_UTILIZADORES_TXT "dados_utilizadores.txt"
#define PATH_PRODUTOS_TXT "dados_produtos.txt"
#define PATH_PRODUTOS "dados_produtos.bin"
#define PATH_DETALHES_TRANSACAO "dados_transacao_detalhes.bin"

#define NUM_MAX_PRODUTOS 100
#define NUM_MAX_DETALHES_TRANSACAO 10000

#define OPCAO_MENU_ESCOLAS_IMPORTAR 11
#define OPCAO_MENU_UTILIZADORES_IMPORTAR 12

#define NUM_CAMPOS_STRUCT_UTILIZADOR 6 // Na verdade sao 7 mas o campo de saldo tem que ser sempre registado na aplicacao para que haja registo de transacoes com a variacao do saldo.

#define OPCAO_MENU_PRODUTOS_INTRODUZIR 13
#define OPCAO_MENU_PRODUTOS_CONSULTAR 14
#define OPCAO_MENU_PRODUTOS_IMPORTAR 15

#define NUM_CAMPOS_STRUCT_PRODUTO 4

// ################# CÓDIGO DA SEGUNDA FASE #################

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
    char data[20];
    char hora[20];
} Transacao;

int menu_opcoes();
void menu_escolas();
void menu_utilizadores();
void menu_transacoes();
void menu_estatisticas();

void carregarTodosDados();
void guardarTodosDados();
void inicializarArrays();

void lerString(char *valor, int tamanho);

char* buscarDataAtual();
char* buscarHoraAtual();

// Todas as funções relacionadas com escolas.
void inicializarArrayEscolas();
Escola registarEscola(int proximoId);
void carregarEscolas();
int obterNumeroEscolasRegistadas(Escola escolas[]);
void mostrarEscolas(Escola escolas[]);
void realizarRegistoEscola(int numEscolasRegistadas, Escola escolas[]);
Escola buscarEscola(int idEscola, Escola escolas[]);

// Todas as funções relacionadas com utilizadores.
void inicializarArrayUtilizadores();
Utilizador registarUtilizador();
Utilizador ConsultarUtilizador();
int obterNumeroUtilizadoresRegistados(Utilizador utilizadores[]);
void mostrarEscolasUtilizador(Escola escolas[]);
void mostrarUtilizadores(Utilizador utilizadores[]);
char *buscarTipoUtilizador(int tipoUtilizador);
void mostrarTiposUtilizador();
int selecionarIdUtilizador(Utilizador utilizadores[]);
void atualizarSaldoUtilizador(Utilizador utilizadores[], int idUtilizador, int tipoTransacao, float valor);
float buscarSaldoUtilizador(Utilizador utilizadores[], int idUtilizador);
Utilizador buscarUtilizador(int idUtilizador, Utilizador utilizadores[]);
void realizarRegistoUtilizador(int numUtilizadoresRegistados, int numEscolasRegistadas, Escola escolas[], Utilizador utilizadores[]);
int existeAlgumUtilizador(Utilizador utilizadores[]);

// Todas as funções relacionadas com transações.
void inicializarArrayTransacoes();
Transacao registarTransacao(int numTransacoes, int idUtilizador, int tipoTransacao, float valor);
void iniciarTransacao(Utilizador utilizadores[], Transacao transacoes[], int numTransacoesRegistadas);
void prepararTransacao(Utilizador utilizadores[], Transacao transacoes[], int numTransacoesRegistadas);
void consultarTransacao(Transacao transacoes[]);
void consultarTransacoes(Transacao transacoes[], Utilizador utilizadores[]);
int obterNumeroTransacoesRegistadas(Transacao transacoes[]);
int selecionarTipoTransacao();
char * buscarTipoTransacao(int tipoTransacao);
float pedirValorTransacao(int tipoTransacao);
int getTotalNumeroTransacoesPorTipo(Transacao transacoes[], int tipoTransacao);

void mostrarTotalFaturadoPorEscola(Escola escolas[], Transacao transacoes[], Utilizador utilizadores[], int numTransacoes); // apresentado no menu principal
void mostrarPercentagemTransacoesPorEscola();      // pagamentos por escola
void filtrarTotalPagamentos(Transacao transacoes[], Utilizador utilizadores[]);
void mostrarTotalPagamentosFiltro(char *dataInicio, char *dataFim, Transacao transacoes[], Utilizador utilizadores[], int tipoUtilizador);             // pagamentos entre duas datas por tipo de utilizador

void gravarFicheiro(void *buffer, int numCamposStruct, int tamanhoArray, char caminhoFicheiro[]);
void lerFicheiro(void *buffer, int numCamposStruct, int tamanhoArray, char caminhoFicheiro[]);

// ################# CÓDIGO DA SEGUNDA FASE #################

typedef struct
{
    int id;
    char nome[50];
    int quantidade;
    float custo;
} Produto;

typedef struct
{
    int id;
    int idTransacao;
    int idProduto;
    int quantidade;
    float valorTransacao;
} DetalheTransacao;

char *lerFicheiroDeTexto(char caminhoFicheiro[]);

void importarEscolas(Escola escolas[], int numEscolasRegistadas);
void converterCharParaEscolas(char charEscolas[], Escola escolas[]);
void converterCharParaCampoEscola(int contadorCamposEscola, int contadorEscolas, Escola escolas[], char splitEscolas[]);

void avisoImportacao(char* tipoImportacao, int limiteImportacao);
int existAlgumaEscola(int idEscola, Escola escolas[]);

void importarUtilizadores(Utilizador utilizadores[], int numUtilizadoresRegistados, Escola escolas[]);
void converterCharParaUtilizadores(char charUtilizadores[], Utilizador utilizadores[]);
void converterCharParaCampoUtilizador(int contadorCamposUtilizador, int contadorUtilizadores, Utilizador utilizadores[], char splitUtilizadores[]);

int existeAlgumUtilizadorPeloId(int idUtilizador, Utilizador utilizadores[]);

void introduzirProduto(int proximoId, Produto produtos[]);
Produto novoProduto();

void importarProdutos(Produto produtos[], int numProdutosRegistados);
void converterCharParaProdutos(char charProdutos[], Produto produtos[]);
void converterCharParaCampoProduto(int contadorCamposProduto, int contadorProdutos, Produto produtos[], char splitProdutos[]);

void comprarProduto(Produto produtos[]);
void registarDetalheTransacao(DetalheTransacao detalhesTransacao[]);
void inicializarArrayProdutos(Produto produtos[]);
void inicializarArrayDetalhesTransacao(DetalheTransacao detalhesTransacao[]);
int obterNumeroProdutosRegistados(Produto produtos[]);
int obterNumeroDetalhesTransacaoRegistados(DetalheTransacao detalhesTransacao[]);

// Necessário para operações que requerem alguém com permissões. Por exemplo introduzir/importar produtos no sistema.
int pedirVerificacaoAdmin();

void menu_produtos();
void mostrarProdutos(Produto produtos[]);

void carregarProdutos(Produto produtos[]);
void carregarDetalhesTransacao(DetalheTransacao detalhesTransacao[]);

int existAlgumProduto(int idProduto, Produto produtos[]);

// ################# CÓDIGO DA SEGUNDA FASE #################

void main()
{
    Escola escolas[NUM_MAX_ESCOLAS];
    Utilizador utilizadores[NUM_MAX_UTILIZADORES];
    Transacao transacoes[NUM_MAX_TRANSACOES];
    Produto produtos[NUM_MAX_PRODUTOS];
    DetalheTransacao detalhesTransacao[NUM_MAX_DETALHES_TRANSACAO];

    inicializarArrays(escolas, utilizadores, transacoes, produtos, detalhesTransacao);
    carregarTodosDados(escolas, utilizadores, transacoes, produtos, detalhesTransacao);

    int opcaoMenu = 0;

    do {
        int numEscolasRegistadas = obterNumeroEscolasRegistadas(escolas);
        int numUtilizadoresRegistados = obterNumeroUtilizadoresRegistados(utilizadores);
        int numTransacoesRegistadas = obterNumeroTransacoesRegistadas(transacoes);
        int numProdutosRegistados = obterNumeroProdutosRegistados(produtos);
        int numDetalhesTransacaoRegistados = obterNumeroDetalhesTransacaoRegistados(detalhesTransacao);

        mostrarTotalFaturadoPorEscola(escolas, transacoes, utilizadores, numTransacoesRegistadas);
        fflush(stdin);
        opcaoMenu = menu_opcoes();
        switch (opcaoMenu) {
            case OPCAO_MENU_ESCOLAS_REGISTAR:
                realizarRegistoEscola(numEscolasRegistadas, escolas);
                break;

            case OPCAO_MENU_ESCOLAS_CONSULTAR:
                mostrarEscolas(escolas);
                break;

            // ################# CÓDIGO DA SEGUNDA FASE #################
            case OPCAO_MENU_ESCOLAS_IMPORTAR:
                system("cls");
                importarEscolas(escolas, numEscolasRegistadas);
                break;
            // ################# CÓDIGO DA SEGUNDA FASE #################

            case OPCAO_MENU_UTILIZADORES_REGISTAR:
                realizarRegistoUtilizador(numUtilizadoresRegistados, numEscolasRegistadas, escolas, utilizadores);
                break;

            case OPCAO_MENU_UTILIZADORES_CONSULTAR:
                mostrarUtilizadores(utilizadores);
                break;

            // ################# CÓDIGO DA SEGUNDA FASE #################
            case OPCAO_MENU_UTILIZADORES_IMPORTAR:
                system("cls");
                importarUtilizadores(utilizadores, numUtilizadoresRegistados, escolas);
                break;
            // ################# CÓDIGO DA SEGUNDA FASE #################

            case OPCAO_MENU_TRANSACOES_REGISTAR:
                iniciarTransacao(utilizadores, transacoes, numTransacoesRegistadas);
                break;

            case OPCAO_MENU_TRANSACOES_CONSULTAR:
                system("cls");
                consultarTransacoes(transacoes, utilizadores);
                break;

            case OPCAO_MENU_ESTATISTICAS_PERCENTAGEM_PAGAMENTOS:
                system("cls");
                mostrarPercentagemTransacoesPorEscola(escolas, transacoes, utilizadores, numTransacoesRegistadas, numEscolasRegistadas);
                break;

            case OPCAO_MENU_ESTATISTICAS_TOTAL_TRANSACOES_HORIZONTE_TEMPORAL:
                system("cls");
                filtrarTotalPagamentos(transacoes, utilizadores);
                break;

            case OPCAO_MENU_GUARDAR_DADOS:
                guardarTodosDados(escolas, utilizadores, transacoes, produtos, detalhesTransacao);
                break;

            // ################# CÓDIGO DA SEGUNDA FASE #################
            case OPCAO_MENU_PRODUTOS_INTRODUZIR:
                system("cls");
                introduzirProduto(numProdutosRegistados, produtos);
                break;

            case OPCAO_MENU_PRODUTOS_CONSULTAR:
                mostrarProdutos(produtos);
                break;

            case OPCAO_MENU_PRODUTOS_IMPORTAR:
                system("cls");
                importarProdutos(produtos, numProdutosRegistados);
                break;
            // ################# CÓDIGO DA SEGUNDA FASE #################

            case OPCAO_MENU_SAIR:
            default:
                break;
        }
    } while (opcaoMenu != OPCAO_MENU_SAIR);

    guardarTodosDados(escolas, utilizadores, transacoes, produtos, detalhesTransacao);
}

void lerString(char *valor, int tamanho) {
    fflush(stdin);
    fgets(valor, tamanho, stdin);
    valor[strcspn(valor, "\n")] = 0;
}

// Vai inicializar o array das escolas e garantir que todos os id's comecem a 0.
// Se o id tiver a 0 significa que a posição do array ainda não foi preenchida.
void inicializarArrays(Escola escolas[], Utilizador utilizadores[], Transacao transacoes[], Produto produtos[], DetalheTransacao detalhesTransacao[]) {
    inicializarArrayEscolas(escolas);
    inicializarArrayUtilizadores(utilizadores);
    inicializarArrayTransacoes(transacoes);
    inicializarArrayProdutos(produtos);
    inicializarArrayDetalhesTransacao(detalhesTransacao);
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
        strcpy(transacoes[index].data, "");
        strcpy(transacoes[index].hora, "");
    }
}

void inicializarArrayProdutos(Produto produtos[]) {
    for (int index = 0; index < NUM_MAX_PRODUTOS; index++) {
        produtos[index].id = 0;
        produtos[index].custo = 0;
        produtos[index].quantidade = 0;
        strcpy(produtos[index].nome, "");
    }
}

void inicializarArrayDetalhesTransacao(DetalheTransacao detalhesTransacao[]) {
    for (int index = 0; index < NUM_MAX_DETALHES_TRANSACAO; index++) {
        detalhesTransacao[index].id = 0;
        detalhesTransacao[index].idProduto = 0;
        detalhesTransacao[index].idTransacao = 0;
        detalhesTransacao[index].quantidade = 0;
        detalhesTransacao[index].valorTransacao = 0;
    }
}

int menu_opcoes() {
    int menuSelecionado = 0;

    printf("******************************** MENU PRINCIPAL ********************************\n");
    menu_escolas();
    menu_utilizadores();
    menu_transacoes();
    menu_estatisticas();
    menu_produtos();
    printf("*******\n");
    printf("* [%d] * Guardar dados\n", OPCAO_MENU_GUARDAR_DADOS);
    printf("* [%d]  * Sair\n", OPCAO_MENU_SAIR);
    printf("* Selecionar menu: ");
    scanf("%d", &menuSelecionado);
    printf("\n********************************************************************************\n");
    return menuSelecionado;
}

void menu_escolas() {
    printf("******* Escolas: ********\n");
    printf("* [%d] * Registar Escolas\n", OPCAO_MENU_ESCOLAS_REGISTAR);
    printf("* [%d] * Consultar Escolas\n", OPCAO_MENU_ESCOLAS_CONSULTAR);

    // ################# CÓDIGO DA SEGUNDA FASE #################
    printf("* [%d] * Importar Escolas\n", OPCAO_MENU_ESCOLAS_IMPORTAR);
}

void menu_utilizadores() {
    printf("******* Utilizadores: ********\n");
    printf("* [%d] * Registar Utilizador\n", OPCAO_MENU_UTILIZADORES_REGISTAR);
    printf("* [%d] * Consultar Utilizador\n", OPCAO_MENU_UTILIZADORES_CONSULTAR);

    // ################# CÓDIGO DA SEGUNDA FASE #################
    printf("* [%d] * Importar Utilizadores\n", OPCAO_MENU_UTILIZADORES_IMPORTAR);
}

void menu_transacoes() {
    printf("******* Transacoes: ********\n");
    printf("* [%d] * Registar Transacao\n", OPCAO_MENU_TRANSACOES_REGISTAR);
    printf("* [%d] * Consultar Transacao\n", OPCAO_MENU_TRANSACOES_CONSULTAR);
}

void menu_estatisticas() {
    printf("******* Estatisticas: ********\n");
    printf("* [%d] * Consultar percentagem pagamentos por escola\n", OPCAO_MENU_ESTATISTICAS_PERCENTAGEM_PAGAMENTOS);
    printf("* [%d] * Consultar total de transacoes entre duas datas e tipo de utilizador\n", OPCAO_MENU_ESTATISTICAS_TOTAL_TRANSACOES_HORIZONTE_TEMPORAL);
}

void menu_produtos() {
    printf("******* Produtos: ********\n");
    printf("* [%d] * Registar produto\n", OPCAO_MENU_PRODUTOS_INTRODUZIR);
    printf("* [%d] * Consultar produtos\n", OPCAO_MENU_PRODUTOS_CONSULTAR);
    printf("* [%d] * Importar produtos\n", OPCAO_MENU_PRODUTOS_IMPORTAR);
}

Escola registarEscola(int proximoId)
{
    system("cls");
    Escola escola;
    escola.id = proximoId + 1;
    printf("* Registo de nova escola\n");
    printf("* Nome: ");
    lerString(escola.nome, 100);
    printf("* Abreviacao: ");
    lerString(escola.abreviacao, 10)    ;
    printf("* Localidade: ");
    lerString(escola.localidade, 20);
    printf("* Campus: ");
    scanf("%d", &escola.campus);
    system("cls");
    return escola;
}

void realizarRegistoEscola(int numEscolasRegistadas, Escola escolas[]) {
    if (numEscolasRegistadas < NUM_MAX_ESCOLAS) {
        escolas[numEscolasRegistadas] = registarEscola(numEscolasRegistadas);
    }
    else {
        printf("* Nao e possivel registar mais escolas pois ja antigiu o limite: %d.\n", NUM_MAX_ESCOLAS);
    }
}

void carregarEscolas(Escola escolas[]) {
    lerFicheiro(escolas, sizeof(Escola), NUM_MAX_ESCOLAS, PATH_ESCOLAS);
}

void mostrarEscolas(Escola escolas[])
{
    system("cls");
    int existeEscolas = 0;
    for (int index = 0; index < NUM_MAX_ESCOLAS; index++)
    {
        if (escolas[index].id > 0) {
            existeEscolas = 1;
            printf("********************************************************************************\n");
            printf("* Id: %d\n", escolas[index].id);
            printf("* Abreviacao: %s\n", escolas[index].abreviacao);
            printf("* Nome: %s\n", escolas[index].nome);
            printf("* Campus: %d\n", escolas[index].campus);
            printf("* Localidade: %s\n", escolas[index].localidade);
        }
        if (index + 1 == NUM_MAX_ESCOLAS)
        {
            if (existeEscolas == 0) {
                printf("* Nao existe qualquer registo de escolas.\n");
            }
            else {
                printf("********************************************************************************\n");
            }
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

Escola buscarEscola(int idEscola, Escola escolas[]) {
    for(int index = 0; index < NUM_MAX_ESCOLAS; index++) {
        if (escolas[index].id == idEscola) {
            return escolas[index];
        }
    }
}

Utilizador registarUtilizador(int proximoId, Escola escolas[]) {
    Utilizador utilizador;
    utilizador.id = proximoId + 1;
    printf("****** Registo de Novo Utilizador:\n");
    mostrarEscolasUtilizador(escolas);
    printf("* Id Escola: ");
    scanf("%d", &utilizador.idEscola);
    printf("* Nome: ");
    lerString(utilizador.nome, 100);
    mostrarTiposUtilizador();
    printf("* Utilizador: ");
    scanf("%d", &utilizador.tipoUtilizador);
    printf("* Email: ");
    lerString(utilizador.email, 100);

    while(utilizador.nif < 100000000) {
        printf("* NIF: ");
        scanf("%d", &utilizador.nif);
        if (utilizador.nif < 100000000) {
            printf("* Por favor, introduza um nif valido.\n");
        }
    }
    utilizador.saldo = 0;
    system("cls");
    return utilizador;
}

void realizarRegistoUtilizador(int numUtilizadoresRegistados, int numEscolasRegistadas, Escola escolas[], Utilizador utilizadores[]) {
    system("cls");
    if (numUtilizadoresRegistados < NUM_MAX_UTILIZADORES) {
        if (numEscolasRegistadas > 0) {
            utilizadores[numUtilizadoresRegistados] = registarUtilizador(numUtilizadoresRegistados, escolas);
        }
        else {
            printf("* Nao e possivel registar um utilizador sem haver pelo menos uma escola registada\n");
        }
    }
    else {
        printf("* Nao e possivel registar mais utilizadores pois ja antigiu o limite: %d.\n", NUM_MAX_UTILIZADORES);
    }
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
    int existeUtilizadores = 0;
    for (int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizadores[index].id > 0) {
            existeUtilizadores = 1;
            printf("********************************************************************************\n");
            printf("* Id: %d\n", utilizadores[index].id);
            printf("* Id Escola: %d\n", utilizadores[index].idEscola);
            printf("* Nif: %d\n", utilizadores[index].nif);
            printf("* Tipo Utilizador: %d\n", utilizadores[index].tipoUtilizador);
            printf("* Email: %s\n", utilizadores[index].email);
            printf("* Nome: %s\n", utilizadores[index].nome);
            printf("* Saldo: %.2f\n", utilizadores[index].saldo);
        }
        if (index + 1 == NUM_MAX_UTILIZADORES)
        {
            if (existeUtilizadores == 0) {
                printf("* Nao existe qualquer registo de utilizadores.\n");
            }
            else {
                printf("********************************************************************************\n");
            }
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

Utilizador buscarUtilizador (int idUtilizador, Utilizador utilizadores[]) {
    Utilizador utilizador;
     for (int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizadores[index].id == idUtilizador) {
            utilizador = utilizadores[index];
            index = NUM_MAX_UTILIZADORES;
        }
     }
     return utilizador;
}

void carregarUtilizadores(Utilizador utilizadores[]) {
    lerFicheiro(utilizadores, sizeof(Utilizador), NUM_MAX_UTILIZADORES, PATH_UTILIZADORES);
}

int selecionarIdUtilizador(Utilizador utilizadores[]) {
    fflush(stdin);
    system("cls");
    int utilizador = 0;
    for (int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizadores[index].id > 0) {
            printf("** Id: [%d] - Nome: %s - Tipo Utilizador: %s\n", utilizadores[index].id, utilizadores[index].nome, buscarTipoUtilizador(utilizadores[index].tipoUtilizador));
        }
    }
    printf("* Selecione um dos utilizadores a cima para continuar a transacao: ");
    scanf("%d", &utilizador);
    return utilizador;
}

float buscarSaldoUtilizador(Utilizador utilizadores[], int idUtilizador) {
    for (int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizadores[index].id == idUtilizador ) {
            return utilizadores[index].saldo;
        }
    }
}

int existeAlgumUtilizador(Utilizador utilizador[]) {
    int existe = 0;
    for(int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizador[index].id > 0) {
            existe = 1;
            index = NUM_MAX_UTILIZADORES;
        }
    }
    return existe;
}

void iniciarTransacao(Utilizador utilizadores[], Transacao transacoes[], int numTransacoesRegistadas) {
    if (numTransacoesRegistadas < NUM_MAX_TRANSACOES) {
        fflush(stdin);
        system("cls");
        if (existeAlgumUtilizador(utilizadores) == 1 ) {
            prepararTransacao(utilizadores, transacoes, numTransacoesRegistadas);
        }
        else {
            printf("* Nao e possivel realizar uma transacao sem utilizadores registados.\n");
        }
    }
    else {
        printf("* Nao e possivel realizar mais transacoes pois ja antigiu o limite: %d.\n", NUM_MAX_TRANSACOES);
    }
}

void prepararTransacao(Utilizador utilizadores[], Transacao transacoes[], int numTransacoesRegistadas) {
    int idUtilizador = selecionarIdUtilizador(utilizadores);
    int tipoTransacao = selecionarTipoTransacao();
    float valorTransacao = pedirValorTransacao(tipoTransacao);
    int dinheiroSuficiente = 1;
    float saldoUtilizador = 0;

    if (tipoTransacao == TIPO_TRANSACAO_PAGAMENTO) {
        saldoUtilizador = buscarSaldoUtilizador(utilizadores, idUtilizador);
        dinheiroSuficiente = saldoUtilizador >= valorTransacao;
    }
    if (dinheiroSuficiente == 1) {
        atualizarSaldoUtilizador(utilizadores, idUtilizador, tipoTransacao, valorTransacao);
        transacoes[numTransacoesRegistadas] = registarTransacao(numTransacoesRegistadas, idUtilizador, tipoTransacao, valorTransacao);
    }
    else {
        printf("* Utilizador com saldo insuficiente para realizar o pagamento.\n");
        printf("* Saldo utilizador: %.2f\n", saldoUtilizador);
    }
}

Transacao registarTransacao(int numTransacoes, int idUtilizador, int tipoTransacao, float valor) {
    Transacao transacao;
    transacao.id = numTransacoes + 1;
    transacao.idUtilizador = idUtilizador;
    transacao.tipoTransacao = tipoTransacao;
    transacao.valorTransacao = valor;
    strcpy(transacao.data, buscarDataAtual());
    strcpy(transacao.hora, buscarHoraAtual());
    return transacao;
}

void atualizarSaldoUtilizador(Utilizador utilizadores[], int idUtilizador, int tipoTransacao, float valor) {
    for(int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizadores[index].id == idUtilizador) {
            float saldoAnteriror = utilizadores[index].saldo;

            if (tipoTransacao == TIPO_TRANSACAO_PAGAMENTO) {
                utilizadores[index].saldo -= valor;
            }
            if (tipoTransacao == TIPO_TRANSACAO_CARREGAMENTO) {
                utilizadores[index].saldo += valor;
                printf("* Carregamento realizado com sucesso.\n");
            }

            printf("* Saldo anterior: %.2f - Saldo atual: %.2f\n", saldoAnteriror, utilizadores[index].saldo);
            index = NUM_MAX_UTILIZADORES;
        }
    }

}

void consultarTransacoes(Transacao transacoes[], Utilizador utilizadores[]) {
    system("cls");
    int existeTransacoes = 0;
    for (int index = 0; index < NUM_MAX_TRANSACOES; index++) {
        if (transacoes[index].id > 0) {
            existeTransacoes = 1;
            printf("* Transacao: %d - Utilizador: %s - Tipo Transacao: %s - Valor: %.2f - Data: %s - Hora: %s\n",
                transacoes[index].id,
                buscarUtilizador(transacoes[index].idUtilizador, utilizadores).nome,
                buscarTipoTransacao(transacoes[index].tipoTransacao),
                transacoes[index].valorTransacao,
                transacoes[index].data,
                transacoes[index].hora);
        }
        if (index + 1 == NUM_MAX_TRANSACOES) {
            if (existeTransacoes == 0) {
                printf("* Nao existe qualquer registo de transacoes.\n");
            }
        }
    }
}

int selecionarTipoTransacao() {
    fflush(stdin);
    int tipoTransacao = 0;
    printf("* Tipo de transacao: \n");
    printf("** [%d] - Pagamento\n", TIPO_TRANSACAO_PAGAMENTO);
    printf("** [%d] - Carregamento\n", TIPO_TRANSACAO_CARREGAMENTO);
    printf("* Selecionar tipo de transacao: ");
    scanf("%d", &tipoTransacao);
    return tipoTransacao;
}

float pedirValorTransacao(int tipoTransacao) {
    fflush(stdin);
    float valor = 0;

    if (tipoTransacao == TIPO_TRANSACAO_PAGAMENTO) {
        printf("* Valor do pagamento: ");
    }

    if (tipoTransacao == TIPO_TRANSACAO_CARREGAMENTO) {
        printf("* Valor do carregamento: ");
    }

    scanf("%f", &valor);
    return valor;
}

char *buscarTipoTransacao(int tipoTransacao) {
    char *tipo;

    if (tipoTransacao == TIPO_TRANSACAO_PAGAMENTO) {
        tipo = "Pagamento";
    }

    if (tipoTransacao == TIPO_TRANSACAO_CARREGAMENTO) {
        tipo = "Carregamento";
    }

    return tipo;
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

char* buscarDataAtual() {
    char* charData;
    time_t dataAtual = time(NULL);
    struct tm *tm_struct  = localtime(&dataAtual);

    int dia = tm_struct->tm_mday;
    int mes = tm_struct->tm_mon;
    int ano = tm_struct->tm_year;

    sprintf(charData, "%d-%d-%d", dia, mes + 1, ano);

    return charData;

}

char* buscarHoraAtual() {
    char* charHora;

    time_t horaAtual = time(NULL);
    struct tm *tm_struct  = localtime(&horaAtual);
    int hora = tm_struct->tm_hour;
    int minuto = tm_struct->tm_min;
    int segundo = tm_struct->tm_sec;

    sprintf(charHora, "%d:%d:%d", hora, minuto, segundo);

    return charHora;
}

void mostrarTotalFaturadoPorEscola(Escola escolas[], Transacao transacoes[], Utilizador utilizadores[], int numTransacoes) {
    for (int indexEscola = 0; indexEscola < NUM_MAX_ESCOLAS; indexEscola++) {
        float totalFaturado = 0;
        if (escolas[indexEscola].id > 0) {
            if (indexEscola == 0) {
                printf("********************************************************************************\n");
                printf("** Total faturado por escola: \n");
            }
            for (int indexUtilizador = 0; indexUtilizador < NUM_MAX_UTILIZADORES; indexUtilizador++) {
                if (escolas[indexEscola].id == utilizadores[indexUtilizador].idEscola) {
                    for (int indexTransacao = 0; indexTransacao < numTransacoes; indexTransacao++) {
                        if (transacoes[indexTransacao].tipoTransacao == TIPO_TRANSACAO_PAGAMENTO && transacoes[indexTransacao].idUtilizador == utilizadores[indexUtilizador].id) {
                            totalFaturado += transacoes[indexTransacao].valorTransacao;
                        }
                    }
                }
            }
            printf("** [Escola]: %s - [Faturado]: %.2f\n", escolas[indexEscola].nome, totalFaturado);
        }
    }
}

void mostrarPercentagemTransacoesPorEscola(Escola escolas[], Transacao transacoes[], Utilizador utilizadores[], int numTransacoes, int numEscolas) {
    for (int indexEscola = 0; indexEscola < NUM_MAX_ESCOLAS; indexEscola++) {
        int totalTransacoesEscola = 0;
        if (escolas[indexEscola].id > 0) {
            if (indexEscola == 0) {
                printf("********************************************************************************\n");
                printf("** Percentagem de pagamentos por escola: \n");
            }
            for (int indexUtilizador = 0; indexUtilizador < NUM_MAX_UTILIZADORES; indexUtilizador++) {
                if (escolas[indexEscola].id == utilizadores[indexUtilizador].idEscola) {
                    for (int indexTransacao = 0; indexTransacao < numTransacoes; indexTransacao++) {
                        if (transacoes[indexTransacao].tipoTransacao == TIPO_TRANSACAO_PAGAMENTO && transacoes[indexTransacao].idUtilizador == utilizadores[indexUtilizador].id) {
                            totalTransacoesEscola++;
                        }
                    }
                }
            }
            int percentagem =  100 * totalTransacoesEscola / getTotalNumeroTransacoesPorTipo(transacoes, TIPO_TRANSACAO_PAGAMENTO);
            printf("** [Escola]: %s - [Percentagem]: %d\n", escolas[indexEscola].nome, percentagem);
        }
    }
}

void filtrarTotalPagamentos(Transacao transacoes[], Utilizador utilizadores[]) {
    char *dataInicio = malloc(100);
    char *dataFim = malloc(100);

    printf("* Introduzir data de inicio: \n");
    lerString(dataInicio, 100);

    printf("* Introduzir data de fim: \n");
    lerString(dataFim, 100);

    int tipoUtilizador = 0;
    mostrarTiposUtilizador();
    printf("* Introduzir tipo utilizador: ");
    scanf("%d", &tipoUtilizador);

    mostrarTotalPagamentosFiltro(dataInicio, dataFim, transacoes, utilizadores, tipoUtilizador);
}

// Filtro não está a ser feito
void mostrarTotalPagamentosFiltro(char *dataInicio, char *dataFim, Transacao transacoes[], Utilizador utilizadores[], int tipoUtilizador) {
    int totalTransacoes = 0;
    for(int indexTransacao = 0; indexTransacao < NUM_MAX_TRANSACOES; indexTransacao++) {
        if (transacoes[indexTransacao].id > 0) {
            for (int indexUtilizador = 0; indexUtilizador < NUM_MAX_UTILIZADORES; indexUtilizador++) {
                if (utilizadores[indexUtilizador].tipoUtilizador == tipoUtilizador && utilizadores[indexUtilizador].id == transacoes[indexTransacao].idUtilizador) {
                    totalTransacoes++;
                }
            }
        }
    }
    if (totalTransacoes > 0) {
        printf("** Total transacoes entre %s e %s com o tipo de utilizador %s - [%d]\n", dataInicio, dataFim, buscarTipoUtilizador(tipoUtilizador), totalTransacoes);
    }
}

int getTotalNumeroTransacoesPorTipo(Transacao transacoes[], int tipoTransacao) {
    int total = 0;
    for (int index = 0; index < NUM_MAX_TRANSACOES; index++) {
        if (transacoes[index].tipoTransacao == tipoTransacao) {
            total++;
        }
    }
    return total;
}

// Função para carregar todos os dados quando o programa é aberto
// Escolas - Utilizadores - Transações - Etc
void carregarTodosDados(Escola escolas[], Utilizador utilizadores[], Transacao transacoes[], Produto produtos[], DetalheTransacao detalhesTransacao[]) {
    carregarEscolas(escolas);
    carregarUtilizadores(utilizadores);
    carregarTransacoes(transacoes);
    carregarProdutos(produtos);
    carregarDetalhesTransacao(detalhesTransacao);
}

// Função para guardar todos os dados
// Escolas - Utilizadores - Transações
void guardarTodosDados(Escola escolas[], Utilizador utilizadores[], Transacao transacoes[], Produto produtos[], DetalheTransacao detalhesTransacao[]) {
    system("cls");
    // Escolas
    gravarFicheiro(escolas, sizeof(Escola), NUM_MAX_ESCOLAS, PATH_ESCOLAS);
    gravarFicheiro(utilizadores, sizeof(Utilizador), NUM_MAX_UTILIZADORES, PATH_UTILIZADORES);
    gravarFicheiro(transacoes, sizeof(Transacao), NUM_MAX_TRANSACOES, PATH_TRANSACOES);
    gravarFicheiro(produtos, sizeof(Produto), NUM_MAX_PRODUTOS, PATH_PRODUTOS);
    gravarFicheiro(detalhesTransacao, sizeof(DetalheTransacao), NUM_MAX_DETALHES_TRANSACAO, PATH_DETALHES_TRANSACAO);
}

void lerFicheiro(void *buffer, int numCamposStruct, int tamanhoArray, char caminhoFicheiro[])
{
    FILE *ficheiro = fopen(caminhoFicheiro, "rb");
    int num_elementos_lidos_sucesso = 0;

    if(ficheiro == NULL)
    {
        printf("Nao foi possivel abrir o ficheiro indicado: %s.\n", caminhoFicheiro);
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
        printf("Nao foi possivel abrir o ficheiro indicado %s\n", caminhoFicheiro);
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

// ################# CÓDIGO DA SEGUNDA FASE #################

char *lerFicheiroDeTexto(char caminhoFicheiro[]) {
    FILE *dadosFicheiro;
    dadosFicheiro = fopen(caminhoFicheiro, "r");
    if (dadosFicheiro == NULL)
    {
        printf("* O ficheiro indicado nao existe: %s\n", caminhoFicheiro);
        exit(EXIT_FAILURE);
    }
    char *buffer = malloc(1024);
    char *dadosFinais = malloc(1024);
    int index = 0;
    while (fgets(buffer, 1024, dadosFicheiro)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        if (index > 0) {
            strcat(dadosFinais, buffer);
        }
        else {
            strcpy(dadosFinais, buffer);
        }
        index++;
    }
    fclose(dadosFicheiro);
    return dadosFinais;
}

void avisoImportacao(char *tipoImportacao, int limiteImportacao) {
    printf("\n********************************************************************************\n");
    printf("** ATENCAO **\n");
    printf("** Limite de importacao de %s: %d\n", tipoImportacao, limiteImportacao);
    printf("** O programa vai ignorar a informacao superior ao limite indicado.\n");
    printf("** Todos os dados que conterem ids ja existentes na aplicacao tambem nao serao importados.\n");
    if (tipoImportacao == "utilizadores") {
        printf("** No caso de ser inserido um utilizador com um ID de uma escola inexistente nao sera importado no sistema.\n");
    }
    printf("********************************************************************************\n");
}

void importarEscolas(Escola escolas[], int numEscolasRegistadas) {
    avisoImportacao("escolas", NUM_MAX_ESCOLAS);
    char *dadosEscolas = lerFicheiroDeTexto(PATH_ESCOLAS_TXT);
    Escola escolasImportadas[NUM_MAX_ESCOLAS];
    inicializarArrayEscolas(escolasImportadas);

    converterCharParaEscolas(dadosEscolas, escolasImportadas);
    int numEscolasImportadas = 0;
    for (int indexEscolasAtuais = 0; indexEscolasAtuais < NUM_MAX_ESCOLAS; indexEscolasAtuais++) {
        for (int indexEscolasImportadas = 0; indexEscolasImportadas < NUM_MAX_ESCOLAS; indexEscolasImportadas++) {
            if (escolasImportadas[indexEscolasImportadas].id > 0 && escolas[indexEscolasAtuais].id == 0 && !existAlgumaEscola(escolasImportadas[indexEscolasImportadas].id, escolas)) {
                if (numEscolasRegistadas < NUM_MAX_ESCOLAS) {
                    numEscolasImportadas++;
                    numEscolasRegistadas++;
                    escolas[indexEscolasAtuais] = escolasImportadas[indexEscolasImportadas];
                    printf("Nome escola importada: %s - Id: %d - Abreviacao: %s\n", escolasImportadas[indexEscolasImportadas].nome, escolasImportadas[indexEscolasImportadas].id, escolasImportadas[indexEscolasImportadas].abreviacao);
                }
                else {
                    printf("* Nao e possivel registar mais escolas pois já chegou ao limite: %d\n", NUM_MAX_ESCOLAS);
                }
            }
        }
    }
    if (escolasImportadas > 0) {
        printf("* Numero de escolas importadas: %d\n", numEscolasImportadas);
    }
}

void converterCharParaEscolas(char charEscolas[], Escola escolas[]) {
    int contadorEscolas = 0, contadorCamposEscola = 1;
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

int existAlgumaEscola(int idEscola, Escola escolas[]) {
    int existe = 0;
    for (int index = 0; index < NUM_MAX_ESCOLAS; index++) {
        if (escolas[index].id > 0 && idEscola == escolas[index].id) {
            existe = 1;
        }
    }
    return existe;
}

void importarUtilizadores(Utilizador utilizadores[], int numUtilizadoresRegistados, Escola escolas[]) {
    avisoImportacao("utilizadores", NUM_MAX_UTILIZADORES);
    char *dadosUtilizadores = lerFicheiroDeTexto(PATH_UTILIZADORES_TXT);
    Utilizador utilizadoresImportados[NUM_MAX_UTILIZADORES];
    inicializarArrayUtilizadores(utilizadoresImportados);
    converterCharParaUtilizadores(dadosUtilizadores, utilizadoresImportados);
    int numUtilizadoresImportados = 0;
    for (int indexUtilizadoresAtuais = 0; indexUtilizadoresAtuais < NUM_MAX_UTILIZADORES; indexUtilizadoresAtuais++) {
        for (int indexUtilizadoresImportados = 0; indexUtilizadoresImportados < NUM_MAX_UTILIZADORES; indexUtilizadoresImportados++) {
            if (utilizadoresImportados[indexUtilizadoresImportados].id > 0 && utilizadores[indexUtilizadoresAtuais].id == 0 && existAlgumaEscola(utilizadoresImportados[indexUtilizadoresImportados].idEscola, escolas) && !existeAlgumUtilizadorPeloId(utilizadoresImportados[indexUtilizadoresImportados].id, utilizadores)) {
                if (numUtilizadoresRegistados < NUM_MAX_UTILIZADORES) {
                    numUtilizadoresImportados++;
                    utilizadores[indexUtilizadoresAtuais] = utilizadoresImportados[indexUtilizadoresImportados];
                    printf("Nome utilizador importado: %s - Id: %d\n", utilizadoresImportados[indexUtilizadoresImportados].nome, utilizadoresImportados[indexUtilizadoresImportados].id);
                }
                else {
                    printf("* Nao e possivel registar mais utilizadores pois já chegou ao limite: %d\n", NUM_MAX_UTILIZADORES);
                }
            }
        }
    }
    if (numUtilizadoresImportados > 0) {
        printf("* Numero de utilizadores importados: %d\n", numUtilizadoresImportados);
    }
}

void converterCharParaUtilizadores(char *charUtilizadores, Utilizador utilizadores[]) {
    int contadorUtilizadores = 0, contadorCamposUtilizador = 1;
    char *splitUtilizadores = strtok(charUtilizadores, ";");

    while (splitUtilizadores != NULL) {
        converterCharParaCampoUtilizador(contadorCamposUtilizador, contadorUtilizadores, utilizadores, splitUtilizadores);
        contadorCamposUtilizador++;

        if (contadorCamposUtilizador > NUM_CAMPOS_STRUCT_UTILIZADOR) {
            contadorCamposUtilizador = 1;
            contadorUtilizadores++;
        }
		splitUtilizadores = strtok(NULL, ";");
	}
}

void converterCharParaCampoUtilizador(int contadorCamposUtilizador, int contadorUtilizadores, Utilizador utilizadores[], char splitUtilizadores[]) {
    switch (contadorCamposUtilizador) {
        case 1:
            utilizadores[contadorUtilizadores].id = (atoi)(splitUtilizadores);
            break;
        case 2:
            utilizadores[contadorUtilizadores].idEscola = (atoi)(splitUtilizadores);
            break;
        case 3:
            strcpy(utilizadores[contadorUtilizadores].nome, splitUtilizadores);
            break;
        case 4:
            utilizadores[contadorUtilizadores].nif = atoi(splitUtilizadores);
            break;
        case 5:
            utilizadores[contadorUtilizadores].tipoUtilizador = atoi(splitUtilizadores);
            break;
        case 6:
            strcpy(utilizadores[contadorUtilizadores].email, splitUtilizadores);
            break;
    }
}

int existeAlgumUtilizadorPeloId(int idUtilizador, Utilizador utilizadores[]) {
    int existe = 0;
    for (int index = 0; index < NUM_MAX_UTILIZADORES; index++) {
        if (utilizadores[index].id > 0 && utilizadores[index].id == idUtilizador) {
            existe = 1;
        }
    }
    return existe;
}

int pedirVerificacaoAdmin() {
    int senha = 1234;
    int senhaIntroduzida = 0;

    int sair = 0;
    do {
        fflush(stdin);
        printf("** Introduzir senha de administracao: ");
        scanf("%d", &senhaIntroduzida);
        if (senhaIntroduzida != senha) {
            printf("** Senha invalida. Quer cancelar a operacao?\n");
            printf("** [0] - Nao | [1] - Sim\n");
            printf("** Opcao: ");
            scanf("%d", &sair);
        }
    } while (senhaIntroduzida != senha && sair != 1);

    return senhaIntroduzida == senha;
}

void introduzirProduto(int proximoId, Produto produtos[]) {
    if (pedirVerificacaoAdmin() == 1) {
        produtos[proximoId] = novoProduto(proximoId + 1);
    }
}

Produto novoProduto(int id) {
    Produto produto;
    produto.id = id;

    printf("* Nome do produto: ");
    lerString(produto.nome, 50);
    printf("* Custo preco: ");
    scanf("%f", &produto.custo);
    printf("* Quantidade: ");
    scanf("%d", &produto.quantidade);

    return produto;
}

void importarProdutos(Produto produtos[], int numProdutosRegistados) {
    if (pedirVerificacaoAdmin() == 1) {
        avisoImportacao("produtos", NUM_MAX_ESCOLAS);
        char *dadosProdutos = lerFicheiroDeTexto(PATH_PRODUTOS_TXT);
        Produto produtosImportados[NUM_MAX_PRODUTOS];
        inicializarArrayProdutos(produtosImportados);

        converterCharParaProdutos(dadosProdutos, produtosImportados);
        int numProdutosImportados = 0;
        for (int indexProdutosAtuais = 0; indexProdutosAtuais < NUM_MAX_PRODUTOS; indexProdutosAtuais++) {
            for (int indexProdutosImportados = 0; indexProdutosImportados < NUM_MAX_PRODUTOS; indexProdutosImportados++) {
                if (produtosImportados[indexProdutosImportados].id > 0 && produtos[indexProdutosAtuais].id == 0 && !existAlgumProduto(produtosImportados[indexProdutosImportados].id, produtos)) {
                    if (numProdutosRegistados < NUM_MAX_PRODUTOS) {
                        numProdutosImportados++;
                        numProdutosRegistados++;
                        produtos[indexProdutosAtuais] = produtosImportados[indexProdutosImportados];
                        printf("Nome produto importado: %s - Id: %d - Custo: %.2f - Quantidade: %d\n", produtosImportados[indexProdutosImportados].nome, produtosImportados[indexProdutosImportados].id, produtosImportados[indexProdutosImportados].custo, produtosImportados[indexProdutosImportados].quantidade);
                    }
                    else {
                        printf("* Nao e possivel registar mais produtos pois já chegou ao limite: %d\n", NUM_MAX_ESCOLAS);
                    }
                }
            }
        }
    }
}

int existAlgumProduto(int idProduto, Produto produtos[]) {
    int existe = 0;
    for (int index = 0; index < NUM_MAX_PRODUTOS; index++) {
        if (produtos[index].id > 0 && idProduto == produtos[index].id) {
            existe = 1;
        }
    }
    return existe;
}

int obterNumeroProdutosRegistados(Produto produtos[]) {
    int contador = 0;
    for (int index = 0; index < NUM_MAX_PRODUTOS; index++) {
        if (produtos[index].id > 0) {
            contador++;
        }
    }
    return contador;
}

int obterNumeroDetalhesTransacaoRegistados(DetalheTransacao detalhesTransacao[]) {
    int contador = 0;
    for (int index = 0; index < NUM_MAX_DETALHES_TRANSACAO; index++) {
        if (detalhesTransacao[index].id > 0) {
            contador++;
        }
    }
    return contador;
}

void mostrarProdutos(Produto produtos[]) {
    system("cls");
    int existeProduto = 0;
    for (int index = 0; index < NUM_MAX_PRODUTOS; index++) {
        if (produtos[index].id > 0) {
            existeProduto = 1;
            printf("********************************************************************************\n");
            printf("* Id: %d\n", produtos[index].id);
            printf("* Nome: %s\n", produtos[index].nome);
            printf("* Preco: %2.f\n", produtos[index].custo);
            printf("* Quantidade: %d\n", produtos[index].quantidade);
        }
        if (index + 1 == NUM_MAX_PRODUTOS) {
            if (existeProduto == 0) {
                printf("* Nao existe qualquer registo de produtos.\n");
            }
            else {
                printf("********************************************************************************\n");
            }
        }
    }
}

void carregarProdutos(Produto produtos[]) {
    lerFicheiro(produtos, sizeof(Produto), NUM_MAX_PRODUTOS, PATH_PRODUTOS);
}

void converterCharParaProdutos(char charProdutos[], Produto produtos[]) {
    int contadorProdutos = 0, contadorCamposProduto = 1;
    char *splitProdutos = strtok(charProdutos, ";");

    while (splitProdutos != NULL) {
        converterCharParaCampoProduto(contadorCamposProduto, contadorProdutos, produtos, splitProdutos);
        contadorCamposProduto++;

        if (contadorCamposProduto > NUM_CAMPOS_STRUCT_PRODUTO) {
            contadorCamposProduto = 1;
            contadorProdutos++;
        }
		splitProdutos = strtok(NULL, ";");
	}
}

void converterCharParaCampoProduto(int contadorCamposProduto, int contadorProdutos, Produto produtos[], char splitProdutos[]) {
    switch (contadorCamposProduto) {
        case 1:
            produtos[contadorProdutos].id = (atoi)(splitProdutos);
            break;
        case 2:
            strcpy(produtos[contadorProdutos].nome, splitProdutos);
            break;
        case 3:
            produtos[contadorProdutos].custo = atoi(splitProdutos);
            break;
        case 4:
            produtos[contadorProdutos].quantidade = (atoi)(splitProdutos);
            break;
    }
}

void carregarDetalhesTransacao(DetalheTransacao detalhesTransacao[]) {
    lerFicheiro(detalhesTransacao, sizeof(DetalheTransacao), NUM_MAX_DETALHES_TRANSACAO, PATH_DETALHES_TRANSACAO);
}

// ################# CÓDIGO DA SEGUNDA FASE #################
