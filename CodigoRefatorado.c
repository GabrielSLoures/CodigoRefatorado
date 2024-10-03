#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 10

typedef struct Purchase {
    double amount;
    int month;
} Purchase;

typedef struct Client {
    int id;
    char name[30];
    int birthYear;
    Purchase purchase;
    int enable;
} Client;

Client *client = NULL;
int numberOfClients = 0;

void printLine();
int getMonth();
int verifyPosition();
void expandArray(bool start);
void printClient(int i);
void inputClientData(int count);
void askAddPurchase(int count);
bool isClientEnabled(int id);
void create();
void deleteClient();
void getPurchase(int option);
void resetPurchase();
void bestCustomer();
void getClientById();

int main() {
    expandArray(true);
    setlocale(LC_ALL, "Portuguese");
    char option;
    do {
        system("cls");
        printLine();
        printf("%ls\n", L" Opções:");
        printf("%ls\n", L" 1. Incluir um novo cliente");
        printf("%ls\n", L" 2. Remover cliente por ID");
        printf("%ls\n", L" 3. Atualizar o montante de compras do cliente realizadas no mês corrente");
        printf("%ls\n", L" 4. Zerar todos os montantes de compras");
        printf("%ls\n", L" 5. Listar cliente com maior montante de compras");
        printf("%ls\n", L" 6. Exibir um montante de compras por cliente");
        printf("%ls\n", L" 7. Sair");
        printf("%ls", L" Digite a opção desejada: ");
        scanf(" %c", &option);
        setbuf(stdin, NULL);
        system("cls");
        switch (option) {
            case '1':
                create();
                break;
            case '2':
                deleteClient();
                break;
            case '3':
                getPurchase(-1);
                break;
            case '4':
                resetPurchase();
                break;
            case '5':
                bestCustomer();
                break;
            case '6':
                getClientById();
                break;
            case '7':
                printf("%ls\n", L" Saindo do programa...");
                break; // sair do loop e do programa
            default:
                printf("%ls\n", L" Opção inválida! Tente novamente.");
                break;
        }
    } while (option != '7');

    free(client);
    return 0;
}

void printLine() {
    char list[45];
    for (int i = 0; i < 45; i++) {
        list[i] = '=';
    }
    printf("%s\n", list);
}

int getMonth() {
    struct tm *timeNow;
    time_t mytime;
    time(&mytime);
    timeNow = localtime(&mytime);
    return timeNow->tm_mon + 1;
}

int verifyPosition() {
    for (int cont = 0; cont < numberOfClients; cont++) {
        if (client[cont].enable != true)
            return cont;
    }
    return -1;
}

void expandArray(bool start) {
    numberOfClients += SIZE;
    if (start) {
        client = malloc(numberOfClients * sizeof(Client));
    } else {
        client = realloc(client, numberOfClients * sizeof(Client));
    }
    if (client == NULL) {
        printf(" Erro de alocação de memória!!!\n");
        exit(1);
    }
}

void printClient(int i) {
    printLine();
    printf("%ls %i\n", L" ID:", client[i].id);
    printf("%ls %s\n", L" Nome:", client[i].name);
    printf("%ls %i\n", L" Ano de nascimento:", client[i].birthYear);
    printf("%ls %i: %lf\n", L" Compras do mês", client[i].purchase.month, client[i].purchase.amount);
}

bool isClientEnabled(int id) {
    return client[id].enable == true;
}

void inputClientData(int count) {
    printf(" ID do Cliente: %i\n", count);
    client[count].id = count;
    client[count].enable = true;
    client[count].purchase.month = getMonth();
    client[count].purchase.amount = 0;

    printf("%ls", L" Digite o Nome: ");
    fgets(client[count].name, sizeof(client[count].name), stdin);

    printf("%ls", L" Digite o ano de nascimento: ");
    scanf("%i", &client[count].birthYear);
    setbuf(stdin, NULL);
}

