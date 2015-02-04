### Data Loader

Header-only C++ library written to ease a burden of loading data from text files in different formats. 

Got tired of writing a code snippet that just loads data every time you need to solve a quiz? Want to put all efforts to algorithm solving without being distracted? You can this with **Data Loader** library.

What kind of format the library supports now?

**Load vectors:**


 1. No size given, each value on new line:

    Data:  
    > 1.2  
    3.4  
    4.0  
    1.1  

    Code:  

	> `// Parse data`  
	> `Loader<float> loader("input_file.txt");`  
	> `loader.read_vector(Flags::NullOpts, Flags_Vector_Parse::ItemsLineByLine);`  
	>  
	> `// Get vector`  
	> `std::vector<float> vect;`  
	> `loader.arg_vector(0, vect);`  

 2. Size given, delimeter-based values

    File:  
    > 4  
    > 1.2;3.4;4.0;1.1  

    API:  

	> `// Parse data`  
	> `Loader<float> loader("input_file.txt");` 
	> `loader.delimeter(';');`
    > `loader.read_vector(Flags::NullOpts, Flags_Vector_Parse::Size | Flags_Vector_Parse::ItemsAtOnce);`
    > 
	> `// Get vector`  
	> `std::vector<float> vect;`  
	> `loader.arg_vector(0, vect);`


**Load matrix:**

Note: Dimentions should be provided  
Note: Only 2D matrices supported

 1. Dimentions: both values on the same line

    Data:  
    > 4 4  
	> 1.7 2.1 2.6 1.2  
	> 0.8 3.6 1.6 0.3  
	> 2.6 1.8 1.4 1.6  
	> 3.5 2.9 0.2 2.3  

    Code:  

	> `// Parse data`  
	> `Loader<float> loader("input_file.txt");`  
	> `loader.delimeter(' ');`  
	> `loader.read_matrix(Flags_Matrix_Parse::DimentionsAtOnce);`  
	>  
	> `// Get flattened matrix`  
	> `auto matrix = loader.arg_matrix();`

 2. Dimentions: each value on separate line:

    Data:  
    > 4 4  
	> 1.7 2.1 2.6 1.2  
	> 0.8 3.6 1.6 0.3  
	> 2.6 1.8 1.4 1.6  
	> 3.5 2.9 0.2 2.3  

    Code:  

	> `// Parse data`  
	> `Loader<float> loader("input_file.txt");`  
	> `loader.delimeter(' ');`  
	> `loader.read_matrix(Flags_Matrix_Parse::DimentionsAtOnce);`  
	>  
	> `// Get flattened matrix`  
	> `auto matrix = loader.arg_matrix();`    