#ifndef TRIE_H
#define TRIE_H


#define DIGITOS 10

typedef struct TrieNode {
    char *palavra;
    struct TrieNode *filhos[DIGITOS]; // filhos[0�9]
} TrieNode;

// Cria��o da raiz da Trie
TrieNode* criar_no();

// Libera mem�ria da Trie
void liberar_trie(TrieNode *raiz);

// Fun��o para converter letra em n�mero 
char letra_para_digito(char c);

// Converte palavra para c�digo num�rico
void palavra_para_codigo(const char *palavra, char *codigo);

// Insere palavra na Trie com ramifica��o usando 0
void inserir_palavra(TrieNode *raiz, const char *codigo, const char *palavra);

// Traduz c�digo num�rico e imprime a palavra
void traduzir_codigo(TrieNode *raiz, const char *codigo);

void imprimir_palavras(TrieNode *raiz);

bool procurar_palavra(TrieNode *raiz, const char *palavra);

bool remover_palavra(TrieNode *raiz, const char *palavra); // Retorna true se removeu, false caso contr�rio

#endif
