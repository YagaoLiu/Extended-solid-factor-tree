#include <cmath>
#include <algorithm>
#include "solid_extend_tree.h"

using namespace std;

SolidExtTree::setNode* SolidExtTree::setNode::addChild(int ind){
	setNode* child = new setNode(ind, this);
	this->children.insert(child);
	return child;
}

void SolidExtTree::setNode::setDiff(stack<pair<int, char>>& diff){
	stack<pair<int, char> > d_copy = diff;
	while(!d_copy.empty()){
		auto p = d_copy.top();
		alts.insert(p);
		d_copy.pop();
	}
}

void SolidExtTree::setNode::eat(setNode* child){
	setNode* grandchild = *child->children.begin();
	grandchild->parent = this;
	this->children.erase(child);
	this->children.insert(grandchild);
	child->children.clear();
	delete child;
}

SolidExtTree::setNode::~setNode(){
	for (auto child : children) {
		delete child;
	}
	unordered_set<setNode*>().swap(children);
	unordered_map<int, char>().swap(alts);
}
	

SolidExtTree::SolidExtTree(vector<vector<double>> &P, string &A, int k, int l, double z){
	vector<double> pi_arr;
	for(auto i = 0; i < P.size(); i++){
		int which_max = max_element(P[i].begin(), P[i].end()) - P[i].begin();
		H+=(A[which_max]);
		double pi = P[i][which_max];
		pi_arr.push_back(pi);
	}
	int n = P.size();
	setNode* root = new setNode;
	unordered_map<char, int> amap;
	for(int i = 0; i < A.size(); i++){
		amap[A[i]] = i;
	}
	
	string S;
	double p = 1;
	int a = n-1;
	unordered_set<int> minimizers;
	stack<pair<int, char>> diff;
	int sig1 = -1;
	setNode* v = root;
	setNode* u;
	while( a != n ){
		cout << "a=" << a << endl;
		int sig = sig1 + 1;
		if( a >= 0 && sig != A.size() ){		
			if( p != 1 || A[sig] != H[a] ){
				if( p * P[a][sig] * z < 1 ){
					sig1 = sig;
					continue;
				}else{
					p *= P[a][sig];
				}
			}
			S.insert(0, 1, A[sig]);
			u = v->addChild(a);
			if(H[a] != A[sig]){
				diff.emplace(a, A[sig]);
			}
			if(S.size() > l){
				string prefixS = S.substr(0,l);
				minimizers.insert(a + find_minimzer_index(prefixS, k));
			}
			a = a - 1;
			v = u;
			sig1 = -1;
		}else{
			v = u->parent;
			if(p < 1){
				p /= P[a+1][amap[S[0]]];
			}
			if(minimizers.find(a) != minimizers.end()){
				u->setMinimizer();
				minimizers.erase(a);
				u->setDiff(diff);
			}else{
				if(u->c_num() == 0){
					delete u;
				}
				if(u->c_num() == 1){
					v->eat(u);
					u = nullptr;
				}
				if (diff.top().first == a) {
					diff.pop();
				}
			}
			u = v;
			a = a + 1;
			sig1 = amap[S[0]];
			if(S.length() > 0) S = S.substr(1);
		}
	}
}

int SolidExtTree::find_minimzer_index (string& s, int k){
	int  minimizer_index = 0;
	for (int i = 1; i <= s.length() - k; i++) {
		if (s.substr(i, k) < s.substr(minimizer_index, k)) {
		  minimizer_index = i;
		}
	}
	return minimizer_index;
}

string SolidExtTree::NodeString( setNode* u ){
	int begin = u->parent->pos+1;
	int l = u->pos - begin + 1;
	string tempH = H;
	if(!u->alts.empty()){
		for(auto alt : u->alts){
			tempH[alt.first] = alt.second;
		}
	}
	string uStr = tempH.substr(begin, l);		
	return uStr;
}