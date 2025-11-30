/*#include <stdio.h>

// Desafio Detective Quest
// Tema 4 - Árvores e Tabela Hash
// Este código inicial serve como base para o desenvolvimento das estruturas de navegação, pistas e suspeitos.
// Use as instruções de cada região para desenvolver o sistema completo com árvore binária, árvore de busca e tabela hash.

int main() {

    // 🌱 Nível Novato: Mapa da Mansão com Árvore Binária
    //
    // - Crie uma struct Sala com nome, e dois ponteiros: esquerda e direita.
    // - Use funções como criarSala(), conectarSalas() e explorarSalas().
    // - A árvore pode ser fixa: Hall de Entrada, Biblioteca, Cozinha, Sótão etc.
    // - O jogador deve poder explorar indo à esquerda (e) ou à direita (d).
    // - Finalize a exploração com uma opção de saída (s).
    // - Exiba o nome da sala a cada movimento.
    // - Use recursão ou laços para caminhar pela árvore.
    // - Nenhuma inserção dinâmica é necessária neste nível.

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}
*/
// ============================================================================
// PROJETO: Detective Quest - Nivel Aventureiro
// Autor: Fabricio Vieira de Souza
// Data: 29/11/2025
// Objetivo: Navegacao da mansao, coleta de pistas e armazenamento ordenado
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOME 50
#define MAX_PISTA 100

/* =========================
   Estruturas de dados
   ========================= */

