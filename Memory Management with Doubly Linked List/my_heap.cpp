#include <iostream>
#include <string>
#include "my_heap.h"

using namespace std;

/*---------------------HELPER FUNCTIONS STARTS---------------------*/

// Calculates the total size of memory block without taking into account whether used or not.
int blockedMemory(memory_block* ptr)
{
	int current = 0;
	while(ptr != NULL){	
		current = current + ptr->size;
		ptr = ptr->right;
	}
	return current;
}


// Finds the biggest free block, that is used for calculating fragmentation.
int findBiggestBlock(memory_block* ptr)
{
	int remainingBytes = MAX_CAPACITY - blockedMemory(ptr);

	int founded = 0;
	while(ptr != NULL){

		if(ptr->used == false && founded <= ptr->size)
			founded = ptr->size;

		ptr = ptr->right;
	}

	if(founded > remainingBytes)
		return founded;
	else
		return remainingBytes;
}

// Calculates number of memory blocks, that are not free
int usedMemoryBlocksCounts(memory_block* ptr)
{
	int counter = 0;
	while(ptr != NULL){	
		if(ptr->used == true){
			counter++;
		}
		ptr = ptr->right;
	}
	return counter;
}

// Calculates number of memory blocks, that are free
int freeMemoryBlocksCounts(memory_block* ptr)
{
	int counter = 0;
	while(ptr != NULL){	
		if(ptr->used == false){
			counter++;
		}
		ptr = ptr->right;
	}
	return counter;
}
/*---------------------HELPER FUNCTIONS END---------------------*/




// Constructor.
My_heap::My_heap()
{
	heap_begin = nullptr;
	blk = nullptr;
	used_bytes = 0;
}

// Destructor.
My_heap::~My_heap()
{
	cout << "At destruction, the heap had a memory leak of " << used_bytes << " bytes." <<endl;

	memory_block* ptr = heap_begin;
	while(heap_begin){
		ptr = heap_begin;
		heap_begin = ptr->right;
		delete ptr;
	}
	ptr = NULL;
	heap_begin = NULL;
	blk = NULL;
}

// This function allocates given amount of bytes to the linked list. 
// If list is empty, created block will be de first block, else block will added to the end of the list
// If there is not enough space to allocate given amount of bytes, function basically returns nullptr
memory_block* My_heap::bump_allocate(int num_bytes)
{
	memory_block* ptr;

	if(heap_begin == NULL){
		heap_begin = new memory_block();
		blk = heap_begin;

		heap_begin->right = NULL;
		heap_begin->left = NULL;
		heap_begin->used = true;
		heap_begin->size = num_bytes;
		heap_begin->starting_address = 0;

		used_bytes = used_bytes + num_bytes;
		ptr = heap_begin;
		return ptr;
	}
	else{ // if there already exist a list
		int remainingBytes = MAX_CAPACITY - used_bytes;

		if(remainingBytes >= num_bytes){ // if there is enough space for new allocation
			blk->right = new memory_block();
			ptr = blk->right; //ptr points to newly added memory block

			ptr->right = NULL;
			ptr->left = blk;
			ptr->used = true;
			ptr->size = num_bytes;
			ptr->starting_address = blk->starting_address + blk->size;
			blk = ptr; // blk now points last pointer of the list again

			used_bytes = used_bytes + num_bytes;
			return ptr;
		}
		else{ 
			return nullptr;
		}	
	}
}

// If there exist free memory block, that given number can fit, status of that block is set as free
// If there not exist such a memory block, new memory block is created via bump_allocate(91).
memory_block* My_heap::first_fit_allocate(int num_bytes)
{
	memory_block* ptr = heap_begin;

	while(ptr != NULL){
		if(ptr->used == false && ptr->size >= num_bytes){
			ptr->used = true;
			used_bytes = used_bytes + ptr->size;
			return ptr;
		}
		ptr = ptr->right;
	}

	memory_block* result = bump_allocate(num_bytes);
	return result;
}

// This function basically does same operation with first_fit_allocate, 
// however it does not use first memory block, that given number can fit,
// instead it finds a block, that has the smallest size, that given bytes can fit.
// If there not exist such a memory block, new memory block is created via bump_allocate(91).
memory_block* My_heap::best_fit_allocate(int num_bytes)
{
	memory_block* ptr1 = heap_begin;
	memory_block* ptr2 = ptr1;

	bool control = true;
	while(ptr1 != NULL ){

		if(ptr1->used == false && ptr1->size >= num_bytes){		
			ptr2 = ptr1->right;
			while(ptr2 != NULL ){

				if(ptr2->used == false && ptr2->size <= ptr1->size && ptr2->size >= num_bytes){
					ptr1 = ptr2;
					control = false;
				}
				ptr2 = ptr2->right;
			}
		}
		if(ptr2 == NULL){
			ptr1->used = true;
			used_bytes = used_bytes + ptr1->size;
			return ptr1;
		}
		ptr1 = ptr1->right;
	}

	memory_block* result = bump_allocate(num_bytes); // if we cannot return anything we try do basic allocation with bump_allocation
	return result;
}

