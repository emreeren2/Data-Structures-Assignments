#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;


class QuadTree
{
	class QuadTreeNode
	{
		QuadTreeNode* NW;
		QuadTreeNode* NE;
		QuadTreeNode* SW;
		QuadTreeNode* SE;
		string cityName;
		int X_coordinate;
		int Y_coordinate;

		QuadTreeNode(const string & city, const int & x, const int & y, QuadTreeNode* nw, QuadTreeNode* ne, QuadTreeNode* sw, QuadTreeNode* se)
		:cityName(city), X_coordinate(x), Y_coordinate(y), NW(nw), NE(ne), SW(sw), SE(se){}
		friend class QuadTree;
	};

	private:
		QuadTreeNode* root;
		int x_size;
		int y_size;

		void insert(const int & x, const int & y, const string & city, QuadTreeNode* & t);
		void search(QuadTreeNode* &, int, int, int, vector<string> &)const;
		void searchForCirlce(int, int, int, QuadTreeNode* &, vector<string> &)const;
		void printTree(QuadTreeNode*);
		void makeEmpty(QuadTreeNode* &);

	public:
		QuadTree();
		~QuadTree();
		bool inBoundary(const int &, const int &);
		void setSize(const int &, const int &);
		void insert(const int &, const int &, const string &);
		void search(int, int, int);
		void printTree();
};
#endif