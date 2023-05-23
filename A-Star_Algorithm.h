		/*
 * Solver.h
 *
 *  Created on: 26 Aug 2020
 *      Author: Kevin Vo Le
 */

/*
 Since there is only four move (up,down,left,right)
 Manhattan Distance is used as the heuristic equation.
 */

#ifndef A_STAR_ALGORITHM_H_
#define A_STAR_ALGORITHM_H_

#include "State.h"

class Solver {
private:
	bool goalFound;
	int val, source, destination;
	int endStep = 18;

public:
	int finalState[BOARDSIZE][BOARDSIZE];
	vector<char> possibleMoves;
	int freeRow, freeColumn;
	int lowest_temp_h;
	map<string, State> state_h_key;
	map<string, int> state_key;
	map<string, string> duplicate_key;
	map<string, int> branch_map;
	vector<string> list_extra;
	//map<string<map<int, State>
	void evaluateNextState(State state);
	string evalutateNextState_Branch(State &currentState,
			vector<State> usedStates, int &step, int &stateNum);
	void initializeFinalState();
	void findByValue(vector<string> &vec, map<string, int> state_key,
			int value);
	void setHighets(int x);
	void setGoalState(vector<int> goal);
	void findFreeSpace(State current);
	void setPossibleMoves();
	void clearBranches(vector<string> &queue, string str);
	void adjust_temp_map(vector<string> vec, map<string, string> duplicate_key);
	void multipleBranches(State &currentState, vector<string> queue,
			vector<string> &lowest_h_state_names, vector<State> &usedStates,
			int step, int &stateNum);
	int returnFreeColumn();
	int returnFreeRow();
	int return_branchHValue();
	//bool isLower(int x);

};

