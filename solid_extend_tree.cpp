#include <cmath>
#include <algorithm>
#include <queue>
#include "solid_extend_tree.h"
#include "krfp.h"

using namespace std;

bool isEqual(double a, double b) {
	double epsilon = 1e-14;
    return std::abs(a - b) <= epsilon * std::max(std::abs(a), std::abs(b));
}

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
	int n = P.size();
	vector<double> pi_prefix(n,1);
	for(auto i = 0; i < P.size(); i++){
		int which_max = max_element(P[i].begin(), P[i].end()) - P[i].begin();
		H+=(A[which_max]);
		double pi = P[i][which_max];
		if(i == 0){
			pi_prefix[i] = log2(pi);
		}else{
			pi_prefix[i] = pi_prefix[i-1] + log2(pi);
		}
	}
	root = new setNode(n, nullptr);
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
	int pos1 = 0;
	setNode* v = root;
	setNode* u;
	//cout << "Heavy string: " << H << endl;
	while( a != n ){
		int sig = sig1 + 1;
		//cout << "at position: " << a << " with letter: " << A[sig] << " and probability: " << p << endl;
		if( a >= 0 && sig != A.size() ){
			if( p != 1 || A[sig] != H[a] ){
				if( p * P[a][sig] * z < 1 ){
					sig1 = sig;
					continue;
				}else{
					p *= P[a][sig];
				}
			}else{
				pos1 = a;
			}
			S.insert(0, 1, A[sig]);
			u = v->addChild(a);
			//cout << "add child (" << a << " , " << A[sig] << ")" << endl;
			if(H[a] != A[sig]){
				diff.emplace(a, A[sig]);
				//stack<pair<int,char>> tmpD = diff;
				//cout << "diff check:";
				//while(!tmpD.empty()){
				//	cout << "(" << tmpD.top().first << "," << tmpD.top().second << ") ";
				//	tmpD.pop();
				//}cout << endl;
			}
			if(S.size() >= l){
				double pi_cum = 1;
				if( pos1 <= 0 ){	
					pi_cum = pow(2,pi_prefix[l]);	//if full length S is the heavy string and reach the beginning, directly use pi prefix
				}else{
					pi_cum = p * pow(2, pi_prefix[a+l-1] - pi_prefix[pos1-1]);
				}
				if(pi_cum * z >= 1){
					//cout << "compute minimizer" << endl;
					string prefixS = S.substr(0,l);
					minimizers.insert(a-1 + find_minimzer_index(prefixS, k));
				}
			}
			a = a - 1;
			v = u;
			sig1 = -1;
		}else{
			v = u->parent;
			if(!isEqual(p,1)){
				//cout << "P<1 here" << endl;
				p /= P[a+1][amap[S[0]]];
			}
			if(minimizers.find(a) != minimizers.end()){
				//cout << "confirm node (" << u->pos << " , " << S[0] << ") as minimizer" << endl;
				u->setMinimizer();
				minimizers.erase(a);
				u->setDiff(diff);
				//stack<pair<int,char>> tmpD = diff;
				//cout << "set diff:";
				//while(!tmpD.empty()){
				//	cout << "(" << tmpD.top().first << "," << tmpD.top().second << ") ";
				//	tmpD.pop();
				//}cout << endl;
			}else if(u != root){
				if(u->c_num() == 0){
					//cout << "remove node (" << u->pos << " , " << S[0] << ")\n";
					v->children.erase(u);
					delete u;
					
				}
				if(u->c_num() == 1){
					//cout << "eat node (" << u->pos << " , " << S[0] << ")\n";
					v->eat(u);
					u = nullptr;
				}
			}
			if ( !diff.empty()){
				if(diff.top().first == a+1) {
					//cout << "remove in diff: (" << diff.top().first << "," << diff.top().second << ")" << endl;
					diff.pop();
				}
			}
			u = v;
			a = a + 1;
			if(S.length() > 0){
				sig1 = amap[S[0]];
				S = S.substr(1);
			}
		}
	}
}

int SolidExtTree::find_minimzer_index (string& text,int k){
	int n = text.length();
	int fp = 0;
	int smallest_fp = fp;
	
	int * FP = ( int * ) malloc( ( n - k + 1  ) *  sizeof( int ) );
	
	for(int j = 0; j<k; j++)
		fp =  karp_rabin_hashing::concat( fp, text[j] , 1 );
		
	FP[0] = fp;
	int pos = 1;
	//cout<<fp<<endl;
	int min_fp = fp;
	int minimizers = 0;
	
	// find all fps for all k substrings
	for(int j = 1; j<=n-k; j++)
	{
		fp = karp_rabin_hashing::concat( fp, text[j+k-1] , 1 );
		fp = karp_rabin_hashing::subtract( fp, text[j-1] , k );
		
		//cout<<fp<<endl;
		FP[pos] = fp;
		pos++;
	}	
	
		
	// minimum fp in first window
   	for (int j = 0; j <= n - k ; j++) 
   	{
		if(FP[j] < min_fp){
			min_fp = FP[j];
			minimizers = j;
		}
 	}
	
	free( FP );
	return minimizers;
}

string SolidExtTree::NodeString( setNode* u ){
	int begin = u->pos;
	string tempH = H;
	if(!u->alts.empty()){
		for(auto alt : u->alts){
			tempH[alt.first] = alt.second;
		}
	}
	string uStr = tempH.substr(begin);		
	return uStr;
}

void SolidExtTree::bfs() {
    queue<SolidExtTree::setNode*> q;
    q.push(root);

    while (!q.empty()) {
        int n = q.size();

        for (int i = 0; i < n; i++) {
            SolidExtTree::setNode* curr = q.front();
            q.pop();
			
			if(curr != root){
				cout << NodeString(curr) << " ";
			}

            for (auto child : curr->children) {
                q.push(child);
            }
        }
		cout << endl;
    }
}

void SolidExtTree::dfs() {
    stack<setNode*> s;
    s.push(root);
    while (!s.empty()) {
        setNode* curr = s.top();
        s.pop();
		
		if(curr != root){
			cout << NodeString(curr) << endl;
		}
		
        for (auto child : curr->children) {
            s.push(child);
        }
    }
}