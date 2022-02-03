#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_ESCOLAS 5
#define NUM_UTILIZADORES 200
#define NUM_TRANSACOES 5000

#define TIPO_UTILIZADOR_ESTUDANTE 1
#define TIPO_UTILIZADOR_DOCENTE 2
#define TIPO_UTILIZADOR_FUNCIONARIO 3

#define OPCAO_MENU_SAIR 0
#define OPCAO_MENU_ESCOLAS_REGISTAR 1
#define OPCAO_MENU_ESCOLAS_CONSULTAR 2
#define OPCAO_MENU_ESCOLAS_IMPORTAR 3

#define OPCAO_MENU_UTILIZADORES_REGISTAR 4
#define OPCAO_MENU_UTILIZADORES_CONSULTAR 5
#define OPCAO_MENU_UTILIZADORES_IMPORTAR 6

#define OPCAO_MENU_ESTATISTICAS_TOTAL_FATURADO_ESCOLA 11
#define OPCAO_MENU_ESTATISTICAS_PERCENTAGEM_PAGAMENTOS 12
#define OPCAO_MENU_ESTATISTICAS_TOTAL_TRANSACOES_HORIZONTE_TEMPORAL 13

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

int menu_opcoes();
void menu_escolas();
void menu_utilizadores();

void carregarTodosDados();
void guardarTodosDados();
void inicializarArrays();

// Todas as funções relacionadas com escolas.
void inicializarArrayEscolas();
Escola registarEscola(int proximoId);
void carregarEscolas();
int obterNumeroEscolasRegistadas(Escola escolas[]);
char *converterEscolasParaChar(Escola escolas[]);
void converterCharParaEscolas(char charEscolas[], Escola escolas[]);
void converterCharParaCampoEscola(int contadorCamposEscola, int contadorEscolas, Escola escolas[], char splitEscolas[]);
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
Transacao RegistarTransacao();
Transacao ConsultarTransacao();

void ConsultarTotalFaturaEscola(); // apresentado no menu principal
void PercentagemTransacoes();      // pagamentos por escola
void TotalTransacao();             // pagamentos entre duas datas por tipo de utilizador

void main()
{
    Escola escolas[NUM_ESCOLAS];
    Utilizador utilizadores[NUM_UTILIZADORES];

    inicializarArrays(escolas, utilizadores);

    carregarTodosDados(escolas);

    int numEscolasRegistadas = obterNumeroEscolasRegistadas(escolas);
    int numUtilizadoresRegistados = obterNumeroUtilizadoresRegistados(utilizadores);

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
            case OPCAO_MENU_ESCOLAS_IMPORTAR:
                carregarEscolas(escolas);
                break;
            case OPCAO_MENU_UTILIZADORES_REGISTAR:
                utilizadores[numUtilizadoresRegistados] = registarUtilizador(numUtilizadoresRegistados, escolas);
                break;
            case OPCAO_MENU_UTILIZADORES_CONSULTAR:
                mostrarUtilizadores(utilizadores);
                break;
            case OPCAO_MENU_SAIR:
            default:
                break;
        }
    } while (opcaoMenu != OPCAO_MENU_SAIR);

    guardarTodosDados(escolas);
}

// Vai inicializar o array das escolas e garantir que todos os id's comecem a 0.
// Se o id tiver a 0 significa que a posição do array ainda não foi preenchida.
void inicializarArrays(Escola escolas[], Utilizador utilizadores[]) {
    inicializarArrayEscolas(escolas);
    inicializarArrayUtilizadores(utilizadores);
}

void inicializarArrayEscolas(Escola escolas[]) {
    for (int index = 0; index < NUM_ESCOLAS; index++) {
        escolas[index].id = 0;
        strcpy(escolas[index].nome, "");
        strcpy(escolas[index].abreviacao, "");
        strcpy(escolas[index].localidade, "");
        escolas[index].campus = 0;
    }
}

