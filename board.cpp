// File: board.cpp


#include "board.h"
using namespace std;
Board::Board( int row_count,int column_count,const vector<int>& some_rows,
const vector<int>& some_columns,vector<Constraint>& some_constraints ) {
	
	vector< vector<char> >temp(row_count,vector<char>(column_count,' '));
	board=temp;
	for (int i=0;i<some_constraints.size();++i) {
		board[some_constraints[i].row][some_constraints[i].column]=some_constraints[i].type;
	}
	//stores original rows/columns as the row and column values before modification
	//this is because rows values are subtracted from when ships are placed
	original_rows = rows = some_rows;
	original_columns = columns=some_columns;
	num_rows=row_count;
	num_columns=column_count;
	constraints=some_constraints;
}

void Board::printBoard() const {
	cout<<'+'<<string(columns.size(),'-')<<'+'<<endl;
	for (int i=0;i<rows.size();++i) {
		cout<<'|';
		for (int j=0;j<columns.size();++j){	
			cout<<board[i][j];
		}
		cout<<'|';
		
		if (original_rows[i]<0) {
			//unknown rows/columns are stored as -1. This means that when
			//ships are placed, the row value goes negative, this formula
			//prints the intended value of the unknown row
			cout<< -1*(rows[i]+1) <<endl;
		} else {
			cout<<original_rows[i]<<endl;
		}
	}
	cout<<'+'<<string(columns.size(),'-')<<'+'<<endl;
	
	cout<<' ';
	for (int j=0;j<columns.size();++j) {
		if (original_columns[j]<0) {
			cout<< -1*(columns[j]+1);
		} else {
			cout<<original_columns[j];
		}
	}
	cout<<endl;
	cout<<endl;
}

//Does the actual replacing of characters on the board
//Takes a ship and the coordinate where it will be placed
//Replaces the characters on the board with the ship and
//adjusts the occupied row/column values to take the new ship into account
void Board::placeShip(Ship& ship,int row,int column) {
	ship.column=column;
	ship.row=row;
	
	
	if (ship.length==1) {
		board[row][column]='o';
		columns[column]-=1;
		rows[row]-=1;

	} else {
		//orientation:
		//0 is horizontal | 1 is vertical
		if (ship.orientation==0 && column+ship.length<=columns.size() 
		&& row+1<=rows.size()) {		
			board[row][column]='<';
			rows[row]-=ship.length;
			columns[column]-=1;
			for (int i=1;i<ship.length-1;++i) {
				board[row][column+i]='X';
				columns[column+i]-=1;
			}
			board[row][column+ship.length-1]='>';
			columns[column+ship.length-1]-=1;
		} else if (ship.orientation == 1 && column+1<=columns.size()
		&& row+ship.length<=rows.size()) {	
			board[row][column]='^';
			columns[column]-=ship.length;
			rows[row]-=1;
			for (int j=1;j<ship.length-1;++j) {
				board[row+j][column]='X';
				rows[row+j]-=1;
			}
			board[row+ship.length-1][column]='V';
			rows[row+ship.length-1]-=1;
		} 
	}
}

//Replaces what used to be the ship with empty spaces
//adjusts the occupied row/column values to take the new ship into account
void Board::removeShip(Ship& ship,int row,int column) {
	row=ship.row;
	column=ship.column;
	
	if (ship.orientation==0 
	&& column+ship.length<=columns.size() && row<rows.size()) {		//horizontal		
		rows[row]+=ship.length;
		
		for (int i=0;i<ship.length;++i) {
			board[row][column+i]=' ';
			columns[column+i]+=1;
		}
	} else if (ship.orientation == 1
	&& column<columns.size() && row+ship.length<=rows.size()) {	//vertical
		columns[column]+=ship.length;
		for (int j=0;j<ship.length;++j) {
			board[row+j][column]=' ';
			rows[row+j]+=1;
		}
	} 
}

