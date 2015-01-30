//
//  main.cpp
//  CSL333Ass1
//
//  Created by Robin Malhotra on 25/01/15.
//  Copyright (c) 2015 Batman And Robin. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <stack>
#include <algorithm>

using namespace std;
FILE *pFile;
float timeInMinutes;//time for program to finish
vector<char> V;
vector<string>inputStrings;
vector<vector<int>> costMatrix;
map<char,int>charToIndex;
int sizeOfVocab,k,numberOfStrings,costOfInsertion;
float avgMisMatchCost;
struct stateOfStrings
{
    vector<long int>indices;
    vector<int>charsAtIndex;
    float costIncurredTillNow;
    stateOfStrings *parent;
    stateOfStrings()
    {
        costIncurredTillNow=0;
    }
};
stateOfStrings y;
stateOfStrings x;
stateOfStrings goal;

bool validateGoal(vector<long> x)
{
    for (int i=0; i<x.size(); i++)
    {
        if (inputStrings.at(i).length()!=x.at(i))
        {
            return false;
        }
    }
    
    return true;
}

float costOfForcefulMatch(int i,int j,string s1,string s2)
{

    cout<<s1[i]<<endl<<s2[j]<<endl;
    cout<<s1<<s2<<endl;
    cout<<charToIndex[s1[i]]<<endl<<charToIndex[s2[j]]<<endl;
    cout<<costMatrix[charToIndex[s1[i]]][charToIndex[s2[j]]];
    return costMatrix[charToIndex[s1[i]]][charToIndex[s2[j]]];
}



float calculateCost (stateOfStrings &x,stateOfStrings &y)//n2k algo, x is final, y is initial
{
    x.costIncurredTillNow=y.costIncurredTillNow;
    
    x.charsAtIndex.resize(inputStrings.size());
    for (int i=0; i<inputStrings.size(); i++)
    {
        if (x.indices.at(i)==y.indices.at(i))
        {
            x.charsAtIndex.at(i)=sizeOfVocab-1;
            x.costIncurredTillNow+=3;
        }
        else
        {
            x.charsAtIndex.at(i)=charToIndex[inputStrings.at(i)[x.indices.at(i)]];
        }
    }
    
    
    for (int i=0; i<inputStrings.size(); i++)
    {
    
        for (int j=i+1; j<inputStrings.size(); j++)
        {
            if (i!=j)
            {
                cout<<costMatrix[x.charsAtIndex.at(j)][sizeOfVocab-1];
                x.costIncurredTillNow+=costMatrix[x.charsAtIndex.at(j)][sizeOfVocab-1];
            }
        }
    }
  
    return y.costIncurredTillNow;
    
}




void readText()
{
    char * dir = getcwd(NULL, 0);
    cout << "Current dir: " << dir << endl;
    pFile = fopen ( "./input.txt" , "rb" );
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}
    
    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    long lSize = ftell (pFile);
    rewind (pFile);
    
    // allocate memory to contain the whole file:
    char *buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
    
    // copy the file into the buffer:
    size_t result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
    
    /* the whole file is now loaded in the memory buffer. */
    
    
    char * pch;
    pch = strtok (buffer," \n,");
    
    timeInMinutes=atof(pch);
    
    pch = strtok (NULL," \n,");
    sizeOfVocab=atoi(pch);
    
    pch = strtok (NULL," \n,");
    for (int i=0; i<sizeOfVocab; i++)
    {
        V.push_back(*pch);
        pch = strtok (NULL," \n,");
    }
    char const *hyphen="-";
    sizeOfVocab++;//for a dash
    V.push_back(*hyphen);
    
    
    numberOfStrings=atoi(pch);
    
    for (int i=0; i<numberOfStrings;i++)
    {
        pch = strtok (NULL," \n,");
        inputStrings.push_back(pch);
    }
    
    
    pch = strtok (NULL," \n,");
    costOfInsertion=atof(pch);
    
    costMatrix.resize(sizeOfVocab);
    pch = strtok (NULL," \n,");

    for (int i=0; i<sizeOfVocab; i++)
    {
        vector<int> v(sizeOfVocab);
        for (int j=0; j<sizeOfVocab; j++)
        {
            v[j]=atoi(pch);
            pch = strtok (NULL," \n");
        }
        costMatrix[i]=v;
    }
    
    

    // terminate
    fclose (pFile);
}




float heuristic(stateOfStrings z)
{
    float max=0;
    for (int i=0; i<inputStrings.size(); i++)
    {
        if (goal.indices.at(i)-z.indices.at(i)>max)
        {
            max=goal.indices.at(i)-z.indices.at(i);
        }
    }
    return max*avgMisMatchCost;
}

vector<stateOfStrings> blackBox(stateOfStrings initial)
{
    vector<stateOfStrings> x;
    return x;
}


bool compareHeuristics(stateOfStrings &a,stateOfStrings &b)
{
    return heuristic(a)>heuristic(b);
}

void dfsBAndB(stateOfStrings &start,stateOfStrings &goal)//No mausam, this isn't breakfast and bed. Go away.
{
    stack<stateOfStrings> stackThing;
    stateOfStrings foundGoal;
    stateOfStrings initial;
    stackThing.push(initial);
    while (!stackThing.empty())
    {
        
        if (validateGoal(initial.indices))
        {
            foundGoal=initial;
//            foundGoal.costIncurredTillNow=calculateCost(<#stateOfStrings &x#>, <#stateOfStrings &y#>) need to discuss cost with saurav
            
            break;
        }
        stackThing.pop();
        vector<stateOfStrings> children;
        for (int i=0; i<children.size(); i++)
        {
            children.at(i).costIncurredTillNow=calculateCost(children.at(i), initial);
        }
//        children=blackBox(initial)
        sort(children.begin(), children.end(), compareHeuristics);
        
    }
    
    float bound=foundGoal.costIncurredTillNow;
    //TODO:Parent bullshit
    
    //after this:add stuff for bounding and shit
    
    
}

int main(int argc, const char * argv[])
{
   
    readText();
    for (int i=0; i<inputStrings.size(); i++)
    {
        y.indices.push_back(0);
    }
    
    
    for (int i=0; i<sizeOfVocab; i++)
    {
        charToIndex[V.at(i)]=i;
    }
    
    for (int i=1; i<costMatrix.size(); i++)
    {
        for (int j=0; j<i; j++)
        {
            avgMisMatchCost+=costMatrix[i][j];
        }
    }
    avgMisMatchCost=avgMisMatchCost/(sizeOfVocab*(sizeOfVocab-1)/2);
    x=y;
    x.indices.at(2)=1;
    
    for (int i=0; i<inputStrings.size(); i++)
    {
        goal.indices.push_back(inputStrings.at(i).size());
    }
    calculateCost(x, y);
    cout<<x.costIncurredTillNow;
    
    
    
    return 0;
}
