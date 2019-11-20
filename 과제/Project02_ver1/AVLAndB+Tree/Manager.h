#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "AVLTree.h"
#include "BpTree.h"

class Manager{
private:
	AVLTree		*avl;	//AVL Tree
	BpTree		*bp;	//B+ Tree
	ofstream	fout;	//ofstream for log

public:
	Manager(int bPOrder);	
	~Manager();

	void	run(const char * command_txt);	
	
	bool	LOAD();
	bool	SEARCH_AVL(char* name);
	bool	SEARCH_BP(double a, double b);
	bool	RANK();	
	bool	PRINT_AVL();
	bool	PRINT_BP();

	void	printErrorCode(int n);	
};

#endif