void Solver::evaluateNextState(State state) {
	int h, step, stateNum;

	//add all moves first
	step = 0;
	stateNum = 1;
	//do one move
	State currentState;
	currentState = state;
	vector<State> usedStates;
	goalFound = false;
	do {
		cout << "Start: " << endl;
		//currentState.printBoard();
		possibleMoves.clear();
		usedStates.push_back(currentState);
//		for (unsigned i = 0; i < usedStates.size(); i++)
//		{
//			usedStates[i].printBoard();
//		}
		if (currentState.manhattanSum == 0) {
			cout << "Goal Reached in " << step << " steps!" << endl;
			cout << "F: " << currentState.f << endl;
			goalFound = true;
			//currentState.printBoard();
			break;
		}
		cout << "Step: " << step << endl;
		if (step == endStep) {	// breaker code
			goalFound = true;
			break;
		}



		vector<State> nextStates;
		findFreeSpace(currentState);
//3 possible moves off example
		/* Max possible moves is 4
		 * Switch case:
		 * Since  we know that the free column is 1 and row 0
		 * its neighbours are (C1,R1) (C1,R2) (C2,R0)
		 * We can swap those 3 with R0,C1
		 * */

		/** Calculating the possible movements
		 * using the state class we will find the free areas
		 * if row = 0 and col = 1
		 * then down cannot be used
		 * left, right, and up.
		 * Switch case:
		 * therefore we push in a vector of string avaliable moves
		 * vector<string> possibleMoves
		 */
		// in reference of the free space movements
		int fC = 0;
		int fR = 0;
		fC = returnFreeColumn();
		//cout << fC;
		fR = returnFreeRow();
		setPossibleMoves();
		/*
		 * From the example in the next step(1)
		 * a previous state can be repeated
		 * therefore a vector of states is used to check
		 * if it has been visited or not
		 *
		 * */
		step++;
		for (unsigned i = 0; i < possibleMoves.size(); i++) {
			char move;
			move = possibleMoves.at(i);
			//cout << move << endl;
			switch (move) {
				case 'U': {
					bool repeated = false;
					State copy(currentState);

					//cout << "Free Row: " << fC << endl;
					copy.moveBlock(fC, fR, fR + 1, fC);
					copy.printBoard();
					for (unsigned i = 0; i < usedStates.size(); i++) {
						if (usedStates[i] == copy) {
							repeated = true;
							//cout << "This state has already been generated" << endl;
							break;
						}
						if (repeated == false) {
							nextStates.push_back(copy);
							//copy.printBoard();
							usedStates.push_back(copy);
							repeated = true;
							break;
						}

					}
					break;
				}
				case 'D': {
					bool repeated = false;
					State copy1(currentState);
//							cout << "Free Row: " << fC << endl;
//							cout << "Free Column: " << fC << endl;
					copy1.moveBlock(fC, fR, fR - 1, fC);
					for (unsigned i = 0; i < usedStates.size(); i++) {
						//usedStates[i].printBoard();
						if (usedStates[i] == copy1) {
							repeated = true;
							//cout << "This state has already been generated" << endl;
						}
						if (repeated == false) {
							nextStates.push_back(copy1);
							//copy1.printBoard();
							usedStates.push_back(copy1);
							break;
						}

					}
					break;
				}
				case 'L': {
					bool repeated = false;
					State copy2(currentState);
					copy2.moveBlock(fC, fR, fR, fC - 1);
					for (unsigned i = 0; i < usedStates.size(); i++) {
						if (usedStates[i] == copy2) {
							repeated = true;
							//cout << "This state has already been generated" << endl;
						}
						if (repeated == false) {
							nextStates.push_back(copy2);
							//copy2.printBoard();
							usedStates.push_back(copy2);
							break;
						}

					}
					break;
				}
				case 'R': {
					bool repeated = false;
					State copy3(currentState);
					copy3.moveBlock(fC, fR, fR, fC + 1);
					for (unsigned i = 0; i < usedStates.size(); i++) {
						if (usedStates[i] == copy3) {
							repeated = true;
							//cout << "This state has already been generated" << endl;
						}
						if (repeated == false) {
							nextStates.push_back(copy3);
							//copy3.printBoard();
							usedStates.push_back(copy3);
							break;
						}

					}
					break;
				}
			}
		}
		vector<int> mDistances;
		vector < string > stateNodes;
		map<string, int> temp_queue;
		//key for each state along with its h value
		/*Three Key types
		 * 1. Key that has the state using the stateName
		 * 2. Key that has the stateName using the h value
		 *
		 * */
		for (unsigned it = 0; it < nextStates.size(); it++) {
			string stateName;
			stateName = "State " + to_string(stateNum);
			int manhattanDistance[8];
			State first = nextStates.at(it);
			h = 0;				//resets h value
			for (int i = 1; i < 9; i++) {
				manhattanDistance[i - 1] = first.manhattan_distance(i,
						finalState);

			}
			for (int i = 0; i < 9; i++) {
				//				cout << "Manhattan Distance for State: " << it << " block:"
				//						<< i+1 <<": "<< manhattanDistance[i] << endl;
				h = h + manhattanDistance[i];

			}
			cout << "H value for State: " << stateName << " is " << h << endl;
			if (h == 0){
				first.setManhattanDistance(h);
				first.setF(h, step);
				cout << "Goal Reached in " << step << " steps!" << endl;
				cout << "F: " << currentState.f << endl;
				goalFound = true;
				first.printBoard();
				break;
			}
			mDistances.push_back(h);
			stateNodes.push_back(stateName);
			first.setManhattanDistance(h);
			first.setF(h, step);
			first.printBoard();
			//create a pair to track the state and its h value
			state_h_key[stateName] = first;
			state_key[stateName] = h;
			temp_queue[stateName] = h;
			stateNum++;

		}
		if (goalFound){
			break;
		}
		/*Compare the f value of each state
		 * Max = 4
		 * Possible to have the same f value
		 * -> Keep those two and evaluate their next f value
		 * Retrieve all values
		 * */
		int max = mDistances.size();
		int h_values[max];
		//base case
		//map <pair <string, int>> state_lists;
		for (unsigned it = 0; it < mDistances.size(); it++) {
			h_values[it] = mDistances.at(it);

		}
		vector<int> multiple_h;

		//pre-queue
		for (int i = 0; i < max; i++) {
			multiple_h.push_back(h_values[i]);
		}
		int lowest_h = mDistances.at(0);
		//first loop find the lowest value
		for (unsigned it = 0; it < multiple_h.size(); it++) {
			//cout << multiple_h.at(it) << endl;
			if (lowest_h > multiple_h.at(it)) {
				lowest_h = multiple_h.at(it);
			}

		}
		//vector<map<string,int>> queue;
		vector < string > queue;
		for (auto &x : temp_queue) {
			//std::cout << x.first << ": " << x.second << '\n';
			if (lowest_h >= x.second) {
				//cout << "Current: " << lowest_h << " new: " << x.second << endl;
				//cout << "Lower/Equal value found" << endl;
				lowest_h = x.second;
				queue.push_back(x.first);
			}

		}
		/*Using the maps created
		 *Expand the ones that have the lowest h value
		 * */
		possibleMoves.clear();
		temp_queue.clear();
		vector < string > lowest_h_state_names;
		if (multiple_h.size() > 1 && step + 1 < endStep) {
			/*Problem encountered
			 * When using map<> same key values are null
			 * since the program relies on different h values
			 * there is a clash
			 * Proposal:
			 create multiples and branch off
			 check the branch states
			 */
			cout << "Breaking" << endl;
			step++;
			//cout << multiple_h.size();
			multipleBranches(currentState, queue, lowest_h_state_names,
					usedStates, step, stateNum);
			clearBranches(lowest_h_state_names, "Bad");
			for (unsigned i = 0; i < lowest_h_state_names.size(); i++) {
				//cout << "Checking:";
				//cout << lowest_h_state_names.at(i) << endl;
			}
			if (lowest_h_state_names.size() != 1) {
				//expand on the bad states
				vector < string > branch_queue;
				cout << "More H values" << endl;
				//cout << "Checking" << endl;
				//cout << list_extra.size();
				for (unsigned i = 0; i < list_extra.size(); i++) {
					//cout << "Checking:";
					//cout << list_extra.at(i) << endl;
					string nextState;
					nextState = list_extra.at(i);
//					currentState = state_h_key.find(nextState)->second;
//					currentState.printBoard();
//					branch_queue.push_back(nextState);

				}
				for (auto &x : branch_map) {
					//std::cout << x.first << ": " << x.second << '\n';
					if (lowest_temp_h >= x.second) {
						//cout << "Current: " << lowest_h << " new: " << x.second << endl;
						lowest_h = x.second;
						branch_queue.push_back(x.first);
					}

				}
				lowest_h_state_names.clear();
				step++;
				if (step > endStep) {
					break;
				}
				multipleBranches(currentState, branch_queue, lowest_h_state_names,
						usedStates, step, stateNum);
				clearBranches(lowest_h_state_names, "Bad");
				clearBranches(lowest_h_state_names, "Same");
				for (unsigned i = 0; i < lowest_h_state_names.size(); i++) {
					//cout << "Checking:";
					//cout << lowest_h_state_names.at(i) << endl;
				}
				lowest_h = 40; //set it to a high value
				if (lowest_h_state_names.size() == 1)
				{
					cout << "Leaving loop" << endl;
					list_extra.clear();
					cout << "List size: "<< list_extra.size() << endl;
					branch_map.clear();
				}
			}
			if(step > endStep){
				break;
			}
				string nextState;
				cout << nextState << endl;
				nextState = lowest_h_state_names.front();
				currentState = state_h_key.find(nextState)->second;
				currentState.printBoard();


		} else {
			map<int, string>::iterator key_it_1;
			//key_it_1 = state_key.find(multiple_h.at(0));
			string nextState;
			nextState = key_it_1->second;
			cout << nextState << endl;
			currentState = state_h_key.find(nextState)->second;
			currentState.printBoard();
		}

		//goalFound = true;

	}while (!goalFound);

}

