#ifndef TRIE_H
#define TRIE_H


#define DIGITOS 10

typedef struct TrieNode {
    char *palavra;
    struct TrieNode *filhos[DIGITOS]; // filhos[0–9]
} TrieNode;

// Criação da raiz da Trie
TrieNode* criar_no();

// Libera memória da Trie
void liberar_trie(TrieNode *raiz);

// Função para converter letra em número 
char letra_para_digito(char c);

// Converte palavra para código numérico
void palavra_para_codigo(const char *palavra, char *codigo);

// Insere palavra na Trie com ramificação usando 0
void inserir_palavra(TrieNode *raiz, const char *codigo, const char *palavra);

// Traduz código numérico e imprime a palavra
void traduzir_codigo(TrieNode *raiz, const char *codigo);

void imprimir_palavras(TrieNode *raiz);

bool procurar_palavra(TrieNode *raiz, const char *palavra);

bool remover_palavra(TrieNode *raiz, const char *palavra); // Retorna true se removeu, false caso contrário

#endif
