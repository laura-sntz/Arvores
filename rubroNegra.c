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

// Nó sentinela para representar NULL
Node *NULL_LEAF;

// Inicializar o nó sentinela
void inicializarNullLeaf() {
    NULL_LEAF = (Node*)malloc(sizeof(Node));
    NULL_LEAF->cor = BLACK;
    NULL_LEAF->esq = NULL_LEAF->dir = NULL_LEAF->pai = NULL_LEAF;
}

// Criando novo produto
Node* criarNoProduto(int codigo, char* nome, int qtd, float preco) {
    Node* novo = (Node*)malloc(sizeof(Node));
    novo->prod.codigo = codigo;
    strcpy(novo->prod.nome, nome);
    novo->prod.quantidade = qtd;
    novo->prod.preco = preco;
    novo->cor = RED;
    novo->esq = novo->dir = NULL_LEAF;
    novo->pai = NULL_LEAF;
    return novo;
}

// Rotação para a esquerda
Node* rotacaoEsquerda(Node* raiz, Node* x) {
    Node* y = x->dir;
    x->dir = y->esq;
    if (y->esq != NULL_LEAF) y->esq->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL_LEAF) raiz = y;
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
    if (x->dir != NULL_LEAF) x->dir->pai = y;
    x->pai = y->pai;
    if (y->pai == NULL_LEAF) raiz = x;
    else if (y == y->pai->dir) y->pai->dir = x;
    else y->pai->esq = x;
    x->dir = y;
    y->pai = x;
    return raiz;
}

// Código feito por Laura Santos Oliveira
Node* inserirBST(Node* raiz, Node* novo) {
    if (raiz == NULL_LEAF) return novo;
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
            if (tio->cor == RED) {
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
            if (tio->cor == RED) {
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
    if (raiz == NULL_LEAF || raiz->prod.codigo == codigo)
        return raiz;
    if (codigo < raiz->prod.codigo)
        return buscar(raiz->esq, codigo);
    else
        return buscar(raiz->dir, codigo);
}

Node* inserir(Node* raiz, int cod, char* nome, int qtd, float preco) {
    Node* novo = criarNoProduto(cod, nome, qtd, preco);

    if (buscar(raiz, cod) != NULL_LEAF) {
        printf("Erro: Produto com código %d já existe!\n", cod);
        free(novo);
        return raiz;
    } else {
        printf("Produto inserido com sucesso!\n");
    }

    raiz = inserirBST(raiz, novo);
    return corrigirInsercao(raiz, novo);
}

void emOrdem(Node* raiz) {
    if (raiz == NULL_LEAF) return;

    emOrdem(raiz->esq);

    printf("Código: %d (%s), Nome: %s, Qtd: %d, Preço: %.2f",
           raiz->prod.codigo,
           raiz->cor == RED ? "R" : "B",
           raiz->prod.nome,
           raiz->prod.quantidade,
           raiz->prod.preco);

    if (raiz->pai == NULL_LEAF) {
        printf(" [RAIZ]");
    }

    printf("\n");

    emOrdem(raiz->dir);
}

void transplantar(Node** raiz, Node* u, Node* v) {
    if (u->pai == NULL_LEAF)
        *raiz = v;
    else if (u == u->pai->esq)
        u->pai->esq = v;
    else
        u->pai->dir = v;

    v->pai = u->pai;
}

Node* minimo(Node* no) {
    while (no->esq != NULL_LEAF)
        no = no->esq;
    return no;
}

void verificarArvore(Node* raiz) {
    if (raiz == NULL_LEAF) return;
    
    // Propriedade 2: A raiz é preta
    if (raiz->pai == NULL_LEAF && raiz->cor != BLACK) {
        printf("ERRO: Raiz não é preta!\n");
    }
    
    // Propriedade 3: Nenhum nó vermelho tem filho vermelho
    if (raiz->cor == RED) {
        if (raiz->esq != NULL_LEAF && raiz->esq->cor == RED) {
            printf("ERRO: Nó %d vermelho com filho esquerdo vermelho!\n", raiz->prod.codigo);
        }
        if (raiz->dir != NULL_LEAF && raiz->dir->cor == RED) {
            printf("ERRO: Nó %d vermelho com filho direito vermelho!\n", raiz->prod.codigo);
        }
    }
    
    verificarArvore(raiz->esq);
    verificarArvore(raiz->dir);
}

Node* corrigirRemocao(Node* raiz, Node* x) {
    while (x != raiz && x->cor == BLACK) {
        if (x == x->pai->esq) {
            Node* w = x->pai->dir;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, x->pai);
                w = x->pai->dir;
            }

            if (w->esq->cor == BLACK && w->dir->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->dir->cor == BLACK) {
                    w->esq->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoDireita(raiz, w);
                    w = x->pai->dir;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->dir->cor = BLACK;
                raiz = rotacaoEsquerda(raiz, x->pai);
                x = raiz;
            }
        } else {
            Node* w = x->pai->esq;
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoDireita(raiz, x->pai);
                w = x->pai->esq;
            }

            if (w->dir->cor == BLACK && w->esq->cor == BLACK) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq->cor == BLACK) {
                    w->dir->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoEsquerda(raiz, w);
                    w = x->pai->esq;
                }
                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                w->esq->cor = BLACK;
                raiz = rotacaoDireita(raiz, x->pai);
                x = raiz;
            }
        }
    }
    x->cor = BLACK;
    return raiz;
}

