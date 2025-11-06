void managerMenu(int connectionFD);
int loginManager(int connectionFD, int mngID, char *password);
void changeStatus(int connectionFD);
void assignLoanApplication(int connectionFD);
void readFeedBack(int connectionFD);
int changeMNGPassword(int connectionFD, int mngID);

// Shared buffers are defined once in server.c
extern char readBuffer[4096], writeBuffer[4096];   

void managerMenu(int connectionFD)
{
    struct Employee manager;
    int mngID, response = 0;
    char password[20];
    char newPassword[20];
    int choice;

label1:
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "\nManager ID: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    mngID = atoi(readBuffer);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Password: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    strcpy(password, readBuffer);

    int mngLoginResult = loginManager(connectionFD, mngID, password);
    if(mngLoginResult == 1)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        printf("Manager whose ID: %d logged In\n", mngID);
        strcpy(writeBuffer, "\nLogin Successfully^");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));

        while(1)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, MNGMENU);
            write(connectionFD, writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
            choice = atoi(readBuffer);

            printf("Manager entered: %d\n", choice);
            switch(choice)
            {
                case 1:
                    // Activate/Deactivate Customer Accounts
                    changeStatus(connectionFD);
                    break;
                case 2:
                    // Assign Loan Application Processes to Employees
                    assignLoanApplication(connectionFD);
                    break;
                case 3:
                    // Review Customer Feedback
                    readFeedBack(connectionFD);
                    break;
                case 4:
                    // Change Password
                    response = changeMNGPassword(connectionFD, mngID);
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
                        logout(connectionFD,mngID);
                    }                                
                    goto label1;
                case 5:
                    // Logout
                    printf("Manager Logged out!\n");
                    logout(connectionFD, mngID);
                    return ;
                case 6:
                    // Exit
                    printf("Manager exited\n");
                    exitClient(connectionFD, mngID);
                    return;
                default:
                    printf("Invalid choice\n");
            }
        }
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        if (mngLoginResult == 3) {
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

// ==================== Login Manager ======================
int loginManager(int connectionFD, int mngID, char *password)
{
    struct Employee mng;
    int file = open(EMPPATH, O_RDWR | O_CREAT, 0644);

    if (file == -1) {
        printf("Error opening file!\n");
        return 0;
    }

    sema = initializeSemaphore(mngID);

    setupSignalHandlers();

    if (sem_trywait(sema) == -1) {
        if (errno == EAGAIN) {
            printf("Manager %d is already logged in!\n", mngID);
        } else {
            perror("sem_trywait failed");
        }
        close(file);
        return 3; // already logged in elsewhere
    }

    lseek(file, 0, SEEK_SET);
    while(read(file, &mng, sizeof(mng)) != 0)
    {
        if (mng.empID == mngID && strcmp(mng.password, crypt(password, HASHKEY)) == 0 && mng.role == 0) {
            close(file);
            return 1;
        }
    }

    snprintf(semName, 50, "/orb_%d", mngID);

    sem_t *sema = sem_open(semName, 0);
    if (sema != SEM_FAILED) {
        sem_post(sema);
        sem_close(sema); 
        sem_unlink(semName);    
    }

    close(file);
    return 0;
}

// ==================== Activate/Deactivate Customer Accounts ====================
void changeStatus(int connectionFD)
{
    int file = open(CUSPATH, O_CREAT | O_RDWR, 0644);
    struct Customer cs;

    int accNo, choice;

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Acc: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));

    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    accNo = atoi(readBuffer);

    while(read(file, &cs, sizeof(cs)) != 0)
    {
        if(cs.accountNumber == accNo)
        {
            bzero(writeBuffer, sizeof(writeBuffer));
            strcpy(writeBuffer, "1) Off  2) On: ");
            write(connectionFD, writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
            choice = atoi(readBuffer);

            if(choice == 1)
            {
                lseek(file,-sizeof(struct Customer), SEEK_CUR);
                cs.activeStatus = 0;
                write(file, &cs, sizeof(cs));
                close(file);

                printf("Manager activated customer %d\n", accNo);

                bzero(readBuffer, sizeof(readBuffer));
                bzero(writeBuffer, sizeof(writeBuffer));
                strcpy(writeBuffer, "Status Changed Successfully^");
                write(connectionFD, writeBuffer, sizeof(writeBuffer));
                read(connectionFD, readBuffer, sizeof(readBuffer));
                return ;
            }
            else if(choice == 2)
            {
                lseek(file, -sizeof(struct Customer), SEEK_CUR);
                cs.activeStatus = 1;
                write(file, &cs, sizeof(cs));
                close(file);

                printf("Manager deactivated customer %d\n", accNo);

                bzero(readBuffer, sizeof(readBuffer));
                bzero(writeBuffer, sizeof(writeBuffer));
                strcpy(writeBuffer, "Status Changed Successfully^");
                write(connectionFD, writeBuffer, sizeof(writeBuffer));
                read(connectionFD, readBuffer, sizeof(readBuffer));
                return ;
            }
        }
    }
    close(file);
    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));
    printf("Manager entered invalid account number: %d\n", accNo);
    strcpy(writeBuffer, "Invalid account number^");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    return ;
}

