#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"

class BpTreeIndexNode : public BpTreeNode{
private:
	map <double, BpTreeNode*> mapIndex; //map(earning, Pointer)
	
public:
	BpTreeIndexNode(){}
	~BpTreeIndexNode(){}
	
	void insertIndexMap(double n, BpTreeNode* pN){
		mapIndex.insert(map<double, BpTreeNode*>::value_type(n, pN));	//insert index map
	}

	void deleteMap(double n){
		mapIndex.erase(n);		//delete map
	}

	//Get Function
	map <double, BpTreeNode*>* getIndexMap()	{ return &mapIndex; }
};

#endif