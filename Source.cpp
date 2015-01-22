#include "DataLoader.hpp"
#include <iostream>

using namespace DataLoader;

int main(int argc, char** argv) 
{
	Loader<int> dr("input.txt");

	dr.read_vector(Flags::VectorNoSize);

	std::vector<int> newVec;
	dr.arg_vector(0, newVec);

	int * storage;
	storage = (int *)malloc(10 * sizeof(int));
	dr.arg_vector(0, storage);

	auto vecRes = dr.arg_vector(0);
	
	auto err = dr.state();

	auto end = 0;
}