#include "Manager.h"

Manager::Manager(int bpOrder){			//Constructor
	fout.open("log.txt", ios::app);	//after the existing contents, additional output
	avl = new AVLTree(&fout);			
	bp = new BpTree(&fout, bpOrder);	
}

Manager::~Manager(){	//Destructor
	delete avl;
	delete bp;
	fout.close();
}

void Manager::run(const char* command_txt){
	ifstream fin;			//load for "command.txt"
	fin.open(command_txt);
	int	load = -1;
		
	if(!fin){				//exception handling for opening "command.txt"
		cout<<"[ERROR] command file open error!"<<endl;
		return;
	}

	char*	str=NULL;		//token command
	char*   str2 = NULL;	//token comment
	char*	str3 = NULL;	//token for SEARCH_BP

	char	buf[128]={0};	//for reading one line
	char	buf2[128]={0};	//copy buf
	double a = 0, b = 0;
	while(fin.getline(buf, 64)){	//read one line
		strcpy(buf2, buf);
		str=strtok(buf, " ");	
		str2 = strtok(NULL, " ");
		str3 = strtok(NULL, " ");

		if (str2 != NULL && str3 != NULL) {	//for SEARCH_BP 
			a = atof(str2);
			b = atof(str3);
		}

		//if coomand Sucess, print StockData, else print error code in "log.txt"
		if(strcmp(str, "LOAD") == 0){				//LOAD
			if(load == 1){
				printErrorCode(100);
			}else if((str=strtok(NULL, " \n")) != NULL || !LOAD()){	//when comment is after LOAD command
				printErrorCode(100);
			}
			load = 1;	//to prevent the LOAD command called more than once
		}
		else if (strcmp(str, "SEARCH_BP") == 0) {	//SEARCH_BP
			if (str2==NULL || str3==NULL) printErrorCode(200);
			else if (!SEARCH_BP(a, b)) printErrorCode(200);
		}
		else if(strcmp(str, "SEARCH_AVL") == 0){	//SEARCH_AVL
			if (str2 == NULL)	printErrorCode(300);
			else if (!SEARCH_AVL(str2)) printErrorCode(300);
		}
		else if(strcmp(str, "RANK") == 0){			//RANK
			if (!RANK()) printErrorCode(400);
		}
		else if (strcmp(str, "PRINT_BP") == 0) {	//PRINT_BP
			if (!PRINT_BP()) printErrorCode(500);
		}
		else if(strcmp(str, "PRINT_AVL") == 0){		//PRINT_AVL
			if (!PRINT_AVL()) printErrorCode(600);
		}
		else if (strcmp(str, "EXIT") == 0) {		//EXIT
			fin.close();
			return;
		}
		fout<<endl;
	}	
	fin.close();
}

bool Manager::LOAD(){		//load existing text file data
	
	if (avl->getRoot() != NULL)	//AVL tree is already exist
		return false;
	if (bp->getRoot() != NULL)	//B+ Tree is already exist
		return false;

	char*	str=NULL;		//token
	char	buf[128]={0};	//for read txt
	char	buf2[128]={0};	//copy buf
	
	//read "stock_list.txt"
	ifstream fin;
	fin.open("stock_list.txt");
	
	//exception handling for opening "stock_list.txt"
	if(!fin){
		return false;
	}


	while(fin.getline(buf, 64)){	//read one line
		StockData* pSto;
		strcpy(buf2, buf);			//copy buf

		pSto = new StockData();		//make node

		str = strtok(buf2, " ");
		pSto->setStockID(atoi(str));	//set ID
		str = strtok(NULL, " ");
		pSto->setName(str);				//set Name
		int open = atoi(strtok(NULL, " "));
		pSto->setOpening_Price(open);	//set Opening price
		int close = atoi(strtok(NULL, " "));
		pSto->setClosing_Price(close);	//set Closing price
		int volume = atoi(strtok(NULL, " "));
		pSto->setVolume(volume);		//set Volume

		pSto->setEarning(open, close);	//set Earning

		avl->Insert(pSto);				//insert in AVL Tree
		bp->Insert(pSto);				//inser in B+ Tree
	}
	//Print Success code in "log.txt"
	fout << "======== LOAD ========" << endl;
	fout << "Success" << endl;
	fout << "=====================" << endl;
	fin.close();
	return true;
}

bool Manager::SEARCH_AVL(char* name){	//SEARCH_AVL
	if(!avl->Search(name))	return false;	//search function in AVL tree
	else					return true;
}

bool Manager::SEARCH_BP(double a, double b){ //SEARCH_BP
	if(!bp->Search(a, b)) return false;	//search function in B+ tree
	else				  return true;
}

bool Manager::RANK(){				//Rank
	if(!avl->Rank())	return false;	//Print function in heap
	else				return true;
}

bool Manager::PRINT_AVL(){			//PRINT_AVL
	if (avl->getRoot() == NULL) return false;
	else {
		fout << "==================== PRINT =====================" << endl;
		avl->Print();				//Print function in AVL tree
		fout << "==============================================" << endl;
		return true;
	}
}

bool Manager::PRINT_BP(){			//PRINT_BP
	if(!bp->Print())		return false;	//Print function in B+ tree
	else					return true;
}

void Manager::printErrorCode(int n){	//print error code
	fout<<"======== ERROR ========"<<endl;
	fout<<n<<endl;
	fout<<"======================="<<endl;
}
