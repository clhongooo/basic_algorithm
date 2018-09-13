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
#include <stack>
#include <functional>
#include <set>
#include <map>
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

#define MAX_VALUE 99999

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
		vector<int> v_trace, optimal_trace;
		int sum_weight = -1;

		v_stack.push(src);
		flags[src-1] = true;
		while(v_stack.empty() == false){
			int vertice = v_stack.top();
			v_stack.pop();
			v_trace.push_back(vertice);

			vector<int> vec;
			GetAdjacentNodes(vertice, vec);

			for(auto i : vec){
				if(i == dst){
					vector<int> trace = {vertice, dst};
					int cmp_val = vertice;
					for(int j = v_trace.size() - 2; j >= 0; j--){
						if(IsAdjacentNode(cmp_val, v_trace[j])){
							trace.insert(trace.begin(), v_trace[j]);
							cmp_val = v_trace[j];
						}	
					}
					
					int all_weight = 0;
					for(int i = 0; i < trace.size()-1; i++){
						all_weight += GetEdgeWeight(trace[i], trace[i+1]);
					}
					if(sum_weight == -1 || sum_weight > all_weight){
						optimal_trace = trace;
						sum_weight = all_weight;
					}
				}else{
					if(flags[i-1] == false){
						v_stack.push(i);
						flags[i-1] = true;
					}
				}
			}
		}
		return optimal_trace;
	}

	/**
	 * \brief floyd algorithm, it's a dp method.
	 * \return the optimal trace from source to destination
	 * \the time complexity: O(n^3)
	 * \the space complexity: O(n^2)
	 */
	vector<int> Floyd(int src, int dst){
		//generate a table for store the shortest path from i to j
		int sz = vertices.size();
		vector<vector<int>> val_table(sz, vector<int>(sz, 0));
		for(int i = 0; i < sz; i++){
			for(int j = 0; j < sz; j++){
				int val = IsAdjacentNode(vertices[i], vertices[j]) == false ? MAX_VALUE :
					GetEdgeWeight(vertices[i], vertices[j]);
				val_table[i][j] = val;
			}
		}

		//generate a table for store paths from i to j
		vector<vector<int>> path_table(sz, vector<int>(sz, -1));

		//code algorithm
		for(int k = 0; k < sz; k++){
			for(int i = 0; i < sz; i++){
				for(int j = 0; j < sz; j++){
					if(val_table[i][j] > val_table[i][k] + val_table[k][j]){
						val_table[i][j] = val_table[i][k] + val_table[k][j];
						path_table[i][j] = k;
					}		
				}
			}
		}

		//generate the trace
		vector<int> optimal_trace;
		function<void(int, int)> generate_optimal_trace;
		generate_optimal_trace = [&](int s, int d){
			if(path_table[s][d] != -1){
				int mid = path_table[s][d];
				optimal_trace.insert(optimal_trace.begin(), vertices[mid]);
				generate_optimal_trace(s, mid);
				generate_optimal_trace(mid, d);
			}
		};
		
		generate_optimal_trace(0, 11);
		optimal_trace.insert(optimal_trace.begin(), src);
		optimal_trace.push_back(dst);
		return optimal_trace;
	}

	/**
	 * \brief dijkstra algorithm
	 * \return the optimal trace from source to destination
	 */
	int Dijkstra(int src, int dst){
		//generate a set of non-visited nodes
		set<int> non_vset;
		for(auto v : vertices){
			if(v != src){
				non_vset.insert(v);
			}	
		}
		
		//generate a set of visited nodes
		set<int> visited_set;
		visited_set.insert(src);
	
		//a container of trace value from i to j
		map<int, int> val_map;
		for(auto v : vertices){
			if(v != src){
				if(IsAdjacentNode(v, src)){
					val_map[v] = GetEdgeWeight(v, src);
				}else{
					val_map[v] = MAX_VALUE;
				}
			}
		}

		//code algorithm
		while(non_vset.empty() == false){
			int cur_node = -1, min_val = -1;
			for(auto v : non_vset){
				if(min_val == -1 || min_val >= val_map[v]){
					cur_node = v;
					min_val = val_map[v];
				}
			}
			non_vset.erase(cur_node);
			visited_set.insert(cur_node);
			
			//fix adjacent node
			vector<int> vec;
			GetAdjacentNodes(cur_node, vec);
			for(auto v : vec){
				if(non_vset.count(v) != 0){
					if(val_map[v] > val_map[cur_node] + GetEdgeWeight(v, cur_node)){
						val_map[v] = val_map[cur_node] + GetEdgeWeight(v, cur_node);
					}
				}	
			}
		}

		return val_map[dst];
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
			if((edge.lnode == lnode && edge.rnode == rnode) || 
				(edge.lnode == rnode && edge.rnode == lnode)){
				weight = edge.weight;
				break;
			}	
		}
		return weight;
	}

	bool IsAdjacentNode(int lnode, int rnode){
		for(auto edge : edges){
			if((lnode == edge.lnode || lnode == edge.rnode)
					&& (rnode == edge.lnode || rnode == edge.rnode)){
				return true;
			}
		}
		return false;
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

#define TEST_RESULT(x, y, z) \
	if(x == y){ \
		cout << z << " succeed!result:" << y; \
	}else{ \
		cout << z << " failed!result:" << y; \
	} \
	cout << endl;

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
	TEST_PATH_FINDING(g.DepthFirstSearch(1, 12), cmp_vec, "depth first search");
	TEST_PATH_FINDING(g.Floyd(1, 12), cmp_vec, "floyd search");
	TEST_RESULT(g.Dijkstra(1, 12), 3, "dijkstra search");
	return 0;
}
