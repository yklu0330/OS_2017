//
//  main.cpp
//  OS_HW2-1
//
//  Created by 呂翊愷 on 2017/11/2.
//  Copyright © 2017年 LU YI-KAI. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

int main(int argc, const char * argv[])
{
    ifstream ifs;
    ifs.open("Q1.txt", ios::in);
    int num;
    ifs >> num;
    
    int *burst = new int[num];
    int *wait = new int[num];
    int *turn = new int[num];
    
    int i;
    for (i = 0; i < num; i++)
    {
        ifs >> burst[i];
        wait[i] = 0;
    }
    
    int waitTotal = 0;
    int turnTotal = 0;
    for (i = 1; i < num; i++)
    {
        wait[i] += wait[i-1] + burst[i-1];
        waitTotal += wait[i];
    }
    for (i = 0; i < num; i++)
    {
        turn[i] = wait[i] + burst[i];
        turnTotal += turn[i];
    }
    
    printf("Process\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < num; i++)
        printf("P[%d]\t%d\t\t%d\n", i+1, wait[i], turn[i]);
    printf("\nAverage waiting time : %.6f", (double)waitTotal/num);
    printf("\nAverage turnaround time : %.6f\n", (double)turnTotal/num);
    
    delete [] burst;
    delete [] wait;
    delete [] turn;
    ifs.close();
    return 0;
}
