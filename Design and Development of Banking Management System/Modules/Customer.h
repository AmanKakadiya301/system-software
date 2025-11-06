// Copied with minimal naming changes from the reference, following same structure
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>

void customerMenu(int connectionFD);
int loginCustomer(int connectionFD, int accountNumber, char *password);
void withdrawMoney(int connectionFD, int accountNumber);
void depositMoney(int connectionFD, int accountNumber);
void customerBal(int connectionFD, int accountNumber);
void applyLoan(int connectionFD, int accountNumber);
void transferFunds(int connectionFD, int accountNumber, int destAcc, float amt);
void addFeedback(int connectionFD);
void transactionHistory(int connectionFD, int accountNumber);
int changePassword(int connectionFD, int accountNumber);
void logout(int connectionFD, int id);

int writeBytes, readBytes, key, loginOffset;
// Shared buffers are defined once in server.c
extern char readBuffer[4096], writeBuffer[4096];



void customerMenu(int connectionFD){
    struct Customer newCustomer;
    int accountNumber;
    int destAcc;
    int response = 0;
    char password[20];
    char customerName[20];
    char newPassword[20];
    float amount;

label1:
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "\nAcc: ");
    writeBytes = write(connectionFD, writeBuffer, sizeof(writeBuffer));
    if(writeBytes == -1)
    {
        printf("Unable to send data\n");
    }
    else
    {
        // read account number
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFD, readBuffer, sizeof(readBuffer));
        if(readBytes == -1)
        {
            printf("Unable to read data\n");
        }
        else
        {
            accountNumber = atoi(readBuffer);
        }
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer,  "Password: ");
    writeBytes = write(connectionFD, writeBuffer, sizeof(writeBuffer));
    if(writeBytes == -1)
    {
        printf("Unable to send data\n");
    }
    else
    {
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFD, readBuffer, sizeof(readBuffer));
        strcpy(password, readBuffer);

        int loginResultFirst = loginCustomer(connectionFD, accountNumber, password);
        if (loginResultFirst == 1)
        {            
            while(1)
            {   
                bzero(writeBuffer, sizeof(writeBuffer));
                strcpy(writeBuffer, CUSMENU);
                writeBytes = write(connectionFD, writeBuffer, sizeof(writeBuffer));
                if(writeBytes == -1)
                {
                    printf("Unable to write to client\n");
                }
                else
                {
                    printf("Menu sent to the client\n");
                    bzero(readBuffer, sizeof(readBuffer));
                    int readBytes = read(connectionFD, readBuffer, sizeof(readBuffer));
                    if(readBytes == -1)
                    {
                        printf("Unable to read client data\n");
                    }
                    else
                    {
                        int choice = atoi(readBuffer);
                        printf("Customer Entered: %d\n", choice);

                        switch(choice)
                        {
                            case 1:
                                depositMoney(connectionFD, accountNumber);
                                break;
                            case 2:
                                withdrawMoney(connectionFD, accountNumber);
                                break;
                            case 3:
                                customerBal(connectionFD, accountNumber);;
                                break;
                            case 4:
                                applyLoan(connectionFD, accountNumber);
                                break;
                            case 5:
                                bzero(writeBuffer, sizeof(writeBuffer));
                                strcpy(writeBuffer, "To: ");
                                write(connectionFD, writeBuffer, sizeof(writeBuffer));

                                bzero(readBuffer, sizeof(readBuffer));
                                read(connectionFD, readBuffer, sizeof(readBuffer));
                                destAcc = atoi(readBuffer);
                                
                                float amt;
                                bzero(writeBuffer, sizeof(writeBuffer));
                                strcpy(writeBuffer, "Amount: ");
                                write(connectionFD, writeBuffer, sizeof(writeBuffer));
                                bzero(readBuffer, sizeof(readBuffer));
                                read(connectionFD, readBuffer, sizeof(readBuffer));

                                amt = atof(readBuffer);
                                transferFunds(connectionFD, accountNumber, destAcc, amt);
                                break;
                            case 6:
                                response = changePassword(connectionFD, accountNumber);
                                if(!response)
                                {
                                    bzero(writeBuffer, sizeof(writeBuffer));
                                    strcpy(writeBuffer, "Unable to change password^");
                                    write(connectionFD, writeBuffer, sizeof(writeBuffer));
                                    read(connectionFD, readBuffer, sizeof(readBuffer));
                                }
                                else
                                { 
                                    bzero(writeBuffer, sizeof(writeBuffer));
                                    strcpy(writeBuffer, "\nPassword changed successfully!\nPlease re-login to continue.^");
                                    write(connectionFD, writeBuffer, sizeof(writeBuffer));
                                    read(connectionFD, readBuffer, sizeof(readBuffer));   
                                    logout(connectionFD, accountNumber);                                
                                }                                    
                                goto label1;
                            case 7:
                                // View Transaction
                                transactionHistory(connectionFD, accountNumber);
                                break;
                            case 8:
                                // Add Feedback
                                addFeedback(connectionFD);
                                break;
                            case 9:
                                // Logout
                                printf("%d logged out!\n", accountNumber);
                                logout(connectionFD, accountNumber);
                                return;
                            case 10:
                                printf("Customer ID: %d Exited!\n", accountNumber);
                                exitClient(connectionFD, accountNumber);
                            default:
                                write(connectionFD, "Invalid Choice from customer menu\n", sizeof("Invalid Choice from customer menu\n"));                                
                        }
                    }
                }
            }
        }
        else {
            int loginResult = loginResultFirst;
            bzero(writeBuffer, sizeof(writeBuffer));
            bzero(readBuffer, sizeof(readBuffer));
            if (loginResult == 2) {
                strcpy(writeBuffer, "\nYour account is deactivated. Returning to main menu...^");
                write(connectionFD, writeBuffer, sizeof(writeBuffer));
                read(connectionFD, readBuffer, sizeof(readBuffer));
                return; // Return to main menu after deactivation message
            } else if (loginResult == 3) {
                strcpy(writeBuffer, "\nYou are login in other terminal^");
                write(connectionFD, writeBuffer, sizeof(writeBuffer));
                read(connectionFD, readBuffer, sizeof(readBuffer));
                return; // Redirect to main login menu
            } else {
                strcpy(writeBuffer, "\nInvalid Credential^");
            }
            write(connectionFD, writeBuffer, sizeof(writeBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
            return; // redirect to main menu
        }
    }
}

