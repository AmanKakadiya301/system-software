/*
============================================================================
Name : 25.c
Author : Aman Kakadiya
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
                i.    Access permission
                ii.   uid, gid
                iii.  Time of last message sent and received
                iv.   Time of last change in the message queue
                v.    Size of the queue
                vi.   Number of messages in the queue
                vii.  Maximum number of bytes allowed
                viii. PID of the msgsnd and msgrcv
Date: 20th Sep, 2025.
============================================================================
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

int main()
{
    key_t key = ftok(".", 'a');
    int msqid = msgget(key, IPC_CREAT | 0666);
    // printf("%d\n", msqid);
    
    struct msqid_ds st;
    msgctl(msqid, IPC_STAT, &st);

    printf("Access Permissions - %o\n", st.msg_perm.mode);
    printf("Uid and gid of owner - %d %d\n", st.msg_perm.uid, st.msg_perm.gid);
    printf("Uid and gid of creator - %d %d\n", st.msg_perm.cuid, st.msg_perm.cgid);
    printf("Time of last message sent and received in queue - %ld %ld\n", st.msg_stime, st.msg_rtime);
    printf("Time of last change - %ld\n", st.msg_ctime);
    printf("Size of the queue - %ld\n", st.__msg_cbytes);
    printf("No of messages in queue - %ld\n", st.msg_qnum);
    printf("Max no. of bytes allowed in queue - %ld\n", st.msg_qbytes);
    printf("Pid of the last msgsnd and msgrcv - %d %d\n", st.msg_lspid, st.msg_lrpid);

    return (0);
}

/*
amank@amank-HP-EliteBook-x360-1030-G2:~/SS_HL2_AmanKakadiya_MT2025058 $ ./a.out
Access Permissions - 666
Uid and gid of owner - 1000 1000
Uid and gid of creator - 1000 1000
Time of last message sent and received in queue - 0 0
Time of last change - 1759239688
Size of the queue - 0
No of messages in queue - 0
Max no. of bytes allowed in queue - 16384
Pid of the last msgsnd and msgrcv - 0 0
*/