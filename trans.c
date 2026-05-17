// Banking System Mini Project
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ================= STRUCTURES =================

struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
    char aadhar[15];
};

struct transactionData
{
    unsigned int acctNum;
    double amount;
    char type; // C = Credit, D = Debit
    char dateTime[25];
};

// ================= FUNCTION PROTOTYPES =================

unsigned int enterChoice(void);

void newRecord(FILE *fPtr);
void deposit(FILE *fPtr);
void withdraw(FILE *fPtr);
void deleteRecord(FILE *fPtr);

void checkBalance(FILE *fPtr);
void searchByName(FILE *fPtr);

void showTransactionsByAccount(void);
void miniStatement(void);

void applyInterest(FILE *fPtr);
void showAllAccounts(FILE *fPtr);

void getDateTime(char *buffer);

// ================= MAIN FUNCTION =================

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // Open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File not found!\n");
        printf("Create credit.dat first.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 11)
    {
        switch (choice)
        {
            case 1:
                newRecord(cfPtr);
                break;

            case 2:
                deposit(cfPtr);
                break;

            case 3:
                withdraw(cfPtr);
                break;

            case 4:
                deleteRecord(cfPtr);
                break;

            case 5:
                checkBalance(cfPtr);
                break;

            case 6:
                searchByName(cfPtr);
                break;

            case 7:
                showTransactionsByAccount();
                break;

            case 8:
                miniStatement();
                break;

            case 9:
                applyInterest(cfPtr);
                break;

            case 10:
                showAllAccounts(cfPtr);
                break;

            default:
                printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);

    return 0;
}

// ================= DATE TIME =================

void getDateTime(char *buffer)
{
    time_t t = time(NULL);

    struct tm *tm_info = localtime(&t);

    sprintf(buffer,
            "%02d/%02d/%04d %02d:%02d:%02d",
            tm_info->tm_mday,
            tm_info->tm_mon + 1,
            tm_info->tm_year + 1900,
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec);
}

// ================= ADD ACCOUNT =================

void newRecord(FILE *fPtr)
{
    struct clientData client = {0};

    unsigned int acc;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(client), SEEK_SET);

    fread(&client, sizeof(client), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists\n");
        return;
    }

    printf("Enter lastname firstname balance aadhaar:\n");

    scanf("%s %s %lf %s",
          client.lastName,
          client.firstName,
          &client.balance,
          client.aadhar);

    client.acctNum = acc;

    fseek(fPtr, (acc - 1) * sizeof(client), SEEK_SET);

    fwrite(&client, sizeof(client), 1, fPtr);

    printf("Account created successfully!\n");
}

// ================= DEPOSIT =================

void deposit(FILE *fPtr)
{
    struct clientData client;

    unsigned int acc;

    double amount;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(client), SEEK_SET);

    fread(&client, sizeof(client), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);

    client.balance += amount;

    FILE *tPtr = fopen("transactions.dat", "ab");

    if (tPtr)
    {
        struct transactionData t;

        char dt[25];

        getDateTime(dt);

        t.acctNum = acc;
        t.amount = amount;
        t.type = 'C';

        strcpy(t.dateTime, dt);

        fwrite(&t, sizeof(t), 1, tPtr);

        fclose(tPtr);
    }

    fseek(fPtr, -(long)sizeof(client), SEEK_CUR);

    fwrite(&client, sizeof(client), 1, fPtr);

    printf("Deposit successful!\n");
    printf("Balance: %.2f\n", client.balance);
}

// ================= WITHDRAW =================

void withdraw(FILE *fPtr)
{
    struct clientData client;

    unsigned int acc;

    double amount;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(client), SEEK_SET);

    fread(&client, sizeof(client), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);

    if (client.balance < amount)
    {
        printf("Insufficient balance!\n");
        return;
    }

    client.balance -= amount;

    FILE *tPtr = fopen("transactions.dat", "ab");

    if (tPtr)
    {
        struct transactionData t;

        char dt[25];

        getDateTime(dt);

        t.acctNum = acc;
        t.amount = amount;
        t.type = 'D';

        strcpy(t.dateTime, dt);

        fwrite(&t, sizeof(t), 1, tPtr);

        fclose(tPtr);
    }

    fseek(fPtr, -(long)sizeof(client), SEEK_CUR);

    fwrite(&client, sizeof(client), 1, fPtr);

    printf("Withdraw successful!\n");
    printf("Balance: %.2f\n", client.balance);
}

