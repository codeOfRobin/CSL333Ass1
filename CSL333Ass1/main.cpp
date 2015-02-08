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
#include <functional>   // std::plus


using namespace std;
FILE *pFile;
float timeInMinutes;//time for program to finish
vector<char> V;
vector<string>inputStrings;
vector<vector<int>> costMatrix;
map<char,int>charToIndex;
int sizeOfVocab,k,numberOfStrings,costOfInsertion;
float avgMisMatchCost;
map<vector<int>, int>indexToCostMatrix;
vector<vector<vector<int>>> allOfTheCosts;
vector<vector<vector<vector<string>>>> allOfthePartialStrings;

struct stateOfStrings
{
    vector<long int>indices;
    vector<int>charsAtIndex;
    float costIncurredTillNow;
    stateOfStrings()
    {
        costIncurredTillNow=0;
        visited=false;
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

vector<stateOfStrings> blackBox(stateOfStrings initial)//http://community.topcoder.com/tc?module=Static&d1=tutorials&d2=bitManipulation
{
    vector<stateOfStrings> x;
    
    long int noOfChildren=pow(2, inputStrings.size())-1;
    for (int i=1; i<=noOfChildren; i++)
    {
        stateOfStrings child;
        child.indices.resize(initial.indices.size());
        vector<int> binaryVector=binaryToIntArray(i);
//        transform(initial.indices.begin(), initial.indices.end(), binaryVector.begin(), child.indices.begin(),std::plus<int>());
        
//        for (int j=0; j<inputStrings.size(); j++)
//        {
//            cout<<binaryVector.at(j);
//        }
        
        for (int j=0; j<inputStrings.size(); j++)
        {
          
            child.indices.at(j)=initial.indices.at(j)+binaryVector.at(j);
            long int computedSize=initial.indices.at(j)+binaryVector.at(j);

        }
        
        bool flag=true;
        for (int j=0; j<inputStrings.size(); j++)
        {
            if (inputStrings.at(j).size()<child.indices.at(j))
            {
                flag=false;
            }
        }
        
        if (flag)
        {
            x.push_back(child);
        }
        
    }
    
    x.shrink_to_fit();

    return x;
}

bool validateGoal(vector<long> x)
{
    string binary = std::bitset<64>(x).to_string(); //to binary
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

//    cout<<s1[i]<<endl<<s2[j]<<endl;
//    cout<<s1<<s2<<endl;
//    cout<<charToIndex[s1[i]]<<endl<<charToIndex[s2[j]]<<endl;
//    cout<<costMatrix[charToIndex[s1[i]]][charToIndex[s2[j]]];
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
            if (i!=j)
            {
//                cout<<costMatrix[x.charsAtIndex.at(j)][sizeOfVocab-1];
                x.costIncurredTillNow+=costMatrix[x.charsAtIndex.at(j)][sizeOfVocab-1];
            }
        }
    }
    
    return cost_of_transition;
    
}

vector<stateOfStrings> blackBox(stateOfStrings initial)
{
    vector<stateOfStrings> x;
    
    long int noOfChildren=pow(2, inputStrings.size())-1;
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
    int sumOfAll=0;
    int sum_of_elems =accumulate(distanceToGoal.begin(),distanceToGoal.end(),0);
    valueOfCost+=(maximum*inputStrings.size()-sum_of_elems)*costOfInsertion;
    return valueOfCost;
}



    return heuristic(a)>heuristic(b);

}
int min(int a, int b)
{
    return (a < b)? a : b;
}

