#include <iostream>
#include <fstream>
#include <sstream>
#include "ModifiedPriorityQueue.h"

using namespace std;

struct building {
	int xCoordinate;
	int label;
	// int side;
};



/*============= Heap Sort Algorithm Start =============*/
void swap(building & x, building & y)
{
	building temp = x;
	x = y;
	y = temp;
}

void heapify(vector<building> & arr, int n, int i) // heapify
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if(l < n && arr[l].xCoordinate > arr[largest].xCoordinate){
		largest = l;
	}
	if(r < n && arr[r].xCoordinate > arr[largest].xCoordinate){
		largest = r;
	}
	if(largest != i){
		swap(arr[i], arr[largest]);
		heapify(arr, n, largest);
	}
}

void heapSort(vector<building> & arr, int n) 
{
	for(int i = n / 2 - 1; i >= 0; i--){
		heapify(arr, n, i);
	}
	for(int i = n - 1; i > 0; i--){
		swap(arr[0], arr[i]);
		heapify(arr, i, 0);
	}
}
/*============= Heap Sort Algorithm End =============*/




// for checking sorting algorithm whether is working or not
/*
void printArrayCoordinates(vector<building> arr, int n)
{
    for (int i = 0; i < n; ++i)
        cout << arr[i].xVal << " ";
    cout << "\n";
}

void printArrayLabels(vector<building> arr, int n)
{
    for (int i = 0; i < n; ++i)
        cout << arr[i].label << " ";
    cout << "\n";
}
*/

int main()
{
	string line;
	int totalBuilding;

	ifstream file;
	file.open("input.txt");
	
	getline(file, line);
	istringstream s(line);
	s >> totalBuilding; // taking number of buildings from first line;

	int n = totalBuilding * 2; // size of the vector that will be created for sorting x coordinates.

	ModifiedPriorityQueue heap(totalBuilding);

	vector<building> xStorage(n); // since we have labels for each building, i can store all cordinates in the same array then i can sort it with heap sort

	vector<int> yStorage(totalBuilding + 1); // to keep track of the labels that will also refer to x coordinates. (i.e. we keep the height of the building, that labeled as 'n' in the yStorage[n]) 
	yStorage[0] = 0;  // make first element empty
	
	int i = 1;
	int xCounter = 0;
	while(getline(file, line)){ // filling coordinates of buildings to xStorage and height of the building to yStorage
		istringstream iss(line);
		iss >> xStorage[xCounter].xCoordinate >> yStorage[i] >> xStorage[xCounter + 1].xCoordinate; 
		i++;
		xCounter = xCounter + 2;
	}

	i = 0;
	int labelCounter = 1;
	while( i < n){ // filling labeles to xStorage array (n = size of the xStorage)
		xStorage[i].label = labelCounter;
		i++;
		xStorage[i].label = labelCounter;
		i++;
		labelCounter++;
	}

	heapSort(xStorage, n);
	//printArrayCoordinates(xArr, xArr.size());
	//printArrayLabels(xArr, xArr.size());

	int currentMax = -1;
	if(xStorage[0].xCoordinate != 0){
		cout << 0 << " " << 0 << endl;
		currentMax = 0;
	}
	for(unsigned int i = 0; i < xStorage.size(); i++){	
		if(i != xStorage.size() - 1 && xStorage[i].xCoordinate == xStorage[i + 1].xCoordinate){
			if(heap.isAvailable(xStorage[i].label)){
				heap.insert(yStorage[xStorage[i].label], xStorage[i].label);
			}
			else{
				heap.remove(xStorage[i].label);
			}

			while(i != xStorage.size() - 1 && xStorage[i].xCoordinate == xStorage[i + 1].xCoordinate){
				
				if(heap.isAvailable(xStorage[i + 1].label)){
					heap.insert(yStorage[xStorage[i + 1].label], xStorage[i + 1].label);
					i++;
				}
				else{
					heap.remove(xStorage[i + 1].label);
					i++;
				}
			}
		}
		else{
			if(heap.isAvailable(xStorage[i].label)){
				heap.insert(yStorage[xStorage[i].label], xStorage[i].label);
			}
			else{
				heap.remove(xStorage[i].label);
			}
		}
		if(!heap.isEmpty() && currentMax != heap.getMax()){ //check to see if the maximum height of the buildings in the MPQ has changed.
			cout << xStorage[i].xCoordinate << " " << heap.getMax() << endl;
			currentMax = heap.getMax();
		}
		else if(heap.isEmpty() && currentMax != 0){ 
			cout << xStorage[i].xCoordinate << " " << 0 << endl;
			currentMax = 0;
		}
	}
	return 0;
}
