#include "AVLTree.h"

AVLTree::AVLTree(ofstream* fout){ //Constructor
	root=NULL;
	make_heap(vHeap.begin(), vHeap.end());	//heap for RANK command
	this->fout = fout;
}

AVLTree::~AVLTree(){	//Destructor

}

bool AVLTree::Insert(StockData* pSto){
	
	AVLNode* pNew = new AVLNode();	//make AVL node
	pNew->setSD(pSto);				//Set stock data
	char	*pName = pSto->getName();

	if (root == NULL) {	//if root is NULL
		root = pNew;
		return true;
	}

	AVLNode* aNode = root;	//aNode
	AVLNode* aPre = NULL;	//aNode parent
	AVLNode* pCur = root;	//moving node
	AVLNode* pPre = NULL;	//moving node parent
	AVLNode* rootSub = NULL;

	while (pCur!=NULL) {			//Searching pNew location
		if (pCur->getBF() != 0) {	//root's balance factor is not 0
			aNode = pCur;			//aNode = pCur
			aPre = pPre;			//aPre = pPre
		}
		if (strcmp(pName,pCur->getSD()->getName())<0) {	//new data name < pCur data name
			pPre = pCur;
			pCur = pCur->getLeft();	//move left child
		}
		else if (strcmp(pName, pCur->getSD()->getName()) > 0) {//new data name > pCur data name
			pPre = pCur;
			pCur = pCur->getRight();//move right child
		}
		else { //if same name exist
			delete pNew;
			return false;
		}
	}

	//Insert pNew in pPre left or right
	if (strcmp(pName, pPre->getSD()->getName()) < 0)
		pPre->setLeft(pNew);
	else
		pPre->setRight(pNew);

	int degree;			//new balance factor
	AVLNode* bNode;		//child of aNode		
	AVLNode* cNode;		//child of bNode

	//Set AVL node degree
	if (strcmp(pName, aNode->getSD()->getName()) > 0) { //new data name > aNode data name
		bNode = pCur = aNode->getRight();
		degree = -1;					//move to right, degree=-1
	}
	else {//new data name < aNode data name
		bNode = pCur = aNode->getLeft();
		degree = 1;						//move to left, degree=1
	}

	//Set AVL node banlance factor
	while (pCur != pNew) {
		if (strcmp(pName, pCur->getSD()->getName()) > 0) {
			pCur->setBF(-1);
			pCur = pCur->getRight();
		}
		else {
			pCur->setBF(1);
			pCur = pCur->getLeft();
		}
	}
	
	//check the balnce factor of aNode
	if (aNode->getBF() == 0 || aNode->getBF() + degree == 0) {
		aNode->setBF(aNode->getBF() + degree);
		return true;		//insert success, balance factor good
	}

	if (degree == 1) { //left is not balanced
		if (bNode->getBF() == 1) {//execute LL rotation
			aNode->setLeft(bNode->getRight());
			bNode->setRight(aNode);
			aNode->setBF(0);
			bNode->setBF(0);
			rootSub = bNode;
		}
		else {//excute LR rotation
			cNode = bNode->getRight();
			bNode->setRight(cNode->getLeft());
			aNode->setLeft(cNode->getRight());
			cNode->setLeft(bNode);
			cNode->setRight(aNode);
			switch (cNode->getBF()) {
			case 0 :
				bNode->setBF(0);
				aNode->setBF(0);
				break;
			case 1 :
				bNode->setBF(0);
				aNode->setBF(-1);
				break;
			case -1 :
				bNode->setBF(1);
				aNode->setBF(0);
				break;
			}
			cNode->setBF(0);
			rootSub = cNode;
		}
	}
	else {//right is not balanced
		if (bNode->getBF() == -1) { //execute RR rotation
			aNode->setRight(bNode->getLeft());
			bNode->setLeft(aNode);
			aNode->setBF(0);
			bNode->setBF(0);
			rootSub = bNode;
		}
		else {						//execute RL rotation
			cNode = bNode->getLeft();
			aNode->setRight(cNode->getLeft());
			bNode->setLeft(cNode->getRight());
			cNode->setLeft(aNode);
			cNode->setRight(bNode);
			switch (cNode->getBF()) {
			case 0 :
				aNode->setBF(0);
				bNode->setBF(0);
				break;
			case 1 :
				aNode->setBF(0);
				bNode->setBF(-1);
				break;
			case -1 :
				aNode->setBF(1);
				bNode->setBF(0);
				break;
			}
			cNode->setBF(0);
			rootSub = cNode;
		}
	}

	//after rotation
	if (!aPre)	//if root unbalanced
		root = rootSub;	//set root
	else if (aNode == aPre->getLeft())	//aNode is aPre left child
		aPre->setLeft(rootSub);
	else                                //aNode is aPre right child
		aPre->setRight(rootSub);
	return true;
}

