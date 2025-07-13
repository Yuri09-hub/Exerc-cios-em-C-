#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#include "login.h"
#include "Trie.h"

// Função para exibir o menu e obter a escolha do usuário
int exibirMenu() {
    int escolha;
    printf("\n--- Menu de Opcoes ---\n");
    printf("1. Inserir nova palavra\n");
    printf("2. Procurar palavra\n");
    printf("3. Remover palavra\n");
    printf("4. Imprimir todas as palavras\n");
    printf("5. Traduzir palavra (por ID numerico)\n");
    printf("0. Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &escolha);
    // Limpa o buffer do teclado para o próximo fgets/scanf
    while (getchar() != '\n');
    return escolha;
}

int main() {
    TabelaHash* tabela_hash;
    char nome_usuario[MAX_NOME_USUARIO];
    char senha[MAX_SENHA];
    int tentativas = 0;
    const int max_tentativas = 3;
    TrieNode *raiz = criar_no();
    char codigo[50];
    char palavra_input[50];
    char nome_arquivo[100]; 
    int opcao;
    
    printf("=== SISTEMA DE LOGIN ===\n\n");
    
    // Cria a tabela hash
    tabela_hash = criar_tabela_hash();
    if (tabela_hash == NULL) {
        printf("Erro ao criar tabela hash!\n");
        return 1;
    }
    
    // Inicializa os usuários pré-cadastrados
    inicializar_usuarios(tabela_hash);
    
    // Loop de tentativas de login
    while (tentativas < max_tentativas) {
        printf("--- Tentativa %d de %d ---\n", tentativas + 1, max_tentativas);
        
        // Solicita nome de usuário
        printf("Email de usuario: ");
        if (fgets(nome_usuario, sizeof(nome_usuario), stdin) != NULL) {
            // Remove o '\n' do final da string
            nome_usuario[strcspn(nome_usuario, "\n")] = '\0';
        }
        
        // Solicita senha (com ocultação)
        printf("Senha: ");
        ocultar_senha(senha, sizeof(senha));
        
        // Verifica autenticação
        if (autenticar_usuario(tabela_hash, nome_usuario, senha)) {
            printf("\nLogin realizado com sucesso!\n");
            printf("Bem-vindo, %s!\n", nome_usuario);
            
            // SOLICITA O NOME DO ARQUIVO AO USUÁRIO
            printf("\nDigite o nome do arquivo de palavras para carregar: ");
            if (fgets(nome_arquivo, sizeof(nome_arquivo), stdin) != NULL) {
                // Remove o '\n' do final da string
                nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0';
                
                // Carrega e insere palavras do arquivo na Trie
                printf("Carregando palavras do arquivo '%s' na Trie...\n", nome_arquivo);
                
                FILE *arquivo = fopen(nome_arquivo, "r");
                if (arquivo != NULL) {
                    char palavra[100];
                    int palavras_inseridas = 0;
                    
                    printf("Inserindo palavras na Trie:\n");
                    while (fgets(palavra, sizeof(palavra), arquivo)) {
                        // Remove o '\n' do final da palavra
                        palavra[strcspn(palavra, "\n")] = '\0';
                        
                        // Verifica se a palavra não está vazia
                        if (strlen(palavra) > 0) {
                            // Converte palavra para código T9
                            palavra_pra_codigo(palavra, codigo);
                            
                            // Insere na Trie
                            inserir_palavra(raiz, codigo, palavra);
                            palavras_inseridas++;
                            
                            // Mostra progresso
                            printf("  - '%s' (codigo: %s)\n", palavra, codigo);
                        }
                    }
                    
                    fclose(arquivo);
                    printf("\n Total de %d palavras inseridas na Trie com sucesso!\n", palavras_inseridas);
                } else {
                    printf("Erro: Nao foi possível abrir o arquivo '%s'.\n", nome_arquivo);
                    printf("Verifique se o arquivo existe e se o nome esta correto.\n");
                }
            }
            
             // Loop do menu
            do {
                opcao = exibirMenu();

                switch (opcao) {
                    case 1: // Inserir nova palavra
                        printf("Digite a palavra para inserir: ");
                        fgets(palavra_input, sizeof(palavra_input), stdin);
                        palavra_input[strcspn(palavra_input, "\n")] = 0; // Remove '\n'
                        palavra_pra_codigo(palavra_input, codigo);
                        inserir_palavra(raiz, codigo, palavra_input);
                        printf("Palavra '%s' inserida (codigo: %s).\n", palavra_input, codigo);
                        break;
                    case 2: // Procurar palavra
                        printf("Digite a palavra para procurar: ");
                        fgets(palavra_input, sizeof(palavra_input), stdin);
                        palavra_input[strcspn(palavra_input, "\n")] = 0; // Remove '\n'
                        if (procurar_palavra(raiz, palavra_input)) {
                            printf("'%s' encontrada na Trie.\n", palavra_input);
                        } else {
                            printf("'%s' NAO encontrada na Trie.\n", palavra_input);
                        }
                        break;
                    case 3: // Remover palavra
                        printf("Digite a palavra para remover: ");
                        fgets(palavra_input, sizeof(palavra_input), stdin);
                        palavra_input[strcspn(palavra_input, "\n")] = 0; // Remove '\n'
                        if (remover_palavra(raiz, palavra_input)) {
                           printf("'%s' removida da Trie.\n", palavra_input);
                        } else {
                           printf("'%s' NAO pode ser removida (talvez nao exista ou haja um erro).\n", palavra_input);
                        }
                        break;
                    case 4: // Imprimir todas
                        imprimir_palavras(raiz);
                        break;
                    case 5: // Traduzir palavra
                        printf("Digite o codigo T9 para traduzir: ");
                        fgets(palavra_input, sizeof(palavra_input), stdin);
                        palavra_input[strcspn(palavra_input, "\n")] = 0; // Remove '\n'
                        traduzir_codigo(raiz, palavra_input);
                        break;
                    case 0: // Sair
                        printf("Saindo do programa!\n");
                        break;
                    default:
                        printf("Opção inválida. Tente novamente.\n");
                        break;
                }
            } while (opcao != 0);
            break;
        } else {
            tentativas++;
            printf("\n? Credenciais invalidas!\n");
            
            if (tentativas < max_tentativas) {
                printf("Tente novamente.\n\n");
            } else {
                printf("Numero maximo de tentativas excedido.\n");
                printf("Acesso bloqueado!\n");
            }
        }
    }
    
    // Libera a memória
    liberar_tabela_hash(tabela_hash);
    
    printf("\nPressione qualquer tecla para sair...");
    getchar();
    
    return 0;
}