Node* remover(Node* raiz, int codigo) {
    if (raiz == NULL_LEAF) {
        printf("A árvore está vazia!\n");
        return raiz;
    }

    Node* z = buscar(raiz, codigo);
    if (z == NULL_LEAF) {
        printf("Produto com código %d não encontrado!\n", codigo);
        return raiz;
    }

    // Verifica se é o caso especial (3 nós no total e removendo a raiz)
    int nos_esq = (raiz->esq != NULL_LEAF);
    int nos_dir = (raiz->dir != NULL_LEAF);
    int total_nos = 1 + nos_esq + nos_dir;

    if (total_nos == 3 && z == raiz) {
        Node* novo_raiz;
        Node* outro_no;
        
        // Determina qual nó será a nova raiz (o menor entre os dois restantes)
        if (raiz->esq != NULL_LEAF && raiz->dir != NULL_LEAF) {
            // Escolhe o menor entre esq e dir para ser a nova raiz
            novo_raiz = (raiz->esq->prod.codigo < raiz->dir->prod.codigo) ? raiz->esq : raiz->dir;
            outro_no = (novo_raiz == raiz->esq) ? raiz->dir : raiz->esq;
        } else {
            novo_raiz = (raiz->esq != NULL_LEAF) ? raiz->esq : raiz->dir;
            outro_no = NULL_LEAF;
        }

        // Configura a nova raiz
        novo_raiz->pai = NULL_LEAF;
        novo_raiz->cor = BLACK;
        
        // Configura o outro nó como filho direito (se existir)
        if (outro_no != NULL_LEAF) {
            novo_raiz->dir = outro_no;
            outro_no->pai = novo_raiz;
            outro_no->cor = RED;
            novo_raiz->esq = NULL_LEAF;
        }

        free(raiz);
        return novo_raiz;
    }

    // Restante da função original para casos normais
    Node* y = z;
    Node* x;
    Color corOriginal = y->cor;

    if (z->esq == NULL_LEAF) {
        x = z->dir;
        transplantar(&raiz, z, z->dir);
    } else if (z->dir == NULL_LEAF) {
        x = z->esq;
        transplantar(&raiz, z, z->esq);
    } else {
        y = minimo(z->dir);
        corOriginal = y->cor;
        x = y->dir;
        if (y->pai == z) {
            x->pai = y;
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

    free(z);
    printf("Produto removido com sucesso!\n");

    if (corOriginal == BLACK)
        raiz = corrigirRemocao(raiz, x);

    return raiz;
}

// Função principal com menu
int main() {
    system("chcp 65001");
    system("cls");

    // Inicializar o nó sentinela
    inicializarNullLeaf();
    
    Node* raiz = NULL_LEAF;
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

        switch (opcao) {
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
                verificarArvore(raiz);
                break;

            case 2:
                printf("Código do produto a remover: ");
                scanf("%d", &cod);
                raiz = remover(raiz, cod);
                verificarArvore(raiz);
                break;

            case 3: {
                printf("Código do produto a buscar: ");
                scanf("%d", &cod);
                Node* encontrado = buscar(raiz, cod);
                if (encontrado != NULL_LEAF) {
                    printf("Produto encontrado: Código: %d, Nome: %s, Qtd: %d, Preço: %.2f\n",
                           encontrado->prod.codigo, encontrado->prod.nome,
                           encontrado->prod.quantidade, encontrado->prod.preco);
                } else {
                    printf("Produto não encontrado.\n");
                }
                break;
            }

            case 4:
                if (raiz == NULL_LEAF) {
                    printf("A árvore está vazia!\n");
                } else {
                    printf("=== LISTA DE PRODUTOS (in-order) ===\n");
                    emOrdem(raiz);
                }
                break;

            case 0:
                printf("Encerrando programa...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}