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

    // Ramifica usando 0 se j� houver uma palavra
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

    // Se este n� cont�m uma palavra, imprima-a
    if (node->palavra) {
        printf("- %s\n", node->palavra);
    }

    // Percorre todos os filhos (d�gitos 0-9)
    int i;
    for (i = 0; i < DIGITOS; i++) {
        _imprimir_recursivo(node->filhos[i]);
    }
}
}

void _imprimir_recursivo(TrieNode *node) {
    if (!node) return;

    // Se este n� cont�m uma palavra, imprima-a
    if (node->palavra) {
        printf("- %s\n", node->palavra);
    }

    // Percorre todos os filhos (d�gitos 0-9)
    int i;
    for (i = 0; i < DIGITOS; i++) {
        _imprimir_recursivo(node->filhos[i]);
    }
}

void imprimir_palavras(TrieNode *raiz) {
    printf("\n--- Todas as Palavras na Trie ---\n");
    if (!raiz) {
        printf("A Trie est� vazia.\n");
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
            return false; // Caminho n�o existe
        }
        atual = atual->filhos[idx];
        i++;
    }

    // Chegou ao n� correspondente ao c�digo. Agora, procure a palavra espec�fica.
    // � necess�rio percorrer as ramifica��es '0' se houver colis�es de c�digo.
    while (atual) {
        if (atual->palavra && strcmp(atual->palavra, palavra) == 0) {
            return true; // Palavra encontrada!
        }
        atual = atual->filhos[0]; // Tenta na pr�xima colis�o
    }

    return false; // A palavra n�o foi encontrada no caminho ou nas colis�es
}

bool _remover_recursivo(TrieNode *node, const char *codigo, const char *palavra, int profundidade) {
    if (!node) return false;

    // Se chegamos ao fim do c�digo
    if (profundidade == strlen(codigo)) {
        // Encontramos o n� que deveria conter a palavra.
        // Agora precisamos encontrar a palavra espec�fica se houver colis�es (ramifica��es '0').
        TrieNode *prev = NULL; // Ponteiro para o n� anterior no caminho '0'
        TrieNode *curr = node; // Ponteiro para o n� atual no caminho '0'

        while (curr) {
            if (curr->palavra && strcmp(curr->palavra, palavra) == 0) {
                // Palavra encontrada!
                free(curr->palavra); // Libera a mem�ria da palavra
                curr->palavra = NULL; // Marca como n�o tendo palavra

                // Se este n� n�o tem filhos (exceto o 0 que estamos limpando)
                // e n�o tem mais palavras, pode ser que possamos liberar este n�.
                bool tem_filhos = false;
                int i;
                for (i = 0; i < DIGITOS; i++) {
                    if (curr->filhos[i] != NULL && i != 0) { // Ignora o filho 0 por enquanto
                        tem_filhos = true;
                        break;
                    }
                }

                // Se o n� removido n�o tinha filhos (al�m do '0' atual)
                // e n�o tinha outras palavras, e n�o � a raiz, ele pode ser liberado.
                // A libera��o do n� '0' filho ser� tratada se ele n�o tiver mais utilidade.
                
                // Se o n� atual (curr) tem um filho 0, e n�s estamos removendo a palavra dele,
                // precisamos conectar o prev (se existir) ao filho 0 de curr, e liberar curr.
                if (prev) { // Se n�o estamos no n� original (h� ramifica��o '0')
                    prev->filhos[0] = curr->filhos[0]; // Reencadeia
                    free(curr); // Libera o n� onde a palavra estava
                    return false; // J� liberamos um n�, n�o h� mais para liberar recursivamente aqui
                } else { // Se estamos no n� original (profundidade == strlen(codigo))
                    // Verifica se o n� pode ser deletado. Se ele tiver um filho 0,
                    // e a palavra foi removida, mas ainda h� um filho 0, n�o devemos liberar este n�,
                    // mas o filho 0 pode precisar ser reencadeado.
                    // Isso � complexo. A simplifica��o �: se n�o tem filhos E n�o tem palavra, pode liberar.
                    bool pode_deletar_curr = true;
                    if (curr->filhos[0] != NULL) pode_deletar_curr = false; // Se tem filho 0, n�o pode deletar
                    int i;
					for(i = 1; i < DIGITOS; i++){ // Se tem outros filhos, n�o pode deletar
                        if(curr->filhos[i] != NULL) pode_deletar_curr = false;
                    }
                    
                    if (pode_deletar_curr) return true; // Indica que o n� pode ser liberado pelo pai
                    return false; // N�o pode ser liberado agora, mas a palavra foi removida
                }
            }
            prev = curr;
            curr = curr->filhos[0]; // Move para a pr�xima colis�o
        }
        return false; // Palavra n�o encontrada no caminho de colis�es
    }

    // Se n�o chegamos ao fim do c�digo, continue a travessia
    int idx = codigo[profundidade] - '0';
    if (idx < 0 || idx >= DIGITOS || !node->filhos[idx]) {
        return false; // Caminho n�o existe, palavra n�o est� na Trie
    }

    // Chamada recursiva para o pr�ximo n�
    bool deve_deletar_filho = _remover_recursivo(node->filhos[idx], codigo, palavra, profundidade + 1);

    // P�s-ordem: verifica se o filho pode ser deletado
    if (deve_deletar_filho) {
        free(node->filhos[idx]);
        node->filhos[idx] = NULL;

        // Verifica se o n� atual (pai) pode ser deletado:
        // Se ele n�o tem palavra e n�o tem mais filhos (incluindo o filho 0)
        bool tem_filhos_restantes = false;
        int i;
		for (i = 0; i < DIGITOS; i++) {
            if (node->filhos[i] != NULL) {
                tem_filhos_restantes = true;
                break;
            }
        }
        // Se o n� atual n�o tem palavra e n�o tem mais filhos, ele tamb�m pode ser deletado
        return !node->palavra && !tem_filhos_restantes;
    }

    return false; // O n� atual n�o pode ser deletado
}


/**@return true se a palavra foi encontrada e removida, false caso contr�rio.*/
bool remover_palavra(TrieNode *raiz, const char *palavra) {
    if (!raiz || !palavra || *palavra == '\0') {
        return false;
    }

    char codigo[strlen(palavra) + 1];
    palavra_pra_codigo(palavra, codigo);

    // Primeiro, verifica se a palavra existe para evitar travessias desnecess�rias
    if (!procurar_palavra(raiz, palavra)) {
        return false; // A palavra n�o est� na Trie, nada a remover
    }

    // Chama a fun��o recursiva de remo��o
    _remover_recursivo(raiz, codigo, palavra, 0);

    // Se a palavra foi removida, procurar_palavra deve retornar falso agora
    return !procurar_palavra(raiz, palavra);
}



