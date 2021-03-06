#ifndef _MODIFIEDPRIORITYQUEUE_H
#define _MODIFIEDPRIORITYQUEUE_H

#include <iostream>
#include <vector>
using namespace std;

struct node
{
	int value;
	int label;

	node(const int & v = 0, int l = 0)
		:value(v), label(l){}
};


class ModifiedPriorityQueue
{	
	public:
		ModifiedPriorityQueue(const int & size) : location(size + 1, -1), array(size + 1) {currentSize = 0;}
		~ModifiedPriorityQueue() {currentSize = 0;}
		void insert(const int & value, int label);
		int remove(int label);
		bool isEmpty();	
		bool isAvailable(int label);		
		int getMax();
		int getLocation(int label);


	private:
		int currentSize;	
		vector<int> location; 
		vector<node> array;	  

		void percolateDown(int & hole);	
		//void buildHeap();
};
#endif