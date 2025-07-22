#include <stdio.h>
//#include <stdio_ext.h>
#include <string.h>
#define TAM 3

int lerInteiro(){
    char buffer[50];
    int valor;
    fgets(buffer,sizeof(buffer),stdin);
    sscanf(buffer,"%d",&valor);
    return valor;
}

float lerFloat(){
    char buffer[50];
    float valor;
    fgets(buffer,sizeof(buffer),stdin);
    for(int i=0;buffer[i] !='\0';i++){
        if (buffer[i]==','){
            buffer[i]='.';
        }
    }
    sscanf(buffer,"%f",&valor);
    return valor;
}

void lerString(char *destino, int tamanho){
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino,"\n")] ='\0';
}

struct Produtos{
    int codigo;
    int quantidade;
    char nome[50];
    float preco;
};

int main() {
 //   struct Produto;
    char buffer[50];
    int opcao,indice=0;
    struct Produtos produto[TAM];
    do{
        printf("1 - Cadastrar produto. \n");
        printf("2 - Listar produtos. \n");
        printf("3 - Efetuar pedido. \n");
        printf("4 - Listar pedidos. \n");
        printf("9 - Sair\n\n");
        printf("Informe uma opção: ");
        opcao = lerInteiro();
        switch(opcao){
            case 1:
                if(indice<TAM){
                    for(int i=indice;i<TAM;i++){
                            produto[i].codigo=i+1;
                            printf("Código produto: %d \nInforme o nome do produto:\n",produto[i].codigo);
                            lerString(produto[i].nome,sizeof(produto[i].nome));

                            printf("Informe a quantidade do produto:\n");
                            produto[i].quantidade=lerInteiro();

                            printf("Informe o preço do produto:\n");
                            produto[i].preco=lerFloat();
                            indice++;
                    }
                } 
                else{
                        printf("O sistema já chegou ao limite de cadastros.\n\n");
                        getchar();
                    }
            break;
            case 2:
                for(int i=0;i<indice;i++){
                    printf("----Código produto: %d ----\n Nome do produto: %s \n Quantidade: %d \n Preço: %.2f\n",produto[i].codigo,produto[i].nome,produto[i].quantidade,produto[i].preco);
                }
                getchar();
            break;
            case 3:
                printf("a");
            break;
            case 9:
                printf("saindo...");
            break;
            default:
                printf("Informe uma opção valida.\n");
                getchar();
        }
    }while(opcao!=9);


    return 0;
}