//Yes or no on whether the ship can actually be placed at the spot
bool Board::canbeplaced(Ship& ship,int row,int column,int orientation) {
	
	//First checks if the current spot is a constraint
	bool onconstraint=false;
	for (int i=0;i<constraints.size();++i) {
		if (constraints[i].row==row && constraints[i].column==column) {
			onconstraint=true;
		}
	}
	
	if (not onconstraint && board[row][column] != ' ' ) {
		return false;
	}
	
	
	if (onconstraint) {		
		if (ship.length!=1 && board[row][column]=='o') {
			return false;
		}
		if (ship.length==1 && (board[row][column]!='o'|| board[row][column]!=' ')) {
			return false;
		}
		if (ship.length>1 && orientation==0 && board[row][column]!='<') {
			return false;
		}
		if (ship.length>1 && orientation==1 && board[row][column]!='^') {
			return false;
		}
	}
	bool canplace;
	if (ship.length==1) { 	//sets it so submarine horizontal and vertical are same
		orientation=0;
	}
	
	//if the row value is -1, then it is an unknown row and row values are ignored
	//Unknown rows are boundless and anything within the limits of the board
	//can be placed there
	if (orientation==0 && (ship.length<=rows[row] || original_rows[row]==-1)
	&& column+ship.length<=columns.size() && row<rows.size()) {
		ship.orientation=0;
		for (int i=-1;i<ship.length+1;++i) {
			if (i+column < 0 || i+column>=columns.size()) {continue;}
			
			//constraint checker
			if ( i>-1 && i<ship.length) {
				if (i>0 && i<ship.length-1) {	
					if (board[row][column+i]!='X' && board[row][column+i]!=' ') {
						return false;
					}
				}
				if (i==ship.length-1 && (board[row][column+i]!='>' 
				&& board[row][column+i]!=' ')) {
					return false;
				}
			}
			
			//ship proximity checker
			//checks tiles around the ship for non water pieces 
			if (row+1<rows.size() && board[row+1][column+i]!=' ' 
			&& board[row+1][column+i]!='-') {
				return false;
			}
			if (row-1>=0 && board[row-1][column+i]!=' '
			&& board[row-1][column+i]!='-') {
				return false;
			}
			if (i==-1 || i== ship.length) {
				if (board[row][column+i]!=' ' && board[row][column+i]!='-') {
					return false;
				}
			}
		} 
		
		//checks if the column has enough space left to hold the ship
		for (int pos=0;pos<ship.length;++pos) {
			if (!(columns[column+pos]>=1 || original_columns[column+pos]==-1)) {
				return false;
			}
		}
		return true;
		
	} else if(orientation == 1
	&& (ship.length<=columns[column] || original_columns[column]==-1)
	&& column<columns.size() && row+ship.length<=rows.size()) {
		ship.orientation=1;
		for (int i=-1;i<ship.length+1;++i) {
			if (i+row < 0 || i+row>=rows.size()) {continue;}
			
			//constraint checker
			if ( i>-1 && i<ship.length) {
				if (i>0 && i<ship.length-1) {	
					if (board[row+i][column]!='X' && board[row+i][column]!=' ') {
						return false;
					}
				}
				if (i==ship.length-1 && (board[row+i][column]!='>' && board[row+i][column]!=' ')) {
					return false;
				}
			}
			
			
			//ship proximity checker
			if (column+1<columns.size() && board[row+i][column+1]!=' ' && board[row+i][column+1]!='-') {
				return false;
			}
			if (column-1>=0 && board[row+i][column-1]!=' ' && board[row+i][column-1]!='-') {
				return false;
			}
			if (i==-1 || i== ship.length) {
				if (board[row+i][column]!=' ' && board[row+i][column]!='-') {
					return false;
				}
			}
		}

		for (int pos=0;pos<ship.length;++pos) {
			if (!(rows[row+pos]>=1 || original_rows[row+pos]==-1)) {
				return false;
			}
		}
		return true;
	} else {
		return false;
	}
}

