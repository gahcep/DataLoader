#include "DataLoader.hpp"
#include <iostream>

using namespace DataLoader;

int main(int argc, char** argv) 
{
	Loader<float> dr("./InputData/vector_newline_based.raw");

	//dr.read_vector(Flags::NullOpts, Flags_Vector_Parse::Size | Flags_Vector_Parse::ItemsLineByLine);
	dr.read_vector(Flags::NullOpts, Flags_Vector_Parse::ItemsLineByLine);

	std::vector<float> newVec;
	dr.arg_vector(0, newVec);

	float * storage;
	storage = (float *)malloc(10 * sizeof(float));
	dr.arg_vector(0, storage);

	auto vecRes = dr.arg_vector(0);
	
	auto err = dr.state();

	auto end = 0;
}