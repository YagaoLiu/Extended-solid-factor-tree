#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class SolidExtTree{
	struct setNode{
		setNode* parent;
		unordered_map<char, setNode*> children;
		unordered_map<int, char> alt;
		bool mini = false;
		setNode();
		setNode * addChild(char c);
		void addDiff(int i, char c);
	};
	string H;
	setNode* root;
	
public:
	SolidExtTree();
	SolidExtTree(vector<vector<double>> &P, string &A, int ell, int k);
	find_minimzer_index(string& s, int k);
};