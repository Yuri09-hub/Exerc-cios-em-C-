#include <stdio.h>
#include <string.h>
#include <conio.h> // Para getch() no Windows
#include "login.h"

// Inicializa os dois usu�rios pr�-cadastrados
void inicializar_usuarios(TabelaHash* th) {
    if (th == NULL) {
        return;
    }
    
    // Usu�rio 1
    inserir_usuario(th, "fabioandrade@gmail.com", "admin123");
    
    // Usu�rio 2
    inserir_usuario(th, "yuri@gmail.com", "senha456");
    
    printf("Usuarios cadastrados:\n");
    printf("1. E-mail de usuario: fabioandrade@gmail.com, Senha: admin123\n");
    printf("2. E-mail de usuario: yuri@gmail.com, Senha: senha456\n\n");
}

// Autentica um usu�rio
int autenticar_usuario(TabelaHash* th, const char* nome_usuario, const char* senha) {
    if (th == NULL || nome_usuario == NULL || senha == NULL) {
        return 0;
    }
    
    Usuario* usuario = buscar_usuario(th, nome_usuario);
    
    if (usuario != NULL && strcmp(usuario->senha, senha) == 0) {
        return 1; // Autentica��o bem-sucedida
    }
    
    return 0; // Falha na autentica��o
}

// Limpa o buffer de entrada
void limpar_buffer_entrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fun��o para ocultar a senha durante a digita��o
void ocultar_senha(char* senha, int tamanho_maximo) {
    int i = 0;
    char ch;
    
    while (i < tamanho_maximo - 1) {
        ch = getch(); // L� caractere sem exibir
        
        if (ch == '\r' || ch == '\n') { // Enter
            break;
        } else if (ch == '\b' && i > 0) { // Backspace
            printf("\b \b");
            i--;
        } else if (ch != '\b') {
            senha[i] = ch;
            printf("*");
            i++;
        }
    }
    
    senha[i] = '\0';
    printf("\n");
}

// NOVA FUN��O: L� e exibe o conte�do de um arquivo de palavras
void lerArquivoDePalavras(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r"); // Abre o arquivo para leitura
    char linha[256]; // Buffer para ler cada linha do arquivo

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de palavras");
        return;
    }

    printf("\n--- Conteudo do Arquivo '%s' ---\n", nomeArquivo);
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha); // Imprime a linha lida (fgets j� inclui o newline)
    }
    printf("----------------------------------\n");

    fclose(arquivo); // Fecha o arquivo
}
