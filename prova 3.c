#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 3

typedef struct {
    char nome[50];
    int cpf;
    int dias_hospedagem;
    int qtdAcompanhante;
} Hospede;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void telaInicial(){
    printf("\t\t\t HAP RESORT\n\n\n");
    printf("\t\t\t\t\t Bem Vindo ao paraiso.\n\n\n");
    printf("Pressione Enter para continuar...");
    getchar();
}

void cadastrarHospede(Hospede hospede[], int *indice){
    int index = *indice;
    if(index < TAM){
        printf("\n --- Cadastro de hospede e seus acompanhantes ---\n");

        printf("Informe o nome do hospede: \n");
        fgets(hospede[index].nome, 50, stdin);
        // Remove o '\n' do final da string
        hospede[index].nome[strcspn(hospede[index].nome, "\n")] = '\0';

        printf("Informe o CPF: \n");
        scanf("%d", &hospede[index].cpf);
        limparBuffer();

        printf("Quantos dias irá se hospedar: \n");
        scanf("%d", &hospede[index].dias_hospedagem);
        limparBuffer();

        printf("Quantidade de acompanhantes: \n");
        scanf("%d", &hospede[index].qtdAcompanhante);
        limparBuffer();

        printf("\n Cadastro efetuado com sucesso!\n");
        (*indice)++;
        printf("Pressione Enter para continuar...");
        getchar();
    } else {
        printf("Quantidade de cadastro excedida.\n");
        printf("Pressione Enter para continuar...");
        getchar();
    }
}

float calculoValorTotal(int dias_hospedagem, int qtdAcompanhantes, float valorDiaria){
    // Valor total considerando hospede + acompanhantes
    int totalPessoas = 1 + qtdAcompanhantes;
    return dias_hospedagem * totalPessoas * valorDiaria;
}

int main(){
    int opcao = 0;
    int indice = 0;
    float valorDiaria = 0, totalFaturamento = 0;
    Hospede hospede[TAM];
    char busca[50];

    telaInicial();

    do {
        printf("\t\t\t HAP RESORT\n\n\n");
        printf("1 - Cadastrar um novo hóspede com seus acompanhantes\n");
        printf("2 - Finalizar hospedagem e calcular valor total a ser pago\n");
        printf("3 - Mostrar faturamento diário do hotel\n");
        printf("4 - Sair\n");
        printf(">> Digite a opção desejada: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1: 
                if(indice < TAM){
                    cadastrarHospede(hospede, &indice);
                } else {
                    printf("Limite de hóspedes atingido.\n");
                    printf("Pressione Enter para continuar...");
                    getchar();
                }
                break;

            case 2:
                if(indice > 0){
                    printf("Informe o nome do hospede: \n");
                    fgets(busca, sizeof(busca), stdin);
                    busca[strcspn(busca, "\n")] = '\0'; // remover \n

                    printf("Informe o valor da diaria: ");
                    scanf("%f", &valorDiaria);
                    limparBuffer();

                    int encontrado = 0;
                    for(int i = 0; i < indice; i++){
                        if(strcmp(busca, hospede[i].nome) == 0){
                            printf("\t\t\t HAP RESORT\n\n\n");
                            printf("NOME DO CLIENTE: %s\n", hospede[i].nome);
                            float valor = calculoValorTotal(hospede[i].dias_hospedagem, hospede[i].qtdAcompanhante, valorDiaria);
                            printf("VALOR TOTAL A PAGAR: %.2f\n", valor);
                            printf("\t\t\t\t Obrigada por sua estadia em nosso hotel.\n");
                            totalFaturamento += valor;
                            encontrado = 1;
                            break;
                        }
                    }
                    if(!encontrado){
                        printf("Hospede não encontrado.\n");
                    }
                    printf("Pressione Enter para continuar...");
                    getchar();
                } else {
                    printf("Nenhum hóspede cadastrado ainda.\n");
                    printf("Pressione Enter para continuar...");
                    getchar();
                }
                break;

            case 3:
                printf("\nFaturamento diario: %.2f\n", totalFaturamento);
                printf("Pressione Enter para continuar...");
                getchar();
                break;

            case 4:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("\nInforme uma opção válida.\n");
                printf("Pressione Enter para continuar...");
                getchar();
                break;
        }

    } while(opcao != 4);

    return 0;
}
