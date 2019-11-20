#ifndef _BPTREEDATANODE_H_
#define _BPTREEDATANODE_H_

#include "BpTreeNode.h"

class BpTreeDataNode : public BpTreeNode
{
private:
	map <double, vector<pair<int, StockData*> > > mapData;	//map(Earning, vector(StockID, Pointer))
	BpTreeNode* pNext;
	BpTreeNode* pPrev;

public:
	BpTreeDataNode(){ //Constructor
		pNext = NULL;
		pPrev = NULL;
	}

	//Set Function
	void setNext(BpTreeNode* pN)	{ pNext = pN; }
	void setPrev(BpTreeNode* pN)	{ pPrev = pN; }
	//Get Function
	BpTreeNode* getNext()				{ return pNext; }
	BpTreeNode* getPrev()				{ return pPrev; }	
	map< double, vector<pair<int, StockData*> > > *getDataMap() { return &mapData; }

	void insertDataMap(double avgGrade, vector<pair<int, StockData*> > m) {
		mapData.insert(map<double, vector<pair<int, StockData*> > >::value_type(avgGrade, m)); //insert data map
	}

	void deleteMap(double n){
		mapData.erase(n);		//delete map
	}	
};

#endif
