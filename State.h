/*
 * State.h
 *
 *  Created on: 26 Aug 2020
 *      Author: Kevin Vo Le
 */

#ifndef STATE_H_
#define STATE_H_

struct cell {
	int parent_c, parent_r;
	double f, g, h;
};

class State {
private:
	vector<int> blocks;
public:
	int grid[BOARDSIZE][BOARDSIZE];

	State() {
		for (int r = 0; r < BOARDSIZE; r++) {
			for (int c = 0; c < BOARDSIZE; c++) {
				grid[r][c] = 0;
			}
		}
	}

	State(const State &s) {
		for (int r = 0; r < BOARDSIZE; r++) {
			for (int c = 0; c < BOARDSIZE; c++) {
				grid[r][c] = s.grid[r][c];
			}
		}
	}

	bool operator==(State s) {
		int similarity = 0;
		for (int r = 0; r < BOARDSIZE; r++) {
			for (int c = 0; c < BOARDSIZE; c++) {
				if (grid[c][r] == s.getValue(c, r)) {
					similarity++;
				}
			}
		}
		//cout << "Similarity: "<< similarity << endl;
		if (similarity == 9)
		{
			return true;
		}
		return false;
	}

	int manhattanSum;
	int f;
	int h;
	void setH(int input);
	void setF(int h, int s);
	void setManhattanDistance(int h);
	void printBoard();
	void generateBlocks();
	void generateIntState();
	void generateBaseState();
	void getIntBoard();
	void pushDown();
	void possibleMoves();
	void setInitialState(vector<int> input);

	void setValue(int row, int column, int value);
	int h_function(int final[][BOARDSIZE]);

	//
	int manhattan_distance(int value, int final[][BOARDSIZE]);
	int findSBlockR(int value, int state[][BOARDSIZE]);
	int findSBlockC(int value, int state[][BOARDSIZE]);

	int freeSpace(int row);
	int getValue(int row, int column);

	int findBlockR(int value);
	int findBlockC(int value);

	void moveBlock(int sourceColumn, int sourceRow, int destinationRow,
			int destinationColumn);

	//validiations
	/* Check if you can move up, down, left, right
	 * from the free space
	 * */
	bool moveUpValid;

};

void State::generateBlocks() {
	for (int i = 0; i < 9; i++) {
		blocks.push_back(i);
	}

	for (int i = 8; i > 0; i--) {
		int num = rand() % i;
		int temp = blocks[i];

		blocks[i] = blocks[num];
		blocks[num] = temp;
	}
}

void State::generateIntState() {
	int bIndex = 0;
	generateBlocks();

	for (int r = 0; r < BOARDSIZE; r++) {
		for (int c = 0; c < BOARDSIZE; c++) {
			setValue(r, c, blocks[bIndex]);
			bIndex++;
		}
	}
}

void State::generateBaseState() {
//	grid[2][0] = 2;
//	grid[2][1] = 8;
//	grid[2][2] = 3;
//	grid[1][0] = 1;
//	grid[1][1] = 6;
//	grid[1][2] = 4;
//	grid[0][0] = 7;
//	grid[0][1] = 0;
//	grid[0][2] = 5;

	grid[2][0] = 2;
	grid[2][1] = 1;
	grid[2][2] = 6;
	grid[1][0] = 4;
	grid[1][1] = 0;
	grid[1][2] = 8;
	grid[0][0] = 7;
	grid[0][1] = 5;
	grid[0][2] = 3;
	printBoard();
}

void State::setInitialState(vector<int> input) {
	stack<int> list;
	for (unsigned it = 0; it < input.size(); it++)
	{
			list.push(input.at(it));
	}
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = BOARDSIZE - 1; j >= 0; j--) {
			grid[i][j] = list.top();
			list.pop();
		}
	}
}


void State::getIntBoard() {
	generateIntState();
	cout << "Initial State:" << endl;
	pushDown();
	printBoard();
}

void State::pushDown() {
	stack<int> list;
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = BOARDSIZE - 1; j >= 0; j--) {
			if (grid[j][i] != 0) {
				int a = grid[j][i];
				list.push(a);
			}
		}
		for (int j = 0; j < BOARDSIZE; j++) {
			if (list.empty()) {
				grid[j][i] = 0;
			} else {
				grid[j][i] = list.top();
				list.pop();
			}
		}
	}
}

void State::printBoard() {

	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			cout << " | " << grid[BOARDSIZE - i - 1][j] << " ";
		}
		cout << "|" << endl;
	}
	cout << "  ---  ---  --- " << endl;
}

void State::setValue(int row, int column, int value) {
	grid[row][column] = value;
}

