#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cores da árvore
typedef enum { RED, BLACK } Color;

// Informações dos produtos
typedef struct Produto {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
} Produto;

// Struct do nó que armazena o produto, a cor do nó e ponteiros importantes
typedef struct Node {
    Produto prod;
    Color cor;
    struct Node *esq, *dir, *pai;
} Node;

// Criando novo produto
Node* criarNoProduto(int codigo, char* nome, int qtd, float preco) {
    // Aloca espaço para o novo nó
    Node* novo = (Node*)malloc(sizeof(Node));
    // O novo nó recebe as informações passadas para que sejam armazenadas 
    novo->prod.codigo = codigo;
    strcpy(novo->prod.nome, nome);
    novo->prod.quantidade = qtd;
    novo->prod.preco = preco;
    // Todo novo nó deve começar com a cor vermelha
    novo->cor = RED;
    novo->esq = novo->dir = novo->pai = NULL;
    return novo;
}

// Rotação para a esquerda
Node* rotacaoEsquerda(Node* raiz, Node* x) {
    Node* y = x->dir;
    x->dir = y->esq;
    if (y->esq) y->esq->pai = x;
    y->pai = x->pai;
    if (!x->pai) raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
    return raiz;
}

// Rotação para a direita
Node* rotacaoDireita(Node* raiz, Node* y) {
    Node* x = y->esq;
    y->esq = x->dir;
    if (x->dir) x->dir->pai = y;
    x->pai = y->pai;
    if (!y->pai) raiz = x;
    else if (y == y->pai->dir) y->pai->dir = x;
    else y->pai->esq = x;
    x->dir = y;
    y->pai = x;
    return raiz;
}

Node* inserirBST(Node* raiz, Node* novo) {
    if (!raiz) return novo;
    if (novo->prod.codigo < raiz->prod.codigo) {
        raiz->esq = inserirBST(raiz->esq, novo);
        raiz->esq->pai = raiz;
    } else if (novo->prod.codigo > raiz->prod.codigo) {
        raiz->dir = inserirBST(raiz->dir, novo);
        raiz->dir->pai = raiz;
    }
    return raiz;
}

Node* corrigirInsercao(Node* raiz, Node* no) {
    Node* tio;
    while (no != raiz && no->pai->cor == RED) {
        if (no->pai == no->pai->pai->esq) {
            tio = no->pai->pai->dir;
            if (tio && tio->cor == RED) {
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->dir) {
                    no = no->pai;
                    raiz = rotacaoEsquerda(raiz, no);
                }
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                raiz = rotacaoDireita(raiz, no->pai->pai);
            }
        } else {
            tio = no->pai->pai->esq;
            if (tio && tio->cor == RED) {
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esq) {
                    no = no->pai;
                    raiz = rotacaoDireita(raiz, no);
                }
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, no->pai->pai);
            }
        }
    }
    raiz->cor = BLACK;
    return raiz;
}

Node* buscar(Node* raiz, int codigo) {
    if (!raiz || raiz->prod.codigo == codigo)
        return raiz;
    if (codigo < raiz->prod.codigo)
        return buscar(raiz->esq, codigo);
    else
        return buscar(raiz->dir, codigo);
}

Node* inserir(Node* raiz, int cod, char* nome, int qtd, float preco) {
    Node* novo = criarNoProduto(cod, nome, qtd, preco);

    // Verificar se o produto já existe
    if (buscar(raiz, cod) != NULL) {
        printf("Erro: Produto com código %d já existe!\n", cod);
        free(novo);
        return raiz;
    }
    else{
        printf("Produto inserido com sucesso!\n"); 
    }

    raiz = inserirBST(raiz, novo);
    return corrigirInsercao(raiz, novo);
}

void emOrdem(Node* raiz) {
    if (!raiz) return;
    emOrdem(raiz->esq);
    printf("Código: %d (%s), Nome: %s, Qtd: %d, Preço: %.2f\n", raiz->prod.codigo,
           raiz->cor == RED ? "R" : "B", raiz->prod.nome, raiz->prod.quantidade, raiz->prod.preco);
    emOrdem(raiz->dir);
}

void transplantar(Node** raiz, Node* u, Node* v) {
    if (u->pai == NULL)
        *raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;

    if (v != NULL)
        v->pai = u->pai;
}

Node* minimo(Node* no) {
    while (no->esq != NULL)
        no = no->esq;
    return no;
}

