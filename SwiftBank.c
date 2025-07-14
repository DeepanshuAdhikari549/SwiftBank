#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <limits.h>

#define MAX_BRANCHES 10
#define INF INT_MAX
#define MAX_ACCOUNTS 1000
#define MAX_TRANSACTIONS 10000

// File names
#define ACCOUNTS_FILE "accounts.txt"
#define TRANSACTIONS_FILE "transactions.txt"
#define LAST_ACCOUNT_FILE "last_account_number.txt"
#define BRANCHES_FILE "branches.txt"

// UI Components ==============================================================================================================

void cls() {
    system("cls");
}

void br(int line) {
    for (int i = 0; i < line; i++) printf("\n");
}

void pre(int tab) {
    for (int i = 0; i < tab; i++) printf("\t");
}

void echo(char text[]) {
    printf("%s", text);
}

void middle1() {
    int lines = 12;
    for (int i = 0; i < lines; i++) printf("\n");
}

void ccolor(int fg, int bg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg << 4) | fg);
}

void getConsoleSize(int* width, int* height) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    } else {
        *width = 80;
        *height = 25;
    }
}

void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

void loadingbar() {
    int width, height;
    getConsoleSize(&width, &height);
    const int barLength = 50;
    cls();
    ccolor(12, 15);
    int centerX = width / 2;
    int centerY = height / 2;
    int loadingTextLen = strlen("Loading: ");
    setCursorPosition(centerX - loadingTextLen / 2, centerY - 1);
    printf("Loading: ");
    for (int i = 10; i <= 100; i += 10) {
        setCursorPosition(centerX + loadingTextLen / 2, centerY - 1);
        printf("%3d%%", i);
        setCursorPosition(centerX - barLength / 2, centerY);
        for (int j = 0; j < i * barLength / 100; j++) {
            ccolor(15, 4);  // Dark red progress
            printf(" ");
        }
        ccolor(12, 15);
        for (int j = i * barLength / 100; j < barLength; j++) {
            printf(" ");
        }
        Sleep(100);
    }
    ccolor(12, 15);
    printf("\n\n");
}

void pwelcome() {
    int width, height;
    getConsoleSize(&width, &height);
    char welcome1[] = "WELCOME TO SWIFT BANK";
    char welcome2[] = "BANK MANAGEMENT SYSTEM";
    int centerX = width / 2;
    int centerY = height / 2;
    cls();
    int y1 = centerY - 2;
    int padding1 = centerX - (int)strlen(welcome1) / 2;
    setCursorPosition(padding1, y1);
    for (int i = 0; i < (int)strlen(welcome1); i++) {
        ccolor(12, 15);
        printf("%c", welcome1[i]);
        Sleep(80);
    }
    int y2 = y1 + 2;
    int padding2 = centerX - (int)strlen(welcome2) / 2;
    setCursorPosition(padding2, y2);
    for (int i = 0; i < (int)strlen(welcome2); i++) {
        ccolor(12, 15);
        printf("%c", welcome2[i]);
        Sleep(60);
    }
    ccolor(12, 15);
    printf("\n\n");
    Sleep(1000);
}

void printCentered(const char* text, int newline) {
    int width, height;
    getConsoleSize(&width, &height);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    int y = csbi.dwCursorPosition.Y;
    int x = (width - (int)strlen(text)) / 2;
    setCursorPosition(x, y);
    printf("%s", text);
    if (newline) printf("\n");
}

void printBanner(const char* title) {
    ccolor(12, 15);
    printCentered("=========================================================================================", 1);
    printCentered("=                                                                                       =", 1);
    
    char bannerLine[100];
    snprintf(bannerLine, sizeof(bannerLine), "= %-85s =", title);
    printCentered(bannerLine, 1);
    
    printCentered("=                                                                                       =", 1);
    printCentered("=========================================================================================", 1);
    ccolor(12, 15);
}

void printWelcomeBanner() {
    printBanner("            WELCOME TO SWIFT BANK - BANK MANAGEMENT SYSTEM                             ");
}

void printAdminBanner() {
    printBanner("                               WELCOME TO SWIFT BANK - ADMIN PANEL                      ");
}

void printUserBanner() {
    printBanner("                               WELCOME TO SWIFT BANK - USER PANEL                       ");
}

// Structure for customer account
typedef struct Account {
    char name[25];
    int account;
    char phone[15];
    char address[25];
    char email[35];
    char citiz[20];
    double blnc;
    char UserID[10];
} Account;

// Structure for transaction record
typedef struct Transaction {
    int transactionId;
    int accountNumber;
    char transactionType[20]; // "Deposit", "Withdrawal", "Transfer"
    double amount;
    char date[30];
    int targetAccount; // Used for transfers
} Transaction;

// Structure for bank branch (for Dijkstra's algorithm)
typedef struct Branch {
    int id;
    char name[50];
    char location[100];
    double distance;
    int visited;
} Branch;

// Global variables
int m, r;
char id[20], password[15];

// Data structures
Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

Transaction transactions[MAX_TRANSACTIONS];
int transactionCount = 0;

Transaction transactionStack[MAX_TRANSACTIONS];
int stackTop = -1;

Transaction transactionQueue[MAX_TRANSACTIONS];
int queueFront = 0, queueRear = -1;

// Branch data for Dijkstra's algorithm
Branch branches[MAX_BRANCHES];
int branchGraph[MAX_BRANCHES][MAX_BRANCHES]; // Adjacency matrix for branch connections

// Function prototypes
void admin();
void login();
void menu();
void transaction();
void option();
void menuexit();
void chkblnc();
void deposit();
void withdrawl();
void transferMoney();
void search();
void search_acc();
void search_name();
void view();
void add();
void edit();
void del();
void about();
void displayTeamMemberDetails(int memberId);
int verify();
int chkacc(int a);
int getNextAccountNumber();

// Data structure operations
void loadAccountsFromFile();
void saveAccountsToFile();
void addAccount(Account newAccount);
Account* findAccount(int accountNumber);
void deleteAccount(int accountNumber);

// Transaction operations
void loadTransactionsFromFile();
void saveTransactionsToFile();
void addTransaction(int accountNumber, const char* type, double amount, int targetAccount);
void pushToTransactionStack(Transaction trans);
Transaction popFromTransactionStack();
void enqueueTransaction(Transaction trans);
Transaction dequeueTransaction();
void viewTransactionHistory();
void processTransactionQueue();

// Dijkstra's algorithm for finding nearest branch
void initializeBranches();
void dijkstraAlgorithm(int startBranch);
void findNearestBranch();

