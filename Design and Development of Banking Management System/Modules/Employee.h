void employeeMenu(int connectionFD);
int loginEmployee(int connectionFD, int empID, char *password);
void addCustomer(int connectionFD);
void approveRejectLoan(int connectionFD, int empID);
void viewAssignedLoan(int connectionFD, int empID);
int changeEMPPassword(int connectionFD, int empID);

void employeeMenu(int connectionFD)
{
    struct Employee employee;
    int empID;
    int accountNumber, response = 0;
    char password[20];
    char newPassword[20];
    int choice;

label1:
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "\nEmp ID: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    empID = atoi(readBuffer);
    printf("Employee entered his ID: %d\n", empID);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Password: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    strcpy(password, readBuffer);


    int empLoginResult = loginEmployee(connectionFD, empID, password);
    if(empLoginResult == 1)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        printf("%d logged In\n", empID);
        strcpy(writeBuffer, "\nLogin Successfully^");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));

        while(1)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, EMPMENU);
            write(connectionFD, writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
            choice = atoi(readBuffer);

            printf("Employee choose: %d\n", choice);
            switch(choice)
            {
                case 1:
                    // Add New Customer
                    addCustomer(connectionFD);
                    break;
                case 2:
                    // Modify Customer Details
                    modifyCE(connectionFD, 1);
                    break;
                case 3:
                    // Approve/Reject Loans
                    approveRejectLoan(connectionFD, empID);
                    break;
                case 4:
                    // View Assigned Loan Applications
                    viewAssignedLoan(connectionFD, empID);
                    break;
                case 5:
                    // View Customer Transactions
                    bzero(writeBuffer, sizeof(writeBuffer));
                    strcpy(writeBuffer, "Acc: ");
                    write(connectionFD, writeBuffer, sizeof(writeBuffer));
                    
                    bzero(readBuffer, sizeof(readBuffer));
                    read(connectionFD, readBuffer, sizeof(readBuffer));
                    accountNumber = atoi(readBuffer);

                    transactionHistory(connectionFD, accountNumber);
                    break;
                case 6:
                    // Change Password
                    response = changeEMPPassword(connectionFD, empID);
                    if(!response)
                    {
                        bzero(writeBuffer, sizeof(writeBuffer));
                        strcpy(writeBuffer, "Unable to change password\n^");
                        write(connectionFD, writeBuffer, sizeof(writeBuffer));
                        read(connectionFD, readBuffer, sizeof(readBuffer));
                    }   
                    else
                    {
                        bzero(writeBuffer, sizeof(writeBuffer));
                        bzero(readBuffer, sizeof(readBuffer));
                        strcpy(writeBuffer,"\nPassword changed successfully\nLogin with new password...\n^");
                        write(connectionFD, writeBuffer, sizeof(writeBuffer));
                        read(connectionFD, readBuffer, sizeof(readBuffer));
                        logout(connectionFD,empID);
                    }                                 
                    goto label1;
                case 7:
                    // Logout
                    printf("Employee ID: %d Logged Out!\n", empID);
                    logout(connectionFD, empID);
                    return ;
                case 8:
                    // Exit
                    printf("Employee ID: %d Exited!\n", empID);
                    exitClient(connectionFD, empID);
                default:
                    printf("Invalid choice\n");                
            }
        }
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        if (empLoginResult == 3) {
            strcpy(writeBuffer, "\nYou are login in other terminal^");
            write(connectionFD, writeBuffer, sizeof(writeBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
            return; // Redirect to main login menu
        } else {
            strcpy(writeBuffer, "\nInvalid Credential^");
            write(connectionFD, writeBuffer, sizeof(writeBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
            return; // redirect to main menu
        }
    }
}

// ================= Login Employee ==================
int loginEmployee(int connectionFD, int empID, char *password)
{
    struct Employee employee;
    int file = open(EMPPATH, O_CREAT | O_RDWR, 0644);

    sema = initializeSemaphore(empID);

    setupSignalHandlers();

    if (sem_trywait(sema) == -1) {
        if (errno == EAGAIN) {
            printf("Employee %d is already logged in!\n", empID);
        } else {
            perror("sem_trywait failed");
        }
        close(file);
        return 3; // already logged in elsewhere
    }

    lseek(file, 0, SEEK_CUR);
    while(read(file, &employee, sizeof(employee)) != 0)
    {
        if (employee.empID == empID && strcmp(employee.password, crypt(password, HASHKEY)) == 0 && employee.role == 1) {
            close(file);
            return 1;
        }
    }
    snprintf(semName, 50, "/orb_%d", empID);

    sem_t *sema = sem_open(semName, 0);
    if (sema != SEM_FAILED) {
        sem_post(sema);
        sem_close(sema); 
        sem_unlink(semName);    
    }

    close(file);
    return 0;   
}

// ================= Add customer ==================
void addCustomer(int connectionFD) {
    struct Customer customer;
    char name[20], password[20], transactionBuffer[1024];

    struct trans_histroy th;

    time_t s, val = 1;
    struct tm* current_time;
    s = time(NULL);
    current_time = localtime(&s);

    int file = open(CUSPATH, O_RDWR | O_APPEND | O_CREAT, 0644);
    int fp = open(HISTORYPATH, O_RDWR | O_APPEND | O_CREAT, 0644);

    if (file == -1) {
        printf("Error opening file!\n");
        return;
    }

    // Customer Name
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter Name: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    strcpy(customer.customerName, readBuffer);

    // Customer Password
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter Password: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    strcpy(customer.password, crypt(readBuffer, HASHKEY));


    // Customer Account Number
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter Account Number: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    customer.accountNumber = atoi(readBuffer);

    // Customer Initial Balance
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter Opening Balance: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    customer.balance = atof(readBuffer);

    bzero(transactionBuffer, sizeof(transactionBuffer));
    sprintf(transactionBuffer, "%.2f Opening Balance %02d:%02d:%02d %d-%d-%d\n", customer.balance, current_time->tm_hour, current_time->tm_min,current_time->tm_sec, (current_time->tm_year)+1900, (current_time->tm_mon)+1, current_time->tm_mday);
    
    bzero(th.hist, sizeof(th.hist));
    strcpy(th.hist, transactionBuffer);
    th.acc_no = customer.accountNumber;
    write(fp, &th, sizeof(th));

    close(fp);

    customer.activeStatus = 1;

    write(file, &customer, sizeof(customer));
    close(file);

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));
    printf("Employee added customer whose account number is: %d\n", customer.accountNumber);
    strcpy(writeBuffer, "Customer added successfully!^");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
}

