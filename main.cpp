#include<fstream>
#include<iostream>
#include<sstream>
#include<string>

using namespace std;

class matrix_t {
	int ** data;
	unsigned int str;
	unsigned int col;

public:
	matrix_t() {
		data = nullptr;
		str = 0;
		col = 0;
	}

	matrix_t(matrix_t const & other) {
		this->str = other.str;
		this->col = other.col;
		this->data = new int *[this->str];
		for (unsigned int i = 0; i < this->str; i++) {
			data[i] = new int[this->col];
			for (unsigned int j = 0; j < this->col; j++) {
				this->data[i][j] = other.data[i][j];
			}
		}
	}

	matrix_t & operator =(matrix_t const & other) {
		for (unsigned int i = 0; i < str; i++) {
			delete[] this->data[i];
		}
		delete[] this->data;

		this->str = other.str;
		this->col = other.col;
		this->data = new int *[str];
		for (unsigned int i = 0; i < this->str; i++) {
			this->data[i] = new int[this->col];
			for (unsigned int j = 0; j < this->col; j++) {
				this->data[i][j] = other.data[i][j];
			}
		}
		return *this;
	}

	matrix_t sub(matrix_t & other) const {
		matrix_t result;

		if (this->str == other.str && this->col == other.col) {
			result.data = new int *[this->str];
			for (unsigned int i = 0; i < this->str; i++) {
				result.data[i] = new int[this->col];
			}
			result.str = this->str;
			result.col = this->col;
			for (unsigned int i = 0; i < this->str; i++) {
				for (unsigned int j = 0; j < this->col; j++) {
					result.data[i][j] = this->data[i][j] - other.data[i][j];
				}
			}
		}
		else {
			cout << endl << "fail";
			exit(0);
		}
		return result;
	}

	matrix_t add(matrix_t & other) const {
		matrix_t result;

		if (this->str == other.str && this->col == other.col) {
			result.data = new int *[this->str];
			for (unsigned int j = 0; j < this->str; j++) {
				result.data[j] = new int[this->col];
			}
			result.str = this->str;
			result.col = this->col;
			for (unsigned int i = 0; i < this->str; i++) {
				for (unsigned int j = 0; j < this->col; j++) {
					result.data[i][j] = this->data[i][j] + other.data[i][j];
				}
			}
		}
		else {
			cout << endl << "fail";
			exit(0);
		}
		return result;
	}

	matrix_t trans(matrix_t & other) const {
		matrix_t result;
		result.str = other.col;
		result.col = other.str;
		result.data = new int *[other.col];
		for (unsigned int i = 0; i < other.col; i++) {
			result.data[i] = new int[other.str];
		}
		for (unsigned int i = 0; i < result.str; ++i) {
			for (unsigned int j = 0; j < result.col; ++j)
			{
				result.data[i][j] = other.data[j][i];
			}
		}
		return result;
	}

	matrix_t mul(matrix_t & other) const {
		matrix_t result;
		if (this->col == other.str) {
			result.str = this->str;
			result.col = other.col;
			result.data = new int *[this->str];
			for (unsigned int i = 0; i < this->str; ++i) {
				result.data[i] = new int[other.col];
			}
			for (unsigned int i = 0; i < this->str; i++) {
				for (unsigned int j = 0; j < other.col; j++) {
					int res = 0;
					for (unsigned int k = 0; k < this->col; k++) {
						res += this->data[i][k] * other.data[k][j];
					}
					result.data[i][j] = res;
				}
			}
		}
		else {
			cout << endl << "fail";
			exit(0);
		}
		return result;
	}
	ifstream & read(std::ifstream & fin) {
		char op;
		string line;
		getline(fin, line);
		istringstream stream(line);
		if (stream >> this->str && stream >> op && op == ',' && stream >> this->col) {
			this->data = new int *[this->str];
			for (unsigned int i = 0; i < this->str; i++) {
				this->data[i] = new int[this->col];
				for (unsigned int j = 0; j < this->col; j++) {
					fin >> this->data[i][j];
				}
			}
		}
		else {
			cout << "fail razmer";
			exit(0);
		}
		fin.close();
		return fin;
	}

	ostream & write(ostream & stream) {
		for (unsigned int i = 0; i < this->str; i++) {
			for (unsigned int j = 0; j < this->col; j++) {
				stream << this->data[i][j] << ' ';
			}
			stream << endl;
		}
		return stream;
	}

	~matrix_t() {
		for (unsigned int i = 0; i < this->str; i++)
		{
			delete[] this->data[i];
		}
		delete[] this->data;
	}
};

bool read_name_of_file(string &name1, char &op, string &name2) {
	string line;
	getline(cin, line);
	istringstream stream(line);

	if (stream >> name1 && stream >> op) {
		if (op == 'T' || op == 'R') {
			return true;
		}
		else {
			if (stream >> name2) {
				return true;
			}
		}
	}
	return false;
}


int main()
{
	matrix_t matrix1;
	matrix_t result;

	char op;
	string name1, name2;
	if (!(read_name_of_file(name1, op, name2))) {
		cout << "fail";
		cin.get();
		return 0;
	}
	ifstream fin;
	const char * str = name1.c_str();
	fin.open(str);
	if (!fin.is_open()) {
		cerr << "file 1 open fail" << endl;
		return 0;
	}
	matrix1.read(fin);
	if (op == '+' || op == '-' || op == '*')
	{
		matrix_t matrix2;
		ifstream fin;
		const char * str = name2.c_str();
		fin.open(str);
		if (!fin.is_open()) {
			cerr << "file 2 open fail" << endl;
			return 0;
		}
		matrix2.read(fin);
		switch (op)
		{
		case '+':
			result = matrix1.add(matrix2);
			break;
		case '-':
			result = matrix1.sub(matrix2);
			break;
		case '*':
			result = matrix1.mul(matrix2);
			break;
		}
	}
	else if (op == 'T')
	{
		result = result.trans(matrix1);

	}
	result.write(cout);
	cin.get();
	return 0;
}