void Solver::initializeFinalState() {
	finalState[2][0] = 1;
	finalState[2][1] = 2;
	finalState[2][2] = 3;
	finalState[1][0] = 8;
	finalState[1][1] = 0;
	finalState[1][2] = 4;
	finalState[0][0] = 7;
	finalState[0][1] = 6;
	finalState[0][2] = 5;

}

void Solver::setGoalState(vector<int> goal) {
	stack<int> list;
	for (unsigned it = 0; it < goal.size(); it++) {
		list.push(goal.at(it));
	}
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = BOARDSIZE - 1; j >= 0; j--) {
			finalState[i][j] = list.top();
			list.pop();
		}
	}
}

void Solver::findFreeSpace(State current) {
	for (int r = 2; r >= 0; r--) {
		//cout << current.getValue(r,current.freeSpace(r)) << endl;
		//cout << "Finding free space" << endl;
		if (current.freeSpace(r) != -1) {
			//find the free space
//					cout << "Free Space " << current.getValue(r,current.freeSpace(r))
//							<< "at column: " << current.freeSpace(r) << endl;
			freeColumn = current.freeSpace(r);
//					cout << "Free row: " << r << endl;
			freeRow = r;
			break;
		}
	}
}

string
Solver::evalutateNextState_Branch(State &currentState,
	vector<State> usedStates, int &step, int &stateNum) {
int h;
bool allSame = false;;
map<string, State> state_h_key_branch;
map<string, int> state_key_branch;
vector < string > list;
int fC = 0;
int fR = 0;
possibleMoves.clear();
currentState.printBoard();
usedStates.push_back(currentState);
//cout << "Finding free space" << endl;
findFreeSpace(currentState);
fC = returnFreeColumn();
fR = returnFreeRow();
setPossibleMoves();
/*
 * Copy of evaluateNextState
 *
 * */

vector<State> nextStates;
//cout << "Free Column " << fC << endl;
//cout << "Free Row " << fC << endl;
nextStates.clear();
for (unsigned i = 0; i < possibleMoves.size(); i++) {
	char move;
	move = possibleMoves.at(i);
	switch (move) {
		case 'U': {
			bool repeated = false;
			State copy(currentState);
			//cout << "Free Row: " << fC << endl;
			copy.moveBlock(fC, fR, fR + 1, fC);
			for (unsigned i = 0; i < usedStates.size(); i++) {
				if (usedStates[i] == copy) {
					repeated = true;
					//cout << "This state has already been generated" << endl;
					break;
				}
				if (repeated == false) {
					nextStates.push_back(copy);
					//copy.printBoard();
					usedStates.push_back(copy);
					repeated = true;
					break;
				}

			}
			break;
		}
		case 'D': {
			bool repeated = false;
			State copy1(currentState);
			//							cout << "Free Row: " << fC << endl;
			//							cout << "Free Column: " << fC << endl;
			copy1.moveBlock(fC, fR, fR - 1, fC);
			for (unsigned i = 0; i < usedStates.size(); i++) {
				//usedStates[i].printBoard();
				if (usedStates[i] == copy1) {
					repeated = true;
					//cout << "This state has already been generated" << endl;
				}
				if (repeated == false) {
					nextStates.push_back(copy1);
					//copy1.printBoard();
					usedStates.push_back(copy1);
					break;
				}

			}
			break;
		}
		case 'L': {
			bool repeated = false;
			State copy2(currentState);
			copy2.moveBlock(fC, fR, fR, fC - 1);
			for (unsigned i = 0; i < usedStates.size(); i++) {
				if (usedStates[i] == copy2) {
					repeated = true;
					//cout << "This state has already been generated" << endl;
				}
				if (repeated == false) {
					nextStates.push_back(copy2);
					//copy2.printBoard();
					usedStates.push_back(copy2);
					break;
				}

			}
			break;
		}
		case 'R': {
			bool repeated = false;
			State copy3(currentState);
			copy3.moveBlock(fC, fR, fR, fC + 1);
			for (unsigned i = 0; i < usedStates.size(); i++) {
				if (usedStates[i] == copy3) {
					repeated = true;
					//cout << "This state has already been generated" << endl;
				}
				if (repeated == false) {
					nextStates.push_back(copy3);
					//copy3.printBoard();
					usedStates.push_back(copy3);
					break;
				}

			}
			break;
		}
	}
}
vector<int> mDistances;
vector < string > stateNodes;
map<string, int> temp_queue;
//key for each state along with its h value
/*Three Key types
 * 1. Key that has the state using the stateName
 * 2. Key that has the stateName using the h value
 *
 * */
for (unsigned it = 0; it < nextStates.size(); it++) {
	string stateName;
	stateName = "State " + to_string(stateNum);
	int manhattanDistance[8];
	State first = nextStates.at(it);
	h = 0;				//resets h value
	for (int i = 1; i < 9; i++) {
		manhattanDistance[i - 1] = first.manhattan_distance(i, finalState);

	}
	for (int i = 0; i < 9; i++) {
		//				cout << "Manhattan Distance for State: " << it << " block:"
		//						<< i+1 <<": "<< manhattanDistance[i] << endl;
		h = h + manhattanDistance[i];

	}
	cout << "H value for State: " << stateName << " is " << h << endl;
	if (h == 0){
		first.setManhattanDistance(h);
		first.setF(h, step);
		cout << "Goal Reached in " << step << " steps!" << endl;
		cout << "F: " << currentState.f << endl;
		goalFound = true;
		first.printBoard();
		break;
	}
	if (goalFound){
		break;
	}
	mDistances.push_back(h);
	stateNodes.push_back(stateName);
	first.setManhattanDistance(h);
	first.setF(h, step);
	first.printBoard();
	//create a pair to track the state and its h value
	state_h_key[stateName] = first;
	state_key[stateName] = h;
	temp_queue[stateName] = h;
	branch_map[stateName] = h;
	stateNum++;

}
/*Compare the f value of each state
 * Max = 4
 * Possible to have the same f value
 * -> Keep those two and evaluate their next f value
 * Retrieve all values
 * */
int max = mDistances.size();
int h_values[max];
//base case
//map <pair <string, int>> state_lists;
for (unsigned it = 0; it < mDistances.size(); it++) {
	h_values[it] = mDistances.at(it);

}
vector<int> multiple_h;

//pre-queue
for (int i = 0; i < max; i++) {
	multiple_h.push_back(h_values[i]);
}
int lowest_h = mDistances.at(0);
//first loop find the lowest value
for (unsigned it = 0; it < multiple_h.size(); it++) {
	//cout << multiple_h.at(it) << endl;
	if (lowest_h > multiple_h.at(it)) {
		lowest_h = multiple_h.at(it);
	}

}
//vector<map<string,int>> queue;
vector < string > queue;
for (auto &x : temp_queue) {
	//std::cout << x.first << ": " << x.second << '\n';
	if (lowest_h >= x.second) {
		//cout << "Current: " << lowest_h << " new: " << x.second << endl;
		//cout << "Lower/Equal value found" << endl;
		lowest_h = x.second;
		queue.push_back(x.first);
	}

}
unsigned matchValue;
matchValue = 0;
for (auto &x : temp_queue) {
	//std::cout << x.first << ": " << x.second << '\n';
	if (lowest_h == x.second) {
		matchValue++;
	}

}
step++;
	if (step > endStep) {
		return "end";
	}
if (matchValue == queue.size()){
	allSame = true;
}
if (lowest_temp_h > lowest_h) {
	lowest_temp_h = lowest_h;
}
//cout << queue.size() << endl;
if (queue.size() != 1 && !allSame) {
	for (unsigned i = 0; i < queue.size(); i++) {
		cout << "Loop" << endl;
		string id;
		id = queue.at(i);
		state_key.find(id);
		cout << id << endl;
//				string branchName = "Branch";
//				branchName = branchName + to_string(branch_num);
//				string name;
//				name = queue.front();
		//
		//cout << "Pushing " << id << " in " << endl;
		list_extra.push_back(id);

	}
	cout << "Bad branch" << endl;
	cout << list.size();
	temp_queue.clear();
	queue.clear();
	return "Bad";

}
if (queue.size() != 1 && !allSame){
	for (unsigned i = 0; i < queue.size(); i++) {
			cout << "Loop" << endl;
			string id;
			id = queue.at(i);
			state_key.find(id);
			cout << id << endl;
			//cout << "Pushing " << id << " in " << endl;
			list_extra.push_back(id);

		}
	cout << "All same values" << endl;
	cout << list.size();
	temp_queue.clear();
	queue.clear();
	return "Same";
}
else {
	string name;
	name = queue.front();
	temp_queue.clear();
	queue.clear();
	return name;
}

}

