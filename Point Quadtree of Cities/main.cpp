#include<iostream>
#include<fstream>
#include<sstream>

#include "QuadTree.h"

using namespace std;

int main()
{
	QuadTree TREE;

	ifstream file;
	file.open("cities.txt");

	string line;
	int x_size, y_size;

	getline(file,line); // takes first line and sets size of the map
	istringstream s(line);
	s >> x_size >> y_size;

	TREE.setSize(x_size, y_size); // setting private variables that refers to

	string cityName;
	int x_coordinate, y_coordinate;
	while(getline(file,line)){ // reading "cities.txt" file to read cities and coordinates
		istringstream iss(line);

		if(!line.empty()){ // if line is empty i do not do anything
			for(unsigned int i = 0; i < line.length(); i++){ // controlling is there any non-valid charachter.
				if((line[i] < 48 || line[i] > 57) && (line[i] < 65 || line[i] > 90) && (line[i] < 97 || line[i] > 122)){
					line[i] = ' '; 
				}
			}

			istringstream x(line);
			x >> cityName >> x_coordinate >> y_coordinate;
			TREE.insert(x_coordinate, y_coordinate, cityName); // putting to tree
		}
	}
	file.clear();
	file.seekg(0);

	TREE.printTree();
	cout << endl<<endl;

	
	fstream file2;
	file2.open("queries.txt");
	int search_x, search_y, radius;

	while(getline(file2, line)){ // reading "queries.txt" files to read searching coordinates and radius
		istringstream s(line);
		
		if(!line.empty()){ // if line is empty i do not do anything
			for(unsigned int i = 0; i < line.length(); i++){ // controlling is there any non-valid charachter.
				if((line[i] < 48 || line[i] > 57) && (line[i] < 65 || line[i] > 90) && (line[i] < 97 || line[i] > 122)){
					line[i] = ' ';
				}
			}

			istringstream x(line);
			x >> search_x >> search_y >> radius;
			TREE.search(search_x, search_y, radius); // searching in the tree
		}
	}
	file2.clear();
	file2.seekg(0);

	return 0;
}