// No da arvore binaria que representa uma sala da mansao
typedef struct Sala
{
    char nome[MAX_NOME];
    char pista[MAX_PISTA];
    bool pistaColetada;
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// No da BST que armazena pistas coletadas
typedef struct PistaNode
{
    char pista[MAX_PISTA];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

/* =========================
   Funcoes de criacao e liberacao
   ========================= */

// cria dinamicamente uma sala com nome e pista (pista pode ser "")
Sala *criarSala(const char *nome, const char *pista)
{
    Sala *nova = (Sala *)malloc(sizeof(Sala));
    if (nova == NULL)
    {
        printf("Erro ao alocar memoria para Sala!\n");
        exit(1);
    }
    strncpy(nova->nome, nome, MAX_NOME - 1);
    nova->nome[MAX_NOME - 1] = '\0';
    if (pista != NULL)
    {
        strncpy(nova->pista, pista, MAX_PISTA - 1);
        nova->pista[MAX_PISTA - 1] = '\0';
    }
    else
    {
        nova->pista[0] = '\0';
    }
    nova->pistaColetada = false;
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// cria no de BST para pista
PistaNode *criarPistaNode(const char *pista)
{
    PistaNode *novo = (PistaNode *)malloc(sizeof(PistaNode));
    if (novo == NULL)
    {
        printf("Erro ao alocar memoria para PistaNode!\n");
        exit(1);
    }
    strncpy(novo->pista, pista, MAX_PISTA - 1);
    novo->pista[MAX_PISTA - 1] = '\0';
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// libera BST de pistas
void liberarPistas(PistaNode *raiz)
{
    if (raiz == NULL)
        return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// libera arvore de salas
void liberarSalas(Sala *raiz)
{
    if (raiz == NULL)
        return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}

/* =========================
   Funcoes de BST (pistas)
   ========================= */

// busca exata na BST (retorna true se ja existe)
bool buscarPistaBST(PistaNode *raiz, const char *chave)
{
    if (raiz == NULL)
        return false;
    int cmp = strcmp(chave, raiz->pista);
    if (cmp == 0)
        return true;
    if (cmp < 0)
        return buscarPistaBST(raiz->esquerda, chave);
    else
        return buscarPistaBST(raiz->direita, chave);
}

// insere pista na BST (mantem ordem alfabetica); evita duplicatas
PistaNode *inserirPistaBST(PistaNode *raiz, const char *pista)
{
    if (pista == NULL || pista[0] == '\0')
        return raiz;
    if (raiz == NULL)
        return criarPistaNode(pista);
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0)
        return raiz;
    else if (cmp < 0)
        raiz->esquerda = inserirPistaBST(raiz->esquerda, pista);
    else
        raiz->direita = inserirPistaBST(raiz->direita, pista);
    return raiz;
}

// imprime pistas em ordem alfabetica (em-ordem)
void exibirPistasEmOrdem(PistaNode *raiz)
{
    if (raiz == NULL)
        return;
    exibirPistasEmOrdem(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistasEmOrdem(raiz->direita);
}

/* =========================
   Funcao de exploracao interativa
   ========================= */
void explorarSalasComPistas(Sala *atual, PistaNode **raizPistas)
{
    if (atual == NULL)
        return;

    char escolha;
    while (atual != NULL)
    {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (atual->pista[0] != '\0' && !atual->pistaColetada)
        {
            printf("Voce encontrou uma pista: \"%s\"\n", atual->pista);
            *raizPistas = inserirPistaBST(*raizPistas, atual->pista);
            atual->pistaColetada = true;
        }
        else if (atual->pista[0] != '\0' && atual->pistaColetada)
        {
            printf("Pista ja coletada neste comodo.\n");
        }
        else
        {
            printf("Nenhuma pista neste comodo.\n");
        }

        printf("\nEscolha seu caminho:\n");
        if (atual->esquerda != NULL)
            printf(" [e] Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf(" [d] Ir para a direita (%s)\n", atual->direita->nome);
        printf(" [s] Sair da exploracao e exibir pistas coletadas\n");
        printf("-> ");

        if (scanf(" %c", &escolha) != 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Entrada invalida. Tente novamente.\n");
            continue;
        }

        if (escolha == 'e' || escolha == 'E')
        {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Nao ha caminho a esquerda.\n");
        }
        else if (escolha == 'd' || escolha == 'D')
        {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Nao ha caminho a direita.\n");
        }
        else if (escolha == 's' || escolha == 'S')
        {
            printf("Exploracao encerrada pelo jogador.\n");
            return;
        }
        else
        {
            printf("Opcao invalida! Use 'e', 'd' ou 's'.\n");
        }
    }
}

/* =========================
   Funcao principal
   ========================= */
int main()
{
    // --- Montagem fixa do mapa da mansao ---
    Sala *hall = criarSala("Hall de Entrada", "Cartao rasgado");
    Sala *salaEstar = criarSala("Sala de Estar", "Manual antigo rasgado");
    Sala *biblioteca = criarSala("Biblioteca", "Livro com pagina faltando");
    Sala *cozinha = criarSala("Cozinha", "Pegadas de lama");
    Sala *jardim = criarSala("Jardim", "Cabeca de fosforo usada");
    Sala *escritorio = criarSala("Escritorio", "Carta ameaçadora");
    Sala *quarto = criarSala("Quarto", "Lencol manchado");
    Sala *sotao = criarSala("Sotao", "Chave enferrujada");
    Sala *porao = criarSala("Porao", "Garrafa quebrada");
    Sala *corredor = criarSala("Corredor", "Bilhete dobrado");

    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    biblioteca->esquerda = escritorio;
    biblioteca->direita = corredor;
    cozinha->esquerda = porao;
    jardim->esquerda = quarto;
    jardim->direita = sotao;

    // --- BST para pistas coletadas ---
    PistaNode *raizPistas = NULL;

    printf("=== Detective Quest - Nivel Aventureiro Atualizado ===\n");
    printf("Explore a mansao a partir do Hall de Entrada. Pistas encontradas serao coletadas automaticamente.\n");

    explorarSalasComPistas(hall, &raizPistas);

    printf("\n--- Pistas coletadas (ordem alfabetica) ---\n");
    if (raizPistas == NULL)
        printf("Nenhuma pista coletada.\n");
    else
        exibirPistasEmOrdem(raizPistas);

    liberarPistas(raizPistas);
    liberarSalas(hall);

    printf("\nEncerrando Detective Quest - bom trabalho, detetive!\n");
    return 0;
}