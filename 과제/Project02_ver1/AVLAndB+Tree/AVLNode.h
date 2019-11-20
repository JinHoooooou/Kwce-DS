#ifndef _AVLNODE_H_
#define _AVLNODE_H_

#include "StockData.h"

class AVLNode
{
private:
	AVLNode* pRight;
	AVLNode* pLeft;
	StockData* pStockData;
	int mBF;	//Balance Factor

public:
	AVLNode(){				//Constructor
		pRight = NULL;
		pLeft = NULL;
		pStockData = NULL;
		mBF = 0;
	}
	~AVLNode(){};			//Destructor

	//Set Function
	void setRight(AVLNode* SN) { pRight = SN; }
	void setLeft(AVLNode* SN) { pLeft = SN; }
	void setSD(StockData* pSto) { pStockData = pSto; }
	void setBF(int a) { mBF = a; }

	//Get Function
	AVLNode* getRight() { return pRight; }
	AVLNode* getLeft() { return pLeft; }
	StockData* getSD() { return pStockData; }
	int getBF()	{ return mBF; }
};

#endif
