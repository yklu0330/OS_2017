//
//  main.cpp
//  OS_HW2-3
//
//  Created by 呂翊愷 on 2017/11/3.
//  Copyright © 2017年 LU YI-KAI. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

int main(int argc, const char * argv[])
{
    ifstream ifs;
    ifs.open("Q3.txt", ios::in);
    int num;
    ifs >> num;
    
    int arrival[num], burst[num], wait[num], turn[num];
    int time = 0;
    bool used[num];
    int burstTotal = 0;
    
    int i, j;
    for (i = 0; i < num; i++)
        ifs >> arrival[i];
    for (i = 0; i < num; i++)
        ifs >> burst[i];
    for (i = 0; i < num; i++)
        wait[i] = 0;
    for (i = 0; i < num; i++)
        used[i] = false;
    for (i = 0; i < num; i++)
        burstTotal += burst[i];
    int idx = 0, min = 99999999;
    for (i = 0; ; i++)
    {
        for (j = 0; j < num; j++) //find the smallest burst time
        {
            if (used[j] == true)
                continue;
            if (arrival[j] <= time)
            {
                if (burst[j] == 0)
                {
                    wait[j] = 0;
                    turn[j] = 0;
                    used[j] = true;
                    continue;
                }
                if (burst[j] < min && burst[j] > 0)
                {
                    idx = j;
                    min = burst[j];
                }
            }
        }
        time++;
        if (min != 99999999)
            burst[idx]--;
        for (j = 0; j < num; j++)
        {
            if (j == idx)
                continue;
            if (arrival[j] < time && used[j] == false)
            {
                wait[j]++;
            }
        }
        if (burst[idx] == 0 && used[idx] == false)
        {
            used[idx] = true;
            turn[idx] = time - arrival[idx];
            idx = 0;
            min = 99999999;
        }
        
        
        int usedNum = 0;
        for (j = 0; j < num; j++)
            if (used[j] == true)
                usedNum++;
        if (usedNum == num)
            break;
    }
    
    int waitTotal = 0, turnTotal = 0;
    for (i = 0; i < num; i++)
    {
        waitTotal += wait[i];
        turnTotal += turn[i];
    }
    
    printf("Process\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < num; i++)
        printf("P[%d]\t%d\t\t%d\n", i + 1, wait[i], turn[i]);
    printf("\nAverage waiting time : %.6f", (double)waitTotal/num);
    printf("\nAverage turnaround time : %.6f\n", (double)turnTotal/num);
    ifs.close();
    return 0;
}
