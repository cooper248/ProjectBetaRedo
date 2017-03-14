//
//  main.cpp
//  Project Beta
//
//  Created by Cooper Richardson on 2/28/17.
//  Copyright © 2017 Cooper Richardson. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <time.h>

using namespace std;


// Using global variables for all functions
int gridWidth;
int gridHeight;
int cycles = 50000;

// moving agent location
int agentX;
int agentY;

//starting agent location
int agentStartX;
int agentStartY;

int goalX;
int goalY;


#define crRand (double)rand()/RAND_MAX

// obtains width & height - assigns goal and agent locations
void promptUser(){
    srand(time(NULL));
    
    // Input length and width of gridworld from user while width or height greater than 1
    while (gridWidth <=1 && gridHeight <=1){
        cout<<"How wide would you like your grid to be? ";
        cin>>gridWidth;
        cout<< endl;
        cout<<"How tall would you like your grid to be? ";
        cin>>gridHeight;
        cout<< endl<<endl;
        
        if(gridWidth <=1 && gridHeight<=1){
            cout<<"Nice... Why are you trying to break me? This time enter dimensions for a valid grid."<<endl<<endl;
        }
    }
    
    // Create agent and goal locations
    agentY = rand()% gridHeight;
    agentX = rand()% gridWidth;
    
    agentStartX = agentX;
    agentStartY = agentY;
    
    goalY = rand()% gridHeight;
    goalX = rand()% gridWidth;
    
    // If agent and goal are in same location, relocate
    while( agentY == goalY && agentX == goalX ){
        
        agentY = rand()% gridHeight;
        agentX = rand()% gridWidth;
        
        agentStartX = agentX;
        agentStartY = agentY;
        
        goalY = rand()% gridHeight;
        goalX= rand()% gridWidth;
    }

};

// Initalizes following grids: reward, states, q values
void initGrids( vector<vector<int>>* rewards, vector<vector<int>>* states, vector<vector<double>>* qTable, vector<vector<char>>* visualGrid ){
    
    //initialize reward grid
    vector<int> rewardRows;
    rewardRows.clear();
    
    // Create gridworld given user inputs
    for(int i=0;i<gridHeight;i++){
        for(int j=0;j<gridWidth;j++){
            
            if(goalY==i && goalX==j){
            rewardRows.push_back(100);
            }
            else{
                rewardRows.push_back(-10);
            }
        }
        rewards->push_back(rewardRows);
        rewardRows.clear();
    }
    
    // initialize states
    vector<int> stateRows;
    stateRows.clear();
    int state=0;
    
    for(int i=0;i<gridHeight;i++)
    {
        for(int j=0;j<gridWidth;j++)
        {
            stateRows.push_back(state);
            state+=1;
        }
        states->push_back(stateRows);
        stateRows.clear();
    }
    
    // initialize q table (state and action)
    vector<double> qTableRows;
    
    for(int i=0;i<gridHeight*gridWidth;i++){
        for(int j=0;j<4;j++)
        {
            qTableRows.push_back(crRand);
        }
        qTable->push_back(qTableRows);
        qTableRows.clear();
    }
    
    // initialize visual grid
    vector<char> visualRows;
    visualRows.clear();
    char placeHolder = '-';
    
    for(int i=0;i<gridHeight;i++)
    {
        for(int j=0;j<gridWidth;j++)
        {
            if(i==agentY && j ==agentX){
            visualRows.push_back('A');
            }
            else if(i==goalY && j ==goalX){
                visualRows.push_back('G');
            }
            else{
                visualRows.push_back(placeHolder);
            }
        }
        visualGrid->push_back(visualRows);
        visualRows.clear();
    }
    
    cout<<"Agent state loc: "<<states->at(agentY)[agentX]<<endl;
    cout<<"Goal state loc: "<<states->at(goalY)[goalX]<<endl;
    
};

// figures out what current state is
int sense(vector<vector<int>>* states){
    int state;
    assert(agentY <= states->size());
    state = states->at(agentY)[agentX];
    
    return state;
};


int decide(vector<vector<double>>* qValueGrid, int state){
    
    int choice = -1;
    int best = -100;
    double explore = .2;
    double random = crRand;
    
   
        //Check up [0]
        if(best<qValueGrid->at(state)[0] && agentY != gridHeight-1){
            choice=0;
            best = qValueGrid->at(state)[0];
        }
        //Check right [1]
        if(best<qValueGrid->at(state)[1] && agentX != gridWidth-1){
            choice=1;
            best = qValueGrid->at(state)[1];
        }
        //Check down [2]
        if(best<qValueGrid->at(state)[2] && agentY != 0){
            choice=2;
            best = qValueGrid->at(state)[2];
        }
        //Check left [3]
        if(best<qValueGrid->at(state)[3] && agentX != 0){
            choice=3;
            best = qValueGrid->at(state)[3];
        }
    
    
    if(random <= explore)
    {
        char valid='n';
        while(valid =='n'){
            choice = rand()% 4;
            
            //Make sure location is not near upper wall
            if(choice == 0 && agentY < gridHeight - 1){
                valid = 'y';
            }
            
            //Make sure location is not near lower wall
            else if(choice == 2 && agentY != 0){
                valid = 'y';
            }
            
            //Make sure location is not near right wall
            else if(choice == 1 && agentX < gridWidth - 1){
                valid = 'y';
            }
            
            //Make sure location is not near left wall
            else if(choice == 3 && agentX != 0){
                valid = 'y';
            }
            
            else{
                valid='n';
            }
        }
    }
    assert(choice != -1);
        return choice;
};