// ======================= Login system =======================
int loginCustomer(int connectionFD, int accountNumber, char *password) {
    struct Customer customer;
    int file = open(CUSPATH, O_CREAT | O_RDWR, 0644);

    if (file == -1) {
        printf("Error opening file!\n");
        return 0;
    }

    // Create a unique semaphore name for this account
    char semName[50];
    snprintf(semName, sizeof(semName), "/orb_%d", accountNumber);

    sem_t *cust_sema = sem_open(semName, O_CREAT, 0644, 1);
    if (cust_sema == SEM_FAILED) {
        perror("sem_open failed");
        close(file);
        return 0;
    }

    // Try to acquire semaphore — if locked, user is already logged in
    if (sem_trywait(cust_sema) == -1) {
        if (errno == EAGAIN) {
            printf("Customer with account number %d is already logged in!\n", accountNumber);
        } else {
            perror("sem_trywait failed");
        }
        sem_close(cust_sema);
        close(file);
        return 3; // already logged in elsewhere
    }

    // Verify credentials
    int valid = 0;
    int deactivated = 0;
    lseek(file, 0, SEEK_SET);
    while (read(file, &customer, sizeof(customer)) == sizeof(customer)) {
        if (customer.accountNumber == accountNumber &&
            strcmp(customer.password, crypt(password, HASHKEY)) == 0) {
            if (customer.activeStatus == 1) {
                valid = 1;
            } else {
                deactivated = 1;
            }
            break;
        }
    }

    close(file);

    if (!valid) {
        // Invalid login → release semaphore immediately
        sem_post(cust_sema);
        sem_close(cust_sema);
        sem_unlink(semName);
        if (deactivated) {
            return 2; // account exists but is deactivated
        }
        return 0;
    }

    printf("Customer %d logged in successfully.\n", accountNumber);
    sem_close(cust_sema); // keep semaphore locked until logout
    return 1;
}