void pairwiseCost() // DP algorithm for constructing pairwise matrices
{
   int k = inputStrings.size();
   //int k=2;
    int numberOfMatrices = k*(k-1)/2;
    
    allOfTheCosts.resize(numberOfMatrices);
    allOfthePartialStrings.resize(numberOfMatrices);
    int loopCount=0;
    for (int p=0; p<k-1; p++) {
        for (int q=p+1; q<k; q++) {
            int firstIndex=p;
            int secondIndex=q;
            vector<vector<int>>bestCostMatrix;
//            vector<vector<vector<string>>> partialStringsInAPlane;
            int m=inputStrings.at(firstIndex).size();
            int n=inputStrings.at(secondIndex).size();
            bestCostMatrix.resize(m+1);
//            partialStringsInAPlane.resize(m+1);
            for (int i=m; i>=0; i--)
            {
                vector<int> temporary(n+1);
//                vector<vector<string>> stringsAtaColumn(n+1);
                bestCostMatrix.at(i)=temporary;
//                partialStringsInAPlane.at(i) = stringsAtaColumn;
                for (int j=n; j>=0; j--)
                {
//                    vector<string> stringsForThisPoint(2);
//                    partialStringsInAPlane.at(i).at(j) = stringsForThisPoint;
                    if (i == m && j == n)
                    {
                        bestCostMatrix.at(i).at(j) = 0;
                      // partialStringsInAPlane.at(i).at(j).at(0)="";
                      // partialStringsInAPlane.at(i).at(j).at(1)="";
                    }
                    else if (i==m && j!=n)
                    {
                        bestCostMatrix.at(i).at(j) = bestCostMatrix.at(i).at(j+1)+costMatrix[charToIndex[inputStrings.at(secondIndex).at(j)]][sizeOfVocab-1];
                      //  partialStringsInAPlane.at(i).at(j).at(0)="-"+partialStringsInAPlane.at(i).at(j+1).at(0);
                      //  partialStringsInAPlane.at(i).at(j).at(1)=inputStrings.at(secondIndex).at(j)+partialStringsInAPlane.at(i).at(j+1).at(1);
                    }
                    else if (i!=m && j==n)
                    {
                        bestCostMatrix.at(i).at(j) = bestCostMatrix.at(i+1).at(j)+costMatrix[charToIndex[inputStrings.at(firstIndex).at(i)]][sizeOfVocab-1];
                       // partialStringsInAPlane.at(i).at(j).at(0)=inputStrings.at(firstIndex).at(i)+partialStringsInAPlane.at(i+1).at(j).at(0);
                       // partialStringsInAPlane.at(i).at(j).at(1)="-"+partialStringsInAPlane.at(i+1).at(j).at(1);
                    }
                    else
                    {
                        int diagonal = bestCostMatrix.at(i+1).at(j+1)+costMatrix[charToIndex[inputStrings.at(firstIndex).at(i)]][charToIndex[inputStrings.at(secondIndex).at(j)]];
                        int fromRight = bestCostMatrix.at(i+1).at(j)+costMatrix[charToIndex[inputStrings.at(firstIndex).at(i)]][sizeOfVocab-1];//+costOfInsertion;
                        int fromDown = bestCostMatrix.at(i).at(j+1)+costMatrix[charToIndex[inputStrings.at(secondIndex).at(j)]][sizeOfVocab-1];//+costOfInsertion;
                        int theMinimum = min(diagonal,min(fromDown,fromRight));
                        bestCostMatrix.at(i).at(j) = theMinimum;
//                        if (theMinimum == fromRight) {
//                            partialStringsInAPlane.at(i).at(j).at(0)=inputStrings.at(firstIndex).at(i)+partialStringsInAPlane.at(i+1).at(j).at(0);
//                            partialStringsInAPlane.at(i).at(j).at(1)="-"+partialStringsInAPlane.at(i+1).at(j).at(1);
//                        }
//                        else if (theMinimum == fromDown){
//                            partialStringsInAPlane.at(i).at(j).at(0)="-"+partialStringsInAPlane.at(i).at(j+1).at(0);
//                            partialStringsInAPlane.at(i).at(j).at(1)=inputStrings.at(secondIndex).at(j)+partialStringsInAPlane.at(i).at(j+1).at(1);
//                        }
//                        else{
//                            partialStringsInAPlane.at(i).at(j).at(0)=inputStrings.at(firstIndex).at(i)+partialStringsInAPlane.at(i+1).at(j+1).at(0);
//                            partialStringsInAPlane.at(i).at(j).at(1)=inputStrings.at(secondIndex).at(j)+partialStringsInAPlane.at(i+1).at(j+1).at(1);
//                        }
                        
                    }
//                    cout<<"<<<<<<<<<<<<<<Yeh raha ek point>>>>>>>>>>>>>>>"<<endl;
//                    cout<<"bestCost at : "<<i<<" and "<<j<<" : "<<bestCostMatrix.at(i).at(j)<<endl;
//                    cout<<"strings hain first: "<<partialStringsInAPlane.at(i).at(j).at(0)<<endl;
//                    cout<<"strings hain second:"<<partialStringsInAPlane.at(i).at(j).at(1)<<endl;
                }
            }
            allOfTheCosts.at(loopCount)=bestCostMatrix;
            //allOfthePartialStrings.at(loopCount)=partialStringsInAPlane;
            loopCount+=1;
           
        }
    }
}

void printPath(stateOfStrings &x)
{
    vector<int> prev;
    vector<string> stringsOfGoal;
    
    vector<string> inputCopy=inputStrings;
    for (int i=0; i<inputStrings.size(); i++)
    {
        prev.push_back(0);
        stringsOfGoal.push_back("");
    }
    for (long int i=0;i<x.pathTillNow.size(); i++)
    {
        cout<<"\n<<<<<<<<<<<<<<<<<<<<GOAL STARTS HERE>>>>>>>>>>>>>>>>>>>>>\n";
        cout<<"Bound :"<<x.costIncurredTillNow<<endl;
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
        
        for (int k=0; k<stringsOfGoal.size(); k++)
        {
            //reverse(stringsOfGoal.at(k).begin(), stringsOfGoal.at(k).end());
            cout<<"string number"<<i<<" "<<stringsOfGoal.at(k)<<endl;
        }
        cout<<"\n<<<<<<<<<<<<<<<<<<<<GOAL ENDS HERE>>>>>>>>>>>>>>>>>>>>>\n";
        
    }
    
}


void dfsBAndB(stateOfStrings &start)//No mausam, this isn't breakfast and bed. Go away.
{
    stack<stateOfStrings> stackThing;
    stateOfStrings current;
    stackThing.push(start);
    float bound=-42;
    current=start;
    while (!stackThing.empty())
    {
        
        if (validateGoal(current.indices))
        {
            stackThing.pop();
            current.costIncurredTillNow=calculateCost(stackThing.top(),current);
            if (bound==-42 || current.costIncurredTillNow<=bound)
            {
                bound=current.costIncurredTillNow;
            }

            break;
        }
        else
        {
            vector<stateOfStrings> children;
            children=blackBox(stackThing.top());
            //TODO:in the initial run, only take hueristics into accouny
            for (int i=0; i<children.size(); i++)
            {
                children.at(i).costIncurredTillNow=calculateCost(children.at(i), stackThing.top());
            }
            sort(children.begin(), children.end(), compareHeuristics);
        }
        
    }
    
    bound=goal.costIncurredTillNow;
    //TODO:Parent bullshit
    
    //after this:add stuff for bounding and shit
    
    
}

int main(int argc, const char * argv[])
{
    clock_t tStart = clock();
    readText();
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
    calculateCost(x, y);
//    cout<<x.costIncurredTillNow;
    
    
    
    
    
    
    
    stateOfStrings x;
    x.indices.push_back(7);
    x.indices.push_back(6);
    x.indices.push_back(0);
    blackBox(x);
    
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}