bool AVLTree::Print(){
	
	if (root == NULL)	//if AVL tree is not exist
		return false;

	//Inorder Traversal
	stack<AVLNode*> s;
	AVLNode* pCur = root;
	while (1) {
		while (pCur) {
			s.push(pCur);
			pCur = pCur->getLeft();
		}
		if (s.empty()) break;	//stack empty -> break

		pCur = s.top();	//last in data of stack

		printStockInfo2(pCur->getSD());	//print stock info
		
		s.pop();		//pop data in stack
		pCur = pCur->getRight();
	}
	return true;
}

bool AVLTree::Search(char* name){
	
	if (root == NULL)
		return false;
	AVLNode *pCur = root; 
	
	//Searching same name of stock data
	while (pCur) {
		char *pName = pCur->getSD()->getName();
		if (strcmp(pName, name) < 0) pCur = pCur->getRight();		//move to right child
		else if (strcmp(pName, name) > 0) pCur = pCur->getLeft();	//move to left child
		else break;
	}

	if (pCur == NULL)	//not exist searching node
		return false;
	
	//print in "log.txt"
	*fout << "======= SEARCH =======" << endl;
	*fout << "SEARCH_AVL " << name << endl;
	printStockInfo(pCur->getSD());			//print stock info
	*fout << "=====================" << endl;

	StockData* pSD = pCur->getSD(); //pCur points to an AVL node with name
	vHeap.push_back(make_pair(make_pair(pSD->getEarning(), pSD->getStockID()), pSD));
	push_heap(vHeap.begin(), vHeap.end(), compare);	//heap push
	
	return true;
}

bool AVLTree::Rank(){
	if(vHeap.empty())
		return false;

	*fout << "==================== RANK =====================" << endl;

	StockData* pSto;
	while(!vHeap.empty()){	//when heap data empty
		pSto = vHeap.front().second;
		pop_heap(vHeap.begin(), vHeap.end());	//first heap data move to last heap
		vHeap.pop_back();						//pop data
		printStockInfo2(pSto);					//print in log.txt
	}
	*fout << "==============================================" << endl;
	return true;
}

void AVLTree::printStockInfo(StockData* pStockData){	//SEARCH
	fout->setf(ios::fixed);
	fout->precision(2);
	*fout << pStockData->getStockID() << " " << pStockData->getName() << " " << pStockData->getEarning() << endl;
}

void AVLTree::printStockInfo2(StockData* pStockData) {	//PRINT, RANK
	fout->setf(ios::fixed);
	fout->precision(2);
	*fout << pStockData->getStockID() << " " << pStockData->getName() << " " << pStockData->getEarning() << endl;
	*fout << "시가: " << pStockData->getOpening_Price() << endl;
	*fout << "종가: " << pStockData->getClosing_Price() << endl;
	*fout << "거래량: " << pStockData->getVolume() << endl;
	*fout << "수익률: " << pStockData->getEarning() << endl << endl;
}
