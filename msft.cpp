#include 'msft.h'
#include <unordered_set>
#include <cmath>

using namespace std;

MinSolFacTree::MinSolFacTree(vector<vector<double>> &P, string &A, int k, int l){
	vector<double> pi_arr;
	for(auto i = 0; i < P.size(); i++){
		int which_max = max_element(P[i].begin(), P[i].end()) - P[i].begin();
		H+=(A[which_max]);
		double pi = P[i][which_max];
		pi_arr.push_back(pi);
	}
	int n = P.size();
	Node* root = new Node;
	unordered_map<char, int> amap;
	for(int i = 0; i < A.size(); i++){
		amap[i] = A[i];
	}
	
	string S;
	double p = 1;
	int a = n-1;
	unordered_set<int> minimizers;
	int sig1 = -1;
	Node* v = root;
	Node* u;
	while( a != n ){
		int sig = sig1 + 1;
		if( a >= 0 && sig != A.size() ){
			if( p != 1 || A[sig] != H[a] ){
				if( p * P[a][sig] * z < 1 ){
					continue;
				}else{
					p *= P[a][sig];
				}
			}
			S = A[sig] + S;
			u = v.addChild(A[sig]);
			if(H[a] != A[sig]){
				u->addDiff(a,A[sig]);
			}
			if(S.size > l){
				minimizers.insert(a + find_minimzer_index(S.substr(0,l), k));
			}
			a = a - 1;
			v = u;
			sig1 = -1;
		}else{
			v = u->parent();
			if(p < 1){
				p /= P[a][amap[S[0]]]; ///check
			}
			if(minimizers.find(a) != minimizers.end()){
				u->setMinimizer();
				minimizers.erase(a);
			}else{
				if(u->c_num() == 1){
					v->eat(u);
				}
				if(u->c_num() == 0){
					u->self_free();
					u = nullptr;
				}
			}			
			u = v;
			a = a + 1;
			sig1 = amap[S[0]];
			S = S.substr(1);
		}
	}
}


