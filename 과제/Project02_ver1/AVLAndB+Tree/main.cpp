#include "Manager.h"

int main(void){
	//express to second decimal place
	cout.setf(ios::fixed);
	cout.precision(2);

	int bpOrder = 3;		//b+ Tree m order

	Manager ds(bpOrder);	
	ds.run("command.txt");

	return 0;
}

bool compare(pair< pair< double, int>, StockData* > a, pair< pair< double, int>, StockData* > b) {	//compare for make heap

	if (a.first.first == b.first.first) {
		return a.first.second > b.first.second;		//if same earning, StockID ascending
	}
	else {
		return a.first.first < b.first.first;		//else earning descending
	}
}
