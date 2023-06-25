#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using namespace std;

class SolidExtTree{
	struct setNode{
		int pos;
		setNode* parent;
		unordered_set<setNode*> children;
		unordered_map<int, char> alt;
		bool mini = false;
		setNode();
		~setNode();
		setNode * addChild(int ind);
		void setDiff(stack<pair<int,char>>& diff);
		int c_num(){ return children.size(); }
		void eat(setNode* child);
	};
	string H;
	setNode* root;
	
public:
	SolidExtTree();
	SolidExtTree(vector<vector<double>> &P, string &A, int ell, int k);
	int find_minimzer_index(string& s, int k);
};