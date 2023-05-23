/*
 * main.cpp
 *
 *  Created on: 26 Aug 2020
 *      Author: Kevin Vo Le
 *
 *      I hereby certify that no other part of this submission has been copied from any
		other sources, including the Internet, books or other student’s work except the
		ones I have listed below. No part of the code has been written/produced for me
		by another person or copied from any other source.
		I hold a copy of this assignment that I can produce if the original is lost or
		damaged.



		8-Puzzle Using A* Search Algorithm
		1. Solve the default example
		2. Set own goal and initial state
		3. Randomize initial state and set goal
 */



#include <iostream>
#include <stack>
#include <ctime>
#include <vector>
#include <queue>
#include <algorithm>
#include <list>
#include <iterator>
#include <queue>
#include <utility>
#include <map>

using namespace std;

const int BOARDSIZE = 3;
const int NUMBER_OF_BLOCKS = 6;
int BOARD_INDEX_MAX = 2;
int MAX_STATES = 100;

#include "Action.h"
#include "State.h"
#include "A-Star_Algorithm.h"
char option;

int main(){
	srand((int)time(0));
	State s;
	//s.generateBaseState();

	vector<int> initial;
	for (int i = 0; i < 9; i++)
	{	int choice;
		cout << "Insert number " << i+1 << " for initial state" << endl;
		cin >> choice;
		initial.push_back(choice);
	}
	s.setInitialState(initial);
	s.printBoard();
	Solver sol;
	sol.initializeFinalState();
	sol.evaluateNextState(s);
	return 0;





}


