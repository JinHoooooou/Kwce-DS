#include "BpTree.h"

BpTree::BpTree(ofstream* fout, int order){
	this->root = NULL;
	this->order = order;
	this->fout = fout;
}

BpTree::~BpTree(){

}

void BpTree::Insert(StockData* pSto){
	double key = pSto->getEarning();	// key = Earning
	vector<pair<int, StockData*> > value;			// value = map< id, stock data* >
	value.push_back( make_pair(pSto->getStockID(), pSto) );	 //set vector's data

	if(root == NULL){ // empty B+ tree
		BpTreeNode* pDataNode = new BpTreeDataNode;	//new B+ Tree Data Node
		pDataNode->insertDataMap(key, value);	//push data in map
		root = pDataNode;						//set root = new Node
		return;
	}
		
	BpTreeNode* pDataNode = searchDataNode(key);	// seach data node with key
	map<double, vector<pair<int, StockData*> > > *m = pDataNode->getDataMap();

	if (m->find(key) == m->end()) {			//if new key is not exist in tree
		m->insert(make_pair(key, value));	//insert to data map
		if (exceedDataNode(pDataNode)) {	// check if data node must split
			splitDataNode(pDataNode);		//split data node
			pDataNode = pDataNode->getParent();	
			while (exceedIndexNode(pDataNode)) {	//check if index node must split
				splitIndexNode(pDataNode);			//split index node
				pDataNode = pDataNode->getParent();
			}
		}
	}
	else {		//if new key is exist in tree alreay
		m->find(key)->second.push_back(make_pair(pSto->getStockID(), pSto)); //push in last of element(vector)
		sort(m->find(key)->second.begin(), m->find(key)->second.end());		//StockID ascending sort
		return;
		
	}
	return;
	
}

BpTreeNode* BpTree::searchDataNode(double n){
	
	BpTreeNode* pCur = root;						//for moving b+ tree
	map<double, BpTreeNode*>::iterator itIndex;		//map iterator

	while(pCur->getMostLeftChild() != NULL){		//data node doesn't have child
		itIndex = pCur->getIndexMap()->begin();		//first map data key

		if(n < itIndex->first){				//if key less than iterator's key, move to most left child
			pCur = pCur->getMostLeftChild();
		}else{
			while(true){
				itIndex++;	//next map data
				if(itIndex == pCur->getIndexMap()->end() || n < itIndex->first){	//if find or move to last
					itIndex--;
					pCur = itIndex->second;
					break;
				}
			}
		}
	}

	return pCur;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode){
	
	int splitPosition_data = ceil((order-1)/2.0) + 1;	//split number
	BpTreeNode *pNewDataNode = new BpTreeDataNode;		//new data node
	map<double, vector<pair<int, StockData*> > >::iterator iterator = pDataNode->getDataMap()->begin();
	int i;
	for (i = 1; i < splitPosition_data; i++)
		iterator++;

	if (pDataNode == root) {//check pDataNode is root
		BpTreeNode *pNew = new BpTreeIndexNode;	//new index node for root
		root = pNew;
		//connect index node with pDataNode
		pNew->setMostLeftChild(pDataNode);	
		pDataNode->setParent(root);
	}
	pNewDataNode->setParent(pDataNode->getParent()); //connect new data node with pDataNode's parent node
	pDataNode->getParent()->insertIndexMap(iterator->first, pNewDataNode);	//insert new data node to parent node
	//connect data node next, prev
	pNewDataNode->setNext(pDataNode->getNext());
	pDataNode->setNext(pNewDataNode);
	pNewDataNode->setPrev(pDataNode);

	if (pNewDataNode->getNext())
		pNewDataNode->getNext()->setPrev(pNewDataNode);

	while (i <= order) {//move data to new data node from original to new
		pNewDataNode->insertDataMap(iterator->first, iterator->second);
		pDataNode->deleteMap((iterator++)->first);
		i++;
	}
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode){
	
	int splitPosition_index = ceil(order/2.0);	//split number
	BpTreeIndexNode* pNewIndexNode = new BpTreeIndexNode;	//new index node
	map<double, BpTreeNode* >::iterator iterator = pIndexNode->getIndexMap()->begin();
	int i;
	for (i = 1; i < splitPosition_index; i++)
		iterator++;
	if (pIndexNode == root) {	//check pIndexNode is root
		BpTreeNode *pNew = new BpTreeIndexNode;	//new index node for root
		root = pNew;
		//connect index node
		pNew->setMostLeftChild(pIndexNode);
		pIndexNode->setParent(pNew);
	}
	//connect index node, parent most left child
	pIndexNode->getParent()->insertIndexMap(iterator->first, pNewIndexNode);
	pNewIndexNode->setParent(pIndexNode->getParent());
	pNewIndexNode->setMostLeftChild(iterator->second);
	iterator->second->setParent(pNewIndexNode);
	pIndexNode->deleteMap((iterator++)->first);
	i++;

	while (i <= order) {// move data to new data node from origianl to new
		iterator->second->setParent(pNewIndexNode);
		pNewIndexNode->insertIndexMap(iterator->first, iterator->second);
		pIndexNode->deleteMap((iterator++)->first);
		i++;
	}
}

