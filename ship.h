// File: board.h

#ifndef _SHIP_H_INCLUDED
#define _SHIP_H_INCLUDED
#include <string>
#include <iomanip>
using namespace std;

class Ship{
	public:
		Ship(string& ship_name,int ship_length,int a_row, int a_column,int an_orientation) 
		: name(ship_name),length(ship_length), row(a_row), column(a_column),
		orientation(an_orientation) {};

		void printShip() const;	//prints | name row column orientation
		
		string name;
		int length,row,column,orientation;
		
		bool operator==(const Ship& ship1) const;
		bool operator!=(const Ship& ship1) const;

};	


bool longest_first(Ship s1,Ship s2);


#endif