// ======================= Deposit Money =======================
void depositMoney(int connectionFD, int accountNumber){
    char readBuffer[4096], writeBuffer[4096], transactionBuffer[1024];

    struct Customer customer;
    struct trans_histroy th;

    time_t s, val = 1;
    struct tm* current_time;
    s = time(NULL);
    current_time = localtime(&s);

    int file = open(CUSPATH, O_CREAT | O_RDWR, 0644);
    int fp = open(HISTORYPATH, O_RDWR | O_APPEND | O_CREAT, 0644);
    lseek(fp, 0, SEEK_END);
  
    int found = 0;
    float depositAmount;

    if (file == -1) {
        printf("Error opening file!\n");
        return;
    }

    while(read(file, &customer, sizeof(customer)) != 0) {
        if (customer.accountNumber == accountNumber) {
            break;
        }
    }
    int offset = lseek(file, -sizeof(struct Customer), SEEK_CUR);

    struct flock fl = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
    int lockStatus = fcntl(file, F_SETLKW, &fl);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter the amount to deposit: ");
    writeBytes = write(connectionFD, writeBuffer, sizeof(writeBuffer));

    if(writeBytes == -1)
    {
        printf("Unable to write client\n");
    }
    else
    {
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFD, readBuffer, sizeof(readBuffer));
        if(readBytes == -1)
        {
            printf("Unable to read client data\n");
        }
        else
        {
            depositAmount = atof(readBuffer);
            printf("Customer whose account no.: %d deposited %.2f\n", accountNumber, depositAmount);

            lseek(file, 0, SEEK_SET);
            while(read(file, &customer, sizeof(customer)) != 0) {
                if (customer.accountNumber == accountNumber) {
                    break;
                }
            }
            lseek(file, -sizeof(struct Customer), SEEK_CUR);

            customer.balance += depositAmount;
            
            bzero(transactionBuffer, sizeof(transactionBuffer));
            sprintf(transactionBuffer, "%.2f deposited at %02d:%02d:%02d %d-%d-%d\n", depositAmount, current_time->tm_hour, current_time->tm_min,current_time->tm_sec, (current_time->tm_year)+1900, (current_time->tm_mon)+1, (current_time->tm_mday));
            
            bzero(th.hist, sizeof(th.hist));
            strcpy(th.hist, transactionBuffer);
            th.acc_no = customer.accountNumber;
            write(fp, &th, sizeof(th));
            write(file, &customer, sizeof(customer));


            fl.l_type = F_UNLCK;
            fcntl(file, F_SETLK, &fl);
            
            close(fp);
            close(file);

            bzero(readBuffer, sizeof(readBuffer));
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "Done.^");
            write(connectionFD, writeBuffer, sizeof(writeBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
        }
    }
    return;
}

// ======================= View Balance =======================
void customerBal(int connectionFD, int accountNumber){
    char readBuffer[4096], writeBuffer[4096];
    struct Customer customer;
    int file = open(CUSPATH, O_RDONLY);
    if (file == -1) {
        printf("Error opening file!\n");
        return ;
    }
    float updatedBalance = 0;

    lseek(file, 0, SEEK_SET);
    while(read(file, &customer, sizeof(customer)) != 0)
    {
        if (customer.accountNumber == accountNumber) {
            updatedBalance = customer.balance;
            break;
        }
    }
    close(file);
    
    printf("Current balance of %d: %.2f\n", accountNumber, updatedBalance);
    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Bal: %.2f^", updatedBalance);
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));

    return;
}

