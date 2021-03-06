#include <iostream>
#include <unordered_map>
#include <fstream>
#include <ostream>
#include <sstream>
#include <tuple>
#include <vector>
#include <tuple>

using namespace std;

unordered_map<string, unordered_map<string, vector<string>>> stadium;
vector<string> block_name_array;
vector<string> row_name_array;
ofstream output_file;
int column_num;

void print(int col_num)
{
	output_file << endl;
	for(int i = 0; i < block_name_array.size(); i++){
		output_file << block_name_array[i]<<endl;
		output_file << "~~~~~~~" <<endl;
		for(int j = 0; j < row_name_array.size(); j++){
			output_file << row_name_array[j] << " : ";
			for(int k = 0; k < col_num; k++){
				for(int x = 0; x < 3; x++){ // to be able to write 3 characters
					output_file << stadium[block_name_array[i]][row_name_array[j]].at(k)[x];
				}
				output_file << " ";
			}
			output_file << endl;
		}
		output_file << "======="<<endl<<endl;
	}
}

void reserve_seat_by_row(string customer_name, string row_name)
{
	vector<int> empty_seat_num (block_name_array.size(), 0);
	
	string b_name; 
	int c_num; 

	bool not_exist = true; // controling if customer already has a reservation
	for(int i = 0; i < block_name_array.size() && not_exist; i++){
		for(int j = 0; j < row_name_array.size() && not_exist; j++){
			for(int k = 0; k < column_num && not_exist; k++){
				if(customer_name == stadium[block_name_array[i]][row_name_array[j]].at(k))
					not_exist = false;
			}
		}
	}

	// calculating empty seats in each block in given row
	for(int i = 0; i < block_name_array.size() ; i++){
		for(int k = 0; k < column_num ; k++){
			if(stadium[block_name_array[i]][row_name].at(k) == "---"){
				empty_seat_num[i]++; // indexes will match with the block_name_array
			}
		}
	}

	// finding the emptiest block
	int max = empty_seat_num[0];
	int block_index = 0;
	for(int i = 0; i < empty_seat_num.size(); i++){
		if(max < empty_seat_num[i]){
			max = empty_seat_num[i];
			block_index = i; // index of the block_name_array that has maximum number of empty seat 
		}
	}

	// if there is no empty seat in row we display appropriate message and return
	if(max == 0){
		output_file << customer_name << " could not reserve a seat!" <<endl;
		return;
	}


	if(not_exist){ // if customer does not have reservation	 
		bool found = false;
		int i = 0;
		while(!found){ // we find empty seat that firstly occures
			if(stadium[block_name_array[block_index]][row_name].at(i) == "---"){
				stadium[block_name_array[block_index]][row_name].at(i) = customer_name;
				found = true;
			}
			else{
				i++;
			}
		}
		output_file << customer_name << " has reserved " << block_name_array[block_index] << " " << row_name << "-" << i << " by emptiest block" <<endl;
	}
	else{
		output_file << customer_name << " could not reserve a seat!" <<endl;
	}
}

void reserve_seat(string customer_name, string block_name, string row_name, int col_num)
{
	bool not_exist = true;  // controling if customer already has a reservation
	for(int i = 0; i < block_name_array.size() && not_exist; i++){
		for(int j = 0; j < row_name_array.size() && not_exist; j++){
			for(int k = 0; k < col_num && not_exist; k++){
				if(customer_name == stadium[block_name_array[i]][row_name_array[j]].at(k))
					not_exist = false;
			}
		}
	}

	if(not_exist && stadium[block_name][row_name].at(col_num) == "---"){ // customer does not have reservation yet and wanted seat is available																											
		stadium[block_name][row_name].at(col_num) = customer_name;
		output_file << customer_name << " has reserved " << block_name << " " << row_name << "-" << col_num <<endl;
	}
	else{
		output_file << customer_name << " could not reserve a seat!" <<endl;
	}
}

void get_seat(string customer_name)
{	
	bool no_reservation = true; // controling if customer already has a reservation
	for(int i = 0; i < block_name_array.size(); i++){
		for(int j = 0; j < row_name_array.size(); j++){
			for(int k = 0; k < column_num; k++){
				if(customer_name == stadium[block_name_array[i]][row_name_array[j]].at(k)){ // if customer has already reservation
					output_file << "Found that " << customer_name << " has a reservation in " << block_name_array[i] << " " << row_name_array[j] << "-" << k <<endl;
					no_reservation = false; // updating no_reservation
				}
			}
		}
	}
	if(no_reservation)
		output_file << "There is no reservation made for " << customer_name << "!"<<endl;

}

void cancel_reservation(string customer_name)
{
	bool not_exist = true; // controling if customer already has a reservation
	for(int i = 0; i < block_name_array.size() && not_exist; i++){
		for(int j = 0; j < row_name_array.size() && not_exist; j++){
			for(int k = 0; k < column_num && not_exist; k++){
				if(customer_name == stadium[block_name_array[i]][row_name_array[j]].at(k)){ // if customer aldready has reservation we can cancel that reservation
					stadium[block_name_array[i]][row_name_array[j]].at(k) = "---";
					output_file << "Cancelled the reservation of " << customer_name <<endl;
					not_exist = false;
				}
			}
		}
	}
	if(not_exist) // if customer does not have any resarvation yet
		output_file << "Could not cancel the reservation for " << customer_name << "; no reservation found!" <<endl;
}



int main()
{
	string block_name, row_name;
	string customer_name;

	ifstream input_file;

	input_file.open("inputs.txt");
	output_file.open("output.txt");

	string instruction;
	string line;
	int line_counter = 0;
	while(getline(input_file, line)){
	
		if(line_counter == 0){ // if we are in 1st line
			istringstream iss(line);
			while(iss >> block_name){
				block_name_array.push_back(block_name);
			}
		}
		else if(line_counter == 1){ // if we are in 2nd line
			istringstream iss(line);
			while(iss >> row_name){	
				row_name_array.push_back(row_name);
			}
		}
		else if(line_counter == 2){ // if we are in 3rd line
			istringstream iss(line);
			iss >> column_num;

			for(int i = 0; i < block_name_array.size(); i++){ // initilazinf stadium with all customers(elemets) as "---"
				for(int j = 0; j < row_name_array.size(); j++){
					for(int k = 0; k < column_num; k++){
						stadium[block_name_array[i]][row_name_array[j]].push_back("---");
					}
				}
			}
		}
		else{ // after first three lines. We are reading instructions
			istringstream iss(line);
			iss >> instruction;

			if(instruction == "print"){ // print instruction
				print(column_num);
			}
			else if(instruction == "reserve_seat"){ // reserve_seat instruction
				int col_num;
				iss >> customer_name >> block_name >> row_name >> col_num;
				reserve_seat(customer_name, block_name, row_name, col_num);
			}
			else if(instruction == "reserve_seat_by_row"){ // reserve_seat_by_row instruction
				iss >> customer_name >> row_name;
				reserve_seat_by_row(customer_name, row_name);
			}
			else if(instruction == "get_seat"){ // get_seat instruction
				iss >> customer_name;
				get_seat(customer_name);
			}
			else if(instruction == "cancel_reservation"){ // cancel_reservation instruction
				iss >> customer_name;
				cancel_reservation(customer_name);
			}
		}
		line_counter++;
	}
	return 0;
}
