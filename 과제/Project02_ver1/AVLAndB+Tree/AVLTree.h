#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include "AVLNode.h"

class AVLTree{
private:
	AVLNode*	root;											//AVL Tree root
	vector < pair< pair<double, int>, StockData* > > vHeap;		//vector( ( (Earning, StockID), Pointer ) )
	ofstream*	fout;											//ofstream for log

public:
	
	AVLTree(ofstream* fout);	//Constructor
	~AVLTree();					//Destructor
	
	bool		Insert(StockData* pStu);	//LOAD
	bool		Print();					//PRINT_AVL
	bool		Search(char* name);			//SEARCH_AVL
	bool		Rank();						//RANK

	//Get Function
	AVLNode* getRoot() { return root; }

	void		printStockInfo(StockData* pStockData);
	void		printStockInfo2(StockData* pStockData);
};

#endif
