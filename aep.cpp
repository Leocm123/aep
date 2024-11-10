#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>  // Biblioteca para suporte de caracteres especiais da l�ngua portuguesa

#define MAX_USERS 100
#define BUFFER_SIZE 256

typedef struct {
    char username[50];
    char password[50];
} User;

// Fun��o para criptografar uma string (usa uma substitui��o simples com deslocamento de 5)
void encrypt(char *str) {
    while (*str) {
        *str = (*str + 5) % 126;  // Criptografa cada caractere com deslocamento de 5
        str++;
    }
}

// Fun��o para descriptografar uma string (inverte o deslocamento da criptografia)
void decrypt(char *str) {
    while (*str) {
        *str = (*str - 5 + 126) % 126;  // Reverte a criptografia
        str++;
    }
}

// Fun��o para salvar os usu�rios em dois arquivos (criptografado e descriptografado)
void saveUsersToFile(User users[], int count) {
    FILE *fileEncrypted = fopen("users_encrypted.txt", "w");
    FILE *fileDecrypted = fopen("users_decrypted.txt", "w");

    if (fileEncrypted == NULL || fileDecrypted == NULL) {
        printf("Erro ao abrir os arquivos.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        encrypt(users[i].username);
        encrypt(users[i].password);
        fprintf(fileEncrypted, "%s %s\n", users[i].username, users[i].password);

        decrypt(users[i].username);
        decrypt(users[i].password);
        fprintf(fileDecrypted, "%s %s\n", users[i].username, users[i].password);
    }

    fclose(fileEncrypted);
    fclose(fileDecrypted);
}

// Fun��o para carregar os usu�rios do arquivo criptografado e descriptograf�-los
int loadUsersFromFile(User users[]) {
    FILE *file = fopen("users_encrypted.txt", "r");

    if (file == NULL) {
        return 0;
    }

    int count = 0;
    while (fscanf(file, "%s %s", users[count].username, users[count].password) != EOF) {
        decrypt(users[count].username);
        decrypt(users[count].password);
        count++;
    }

    fclose(file);
    return count;
}

// Fun��o para verificar se a senha cont�m apenas letras e n�meros
int isValidPassword(char *password) {
    for (int i = 0; password[i] != '\0'; i++) {
        if (!isalnum(password[i])) {
            return 0;
        }
    }
    return 1;
}

// Fun��o para adicionar um novo usu�rio
void addUser(User users[], int *count) {
    if (*count >= MAX_USERS) {
        printf("N�mero m�ximo de usu�rios atingido.\n");
        return;
    }

    User newUser;
    printf("\n============================\n");
    printf("     Adicionar Usu�rio\n");
    printf("============================\n");

    printf("Digite o nome de usu�rio: ");
    scanf("%s", newUser.username);

    printf("Digite a senha (apenas letras e n�meros): ");
    scanf("%s", newUser.password);

    if (!isValidPassword(newUser.password)) {
        printf("Erro: A senha deve conter apenas letras e n�meros.\n");
        return;
    }

    users[*count] = newUser;
    (*count)++;
    saveUsersToFile(users, *count);

    printf("Usu�rio adicionado com sucesso.\n");
}

// Fun��o para atualizar a senha de um usu�rio existente
void updateUser(User users[], int count) {
    char username[50];
    printf("\n============================\n");
    printf("    Atualizar Usu�rio\n");
    printf("============================\n");

    printf("Digite o nome de usu�rio a ser atualizado: ");
    scanf("%s", username);

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("Digite a nova senha (apenas letras e n�meros): ");
            scanf("%s", users[i].password);

            if (!isValidPassword(users[i].password)) {
                printf("Erro: A senha deve conter apenas letras e n�meros.\n");
                return;
            }

            saveUsersToFile(users, count);
            printf("Senha atualizada com sucesso.\n");
            return;
        }
    }

    printf("Usu�rio n�o encontrado.\n");
}

// Fun��o para excluir um usu�rio da lista
void deleteUser(User users[], int *count) {
    char username[50];
    printf("\n============================\n");
    printf("     Excluir Usu�rio\n");
    printf("============================\n");

    printf("Digite o nome de usu�rio a ser exclu�do: ");
    scanf("%s", username);

    for (int i = 0; i < *count; i++) {
        if (strcmp(users[i].username, username) == 0) {
            for (int j = i; j < *count - 1; j++) {
                users[j] = users[j + 1];
            }
            (*count)--;

            saveUsersToFile(users, *count);
            printf("Usu�rio exclu�do com sucesso.\n");
            return;
        }
    }

    printf("Usu�rio n�o encontrado.\n");
}

// Fun��o para listar os usu�rios com suas senhas, mostrando os dados originais e criptografados
void listUsersWithEncryptedPasswords(User users[], int count) {
    printf("\n============================\n");
    printf("   Usu�rios e Senhas\n");
    printf("============================\n");

    for (int i = 0; i < count; i++) {
        char encryptedUsername[50];
        char encryptedPassword[50];

        strcpy(encryptedUsername, users[i].username);
        strcpy(encryptedPassword, users[i].password);

        encrypt(encryptedUsername);
        encrypt(encryptedPassword);

        printf("Usu�rio: %s | Senha: %s\n", users[i].username, users[i].password);
        printf("Usu�rio Criptografado: %s | Senha Criptografada: %s\n", encryptedUsername, encryptedPassword);
    }
}

// Fun��o para testar a descriptografia
void testDecryption() {
    char encryptedPassword[50];
    printf("\n============================\n");
    printf("     Testar Descriptografia\n");
    printf("============================\n");

    printf("Digite a senha criptografada: ");
    scanf("%s", encryptedPassword);

    decrypt(encryptedPassword);
    printf("Senha original: %s\n", encryptedPassword);
}

// Fun��o principal que gerencia o menu do sistema
int main() {
    setlocale(LC_ALL, "Portuguese");

    User users[MAX_USERS];
    int userCount = loadUsersFromFile(users);
    int choice;

    do {
        printf("\n====================================\n");
        printf("      Sistema de Gest�o de Usu�rios\n");
        printf("====================================\n");
        printf("1. Adicionar Usu�rio\n");
        printf("2. Alterar Usu�rio\n");
        printf("3. Excluir Usu�rio\n");
        printf("4. Listar Usu�rios\n");
        printf("5. Ver Detalhes dos Usu�rios (incluindo senhas e criptografia)\n");
        printf("6. Testar Descriptografia\n");
        printf("7. Sair\n");
        printf("====================================\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addUser(users, &userCount);
                break;
            case 2:
                updateUser(users, userCount);
                break;
            case 3:
                deleteUser(users, &userCount);
                break;
            case 4:
                listUsersWithEncryptedPasswords(users, userCount);
                break;
            case 5:
                listUsersWithEncryptedPasswords(users, userCount);
                break;
            case 6:
                testDecryption();
                break;
            case 7:
                printf("Obrigado por usar o sistema!\n");
                printf("Desenvolvido por: Victor, Leonardo e Bruno\n");
                break;
            default:
                printf("Op��o inv�lida.\n");
        }
    } while (choice != 7);

    return 0;
}

