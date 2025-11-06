/*
============================================================================
Name : 27.c
Author : Aman Kakadiya
Description : Write a program to receive messages from the message queue
                a.  With 0 as a flag
                b.  With IPC_NOWAIT as a flag
Date: 20th Sep, 2025.
============================================================================
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

struct my_msg
{
    long mtype;
    char mtext[80];
};

int main()
{
    key_t key = ftok(".", 'a');

    struct my_msg msg;
    int type;
    int msqid = -1;

    int choice = -1;
    printf("Choose a flag:\n1.) Use 0 as flag\n2.) Use IPC_NOWAIT as flag\n=> ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        msqid = msgget(key, 0);

        printf("Enter message type:\n");
        scanf("%d", &type);

        if (msgrcv(msqid, (struct msgbuf *)&msg, 80, type, 0) == -1)
        {
            printf("No message\n");
        }
        else
        {
            printf("Message - %s \n", msg.mtext);
        }

        break;
    case 2:
        msqid = msgget(key, 0);

        printf("Enter message type:\n");
        scanf("%d", &type);

        if (msgrcv(msqid, (struct msgbuf *)&msg, 80, type, IPC_NOWAIT) == -1)
        {
            printf("No message\n");
        }
        else
        {
            printf("Message - %s \n", msg.mtext);
        }

        break;
    default:
        printf("Invalid choice\n");
        break;
    }

    return (0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
Choose a flag:
1.) Use 0 as flag
2.) Use IPC_NOWAIT as flag
=> 1
Enter message type:
HELLO
Message - Hello 
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
Choose a flag:
1.) Use 0 as flag
2.) Use IPC_NOWAIT as flag
=> 1
Enter message type:
HELLO
Message - Hey 
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
Choose a flag:
1.) Use 0 as flag
2.) Use IPC_NOWAIT as flag
=> 2
Enter message type:
HELLO
Message - HELLO 
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058$ ./a.out 
Choose a flag:
1.) Use 0 as flag
2.) Use IPC_NOWAIT as flag
=> 2
Enter message type:
HELLO WORLD
Message - HELLO 
*/