/*************************************************************************
	> File Name: graph_traversal.cc
	> Author: clhongooo
	> Mail: clhongooo@163.com 
	> Created Time: Wed 12 Sep 2018 09:48:04 AM CST
 ************************************************************************/

#include <vector>
#include <initializer_list>
#include <iostream>
#include <queue>
#include <tuple>
using namespace std;

bool CompareVec(vector<int> left, vector<int> right){
	if(left.size() != right.size()){
		return false;
	}
	bool rflag = true;
	for(int i = 0; i < left.size(); i++){
		if(left[i] != right[i]){
			rflag = false;
			break;
		}
	}
	return rflag;
}

struct Edge{
	int lnode;
	int rnode;
	int weight;
};

class Graph{
public:
	Graph(initializer_list<int> v, initializer_list<Edge> e)
		: vertices(v)
		, edges(e) {}
	~Graph() {}

	/**
	 * \brief breadth first search
	 * \return the shortest path from source to destination.
	 */
	vector<int> BreadthFirstSearch(int src, int dst){
		int sz = vertices.size();
		vector<int> optimal_trace;
		int sum_weight = -1;
		vector<bool> flags(sz, false);

		queue<tuple<int, vector<int>>> visiting_queue;
		vector<int> trace = {src};
		visiting_queue.push(make_tuple(src, trace));

		while(visiting_queue.empty() == false){
			int cur_visit = 0;
			tie(cur_visit, trace) = visiting_queue.front();
			flags[cur_visit-1] = true;
			visiting_queue.pop();

			vector<int> vec;
			GetAdjacentNodes(cur_visit, vec);
			for(auto i : vec){
				if(flags[i-1] == true){
					continue;
				}
				if(i == dst){
					trace.push_back(i);
					int all_weight = 0;
					for(int i = 0; i < trace.size() - 1; i++){
						all_weight += GetEdgeWeight(trace[i], trace[i+1]);
					}
					if(sum_weight == -1 || sum_weight > all_weight){
						optimal_trace = trace;
						sum_weight = all_weight;
					}
				}else{
					vector<int> vec = trace;
					vec.push_back(i);
					visiting_queue.push(make_tuple(i, vec));	
				}
			}
		}
		return optimal_trace;	
	}

	/**
	 * \brief depth first search
	 * \return the optimal trace from source to destination
	 */
	vector<int> DepthFirstSearch(int src, int dst){
		int sz = vertices.size();
		vector<bool> flags(sz, false);
		stack<int> v_stack;
		v_stack.push(src);
		while(v_stack.empty() == false){
			int vertice = v_stack.pop();
			flags[vertice-1] = true;
			cout << vertice << endl;
			vector<int> vec;
			GetAdjacentNodes(vertice, vec);
			for(auto i : vec){
				if(flags[i-1] == false){
					v_stack.push(i);
				}
			}
		}
	}

private:
	void GetAdjacentNodes(int node, vector<int>& adjacents){
		for(auto edge : edges){
			if(edge.lnode == node){
				adjacents.push_back(edge.rnode);
			}else if(edge.rnode == node){
				adjacents.push_back(edge.lnode);
			}else{
				continue;
			}
		}
	}

	int GetEdgeWeight(int lnode, int rnode){
		int weight = 0;
		for(auto edge : edges){
			if(edge.lnode == lnode && edge.rnode == rnode){
				weight = edge.weight;
				break;
			}	
		}
		return weight;
	}

private:
	vector<int> vertices;
	vector<Edge> edges;
};

/**
 * test function
 */
#define TEST_PATH_FINDING(x, y, z) \
	if(CompareVec(x, y)){ \
		cout << z << " succeed!"; \
		for(auto i : x) { \
			cout << i << " "; \
		}\
		cout << endl; \
	}else{ \
		cout << z << " failed!"; \
		for(auto i : x){ \
			cout << i << " "; \
		} \
		cout << endl; \
	}

int main(){
	Graph g = {
		{1,2,3,4,5,6,7,8,9,10,11,12},
		{
			{1,2,1},{1,3,1},{1,4,1},
			{2,5,1},{2,11,1},
			{5,9,1},{5,10,1},
			{4,7,1},{4,8,1},
			{7,11,1},{7,12,1},
			{10,12,1}
		}
	};
	vector<int> cmp_vec = {1, 4, 7, 12};
	TEST_PATH_FINDING(g.BreadthFirstSearch(1, 12), cmp_vec, "breadth first search");
	return 0;
}