void securePasswordInput(char *password) {
    char ch;
    int i = 0;
    while (1) {
        ch = getch();
        if (ch == 13) {
            password[i] = '\0';
            break;
        } else if (ch == 8 && i > 0) {
            i--;
            printf("\b \b");
        } else if (i < 15 - 1 && ch >= 32 && ch <= 126) {
            password[i++] = ch;
            printf("*");
        }
    }
}

// Helper functions for UI
void printTableHeader(const char* header);
void printTableRow(const char* content);
void printSuccessMessage(const char* message);
void printErrorMessage(const char* message);
void printInfoMessage(const char* message);
void viewAccountsDetailed();
void viewFilteredTransactions();
void viewTransactionsByAccount(int account);
void viewTransactionsByType(const char* type);
void viewTransactionsByDate(const char* startDate, const char* endDate);
void enhancedMenu();
void customerManagementMenu();
void accountOperationsMenu();

int main() {
    loadingbar();
    cls();
    pwelcome();
    Sleep(1000);
    cls();
    
    // Set default color scheme
    ccolor(12, 15);
    
    // Initialize data structures
    loadAccountsFromFile();
    loadTransactionsFromFile();
    initializeBranches();
    
    int choice;
    while (1) {
        cls();
        printWelcomeBanner();
        
        br(2); printCentered("Please select an option to continue",1); Sleep(300);
        br(1); printCentered("===> 1. Login as Administrator",1); Sleep(200);
        br(1); printCentered("===> 2. Login as User",1); Sleep(200);
        br(1); printCentered("===> 3. Exit System",1); Sleep(200);
        br(2); printCentered("Enter your choice (1-3): ",0);
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                m = 1;
                admin();
                break;
            case 2:
                m = 2;
                admin();
                break;
            case 3:
                cls(); middle1();
                printCentered("Thank you for using Swift Bank!",1);
                saveAccountsToFile();
                saveTransactionsToFile();
                Sleep(1000);
                exit(0);
            default:
                cls(); middle1();
                printCentered("Invalid choice! Try again.",1);
                Sleep(1000);
        }
    }
    return 0;
}

void admin() {
    cls();
    if (m == 1) printAdminBanner();
    else printUserBanner();
    login();
    if (verify() == 1) {
        if (m == 1)
            menu();
        else
            transaction();
    } else {
        cls(); middle1();
        printCentered("Incorrect Username / Password !!!!", 1);
        Sleep(1000);
        admin();
    }
}

void login() {
    br(1); printCentered("Enter username: ", 0);
    scanf(" %[^\n]", id);
    br(1); printCentered("Enter password: ", 0);
    securePasswordInput(password);
}

int verify() {
    if (m == 1) {
        if ((strcmp(id, "admin") == 0 && strcmp(password, "shadow") == 0)) {
            cls(); middle1();
            printCentered("Login successful! Welcome, admin.", 1);
            Sleep(1000);
            return 1;
        } else
            return 0;
    } else if (m == 2) {
        if (strcmp(id, "user") == 0 && strcmp(password, "byte") == 0) {
            cls(); middle1();
            printCentered("Login successful! Welcome, user.", 1);
            Sleep(1000);
            return 1;
        } else
            return 0;
    }
    return 0;
}

void menu() {
    int choice;
    do {
        cls();
        printAdminBanner();
        br(2); printCentered("1. View Customer Accounts", 1);
        br(1); printCentered("2. Customer Account Registration", 1);
        br(1); printCentered("3. Edit Customer Account", 1);
        br(1); printCentered("4. Delete Customer Account", 1);
        br(1); printCentered("5. Search Customer Account", 1);
        br(1); printCentered("6. Transaction", 1);
        br(1); printCentered("7. View Transaction History", 1);
        br(1); printCentered("8. Process Pending Transactions", 1);
        br(1); printCentered("9. Find Nearest Branch", 1);
        br(1); printCentered("10. Log Out", 1);
        br(1); printCentered("11. About US", 1);
        br(2); printCentered("Enter your choice: ", 0);
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                view();
                break;
            case 2:
                add();
                break;
            case 3:
                edit();
                break;
            case 4:
                del();
                break;
            case 5:
                search();
                break;
            case 6:
                transaction();
                break;
            case 7:
                viewTransactionHistory();
                break;
            case 8:
                processTransactionQueue();
                break;
            case 9:
                findNearestBranch();
                break;
            case 10:
                cls(); middle1();
                printCentered("Logging out...", 1);
                Sleep(1000);
                return;
            case 11:
                about();
                break;
            default:
                cls(); middle1();
                printCentered("Invalid choice! Please try again.", 1);
                Sleep(1000);
        }
    } while (1);
}

void loadAccountsFromFile() {
    FILE *f = fopen(ACCOUNTS_FILE, "r");
    if (f == NULL) {
        return; // File doesn't exist yet
    }
    
    accountCount = 0;
    while (fscanf(f, "%25[^|]|%d|%15[^|]|%25[^|]|%35[^|]|%20[^|]|%lf|%10[^\n]\n",
                  accounts[accountCount].name,
                  &accounts[accountCount].account,
                  accounts[accountCount].phone,
                  accounts[accountCount].address,
                  accounts[accountCount].email,
                  accounts[accountCount].citiz,
                  &accounts[accountCount].blnc,
                  accounts[accountCount].UserID) == 8) {
        accountCount++;
        if (accountCount >= MAX_ACCOUNTS) break;
    }
    
    fclose(f);
}

void saveAccountsToFile() {
    FILE *f = fopen(ACCOUNTS_FILE, "w");
    if (f == NULL) {
        printf("Error opening accounts file for writing\n");
        return;
    }
    
    for (int i = 0; i < accountCount; i++) {
        fprintf(f, "%s|%d|%s|%s|%s|%s|%.2lf|%s\n",
                accounts[i].name,
                accounts[i].account,
                accounts[i].phone,
                accounts[i].address,
                accounts[i].email,
                accounts[i].citiz,
                accounts[i].blnc,
                accounts[i].UserID);
    }
    
    fclose(f);
}

void addAccount(Account newAccount) {
    if (accountCount < MAX_ACCOUNTS) {
        accounts[accountCount++] = newAccount;
    }
}

Account* findAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].account == accountNumber) {
            return &accounts[i];
        }
    }
    return NULL;
}

void deleteAccount(int accountNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].account == accountNumber) {
            // Move all subsequent accounts one position back
            for (int j = i; j < accountCount - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            accountCount--;
            return;
        }
    }
}

