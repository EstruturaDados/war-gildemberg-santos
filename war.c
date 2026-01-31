// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#define NUM_TERRITORIOS 6
#define NUM_MISSOES 3
#define TAM_MAX_NOME 32
#define TAM_MAX_MISSAO 128

typedef struct {
    char nome[TAM_MAX_NOME];
    char cor[16];
    int tropas;
} Territorio;

Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio* mapa);
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int idMissao);
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* mapa, int origem, int destino, const char* corJogador);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador);
void limparBufferEntrada();

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand((unsigned)time(NULL));

    Territorio* mapa = alocarMapa();
    if (!mapa) {
        printf("Erro ao alocar memória para o mapa!\n");
        return 1;
    }
    inicializarTerritorios(mapa);

    char corJogador[16] = "Vermelho";
    int idMissao = sortearMissao();
    int venceu = 0;
    int opcao;

    do {
        exibirMapa(mapa);
        exibirMissao(idMissao);
        exibirMenuPrincipal();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBufferEntrada();
        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                venceu = verificarVitoria(mapa, idMissao, corJogador);
                if (venceu)
                    printf("\nParabéns! Você cumpriu sua missão!\n");
                else
                    printf("\nMissão ainda não cumprida. Continue jogando!\n");
                break;
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
        if (!venceu && opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            getchar();
        }
    } while (opcao != 0 && !venceu);

    liberarMemoria(mapa);
    return 0;
}

Territorio* alocarMapa() {
    Territorio* mapa = (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
    return mapa;
    free(mapa);
}

void inicializarTerritorios(Territorio* mapa) {
    const char* nomes[NUM_TERRITORIOS] = {
        "Território A", "Território B", "Território C",
        "Território D", "Território E", "Território F"
    };
    const char* cores[NUM_TERRITORIOS] = {
        "Vermelho", "Azul", "Verde",
        "Amarelo", "Preto", "Branco"
    };
    int tropasIniciais[NUM_TERRITORIOS] = {5, 3, 4, 2, 6, 1};

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strncpy(mapa[i].nome, nomes[i], TAM_MAX_NOME);
        strncpy(mapa[i].cor, cores[i], 16);
        mapa[i].tropas = tropasIniciais[i];
    }
}

void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

void exibirMenuPrincipal() {
    printf("\n--- Menu Principal ---\n");
    printf("1. Atacar\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair\n");
}

void exibirMapa(const Territorio* mapa) {
    printf("\n--- Mapa Atual ---\n");
    printf("%-3s %-15s %-10s %-10s\n", "ID", "Território", "Cor", "Tropas");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-3d %-15s %-10s %-10d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

void exibirMissao(int idMissao) {
    const char* missoes[NUM_MISSOES] = {
        "Destruir o exército Azul.",
        "Conquistar 3 territórios.",
        "Destruir o exército Verde."
    };
    printf("\n--- Missão Atual ---\n");
    if (idMissao >= 0 && idMissao < NUM_MISSOES) {
        printf("%s\n", missoes[idMissao]);
    } else {
        printf("Missão inválida.\n");
    }
}

void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    int origem, destino;
    origem = 0;
    printf("\n--- Fase de Ataque ---\n");
    printf("Território de origem %-10s\n", mapa[origem].cor);
    printf("Escolha o território de destino (1-%d): ", NUM_TERRITORIOS - 1);
    scanf("%d", &destino);
    limparBufferEntrada();

    if (origem < 0 || origem >= NUM_TERRITORIOS || destino < 0 || destino >= NUM_TERRITORIOS) {
        printf("Território inválido!\n");
        return;
    }
    simularAtaque(mapa, origem, destino, corJogador);
}

void simularAtaque(Territorio* mapa, int origem, int destino, const char* corJogador) {
    if (strcmp(mapa[origem].cor, corJogador) != 0) {
        printf("Você só pode atacar de territórios que possui!\n");
        return;
    }
    if (strcmp(mapa[destino].cor, corJogador) == 0) {
        printf("Você não pode atacar seus próprios territórios!\n");
        return;
    }
    if (mapa[origem].tropas < 2) {
        printf("Território de origem não tem tropas suficientes para atacar!\n");
        return;
    }

    int ataqueDado = rand() % 6 + 1;
    int defesaDado = rand() % 6 + 1;

    printf("Dado de Ataque: %d | Dado de Defesa: %d\n", ataqueDado, defesaDado);

    if (ataqueDado > defesaDado) {
        mapa[destino].tropas--;
        printf("Ataque bem-sucedido! Tropas defensoras restantes: %d\n", mapa[destino].tropas);
        if (mapa[destino].tropas == 0) {
            printf("Território conquistado!\n");
            strncpy(mapa[destino].cor, corJogador, 16);
            mapa[destino].tropas = 1;
            mapa[origem].tropas--;
        }
    } else {
        mapa[origem].tropas--;
        printf("Ataque falhou! Tropas atacantes restantes: %d\n", mapa[origem].tropas);
    }
}

int sortearMissao() {
    return rand() % NUM_MISSOES;
}

int verificarVitoria(const Territorio* mapa, int idMissao, const char* corJogador) {
    int countTerritorios = 0;
    switch (idMissao) {
        case 0: // INFO: Destruir o exército Azul
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, "Azul") == 0) {
                    return 0; // Ainda há territórios Azuis
                }
            }
            return 1; // INFO: Todos os territórios Azuis destruídos
        case 1: // INFO: Conquistar 3 territórios
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0) {
                    countTerritorios++;
                }
            }
            return (countTerritorios >= 3) ? 1 : 0;
        case 2: // INFO: Destruir o exército Verde
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, "Verde") == 0) {
                    return 0; // Ainda há territórios Verdes
                }
            }
            return 1; // INFO: Todos os territórios Verdes destruídos
        default:
            return 0; // INFO: Missão inválida
    }
}

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
