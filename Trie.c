#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Trie.h"

TrieNode* criar_no() {
    TrieNode *no = malloc(sizeof(TrieNode));
    no->palavra = NULL;
    int i;
    for (i = 0; i < DIGITOS; i++) {
        no->filhos[i] = NULL;
    }
    return no;
}

void liberar_trie(TrieNode *raiz) {
    if (!raiz) return;
    int i;
    for (i = 0; i < DIGITOS; i++) {
        liberar_trie(raiz->filhos[i]);
    }
    if (raiz->palavra) free(raiz->palavra);
    free(raiz);
}

char letra_para_digito(char c) {
    c = toupper(c);
    if (c >= 'A' && c <= 'C') return '2';
    if (c >= 'D' && c <= 'F') return '3';
    if (c >= 'G' && c <= 'I') return '4';
    if (c >= 'J' && c <= 'L') return '5';
    if (c >= 'M' && c <= 'O') return '6';
    if (c >= 'P' && c <= 'S') return '7';
    if (c >= 'T' && c <= 'V') return '8';
    if (c >= 'W' && c <= 'Z') return '9';
    return '\0';
}

void palavra_pra_codigo(const char *palavra, char *codigo) {
    int i;
    for (i = 0; palavra[i] != '\0'; i++) {
        codigo[i] = letra_para_digito(palavra[i]);
    }
    codigo[i] = '\0';
}

void inserir_palavra(TrieNode *raiz, const char *codigo, const char *palavra) {
    TrieNode *atual = raiz;
    int i = 0;

    while (codigo[i] != '\0') {
        int idx = codigo[i] - '0';
        if (!atual->filhos[idx])
            atual->filhos[idx] = criar_no();
        atual = atual->filhos[idx];
        i++;
    }

    // Ramifica usando 0 se já houver uma palavra
    while (atual->palavra != NULL) {
        if (!atual->filhos[0])
            atual->filhos[0] = criar_no();
        atual = atual->filhos[0];
    }

	// Duplica a palavra para armazena
    atual->palavra = strdup(palavra);
}

void traduzir_codigo(TrieNode *raiz, const char *codigo) {
    TrieNode *atual = raiz;
    int i = 0;

    while (codigo[i] != '\0') {
        int idx = codigo[i] - '0';
        if (!atual->filhos[idx]) {
            printf("Nenhuma palavra encontrada.\n");
            return;
        }
        atual = atual->filhos[idx];
        i++;
    }

    // Se termina em 0 ? imprime todas a partir dali
    if (codigo[i - 1] == '0') {
        while (atual) {
            if (atual->palavra)
                printf("? %s\n", atual->palavra);
            atual = atual->filhos[0];
        }
    } else {
        if (atual->palavra)
            printf(" %s\n", atual->palavra);
        else
            printf("Nenhuma palavra encontrada.\n");
    }
    
    void _imprimir_recursivo(TrieNode *node) {
    if (!node) return;

    // Se este nó contém uma palavra, imprima-a
    if (node->palavra) {
        printf("- %s\n", node->palavra);
    }

    // Percorre todos os filhos (dígitos 0-9)
    int i;
    for (i = 0; i < DIGITOS; i++) {
        _imprimir_recursivo(node->filhos[i]);
    }
}
}

void _imprimir_recursivo(TrieNode *node) {
    if (!node) return;

    // Se este nó contém uma palavra, imprima-a
    if (node->palavra) {
        printf("- %s\n", node->palavra);
    }

    // Percorre todos os filhos (dígitos 0-9)
    int i;
    for (i = 0; i < DIGITOS; i++) {
        _imprimir_recursivo(node->filhos[i]);
    }
}

void imprimir_palavras(TrieNode *raiz) {
    printf("\n--- Todas as Palavras na Trie ---\n");
    if (!raiz) {
        printf("A Trie está vazia.\n");
        return;
    }
    _imprimir_recursivo(raiz);
    printf("-------------------------------\n");
}


bool procurar_palavra(TrieNode *raiz, const char *palavra) {
    if (!raiz || !palavra || *palavra == '\0') {
        return false;
    }

    char codigo[strlen(palavra) + 1];
    palavra_pra_codigo(palavra, codigo);

    TrieNode *atual = raiz;
    int i = 0;

    while (codigo[i] != '\0') {
        int idx = codigo[i] - '0';
        if (idx < 0 || idx >= DIGITOS || !atual->filhos[idx]) {
            return false; // Caminho não existe
        }
        atual = atual->filhos[idx];
        i++;
    }

    // Chegou ao nó correspondente ao código. Agora, procure a palavra específica.
    // É necessário percorrer as ramificações '0' se houver colisões de código.
    while (atual) {
        if (atual->palavra && strcmp(atual->palavra, palavra) == 0) {
            return true; // Palavra encontrada!
        }
        atual = atual->filhos[0]; // Tenta na próxima colisão
    }

    return false; // A palavra não foi encontrada no caminho ou nas colisões
}