void askAddPurchase(int count) {
    printf("%ls", L" Deseja adicionar compras para o cliente (S/N)? ");
    char optionPurchase;
    scanf(" %c", &optionPurchase);
    setbuf(stdin, NULL);

    if (tolower(optionPurchase) == 's') {
        getPurchase(count);
    }
}

void create() {
    int count = verifyPosition();
    if (count == -1) {
        expandArray(false);
        count = verifyPosition();
    }

    if (count != -1) {
        inputClientData(count);
        askAddPurchase(count);

        char option;
        printf("%ls", L" Deseja cadastrar um novo cliente (S/N)? ");
        scanf(" %c", &option);
        setbuf(stdin, NULL);

        if (tolower(option) == 's') {
            create();
        }
    }
}

void deleteClient() {
    int id;
    char option;
    printf("%ls", L" Qual o ID do cliente: ");
    scanf("%i", &id);
    setbuf(stdin, NULL);
    system("cls");
    
    if (isClientEnabled(id)) {
        printClient(id);
        printf("%ls", L"\n Deseja realmente remover o cliente (S/N)? ");
        scanf(" %c", &option);
        setbuf(stdin, NULL);
        if (tolower(option) == 's') {
            client[id].enable = false;
            system("cls");
        }
    } else {
        printf("%ls%i%ls\n\n", L" Cliente ID ", id, L" não encontrado!");
        system("pause");
    }
}

void getPurchase(int option) {
    if (option == -1) {
        printf("%ls", L" Digite ID do cliente: ");
        scanf("%i", &option);
        setbuf(stdin, NULL);
    }
    if (isClientEnabled(option)) {
        int currentMonth = getMonth();
        if (client[option].purchase.month == currentMonth) {
            double buy = 0;
            printf("%ls", L" Digite as compras do cliente: ");
            scanf("%lf", &buy);
            setbuf(stdin, NULL);
            client[option].purchase.amount += buy;
        } else {
            char optionReset;
            printf("%ls", L" O mês de compras do cliente é diferente do mês atual, deseja reiniciar o montante (S/N)? ");
            scanf(" %c", &optionReset);
            setbuf(stdin, NULL);
            if (tolower(optionReset) == 's') {
                client[option].purchase.month = currentMonth;
                client[option].purchase.amount = 0;
                system("cls");
                getPurchase(option);
            }
        }
    } else {
        system("cls");
        printf("%ls%i%ls\n\n", L" Cliente ID ", option, L" não encontrado!");
        system("pause");
    }
}

void resetPurchase() {
    char option;
    printf("%ls", L" Você realmente deseja reiniciar o montante de todos os clientes (S/N)? ");
    scanf(" %c", &option);
    setbuf(stdin, NULL);
    if (tolower(option) == 's') {
        int count = 0;
        int currentMonth = getMonth();
        for (int i = 0; i < numberOfClients; i++) {
            if (isClientEnabled(i)) {
                client[i].purchase.month = currentMonth;
                client[i].purchase.amount = 0;
                count++;
            }
        }
        printf(" %i%ls\n\n", count, L" clientes com montante reiniciados!");
        system("pause");
    }
}

void bestCustomer() {
    double best = 0;
    int id = -1;
    for (int i = 0; i < numberOfClients; i++) {
        if (isClientEnabled(i) && best < client[i].purchase.amount) {
            best = client[i].purchase.amount;
            id = client[i].id;
        }
    }
    if (id > -1) {
        printf("%ls\n", L" Nosso melhor cliente:");
        printClient(id);
        system("pause");
    } else {
        printf("%ls\n\n", L" Nenhuma compra cadastrada!");
        system("pause");
    }
}

void getClientById() {
    int id;
    printf("%ls", L" Digite o ID do cliente que deseja listar: ");
    scanf("%i", &id);
    setbuf(stdin, NULL);
    system("cls");
    if (isClientEnabled(id)) {
        printClient(id);
        system("pause");
    } else {
        printf("%ls%i%ls\n\n", L" Cliente ID ", id, L" não encontrado!");
        system("pause");
    }
}
