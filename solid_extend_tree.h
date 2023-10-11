#ifndef MST_SET_H
#define MST_SET_H

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
		bool isMinimizer;
		
		unordered_set<setNode*> children;
		unordered_map<int, char> alts;
		
		setNode(): pos(0), parent(nullptr), isMinimizer(false) {}
		setNode(int index, setNode* p) : pos(index), parent(p), isMinimizer(false) {}
		~setNode();
		setNode * addChild(int ind);
		void setDiff(stack<pair<int,char>>& diff);
		void setMinimizer(){ isMinimizer = true; }
		int c_num(){ return children.size(); }
		void eat(setNode* child);
	};
	string H;
	setNode* root;
	
public:
	SolidExtTree();
	SolidExtTree(vector<vector<double>> &P, string &A, int ell, int k, double z);
	string NodeString(setNode * u);
	void bfs();
	void dfs();
	int find_minimzer_index(string& s, int k);
};

#endif //MST_SET_H  