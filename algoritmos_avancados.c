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
// PROJETO: Detective Quest - Nivel Mestre.
// Autor: Fabricio Vieira de Souza
// Data: 30/11/2025
// Objetivo: Implementar sistema de investigação da mansão com:
// - Árvore binária de salas para exploração.
// - BST para armazenamento das pistas coletadas.
// - Tabela hash para associação pista → suspeito.
// - Exploração interativa e julgamento final do jogador.
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 10

// ----------------------- ÁRVORE DE SALAS -----------------------
typedef struct Sala
{
    char nome[50];
    char pista[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Cria dinamicamente um cômodo
Sala *criarSala(const char *nome, const char *pista)
{
    Sala *sala = (Sala *)malloc(sizeof(Sala));
    strcpy(sala->nome, nome);
    strcpy(sala->pista, pista);
    sala->esquerda = NULL;
    sala->direita = NULL;
    return sala;
}

// ----------------------- BST DE PISTAS -----------------------
typedef struct BST_Pista
{
    char pista[50];
    struct BST_Pista *esquerda;
    struct BST_Pista *direita;
} BST_Pista;

// Inserir pista na BST (mantém ordenação)
void inserirPista(BST_Pista **raiz, const char *pista)
{
    if (*raiz == NULL)
    {
        BST_Pista *novo = (BST_Pista *)malloc(sizeof(BST_Pista));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        *raiz = novo;
        return;
    }
    if (strcmp(pista, (*raiz)->pista) < 0)
        inserirPista(&(*raiz)->esquerda, pista);
    else if (strcmp(pista, (*raiz)->pista) > 0)
        inserirPista(&(*raiz)->direita, pista);
}

// Percorre BST in-order e imprime pistas
void mostrarPistas(BST_Pista *raiz)
{
    if (raiz == NULL)
        return;
    mostrarPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    mostrarPistas(raiz->direita);
}

// ----------------------- TABELA HASH -----------------------
typedef struct HashNodo
{
    char pista[50];
    char suspeito[50];
    struct HashNodo *proximo;
} HashNodo;

HashNodo *tabela_hash[TAMANHO_HASH];

// Função hash simples: soma ASCII % TAMANHO_HASH
int funcaoHash(const char *chave)
{
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++)
        soma += chave[i];
    return soma % TAMANHO_HASH;
}

// Insere pista → suspeito
void inserirNaHash(const char *pista, const char *suspeito)
{
    int indice = funcaoHash(pista);
    HashNodo *novo = (HashNodo *)malloc(sizeof(HashNodo));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabela_hash[indice];
    tabela_hash[indice] = novo;
}

// Encontra suspeito associado a uma pista
char *encontrarSuspeito(const char *pista)
{
    int indice = funcaoHash(pista);
    HashNodo *atual = tabela_hash[indice];
    while (atual != NULL)
    {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->proximo;
    }
    return NULL;
}

// Conta quantas pistas apontam para um suspeito
int contarPistasPorSuspeito(HashNodo *tabela[], const char *suspeito)
{
    int contador = 0;
    for (int i = 0; i < TAMANHO_HASH; i++)
    {
        HashNodo *atual = tabela[i];
        while (atual != NULL)
        {
            if (strcmp(atual->suspeito, suspeito) == 0)
                contador++;
            atual = atual->proximo;
        }
    }
    return contador;
}

// Determina o suspeito mais citado
char *suspeitoMaisCitado(HashNodo *tabela[])
{
    char *suspeitos[] = {"Ana", "Bruno", "Carlos", "Marina"};
    int qtd = sizeof(suspeitos) / sizeof(suspeitos[0]);
    int max = 0;
    char *top = NULL;

    for (int i = 0; i < qtd; i++)
    {
        int c = contarPistasPorSuspeito(tabela, suspeitos[i]);
        if (c > max)
        {
            max = c;
            top = suspeitos[i];
        }
    }
    return top;
}

// ----------------------- EXPLORAÇÃO INTERATIVA -----------------------
void explorarSalas(Sala *sala, BST_Pista **raiz)
{
    if (!sala)
        return;
    printf("\nVocê entrou na sala: %s\n", sala->nome);
    if (strlen(sala->pista) > 0)
    {
        printf("Você encontrou a pista: %s\n", sala->pista);
        inserirPista(raiz, sala->pista);
    }

    printf("Escolha direção: esquerda (e), direita (d), sair (s): ");
    char opcao;
    scanf(" %c", &opcao);

    if (opcao == 'e')
        explorarSalas(sala->esquerda, raiz);
    else if (opcao == 'd')
        explorarSalas(sala->direita, raiz);
    else
        return;
}

// ----------------------- JULGAMENTO FINAL -----------------------
void verificarSuspeitoFinal(BST_Pista *raiz, HashNodo *tabela[], const char *acusado)
{
    if (!acusado)
        return;
    int cont = 0;

    // Conta pistas que apontam para o acusado
    for (int i = 0; i < TAMANHO_HASH; i++)
    {
        HashNodo *atual = tabela[i];
        while (atual)
        {
            if (strcmp(atual->suspeito, acusado) == 0)
                cont++;
            atual = atual->proximo;
        }
    }

    printf("\nNúmero de pistas que indicam %s: %d\n", acusado, cont);
    if (cont >= 2)
        printf("Acusação correta! O culpado é %s.\n", acusado);
    else
        printf("Acusação incorreta. %s não tem pistas suficientes.\n", acusado);
}

// ----------------------- MAIN -----------------------
int main()
{
    // Inicializa tabela hash
    for (int i = 0; i < TAMANHO_HASH; i++)
        tabela_hash[i] = NULL;

    // Cria mapa da mansão
    Sala *sala1 = criarSala("Entrada", "Pegadas de lama");
    Sala *sala2 = criarSala("Cozinha", "Faca com sangue");
    Sala *sala3 = criarSala("Biblioteca", "Livro caído");
    Sala *sala4 = criarSala("Jardim", "Fio de cabelo");
    Sala *sala5 = criarSala("Escritorio", "Bilhete rasgado");

    // Conecta salas
    sala1->esquerda = sala2;
    sala1->direita = sala3;
    sala2->esquerda = sala4;
    sala2->direita = sala5;

    // BST de pistas coletadas
    BST_Pista *raizPistas = NULL;

    // Associa pistas a suspeitos via hash
    inserirNaHash("Pegadas de lama", "Bruno");
    inserirNaHash("Faca com sangue", "Carlos");
    inserirNaHash("Livro caído", "Marina");
    inserirNaHash("Fio de cabelo", "Ana");
    inserirNaHash("Bilhete rasgado", "Carlos");

    // Exploração interativa
    explorarSalas(sala1, &raizPistas);

    // Mostra pistas coletadas
    printf("\nPistas coletadas:\n");
    mostrarPistas(raizPistas);

    // Mostra suspeito mais citado
    char *top = suspeitoMaisCitado(tabela_hash);
    printf("\nSuspeito mais citado pelas pistas: %s\n", top);

    // Jogador escolhe acusado
    char acusado[50];
    printf("Digite o suspeito que deseja acusar: ");
    scanf("%s", acusado);

    // Verificação do julgamento final
    verificarSuspeitoFinal(raizPistas, tabela_hash, acusado);

    return 0;
}
