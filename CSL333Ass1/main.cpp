//
//  main.cpp
//  CSL333Ass1
//
//  Created by Robin Malhotra on 25/01/15.
//  Copyright (c) 2015 Batman And Robin. All rights reserved.
//

#include <fstream>//≤
#include <iostream>
#include <vector>
#include <unistd.h>
#include <map>
#include <stack>
#include <algorithm>
#include <math.h>
#include <functional> 
#include <numeric>// std::plus
#include <time.h>
#include <bitset>
#include <stdlib.h>
#include <string.h>


using namespace std;


FILE *pFile;
float timeInMinutes;//time for program to finish
vector<char> V;
vector<string>inputStrings;
vector<vector<int> > costMatrix;
map<char,int>charToIndex;
int sizeOfVocab,k,numberOfStrings,costOfInsertion;
float avgMisMatchCost;
map<vector<int>, int>indexToCostMatrix;
vector<vector<vector<int> > > allOfTheCosts;

struct stateOfStrings
{
    vector<long int>indices;
    vector<int>charsAtIndex;
    float costIncurredTillNow;
    bool visited;
    vector<vector<long int> >pathTillNow;
    float heuristicValue;

    stateOfStrings()
    {
        costIncurredTillNow=0;
        visited=false;
        heuristicValue = 0;
    }
};

stateOfStrings y;
stateOfStrings x;
stateOfStrings goal;


vector<int>binaryToIntArray(int x)
{
    string binary = bitset<64>(x).to_string(); //to binary
    string substringed=binary.substr(64-inputStrings.size() ,inputStrings.size());
    vector<int> y;
    for (int i=0; i<inputStrings.size(); i++)
    {
        char a=substringed.at(i);
        y.push_back(atoi(&a));
    }
    
    return y;
}

float calculateCost (stateOfStrings x,stateOfStrings y)//n2k algo, x is final, y is initial
{
    float cost_of_transition=0;
    y.charsAtIndex.resize(inputStrings.size());
    for (int i=0; i<inputStrings.size(); i++)
    {
        if (x.indices.at(i)==y.indices.at(i))
        {
            y.charsAtIndex.at(i)=sizeOfVocab-1;
            cost_of_transition+=costOfInsertion;
        }
        else
        {
            y.charsAtIndex.at(i)=charToIndex[inputStrings.at(i)[y.indices.at(i)]];
        }
    }
    
    
    for (int i=0; i<inputStrings.size(); i++)
    {
        
        for (int j=i+1; j<inputStrings.size(); j++)
        {
            
            cost_of_transition+=costMatrix[y.charsAtIndex.at(j)][y.charsAtIndex.at(i)];
            
        }
    }
    
    return cost_of_transition;
    
}

float heuristic(stateOfStrings &z)
{
    int k=inputStrings.size();
    int valueOfCost=0;
    for (int p=0; p<k-1; p++) {
        for (int q=p+1;q<k; q++) {
            int indexOfRelevantMatrix = ((k-1)*p)+q-(p*(p+1)/2)-1;
            valueOfCost+=allOfTheCosts.at(indexOfRelevantMatrix).at(z.indices.at(p)).at(z.indices.at(q));
        }
    }
    vector<int> distanceToGoal(inputStrings.size());
    int maximum =0;
    for (int dummy=0; dummy<inputStrings.size(); dummy++) {
        int distanceAtDummy = goal.indices.at(dummy)-z.indices.at(dummy);
        distanceToGoal.at(dummy)=distanceAtDummy;
        if (distanceAtDummy>maximum) {
            maximum = distanceAtDummy;
        }
    }
    int sum_of_elems =accumulate(distanceToGoal.begin(),distanceToGoal.end(),0);
    valueOfCost+=(maximum*inputStrings.size()-sum_of_elems)*costOfInsertion;
    return valueOfCost;
}

vector<stateOfStrings> blackBox(stateOfStrings initial)
{
    vector<stateOfStrings> x;
    
    long int noOfChildren=pow(2, inputStrings.size())-1;
    //cout<<noOfChildren;
    for (int i=1; i<=noOfChildren; i++)
    {
        stateOfStrings child;
        child.indices.resize(initial.indices.size());
        vector<int> binaryVector=binaryToIntArray(i);
       
        for (int j=0; j<inputStrings.size(); j++)
        {
          
            child.indices.at(j)=initial.indices.at(j)+binaryVector.at(j);
        }
        
        bool flag=true;
        for (int j=0; j<inputStrings.size(); j++)
        {
            if (inputStrings.at(j).size()<child.indices.at(j))
            {
                flag=false;
                break;
            }
        }
        
        if (flag)
        {
            child.costIncurredTillNow=initial.costIncurredTillNow+calculateCost(child, initial);
            child.pathTillNow=initial.pathTillNow;
            child.pathTillNow.push_back(child.indices);
            child.heuristicValue = heuristic(child);
            x.push_back(child);
        }
        
    }
    return x;
}

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


