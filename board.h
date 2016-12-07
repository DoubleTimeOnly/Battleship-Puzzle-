// File: board.h

#ifndef _BOARD_H_INCLUDED
#define _BOARD_H_INCLUDED
#include<algorithm>
#include <vector>
#include <iostream>
#include "ship.h"
#include "constraint.h"
using namespace std;

class Board {
	public:
		//Constructor
		//Takes in board size/data and constraints
		Board( int row_count,int column_count,const vector<int>& some_rows,
		const vector<int>& some_columns,vector<Constraint>& some_constraints);
		
		void printBoard() const;
		
		//actual placing/removing of ship on board
		void placeShip(Ship& ship,int row,int column);
		void removeShip(Ship& ship,int row,int column);
		
		//goes through board looking for spot for ships
		void findSpot(vector<Ship>& ships,vector<Ship>& placed_ships,
		int ship_num,vector< vector<Ship> >& possible_solutions,bool allSolutions);
		
		//checks if placed spot is legal for a ship
		bool canbeplaced(Ship& ship,int row,int column,int orientation);

	private:
		bool placed;
		vector< vector<char> > board;
		
		int num_rows;
		int num_columns;
		vector<int> rows;
		vector<int> columns;
		
		//The rows and columns before they get modified
		vector<int> original_rows;
		vector<int> original_columns;
		
		vector<Constraint> constraints;
	

};	
#endif