bool _remover_recursivo(TrieNode *node, const char *codigo, const char *palavra, int profundidade) {
    if (!node) return false;

    // Se chegamos ao fim do código
    if (profundidade == strlen(codigo)) {
        // Encontramos o nó que deveria conter a palavra.
        // Agora precisamos encontrar a palavra específica se houver colisões (ramificações '0').
        TrieNode *prev = NULL; // Ponteiro para o nó anterior no caminho '0'
        TrieNode *curr = node; // Ponteiro para o nó atual no caminho '0'

        while (curr) {
            if (curr->palavra && strcmp(curr->palavra, palavra) == 0) {
                // Palavra encontrada!
                free(curr->palavra); // Libera a memória da palavra
                curr->palavra = NULL; // Marca como não tendo palavra

                // Se este nó não tem filhos (exceto o 0 que estamos limpando)
                // e não tem mais palavras, pode ser que possamos liberar este nó.
                bool tem_filhos = false;
                int i;
                for (i = 0; i < DIGITOS; i++) {
                    if (curr->filhos[i] != NULL && i != 0) { // Ignora o filho 0 por enquanto
                        tem_filhos = true;
                        break;
                    }
                }

                // Se o nó removido não tinha filhos (além do '0' atual)
                // e não tinha outras palavras, e não é a raiz, ele pode ser liberado.
                // A liberação do nó '0' filho será tratada se ele não tiver mais utilidade.
                
                // Se o nó atual (curr) tem um filho 0, e nós estamos removendo a palavra dele,
                // precisamos conectar o prev (se existir) ao filho 0 de curr, e liberar curr.
                if (prev) { // Se não estamos no nó original (há ramificação '0')
                    prev->filhos[0] = curr->filhos[0]; // Reencadeia
                    free(curr); // Libera o nó onde a palavra estava
                    return false; // Já liberamos um nó, não há mais para liberar recursivamente aqui
                } else { // Se estamos no nó original (profundidade == strlen(codigo))
                    // Verifica se o nó pode ser deletado. Se ele tiver um filho 0,
                    // e a palavra foi removida, mas ainda há um filho 0, não devemos liberar este nó,
                    // mas o filho 0 pode precisar ser reencadeado.
                    // Isso é complexo. A simplificação é: se não tem filhos E não tem palavra, pode liberar.
                    bool pode_deletar_curr = true;
                    if (curr->filhos[0] != NULL) pode_deletar_curr = false; // Se tem filho 0, não pode deletar
                    int i;
					for(i = 1; i < DIGITOS; i++){ // Se tem outros filhos, não pode deletar
                        if(curr->filhos[i] != NULL) pode_deletar_curr = false;
                    }
                    
                    if (pode_deletar_curr) return true; // Indica que o nó pode ser liberado pelo pai
                    return false; // Não pode ser liberado agora, mas a palavra foi removida
                }
            }
            prev = curr;
            curr = curr->filhos[0]; // Move para a próxima colisão
        }
        return false; // Palavra não encontrada no caminho de colisões
    }

    // Se não chegamos ao fim do código, continue a travessia
    int idx = codigo[profundidade] - '0';
    if (idx < 0 || idx >= DIGITOS || !node->filhos[idx]) {
        return false; // Caminho não existe, palavra não está na Trie
    }

    // Chamada recursiva para o próximo nó
    bool deve_deletar_filho = _remover_recursivo(node->filhos[idx], codigo, palavra, profundidade + 1);

    // Pós-ordem: verifica se o filho pode ser deletado
    if (deve_deletar_filho) {
        free(node->filhos[idx]);
        node->filhos[idx] = NULL;

        // Verifica se o nó atual (pai) pode ser deletado:
        // Se ele não tem palavra e não tem mais filhos (incluindo o filho 0)
        bool tem_filhos_restantes = false;
        int i;
		for (i = 0; i < DIGITOS; i++) {
            if (node->filhos[i] != NULL) {
                tem_filhos_restantes = true;
                break;
            }
        }
        // Se o nó atual não tem palavra e não tem mais filhos, ele também pode ser deletado
        return !node->palavra && !tem_filhos_restantes;
    }

    return false; // O nó atual não pode ser deletado
}


/**@return true se a palavra foi encontrada e removida, false caso contrário.*/
bool remover_palavra(TrieNode *raiz, const char *palavra) {
    if (!raiz || !palavra || *palavra == '\0') {
        return false;
    }

    char codigo[strlen(palavra) + 1];
    palavra_pra_codigo(palavra, codigo);

    // Primeiro, verifica se a palavra existe para evitar travessias desnecessárias
    if (!procurar_palavra(raiz, palavra)) {
        return false; // A palavra não está na Trie, nada a remover
    }

    // Chama a função recursiva de remoção
    _remover_recursivo(raiz, codigo, palavra, 0);

    // Se a palavra foi removida, procurar_palavra deve retornar falso agora
    return !procurar_palavra(raiz, palavra);
}