// ================= Approve/Reject Loan =================
void approveRejectLoan(int connectionFD, int empID)
{
    char transactionBuffer[1024];
    struct LoanDetails ld;
    
    struct trans_histroy th;
    time_t s, val = 1;
    struct tm* current_time;
    s = time(NULL);
    current_time = localtime(&s);

    int lID;
    struct Customer cs;

    int file = open(LOANPATH, O_CREAT | O_RDWR, 0644);
    int fp = open(HISTORYPATH, O_RDWR | O_APPEND);
    lseek(file, 0, SEEK_SET);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Loan ID: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    lID = atoi(readBuffer);

    int srcOffset1 = -1, sourceFound1 = 0, srcOffset2 = -1, sourceFound2 = 0;
    while (read(file, &ld, sizeof(ld)) != 0)
    {
        if(ld.loanID == lID)
        {
            srcOffset1 = lseek(file, -sizeof(struct LoanDetails), SEEK_CUR);
            sourceFound1 = 1;
        }
        if(sourceFound1)
            break;
    }

    struct flock fl1 = {F_WRLCK, SEEK_SET, srcOffset1, sizeof(struct LoanDetails), getpid()};
    int result1 = fcntl(file, F_SETLK, &fl1);

    int approveFlag = 0, rejectFlag = 0, devactiveFlag = 0;

    if(result1 != -1)
    {
        int file2 = open(CUSPATH, O_CREAT | O_RDWR, 0644);
        while (read(file2, &cs, sizeof(cs)) != 0)
        {
            if(cs.accountNumber == ld.accountNumber)
            {
                srcOffset2 = lseek(file2, -sizeof(struct Customer), SEEK_CUR);
                sourceFound2 = 1;
            }
            if(sourceFound2)
                break;
        }
        struct flock fl2 = {F_WRLCK, SEEK_SET, srcOffset2, sizeof(struct Customer), getpid()};
        int result2 = fcntl(file2, F_SETLKW, &fl2);

        int choice;
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "1) OK  2) No: ");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));

        bzero(readBuffer, sizeof(readBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));
        choice = atoi(readBuffer);

        if(choice == 1)
        {
            if(cs.activeStatus == 0)
            {
                ld.status = 3;   // rejected                         
                printf("%d rejected loan for account number: %d reason customer is deactive\n", empID, cs.accountNumber);
                lseek(file, srcOffset1, SEEK_SET);
            write(file, &ld, sizeof(ld));
            fsync(file);
                devactiveFlag = 1;
            }
            else
            {
                cs.balance += ld.loanAmount;
                ld.status = 2; // approved

                printf("%d approved loan for account number: %d\n", empID, cs.accountNumber);

                bzero(transactionBuffer, sizeof(transactionBuffer));
                sprintf(transactionBuffer, "%d credited by loan id %d at %02d:%02d:%02d %d-%d-%d\n", ld.loanAmount, lID, current_time->tm_hour, current_time->tm_min,current_time->tm_sec, (current_time->tm_year)+1900, (current_time->tm_mon)+1, (current_time->tm_mday));
            
                bzero(th.hist, sizeof(th.hist));
                strcpy(th.hist, transactionBuffer);
                th.acc_no = cs.accountNumber;
                write(fp, &th, sizeof(th));

                lseek(file2, srcOffset2, SEEK_SET);
                write(file2, &cs, sizeof(cs));
                fsync(file2);
                lseek(file, srcOffset1, SEEK_SET);
            write(file, &ld, sizeof(ld));
            fsync(file);

                fl2.l_type = F_UNLCK;
                fl2.l_whence = SEEK_SET;
                fl2.l_start = srcOffset2;
                fl2.l_len = sizeof(struct Customer);
                fl2.l_pid = getpid();

                fcntl(file2, F_UNLCK, &fl2);

                close(file2);
                close(fp);
                approveFlag = 1;
            }
        }
        else if(choice == 2)
        {

            printf("%d rejected loan for account number: %d\n", empID, cs.accountNumber);
            ld.status = 3;
            lseek(file, srcOffset1, SEEK_SET);
            write(file, &ld, sizeof(ld));
            fsync(file);
            rejectFlag = 1;

            fl2.l_type = F_UNLCK;
            fl2.l_whence = SEEK_SET;
            fl2.l_start = srcOffset2;
            fl2.l_len = sizeof(struct Customer);
            fl2.l_pid = getpid();

            fcntl(file2, F_UNLCK, &fl2);

            close(file2);
        }
        else
        {
            printf("Invalid Choice\n");
        }
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        sprintf(writeBuffer, "Given Loan ID %d is already either approved or rejected^", lID);
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer)); 
    }

    fl1.l_type = F_UNLCK;
    fl1.l_whence = SEEK_SET;
    fl1.l_start = srcOffset1;
    fl1.l_len = sizeof(struct LoanDetails);
    fl1.l_pid = getpid();
    fcntl(file, F_UNLCK, &fl1);

    close(file);

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));
    if(approveFlag == 1)
    {
        strcat(writeBuffer, "Loan Approved\n");
    }
    else if(rejectFlag == 1)
    {
        strcat(writeBuffer, "Loan rejected\n");
    }
    else if(devactiveFlag == 1)
    {
        strcat(writeBuffer, "Account is already deactivate so can't approve/reject loan\n");
    }
    strcat(writeBuffer, "^");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
}

