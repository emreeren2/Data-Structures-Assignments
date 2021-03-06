#include "QuadTree.h"
int COUNTER = 0;



/*____________PRIVATE SECTION____________*/
// we have to have both private and public function. Public function calls related private functions, 
// because if we don't do that we encounter with problem that is loosing root.


// inserting elements to tree recursively
void QuadTree::insert(const int & x_coordinate, const int & y_coordinate, const string & cityName, QuadTreeNode* & t) //private insert
{
	if(t == NULL){
		t = new QuadTreeNode(cityName, x_coordinate, y_coordinate, NULL, NULL, NULL, NULL);
	}	
	else if(x_coordinate > t->X_coordinate && y_coordinate < t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){ 
		insert(x_coordinate, y_coordinate, cityName, t->SE);
	}	
	else if(x_coordinate < t->X_coordinate && y_coordinate < t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){ 
		insert(x_coordinate, y_coordinate, cityName, t->SW);
	}		
	else if(x_coordinate > t->X_coordinate && y_coordinate > t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){ 
		insert(x_coordinate, y_coordinate, cityName, t->NE);
	}
	else if(x_coordinate < t->X_coordinate && y_coordinate > t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){ 
		insert(x_coordinate, y_coordinate, cityName, t->NW); 
	}	
	else if(x_coordinate == t->X_coordinate && y_coordinate > t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){
		insert(x_coordinate, y_coordinate, cityName, t->NE);
	}
	else if(x_coordinate == t->X_coordinate && y_coordinate < t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){
		insert(x_coordinate, y_coordinate, cityName, t->SE);
	}
	else if(x_coordinate > t->X_coordinate && y_coordinate == t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){
		insert(x_coordinate, y_coordinate, cityName, t->NE);
	}
	else if(x_coordinate < t->X_coordinate && y_coordinate == t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){
		insert(x_coordinate, y_coordinate, cityName, t->SW);
	}
	else if(x_coordinate == t->X_coordinate && y_coordinate == t->Y_coordinate && inBoundary(x_coordinate, y_coordinate)){
		insert(x_coordinate, y_coordinate, cityName, t->NE);
	}
	else	
		;
}

// searching according to given search coordinates and radius. 
void QuadTree::search(QuadTreeNode* & t, int x_search, int y_search, int radius, vector<string> & city)const // private search
{
	int N_boundary = y_search + radius;
	int S_boundary = y_search - radius;
	int W_boundary = x_search - radius;
	int E_boundary = x_search + radius;

	if(t != NULL){
		//cout << t->cityName << ", ";
		city.push_back(t->cityName); // since there should be no comma end of the each line, i control this situation with vector.

		float dif_x = t->X_coordinate - x_search;
		float dif_y = t->Y_coordinate - y_search;
		int a1 = (int) pow(dif_x,2);
		int a2 = (int) pow(dif_y,2);
		int distance = sqrt(a1 + a2);

		if(distance <= radius){ // inside the circle
			search(t->SE, x_search, y_search, radius, city);
			search(t->SW, x_search, y_search, radius, city);
			search(t->NE, x_search, y_search, radius, city);	
			search(t->NW, x_search, y_search, radius, city);
		}
		else if( (t->X_coordinate >= W_boundary && t->X_coordinate <= E_boundary)  // region between square and circle
			  && (t->Y_coordinate >= S_boundary && t->Y_coordinate <= N_boundary)){

			if(t->X_coordinate >= W_boundary && t->X_coordinate <= x_search 
			&& t->Y_coordinate <= N_boundary && t->Y_coordinate >= y_search){ // 9th region
				search(t->SE, x_search, y_search, radius, city);
				search(t->SW, x_search, y_search, radius, city);
				search(t->NE, x_search, y_search, radius, city);
						
			}
			else if(t->X_coordinate <= E_boundary && t->X_coordinate >= x_search
				 && t->Y_coordinate <= N_boundary && t->Y_coordinate >= y_search){ // 10th boundary
				search(t->SE, x_search, y_search, radius, city);
				search(t->SW, x_search, y_search, radius, city);
				search(t->NW, x_search, y_search, radius, city);
				
				
			}	
			else if(t->X_coordinate >= W_boundary && t->X_coordinate <= x_search
				 && t->Y_coordinate >= S_boundary && t->Y_coordinate <= y_search){ // 11th region		
				search(t->SE, x_search, y_search, radius, city);
				search(t->NE, x_search, y_search, radius, city);
				search(t->NW, x_search, y_search, radius, city);
			}
			else if(t->X_coordinate <= E_boundary && t->X_coordinate >= x_search
			     && t->Y_coordinate >= S_boundary && t->Y_coordinate <= y_search){ // 12th region
				search(t->SW, x_search, y_search, radius, city);
				search(t->NE, x_search, y_search, radius, city);
				search(t->NW, x_search, y_search, radius, city);
			}
			else
				;// cout << " emre. nabion laaa ";	
		}
		else{ // outside the square boundary
			
			if((t->X_coordinate <= W_boundary && t->Y_coordinate >= N_boundary)){ // 1st region in figure 2.17
				search(t->SE, x_search, y_search, radius, city);
			}
			else if(t->X_coordinate >= W_boundary && t->X_coordinate <= E_boundary && t->Y_coordinate >= N_boundary){ // 2nd region
				search(t->SE, x_search, y_search, radius, city);
				search(t->SW, x_search, y_search, radius, city);		
			}
			else if(t->X_coordinate >= E_boundary && t->Y_coordinate >= N_boundary){ // 3rd region
				search(t->SW, x_search, y_search, radius, city);
			}		
			else if(t->X_coordinate <= W_boundary && t->Y_coordinate >= S_boundary && t->Y_coordinate <= N_boundary){ // 4th region
				search(t->SE, x_search, y_search, radius, city);
				search(t->NE, x_search, y_search, radius, city);
			}
			else if(t->X_coordinate >= E_boundary && t->Y_coordinate <= N_boundary && t->Y_coordinate >= S_boundary){ // 5th region
				search(t->SW, x_search, y_search, radius, city);
				search(t->NW, x_search, y_search, radius, city);
			}	
			else if(t->X_coordinate <= W_boundary && t->Y_coordinate <= S_boundary){ // 6th region
				search(t->NE, x_search, y_search, radius, city);
			}	
			else if(t->X_coordinate >= W_boundary && t->X_coordinate <= E_boundary && t->Y_coordinate <= S_boundary){ // 7th region
				search(t->NE, x_search, y_search, radius, city);
				search(t->NW, x_search, y_search, radius, city);
				
			}
			else if(t->X_coordinate >= E_boundary && t->Y_coordinate <= S_boundary){ // 8th region
				search(t->NW, x_search, y_search, radius, city);
			}
			else
				;// cout << "nabion la burda"<<endl;	
		}
	}
	COUNTER = 0;
}

