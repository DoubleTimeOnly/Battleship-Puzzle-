// File: battleship.cpp

#include <algorithm> //for sorting vectors
#include <iostream>
#include <fstream>
#include <string>
#include "board.h"

using namespace std;

//unknown ship adder/placer function
//takes in number of unknown ships and adds them to vector of known ships
//iterates through their possible sizes. Ignores sizes that are certain to not
//have solutions or are redundant
void find_unknown_ships(int num_unknown_ships,vector<Ship>& ships,
vector<Ship>& placed_ships, vector<vector<Ship> >& possible_solutions,
bool allSolutions,vector<int>& rows,vector<int>& columns,vector<Constraint>& constraints,
int& num_solutions,int unknowns_placed) {
	
	//base case
	if (num_unknown_ships==0) {
		//checks for unknown rows/columns
		//if rows and columns are known, possible ships are limited
		bool has_unknown_row=false;
		int sum_row=0;
		for (int r=0;r<rows.size();++r) {
			if (rows[r]<0) {
				has_unknown_row=true;
				break;
			} else {
				sum_row+=rows[r];
			}
		}
		bool has_unknown_column=false;
		int sum_column=0;
		for (int r=0;r<columns.size();++r) {
			if (columns[r]<0) {
				has_unknown_column=true;
				break;
			} else {
				sum_column+=columns[r];
			}
		}
		
		//if row total and column total are not the ship total, do not test
		if (not has_unknown_column || not has_unknown_row) {
			int sum_ship_length=0;
			for (int s=0;s<ships.size();++s) {
				sum_ship_length+=ships[s].length;
			}
			if (sum_ship_length != sum_row || sum_ship_length != sum_column ) {
				return;
			}	
		}
		
		for (int i=0;i<ships.size();++i) {
			ships[i].row=0;
			ships[i].column=-1;
			ships[i].orientation=0;
		}

		placed_ships.clear();
		possible_solutions.clear();
		Board board(rows.size(),columns.size(),rows,columns,constraints);
		board.findSpot(ships,placed_ships,0,possible_solutions,allSolutions);
		num_solutions+=possible_solutions.size();

		return;
	}

	for (int j=7;j>=1;--j) {
		
		//Does not need to test if the current ship is longer than previous ship
		//This is because non descending ships sets are unsorted descending ships set
	 	 if (unknowns_placed!=0 && j>ships[ships.size()-1].length ||
		 j>rows.size() || j>columns.size() ) {
			continue;
		} 
			string ship_name;
			if (j==1) {
				ship_name="submarine";
				ships.push_back(Ship(ship_name,1,0,-1,0));
			} else if (j == 2) {
				ship_name="destroyer";
				ships.push_back(Ship(ship_name,2,0,-1,0));
			} else if (j == 3) {
				ship_name="cruiser";
				ships.push_back(Ship(ship_name,3,0,-1,0));
			} else if (j == 4) {
				ship_name="battleship";
				ships.push_back(Ship(ship_name,4,0,-1,0));
			} else if (j == 5){
				ship_name="carrier";
				ships.push_back(Ship(ship_name,5,0,-1,0));
			} else if (j == 6) {
				ship_name="cargo";
				ships.push_back(Ship(ship_name,6,0,-1,0));
			} else if (j == 7) {
				ship_name="tanker";
				ships.push_back(Ship(ship_name,7,0,-1,0));
			}
			find_unknown_ships(num_unknown_ships-1,ships,placed_ships,
			possible_solutions,allSolutions,rows,columns,constraints,
			num_solutions,unknowns_placed+1);
			if (not allSolutions && num_solutions==1) {return;}
			ships.pop_back();
	}
	return;
}

int main(int argc,char *argv[]) {
	
	ifstream puzzle_file(argv[1]);
	string string_temp;
	
	int num_rows,num_columns;
	string num_temp;
	puzzle_file >> string_temp;
	puzzle_file >> num_rows;
	puzzle_file >> num_columns;
	
	//row and column adder
	//if a value is unknown then it is stored as -1
	vector<int> rows;
	vector<int> columns;
	puzzle_file>>string_temp;
	for (int i=0;i<num_rows;++i) {	
		puzzle_file>>num_temp;
		if (num_temp=="?") {num_temp="-1";}	
		int temp = atoi(num_temp.c_str());
		rows.push_back(temp);
	}
	
	puzzle_file>>string_temp;
	for (int i=0;i<num_columns;++i) {	
		puzzle_file>>num_temp;
		if (num_temp=="?") {num_temp="-1";}
		int temp = atoi(num_temp.c_str());
		columns.push_back(temp);
	}
	
	//Ship adder
	//unknowns ships are added later
	vector<Ship> ships;
	vector<Constraint> constraints;
	string ship_name;
	int num_unknown_ships=0;
	
	while(puzzle_file>>ship_name) {
		if (ship_name=="submarine") {
			ships.push_back(Ship(ship_name,1,0,-1,0));
		} else if (ship_name == "destroyer") {
			ships.push_back(Ship(ship_name,2,0,-1,0));
		} else if (ship_name == "cruiser") {
			ships.push_back(Ship(ship_name,3,0,-1,0));
		} else if (ship_name == "battleship") {
			ships.push_back(Ship(ship_name,4,0,-1,0));
		} else if (ship_name == "carrier") {
			ships.push_back(Ship(ship_name,5,0,-1,0));
		} else if (ship_name == "cargo") {
			ships.push_back(Ship(ship_name,6,0,-1,0));
		} else if (ship_name == "tanker") {
			ships.push_back(Ship(ship_name,7,0,-1,0));
		} else if (ship_name == "unknown") {
			num_unknown_ships+=1;
		} else if (ship_name == "constraint") {break;}
	}
	
	//Constraint adder
	string constraint;
	int value;
	while(puzzle_file>>value) {
		int con_row,con_column;
		char con_char;
		con_row=value;
		puzzle_file>>con_column;
		puzzle_file>>con_char;
		puzzle_file>>constraint;
		constraints.push_back(Constraint(con_row,con_column,con_char));
	}
	
	//sort the ships longest to shortest to reduce required computations
	sort(ships.begin(),ships.end(),longest_first);
	vector< vector<Ship> > possible_solutions;
	vector<Ship> placed_ships;	
	bool allSolutions=false;
	if (argc==3 && string(argv[2])=="find_all_solutions") {
		allSolutions=true;
	}
	
	if (num_unknown_ships>0) {
		int num_solutions=0;
		int unknowns_placed=0;
		
		find_unknown_ships(num_unknown_ships,ships,placed_ships,
		possible_solutions,allSolutions,rows,columns,constraints,
		num_solutions,unknowns_placed);
		
		if (num_solutions==0) {
			cout<<"No Solutions"<<endl;
		} else {
			cout<<"Found "<<num_solutions<<" solution(s)"<<endl;
		} 
	} else {
		Board board(num_rows,num_columns,rows,columns,constraints);
		board.findSpot(ships,placed_ships,0,possible_solutions,allSolutions);
		if (possible_solutions.size()==0) {
			cout<<"No Solutions"<<endl;
		} else {
			cout<<"Found "<<possible_solutions.size()<<" solution(s)"<<endl;
		}
	}
	return 0;
}
