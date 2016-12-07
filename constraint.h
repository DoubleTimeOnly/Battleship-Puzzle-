//File: constraint.h
#ifndef _CONSTRAINT_H_INCLUDED
#define _CONSTRAINT_H_INCLUDED
class Constraint {
	
	public:
		Constraint(int a_row,int a_column,char a_type)
		: row(a_row),column(a_column),type(a_type) {}
		
		int row,column;
		char type;
	
};

#endif