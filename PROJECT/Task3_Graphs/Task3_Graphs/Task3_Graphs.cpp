#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

// A structure to represent an edge in the graph
struct Edge {
	int neighbor;
	double weight;
};

// A structure to represent a node in the graph
struct Node {
	int id;
	vector<Edge> edges;
	double weight;
};

// A structure to represent a weighted graph
struct Graph {
	int numNodes;
	Node* nodes;
};

// Adds an edge to the graph
void addEdge(Graph& graph, int source, int target, double weight) {
	if (source != target) {
		// Add the edge to the source node's adjacency list
		Edge edge;
		edge.neighbor = target;
		edge.weight = weight;
		graph.nodes[source].edges.push_back(edge);
		// Increase the weight of the source node by the weight of the edge
		graph.nodes[source].weight += weight;
	}
}

vector<vector<double>> filterGraph(const vector<vector<double>>& correlationMatrix, double threshold) {
	int rows = correlationMatrix.size();
	int cols = correlationMatrix[0].size();
	vector<vector<double>> filteredMatrix(rows, vector<double>(cols));

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (correlationMatrix[i][j] >= threshold) {
				filteredMatrix[i][j] = correlationMatrix[i][j];
			}
			else {
				filteredMatrix[i][j] = 0;
			}
		}
	}

	return filteredMatrix;
}
vector<int> getHighestWeightCluster(const vector<vector<double>>& filteredMatrix, double threshold) {
	int rows = filteredMatrix.size();
	vector<bool> visited(filteredMatrix.size(), false);

	vector<int> cluster;
	vector<int> neighbors;

	// Find the node with the highest weight
	int highestWeightNode = -1;
	double highestWeight = -1;
	for (int i = 0; i < rows; i++) {
		if (i < 0 || i >= rows) continue; // skip invalid indices
		double weight = 0;
		for (int j = 0; j < rows; j++) {
			if (filteredMatrix[i][j] >= threshold) {
				weight += filteredMatrix[i][j];
			}
		}
		if (weight > highestWeight) {
			highestWeight = weight;
			highestWeightNode = i;
		}
	}

	// Add the highest weight node to the cluster
	cluster.push_back(highestWeightNode);
	visited[highestWeightNode] = true;

	// Find the neighbors of the highest weight node
	for (int j = 0; j < rows; j++) {
		if (filteredMatrix[highestWeightNode][j] >= threshold && !visited[j]) {
			neighbors.push_back(j);
			visited[j] = true;
		}
	}

	return cluster;
	return neighbors;
}

vector<vector<int>> getAllClusters(vector<vector<double>>& filteredMatrix, double threshold) {
	vector<vector<int>> clusters;

	while (true) {
		// Find the highest weight cluster
		vector<int> cluster = getHighestWeightCluster(filteredMatrix, threshold);

		// If the cluster is empty, we are done
		if (cluster.empty()) {
			break;
		}

		// Add the cluster to the list of clusters
		clusters.push_back(cluster);

		// Remove the nodes in the cluster from the filtered matrix
		for (int i : cluster) {
			for (int j = 0; j < (int)filteredMatrix.size(); j++) {
				filteredMatrix[i][j] = 0;
				filteredMatrix[j][i] = 0;
			}
			filteredMatrix[i][i] = 0;
		}
	}

	return clusters;
}

int main() {
	// Read the matrix from a text file
	ifstream file("correlation_matrix.txt");
	int rows = 125, cols = 125;
	vector<vector<double>> correlationMatrix(rows, vector<double>(cols));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			file >> correlationMatrix[i][j];
		}
	}

	// Print the matrix
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << correlationMatrix[i][j] << " ";
		}
		cout << endl;
	}


	double threshold;
	// Read in the threshold value from the user
	cout << "Enter the threshold value: ";
	cin >> threshold;


	// Filter the graph and save the new matrix
	vector<vector<double>> filteredMatrix = filterGraph(correlationMatrix, threshold);

	// Print the filtered matrix
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << filteredMatrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	// Get the highest weight cluster
	vector<int> cluster = getHighestWeightCluster(filteredMatrix, threshold);


	// Print the cluster and its neighbors
	cout << "Cluster nodes: ";
	for (int node : cluster) {
		cout << node << " ";
	}
	cout << endl;
	cout << "Neighbors: ";
	for (int node : cluster) {
		for (int j = 0; j < rows; j++) {
			if (filteredMatrix[node][j] >= threshold && j != node) {
				cout << j << " ";
			}
		}
	}
	cout << endl;

	ofstream cfile("cluster.txt");
	for (int node : cluster) {
		cfile << node << " ";
	}
	cfile.close();

	return 0;
}


