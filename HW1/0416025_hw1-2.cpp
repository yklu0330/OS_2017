//
//  main.cpp
//  OS HW1-2
//
//  Created by 呂翊愷 on 2017/10/13.
//  Copyright © 2017年 LU YI-KAI. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/time.h>

using namespace std;

int main(int argc, const char * argv[])
{
    int dimension;
    cout << "Dimension: ";
    cin >> dimension;
    unsigned int *matrix = new unsigned int[dimension * dimension];
    int i, j, k, l;
    for (i = 0; i < dimension * dimension; i++)
        matrix[i] = i;
    int shmid = shmget(IPC_PRIVATE, sizeof(unsigned int) * dimension * dimension, IPC_CREAT | 0600);
    unsigned int *result = (unsigned int *)shmat(shmid, NULL, 0);
    
    
    for (int p = 1; p <= 4; p++)
    {
        if (p == 2 || p == 3)
            continue;
        int pids[4];
        
        struct timeval start, end;
        gettimeofday(&start, 0);
        
        for (i = 0; i < p; i++)
        {
            pid_t pid;
            pid = fork();
            if (pid < 0)
            {
                cout << "fork error\n";
                return 0;
            }
            else if (pid == 0)
            {
                int segSize = dimension / p;
                int end = (i == p-1)? dimension: segSize * (i+1);
                for (j = segSize * i; j < end; j++)
                {
                    for (k = 0; k < dimension; k++)
                    {
                        result[k + j*dimension] = 0;
                        for (l = 0; l < dimension; l++)
                            result[k + j*dimension] += matrix[l + j*dimension] * matrix[k + l*dimension];
                    }
                }
                shmdt(result);
                return 0;
            }
            else
            {
                pids[i] = pid;
            }
        }
        
        for (i = 0; i < p; i++)
            waitpid(pids[i], NULL, 0);
        
        gettimeofday(&end, 0);
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        usleep(100000);
        
        unsigned int checksum = 0;
        for (i = 0; i < dimension; i++)
        {
            for (j = 0; j < dimension; j++)
            {
                checksum += result[i*dimension + j];
            }
        }
        
        cout << p << "-process, checksum = " << checksum << "\nelapsed " << (float) sec + (usec/1000000.0) << " s\n";
    }

    shmdt(result) ;
    shmctl(shmid, IPC_RMID, NULL) ;
    delete []matrix;
    return 0;
}
