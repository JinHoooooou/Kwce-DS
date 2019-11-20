#ifndef _STUDENTDATA_H_
#define _STUDENTDATA_H_

#include <iostream>
#include <cstring>
#include <fstream>
#include <map>
#include <math.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>
#include <stack>

using namespace std;

class StockData{
private:
	int	StockID;			//Stock Number
	char Name[100];			//Stock Name
	double Earning;			//Stock Earning
	int Opening_Price;		//Stock Opening Price
	int Closing_Price;		//Stock Closing Price
	int Volume;				//Stock Volume

public:
	StockData() {			//Constructor
		StockID = 0;
		memset(Name, 0, 100);
		Earning = 0;
		Opening_Price = 0;
		Closing_Price = 0;
		Volume = 0;
	};
	~StockData() {};		//Destructor

	//Set Function
	void setStockID(int a) { StockID = a; }
	void setName(char* a) { strcpy(Name, a); }
	void setOpening_Price(int a) { Opening_Price = a; }
	void setClosing_Price(int a) { Closing_Price = a; }
	void setVolume(int a) { Volume = a; }	
	void setEarning(int a, int b){
		Earning = (b - a) / (double)a;			//Stock Earning = (Closing Price - Opening Price) / Opening  Price
		Earning = round(Earning*100) / 100;		//Round to two decimal places				
	}
	
	//Get Function
	int			getStockID() { return StockID; }
	char*		getName() { return Name; }
	double	getEarning() { return Earning; }
	int getOpening_Price() { return Opening_Price; }
	int getClosing_Price() { return Closing_Price; }
	int getVolume() { return Volume; }

};

bool compare(pair< pair< double, int>, StockData* > a, pair< pair< double, int>, StockData* > b); //compare for make heap

#endif