void act(int choice){
    
    
    if(choice==0){
        agentY=agentY+1;
    }
    else if(choice==2){
        agentY=agentY-1;
        
    }
    else if(choice==1){
        agentX=agentX+1;
        
    }
    else if(choice==3){
        agentX=agentX-1;
    }
   //cout<<"agent x loc: "<<agentX<<"  Agent Y loc: "<<agentY<<endl;
    //cout<<"goal x loc: "<<goalX<<"  goal Y loc: "<<goalY<<endl;
    assert(agentY<gridHeight && agentY >-1 && agentX<gridWidth && agentX > -1);
};


double maxQ(int state, vector<vector<double>>* qValueGrid){
    double best = -1000;
    
    for(int i =0;i<4;i++)
    {
        //Check up [0]
        if(best<qValueGrid->at(state)[0]){
            best = qValueGrid->at(state)[0];
        }
        //Check right [1]
        if(best<qValueGrid->at(state)[1]){
            best = qValueGrid->at(state)[1];
        }
        //Check down [2]
        if(best<qValueGrid->at(state)[2]){
            best = qValueGrid->at(state)[2];
        }
        //Check left [3]
        if(best<qValueGrid->at(state)[3]){
            best = qValueGrid->at(state)[3];
        }
    }
    return best;
};

int react(int choice, vector<vector<double>>* qValueGrid, vector<vector<int>>* rewardTable, vector<vector<int>>* states){
    
    int reward = rewardTable->at(agentY)[agentX];
    double alpha = .2;
    double discount = .9;
    double max;
    int prevState;
    int currentState = states->at(agentY)[agentX];
    
    max = maxQ(currentState, qValueGrid);
    
    if(choice == 0){
        prevState = states->at(agentY-1)[agentX];
        qValueGrid->at(prevState)[0] = qValueGrid->at(prevState)[0] + alpha * ( reward + discount*max - qValueGrid->at(prevState)[0]);
    }
    else if(choice == 1){
        prevState = states->at(agentY)[agentX-1];
        qValueGrid->at(prevState)[1] = qValueGrid->at(prevState)[1] + alpha * ( reward + discount*max - qValueGrid->at(prevState)[1]);
    }
    else if(choice == 2){
        prevState = states->at(agentY+1)[agentX];
        qValueGrid->at(prevState)[2] = qValueGrid->at(prevState)[2] + alpha * ( reward + discount*max - qValueGrid->at(prevState)[2]);
    }
    else if(choice == 3){
        prevState = states->at(agentY)[agentX+1];
        qValueGrid->at(prevState)[3] = qValueGrid->at(prevState)[3] + alpha * ( reward + discount*max - qValueGrid->at(prevState)[3]);
    }
    return reward;
};


void runProgramOnce(vector<vector<double>>* qValueGrid, vector<int>* iter, vector<vector<int>>* statesTable, vector<vector<int>>* rewardTable){
    
    int choice=-1;
    int iterations = 0;
    double reward = -10;
    int state;
    
    
    for(int i=0;i<cycles;i++)
    {
        while(reward==-10)
        {
            state = sense(statesTable);
            choice = decide(qValueGrid, state);
            act(choice);
            reward = react(choice,qValueGrid, rewardTable, statesTable);
            iterations+=1;
                    }
        iter->push_back(iterations);
        iterations = 0;
        reward = -10;
        agentX= agentStartX;
        agentY= agentStartY;
    }
    
};

void printTable(vector<vector<double>>* qTableValues, vector<vector<int>>* rewardTable, vector<vector<int>>* stateTable, vector<vector<char>>* visualGrid){
    
    cout<<"Q Table: "<< endl<<endl;
    cout<<"            Up        "<<"Right       "<<"Down       "<<"Left     "<<endl;
    for(int i = (gridHeight * gridWidth) - 1; i>-1 ; i--){
        for(int j=0;j<4;j++){
            if(j==0){
                cout<<"State: "<< i << "  ";
            }
            cout<<" " <<qTableValues->at(i)[j]<<" ";
        }
        cout<<endl;
    }
    
    cout<<"qvalue at state 5, action right; "<< qTableValues->at(5)[1]<<endl;
    
    cout<<"Reward Table: "<< endl<<endl;
    for(int i=gridHeight-1;i>-1;i--){
        for(int j=0;j<gridWidth;j++){
            
            cout<<rewardTable->at(i)[j]<<" ";
        }
        cout<<endl;
    }
    
    cout<<"State Table: "<< endl<<endl;
    for(int i=gridHeight-1;i>-1;i--){
        for(int j=0;j<gridWidth;j++){
            
            cout<<stateTable->at(i)[j]<<" ";
        }
        cout<<endl;
    }
    
    cout<<"Visual Grid: "<<endl<<endl;
    for(int i=gridHeight-1;i>-1;i--){
        for(int j=0;j<gridWidth;j++){
            
            cout<<visualGrid->at(i)[j]<<" ";
        }
        cout<<endl;
    }
    
    cout<<"agent x loc: "<<agentX<<"  Agent Y loc: "<<agentY<<endl;
    cout<<"goal x loc: "<<goalX<<"  goal Y loc: "<<goalY<<endl;
    
    
};

int main() {
    vector<vector<double>> qValueGrid;
    vector<vector<int>> rewardTable;
    vector<vector<int>> state;
    vector<vector<char>> visualGrid;
    vector<int> iter;
    
    promptUser();
    initGrids( &rewardTable, &state, &qValueGrid, &visualGrid);
    printTable( &qValueGrid, &rewardTable, &state, &visualGrid);

    runProgramOnce(&qValueGrid, &iter, &state, &rewardTable);
    
    for(int i = 0; i < iter.size(); i++){
    
        cout<<iter.at(i)<<endl;
    }
    
    printTable( &qValueGrid, &rewardTable, &state, &visualGrid);
    
    
    
    return 0;
}

