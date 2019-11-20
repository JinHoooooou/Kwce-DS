#ifndef _BPTREE_H_
#define _BPTREE_H_

#include "BpTreeIndexNode.h"
#include "BpTreeDataNode.h"

class BpTree{
private:
	BpTreeNode*	root;	//B+ Tree root
	int	order;			//m-way order
	ofstream* fout;		//ofstream for log

public:
	BpTree(ofstream* fout, int order);	// Constructor
	~BpTree();							// Destructor

	//LOAD
	void	Insert(StockData* pStockData);
	bool	exceedDataNode(BpTreeNode* pDataNode);	
	bool	exceedIndexNode(BpTreeNode* pIndexNode);
	void	splitDataNode(BpTreeNode* pDataNode);	
	void	splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode*	searchDataNode(double n);
	
	BpTreeNode* getRoot() { return root; }

	bool	Print(); // PRINT_BP
	bool	Search(double a, double b); // SEARCH_BP	

	void	printStockInfo(StockData* pStockData);
	void	printStockInfo2(StockData* pStockData);
};

#endif