bool BpTree::exceedDataNode(BpTreeNode* pDataNode){
	
	map <double, vector<pair<int, StockData*> > > *m = pDataNode->getDataMap();

	//order is equal to the element number
	if(m->size() > order-1)		return true;
	else						return false;
}

bool BpTree::exceedIndexNode(BpTreeNode* pIndexNode){
	
	map <double, BpTreeNode*>* m = pIndexNode->getIndexMap();
	
	//order is equal to the element number
	if(m->size() > order-1)		return true;
	else								return false;
}



bool BpTree::Print(){	//PRINT_BP
	
	if (root == NULL) //not exist node in B+ Tree
		return false;
	
	BpTreeNode* pCur = root;
	
	while (pCur->getMostLeftChild()) {		//move to pCur's most child and move to data node
		pCur = pCur->getMostLeftChild();
	}
	BpTreeNode* pPre = pCur;
	while (pCur->getNext()) {			//move to data node's end
		pPre = pCur;
		pCur = pCur->getNext();
	}
	map<double, vector<pair<int, StockData*> > >::iterator pIterator; // map iterator
	*fout << "==================== PRINT =====================" << endl;

	while (pCur != NULL) {	//if pCur is not NULL
		//set iterator last map data of data node
		pIterator = pCur->getDataMap()->end();
		pIterator--;

		while (pIterator != pCur->getDataMap()->begin()) { //print Up to the first data of map data
			vector<pair<int, StockData*> >::iterator vIterator = pIterator->second.begin(); //vector iterator
			for (int i = 0; i < pIterator->second.size(); i++) { //print map data's element(stock data)
				printStockInfo2(vIterator->second);
				vIterator++;
			}
			pIterator--;
		}
		//print first data of map
		vector<pair<int, StockData*> >::iterator vIterator = pIterator->second.begin();
		for (int i = 0; i < pIterator->second.size(); i++) {
			printStockInfo2(vIterator->second);
			vIterator++;
		}

		//move prev data node
		if (pIterator == pCur->getDataMap()->begin()) {
			pCur = pPre;
			if (pCur == NULL) //if pCur moves up to first data node
				break;
			pPre = pPre->getPrev();
		}
	}
	*fout << "==============================================" << endl;
	return true;
}