// ================= DELETE ACCOUNT =================

void deleteRecord(FILE *fPtr)
{
    struct clientData blank = {0};

    unsigned int acc;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(blank), SEEK_SET);

    fwrite(&blank, sizeof(blank), 1, fPtr);

    printf("Account deleted successfully!\n");
}

// ================= CHECK BALANCE =================

void checkBalance(FILE *fPtr)
{
    struct clientData client;

    unsigned int acc;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(client), SEEK_SET);

    fread(&client, sizeof(client), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
        return;
    }

    printf("\n===== ACCOUNT DETAILS =====\n");

    printf("Account Number : %u\n", client.acctNum);
    printf("Name           : %s %s\n",
           client.firstName,
           client.lastName);

    printf("Balance        : %.2f\n", client.balance);

    printf("Aadhaar        : %s\n", client.aadhar);
}

// ================= SEARCH BY NAME =================

void searchByName(FILE *fPtr)
{
    struct clientData client;

    char name[15];

    int found = 0;

    printf("Enter last name: ");
    scanf("%s", name);

    rewind(fPtr);

    while (fread(&client, sizeof(client), 1, fPtr))
    {
        if (strcmp(client.lastName, name) == 0 &&
            client.acctNum != 0)
        {
            printf("\nFound Record:\n");

            printf("%u %s %s %.2f %s\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance,
                   client.aadhar);

            found = 1;
        }
    }

    if (!found)
    {
        printf("No record found\n");
    }
}

// ================= SHOW TRANSACTIONS =================

void showTransactionsByAccount()
{
    FILE *tPtr = fopen("transactions.dat", "rb");

    struct transactionData t;

    unsigned int acc;

    if (!tPtr)
    {
        printf("No transactions found\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%u", &acc);

    printf("\n===== TRANSACTIONS =====\n");

    while (fread(&t, sizeof(t), 1, tPtr))
    {
        if (t.acctNum == acc)
        {
            printf("%c  %.2f  %s\n",
                   t.type,
                   t.amount,
                   t.dateTime);
        }
    }

    fclose(tPtr);
}

// ================= MINI STATEMENT =================

void miniStatement()
{
    FILE *tPtr = fopen("transactions.dat", "rb");

    struct transactionData t[100];

    int count = 0;

    int i;

    unsigned int acc;

    if (!tPtr)
    {
        printf("No transactions found\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%u", &acc);

    while (fread(&t[count], sizeof(t[0]), 1, tPtr))
    {
        if (t[count].acctNum == acc)
        {
            count++;
        }
    }

    printf("\n===== LAST 5 TRANSACTIONS =====\n");

    for (i = count - 1;
         i >= 0 && i > count - 6;
         i--)
    {
        printf("%c  %.2f  %s\n",
               t[i].type,
               t[i].amount,
               t[i].dateTime);
    }

    fclose(tPtr);
}

// ================= APPLY INTEREST =================

void applyInterest(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    while (fread(&client, sizeof(client), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            client.balance += client.balance * 0.05;

            fseek(fPtr,
                   -(long)sizeof(client),
                   SEEK_CUR);

            fwrite(&client, sizeof(client), 1, fPtr);
        }
    }

    printf("5%% interest applied successfully!\n");
}

// ================= SHOW ALL ACCOUNTS =================

void showAllAccounts(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n===== ALL ACCOUNTS =====\n");

    while (fread(&client, sizeof(client), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%u %s %s %.2f %s\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance,
                   client.aadhar);
        }
    }
}

// ================= MENU =================

unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n");
    printf("1  - Add Account\n");
    printf("2  - Deposit\n");
    printf("3  - Withdraw\n");
    printf("4  - Delete Account\n");
    printf("5  - Check Balance\n");
    printf("6  - Search By Name\n");
    printf("7  - Show Transactions\n");
    printf("8  - Mini Statement\n");
    printf("9  - Apply Interest\n");
    printf("10 - Show All Accounts\n");
    printf("11 - Exit\n");

    printf("Enter your choice: ");

    scanf("%u", &choice);

    return choice;
}