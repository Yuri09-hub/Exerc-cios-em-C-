#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Cria uma nova tabela hash
TabelaHash* criar_tabela_hash() {
    TabelaHash* th = (TabelaHash*)malloc(sizeof(TabelaHash));
    if (th == NULL) {
        printf("Erro ao alocar memoria para tabela hash\n");
        return NULL;
    }
    
    // Inicializa todos os ponteiros como NULL
    int i;
    for (i = 0; i < TAMANHO_TABELA; i++) {
        th->tabela[i] = NULL;
    }
    
    return th;
}

// Fun??o de hash simples usando m?todo da divis?o
unsigned int funcao_hash(const char* chave) {
    unsigned int hash = 0;
    int i;
    for (i = 0; chave[i] != '\0'; i++) {
        hash += chave[i];
    }
    return hash % TAMANHO_TABELA;
}

// Insere um usu?rio na tabela hash
void inserir_usuario(TabelaHash* th, const char* nome_usuario, const char* senha) {
    if (th == NULL || nome_usuario == NULL || senha == NULL) {
        return;
    }
    
    unsigned int indice = funcao_hash(nome_usuario);
    
    // Cria novo usu?rio
    Usuario* novo_usuario = (Usuario*)malloc(sizeof(Usuario));
    if (novo_usuario == NULL) {
        printf("Erro ao alocar memoria para usuario\n");
        return;
    }
    
    strcpy(novo_usuario->nome_usuario, nome_usuario);
    strcpy(novo_usuario->senha, senha);
    novo_usuario->proximo = NULL;
    
    // Se a posi??o estiver vazia, insere diretamente
    if (th->tabela[indice] == NULL) {
        th->tabela[indice] = novo_usuario;
    } else {
        // Tratamento de colis?o - insere no in?cio da lista encadeada
        novo_usuario->proximo = th->tabela[indice];
        th->tabela[indice] = novo_usuario;
    }
}

// Busca um usu?rio na tabela hash
Usuario* buscar_usuario(TabelaHash* th, const char* nome_usuario) {
    if (th == NULL || nome_usuario == NULL) {
        return NULL;
    }
    
    unsigned int indice = funcao_hash(nome_usuario);
    Usuario* atual = th->tabela[indice];
    
    // Percorre a lista encadeada na posi??o do hash
    while (atual != NULL) {
        if (strcmp(atual->nome_usuario, nome_usuario) == 0) {
            return atual;
        }
        atual = atual->proximo;
    }
    
    return NULL; // Usu?rio n?o encontrado
}

// Libera a mem?ria da tabela hash
void liberar_tabela_hash(TabelaHash* th) {
    if (th == NULL) {
        return;
    }
    int i;
    for (i = 0; i < TAMANHO_TABELA; i++) {
        Usuario* atual = th->tabela[i];
        while (atual != NULL) {
            Usuario* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    
    free(th);
}
