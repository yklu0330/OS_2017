//
//  main.cpp
//  OS_HW2-2
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
    ifs.open("Q2.txt", ios::in);
    int num;
    ifs >> num;
    
    int arrival[num], burst[num], wait[num], turn[num], order[num], finish[num];
    int time = 0;
    bool used[num];
    
    int i, j;
    for (i = 0; i < num; i++)
        ifs >> arrival[i];
    for (i = 0; i < num; i++)
        ifs >> burst[i];
    for (i = 0; i < num; i++)
        used[i] = false;
    
    for (i = 0; ; )
    {
        int usedNum = 0;
        for (j = 0; j < num; j++)
            if (used[j] == true)
                usedNum++;
        if (usedNum == num)
            break;
        
        int idx = 0, min = 99999999;
        for (j = 0; j < num; j++)
        {
            if (used[j] == true)
                continue;
            if (arrival[j] <= time)
            {
                if (burst[j] < min)
                {
                    idx = j;
                    min = burst[j];
                }
            }
        }
        if (min != 99999999)
        {
            time += min;
            finish[idx] = time;
            order[i] = idx;
            //cout <<"time: " << time<<"order: "<< order[i] << " " << endl;
            i++;
            used[idx] = true;
        }
        
        if (min == 99999999)
            time++;
        //cout <<"time: " << time<<"order: "<< order[i] << " " << endl;
    }
    /*for (i = 0; i < num; i++)
        cout << order[i] << " ";*/
    int waitTotal = 0, turnTotal = 0;
    wait[order[0]] = 0;
    turn[order[0]] = burst[order[0]];
    waitTotal += wait[order[0]];
    turnTotal += wait[order[0]] + burst[order[0]];
    
    for (i = 1; i < num; i++)
    {
        int waitTime = 0;
        /*for (j = 0; j < i; j++)
            waitTime += burst[order[j]];*/
        wait[order[i]] = finish[order[i-1]] - arrival[order[i]];
        //cout <<"i: "<<i<<" order: " <<order[i]<<" waittime: " << waitTime << " arrival: " << arrival[order[i]] << endl;
        turn[order[i]] = wait[order[i]] + burst[order[i]];
        waitTotal += wait[order[i]];
        turnTotal += turn[order[i]];
    }
    printf("Process\tWaiting Time\tTurnaround Time\n");
    for (i = 0; i < num; i++)
        printf("P[%d]\t%d\t\t%d\n", i + 1, wait[i], turn[i]);
    printf("\nAverage waiting time : %.6f", (double)waitTotal/num);
    printf("\nAverage turnaround time : %.6f\n", (double)turnTotal/num);
    ifs.close();
    return 0;
}
