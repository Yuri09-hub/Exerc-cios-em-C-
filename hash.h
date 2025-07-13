#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#define TAMANHO_TABELA 10
#define MAX_NOME_USUARIO 50
#define MAX_SENHA 50

// Estrutura para armazenar dados do usuário
typedef struct Usuario {
    char nome_usuario[MAX_NOME_USUARIO];
    char senha[MAX_SENHA];
    struct Usuario* proximo; // Para tratamento de colisões (encadeamento)
} Usuario;

// Estrutura da tabela hash
typedef struct TabelaHash {
    Usuario* tabela[TAMANHO_TABELA];
} TabelaHash;

// Funções da tabela hash
TabelaHash* criar_tabela_hash();
unsigned int funcao_hash(const char* chave);
void inserir_usuario(TabelaHash* th, const char* nome_usuario, const char* senha);
Usuario* buscar_usuario(TabelaHash* th, const char* nome_usuario);
void liberar_tabela_hash(TabelaHash* th);

#endif
