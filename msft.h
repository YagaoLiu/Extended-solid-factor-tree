#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class MinSolFacTree{
	struct Node{
		Node* parent;
		unordered_map<char, Node*> children;
		unordered_map<int, char> alt;
		bool mini = false;
		Node();
	};
	string H;
	Node* root;
	
public:
	MinSolFacTree();
	MinSolFacTree(vector<vector<double>> &P, string &A, int ell, int k);
};