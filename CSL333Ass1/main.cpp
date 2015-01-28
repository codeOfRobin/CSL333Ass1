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

using namespace std;
FILE *pFile;
float timeInMinutes;//time for program to finish
vector<char> V;
vector<string>inputStrings;
vector<vector<int>> costMatrix;
map<char,int>charToIndex;
int sizeOfVocab,k,numberOfStrings,costOfInsertion;

struct stateOfStrings
{
    vector<string>strings;
    vector<int>indices;
    float costIncurredTillNow;
    float heuristic;
};

float costOfForcefulMatch(int i,string s1,string s2)
{
    return costMatrix[charToIndex[s1[i]]][charToIndex[s2[i]]];
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
    
    
    numberOfStrings=atoi(pch);
    
    for (int i=0; i<numberOfStrings;i++)
    {
        pch = strtok (NULL," \n,");
        inputStrings.push_back(pch);
    }
    
    
    pch = strtok (NULL," \n,");
    costOfInsertion=atof(pch);
    
    costMatrix.resize(sizeOfVocab+1);
    pch = strtok (NULL," \n,");

    for (int i=0; i<sizeOfVocab+1; i++)
    {
        vector<int> v(sizeOfVocab+1);
        for (int j=0; j<sizeOfVocab+1; j++)
        {
            v[j]=atoi(pch);
            pch = strtok (NULL," \n");
        }
        costMatrix[i]=v;
    }
    
    

    // terminate
    fclose (pFile);
}

vector<stateOfStrings> generateOptions(stateOfStrings x)
{
    vector<stateOfStrings> ans;
    for (int i=0; i<x.strings.size(); i++)
    {
//        generate costs here: TODISCUSS: don't create all strings just yet, right?
        
    }
    return ans;
}

int main(int argc, const char * argv[])
{
   
    readText();
    
    return 0;
}
