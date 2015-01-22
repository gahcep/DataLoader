#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <type_traits>

namespace DataLoader
{
	enum class Kind 
	{
		Arbitrary,
		Vector,
		Matrix
	};

	namespace Flags 
	{
		// [ReadVector]

		/* Assume that size for vector is not provided */
		const unsigned VectorNoSize = 1;
		/* Read size of vector and validate if the number of given items corresponds to */
		const unsigned CheckVectorLength = (1 << 1);
		/* Check if all vectors have the same length */
		const unsigned CheckVectorsLenEquality = (1 << 2);

		// [ReadMatrix]

		/* Read dimensions (row, col) */
		const unsigned ReadDimentions = (1 << 3);
		/* Read dimentions: each value on separate line {val_1 \n val_2} */
		const unsigned ReadDimLineByLine = (1 << 4);
		/* Read dimentions: each value on the same line {val_1 <delim> val_2 } */
		const unsigned ReadDimOneLine = (1 << 5);

		// [ReadArbitrary]

		/* If met delimeter separated values, read one by one */
		const unsigned ReadSeparately = (1 << 6);
		/* If met delimeter separated values, read as vector */
		const unsigned ReadContinuously = (1 << 7);
	}

	namespace State
	{
		const unsigned Idle = 1; // ok
		const unsigned FileAccessError = (1 << 1); // bad
		const unsigned FileReadFormatError = (1 << 2); // fail
		const unsigned FileReadFailure = (1 << 3); // ios::bad
		const unsigned VectorItemsWrongNumber = (1 << 4); // fail
		const unsigned VectorsHaveDifferentLength = (1 << 4); // fail
	}

	// Restricting the usage of Loader class
	template<class U>
	struct EnableForArithmeticType {
		typedef typename std::enable_if<std::is_arithmetic<U>::value>::type type;
	};

	template<typename U, typename Enable = EnableForArithmeticType<U>::type>
	class Loader
	{
	private: 

		// Name of the input data file
		std::string _file;

		// Default delimeter
		char _delimeter{ ',' };

		// Reader state
		unsigned _state{ State::Idle };

		// Service variables
		size_t sizeRow{ 0 }, sizeCol{ 0 };

		// Storage for read data
		std::vector<std::vector<U>> _storageVector;

		void split(std::string line, std::vector<U>& storage)
		{
			std::stringstream sstream(line);
			std::stringstream sconvert;
			std::string item;
			U num;
			while (std::getline(sstream, item, _delimeter)) {
				sconvert.str(item);
				sconvert >> num;
				sconvert.clear();
				storage.push_back(num);
			}
		}

	public:

		explicit Loader(std::string file) : _file(file) {};

		// Settings: change delimeter character (',' by default)
		inline void delimeter(char delimeter) { _delimeter = delimeter; }

		// Service: get current state
		inline unsigned state() const { return _state; }

		// Service: state check aliases
		inline bool is_ok() const { return _state == State::Idle; };
		inline bool is_bad() const { return _state == State::FileAccessError || _state == State::FileReadFailure; };
		inline bool is_fail() const { return !is_ok() && !is_bad(); };

		// Service: reset internal state
		void refresh()
		{
			_state = State::Idle;
			_storageVector.clear();
		}

		// Service: get vector length
		size_t arg_vector_len(size_t idx) const
		{
			if (idx >= _storageVector.size())
				return;

			return _storageVector[idx].size();
		}

		// Return data vector
		std::vector<U> arg_vector(size_t idx)
		{
			if (idx >= _storageVector.size())
				return std::vector<U>{};

			return _storageVector[idx];
		};

		// Fill in given vector storage
		void arg_vector(size_t idx, std::vector<U>& storage)
		{
			if (idx >= _storageVector.size())
				return;

			storage.clear();
			storage.reserve(_storageVector[idx].size());
			std::copy(_storageVector[idx].begin(), _storageVector[idx].end(), std::back_inserter(storage));
		};

		// Fill in given array storage
		void arg_vector(size_t idx, U* storage)
		{
			if (!storage || idx >= _storageVector.size())
				return;

			for (auto v : _storageVector[idx])
				*storage++ = U(v);
		}

		// Parse input file without data specification
		void read_arbitrary(unsigned opts = Flags::ReadSeparately, Kind type = Kind::Arbitrary)
		{}

		// Parse input file for vector-based data
		void read_vector(unsigned opts = Flags::VectorNoSize, Kind type = Kind::Vector)
		{
			size_t lenNextVector{ 0 }, lenAllVectors{ 0 };
			std::string line, line1, line2;
			std::fstream fstream;
			std::istringstream sstream;

			fstream.open(_file, std::ios_base::in);

			if (!fstream.is_open())
			{
				_state = State::FileAccessError;
				return;
			}

			_storageVector.clear();

			// Flags
			auto checkVecLength = opts & Flags::CheckVectorLength;
			auto checkVecEquality = opts & Flags::CheckVectorsLenEquality;
			while (std::getline(fstream, line))
			{
				std::vector<U> item;

				// Read vector's length
				if (checkVecLength)
				{
					sstream.str(line);
					sstream >> lenNextVector;
					sstream.clear();

					// Read vector's items
					std::getline(fstream, line);

					if (fstream.rdstate() != std::ios::goodbit)
					{
						if (fstream.fail()) _state = State::FileReadFormatError;
						if (fstream.bad()) _state = State::FileReadFailure;
						break;
					}

					item.reserve(lenNextVector);
				}
				
				// Parse vector's elements
				split(line, item);

				// Check if vectors are equal
				if (checkVecEquality)
				{
					if (lenAllVectors == 0)
						lenAllVectors = item.size();
					else if (lenAllVectors != item.size())
					{
						_state = State::VectorsHaveDifferentLength;
						break;
					}
				}

				// Check if given length for vector items is valid
				if (checkVecLength && lenNextVector != item.size())
				{
					_state = State::VectorItemsWrongNumber;
					break;
				}
				_storageVector.push_back(item);
			}
			fstream.close();
		}

		// Parse input file for matrix-based data
		void ReadMatrix(unsigned opts = (Flags::ReadDimentions | Flags::ReadDimOneLine), Kind type = Kind::Matrix)
		{
			std::string line, line1, line2;
			std::fstream fstream;
			std::stringstream sstream;

			fstream.open(_file, std::ios_base::in);

			// Flags::ReadDimentions
			if (opts & (Flags::ReadDimentions | Flags::ReadDimLineByLine))
			{
				std::getline(fstream, line1);
				std::getline(fstream, line2);

				// Row
				sstream.str(line1);
				sstream >> sizeRow;
				sstream.clear();

				// Col
				sstream.str(line2);
				sstream >> sizeCol;
				sstream.clear();
			}

			// Flags::ReadDimentions
			if (opts & (Flags::ReadDimentions | Flags::ReadDimOneLine))
			{
				std::getline(fstream, line);
				sstream.str(line);
				sstream >> sizeRow >> sizeCol;
				sstream.clear();
			}
		}
	};
}
