#include "ModifiedPriorityQueue.h"

// constructor already initialized in .h

// destructor already initialized in .h


// inserting an item to MPQ (from slides)
// Insert item into the priority queue, maintaining heap order.​
// Duplicates are allowed.​
void ModifiedPriorityQueue::insert(const int & value, int label)
{
	int hole = ++currentSize;

	for(; hole > 1 && value > array[hole/2].value; hole /= 2){
		
		location[array[hole / 2].label] = hole;
		array[hole] = array[hole/2];		
	}
	array[hole].value = value;
	array[hole].label = label;
	location[label] = hole;
}

// removing an item from MPQ (inpired by deleteMin from lecture slides and it was adjusted accordingly)
int ModifiedPriorityQueue::remove(int label)
{
	if(isEmpty()){ // if queue is empty we thow undeflow
		throw UNDERFLOW;
	}

	int returnValue = array[location[label]].value;
	int hole = location[label];
	array[hole] = array[currentSize--];

	percolateDown(hole);

	location[array[hole].label] = hole;
	location[label] = -1;
	return returnValue;
}

// controling MPQ is empty or not
bool ModifiedPriorityQueue::isEmpty()
{
	if(currentSize == 0)
		return true;
	else
		return false;
}

// controling that given label is available or not
// initially location array filled with -1 so, if location[label] is -1, given cell is not filled
bool ModifiedPriorityQueue::isAvailable(int label)
{
	if(location[label] == -1)
		return true;
	else
		return false;
}

int ModifiedPriorityQueue::getMax()
{
	return array[1].value;
}

int ModifiedPriorityQueue::getLocation(int label)
{
	return location[label];
}


// percolating down operation (from slides)
// Internal method to percolate down in the heap.​
// hole is the index at which the percolate begins.​
void ModifiedPriorityQueue::percolateDown(int & hole) 
{
	int child;
	int tempLabel = array[hole].label;
	int tempValue = array[hole].value;

	for(; hole * 2 <= currentSize; hole = child){
		child = hole * 2;
		if(child != currentSize && array[child + 1].value < array[child].value){
			child++;
		}
		if(array[child].value > tempValue){
			location[array[child].label] = hole;
			array[hole] = array[child];
		}
		else
			break;

		hole = child;
	}
	array[hole].value = tempValue;
	array[hole].label = tempLabel;
}