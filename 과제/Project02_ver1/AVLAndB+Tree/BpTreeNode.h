#ifndef _BpTreeNode_H_
#define _BpTreeNode_H_

#include "StockData.h"

class BpTreeNode{
private:
	BpTreeNode* pParent;
	BpTreeNode* pMostLeftChild;

public:
	BpTreeNode(){
		pParent = NULL;
		pMostLeftChild = NULL;
	}

	//Set Function
	void setMostLeftChild(BpTreeNode* pN)	{ pMostLeftChild = pN; }
	void setParent(BpTreeNode* pN)				{ pParent = pN; }
	
	//Get Function
	BpTreeNode* getParent()						{ return pParent;	}
	BpTreeNode* getMostLeftChild()				{ return pMostLeftChild; }

	//////////////////////////////////////////////////virtual Function/////////////////////////////////////////////////////

	//Set Function
	virtual void setNext(BpTreeNode* p) {}
	virtual void setPrev(BpTreeNode* p) {}
	//Get Function
	virtual BpTreeNode* getNext() { return NULL; }
	virtual BpTreeNode* getPrev() { return NULL; }
	virtual map <double, vector<pair<int, StockData*> > >* getDataMap() { map <double, vector<pair<int, StockData*> > > m; return &m; }
	virtual map <double, BpTreeNode*>* getIndexMap() { map<double, BpTreeNode*>m; return &m; }


	virtual void insertDataMap(double a, vector<pair<int, StockData*> > b) {}
	virtual void insertIndexMap(double n, BpTreeNode* pN) {}
	virtual void deleteMap(double n) {}
};

#endif