// ======================= Withdraw Money =======================
void withdrawMoney(int connectionFD, int accountNumber){
    char readBuffer[4096], writeBuffer[4096], transactionBuffer[1024];
    struct Customer customer;
    struct trans_histroy th;

    time_t s, val = 1;
    struct tm* current_time;
    s = time(NULL);
    current_time = localtime(&s);

    int found = 0;
    float withdrawAmount;

    int file = open(CUSPATH, O_CREAT | O_RDWR, 0644);
    int fp = open(HISTORYPATH, O_RDWR | O_APPEND | O_CREAT, 0644);
    lseek(fp, 0, SEEK_END);

    while (read(file, &customer, sizeof(customer)) != 0)
    {
        if(customer.accountNumber == accountNumber)
        {
            break;
        }
    }
    int offset = lseek(file, -sizeof(struct Customer), SEEK_CUR);

    struct flock fl = {F_WRLCK, SEEK_SET, offset, sizeof(struct Customer), getpid()};
    int lockStatus = fcntl(file, F_SETLKW, &fl);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Amount: ");
    writeBytes = write(connectionFD, writeBuffer, sizeof(writeBuffer));

    if(writeBytes == -1)
    {
        printf("Unable to write client\n");
    }
    else
    {
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectionFD, readBuffer, sizeof(readBuffer));
        if(readBytes == -1)
        {
            printf("Unable to read from client\n");
        }
        else
        {
            if(lockStatus == -1)
            {
                printf("Unable to lock file\n");
                exit(0);
            }

            withdrawAmount = atof(readBuffer);

            lseek(file, 0, SEEK_SET);
            while(read(file, &customer, sizeof(customer)) != 0) {
                if (customer.accountNumber == accountNumber) {
                    break;
                }
            }
            lseek(file, -sizeof(struct Customer), SEEK_CUR);

            printf("Requested to withdraw from %d account number: %.2f\n", accountNumber, withdrawAmount);

            if (customer.balance < withdrawAmount){
                bzero(writeBuffer, sizeof(writeBuffer));
                bzero(readBuffer, sizeof(readBuffer));
                
                printf("Insufficient balance in account: %d\n", accountNumber);

                sprintf(writeBuffer, "No funds.^");
                writeBytes = write(connectionFD, writeBuffer, sizeof(writeBuffer));
                read(connectionFD, readBuffer, sizeof(readBuffer));
                if(writeBytes == -1)
                {
                    printf("Unable to write to client\n");
                }

                fl.l_type = F_UNLCK;
                fcntl(file, F_SETLK, &fl);

                close(fp);
                close(file);
                return;
            }
            customer.balance -= withdrawAmount;

            bzero(transactionBuffer, sizeof(transactionBuffer));
            sprintf(transactionBuffer, "%.2f withdraw at %02d:%02d:%02d %d-%d-%d\n", withdrawAmount, current_time->tm_hour, current_time->tm_min,current_time->tm_sec, (current_time->tm_year)+1900, (current_time->tm_mon)+1, (current_time->tm_mday));

            bzero(th.hist, sizeof(th.hist));
            strcpy(th.hist, transactionBuffer);
            th.acc_no = customer.accountNumber;
            write(fp, &th, sizeof(th));
            fsync(fp); 

            write(file, &customer, sizeof(customer));
            fsync(file);


            fl.l_type = F_UNLCK;
            fcntl(file, F_SETLK, &fl);

            close(fp);
            close(file);

            bzero(readBuffer, sizeof(readBuffer));
            bzero(writeBuffer, sizeof(writeBuffer));
            printf("New balance of %d account number: %.2f\n", accountNumber, customer.balance);
            sprintf(writeBuffer, "Withdrawal successful!^");

            writeBytes = write(connectionFD, writeBuffer, sizeof(writeBuffer));
            readBytes = read(connectionFD, readBuffer, sizeof(readBuffer));
            if(writeBytes == -1)
            {
                printf("Unable to write to client\n");
            }
        }
    }
    return;
}

