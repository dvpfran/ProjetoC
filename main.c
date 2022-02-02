#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NUM_ESCOLAS 5
#define NUM_UTilizadores 200
#define NUM_Transacoes 5000

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
Utilizador registarUtilizador();
Utilizador ConsultarUtilizador();

// Todas as funções relacionadas com transações.
Transacao RegistarTransacao();
Transacao ConsultarTransacao();

void ConsultarTotalFaturaEscola(); // apresentado no menu principal
void PercentagemTransacoes();      // pagamentos por escola
void TotalTransacao();             // pagamentos entre duas datas por tipo de utilizador

char *lerDadosFicheiro(char caminhoFicheiro[]);
void guardarDadosFicheiro(); // guardar em binário

void main()
{
    Escola escolas[NUM_ESCOLAS];
    inicializarArrayEscolas(escolas);

    carregarTodosDados(escolas);

    int escolasRegistadas = obterNumeroEscolasRegistadas(escolas);

    int opcaoMenu = 0;
    
    do {
        opcaoMenu = menu_opcoes();

        switch (opcaoMenu) {
            case OPCAO_MENU_ESCOLAS_REGISTAR:
                escolas[escolasRegistadas] = registarEscola(escolasRegistadas);
                break;

            case OPCAO_MENU_ESCOLAS_CONSULTAR:
                mostrarEscolas(escolas);
                break;

            case OPCAO_MENU_ESCOLAS_IMPORTAR:
                carregarEscolas(escolas);
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
void inicializarArrayEscolas(Escola escolas[]) {
    for (int index = 0; index < NUM_ESCOLAS; index++) {
        escolas[index].id = 0;
        strcpy(escolas[index].nome, "");
        strcpy(escolas[index].abreviacao, "");
        strcpy(escolas[index].localidade, "");
        escolas[index].campus = 0;
    }
}

int menu_opcoes() {
    int menuSelecionado = 0;

    printf("******************************** MENU PRINCIPAL ********************************\n");
    menu_escolas();
    menu_utilizadores();
    printf("********************************************************************************\n");
    
    scanf("%d", &menuSelecionado);
    return menuSelecionado;
}

void menu_escolas() {
    printf("******** Escolas: ********\n");
    printf("* %d - Registar Escolas   *\n", OPCAO_MENU_ESCOLAS_REGISTAR);
    printf("* %d - Consultar Escolas  *\n", OPCAO_MENU_ESCOLAS_CONSULTAR);
    printf("* %d - Importar Escolas   *\n", OPCAO_MENU_ESCOLAS_IMPORTAR);
    printf("* %d - Sair               *\n", OPCAO_MENU_SAIR);
    printf("* Selecionar menu: ");
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

// Função para carregar todos os dados quando o programa é aberto
// Escolas - Utilizadores - Transações - Etc
void carregarTodosDados(Escola escolas[]) {
    carregarEscolas(escolas);
}

void carregarEscolas(Escola escolas[]) {
    char *dadosEscolas = lerDadosFicheiro(PATH_ESCOLAS);
    converterCharParaEscolas(dadosEscolas, escolas);
}

// Função para guardar todos os dados
// Escolas - Utilizadores - Transações
void guardarTodosDados(Escola escolas[]) {
    // Escolas
    char *charEscola = converterEscolasParaChar(escolas);
    guardarDadosFicheiro(charEscola, PATH_ESCOLAS);
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