void Board::findSpot(vector<Ship>& ships,vector<Ship>& placed_ships,int ship_num,
vector< vector<Ship> >& possible_solutions,bool allSolutions) {
	
	//Base Case
	if (placed_ships.size()==ships.size()) {
		bool invalid_solution = false;
		//duplicate solution checker, checks if each ship in the vector shares
		//the same length, position, and orientation
		vector<Ship> temp_ships=ships;
		sort(temp_ships.begin(),temp_ships.end(),longest_first);
		int i=0;
		while (i<possible_solutions.size() && not invalid_solution) {
			if (temp_ships==possible_solutions[i]) {invalid_solution = true;}
			++i;
		}
		
		//constraint checker
		//if there isn't a constraint where there should be one fails the solution
		if (not invalid_solution) {
			for (int c=0;c<constraints.size();++c) {
				if (board[constraints[c].row][constraints[c].column]!=constraints[c].type) {
					invalid_solution=true;
					break;
				}
			}
		}
		
		//non-matching ship length and row/column spaces checker
		//if ship length total doesn't match occupied space total fails solution
		if (not invalid_solution) {
			int sum_ship_length=0;
			int sum_row_length=0;
			int sum_column_length=0;
			for (int i=0;i<ships.size();++i) {sum_ship_length+=ships[i].length;}
			for (int i=0;i<original_rows.size();++i) {
				if (original_rows[i]<0) {
					sum_row_length+= -1*(rows[i]+1);
				} else {
					sum_row_length+=original_rows[i];
				}
			}
			for (int i=0;i<original_columns.size();++i) {
				if (original_columns[i]<0) {
					sum_column_length+= -1*(columns[i]+1);
				} else {
					sum_column_length+=original_columns[i];
				}
			}
			if (sum_row_length!=sum_ship_length || sum_column_length!=sum_ship_length) {
				invalid_solution = true;
			}
		}
		
		//checks if there are stray constraints that are unattached to ships
		//if each row's occupied space does not match the number of pieces in each
		//row, due to orphaned constraints, then fails the solution
		if (not invalid_solution) {
			for (int r=0;r<original_rows.size();++r) {
				int sum_ship_length=0;
				for (int c=0;c<original_columns.size();++c) {
					if (board[r][c]!=' ' && board[r][c]!='-') {
						sum_ship_length++;
					}
				}
				int allowed_space;
				if (original_rows[r]==-1) {
					allowed_space=-1*(rows[r]+1);
				} else {
					allowed_space=original_rows[r];
				}
				if (sum_ship_length!=allowed_space) {invalid_solution=true;break;}
			}
		}
			
		//prints solution
		if (not invalid_solution) {
			vector<Ship> temp;
			temp = ships;
			possible_solutions.push_back(temp);
			cout<<"Solution:"<<endl;
			for (int s=0;s<temp_ships.size();++s) {
				temp_ships[s].printShip();
			}
			printBoard();	
		}
		return;
	}	//end base case
	
	
	int i,j;
	i=ships[ship_num].row;
	j=ships[ship_num].column;
	++j;
	while (i<num_rows) {
		while (j<num_columns) {
			if (canbeplaced(ships[ship_num],i,j,0)) {	//horizontal check
				placed_ships.push_back(ships[ship_num]);
				placeShip(ships[ship_num],i,j);
				
				//if the next ship is the same as current ship,
				//start the next ship off where current ended
				//saves computations by not performing redundant searches
				if (ship_num<ships.size()-1 && ships[ship_num].name==ships[ship_num+1].name) {
					ships[ship_num+1].row = i;
					ships[ship_num+1].column = j;
				}
				findSpot(ships,placed_ships,ship_num+1,possible_solutions,allSolutions);
				if (possible_solutions.size()==1 && not allSolutions) {return;}
				if (placed_ships.size()==ships.size()) {
					placed_ships.pop_back();
					removeShip(ships[ship_num],ships[ship_num].row,ships[ship_num].column);
				}
			} if (canbeplaced(ships[ship_num],i,j,1)) {		//vertical check
				placed_ships.push_back(ships[ship_num]);
				placeShip(ships[ship_num],i,j);

				if (ship_num<ships.size()-1 && ships[ship_num].name==ships[ship_num+1].name) {
					ships[ship_num+1].row = i;
					ships[ship_num+1].column = j;
				}
				findSpot(ships,placed_ships,ship_num+1,possible_solutions,allSolutions);
				if (possible_solutions.size()==1 && not allSolutions) {return;}
				if (placed_ships.size()==ships.size()) {
					placed_ships.pop_back();
					removeShip(ships[ship_num],ships[ship_num].row,ships[ship_num].column);
				}
			}
			++j;
		}
		j=0;
		++i;
	}
	if (placed_ships.size()>0) {	
		removeShip(ships[ship_num-1],ships[ship_num-1].row,ships[ship_num-1].column);
		ships[ship_num].row=0;
		ships[ship_num].column=-1;
		placed_ships.pop_back();
	}
	return;	
}























