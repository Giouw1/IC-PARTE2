#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <algorithm>

using namespace std;

//GLOBAL VARIABLE
string filename;

void swapVals(int &a, int &b) {
	int x = a; a = b; b = x;
}

void logAndExit(string s) {
	//Writes message s to screen and log file and then exits the program
	ofstream resultsLog("genLog.txt", ios::app);
	ofstream outStream;
	outStream.open(filename);
	resultsLog << s;
	cout << s;
	outStream << "NULL";
	outStream.close();
	resultsLog.close();
	exit(1);
}

void outputDegreeInfo(vector<vector<bool> > &adjacent, ofstream &outStream) {
	int i, j, n = adjacent.size();
	//Constructs the degree array and writes it to the output stream
	vector<int> deg(n, 0);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (adjacent[i][j] == true && i != j) deg[i]++;
		}
	}
	outStream << "c The degrees of nodes 1 to " << n << ", respectively, are as follows " << endl << "c [" << deg[0] << ", ";
	for (i = 1; i < n - 1; i++) {
		outStream << deg[i] << ", ";
		if (i % 25 == 0) outStream << "\nc  ";
	}
	outStream << deg[i] << "] " << endl << "c  " << endl;
}

//------------------------------------------------------------------------------------------------------------------------------------
//Procedure for making a random graph
void makeRandomGraph(vector<vector<bool> > &adjacent, int &numEdges, double p) {
	int i, j, n = adjacent.size();
	for (i = 0; i < n - 1; i++) {
		for (j = i + 1; j < n; j++) {
			if (rand() / double(RAND_MAX) < p) {
				//Add an edge between vertices i and j
				adjacent[i][j] = true;
				adjacent[j][i] = true;
				numEdges++;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------------
//Procedures for making a scale free graph
int selectLinkRoulette(vector<int> &tempDeg, int u, int totalTempDeg) {
	int i, r, cumulative = 0;
	r = rand() % totalTempDeg;
	for (i = 0; i < u; i++) {
		cumulative += tempDeg[i];
		if (cumulative > r)
			return i;
	}
	//If we are here, something has gone wrong
	logAndExit("Error. Invalid use of roulette wheel function\n");
	return -1;
}
bool makeScaleFree(vector<vector<bool> > &adjacent, int &numEdges, int m) {
	//Procedure for making a scale free graph, where m edges are added in each iteration
	int i, j, n = adjacent.size(), v, totalTempDeg, m0;
	vector<int> deg(n, 0);
	vector<int> tempDeg(n);
	if (m == 0) {
		//No edges to be added, so return the empty graoh on n vertices
		return true;
	}
	//Start with an initial connected network of m0 >= m nodes. 
	if (m > 1) {
		//We use m_0 = m nodes and connect all pairs to form a complete subgraph K_m}
		for (i = 0; i < m; i++) {
			for (j = 0; j < m; j++) {
				if (i != j) adjacent[i][j] = true;
			}
			deg[i] = m - 1;
		}
		numEdges = (m * (m - 1)) / 2;
		m0 = m;
	}
	else if (m == 1) {
		//We also use m_0 = 2 nodes and connect them to form a complete subgraph K_{ 2 }
		adjacent[0][1] = true;
		adjacent[1][0] = true;
		deg[0] = 1;
		deg[1] = 1;
		numEdges = 1;
		m0 = 2;
	}
	//Now add new nodes to the network one at a time, and give them m new edges. New links are chosen via roulette wheel selection using probabilities
	//that are proportional to the number of links that existing nodes already have. Formally, the probability p_i that the new node is connected to 
	//an existing vertex i is p_i = (deg(i))/(2*numEdges). 
	for (i = m0; i < n; i++) {
		//Add m new edges to vertex i (using only vertices 0 to i - 1)
		totalTempDeg = numEdges * 2;
		tempDeg = deg;
		for (j = 0; j < m; j++) {
			//Choose a node v that i is not currently linked to
			v = selectLinkRoulette(tempDeg, i, totalTempDeg);
			//Adjust tempDeg and totalTempDeg to ensure v is not chosen again
			totalTempDeg -= tempDeg[v];
			tempDeg[v] = 0;
			//And link nodes i and v
			adjacent[i][v] = true;
			adjacent[v][i] = true;
			deg[i]++;
			deg[v]++;
		}
		numEdges += m;
	}
	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------
//Procedures for making a random d-regular graph using the Steger and Wormald method
void removePair(int p1, int p2, vector<int> &A) {
	//remove elements at positions p1 and p2 in an array where ordering is irrelevant
	if (p1 > p2) swapVals(p1, p2);
	A[p2] = A.back();
	A.pop_back();
	A[p1] = A.back();
	A.pop_back();
}
void choosePair(vector<int> &points, int &x, int &y, int n, vector<vector<bool> > &suitable) {
	int i, j;
	for (i = 0; i < points.size() - 1; i++) {
		for (j = i + i; j < points.size(); j++) {
			if (suitable[points[i] % n][points[j] % n]) {
				x = i;
				y = j;
				return;
			}
		}
	}
	//If we are here, no pair was found
	x = y = -1;
	return;
}
bool getPairs(vector<int> &pointPairs, int n, int d) {
	int i, j, x, y, group1, group2;
	pointPairs.clear();
	//define array of all points and shuffle randomly
	vector<int> points(n*d);
	for (i = 0; i < points.size(); i++) points[i] = i;
	random_shuffle(points.begin(), points.end());
	//Define matrix for holding pairs of groups that are "suitable"
	vector<vector<bool> > suitable(n, vector<bool>(n, true));
	for (i = 0; i < n; i++) suitable[i][i] = false;
	//Main loop
	while (!points.empty()) {
		//Choose randomly two positions in points array that define a pair of suitable points
		choosePair(points, x, y, n, suitable);
		if (x != -1 && y != -1) {
			//suitable pair i and j has been found so update the data structures
			i = points[x];
			j = points[y];
			removePair(x, y, points);
			pointPairs.push_back(i);
			pointPairs.push_back(j);
			group1 = i % n;
			group2 = j % n;
			suitable[group1][group2] = false;
			suitable[group2][group1] = false;
		}
		else {
			//No suitable pair found so end
			return false;
		}
	}
	//If we are here, we have constructed our list of points
	return true;
}
bool graphIsDRegular(vector<int> &pointPairs, int n) {
	int i, u, v;
	vector<int> deg(n, 0);
	for (i = 0; i < pointPairs.size() / 2; i++) {
		u = pointPairs[i * 2] % n;
		v = pointPairs[i * 2 + 1] % n;
		deg[u]++;
		deg[v]++;
	}
	for (i = 1; i < deg.size(); i++) {
		if (deg[0] != deg[i]) return false;
	}
	return true;
}
void makeDRegularGraph(vector<vector<bool> > &adjacent, int &numEdges, int d) {
	//Make a d-regular graph. I.e. all vertices have a fixed degree of d. We do this using the Steger and Wormald method
	//This means that if d > n / 2, we make the complement and then flip the bits in the adjaency matrix. There is a small
	//chance that this method can fail, so the algorithm iterates until it succeeds.
	//Define array of point pairs
	int n = adjacent.size(), i, u, v;
	bool makeComplement = false;
	if (d > n / 2) {
		makeComplement = true;
		d = n - d - 1;
	}
	vector<int> pointPairs;
	while (true) {
		//Generate the list of points pairs
		if (getPairs(pointPairs, n, d) == true) {
			//Check output is d-regular. If so we can end
			if (graphIsDRegular(pointPairs, n) == true) {
				break;
			}
		}
	}
	//populate the adjacency matrix
	for (i = 0; i < pointPairs.size() / 2; i++) {
		u = pointPairs[i * 2] % n;
		v = pointPairs[i * 2 + 1] % n;
		adjacent[u][v] = true;
		adjacent[v][u] = true;
		numEdges++;
	}
	if (makeComplement) {
		//Flip the vertices and change number of edges
		numEdges = 0;
		for (u = 0; u < n; u++) {
			for (v = 0; v < n; v++) {
				if (u != v) {
					adjacent[u][v] = !adjacent[u][v];
					if (adjacent[u][v]) numEdges++;
				}
			}
		}
		numEdges = numEdges / 2;
	}
}

//------------------------------------------------------------------------------------------------------------------------------------
//Procedures for ensuring that endpoints of edges do not have different colours (if this option is selected)
bool hasNeighbourWithDiffColourToCol(int v, int col, vector<int> &colour, vector<vector<int> > &adjList) {
	//Returns true if v has a neighbour u that is already assigned to a colour that is not col
	int i, u;
	for (i = 0; i < adjList[v].size(); i++) {
		u = adjList[v][i];
		if (colour[u] != col && colour[u] != -1)
			return true;
	}
	//If we are here, v is not adjacent to a vertex with a colour that is not col
	return false;
}
int getSuitableColour(int v, vector<int> &colour, vector<vector<int> > &adjList, int k) {
	//A suitable colour exisits for v if and only if it has exactly zero or one different colours adjacent to it
	int i, u, numColsInNhood = 0, c = -1;
	vector<bool> colUsedInNhood(k, false);
	//Determine the colours that are adjacent to vertex v
	for (i = 0; i < adjList[v].size(); i++) {
		u = adjList[v][i];
		if (colour[u] != -1) {
			colUsedInNhood[colour[u]] = true;
		}
	}
	//Count the number of different colours adjacent to v
	for (i = 0; i < k; i++) {
		if (colUsedInNhood[i] == true) {
			numColsInNhood++;
			c = i;
		}
	}
	if (numColsInNhood == 0) {
		//v is adjacent to no colours, so assign it to any colour
		return(rand() % k);
	}
	else if (numColsInNhood == 1) {
		//v is only adjacent to one colour, colour c. So also assign it to c 
		return c;
	}
	else {
		//v is adjacent to more than one colour, so it cannot be assigned to a colour
		return -1;
	}
}
bool makePrecolouringsWithRestrictions(vector<int> &perm, vector<vector<int> > &adjList, vector<vector<bool> > &adjacent, vector<int> &colour, int k, int m, string &comment) {
	//This procedure randomly assigns colours to vertices. As usual, we ensure that each colour is used at least once; however, we also ensure that endpoints of edges 
	//do not have different colours assigned to them. This is done randomly, but with extra checks. The procedure makes "maxFails" = 100 attempts at precolouring the 
	//vertices and if it fails them all it just gives up.	
	int failedCount = 0, maxFails = 100;
	while (failedCount < maxFails) {
		int i = 0, col = 0, v, n = perm.size(), numAssigned = 0;
		bool failedColourAssignment = false;
		//First, for each colour, identify a single vertex v that can be assigned to this colour
		for (col = 0; col < k; col++) {
			for (i = 0; i < n; i++) {
				v = perm[i];
				if (colour[v] == -1) {
					if ((hasNeighbourWithDiffColourToCol(v, col, colour, adjList) == false)) {
						//We can legally assign v to colour col
						colour[v] = col;
						numAssigned++;
						break;
					}
				}
			}
			if (i >= n) {
				failedColourAssignment = true;
				break;
			}
		}
 		if (numAssigned < m && !failedColourAssignment) {
			//If we are here, we haven't failed yet, and have legally assigned one vertex to each of the k colours. We now assign the remaining (m - k) vertices
			for (i = 0; i < n; i++) {
				v = perm[i];
				if (colour[v] == -1) {
					//Try to find a suitable colour for the uncoloured vertex v. This only fails if v is adjacent to more than one colour
					col = getSuitableColour(v, colour, adjList, k);
					if (col >= 0) {
						colour[v] = col;
						numAssigned++;
						if (numAssigned == m) {
							//We have precoloured m vertices so we can end
							break;
						}
					}
				}
			}
			if (i >= n) {
				//We have failed this colour assignment
				failedColourAssignment = true;
			}
		}
		if (!failedColourAssignment) {
			//If we are here, the instance has been created succesfully, so exit
			comment = "Creation-Successful-After-(" + to_string(failedCount + 1) + "/" + to_string(maxFails) + ")-Attempts";
			return true;
		}

		//If we are here, then the colour assignment has failed, so we try again, remembering to reset the arrays.
		random_shuffle(perm.begin(), perm.end());
		colour.clear();
		colour.resize(n, -1);
		failedCount++;
	}
	//If we are here, the instance could not be created in the in "maxFails" trials, so end
	comment = "Creation-Not-Successful-After-" + to_string(maxFails) + "-Attempts";
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------
void sortVerticesByDegree(vector<vector<bool> > &adjacent) {
	//This relabels the vertices so that they are in degree order (largest first)
	int i, j, n = adjacent.size();
	//Calculate the degrees of the vertices
	vector<int> deg(n, 0);
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (adjacent[i][j] == true && i != j) deg[i]++;
		}
	}
	//Make an array with the labels of the vertices
	vector<int> label(n);
	for (i = 0; i < n; i++) label[i] = i;
	//Sort the degrees and update the labels
	for (i = n - 1; i >= 0; i--) {
		for (j = 1; j <= i; j++) {
			if (deg[j - 1] < deg[j]) {
				swapVals(deg[j - 1], deg[j]);
				swapVals(label[j - 1], label[j]);
			}
		}
	}
	//Now create the new adjacency matrix with relabelled vertices to overwrite the current one
	vector<vector<bool> > A(n, vector<bool>(n));
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			A[i][j] = adjacent[label[i]][label[j]];
		}
	}
	A.swap(adjacent);
}

//------------------------------------------------------------------------------------------------------------------------------------
void usage() {
	cout << " Maximum Happy Vertices Problem Generator:\n\n"
		<< " USAGE:\n"
		<< " -------------\n"
		<< " PARAMETERS:\n"
		<< "       -n <int>       (number of vertices. Default = 100)\n"
		<< "       -p <double>    (proportion of vertices to be preassigned to a colour. Default = 0.1)\n"
		<< "       -k <int>       (number of different colours to be used in the preassignments. Default = 5)\n"
		<< " -------------\n"
		<< " GRAPH TYPE: (default = -R 0.5)\n"
		<< "       -R <double>    (produce a random graph with the stated edge probability (between 0 and 1).\n"
		<< "       -S <int>       (produce a scale-free graph, adding the stated number of links for each new vertex (between 0 and n - 1).\n"
		<< "       -D <int>       (produce a d-regular graph where each vertex has the same, specified degree (between 0 and n - 1).\n"
		<< " ------------- \n"
		<< " OTHER OPTIONS:\n"
		<< "       -s <int>       (random seed. Default = 1)\n"
		<< "       -f <string>    (name of output file. Default = graph.txt)\n"
		<< "       -d <int>       (precolour certain vertices. 1 = random choice, 2 = highest degrees only, 3 = lowest degrees only. Default = 1)\n"
		<< "       -A             (if present we do not allow adjacent vertices to be preassigned to different colours.)\n"
		<< " -------------\n\n";
	exit(1);
}

//------------------------------------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	if (argc <= 1) {
		usage();
	}

	//Declare variables and read in the input from the command line
	int i, seed = 1, n = 100, j, numEdges = 0, numPrecol, k = 5, graphType = 1, linksPerNewNode = 1, fixedDegree = 1, precolourOption = 1;
	double edgeProb = 0.5, propPrecol = 0.1;
	filename = "graph.txt";
	bool adjPrecolAllowed = true;
	try {
		for (i = 1; i < argc; i++) {
			if (strcmp("-n", argv[i]) == 0) {
				n = atoi(argv[++i]);
			}
			else if (strcmp("-p", argv[i]) == 0) {
				propPrecol = atof(argv[++i]);
			}
			else if (strcmp("-k", argv[i]) == 0) {
				k = atoi(argv[++i]);
			}
			else if (strcmp("-R", argv[i]) == 0) {
				graphType = 1;
				edgeProb = atof(argv[++i]);
			}
			else if (strcmp("-S", argv[i]) == 0) {
				graphType = 2;
				linksPerNewNode = atoi(argv[++i]);
			}
			else if (strcmp("-D", argv[i]) == 0) {
				graphType = 3;
				fixedDegree = atoi(argv[++i]);
			}
			else if (strcmp("-A", argv[i]) == 0) {
				adjPrecolAllowed = false;
			}
			else if (strcmp("-s", argv[i]) == 0) {
				seed = atoi(argv[++i]);
			}
			else if (strcmp("-d", argv[i]) == 0) {
				precolourOption = atoi(argv[++i]);
			}
			else if (strcmp("-f", argv[i]) == 0) {
				filename = argv[++i];
			}
			else {
				logAndExit("Error: Invalid input statement. Ending...\n");
			}
		}
	}
	catch (...) {
		logAndExit("Error: Invalid input statement. Ending...\n");
	}

	//Check the input parameters. If they are invalid, end
	numPrecol = int(propPrecol * n);
	if (numPrecol < k) {
		logAndExit("Error: This combination of input parameters is illegal: -k = " + to_string(k) + " is larger than the reuired number of precoloured vertices = " + to_string(int(propPrecol * n)) + ". Ending...\n");
	}
	if (edgeProb < 0.0 || edgeProb > 1.0 || numPrecol < 0 || numPrecol > n || k > n || linksPerNewNode < 0 || linksPerNewNode >= n || fixedDegree < 0 || fixedDegree >= n) {
		logAndExit("Error: Invalid numerical values in the input statement. Ending...\n");
	}
	if (graphType == 3 && (fixedDegree * n) % 2 == 1) {
		logAndExit("Error: -D = " + to_string(fixedDegree) + " and -n = " + to_string(n) + ". Can only produce d-regular graphs when the product of these two values is even. Ending...\n");
	}
	if (precolourOption != 1 && !adjPrecolAllowed) {
		logAndExit("Error: Use of the -A option is only permitted when -d is set to 1. Ending...\n");
	}

	//Define random seed
	srand(seed);

	//Populate the adjacency matrix by constructing the graph
	vector<vector<bool> > adjacent(n, vector<bool>(n, false));
	if (graphType == 1) {
		makeRandomGraph(adjacent, numEdges, edgeProb);
	}
	else if (graphType == 2) {
		makeScaleFree(adjacent, numEdges, linksPerNewNode);
	}
	else {
		makeDRegularGraph(adjacent, numEdges, fixedDegree);
	}

	//Now sort the vertex labels so that they are labelled in increasing order of degree
	sortVerticesByDegree(adjacent);

	//And make the adjacency list
	vector<vector<int> > adjList(n, vector<int>());
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			if (adjacent[i][j]) {
				adjList[i].push_back(j);
			}
		}
	}

	//Now precolour the vertices in the graph (if we can)
	vector<int> perm(n);
	vector<int> colour(n, -1);
	bool constructedInstance = true;
	string comment;
	if (precolourOption == 2) {
		//precolour the highest degree vertices only
		for (i = 0; i < n; i++) perm[i] = i;
		random_shuffle(perm.begin(), perm.begin() + k);
	}
	else if (precolourOption == 3) {
		//precolour the lowest degree vertices only
		for (i = 0; i < n; i++) perm[i] = n - i - 1;
		random_shuffle(perm.begin(), perm.begin() + k);
	}
	else {
		//precolour the vertices randomly
		for (i = 0; i < n; i++) perm[i] = i;
		random_shuffle(perm.begin(), perm.end());
	}
	if (adjPrecolAllowed == true) {
		//Colour k vertices to ensure each colour is used at least once
		for (i = 0; i < k; i++) {
			colour[perm[i]] = i;
		}
		//and now colour colour the remaining (m - k) vertices randomly.
		for (i = k; i < numPrecol; i++) {
			colour[perm[i]] = rand() % k;
		}
		comment = "Creation-Successful";
	}
	else {
		//Otherwise, colour the vertices, but make sure that the endpoints of an edge do not have different colours
		constructedInstance = makePrecolouringsWithRestrictions(perm, adjList, adjacent, colour, k, numPrecol, comment);
	}

	//---------------------------------------------------------------------------------------
	if (constructedInstance) {
		//Output the information to file 
		ofstream outStream;
		outStream.open(filename);
		outStream << "c FILE " << filename << endl;
		if (graphType == 1) {
			outStream << "c SOURCE This is a random graph formed for the happy graph colouring problem.\n"
				<< "c DESCRIPTION It was generated using the problem generator of Rhyd Lewis\n"
				<< "c\n"
				<< "c GRAPH STATS\n"
				<< "c Seed                             = " << seed << endl
				<< "c Number of Nodes                  = " << n << endl
				<< "c Edge probability defined by user = " << edgeProb << endl
				<< "c Number of colours k              = " << k << endl
				<< "c Number of pre-assignments        = " << numPrecol << endl
				<< "c Actual density                   = " << numEdges / ((n*(n - 1)) / 2.0) << endl
				<< "c \n";
		}
		else if (graphType == 2) {
			outStream << "c SOURCE This is a scale-free graph formed for the happy graph colouring problem.\n"
				<< "c DESCRIPTION It was generated using the problem generator of Rhyd Lewis\n"
				<< "c\n"
				<< "c GRAPH STATS\n"
				<< "c Seed                             = " << seed << endl
				<< "c Number of Nodes                  = " << n << endl
				<< "c Number of colours k              = " << k << endl
				<< "c Number of pre-assignments        = " << numPrecol << endl
				<< "c Number of links per new node     = " << linksPerNewNode << endl
				<< "c Density                          = " << numEdges / ((n*(n - 1)) / 2.0) << endl
				<< "c \n";
		}
		else {
			outStream << "c SOURCE This is a d-regular graph formed for the happy graph colouring problem.\n"
				<< "c DESCRIPTION It was generated using the problem generator of Rhyd Lewis\n"
				<< "c\n"
				<< "c GRAPH STATS\n"
				<< "c Seed                             = " << seed << endl
				<< "c Number of Nodes                  = " << n << endl
				<< "c Number of colours k              = " << k << endl
				<< "c Number of pre-assignments        = " << numPrecol << endl
				<< "c Degree of all vertices           = " << fixedDegree << endl
				<< "c Density                          = " << numEdges / ((n*(n - 1)) / 2.0) << endl
				<< "c \n";
		}
		outputDegreeInfo(adjacent, outStream);
		outStream << "p edge " << n << " " << numEdges << " " << k << endl;
		//Output the edges of the graph
		for (i = 0; i < n - 1; i++) {
			for (j = i + 1; j < n; j++) {
				if (adjacent[i][j])outStream << "e " << i + 1 << ' ' << j + 1 << '\n';
			}
		}
		//Output and preassignments
		for (i = 0; i < n; i++) {
			if (colour[i] != -1) {
				outStream << "n " << i + 1 << " " << colour[i] + 1 << "\n";
			}
		}
		outStream.close();
	}
	else {
		//The graph could not be constructed. So we write the word "NULL" to the text file instead
		ofstream outStream;
		outStream.open(filename);
		outStream << "NULL";
		outStream.close();
	}
	
	//---------------------------------------------------------------------------------------
	//Whether we have constructed the problem instance succesfully or not, we now write some details to the log file
	string str;
	str = filename + "\t" + to_string(n) + "\t" + to_string(k) + "\t" + to_string(numPrecol) + "\t";
	if (graphType == 1) {
		str += "Random\t" + to_string(edgeProb) + "\t";
	}
	else if (graphType == 2) {
		str += "ScaleFree\t" + to_string(linksPerNewNode) + "\t";
	}
	else if (graphType == 3) {
		str += "d-regular\t" + to_string(fixedDegree) + "\t";
	}
	str += to_string(numEdges / ((n*(n - 1)) / 2.0)) + "\t" + to_string(seed) + "\t" + to_string(precolourOption) + "\t" + to_string(adjPrecolAllowed) + "\t" + comment + "\n";
	ofstream resultsLog("genLog.txt", ios::app);
	cout << str;
	resultsLog << str;
	resultsLog.close();

	return 0;
}