int State::h_function(int final[][3]) {
	int h = 0;
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = BOARDSIZE - 1; j >= 0; j--) {
			//cout << grid[j][i];
			if (grid[j][i] != final[j][i]) {
				h++;
			}
		}
	}
	return h;
}

int State::manhattan_distance(int value, int final[][3]) {
	/*
		Find out how far away the current state is from the goal
		i.e
		1 2 3 4 5 6 7 8
		---------------
		1 1 0 0 0 1 0 2
		h = 1 + 1 + 1 + 2 = 5

		Find out where i is in the Current State
		Get its index
		Find out where i is in the Goal State
		Get its index
		Example:
		1 in current is at [2][0] and 1 in the goal is [3][0]
		Thus there is a distance of 1 (3-2)
		8 is in [3][1], Goal [2][0] = (3-2) (1-0) = 1 + 1 = 2
		--------Function to calculate------------------------
		Step 1: s.getRow(i), s.getCol(i) | g.getRow(i), g.getCol(i)
		Step 2: compare values so sR < gR -> sR - gR,
											sC < sR -> sC -sR
		Step 3: h = delta sR, gR + delta sC, sR
	*/
	int delta = 0;
	int currentRow, currentColumn, goalRow, goalColumn;
	int deltaRow, deltaColumn;

	//cout << "Evaluating: " << value << endl;
	currentColumn = findSBlockC(value, grid);
	currentRow = findSBlockR(value, grid);

	goalColumn = findSBlockC(value, final);
	goalRow = findSBlockR(value, final);

	if (currentColumn < goalColumn) {
		deltaColumn = goalColumn - currentColumn;
	} else {
		deltaColumn = currentColumn - goalColumn;

	}

	if (currentRow < goalRow) {
		deltaRow = goalRow - currentRow;
	} else {
		deltaRow = currentRow - goalRow;

	}

	delta = deltaRow + deltaColumn;
	return delta;

}


int State::freeSpace(int row) {
	//checks row (either 0,1,2) returns -1 on failure
	bool found = false;
	//cout << "Row " << row << endl;
	for (int c = 0; c <= BOARD_INDEX_MAX; c++) {
		//cout << "value: " << grid[row][c] << endl;
		if (grid[row][c] == 0) {
			//cout << "FOUND!" << endl;
			//returns column number
			//cout << c;
			return c;
			found = true;
		}
		if (found){
			break;
		}
	}
	return -1;
}

void State::moveBlock(int sourceColumn, int sourceRow, int destinationRow,
		int destinationColumn) {
	//we get the value of (SC,SR) and swap it with (DC,DR)
	int tempSource = grid[sourceRow][sourceColumn];
	//cout << "destinationRow: " << destinationRow << ", Destination Column: " << destinationColumn << endl;
	int tempDes = grid[destinationRow][destinationColumn];
	//cout << tempSource << " " << tempDes << endl;

	grid[sourceRow][sourceColumn] = tempDes;
	grid[destinationRow][destinationColumn] = tempSource;

}

int State::findBlockR(int value) {
	for (int r = BOARD_INDEX_MAX; r >= 0; r--) {
		for (int c = 0; c <= BOARD_INDEX_MAX; c++) {
			if (grid[r][c] == value) {
				return r;
			}
		}
	}
	return -1;
}

int State::findBlockC(int value) {
	for (int r = BOARD_INDEX_MAX; r >= 0; r--) {
		for (int c = 0; c <= BOARD_INDEX_MAX; c++) {
			if (grid[r][c] == value) {
				return c;
			}
		}
	}
	return -1;
}

int State::findSBlockC(int value, int state[][BOARDSIZE]) {
	for (int r = BOARD_INDEX_MAX; r >= 0; r--) {
		for (int c = 0; c <= BOARD_INDEX_MAX; c++) {
			if (state[r][c] == value) {
				return c;
			}
		}
	}
	return -1;
}

int State::getValue(int row, int column) {
	return grid[row][column];
}

int State::findSBlockR(int value, int state[][BOARDSIZE]) {
	for (int r = BOARD_INDEX_MAX; r >= 0; r--) {
		for (int c = 0; c <= BOARD_INDEX_MAX; c++) {
			if (state[r][c] == value) {
				return r;
			}
		}
	}
	return -1;
}

void State::setManhattanDistance(int h){
	manhattanSum = h;
}

void State::setF(int h, int s){
	/* Using Definition 2
	 * f = g + h2
	 * g(n)  = number of moves taken in going from S1 to S.
	 * h2(n) = sum of distances of the tiles from Sf position
	 *
	 * Definition 1 is more vague as it checks for misplaced
	 * tiles not how far they are from the goal.
	 * */
	f = s + h;
}


#endif /* STATE_H_ */