int getNextAccountNumber() {
    int lastAccountNumber = 100; // Default starting account number

    // Read from file
    FILE *f = fopen(LAST_ACCOUNT_FILE, "r");
    if (f != NULL) {
        fscanf(f, "%d", &lastAccountNumber);
        fclose(f);
    }

    // Increment and save
    lastAccountNumber++;
    f = fopen(LAST_ACCOUNT_FILE, "w");
    if (f != NULL) {
        fprintf(f, "%d", lastAccountNumber);
        fclose(f);
    }

    return lastAccountNumber;
}

void view() {
    cls();
    printBanner("CUSTOMERS LIST");
    
    ccolor(12, 15);
    printCentered("------------------------------------------------------------------------------------------------", 1);
    printCentered("| A/C No. | Account Name       | UserID       | Email Address         | Phone No.  | Balance   |", 1);
    printCentered("------------------------------------------------------------------------------------------------", 1);
    
    for (int i = 0; i < accountCount; i++) {
        char line[150];
        snprintf(line, sizeof(line), "| %-7d | %-18s | ", accounts[i].account, accounts[i].name);
        
        // Add UserID
        char userID[11] = {0};
        for (r = 0; r < 10; r++) {
            userID[r] = accounts[i].UserID[r] + '0';
        }
        strcat(line, userID);
        strcat(line, " | ");
        
        // Add email (truncated if too long)
        char email[21];
        strncpy(email, accounts[i].email, 20);
        email[20] = '\0';
        strcat(line, email);
        strcat(line, " | ");
        
        // Add phone
        strcat(line, accounts[i].phone);
        strcat(line, " | ");
        
        // Add balance
        char balance[15];
        snprintf(balance, sizeof(balance), "$%.2lf", accounts[i].blnc);
        strcat(line, balance);
        strcat(line, " |");
        
        printCentered(line, 1);
        printCentered("------------------------------------------------------------------------------------------------", 1);
        
        Sleep(100);
    }
    
    br(2);
    printCentered("Press any key to return to main menu...", 1);
    getch();
}

void add() {
    char c;
    int i = 0;
    
    do {
        cls();
        printBanner("CUSTOMER ACCOUNT REGISTRATION");
        
        Account newAccount;
        
        br(1); printCentered("[1] . Enter Your Name           : ", 0);
        scanf(" %[^\n]", newAccount.name);
        
        br(1); printCentered("[2] . Account Number (Auto-generated): ", 0);
        newAccount.account = getNextAccountNumber();
        printf("%d", newAccount.account);
        
        br(1); printCentered("[3] . Enter Your Phone Number   : ", 0);
        scanf(" %[^\n]", newAccount.phone);
        
        br(1); printCentered("[4] . Enter Your Address        : ", 0);
        scanf(" %[^\n]", newAccount.address);
        
        br(1); printCentered("[5] . Enter Your E-mail         : ", 0);
        scanf(" %[^\n]", newAccount.email);
        
        br(1); printCentered("[6] . Enter Your Citizenship No.: ", 0);
        scanf(" %[^\n]", newAccount.citiz);
        
        br(1); printCentered("[7] . Enter Amount To Deposit   : $", 0);
        scanf(" %lf", &newAccount.blnc);
        
        for (r = 0; r < 10; r++) {
            newAccount.UserID[r] = rand() % 10;
        }
        
        // Add account to array
        addAccount(newAccount);
        
        // Save accounts to file
        saveAccountsToFile();
        
        // Add transaction record and save it
        addTransaction(newAccount.account, "Opening Deposit", newAccount.blnc, 0);
        
        cls(); middle1();
        printCentered("CUSTOMER ACCOUNT REGISTRATION SUCCESSFUL", 1);
        
        i++;
        br(2); printCentered("Do You Want To Add Another Record ? (Y/N) : ", 0);
        scanf(" %c", &c);
    } while (c == 'Y' || c == 'y');
    
    cls(); middle1();
    printCentered("Returning to main menu...", 1);
    Sleep(1000);
}

int chkacc(int a) {
    return (findAccount(a) != NULL);
}

void edit() {
    int a;
    cls();
    printBanner("EDIT CUSTOMER ACCOUNT");
    
    br(1); printCentered("Enter Your Account Number To Edit : ", 0);
    scanf("%d", &a);
    
    Account* account = findAccount(a);
    
    if (account != NULL) {
        cls();
        printBanner("EDIT CUSTOMER ACCOUNT DETAILS");
        
        br(1); printCentered("Account Number remains    : ", 0);
        printf("%d", account->account);
        
        br(1); printCentered("[1] . Enter Your Name           : ", 0);
        scanf(" %[^\n]", account->name);
        
        br(1); printCentered("[2] . Enter Your Phone Number   : ", 0);
        scanf(" %[^\n]", account->phone);
        
        br(1); printCentered("[3] . Enter Your Address        : ", 0);
        scanf(" %[^\n]", account->address);
        
        br(1); printCentered("[4] . Enter Your E-mail         : ", 0);
        scanf(" %[^\n]", account->email);
        
        br(1); printCentered("[5] . Enter Your Citizenship    : ", 0);
        scanf(" %[^\n]", account->citiz);
        
        // Save updated accounts to file
        saveAccountsToFile();
        
        cls(); middle1();
        printCentered("CUSTOMER ACCOUNT UPDATE SUCCESSFUL", 1);
    } else {
        cls(); middle1();
        printCentered("Account Doesn't Exist", 1);
        Sleep(1000);
        edit();
        return;
    }
    
    Sleep(1000);
}

void del() {
    int a;
    cls();
    printBanner("DELETE CUSTOMER ACCOUNT");
    
    br(1); printCentered("Enter Your Account Number To Delete : ", 0);
    scanf("%d", &a);
    
    if (chkacc(a)) {
        // Delete the account
        deleteAccount(a);
        
        // Save updated accounts to file
        saveAccountsToFile();
        
        cls(); middle1();
        printCentered("CUSTOMER ACCOUNT DELETED SUCCESSFULLY", 1);
    } else {
        cls(); middle1();
        printCentered("Account Doesn't Exist", 1);
        Sleep(1000);
        del();
        return;
    }
    
    Sleep(1000);
}

void search() {
    int choice;
    cls();
    printBanner("SEARCH MENU");
    
    br(1); printCentered("1. Search By Account", 1);
    br(1); printCentered("2. Search By Name", 1);
    br(2); printCentered("Enter Your Choice [1-2] : ", 0);
    scanf("%d", &choice);
    
    if (choice == 1) {
        search_acc();
    } else if (choice == 2) {
        search_name();
    } else {
        cls(); middle1();
        printCentered("Invalid choice! Returning to menu.", 1);
        Sleep(1000);
    }
}

