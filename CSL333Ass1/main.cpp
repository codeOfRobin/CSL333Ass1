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
#include <math.h>
#include <functional>   // std::plus


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
    vector<string>stringsInState;
    vector<long int>indices;
    float costIncurredTillNow;
    vector<vector<int>>hyphenIndices;
    stateOfStrings()
    {
        costIncurredTillNow=0;
    }
};
stateOfStrings y;
stateOfStrings x;
stateOfStrings goal;


vector<int>binaryToIntArray(int x)
{
    string binary = std::bitset<64>(x).to_string(); //to binary
    string substringed=binary.substr(64-inputStrings.size() ,inputStrings.size());
    vector<int> y;
    for (int i=0; i<inputStrings.size(); i++)
    {
        char a=substringed.at(i);
        y.push_back(atoi(&a));
    }
    
//    cout<<y[0]<<y[1]<<y[2]<<endl;
    return y;
}




float costOfForcefulMatch(int i,int j,string s1,string s2)
{
    return costMatrix[charToIndex[s1[i]]][charToIndex[s2[j]]];
}


float calculateCost (stateOfStrings x,stateOfStrings y)//n2k algo, x is final, y is initial
{
    float costIncurred=0;
    if (x.stringsInState.size()!=y.stringsInState.size())
    {
        cout<<"ERROR:both states have unequal sizes. Sab Suarav ki galti hain";
        exit(4);
    }
    else
    {
        for (int i=0; i<x.stringsInState.size(); i++)
        {
            for (int j=i; j<y.stringsInState.size(); j++)
            {
                for (int k=0; k<x.stringsInState[i].size(); k++)
                {
                    costIncurred+=costMatrix[charToIndex[x.stringsInState.at(i)[k]]][charToIndex[y.stringsInState.at(i)[k]]];
                }
            }
        }
    }
    
    return costIncurred;
}





void readText()
{
//    char * dir = getcwd(NULL, 0);
//    cout << "Current dir: " << dir << endl;
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

vector<stateOfStrings> sortedNeighbourHoodFunction(stateOfStrings &x)
{
    vector<stateOfStrings> y;
    x.stringsInState.push_back("asdf_dsa");
    vector<int>intVector;
    intVector.push_back(4);
    x.hyphenIndices.push_back(intVector);
    srand (time(0));
    int randomStringIndex = (rand() % (int)(x.stringsInState.size()));
    int randomCharInSelectedString=(rand() % (int)(x.hyphenIndices[randomStringIndex].size()));
    //hyphen to be removed
    srand(time(0));
    int randomIndexToPutHyphenIn=(rand() %(x.stringsInState[randomStringIndex].size()));
    string copy=x.stringsInState[randomStringIndex];
    copy=copy.substr(0,x.hyphenIndices[randomStringIndex][randomCharInSelectedString])+copy.substr(x.hyphenIndices[randomStringIndex][randomCharInSelectedString]+1,x.stringsInState.size()-x.hyphenIndices[randomStringIndex][randomCharInSelectedString]);
    copy.insert(randomIndexToPutHyphenIn-1, "_");
    x.hyphenIndices[randomStringIndex][randomCharInSelectedString]=randomIndexToPutHyphenIn;
    x.stringsInState[randomStringIndex]=copy;
    return y;
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
    

    x=y;
    x.indices.at(2)=1;
    
    for (int i=0; i<inputStrings.size(); i++)
    {
        goal.indices.push_back(inputStrings.at(i).size());
    }
    calculateCost(x, y);
//    cout<<x.costIncurredTillNow;
    
    
    
    
    
    
    
    stateOfStrings x;
    x.indices.push_back(7);
    x.indices.push_back(6);
    x.indices.push_back(0);
    stateOfStrings y;
    sortedNeighbourHoodFunction(y);
    return 0;
}