// This function basically does same operation with first_fit_allocate, 
// however when it finds a memory block, it spilts that memory block into two parts.
// One of these part (left side) is used for allocation, and size of that block is same as given amount of bytes,
// and other part (right side) stays as free memory block, and size of that is what remains in splitted memory block.
// If given amount of byte is same as founded memory block, splittin is not occured.
memory_block* My_heap::first_fit_split_allocate(int num_bytes)
{
	memory_block* ptr = first_fit_allocate(num_bytes);
	memory_block* temp;
	
	if(ptr != NULL){
		if(ptr->size > num_bytes){
			temp = new memory_block();
			temp->right = ptr->right;
			temp->left = ptr;
			ptr->right->left = temp;
			ptr->right = temp;

			temp->used = false;
			temp->size = ptr->size - num_bytes;
			temp->starting_address = ptr->starting_address + num_bytes;

			ptr->size = num_bytes;
			used_bytes = used_bytes - temp->size;
			return ptr;
		}
		else{ // num_bytes exactly fit to the size
			return ptr;
		}
	}
	else{
		return nullptr;
	}
}

// When we dealocate a block, basically we make its status as free
// If there exists neighbors, that status also free, they are merged as one memory block.
// Merging operation is always done to the left node block.
void My_heap::deallocate(memory_block* to_delete)
{
	to_delete->used = false; 
	used_bytes = used_bytes - to_delete->size;

	memory_block* ptr = to_delete->right; // pointer points to the to_delete node
	if(to_delete->right != NULL && to_delete->right->used == false){       

		to_delete->size = to_delete->size + ptr->size;

		if(ptr->right == NULL){ // if ptr->right is NULL this means ptr is the last memory_block in the list
			blk = to_delete;
			to_delete->right = NULL;
			ptr->left = NULL;
		}
		else{
			to_delete->right = ptr->right;
			ptr->right->left = to_delete;
		}
		delete ptr;	
	}

	ptr = to_delete->left;
	if(to_delete->left != NULL && to_delete->left->used == false){ // if ptr->left is NULL this means ptr is the first memory_block in the list

		ptr->size = ptr->size + to_delete->size;

		if(to_delete->right == NULL){
			blk = ptr;
			ptr->right = NULL;
			to_delete->left = NULL;
		}
		else{
			ptr->right = to_delete->right;
			to_delete->right->left = ptr;
		}
		delete to_delete;
	}
}

// Calculating fragmantation
float My_heap::get_fragmantation()
{
	float free_memory = MAX_CAPACITY - used_bytes; // belki sorun cikarabilir!!!!!!!!!!!!!!!!!!!!!!
	float biggest_free_block = findBiggestBlock(heap_begin);
	float fragmentation;
	fragmentation = ((free_memory - biggest_free_block)/free_memory)*100;
	return fragmentation;
}

// Printing needed information about heap
void My_heap::print_heap()
{
	int currently_used = used_bytes;
	int total_used_memory_blocks = usedMemoryBlocksCounts(heap_begin);
	int total_free_memory_blocks = freeMemoryBlocksCounts(heap_begin);
	int total_memory_blocks = total_free_memory_blocks + total_used_memory_blocks;

	float frag = get_fragmantation();

	cout << "Maximum capacity of heap: " << MAX_CAPACITY << "B" <<endl;
	cout << "Currently used memory (B): " << currently_used <<endl;
	cout <<	"Total memory blocks: " << total_memory_blocks <<endl;
	cout <<	"Total used memory blocks: " << total_used_memory_blocks <<endl;
	cout <<	"Total free memory blocks: " << total_free_memory_blocks <<endl;
	cout <<	"Fragmentation: " << frag << "%" <<endl;
	cout <<	"------------------------------"<<endl;

	memory_block* ptr = heap_begin;
	string boolean;
	int counter = 0;
	while(ptr != NULL){
		
		if(ptr->used)
			boolean = "True";
		else
			boolean = "False";

		cout << "Block " << counter << "\t\tUsed: " << boolean << "\tSize (B): "<< ptr->size <<"\tStartingAdress: 0x";
		printf("%x \n" , ptr->starting_address);

		ptr = ptr->right;
		counter++;
	}
	cout <<	"------------------------------"<<endl;
	cout <<	"------------------------------"<<endl;
}