void search_acc() {
    int a;
    cls();
    printBanner("SEARCH CUSTOMER ACCOUNT");
    
    br(1); printCentered("Enter Your Account Number To Search : ", 0);
    scanf("%d", &a);
    
    Account* account = findAccount(a);
    
    if (account != NULL) {
        cls();
        printBanner("CUSTOMER ACCOUNT DETAILS");
        
        br(1); printCentered("Account Number : ", 0);
        printf("%d", account->account);
        
        br(1); printCentered("Name           : ", 0);
        printf("%s", account->name);
        
        br(1); printCentered("UserID         : ", 0);
        for (r = 0; r < 10; r++) {
            printf("%d", account->UserID[r]);
        }
        
        br(1); printCentered("Phone Number   : ", 0);
        printf("%s", account->phone);
        
        br(1); printCentered("Address        : ", 0);
        printf("%s", account->address);
        
        br(1); printCentered("E-mail         : ", 0);
        printf("%s", account->email);
        
        br(1); printCentered("Citizenship No : ", 0);
        printf("%s", account->citiz);
        
        br(1); printCentered("Current Balance: $", 0);
        printf("%.2lf", account->blnc);
    } else {
        cls(); middle1();
        printCentered("Account Doesn't Exist", 1);
        Sleep(1000);
        search_acc();
        return;
    }
    
    br(2); printCentered("Press any key to return to main menu...", 1);
    getch();
}

void search_name() {
    char name[25];
    int found = 0;
    
    cls();
    printBanner("SEARCH CUSTOMER BY NAME");
    
    br(1); printCentered("Enter Customer Name To Search : ", 0);
    scanf(" %[^\n]", name);
    
    ccolor(12, 15);
    printCentered("------------------------------------------------------------------------------------------------", 1);
    printCentered("| A/C No. | Account Name       | UserID       | Email Address         | Phone No.  | Balance   |", 1);
    printCentered("------------------------------------------------------------------------------------------------", 1);
    
    for (int i = 0; i < accountCount; i++) {
        // Case-insensitive search (simple approach)
        char tempName1[25], tempName2[25];
        strcpy(tempName1, accounts[i].name);
        strcpy(tempName2, name);
        for (int j = 0; tempName1[j]; j++) tempName1[j] = toupper(tempName1[j]);
        for (int j = 0; tempName2[j]; j++) tempName2[j] = toupper(tempName2[j]);
        
        if (strstr(tempName1, tempName2) != NULL) {
            found = 1;
            char line[150];
            snprintf(line, sizeof(line), "| %-7d | %-18s | ", accounts[i].account, accounts[i].name);
            
            // Add UserID
            char userID[11] = {0};
            for (r = 0; r < 10; r++) {
                userID[r] = accounts[i].UserID[r] + '0';
            }
            strcat(line, userID);
            strcat(line, " | ");
            
            // Add email (truncated if too long)
            char email[21];
            strncpy(email, accounts[i].email, 20);
            email[20] = '\0';
            strcat(line, email);
            strcat(line, " | ");
            
            // Add phone
            strcat(line, accounts[i].phone);
            strcat(line, " | ");
            
            // Add balance
            char balance[15];
            snprintf(balance, sizeof(balance), "$%.2lf", accounts[i].blnc);
            strcat(line, balance);
            strcat(line, " |");
            
            printCentered(line, 1);
            printCentered("------------------------------------------------------------------------------------------------", 1);
        }
    }
    
    if (!found) {
        cls(); middle1();
        printCentered("No matching records found!", 1);
    }
    
    br(2);
    printCentered("Press any key to return to main menu...", 1);
    getch();
}

void transaction() {
    int choice;
    do {
        cls();
        printUserBanner();
        
        br(1); printCentered("1. Check Balance", 1);
        br(1); printCentered("2. Deposit", 1);
        br(1); printCentered("3. Withdraw", 1);
        br(1); printCentered("4. Transfer Money", 1);
        br(1); printCentered("5. Back To Main Menu", 1);
        br(2); printCentered("Enter Your Choice [1-5] : ", 0);
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                chkblnc();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdrawl();
                break;
            case 4:
                transferMoney();
                break;
            case 5:
                if (m == 1) {
                    menu();
                    return;
                } else {
                    menuexit();
                    return;
                }
            default:
                cls(); middle1();
                printCentered("Invalid choice! Please try again.", 1);
                Sleep(1000);
        }
    } while (1);
}

void chkblnc() {
    int a;
    
    cls();
    printBanner("CHECK BALANCE");
    
    br(1); printCentered("Enter Your Account Number : ", 0);
    scanf("%d", &a);
    
    Account* account = findAccount(a);
    
    if (account != NULL) {
        br(2); printCentered("User Name    : ", 0);
        printf("%s", account->name);
        
        br(1); printCentered("Account No.  : ", 0);
        printf("%d", account->account);
        
        br(1); printCentered("Current Balance : $", 0);
        printf("%.2lf", account->blnc);
    } else {
        br(2); printCentered("Account doesn't exist!", 1);
    }
    
    br(2); printCentered("Press any key to return to transaction menu...", 1);
    getch();
}

void deposit() {
    int a;
    double b;
    
    cls();
    printBanner("DEPOSIT MONEY");
    
    br(1); printCentered("Enter Your Account Number : ", 0);
    scanf("%d", &a);
    
    Account* account = findAccount(a);
    
    if (account != NULL) {
        br(1); printCentered("Current Balance : $", 0);
        printf("%.2lf", account->blnc);
        
        br(1); printCentered("Enter Amount To Deposit : $", 0);
        scanf("%lf", &b);
        
        if (b > 0) {
            account->blnc += b;
            // Save updated accounts to file
            saveAccountsToFile();
            
            // Add transaction record and save it
            addTransaction(account->account, "Deposit", b, 0);
            
            br(2); printCentered("Deposit successful!", 1);
            br(1); printCentered("New Balance : $", 0);
            printf("%.2lf", account->blnc);
        } else {
            br(2); printCentered("Invalid amount! Please enter a positive value.", 1);
        }
    } else {
        br(2); printCentered("Account doesn't exist!", 1);
    }
    
    br(2); printCentered("Press any key to return to transaction menu...", 1);
    getch();
}

