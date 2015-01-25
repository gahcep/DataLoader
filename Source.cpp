#include "DataLoader.hpp"
#include <iostream>
#include <initializer_list>

using namespace DataLoader;

int main(int argc, char** argv) 
{
	Loader<float> dr_matrix("./InputData/matrix_a.raw");

	dr_matrix.delimeter(' ');
	dr_matrix.read_matrix(Flags_Matrix_Parse::DimentionsAtOnce);

	Loader<float> dr_single("./InputData/vector_newline_based.raw");

	dr_single.read_vector(Flags::NullOpts, Flags_Vector_Parse::Size | Flags_Vector_Parse::ItemsLineByLine);

	std::vector<float> newVec;
	dr_single.arg_vector(0, newVec);

	float * storage;
	storage = (float *)malloc(dr_single.arg_vector_len(0) * sizeof(float));
	dr_single.arg_vector(0, storage);

	auto vecRes = dr_single.arg_vector(0);
	
	auto err = dr_single.state();
}