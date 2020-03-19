//
//  main.cpp
//  OS HW1
//
//  Created by 呂翊愷 on 2017/10/9.
//  Copyright © 2017年 LU YI-KAI. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, const char * argv[])
{
    cout << "Main Process ID : " << getpid() << endl << endl;
    int pid;
    pid = fork();
    int count;
    count = 1;
    if (pid < 0)
    {
        fprintf(stderr, "forked failed");
        exit(-1);
    }
    else if (pid > 0) //parent
    {
        cout << "Fork " << count << ". I'm the child " << pid << ", my parent is " << getpid() << endl;
    }
    else //child
    {
        wait(NULL);
        int pid2 = fork();
        count++;
        int pid3;
        if (pid2 != 0)
        {
            pid3 = fork();
            if (pid3 != 0)
            {
                cout << "Fork " << count << ". I'm the child " << pid2 << ", my parent is " << getpid() << endl;
                if (pid3 != pid2)
                {
                    cout << "Fork " << count << ". I'm the child " << pid3 << ", my parent is " << getpid() << endl;
                }
            }
        }
        if (pid2 == 0 || pid3 == 0)
        {
            wait(NULL);
            int pid4 = fork();
            count++;
            if (pid4 != 0)
                cout << "Fork " << count << ". I'm the child " << pid4 << ", my parent is " << getpid() << endl;
        }
    }
    return 0;
}