void withdrawl() {
    int a;
    double b;
    
    cls();
    printBanner("WITHDRAW MONEY");
    
    br(1); printCentered("Enter Your Account Number : ", 0);
    scanf("%d", &a);
    
    Account* account = findAccount(a);
    
    if (account != NULL) {
        br(1); printCentered("Current Balance : $", 0);
        printf("%.2lf", account->blnc);
        
        br(1); printCentered("Enter Amount To Withdraw : $", 0);
        scanf("%lf", &b);
        
        if (b > 0 && b <= account->blnc) {
            account->blnc -= b;
            // Save updated accounts to file
            saveAccountsToFile();
            
            // Add transaction record and save it
            addTransaction(account->account, "Withdrawal", b, 0);
            
            br(2); printCentered("Withdrawal successful!", 1);
            br(1); printCentered("New Balance : $", 0);
            printf("%.2lf", account->blnc);
        } else if (b <= 0) {
            br(2); printCentered("Invalid amount! Please enter a positive value.", 1);
        } else {
            br(2); printCentered("Insufficient funds!", 1);
        }
    } else {
        br(2); printCentered("Account doesn't exist!", 1);
    }
    
    br(2); printCentered("Press any key to return to transaction menu...", 1);
    getch();
}

void transferMoney() {
    int sourceAcc, targetAcc;
    double amount;
    
    cls();
    printBanner("TRANSFER MONEY");
    
    br(1); printCentered("Enter Your Account Number : ", 0);
    scanf("%d", &sourceAcc);
    
    Account* sourceAccount = findAccount(sourceAcc);
    
    if (sourceAccount != NULL) {
        br(1); printCentered("Current Balance : $", 0);
        printf("%.2lf", sourceAccount->blnc);
        
        br(1); printCentered("Enter Target Account Number : ", 0);
        scanf("%d", &targetAcc);
        
        if (sourceAcc == targetAcc) {
            br(2); printCentered("Cannot transfer to the same account!", 1);
        } else {
            Account* targetAccount = findAccount(targetAcc);
            
            if (targetAccount != NULL) {
                br(1); printCentered("Enter Amount To Transfer : $", 0);
                scanf("%lf", &amount);
                
                if (amount > 0 && amount <= sourceAccount->blnc) {
                    sourceAccount->blnc -= amount;
                    targetAccount->blnc += amount;
                    
                    // Save updated accounts to file
                    saveAccountsToFile();
                    
                    // Add transaction record and save it
                    addTransaction(sourceAccount->account, "Transfer", amount, targetAccount->account);
                    
                    br(2); printCentered("Transfer successful!", 1);
                    br(1); printCentered("New Balance : $", 0);
                    printf("%.2lf", sourceAccount->blnc);
                } else if (amount <= 0) {
                    br(2); printCentered("Invalid amount! Please enter a positive value.", 1);
                } else {
                    br(2); printCentered("Insufficient funds!", 1);
                }
            } else {
                br(2); printCentered("Target account doesn't exist!", 1);
            }
        }
    } else {
        br(2); printCentered("Account doesn't exist!", 1);
    }
    
    br(2); printCentered("Press any key to return to transaction menu...", 1);
    getch();
}

void loadTransactionsFromFile() {
    FILE *f = fopen(TRANSACTIONS_FILE, "r");
    if (f == NULL) {
        return; // File doesn't exist yet
    }
    
    transactionCount = 0;
    while (fscanf(f, "%d|%d|%20[^|]|%lf|%30[^|]|%d\n",
                 &transactions[transactionCount].transactionId,
                 &transactions[transactionCount].accountNumber,
                 transactions[transactionCount].transactionType,
                 &transactions[transactionCount].amount,
                 transactions[transactionCount].date,
                 &transactions[transactionCount].targetAccount) == 6) {
        transactionCount++;
        if (transactionCount >= MAX_TRANSACTIONS) break;
    }
    
    fclose(f);
}

void saveTransactionsToFile() {
    FILE *f = fopen(TRANSACTIONS_FILE, "w");
    if (f == NULL) {
        printf("Error opening transactions file for writing\n");
        return;
    }
    
    for (int i = 0; i < transactionCount; i++) {
        fprintf(f, "%d|%d|%s|%.2lf|%s|%d\n",
                transactions[i].transactionId,
                transactions[i].accountNumber,
                transactions[i].transactionType,
                transactions[i].amount,
                transactions[i].date,
                transactions[i].targetAccount);
    }
    
    fclose(f);
}

void addTransaction(int accountNumber, const char* type, double amount, int targetAccount) {
    if (transactionCount >= MAX_TRANSACTIONS) {
        printf("Transaction limit reached!\n");
        return;
    }

    time_t now;
    struct tm *timeinfo;

    time(&now);
    timeinfo = localtime(&now);

    Transaction newTransaction;
    newTransaction.transactionId = rand() % 10000 + 1000;
    newTransaction.accountNumber = accountNumber;
    strcpy(newTransaction.transactionType, type);
    newTransaction.amount = amount;
    strftime(newTransaction.date, sizeof(newTransaction.date), "%Y-%m-%d %H:%M:%S", timeinfo);
    newTransaction.targetAccount = targetAccount;

    // Add to transactions array
    transactions[transactionCount++] = newTransaction;
    
    // Save transactions to file
    saveTransactionsToFile();

    // Also push to transaction stack
    pushToTransactionStack(newTransaction);

    // for pending queue
    if (strcmp(type, "Transfer") == 0 && amount > 1000) {
        enqueueTransaction(newTransaction);
    }
}

void pushToTransactionStack(Transaction trans) {
    if (stackTop < MAX_TRANSACTIONS - 1) {
        transactionStack[++stackTop] = trans;
    }
}

Transaction popFromTransactionStack() {
    if (stackTop >= 0) {
        return transactionStack[stackTop--];
    }
    Transaction empty;
    memset(&empty, 0, sizeof(Transaction));
    return empty;
}

void enqueueTransaction(Transaction trans) {
    if (queueRear < MAX_TRANSACTIONS - 1) {
        transactionQueue[++queueRear] = trans;
    }
}

Transaction dequeueTransaction() {
    if (queueFront <= queueRear) {
        return transactionQueue[queueFront++];
    }
    Transaction empty;
    memset(&empty, 0, sizeof(Transaction));
    return empty;
}

void viewTransactionHistory() {
    cls();
    printBanner("TRANSACTION HISTORY");

    ccolor(12, 15);
    printCentered("------------------------------------------------------------------------------------------------", 1);
    printCentered("| Trans ID | Account No. | Type        | Amount    | Date & Time         | Target Account |", 1);
    printCentered("------------------------------------------------------------------------------------------------", 1);

    if (transactionCount == 0) {
        br(2); printCentered("No transaction records found!", 1);
    } else {
        for (int i = 0; i < transactionCount; i++) {
            char line[150];
            snprintf(line, sizeof(line), "| %-8d | %-11d | %-11s | $%-8.2lf | %-19s | ", 
                   transactions[i].transactionId, transactions[i].accountNumber, 
                   transactions[i].transactionType, transactions[i].amount, transactions[i].date);
            
            if (transactions[i].targetAccount > 0) {
                char target[15];
                snprintf(target, sizeof(target), "%-14d |", transactions[i].targetAccount);
                strcat(line, target);
            } else {
                strcat(line, "N/A           |");
            }
            
            printCentered(line, 1);
            printCentered("------------------------------------------------------------------------------------------------", 1);
            
            Sleep(100);
        }
    }

    br(2);
    printCentered("Press any key to return to main menu...", 1);
    getch();
}

