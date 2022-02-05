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
#define OPCAO_MENU_ESCOLAS_IMPORTAR 11
#define OPCAO_MENU_UTILIZADORES_IMPORTAR 12

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

char *lerFicheiroDeTexto(char caminhoFicheiro[]);
void importarEscolas(Escola escolas[], int numEscolasRegistadas);
void converterCharParaEscolas(char charEscolas[], Escola escolas[]);
void converterCharParaCampoEscola(int contadorCamposEscola, int contadorEscolas, Escola escolas[], char splitEscolas[]);
void avisoImportacao(char* tipoImportacao, int limiteImportacao);
int existAlgumaEscola(int idEscola, Escola escolas[]);

// ################# CÓDIGO DA SEGUNDA FASE #################

void main()
{
    Escola escolas[NUM_MAX_ESCOLAS];
    Utilizador utilizadores[NUM_MAX_UTILIZADORES];
    Transacao transacoes[NUM_MAX_TRANSACOES];

    inicializarArrays(escolas, utilizadores, transacoes);
    carregarTodosDados(escolas, utilizadores, transacoes);

    int opcaoMenu = 0;

    do {
        int numEscolasRegistadas = obterNumeroEscolasRegistadas(escolas);
        int numUtilizadoresRegistados = obterNumeroUtilizadoresRegistados(utilizadores);
        int numTransacoesRegistadas = obterNumeroTransacoesRegistadas(transacoes);

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

            case OPCAO_MENU_UTILIZADORES_REGISTAR:
                realizarRegistoUtilizador(numUtilizadoresRegistados, numEscolasRegistadas, escolas, utilizadores);
                break;

            // ################# CÓDIGO DA SEGUNDA FASE #################
            case OPCAO_MENU_ESCOLAS_IMPORTAR:
                system("cls");
                importarEscolas(escolas, numEscolasRegistadas);
                break;
            // ################# CÓDIGO DA SEGUNDA FASE #################

            case OPCAO_MENU_UTILIZADORES_CONSULTAR:
                mostrarUtilizadores(utilizadores);
                break;

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
                guardarTodosDados(escolas, utilizadores, transacoes);
                break;

            case OPCAO_MENU_SAIR:
            default:
                break;
        }
    } while (opcaoMenu != OPCAO_MENU_SAIR);

    guardarTodosDados(escolas, utilizadores, transacoes);
}

void lerString(char *valor, int tamanho) {
    fflush(stdin);
    fgets(valor, tamanho, stdin);
    valor[strcspn(valor, "\n")] = 0;
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
        strcpy(transacoes[index].data, "");
        strcpy(transacoes[index].hora, "");
    }
}

int menu_opcoes() {
    int menuSelecionado = 0;

    printf("******************************** MENU PRINCIPAL ********************************\n");
    menu_escolas();
    menu_utilizadores();
    menu_transacoes();
    menu_estatisticas();
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
void carregarTodosDados(Escola escolas[], Utilizador utilizadores[], Transacao transacoes[]) {
    carregarEscolas(escolas);
    carregarUtilizadores(utilizadores);
    carregarTransacoes(transacoes);
}

// Função para guardar todos os dados
// Escolas - Utilizadores - Transações
void guardarTodosDados(Escola escolas[], Utilizador utilizadores[], Transacao transacoes[]) {
    system("cls");
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
    printf("** Todos os dados que conterem ids ja existentes na aplicacao tambem serao ignorados");
    printf("\n********************************************************************************\n");
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
            if (escolasImportadas[indexEscolasImportadas].id > 0 && escolas[indexEscolasAtuais].id == 0 && !existAlgumaEscola(escolas[indexEscolasImportadas].id, escolas)) {
                if (numEscolasRegistadas < NUM_MAX_ESCOLAS) {
                    numEscolasImportadas++;
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

// ################# CÓDIGO DA SEGUNDA FASE #################
