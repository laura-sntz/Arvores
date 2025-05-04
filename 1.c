#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// estrutura para armazenar os dados do usuário
typedef struct Usuario {
    char nome[100];
    int id;
    char email[100];
} Usuario;

// estrutura do nó da árvore AVL
typedef struct NO {
    Usuario usuario;
    int altura;
    struct NO *esq;
    struct NO *dir;
} NO;

// retorna a altura de um nó ou -1 se for nulo
int altura_NO(NO *no) {
    if (no == NULL) return -1;
    return no->altura;
}

// retorna o maior valor entre dois inteiros
int maior(int a, int b) {
    return (a > b) ? a : b;
}

// cria um novo nó com os dados do usuário
NO* novoNO(Usuario u) {
    NO* no = (NO*)malloc(sizeof(NO));
    no->usuario = u;
    no->altura = 0;
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

// realiza uma rotação simples à esquerda (caso direita-direita)
NO* rotacaoRR(NO *raiz) {
    NO *no = raiz->dir;
    raiz->dir = no->esq;
    no->esq = raiz;
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), altura_NO(no->dir)) + 1;
    return no;
}

// realiza uma rotação simples à direita (caso esquerda-esquerda)
NO* rotacaoLL(NO *raiz) {
    NO *no = raiz->esq;
    raiz->esq = no->dir;
    no->dir = raiz;
    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    no->altura = maior(altura_NO(no->esq), altura_NO(no->dir)) + 1;
    return no;
}

// realiza rotação esquerda-direita (caso esquerda-direita)
NO* rotacaoLR(NO *raiz) {
    raiz->esq = rotacaoRR(raiz->esq);
    return rotacaoLL(raiz);
}

// realiza rotação direita-esquerda (caso direita-esquerda)
NO* rotacaoRL(NO *raiz) {
    raiz->dir = rotacaoLL(raiz->dir);
    return rotacaoRR(raiz);
}

// calcula o fator de balanceamento de um nó
int fatorBalanceamento(NO *no) {
    if (no == NULL) return 0;
    return altura_NO(no->esq) - altura_NO(no->dir);
}

// insere um usuário na árvore AVL
NO* inserir(NO *raiz, Usuario u) {
    if (raiz == NULL) return novoNO(u);

    int cmp = strcmp(u.nome, raiz->usuario.nome);
    if (cmp < 0)
        raiz->esq = inserir(raiz->esq, u);
    else if (cmp > 0)
        raiz->dir = inserir(raiz->dir, u);
    else {
        printf("\nusuario com esse nome ja existe!\n");
        return raiz;
    }

    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && strcmp(u.nome, raiz->esq->usuario.nome) < 0) return rotacaoLL(raiz);
    if (fb < -1 && strcmp(u.nome, raiz->dir->usuario.nome) > 0) return rotacaoRR(raiz);
    if (fb > 1 && strcmp(u.nome, raiz->esq->usuario.nome) > 0) return rotacaoLR(raiz);
    if (fb < -1 && strcmp(u.nome, raiz->dir->usuario.nome) < 0) return rotacaoRL(raiz);

    return raiz;
}

// encontra o menor nó (usado na remoção)
NO* menorValor(NO* no) {
    NO* atual = no;
    while (atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

// remove um usuário da árvore AVL pelo nome
NO* remover(NO *raiz, char nome[]) {
    if (raiz == NULL) return raiz;

    int cmp = strcmp(nome, raiz->usuario.nome);
    if (cmp < 0)
        raiz->esq = remover(raiz->esq, nome);
    else if (cmp > 0)
        raiz->dir = remover(raiz->dir, nome);
    else {
        if (raiz->esq == NULL || raiz->dir == NULL) {
            NO *temp = raiz->esq ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            NO *temp = menorValor(raiz->dir);
            raiz->usuario = temp->usuario;
            raiz->dir = remover(raiz->dir, temp->usuario.nome);
        }
    }

    if (raiz == NULL) return raiz;

    raiz->altura = maior(altura_NO(raiz->esq), altura_NO(raiz->dir)) + 1;
    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0) return rotacaoLL(raiz);
    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) return rotacaoLR(raiz);
    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0) return rotacaoRR(raiz);
    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) return rotacaoRL(raiz);

    return raiz;
}

// busca um usuário pelo nome na árvore
NO* buscar(NO *raiz, char nome[]) {
    if (raiz == NULL) return NULL;

    int cmp = strcmp(nome, raiz->usuario.nome);
    if (cmp == 0) return raiz;
    if (cmp < 0) return buscar(raiz->esq, nome);
    return buscar(raiz->dir, nome);
}

// imprime a árvore em ordem alfabética
void imprimirEmOrdem(NO *raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esq);
        printf("nome: %s | id: %d | email: %s\n", raiz->usuario.nome, raiz->usuario.id, raiz->usuario.email);
        imprimirEmOrdem(raiz->dir);
    }
}

// função principal com menu de opções
int main() {
    NO *raiz = NULL;
    int opcao;
    do {
        printf("\nEscolha a opção desejada!!?\n");
        printf("1 - Cadastrar usuario\n2 - Remover usuario\n3 - Listar usuarios\n4 - Buscar usuario\n0 - Sair\n> ");
        scanf("%d", &opcao);
        getchar();

        if (opcao == 1) {
            Usuario u;
            printf("Nome: "); fgets(u.nome, 100, stdin); u.nome[strcspn(u.nome, "\n")] = 0;
            printf("Id: "); scanf("%d", &u.id); getchar();
            printf("Email: "); fgets(u.email, 100, stdin); u.email[strcspn(u.email, "\n")] = 0;
            raiz = inserir(raiz, u);
        } else if (opcao == 2) {
            char nome[100];
            printf("Digite o nome do usuario a remover: ");
            fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = 0;
            raiz = remover(raiz, nome);
        } else if (opcao == 3) {
            imprimirEmOrdem(raiz);
        } else if (opcao == 4) {
            char nome[100];
            printf("Digite o nome do usuario a buscar: ");
            fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = 0;
            NO* encontrado = buscar(raiz, nome);
            if (encontrado)
                printf("Usuario encontrado: %s | id: %d | email: %s\n", encontrado->usuario.nome, encontrado->usuario.id, encontrado->usuario.email);
            else
                printf("Usuario nao encontrado\n");
        }

    } while (opcao != 0);

    return 0;
}