void processTransactionQueue() {
    cls();
    printBanner("PROCESS PENDING TRANSACTIONS");

    int count = 0;

    br(1); printCentered("Processing pending transactions...", 1);

    if (queueFront > queueRear) {
        br(2); printCentered("No pending transactions in queue!", 1);
    } else {
        while (queueFront <= queueRear) {
            Transaction trans = dequeueTransaction();
            
            br(1); printCentered("Processed: ", 0);
            printf("ID %d, Account %d, Type %s, Amount $%.2lf", 
                   trans.transactionId, trans.accountNumber, 
                   trans.transactionType, trans.amount);
            
            count++;
        }
        
        br(2); printCentered("Successfully processed ", 0);
        printf("%d transaction(s)!", count);
    }

    br(2); printCentered("Press any key to return to main menu...", 1);
    getch();
}

void initializeBranches() {
    // Initialize branch data
    branches[0] = (Branch){0, "Main Branch", "Near Clock Tower, Paltan Bazaar,Dehradun", 0, 0};
    branches[1] = (Branch){1, "North Branch", "Lane no. 13, New Rd,Race Course", 0, 0};
    branches[2] = (Branch){2, "East Branch", "ISBT, Dehradun", 0, 0};
    branches[3] = (Branch){3, "South Branch", "Prem Nagar, Dehradun", 0, 0};
    branches[4] = (Branch){4, "West Branch", "Harrawalar", 0, 0};
    branches[5] = (Branch){5, "Central Branch", "Quarter Deck Road, Bharuwala Colony, Dehradun", 0, 0};
    branches[6] = (Branch){6, "Airport Branch", "Airport Road, Jauligranth, Dehradun", 0, 0};
    branches[7] = (Branch){7, "Graphic Era Branch", "Bell Road, Society Area, Dehradun", 0, 0};
    branches[8] = (Branch){8, "Morowala Branch", "Lane No. 14 ,near APS, Morowala, Dehradun", 0, 0};
    branches[9] = (Branch){9, "Industrial Branch", "IT Park Tiraha, Main Sahastradhara Road, Dehradun", 0, 0};

    // Initialize adjacency matrix with distances between branches (in km)
    // INF represents no direct connection
    for (int i = 0; i < MAX_BRANCHES; i++) {
        for (int j = 0; j < MAX_BRANCHES; j++) {
            branchGraph[i][j] = INF;
        }
    }

    // connections and distances between branches
    branchGraph[0][1] = 4;  // Main to North
    branchGraph[0][2] = 8;  // Main to East
    branchGraph[0][3] = 10;  // Main to South
    branchGraph[0][4] = 14;  // Main to West
    branchGraph[0][5] = 9;  // Main to Central
    branchGraph[1][5] = 9;  // North to Central
    branchGraph[1][6] = 26; // North to Airport
    branchGraph[2][5] = 3;  // East to Central
    branchGraph[2][7] = 5;  // East to Graphic Era University
    branchGraph[3][5] = 11;  // South to Central
    branchGraph[3][8] = 11;  // South to Morowala
    branchGraph[4][5] = 15;  // West to Central
    branchGraph[4][9] = 17;  // West to IT Park
    branchGraph[5][6] = 31;  // Central to Airport
    branchGraph[5][7] = 3;  // Central to University
    branchGraph[5][8] = 6;  // Central to Morowala
    branchGraph[5][9] = 17;  // Central to IT Park
    branchGraph[6][7] = 32; // Airport to University
    branchGraph[7][8] = 7;  // University to Morowala
    branchGraph[8][9] = 15;  // Morowala to IT Park

    for (int i = 0; i < MAX_BRANCHES; i++) {
        for (int j = 0; j < MAX_BRANCHES; j++) {
            if (branchGraph[i][j] != INF) {
                branchGraph[j][i] = branchGraph[i][j];
            }
        }
    }
}

void dijkstraAlgorithm(int startBranch) {
    // Initialize all branches
    for (int i = 0; i < MAX_BRANCHES; i++) {
        branches[i].distance = INF;
        branches[i].visited = 0;
    }

    // Distance to start branch is 0
    branches[startBranch].distance = 0;

    // Process all branches
    for (int count = 0; count < MAX_BRANCHES - 1; count++) {
        // Find branch with minimum distance
        int minDist = INF;
        int minIndex = -1;
        
        for (int i = 0; i < MAX_BRANCHES; i++) {
            if (!branches[i].visited && branches[i].distance <= minDist) {
                minDist = branches[i].distance;
                minIndex = i;
            }
        }
        
        // Mark the branch as visited
        branches[minIndex].visited = 1;
        
        // Update distances to adjacent branches
        for (int i = 0; i < MAX_BRANCHES; i++) {
            if (!branches[i].visited && 
                branchGraph[minIndex][i] != INF && 
                branches[minIndex].distance != INF &&
                branches[minIndex].distance + branchGraph[minIndex][i] < branches[i].distance) {
                branches[i].distance = branches[minIndex].distance + branchGraph[minIndex][i];
            }
        }
    }
}

void findNearestBranch() {
    int userPos, i;

    cls();
    printBanner("FIND NEAREST BRANCH");

    br(1); printCentered("Select Your Current Position:", 1);

    for (i = 0; i < MAX_BRANCHES; i++) {
        char line[150];
        snprintf(line, sizeof(line), "[%d] %s - %s", i, branches[i].name, branches[i].location);
        br(1); printCentered(line, 1);
    }

    br(1); printCentered("Enter your choice (0-9): ", 0);
    scanf("%d", &userPos);

    if (userPos < 0 || userPos >= MAX_BRANCHES) {
        cls(); middle1();
        printCentered("Invalid selection!", 1);
    } else {
        // Run Dijkstra's algorithm to find shortest paths
        dijkstraAlgorithm(userPos);
        
        cls();
        printBanner("BRANCH DISTANCES");
        
        br(1); printCentered("Distances from ", 0);
        printf("%s:", branches[userPos].name);
        
        int y = 3;
        // Display branches sorted by distance
        for (int dist = 0; dist < 100; dist++) {
            for (i = 0; i < MAX_BRANCHES; i++) {
                if (i != userPos && branches[i].distance == dist) {
                    char line[150];
                    snprintf(line, sizeof(line), "%s - %s: %d km", branches[i].name, branches[i].location, dist);
                    br(1); printCentered(line, 1);
                    y++;
                }
            }
        }
        
        // Find and display the nearest branch
        int nearest = -1;
        int minDist = INF;
        
        for (i = 0; i < MAX_BRANCHES; i++) {
            if (i != userPos && branches[i].distance < minDist) {
                minDist = branches[i].distance;
                nearest = i;
            }
        }
        
        if (nearest != -1) {
            br(2); printCentered("The nearest branch is: ", 0);
            printf("%s - %s (%d km)", 
                   branches[nearest].name, branches[nearest].location, minDist);
        }
    }

    br(2); printCentered("Press any key to return to main menu...", 1);
    getch();
}