void Solver::setPossibleMoves() {
if (freeRow == 0) {					//if the 0 is at the bottom or top
	if (freeColumn == 0) {
		possibleMoves.push_back('U');
		possibleMoves.push_back('R');
	}
	if (freeColumn == 1) {
		possibleMoves.push_back('U');
		possibleMoves.push_back('L');
		possibleMoves.push_back('R');
	}
	if (freeColumn == 2) {
		possibleMoves.push_back('U');
		possibleMoves.push_back('L');
	}
}
if (freeRow == 2) {
	if (freeColumn == 0) {
		possibleMoves.push_back('D');
		possibleMoves.push_back('R');
	}
	if (freeColumn == 1) {
		possibleMoves.push_back('D');
		possibleMoves.push_back('L');
		possibleMoves.push_back('R');
	}
	if (freeColumn == 2) {
		possibleMoves.push_back('D');
		possibleMoves.push_back('L');
	}
}
if (freeRow == 1) {
	if (freeColumn == 0) {
		//cout << "Insert at column 0 (U,R,D)" << endl;
		possibleMoves.push_back('U');
		possibleMoves.push_back('R');
		possibleMoves.push_back('D');
	}
	if (freeColumn == 1) {
		//cout << "Insert at column 1 (U,L,R,D)" << endl;
		possibleMoves.push_back('U');
		possibleMoves.push_back('L');
		possibleMoves.push_back('R');
		possibleMoves.push_back('D');
	}
	if (freeColumn == 2) {
		//cout << "Insert at column 2 (U,L,D)" << endl;
		possibleMoves.push_back('U');
		possibleMoves.push_back('L');
		possibleMoves.push_back('D');
	}
}
}