// searching the cities that inside of the given circle (given its center coordinate and radius)
void QuadTree::searchForCirlce(int x_search, int y_search, int radius, QuadTreeNode* & ptr, vector<string> & city)const
{

	if(ptr!=NULL){
		float dif_x = ptr->X_coordinate - x_search;
		float dif_y = ptr->Y_coordinate - y_search;
		int a1 = (int) pow(dif_x,2);
		int a2 = (int) pow(dif_y,2);
		int distance = sqrt(a1 + a2);
		
		if(distance < radius){ // inside the circle
			COUNTER++;
			city.push_back(ptr->cityName); // since there should be no comma end of the each line, i control this situation with vector.
		}

		searchForCirlce(x_search, y_search, radius, ptr->SE, city);
		searchForCirlce(x_search, y_search, radius, ptr->SW, city);
		searchForCirlce(x_search, y_search, radius, ptr->NE, city);	
		searchForCirlce(x_search, y_search, radius, ptr->NW, city);
	} 	
}
// recursive print tree function
void QuadTree::printTree(QuadTreeNode* root)  // private printTree
{
	if(root != NULL){							 
		cout << root->cityName <<endl;
		printTree(root->SE);				
		printTree(root->SW);			   		 
		printTree(root->NE);					
		printTree(root->NW);		}}

// make tree empty recursively
void QuadTree::makeEmpty(QuadTreeNode* & t)
{
	if(t != NULL){
		makeEmpty(t->SE);
		makeEmpty(t->SW);
		makeEmpty(t->NE);
		makeEmpty(t->NW);
		
		delete t;
	}
	t = NULL;
}





/*____________PUBLIC SECTION____________*/

// constructor
QuadTree::QuadTree()
{
	root = NULL;
	//x_size = 0;
	//y_size = 0;
}

// destructor. we call private makeEmpt() function 
QuadTree::~QuadTree()
{
	makeEmpty(root);
}

// we control given search coordinates to look that point is inside of the map or not
bool QuadTree::inBoundary(const int & x, const int & y) // public function
{
	if(x < 0 || x > x_size){
		return false;
	}
	else if(y < 0 || y > y_size){
		return false;
	}
	return true; // otherwise
}

// we read the first line of cities.txt and take this number to adjust our private size variables,
// which refer total size of the map.
void QuadTree::setSize(const int & xSize, const int & ySize) // public function
{
	x_size = xSize;
	y_size = ySize;
}

// public search
// calling private insert with pointer that points to root
void QuadTree::insert(const int & x_coordinate, const int & y_coordinate, const string & cityName) 
{
	insert(x_coordinate, y_coordinate, cityName, root);
}

// public search function
// we call private function with pointer that points to the root
void QuadTree::search(int x_coordinate, int y_coordinate, int radius) 
{
	QuadTreeNode* ptr = root;
	vector<string> inCircle;
	vector<string> allPath;

	searchForCirlce(x_coordinate, y_coordinate, radius, ptr, inCircle); // firstly we look inide the circle. for displaying first line 

	if(COUNTER == 0){       // when we display cities that inside the circle, if there is city, we increment counter(initally 0) by one, if we cannot increment counter
		cout << "<None>" ;  // and it is still 0 this means we do not found any city inside that circle. Additionally we make counter 0 when we complete each line
	}
	else{	
		int size_inCircle = inCircle.size() - 1;
		for(int i = 0; i < size_inCircle; i++){
			cout << inCircle[i] << ", ";
		}
		cout << inCircle[size_inCircle]; // we display last element of the vector (which is the last element, that is printed to consol) without comma.
	}
	cout <<endl;

	search(root, x_coordinate, y_coordinate, radius, allPath); // secondly we look whole map, for displaying second line of the each queries
	int size_allPath = allPath.size() - 1;
	for(int i = 0; i < size_allPath; i++){
		cout << allPath[i] << ", ";
	}
	cout << allPath[size_allPath]; // we display last element of the vector (which is the last element, that is printed to consol) without comma.
	cout << endl <<endl;
}

// public print function
// we call private print function with pointer that points to root.
void QuadTree::printTree() 
{
	printTree(root);}