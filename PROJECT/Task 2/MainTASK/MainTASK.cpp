//Signature matrix Task 2

#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <ctime>
#include<algorithm>
using namespace std;

// Structure to represent a node in the graph
struct Node {
	int index;
	double weight;
	Node(int index, double weight) : index(index), weight(weight) {}
};

void Permute(vector<vector<double>> data, int rows, int cols, string fileName)
{
	// Seed the rand() function with the current time
	srand(time(NULL));

	for (int i = 0; i < rows; i++) {
		int j = rand() % rows;

		// Swap the current row with the row at the random index
		vector<double> temp(cols);
		for (int k = 0; k < cols; k++) {
			temp[k] = data[i][k];
			data[i][k] = data[j][k];
			data[j][k] = temp[k];
		}
	}

	// Open the file in write mode
	ofstream outFile;
	outFile.open(fileName);

	// Write the permuted matrix to the file
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			outFile << data[i][j] << " ";
		}
		outFile << endl;
	}

	// Close the file
	outFile.close();
}


void DeepCopyMatrix(const vector<vector<double>>& Matrix1, vector<vector<double>>& Copy, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Copy[i][j] = Matrix1[i][j];    //Copying M1---->M2
		}

	}
}

double mean(const vector<double>& row) {
	int n = row.size();
	if (n == 0) {
		return 0.0;
	}
	double sum = 0.0;
	for (double x : row) {
		sum = sum + x;
	}
	return sum / n;
}

// A comparator function that compares the signature value of two rows
bool compareRowsBySignature(const vector<double>& row1, const vector<double>& row2) {
	double signature1 = 0.0;
	double signature2 = 0.0;
	int n1 = row1.size();
	int n2 = row2.size();
	if (n1 > 0) {
		signature1 = mean(row1) * accumulate(row1.begin(), row1.end(), 0.0);
	}
	if (n2 > 0) {
		signature2 = mean(row2) * accumulate(row2.begin(), row2.end(), 0.0);
	}
	return signature1 < signature2;
}


int main() {
	// Open the input file
	std::ifstream input("Sample data-1-IRIS.txt");

	// Read the number of rows and columns
	int rows, cols;
	input >> rows >> cols;

	// Create the 2D array to store the data
	vector<vector<double>> matrix(rows, vector<double>(cols));

	vector<vector<double>> OG_MATRIX(rows, vector<double>(cols));

	vector<vector<double>> Permute_Matrix(rows, vector<double>(cols));

	vector<vector<double>> Sign_Sort_Matrix(rows, vector<double>(cols));


	// Read the data from the file and store it in the matrix
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			input >> matrix[i][j];


		}
	}

	// Close the input file
	input.close();
	DeepCopyMatrix(matrix, OG_MATRIX, rows, cols);
	// Print the matrix to the console
	cout << "Data set is :" << endl;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;


	DeepCopyMatrix(OG_MATRIX, Permute_Matrix, rows, cols);

	Permute(Permute_Matrix, rows, cols, "permuted_Matrix.txt");

	cout << endl << "Sorted Matrix by Signature Method - Sign compared between two rows" << endl;
	DeepCopyMatrix(OG_MATRIX, Sign_Sort_Matrix, rows, cols);

	sort(Sign_Sort_Matrix.begin(), Sign_Sort_Matrix.end(), compareRowsBySignature);

	// Print the sorted matrix
	for (const vector<double>& row : Sign_Sort_Matrix) {
		for (double x : row) {
			cout << x << " ";
		}
		cout << endl;
	}
	// Open a file for writing
	ofstream sfile("signature_matrix.txt");

	// Iterate over the rows and columns of the matrix
	for (int i = 0; i < Sign_Sort_Matrix.size(); i++) {
		for (int j = 0; j < Sign_Sort_Matrix[i].size(); j++) {
			// Write the element to the file
			sfile << Sign_Sort_Matrix[i][j] << " ";
		}
		// Add a newline after each row
		sfile << endl;
	}


	return 0;
}