void Solver::multipleBranches(State &currentState, vector<string> queue,
	vector<string> &lowest_h_state_names, vector<State> &usedStates,
	int step, int &stateNum) {
for (unsigned it = 0; it < queue.size(); it++) {
	//cout << queue.at(it) << endl;
	map<string, State>::iterator key_it_1;
	string nextState;
	nextState = queue.at(it);
	//				cout << nextState << endl;
	currentState = state_h_key.find(nextState)->second;
	currentState.printBoard();

	string state_status;
	state_status = (evalutateNextState_Branch(currentState, usedStates,
					step, stateNum));
	cout << "Status: " << state_status << endl;
	lowest_h_state_names.push_back(state_status);
	cout << "Next number" << endl;

}
}

void Solver::clearBranches(vector<string> &queue, string str) {
std::vector<string>::iterator iter = queue.begin();

while (iter != queue.end()) {
	if (*iter == str)
	iter = queue.erase(iter);
	else
	iter++;
}
}

void Solver::findByValue(vector<string> &vec, map<string, int> state_key,
	int value) {
auto it = state_key.begin();
// Iterate through the map
while (it != state_key.end()) {
	// Check if value of this entry matches with given value
	if (it->second > value) {
		//cout << "Higher value found for State: " << endl;
		// Push the key in given map
		cout << it->first << endl;
		vec.push_back(it->first);
	}
	// Go to next entry in map
	it++;
}
}

void Solver::adjust_temp_map(vector<string> vec,
	map<string, string> duplicate_key) {
map<string, string>::iterator it;
string name;
for (unsigned iter = 0; iter < vec.size(); iter++) {
	name = vec.at(iter);
	it = duplicate_key.find(name);
	cout << "Erasing " << name << endl;
	duplicate_key.erase(it);
}
}

int Solver::returnFreeColumn() {
return freeColumn;
}

int Solver::returnFreeRow() {
//cout << freeRow;
return freeRow;
}

#endif /* A_STAR_ALGORITHM_H_ */