void readText(char* inputPath)
{
//    char * dir = getcwd(NULL, 0);
//   cout << "Current dir: " << dir << endl;
    pFile = fopen ( inputPath , "rb" );
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


// bool compareHeuristics(const stateOfStrings &a, const stateOfStrings &b){

//     return heuristic(a)>heuristic(b);

// }

struct compareHeuristics {
    bool operator()(const stateOfStrings& lhs, const stateOfStrings& rhs) const {
        return lhs.heuristicValue > rhs.heuristicValue;
    }
};

int min(int a, int b)
{
    return (a < b)? a : b;
}

void pairwiseCost() // DP algorithm for constructing pairwise matrices
{
   int k = inputStrings.size();
    int numberOfMatrices = k*(k-1)/2;
    
    allOfTheCosts.resize(numberOfMatrices);
    int loopCount=0;
    for (int p=0; p<k-1; p++) {
        for (int q=p+1; q<k; q++) {
            int firstIndex=p;
            int secondIndex=q;
            vector<vector<int> >bestCostMatrix;
            int m=inputStrings.at(firstIndex).size();
            int n=inputStrings.at(secondIndex).size();
            bestCostMatrix.resize(m+1);
            for (int i=m; i>=0; i--)
            {
                vector<int> temporary(n+1);
                bestCostMatrix.at(i)=temporary;
                for (int j=n; j>=0; j--)
                {
                    if (i == m && j == n)
                    {
                        bestCostMatrix.at(i).at(j) = 0;
                    }
                    else if (i==m && j!=n)
                    {
                        bestCostMatrix.at(i).at(j) = bestCostMatrix.at(i).at(j+1)+costMatrix[charToIndex[inputStrings.at(secondIndex).at(j)]][sizeOfVocab-1];
                    }
                    else if (i!=m && j==n)
                    {
                        bestCostMatrix.at(i).at(j) = bestCostMatrix.at(i+1).at(j)+costMatrix[charToIndex[inputStrings.at(firstIndex).at(i)]][sizeOfVocab-1];
                    }
                    else
                    {
                        int diagonal = bestCostMatrix.at(i+1).at(j+1)+costMatrix[charToIndex[inputStrings.at(firstIndex).at(i)]][charToIndex[inputStrings.at(secondIndex).at(j)]];
                        int fromRight = bestCostMatrix.at(i+1).at(j)+costMatrix[charToIndex[inputStrings.at(firstIndex).at(i)]][sizeOfVocab-1];//+costOfInsertion;
                        int fromDown = bestCostMatrix.at(i).at(j+1)+costMatrix[charToIndex[inputStrings.at(secondIndex).at(j)]][sizeOfVocab-1];//+costOfInsertion;
                        int theMinimum = min(diagonal,min(fromDown,fromRight));
                        bestCostMatrix.at(i).at(j) = theMinimum;

                        
                    }
                }
            }
            allOfTheCosts.at(loopCount)=bestCostMatrix;
            loopCount+=1;
           
        }
    }
}

vector<string> stringsOfGoal;


void printPath(stateOfStrings &x)
{
    vector<int> prev;
    
    vector<string> inputCopy=inputStrings;
    stringsOfGoal.clear();
    for (int i=0; i<inputStrings.size(); i++)
    {
        prev.push_back(0);
        stringsOfGoal.push_back("");
    }
    for (long int i=0;i<x.pathTillNow.size(); i++)
    {
        for (long int j=0 ; j<x.pathTillNow.at(i).size(); j++)
        {
            if (prev.at(j)==x.pathTillNow[i][j])
            {
                stringsOfGoal.at(j).push_back('-');
            }
            else
            {
                stringsOfGoal.at(j).push_back(inputCopy.at(j).front());
                inputCopy.at(j)=inputCopy.at(j).substr(1,inputCopy.at(j).size()-1);
            }
            
        }
        
        for (int k=0; k<x.pathTillNow.at(i).size(); k++)
        {
            prev[k]=x.pathTillNow[i][k];
        }
        
        
        
    }
    
}


stateOfStrings finalGoal;

void dfsBAndB(stateOfStrings &start)
{
    stack<stateOfStrings> stackThing;
    stateOfStrings current;
    current=start;
    stackThing.push(current);
   
    float bound=140;
    //int numberOfNodes
    int count=0;
    while (!stackThing.empty())
    {
        count++;
       // bool debug = current.visited;
        current = stackThing.top();
        stackThing.pop();
      //  cout<<debug<<endl;
        if (!current.visited){
            
            if (validateGoal(current.indices))
            {
                
                if(current.costIncurredTillNow<bound){
                    bound=current.costIncurredTillNow;
                    finalGoal=current;
                }
            }
            else
            {
                if((current.costIncurredTillNow+heuristic(current))<=bound){
                    vector<stateOfStrings> children;
                    children=blackBox(current);
                    sort(children.begin(), children.end(), compareHeuristics());
                    for (int k=0; k<children.size(); k++) {
                        
                        stackThing.push(children.at(k));
                        
                    }
                }
               
            }
            current.visited=true;
        }
        else{
            
        }
        
        
    }
    cout<<"Number of nodes: "<<count<<endl;
    
    
}

int main(int argc, const char * argv[])
{
    clock_t tStart = clock();
    char inputFilePath[64];
    strcpy(inputFilePath, argv[1]);
    readText(inputFilePath);
    for (int i=0; i<inputStrings.size(); i++)
    {
        y.indices.push_back(0);
    }
    
    
    for (int i=0; i<sizeOfVocab; i++)
    {
        charToIndex[V.at(i)]=i;
    }
    
    
    for (int i=0; i<inputStrings.size(); i++)
    {
        goal.indices.push_back(inputStrings.at(i).size());
    }
    
    
    stateOfStrings x;
    for (int l=0; l<inputStrings.size(); l++) {
        x.indices.push_back(0);
    }
  
    pairwiseCost();
   dfsBAndB(x);
    printPath(finalGoal);
    ofstream myfile;
    myfile.open (argv[2]);

    for (int k=0; k<stringsOfGoal.size(); k++)
    {
        //reverse(stringsOfGoal.at(k).begin(), stringsOfGoal.at(k).end());
        myfile<<stringsOfGoal.at(k)<<endl;
    }
    myfile.close();

    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}
