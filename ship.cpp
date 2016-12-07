// File: board.cpp


#include "ship.h"
#include <iostream>
using namespace std;

//sorts by length,then row, then column
bool longest_first(Ship s1,Ship s2){
	if (s1.length>s2.length) {
		return true;
	} else if( s1.length==s2.length && s1.row<s2.row) {
		return true;
	} else if (s1.length==s2.length && s1.row==s2.row && s1.column<s2.column) {
		return true;
	} else if (s1.length==s2.length && s1.row==s2.row && s1.column==s2.column && s1.orientation<s2.orientation) {
		return true;
	} return false;
}

void Ship::printShip() const {
	cout.width(10);cout<<left<<name<<" ";
	cout<<row<<" "<<column;
	if (length!=1) {
		if (orientation==0) {cout<<" horizontal"<<endl;}
		else {cout<<" vertical"<<endl;}
	} else {cout<<endl;}
	
}

bool Ship::operator==(const Ship& ship1) const {
	return (ship1.length==length && ship1.row==row && ship1.column==column
	&& ship1.orientation==orientation);
}

bool Ship::operator!=(const Ship& ship1) const {
	return not (ship1.length==length && ship1.row==row
	&& ship1.column==column && ship1.orientation==orientation) ;
}
