#include <stdio.h>
#define TAM 3

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

struct Produtos {
    int codigo;
    int quantidade;
    char nome[15];
    float preco;
};

int main() {
    int opcao=0, indice=0, sair=0;
    struct Produtos produto[TAM];
    int codPedido, quantPedido;
    do{
        printf("1 - Cadastrar produto.\n");
        printf("2 - Listar produtos.\n");
        printf("3 - Efetuar pedido.\n");
        printf("4 - Listar pedidos.\n");
        printf("0 - Sair\n\n");
        printf("Escolha a opção:"); 
        limpar_buffer();
        scanf(" %d", &opcao);
        sair=opcao;

        switch (opcao) {
            case 1:
                for (int i = indice; i < TAM; i++) {
                    produto[i].codigo = i + 1;
                    printf("Código produto: %d - Informe o nome do produto:\n", produto[i].codigo);
                    fgets(produto[i].nome, sizeof(produto[i].nome), stdin);
                    printf("Informe a quantidade do produto:\n");
                    scanf("%d", &produto[i].quantidade);
                    printf("Informe o preço do produto:\n");
                    scanf("%f", &produto[i].preco);
                    indice++;
                }
                break;

            case 2:
                for (int i = 0; i < indice; i++) {
                    printf("Código produto: %d\n", produto[i].codigo);
                    printf("Nome do produto: %s\n", produto[i].nome);
                    printf("Quantidade: %d\n", produto[i].quantidade);
                    printf("Preço: %.2f\n", produto[i].preco);
                    printf("---------------\n");
                }
                getchar();
                break;

            case 3:
                printf("Informe o código do produto para pedido:\n");
                scanf("%d", &codPedido);
                printf("Informe a quantidade desejada:\n");
                scanf("%d", &quantPedido);

                for (int i = 0; i < indice; i++) {
                    if (produto[i].codigo == codPedido) {
                        if (produto[i].quantidade >= quantPedido) {
                            produto[i].quantidade -= quantPedido;
                            printf("Pedido realizado com sucesso!\n");
                        } else {
                            printf("Quantidade insuficiente em estoque.\n");
                        }
                    }
                }
                break;

            case 4:
                printf("Pedidos efetuados (estoque atualizado):\n");
                for (int i = 0; i < indice; i++) {
                    printf("Código produto: %d\n", produto[i].codigo);
                    printf("Nome do produto: %s", produto[i].nome);
                    printf("Quantidade restante: %d\n", produto[i].quantidade);
                    printf("Preço: %.2f\n", produto[i].preco);
                }
                break;

            default:
                printf("Informe uma opção valida.\n");
        }
        
    } while (sair!=0);   
    return 0;
}