bool BpTree::Search(double a, double b){
	if (root == NULL) { //not exist node in B+ Tree
		return false;
	}
	if (a > b)				//first number must be less than second number
		return false;
	if (a < -1 || b>100)	//parameter range
		return false;

	
	BpTreeNode* pStart = searchDataNode(a);
	BpTreeNode* pEnd = searchDataNode(b);
	map<double, vector<pair<int, StockData*> > >* mStart = pStart->getDataMap();
	map<double, vector<pair<int, StockData*> > >* mEnd = pEnd->getDataMap();
	
	//first map data
	map<double, vector<pair<int, StockData*> > >::iterator sIterator = mStart->begin();

	//last map data
	map<double, vector<pair<int, StockData*> > >::iterator eIterator = mEnd->end();
	eIterator--;

	while (a > sIterator->first) { //compare parameter with map's key
		sIterator++;
		if (sIterator == mStart->end()) { //if iterator move last, move to next data node
			pStart = pStart->getNext();
			mStart = pStart->getDataMap();
			sIterator = mStart->begin();
		}
	}
	while (b < eIterator->first) {	//compare parameter with map's key
		if (eIterator == mEnd->begin()) {//if iterator move first, move to prev data node
			pEnd = pEnd->getPrev();
			mEnd = pEnd->getDataMap();
			eIterator = mEnd->end();
		}
		eIterator--;
	}

	if (sIterator->first > eIterator->first) //if not exist data in input range
		return false;

	fout->setf(ios::fixed);
	fout->precision(2);
	//print up to the second decimal place
	*fout << "======= SEARCH =======" << endl;
	*fout << "SEARCH_BP " << a << " " << b << endl;

	if (a==b) {	//if input parameter same, ex) 20.00 20.00
		vector<pair<int, StockData*> >::iterator vIterator = sIterator->second.begin();
		for (int i = 0; i < sIterator->second.size(); i++) { // print map's elemetn(stock data)
			printStockInfo(vIterator->second);
			vIterator++;
		}
		*fout << "=====================" << endl;
		return true;
	}

	//print first parameter range data node
	while (sIterator != pStart->getDataMap()->end()) { 
		vector<pair<int, StockData*> >::iterator vIterator = sIterator->second.begin();
		for (int i = 0; i < sIterator->second.size(); i++) {
			printStockInfo(vIterator->second);
			vIterator++;
		}
		if (sIterator->first == eIterator->first)
			break;
		else
			sIterator++;	
	}
	if (pStart == pEnd) {
		*fout << "=====================" << endl;
		return true;
	}
	pStart = pStart->getNext();

	while (pStart != NULL) { //start data node is not NULL
		if (pStart == pEnd) {// start data node == end data node
			sIterator = pStart->getDataMap()->begin(); //set iterator first map data
			while (sIterator != eIterator) {//print up to last map data
				vector<pair<int, StockData*> >::iterator vIterator = sIterator->second.begin();
				for (int i = 0; i < sIterator->second.size(); i++) {
					printStockInfo(vIterator->second);
					vIterator++;
				}
				sIterator++; //move next iterator
			}
			//print last map data
			vector<pair<int, StockData*> >::iterator vIterator = sIterator->second.begin();
			for (int i = 0; i < sIterator->second.size(); i++) {
				printStockInfo(vIterator->second);
				vIterator++;
			}
			break; //print all data in range
		}

		sIterator = pStart->getDataMap()->begin(); //set iterator first map data
		while (sIterator != pStart->getDataMap()->end()) {//print map data while end of map
			vector<pair<int, StockData*> >::iterator vIterator = sIterator->second.begin();
			for (int i = 0; i < sIterator->second.size(); i++) {
				printStockInfo(vIterator->second);
				vIterator++;
			}
			sIterator++; //move next iterator
		}
		pStart = pStart->getNext(); //move next data node
	}
	*fout << "=====================" << endl;
	return true;
}

void BpTree::printStockInfo(StockData* pStockData){
	fout->setf(ios::fixed);
	fout->precision(2);
	//print up to the second decimal place
	*fout << pStockData->getStockID() << " " << pStockData->getName() << " " << pStockData->getEarning() << endl;
}

void BpTree::printStockInfo2(StockData* pStockData) {
	fout->setf(ios::fixed);
	fout->precision(2);
	//print up to the second decimal place
	*fout << pStockData->getStockID() << " " << pStockData->getName() << " " << pStockData->getEarning() << endl;
	*fout << "시가: " << pStockData->getOpening_Price() << endl;
	*fout << "종가: " << pStockData->getClosing_Price() << endl;
	*fout << "거래량: " << pStockData->getVolume() << endl;
	*fout << "수익률: " << pStockData->getEarning() << endl << endl;
}