// ================= View Assigned Employee =================
void viewAssignedLoan(int connectionFD, int empID)
{
    struct LoanDetails ld;
    int file = open(LOANPATH, O_RDONLY);
    if(file == -1)
    {
        printf("Error in opening file\n");
        return;
    }

    while(read(file, &ld, sizeof(ld)) != 0)
    {
        if(ld.empID == empID && ld.status == 1)
        {
            bzero(readBuffer, sizeof(readBuffer));
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "Loan ID: %d\nAccount Number: %d\nLoan Amount: %d\n", ld.loanID, ld.accountNumber, ld.loanAmount);
            write(connectionFD, writeBuffer, sizeof(writeBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
        }
    }
    close(file);
    
    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "^");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
}

// ================= Change Password =================
int changeEMPPassword(int connectionFD, int empID)
{
    char newPassword[20];

    struct Employee emp;
    int file = open(EMPPATH, O_CREAT | O_RDWR, 0644);
    
    lseek(file, 0, SEEK_SET);

    int srcOffset = -1, sourceFound = 0;

    while (read(file, &emp, sizeof(emp)) != 0)
    {
        if(emp.empID == empID)
        {
            srcOffset = lseek(file, -sizeof(struct Employee), SEEK_CUR);
            sourceFound = 1;
        }
        if(sourceFound)
            break;
    }

    struct flock fl1 = {F_WRLCK, SEEK_SET, srcOffset, sizeof(struct Employee), getpid()};
    fcntl(file, F_SETLKW, &fl1);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter password: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    
    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    strcpy(newPassword, readBuffer);

    strcpy(emp.password, crypt(newPassword, HASHKEY));
    write(file, &emp, sizeof(emp));

    fl1.l_type = F_UNLCK;
    fl1.l_whence = SEEK_SET;
    fl1.l_start = srcOffset;
    fl1.l_len = sizeof(struct Employee);
    fl1.l_pid = getpid();

    fcntl(file, F_UNLCK, &fl1);
    close(file);

    printf("Employee %d changed password\n", empID);
    return 1;
}