// ======================= Apply loan =======================
void applyLoan(int connectionFD, int accountNumber) {
    struct Counter ct;
    int lc = 0; // declare here so it's available later

    // ---------- Read & update loan counter ----------
    int file = open(COUNTERPATH, O_RDWR | O_CREAT, 0644);
    if (file == -1) {
        perror("open COUNTERPATH");
        ct.count = 0;
    } else {
        ssize_t r = read(file, &ct, sizeof(ct));
        if (r != sizeof(ct)) {
            // File may contain garbage or be empty — fallback to 0
            lseek(file, 0, SEEK_SET);
            char buf[64] = {0};
            ssize_t n = read(file, buf, sizeof(buf)-1);
            if (n > 0)
                ct.count = atoi(buf);
            else
                ct.count = 0;
        }

        lc = ct.count;          // store current counter
        ct.count = lc + 1;      // increment for next use

        lseek(file, 0, SEEK_SET);
        if (write(file, &ct, sizeof(ct)) != sizeof(ct))
            perror("write COUNTERPATH");

        if (ftruncate(file, sizeof(ct)) == -1)
            perror("ftruncate COUNTERPATH");

        close(file);
    }

    // ---------- Proceed with loan creation ----------
    char readBuffer[4096], writeBuffer[4096];
    struct LoanDetails ld;
    int file1 = open(LOANPATH, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (file1 == -1) {
        perror("open LOANPATH");
        return;
    }

    int amount;
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter Loan Amount: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    lseek(file1, 0, SEEK_END);
    read(connectionFD, readBuffer, sizeof(readBuffer));

    amount = atoi(readBuffer);
    printf("Applied loan of %d from acc no.: %d\n", amount, accountNumber);

    ld.empID = -1;
    ld.accountNumber = accountNumber;
    ld.loanAmount = amount;
    ld.status = 0;         // Requested
    ld.loanID = lc + 1;    // assign incremented Loan ID

    int response = write(file1, &ld, sizeof(ld));
    close(file1);

    if (response <= 0) {
        bzero(readBuffer, sizeof(readBuffer));
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Unable to apply for loan!^");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));
    } else {
        bzero(readBuffer, sizeof(readBuffer));
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Loan applied successfully^");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));
    }
}


// ======================= Money Transfer =======================
void transferFunds(int connectionFD, int sourceAccount, int destAccount, float amount) {
    char readBuffer[4096], writeBuffer[4096], transactionBuffer[1024];
    struct Customer srcCustomer, dstCustomer;
    struct trans_histroy th;
    int srcFound = 0, dstFound = 0;
    int srcOffset = -1, dstOffset = -1;

    time_t s = time(NULL);
    struct tm* current_time = localtime(&s);

    int file = open(CUSPATH, O_RDWR);
    if (file == -1) {
        perror("Error opening customer file");
        return;
    }

    int fp = open(HISTORYPATH, O_RDWR | O_APPEND | O_CREAT, 0644);
    if (fp == -1) {
        perror("Error opening history file");
        close(file);
        return;
    }

    // --- Lock the whole file ---
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0; // lock entire file
    fl.l_pid = getpid();
    if (fcntl(file, F_SETLKW, &fl) == -1) {
        perror("Cannot lock customer file");
        close(file);
        close(fp);
        return;
    }

    // --- Find both accounts and their offsets ---
    lseek(file, 0, SEEK_SET);
    struct Customer temp;
    while (read(file, &temp, sizeof(temp)) == sizeof(temp)) {
        if (temp.accountNumber == sourceAccount && !srcFound) {
            srcOffset = lseek(file, -sizeof(temp), SEEK_CUR);
            srcCustomer = temp;
            srcFound = 1;
        } else if (temp.accountNumber == destAccount && !dstFound) {
            dstOffset = lseek(file, -sizeof(temp), SEEK_CUR);
            dstCustomer = temp;
            dstFound = 1;
        }
        if (srcFound && dstFound)
            break;
    }

    if (!srcFound || !dstFound) {
        sprintf(writeBuffer, "Invalid account number(s).^");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));
        goto unlock_exit;
    }

    // --- Check funds ---
    if (srcCustomer.balance < amount) {
        sprintf(writeBuffer, "Insufficient funds in source account.^");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));
        goto unlock_exit;
    }

    // --- Update balances ---
    srcCustomer.balance -= amount;
    dstCustomer.balance += amount;

    // --- Write back source ---
    lseek(file, srcOffset, SEEK_SET);
    write(file, &srcCustomer, sizeof(srcCustomer));

    // --- Write back destination ---
    lseek(file, dstOffset, SEEK_SET);
    write(file, &dstCustomer, sizeof(dstCustomer));

    // --- Log transactions ---
    sprintf(transactionBuffer, "%.2f transferred to account %d on %02d-%02d-%04d %02d:%02d:%02d\n",
            amount, destAccount,
            current_time->tm_mday, current_time->tm_mon + 1, current_time->tm_year + 1900,
            current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
    th.acc_no = sourceAccount;
    strcpy(th.hist, transactionBuffer);
    write(fp, &th, sizeof(th));

    sprintf(transactionBuffer, "%.2f received from account %d on %02d-%02d-%04d %02d:%02d:%02d\n",
            amount, sourceAccount,
            current_time->tm_mday, current_time->tm_mon + 1, current_time->tm_year + 1900,
            current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
    th.acc_no = destAccount;
    strcpy(th.hist, transactionBuffer);
    write(fp, &th, sizeof(th));

    fsync(file);
    fsync(fp);

    // --- Unlock and close ---
unlock_exit:
    fl.l_type = F_UNLCK;
    fcntl(file, F_SETLK, &fl);
    close(file);
    close(fp);

    // --- Return updated balance of source ---
    sprintf(writeBuffer, "Transfer complete. New balance of %d account is: %.2f^", sourceAccount, srcCustomer.balance);
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));

    printf("[INFO] Transfer: %.2f from %d -> %d | New src balance: %.2f\n",
           amount, sourceAccount, destAccount, srcCustomer.balance);
}


