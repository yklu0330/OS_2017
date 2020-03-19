//
//  main.cpp
//  OS_HW2-4
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
    ifs.open("Q4.txt", ios::in);
    int num;
    ifs >> num;
    
    int arrival[num], burst[num], end1[num], end2[num], wait[num], turn[num], priority[num];
    int high, medium;
    int time = 0;
    bool used[num];
    int waitTotal = 0, turnTotal = 0, burstTotal = 0;
    
    int i, j;
    for (i = 0; i < num; i++)
        ifs >> arrival[i];
    for (i = 0; i < num; i++)
        ifs >> burst[i];
    ifs >> high >> medium;
    for (i = 0; i < num; i++)
    {
        wait[i] = 0;
        priority[i] = 1;
        used[i] = false;
        burstTotal += burst[i];
    }
    
    bool newArrival = false;
    int usedNum = 0;
    while (1)
    {
        for (i = 0; i < burstTotal; i++) //high priority queue
        {
            int idx = 0, min = 99999999;
            for (j = 0; j < num; j++) //find the smallest arrival time process
            {
                if (priority[j] == 2 || used[j] == true)
                    continue;
                if (arrival[j] <= time && priority[j] == 1)
                {
                    if (arrival[j] < min)
                    {
                        idx = j;
                        min = arrival[j];
                    }
                }
            }
            if (j == num && min == 99999999)
                break;
            if (burst[idx] <= high)
            {
                time += burst[idx];
                wait[idx] += time - burst[idx] - arrival[idx];
                burst[idx] = 0;
                used[idx] = true;
                turn[idx] = time - arrival[idx];
            }
            else
            {
                burst[idx] -= high;
                time += high;
                wait[idx] += time - high - arrival[idx];
            }
            end1[idx] = time;
            priority[idx] = 2;
        }
        for (i = 0; i < burstTotal; i++) //medium priority queue
        {
            for (j = 0; j < num; j++) //find if there is any new arrival process
            {
                if (arrival[j] <= time && priority[j] == 1)
                {
                    newArrival = true;
                    break;
                }
            }
            if (newArrival == true)
                break;
            
            int idx = 0, min = 99999999;
            for (j = 0; j < num; j++) //find the smallest arrival time process
            {
                if (priority[j] == 3 || used[j] == true)
                    continue;
                if (arrival[j] <= time && priority[j] == 2)
                {
                    if (arrival[j] < min)
                    {
                        idx = j;
                        min = arrival[j];
                    }
                }
            }
            if (j == num && min == 99999999)
                break;
            if (burst[idx] <= medium)
            {
                time += burst[idx];
                wait[idx] += time - end1[idx] - burst[idx];
                burst[idx] = 0;
                used[idx] = true;
                turn[idx] = time - arrival[idx];
            }
            else
            {
                burst[idx] -= medium;
                time += medium;
                wait[idx] += time - end1[idx] - medium;
            }
            
            end2[idx] = time;
            priority[idx] = 3;
            
            
        }
        if (newArrival == true)
        {
            newArrival = false;
            continue;
        }
        
        for (i = 0; i < burstTotal; i++) //low priority queue
        {
            for (j = 0; j < num; j++) //find if there is any new arrival process
            {
                if (arrival[j] <= time && (priority[j] == 1 || priority[j] == 2) && used[j] == false)
                {
                    newArrival = true;
                    break;
                }
            }
            if (newArrival == true)
                break;
            
            int idx = 0, min = 99999999;
            for (j = 0; j < num; j++) //find the smallest burst time process
            {
                if (used[j] == true)
                    continue;
                if (arrival[j] <= time && priority[j] == 3)
                {
                    if (burst[j] < min)
                    {
                        idx = j;
                        min = burst[j];
                    }
                }
            }
            if (j == num && min == 99999999)
                break;
            time += burst[idx];
            wait[idx] += time - end2[idx] - burst[idx];
            burst[idx] = 0;
            turn[idx] = time - arrival[idx];
            used[idx] = true;
            
        }
        if (newArrival == true)
        {
            newArrival = false;
            continue;
        }
        
        for (j = 0; j < num; j++) //find if all processes finished
        {
            if (used[j] == true)
            {
                usedNum++;
            }
        }
        if (usedNum == num)
            break;
    }
    
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