void inicializarArrayUtilizadores(Utilizador utilizadores[]) {
    for (int index = 0; index < NUM_UTILIZADORES; index++) {
        utilizadores[index].id = 0;
        utilizadores[index].idEscola = 0;
        utilizadores[index].nif = 0;
        utilizadores[index].tipoUtilizador = 0;
        strcpy(utilizadores[index].email, "");
        strcpy(utilizadores[index].nome, "");
        utilizadores[index].saldo = 0;
    }
}

int menu_opcoes() {
    int menuSelecionado = 0;

    printf("******************************** MENU PRINCIPAL ********************************\n");
    menu_escolas();
    menu_utilizadores();
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
    printf("* [%d] * Importar Escolas   *\n", OPCAO_MENU_ESCOLAS_IMPORTAR);
}

void menu_utilizadores() {
    printf("******* Utilizadores: ********\n");
    printf("* [%d] * Registar Utilizador   *\n", OPCAO_MENU_UTILIZADORES_REGISTAR);
    printf("* [%d] * Consultar Utilizador   *\n", OPCAO_MENU_UTILIZADORES_CONSULTAR);
    printf("* [%d] * Importar Utilizador   *\n", OPCAO_MENU_UTILIZADORES_IMPORTAR);
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
    // char *dadosEscolas = lerDadosFicheiro(PATH_ESCOLAS);
    // converterCharParaEscolas(dadosEscolas, escolas);
}

void mostrarEscolas(Escola escolas[])
{
    system("cls");
    for (int index = 0; index < NUM_ESCOLAS; index++)
    {
        if (escolas[index].id > 0) {
            printf("********************************************************************************\n");
            printf("* Id: %d\n", escolas[index].id);
            printf("* Abreviacao: %s\n", escolas[index].abreviacao);
            printf("* Nome: %s\n", escolas[index].nome);
            printf("* Campus: %d\n", escolas[index].campus);
            printf("* Localidade: %s\n", escolas[index].localidade);
        }

        if (index + 1 == NUM_ESCOLAS)
        {
            printf("********************************************************************************\n");
        }
    }
}

int obterNumeroEscolasRegistadas(Escola escolas[]) {
    int contador = 0;
    for (int index = 0; index < NUM_ESCOLAS; index++) {
        if (escolas[index].id > 0) {
            contador++;
        }
    }
    return contador;
}

// Prepara os valores das escolas para depois serem guardados num ficheiro.
char *converterEscolasParaChar(Escola escolas[])
{
    char *charEscolas = malloc(1024);
    for (int index = 0; index < NUM_ESCOLAS; index++)
    {
        if (escolas[index].id > 0) {
            char charEscola[200];
            // Junta todas as variáveis da escolas[index] na variável charEscola.
            sprintf(charEscola, "%d;%s;%s;%d;%s;", escolas[index].id, escolas[index].nome, escolas[index].abreviacao, escolas[index].campus, escolas[index].localidade);

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
    }
    return charEscolas;
}

// Vai retornar o número de escolas existentes.
void converterCharParaEscolas(char charEscolas[], Escola escolas[]) {
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
    for (int index = 0; index < NUM_ESCOLAS; index++) {
        if (escolas[index].id > 0) {
            printf("** Id: [%d] - Nome: %s\n", escolas[index].id, escolas[index].nome);
        }
    }
}

void mostrarUtilizadores(Utilizador utilizadores[]) {
    system("cls");
    for (int index = 0; index < NUM_UTILIZADORES; index++) {
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
    for (int index = 0; index < NUM_UTILIZADORES; index++) {
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

// Função para carregar todos os dados quando o programa é aberto
// Escolas - Utilizadores - Transações - Etc
void carregarTodosDados(Escola escolas[]) {
    carregarEscolas(escolas);
}

// Função para guardar todos os dados
// Escolas - Utilizadores - Transações
void guardarTodosDados(Escola escolas[]) {
    // Escolas
}