void menuexit() {
    cls(); middle1();
    printCentered("THANK YOU FOR USING SWIFT BANK", 1);
    br(1); printCentered("GOODBYE!", 1);
    saveAccountsToFile();
    saveTransactionsToFile();
    Sleep(1000);
    exit(0);
}

void displayTeamMemberDetails(int memberId) {
    cls();
    printBanner("TEAM MEMBER DETAILS");
    br(2);
    switch (memberId) {
        case 1:
            printCentered("Name: Rohit Pant", 1);
            printCentered("Section: D", 1);
            printCentered("Role: Project Lead", 1);
            break;
        case 2:
            printCentered("Name: Deepanshu Adhikari", 1);
            printCentered("Section: B", 1);
            printCentered("Role: Lead Developer", 1);
            break;
        case 3:
            printCentered("Name: Anamika", 1);
            printCentered("Section: C", 1);
            printCentered("Role: UI/UX Designer", 1);
            break;
        case 4:
            printCentered("Name: Anisha", 1);
            printCentered("Section: A", 1);
            printCentered("Role: Quality Assurance", 1);
            break;
        default:
            printCentered("Invalid member number.", 1);
            break;
    }
    br(2);
    printCentered("Press any key to return to About Us...", 1);
    getch();
}

void about() {
    int memberChoice;
    do {
        cls();
        printBanner("ABOUT SWIFT BANK");

        br(1); printCentered("Swift Banking System is a comprehensive banking management solution", 1);
        br(1); printCentered("designed to streamline banking operations while providing an intuitive", 1);
        br(1); printCentered("user experience. This system incorporates advanced features including:", 1);

        br(1); printCentered("- Customer account management with secure user authentication", 1);
        br(1); printCentered("- Complete transaction processing with history tracking", 1);
        br(1); printCentered("- Efficient data structures including linked lists, stacks, and queues", 1);
        br(1); printCentered("- Branch location services using Dijkstra's shortest path algorithm", 1);

        br(1); printCentered("Developed by: Team Swift - Bank Management System", 1);
        br(1); printCentered("-----------------------------------------------------", 1);
        br(1); printCentered("Team Members:", 1);
        br(1); printCentered("1: Rohit Pant Section D", 1);
        br(1); printCentered("2: Deepanshu Adhikari Section B", 1);
        br(1); printCentered("3: Anamika Section C", 1);
        br(1); printCentered("4: Anisha Section A", 1);
        br(1); printCentered("5: Back to Main Menu", 1);
        br(1); printCentered("-----------------------------------------------------", 1);

        br(2); printCentered("Enter a number to view member details (1-4) or 5 to go back: ", 0);
        scanf("%d", &memberChoice);

        if (memberChoice >= 1 && memberChoice <= 4) {
            displayTeamMemberDetails(memberChoice);
        } else if (memberChoice == 5) {
            return; // Exit the about function
        } else {
            cls();
            middle1();
            printCentered("Invalid choice! Please enter a number between 1 and 5.", 1);
            Sleep(1000);
        }
    } while (1); // Loop until user chooses to go back
}

// Additional helper functions for UI consistency
void printTableHeader(const char* header) {
    printBanner(header);
}

void printTableRow(const char* content) {
    printCentered(content, 1);
    printCentered("--------------------------------------------------------------------------------------------------------------------", 1); // Adjusted length
}

void printSuccessMessage(const char* message) {
    cls(); 
    middle1();
    printCentered(message, 1);
    Sleep(1000);
}

void printErrorMessage(const char* message) {
    cls();
    middle1();
    printCentered(message, 1);
    Sleep(1000);
}

void printInfoMessage(const char* message) {
    cls();
    middle1();
    printCentered(message, 1);
    Sleep(1000);
}

// Enhanced account view with better formatting
void viewAccountsDetailed() {
    cls();
    printBanner("CUSTOMER ACCOUNTS DETAILED VIEW");
    
    int count = 0;
    
    ccolor(12, 15);
    printCentered("--------------------------------------------------------------------------------------------------------------------", 1);
    printCentered("| ID  | Name                | Account No. | Balance    | Phone        | Email                     | Address          |", 1);
    printCentered("--------------------------------------------------------------------------------------------------------------------", 1);
    
    for (int i = 0; i < accountCount && count < 20; i++) {
        char line[200];
        snprintf(line, sizeof(line), 
                "| %-3d | %-19s | %-11d | $%-9.2lf | %-12s | %-25s | %-16s |",
                ++count, accounts[i].name, accounts[i].account, accounts[i].blnc, 
                accounts[i].phone, accounts[i].email, accounts[i].address);
        
        printTableRow(line);
    }
    
    if (accountCount > 20) {
        printCentered("(More accounts available - not all displayed)", 1);
    }
    
    br(2);
    printCentered("Press any key to return to main menu...", 1);
    getch();
}

// Enhanced transaction view with filtering options
void viewFilteredTransactions() {
    cls();
    printBanner("FILTER TRANSACTIONS");
    
    br(1); printCentered("1. View All Transactions", 1);
    br(1); printCentered("2. Filter by Account", 1);
    br(1); printCentered("3. Filter by Type", 1);
    br(1); printCentered("4. Filter by Date Range", 1);
    br(1); printCentered("5. Cancel", 1);
    br(2); printCentered("Enter your choice: ", 0);
    
    int choice;
    scanf("%d", &choice);
    
    switch (choice) {
        case 1:
            viewTransactionHistory();
            break;
        case 2: {
            int account;
            br(1); printCentered("Enter Account Number: ", 0);
            scanf("%d", &account);
            viewTransactionsByAccount(account);
            break;
        }
        case 3: {
            char type[20];
            br(1); printCentered("Enter Transaction Type (Deposit/Withdrawal/Transfer): ", 0);
            scanf("%19s", type);
            viewTransactionsByType(type);
            break;
        }
        case 4: {
            char startDate[20], endDate[20];
            br(1); printCentered("Enter Start Date (YYYY-MM-DD): ", 0);
            scanf("%19s", startDate);
            br(1); printCentered("Enter End Date (YYYY-MM-DD): ", 0);
            scanf("%19s", endDate);
            viewTransactionsByDate(startDate, endDate);
            break;
        }
        case 5:
            return;
        default:
            printErrorMessage("Invalid choice!");
            viewFilteredTransactions();
            return;
    }
}