// ==================== Read Feedback =========================
void readFeedBack(int connectionFD)
{
    struct FeedBack fb;
    char tempBuffer[4096];

    int file = open(FEEDPATH, O_RDONLY);
    if(file == -1)
    {
        printf("Error in opening file\n");
    }

    bzero(tempBuffer, sizeof(tempBuffer));
    while(read(file, &fb, sizeof(fb)) != 0)
    {
        strcat(tempBuffer, fb.feedback);
        strcat(tempBuffer, "\n");
    }
    strcat(tempBuffer, "^");
    printf("Manager reading customers feedback\n");
    bzero(writeBuffer, sizeof(writeBuffer));
    bzero(readBuffer, sizeof(readBuffer));
    strcpy(writeBuffer, tempBuffer);
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
}

void assignLoanApplication(int connectionFD)
{
    struct LoanDetails ld;
    int file = open(LOANPATH, O_CREAT | O_RDWR, 0644);
    if(file == -1)
    {
        printf("Error in opening file\n");
        return ;
    }

    // Printing Unassigned loan application
    while(read(file, &ld, sizeof(ld)) != 0)
    {
        if(ld.empID == -1)
        {
            bzero(readBuffer, sizeof(readBuffer));
            bzero(writeBuffer, sizeof(writeBuffer));
            sprintf(writeBuffer, "Loan ID: %d\nAccount Number: %d\nLoan Amount: %d^", ld.loanID, ld.accountNumber, ld.loanAmount);
            write(connectionFD, writeBuffer, sizeof(writeBuffer));
            read(connectionFD, readBuffer, sizeof(readBuffer));
        }
    }
    lseek(file, 0, SEEK_SET);

    // Locking
    int lID, eID;

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Loan ID: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    
    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    lID = atoi(readBuffer);

    int srcOffset = -1, sourceFound = 0;
    while (read(file, &ld, sizeof(ld)) != 0)
    {
        if(ld.loanID == lID)
        {
            srcOffset = lseek(file, -sizeof(struct LoanDetails), SEEK_CUR);
            sourceFound = 1;
        }
        if(sourceFound)
            break;
    }
    if(srcOffset == -1)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        sprintf(writeBuffer, "Invalid loan ID^");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));
        // Add these two lines for AI issue assigned tasks
        close(file); 
        return;
    }
    
    struct flock fl1 = {F_WRLCK, SEEK_SET, srcOffset, sizeof(struct LoanDetails), getpid()};
    int result = fcntl(file, F_SETLK, &fl1);

    if(result != -1)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Emp ID: ");
        write(connectionFD, writeBuffer, sizeof(writeBuffer));

        bzero(readBuffer, sizeof(readBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));
        eID = atoi(readBuffer);

        ld.empID = eID;
        ld.status = 1; // Pending

        // Log to server
        printf("Manager assigned Employee %d to Loan ID %d successfully.\n", eID, lID);

        // Write updated record
        lseek(file, srcOffset, SEEK_SET);
        write(file, &ld, sizeof(ld));
        fsync(file);

        // Confirmation to client
        bzero(writeBuffer, sizeof(writeBuffer));
        sprintf(writeBuffer, "Loan ID %d assigned to Employee %d successfully.^", lID, eID);
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));

        // Unlock
        fl1.l_type = F_UNLCK;
        fl1.l_whence = SEEK_SET;
        fl1.l_start = srcOffset;
        fl1.l_len = sizeof(struct LoanDetails);
        fl1.l_pid = getpid();

        fcntl(file, F_UNLCK, &fl1);
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        bzero(readBuffer, sizeof(readBuffer));
        sprintf(writeBuffer, "Employee already assigned for given %d loan ID^", lID);
        write(connectionFD, writeBuffer, sizeof(writeBuffer));
        read(connectionFD, readBuffer, sizeof(readBuffer));
    }

    close(file);
    
    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "^");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
}

// ==================== Change Password ====================
int changeMNGPassword(int connectionFD, int mngID)
{
    char newPassword[20];

    struct Employee m;
    int file = open(EMPPATH, O_CREAT | O_RDWR, 0644);
    
    lseek(file, 0, SEEK_SET);

    int srcOffset = -1, sourceFound = 0;

    while (read(file, &m, sizeof(m)) != 0)
    {
        if(m.empID == mngID)
        {
            srcOffset = lseek(file, -sizeof(struct Employee), SEEK_CUR);
            sourceFound = 1;
        }
        if(sourceFound)
            break;
    }

    struct flock fl1 = (struct flock){F_WRLCK, SEEK_SET, srcOffset, sizeof(struct Employee), getpid()};
    fcntl(file, F_SETLKW, &fl1);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, "Enter password: ");
    write(connectionFD, writeBuffer, sizeof(writeBuffer));
    
    bzero(readBuffer, sizeof(readBuffer));
    read(connectionFD, readBuffer, sizeof(readBuffer));
    strcpy(newPassword, readBuffer);

    strcpy(m.password, crypt(newPassword, HASHKEY));
    write(file, &m, sizeof(m));

    fl1.l_type = F_UNLCK;
    fl1.l_whence = SEEK_SET;
    fl1.l_start = srcOffset;
    fl1.l_len = sizeof(struct Employee);
    fl1.l_pid = getpid();

    fcntl(file, F_UNLCK, &fl1);
    close(file);

    printf("Manager %d changed password\n", mngID);
    return 1;
}