Node* corrigirRemocao(Node* raiz, Node* x) {
    Node* w;
    while (x != raiz && (x == NULL || x->cor == BLACK)) {
        if (x == x->pai->esq) {
            w = x->pai->dir; // irmão de x

            // Caso 1: irmão vermelho
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, x->pai);
                w = x->pai->dir;
            }

            // Caso 2: irmão e filhos pretos
            if ((w->esq == NULL || w->esq->cor == BLACK) &&
                (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                // Caso 3: irmão preto, filho esquerdo vermelho
                if (w->dir == NULL || w->dir->cor == BLACK) {
                    if (w->esq != NULL && w->esq->cor == RED) w->esq->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoDireita(raiz, w);
                    w = x->pai->dir;
                }

                // Caso 4: irmão preto, filho direito vermelho
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->dir != NULL && w->dir->cor == RED) w->dir->cor = BLACK;
                raiz = rotacaoEsquerda(raiz, x->pai);
                x = raiz;
            }
        } else {
            // Mesma lógica, mas espelhada para x ser filho da direita
            Node* w = x->pai->esq;

            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoDireita(raiz, x->pai);
                w = x->pai->esq;
            }

            if ((w->esq == NULL || w->esq->cor == BLACK) &&
                (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq == NULL || w->esq->cor == BLACK) {
                    if (w->dir != NULL && w->dir->cor == RED) w->dir->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoEsquerda(raiz, w);
                    w = x->pai->esq;
                }

                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->esq != NULL && w->esq->cor == RED) w->esq->cor = BLACK;
                raiz = rotacaoDireita(raiz, x->pai);
                x = raiz;
            }
        }
    }

    if (x != NULL)
        x->cor = BLACK;

    return raiz;
}

Node* remover(Node* raiz, int valor) {
    if (!raiz) {
        printf("Árvore vazia!\n");
        return raiz;
    }

    Node* z = buscar(raiz, valor);  // Nó a ser removido
    if (!z) {
        printf("Produto com código %d não encontrado!\n", valor);
        return raiz;
    }
    else{
        printf("Remoção concluída.\n");
    }

    Node *y = z;                    // Nó que será efetivamente removido
    Node *x;                        // Nó que tomará o lugar (ou NULL_LEAF)
    Color cor_original = y->cor;    // Precisamos saber a cor original

    if (z->esq == NULL) {
        x = z->dir;
        transplantar(&raiz, z, z->dir);
    } else if (z->dir == NULL) {
        x = z->esq;
        transplantar(&raiz, z, z->esq);
    } else {
        y = minimo(z->dir);
        cor_original = y->cor;
        x = y->dir;

        if (y->pai == z) {
            if (x != NULL) x->pai = y;
        } else {
            transplantar(&raiz, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }

        transplantar(&raiz, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }

    if (cor_original == BLACK && x != NULL)
        raiz = corrigirRemocao(raiz, x);
    else if (cor_original == BLACK && y != NULL && y->pai == NULL)
        y->cor = BLACK; // Se o nó removido era a raiz preta e foi substituído por um vermelho, pinte de preto

    free(z);
    return raiz;
}

// Função principal com menu
int main() {
    // Para aparecer acentuação no terminal
    system("chcp 65001");
    system("cls");

    Node* raiz = NULL;
    int opcao, cod, qtd;
    float preco;
    char nome[50];

    do {
        printf("\n==== MENU INVENTÁRIO ====\n");
        printf("1 - Cadastrar Produto\n");
        printf("2 - Remover Produto\n");
        printf("3 - Buscar Produto\n");
        printf("4 - Listar Produtos\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Código: ");
                scanf("%d", &cod);
                printf("Nome: ");
                scanf(" %[^\n]", nome);
                printf("Quantidade: ");
                scanf("%d", &qtd);
                printf("Preço: ");
                scanf("%f", &preco);
                raiz = inserir(raiz, cod, nome, qtd, preco);
                break;

            case 2:
                printf("Código do produto a remover: ");
                scanf("%d", &cod);
                raiz = remover(raiz, cod);
                break;

            case 3: {
                printf("Código do produto a buscar: ");
                scanf("%d", &cod);
                Node* encontrado = buscar(raiz, cod);
                if (encontrado) {
                    printf("Produto encontrado: Código: %d, Nome: %s, Qtd: %d, Preço: %.2f\n",
                           encontrado->prod.codigo, encontrado->prod.nome,
                           encontrado->prod.quantidade, encontrado->prod.preco);
                } else {
                    printf("Produto não encontrado.\n");
                }
                break;
            }

            case 4:
                printf("=== LISTA DE PRODUTOS (in-order) ===\n");
                emOrdem(raiz);
                break;

            case 0:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while(opcao != 0);

    return 0;
}