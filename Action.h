/*
 * Action.h
 *
 *  Created on: 26 Aug 2020
 *      Author: Kevin Vo Le
 */

#ifndef ACTION_H_
#define ACTION_H_

class Action{
private:
	int source;
	int destinationR;
	int destinationC;
	int block;
	int src;
	int dest;
	int heuristic;
public:
	int value;
	int targetBlock;
	int destR;
	int destC;

	//goal values
	int val, col, row;


	//overload priority queue
	bool operator<(const Action a) const{
		if (heuristic < a.heuristic){
			return true;
		}
		else {
			return false;
		}
	}
	int getValue();
	int getBlock();
	int getDRow();
	int getDCol();
	void setAction(int s, int d);
	void setGoal();
	void setHeuristic(int h);
	int getSource();

	int getSrc(){ return src; }
	int getHeuristic();
	int getDest() { return dest; }
	int returnCurrentBlock();
	void printAction();

};



void Action::setGoal(){
	int v, r ,c;
		cout << "Input a goal in this format (block, row, column): " << endl;
		do{
			cout << "Block (1-6): ";
			cin >> v;
		} while ( v < 1 || v > 6);

		 do{
			 cout << "Row (0-2)[Bottom-Up]: ";
			 cin >> r;
		} while (r < 0 || r > 2);

		do{
			cout << "Column (0-2)[Left-Right]: ";
			cin >> c;

		 } while (c < 0 || c > 2);
		block = v;
		destinationR = r;
		destinationC = c;

}

int Action::getDRow(){
	row = destinationR;
	return row;
}

int Action::getDCol(){
	col = destinationC;
	return col;
}

int Action::getBlock(){
	val = block;
	return val;
}

int Action::getValue(){
	return block;
}

void Action::setAction(int s, int d){
	src = s;
	dest = d;
}

void Action::setHeuristic(int h){
	heuristic = h;
}

int Action::getSource(){
	int s = src;
	return s;
}



int Action::getHeuristic(){
	return heuristic;
}



#endif /* ACTION_H_ */