// ======================= View Transaction History =======================
void transactionHistory(int connectionFD, int accountNumber) {
    char tempBuffer[4096];
    struct trans_histroy th;
    int transCount = 0;

    int file = open(HISTORYPATH, O_RDONLY | O_CREAT, 0644);
    if (file == -1) {
        perror("Error opening transaction history file");
        return;
    }

    bzero(writeBuffer, sizeof(writeBuffer));
    lseek(file, 0, SEEK_SET); 

    while (read(file, &th, sizeof(th)) == sizeof(th)) {
        if (th.acc_no == accountNumber) {
            bzero(tempBuffer, sizeof(tempBuffer));
            strcpy(tempBuffer, th.hist);
            strcat(writeBuffer, tempBuffer);
            transCount++;
        }
    }

    close(file);

    if (transCount == 0) {
        strcpy(writeBuffer, "No transactions found for this account.^");
    } else {
        strcat(writeBuffer, "^");
    }

    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
}

// ======================= Add Feedback =======================
void addFeedback(int connectionFD){
    
    struct FeedBack fb;

    int file = open(FEEDPATH, O_CREAT | O_RDWR | O_APPEND, 0644);
    if(file == -1)
    {
        printf("Error in opening file\n");
    }

    int choice;
    bzero(writeBuffer, sizeof(writeBuffer));
    bzero(readBuffer, sizeof(readBuffer));
    strcpy(writeBuffer, "Enter Feedback:\n1. Good\n2. Bad\n3. Worse\n");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    
    read(connectionFD, readBuffer, sizeof(readBuffer));
    choice = atoi(readBuffer);

    if(choice == 1)
    {
        strcpy(fb.feedback, "Good");
    }
    else if(choice == 2)
    {
        strcpy(fb.feedback, "Bad");
    }
    else
    {
        strcpy(fb.feedback, "Worse");
    }
    write(file, &fb, sizeof(fb));
    close(file);

    bzero(writeBuffer, sizeof(writeBuffer));
    bzero(readBuffer, sizeof(readBuffer));
    strcpy(writeBuffer, "^");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
}

// ======================= Change Password =======================
int changePassword(int connectionFD, int accountNumber){
    char readBuffer[4096], writeBuffer[4096];

    char newPassword[20];

    struct Customer c;
    int file = open(CUSPATH,  O_CREAT | O_RDWR, 0644);
    
    lseek(file, 0, SEEK_SET);

    int srcOffset = -1, sourceFound = 0;

    while (read(file, &c, sizeof(c)) != 0)
    {
        if(c.accountNumber == accountNumber)
        {
            srcOffset = lseek(file, -sizeof(struct Customer), SEEK_CUR);
            sourceFound = 1;
        }
        if(sourceFound)
            break;
    }

    struct flock fl1 = {F_WRLCK, SEEK_SET, srcOffset, sizeof(struct Customer), getpid()};
    fcntl(file, F_SETLKW, &fl1);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter password: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    
    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    strcpy(newPassword, readBuffer);

    strcpy(c.password, crypt(newPassword, HASHKEY));
    write(file, &c, sizeof(c));

    fl1.l_type = F_UNLCK;
    fl1.l_whence = SEEK_SET;
    fl1.l_start = srcOffset;
    fl1.l_len = sizeof(struct Customer);
    fl1.l_pid = getpid();
    
    fcntl(file, F_UNLCK, &fl1);
    close(file);
    printf("Customer %d changed password\n", accountNumber);
    return 1;
}

// ======================= Logout =======================
void logout(int connectionFD, int accountNumber) {
    char semName[50];
    snprintf(semName, sizeof(semName), "/orb_%d", accountNumber);

    sem_t *cust_sema = sem_open(semName, 0);
    if (cust_sema != SEM_FAILED) {
        sem_post(cust_sema);
        sem_close(cust_sema);
        sem_unlink(semName);
    }

    printf("Customer %d logged out successfully.\n", accountNumber);
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "^");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
}