void viewTransactionsByAccount(int account) {
    cls();
    char header[100];
    snprintf(header, sizeof(header), "TRANSACTIONS FOR ACCOUNT %d", account);
    printBanner(header);
    
    int count = 0;
    
    ccolor(12, 15);
    printCentered("--------------------------------------------------------------------------------", 1);
    printCentered("| ID       | Type        | Amount    | Date                | Target Account |", 1);
    printCentered("--------------------------------------------------------------------------------", 1);
    
    for (int i = 0; i < transactionCount; i++) {
        if (transactions[i].accountNumber == account) {
            char line[150];
            snprintf(line, sizeof(line), 
                    "| %-8d | %-11s | $%-8.2lf | %-19s | %-14d |",
                    transactions[i].transactionId, transactions[i].transactionType, 
                    transactions[i].amount, transactions[i].date, transactions[i].targetAccount);
            
            printTableRow(line);
            count++;
        }
    }
    
    if (count == 0) {
        printErrorMessage("No transactions found for this account!");
        return;
    }
    
    br(2);
    printCentered("Press any key to return...", 1);
    getch();
}

void viewTransactionsByType(const char* type) {
    cls();
    char header[100];
    snprintf(header, sizeof(header), "%s TRANSACTIONS", type);
    printBanner(header);
    
    int count = 0;
    
    ccolor(12, 15);
    printCentered("--------------------------------------------------------------------------------", 1);
    printCentered("| ID       | Account     | Amount    | Date                | Target Account |", 1);
    printCentered("--------------------------------------------------------------------------------", 1);
    
    for (int i = 0; i < transactionCount; i++) {
        if (strcasecmp(transactions[i].transactionType, type) == 0) {
            char line[150];
            snprintf(line, sizeof(line), 
                    "| %-8d | %-11d | $%-8.2lf | %-19s | %-14d |",
                    transactions[i].transactionId, transactions[i].accountNumber,
                    transactions[i].amount, transactions[i].date, transactions[i].targetAccount);
            
            printTableRow(line);
            count++;
        }
    }
    
    if (count == 0) {
        printErrorMessage("No transactions found of this type!");
        return;
    }
    
    br(2);
    printCentered("Press any key to return...", 1);
    getch();
}

void viewTransactionsByDate(const char* startDate, const char* endDate) {
    cls();
    char header[150];
    snprintf(header, sizeof(header), "TRANSACTIONS FROM %s TO %s", startDate, endDate);
    printBanner(header);
    
    int count = 0;
    
    ccolor(12, 15);
    printCentered("------------------------------------------------------------------------------------------", 1);
    printCentered("| ID       | Account     | Type        | Amount    | Date                | Target Acc |", 1);
    printCentered("------------------------------------------------------------------------------------------", 1);
    
    for (int i = 0; i < transactionCount; i++) {
        // Simple date comparison (for proper implementation would need date parsing)
        if (strncmp(transactions[i].date, startDate, 10) >= 0 && 
            strncmp(transactions[i].date, endDate, 10) <= 0) {
            char line[150];
            snprintf(line, sizeof(line), 
                    "| %-8d | %-11d | %-11s | $%-8.2lf | %-19s | %-10d |",
                    transactions[i].transactionId, transactions[i].accountNumber,
                    transactions[i].transactionType, transactions[i].amount, 
                    transactions[i].date, transactions[i].targetAccount);
            
            printTableRow(line);
            count++;
        }
    }
    
    if (count == 0) {
        printErrorMessage("No transactions found in this date range!");
        return;
    }
    
    br(2);
    printCentered("Press any key to return...", 1);
    getch();
}

// Enhanced main menu with additional options
void enhancedMenu() {
    int choice;
    do {
        cls();
        printAdminBanner();
        
        br(1); printCentered("1. Customer Management", 1);
        br(1); printCentered("2. Account Operations", 1);
        br(1); printCentered("3. Transaction Processing", 1);
        br(1); printCentered("4. Reports and Analytics", 1);
        br(1); printCentered("5. Branch Services", 1);
        br(1); printCentered("6. System Settings", 1);
        br(1); printCentered("7. Logout", 1);
        br(2); printCentered("Enter your choice: ", 0);
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                customerManagementMenu();
                break;
            case 2:
                accountOperationsMenu();
                break;
            case 3:
                viewFilteredTransactions();
                break;
            case 4:
                viewAccountsDetailed();
                break;
            case 5:
                findNearestBranch();
                break;
            case 6:
                about();
                break;
            case 7:
                cls(); middle1();
                printCentered("Logging out...", 1);
                Sleep(1000);
                return;
            default:
                printErrorMessage("Invalid choice! Please try again.");
        }
    } while (1);
}

void customerManagementMenu() {
    int choice;
    do {
        cls();
        printBanner("CUSTOMER MANAGEMENT");
        
        br(1); printCentered("1. View All Customers", 1);
        br(1); printCentered("2. Add New Customer", 1);
        br(1); printCentered("3. Edit Customer", 1);
        br(1); printCentered("4. Delete Customer", 1);
        br(1); printCentered("5. Search Customer", 1);
        br(1); printCentered("6. Back to Main Menu", 1);
        br(2); printCentered("Enter your choice: ", 0);
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                viewAccountsDetailed();
                break;
            case 2:
                add();
                break;
            case 3:
                edit();
                break;
            case 4:
                del();
                break;
            case 5:
                search();
                break;
            case 6:
                return;
            default:
                printErrorMessage("Invalid choice!");
        }
    } while (1);
}

void accountOperationsMenu() {
    int choice;
    do {
        cls();
        printBanner("ACCOUNT OPERATIONS");
        
        br(1); printCentered("1. Check Balance", 1);
        br(1); printCentered("2. Deposit Funds", 1);
        br(1); printCentered("3. Withdraw Funds", 1);
        br(1); printCentered("4. Transfer Funds", 1);
        br(1); printCentered("5. Account Statement", 1);
        br(1); printCentered("6. Back to Main Menu", 1);
        br(2); printCentered("Enter your choice: ", 0);
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                chkblnc();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdrawl();
                break;
            case 4:
                transferMoney();
                break;
            case 5:
                viewFilteredTransactions();
                break;
            case 6:
                return;
            default:
                printErrorMessage("Invalid choice!");
        }
    } while (1);
}