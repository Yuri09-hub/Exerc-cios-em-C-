#ifndef LOGIN_H
#define LOGIN_H

#include "hash.h"

// Funções do sistema de login
void inicializar_usuarios(TabelaHash* th);
int autenticar_usuario(TabelaHash* th, const char* nome_usuario, const char* senha);
void limpar_buffer_entrada();
void ocultar_senha(char* senha, int tamanho_maximo);
void lerArquivoDePalavras(const char *nomeArquivo); 

#endif
