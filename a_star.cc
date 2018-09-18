/*************************************************************************
	> File Name: a_star.cc
	> Author: clhongooo
	> Mail: clhongooo@163.com 
	> Created Time: Fri 14 Sep 2018 09:41:30 AM CST
 ************************************************************************/

#include<vector>
#include<algorithm>
#include<iostream>
#include<stdlib.h>
using namespace std;

/**
 * *************initialization********************
 * start,goal
 * hvalue:it is a value of the estimated distance from the current node to the goal.
 * gvalue:it is a value of the distance from the start to the current node.
 * fvalue:fvalue = hvalue + gvalue.
 * open set:a set of the checking nodes.
 * close set:a set of the checked nodes.
 * come from tree: a tree that record the node's relationship.We can get the path from the start to the goal use it.
 *
 * *************procedure*************************
 * add the start into the open set.
 * loop execution
 *		look for the node that has a smallest fvalue from the open set, let it as the current node.
 *		remove the current node from the open set.
 *		add the current node into the close set.
 *		for each adjacent node of the current node,we do the follows:
 *			If the adjacent node is in the close set or can't be visited.
 *				We should ignore it.
 *			If the adjacent node is not in the open set.
 *				We should add the one into the open set.
 *			If the adjacent node is in the open set.
 *				We should compare the gvalue between the adjacent node and the current node with their distance.
 *					If the gvalue of the current node with their distance is smaller, make the current node as the parent of the adjacent node.and update the gvalue and the fvalue.
 *					If the gvalue of the adjacent node is smaller, pass.
 *			If the goal is in the open set, end the execution.There is a path that from the start to the goal tracing the come from tree.
 *			If the open set is null,end the execution,there is not a path from the start to the goal.
 */

struct STPoint{
	int x;
	int y;
};

class AStarNode{
public:
	AStarNode(int x, int y, bool visit)
		: parent(NULL)
		, hvalue(0)
		, gvalue(0)
		, fvalue(0)
		, access(visit)
	{
		point.x = x;
		point.y = y;
	}

	bool operator()(AStarNode l, AStarNode r){
		return true;
	}

	void set_access(bool visit) { access = visit; }
	void set_value(int h, int g, int f) { hvalue = h; gvalue = g; fvalue = f; }
	int get_fvalue()const { return fvalue; }
	int get_gvalue() const { return gvalue; }
	STPoint get_point() { return point; }
	bool get_access() { return access; }
	AStarNode* get_parent() { return parent; }
	void set_parent(AStarNode* node) { parent = node; }

private:
	AStarNode* parent;
	int hvalue;
	int gvalue;
	int fvalue;
	bool access;
	STPoint point;
};

bool AStarCompare(const AStarNode* a, const AStarNode* b){
	return a->get_fvalue() < b->get_fvalue();	
}

const int hscale = 8;
const int vscale = 6;

int main(){
	//init a map
	vector<vector<AStarNode*>> table(vscale, vector<AStarNode*>(hscale, NULL));
	for(int i = 0; i < vscale; i++){
		for(int j = 0; j < hscale; j++){
			AStarNode* node = new AStarNode(j ,i, true);
			if(j == 4 && (i > 0 && i < 4)){
				node->set_access(false);	
			}
			table[i][j] = node;
			if(table[i][j]->get_access()){
				if((i == 2 && j == 2) || (i == 2 && j == 6)){
					cout << "("<<table[i][j]->get_point().x << "," << table[i][j]->get_point().y << "*)" << " ";
				}else{
					cout << "("<<table[i][j]->get_point().x << "," << table[i][j]->get_point().y << " )" << " ";
				}
			}else{
				cout << "("<<table[i][j]->get_point().x << "," << table[i][j]->get_point().y << ".)" << " ";
			}

		}
		cout << endl;
	}
	AStarNode* start_node = table[2][2];
	AStarNode* goal_node = table[2][6];

	vector<AStarNode*> open_vec, closed_vec;
	open_vec.push_back(start_node);

	//get neighbor node
	auto get_neighbor_node = [&](AStarNode* node)->vector<AStarNode*>{
		vector<AStarNode*> result_vec;
		if(node){
			STPoint stpoint = node->get_point();
			int left = stpoint.x > 0 ? stpoint.x - 1 : stpoint.x;
			int right = stpoint.x < hscale - 1 ? stpoint.x + 1 : stpoint.x;
			int top = stpoint.y < vscale - 1 ? stpoint.y + 1 : stpoint.y;
			int bottom = stpoint.y > 0 ? stpoint.y - 1 : stpoint.y;
			for(int i = bottom; i <= top; i++){
				for(int j = left; j <= right; j++){
					if(j == stpoint.x && i == stpoint.y){
						continue;
					}
					bool ignore = false;
					//ignore if it is in the close set.
					for(auto node : closed_vec){
						if(node->get_point().x == table[i][j]->get_point().x
							&& node->get_point().y == table[i][j]->get_point().y){
							ignore = true;
							break;
						}
					}
					//ignore if it can't be visited.
					if(table[i][j]->get_access() == false){
						ignore = true;
					}

					if(ignore == false){
						result_vec.push_back(table[i][j]);
					}
				}
			}
		}
		return move(result_vec);
	};

	//judge whether the node is in the open set or not.
	auto is_in_open_set = [&](AStarNode* cur_node)->bool{
		bool is_exist = false;
		for(auto node : open_vec){
			if(cur_node->get_point().x == node->get_point().x
					&& cur_node->get_point().y == node->get_point().y){
				is_exist = true;
				break;
			}
		}
		return is_exist;
	};	
	
	//set gvalue, hvalue, fvalue
	auto calculate_node_value = [&](AStarNode* node){
		if(node){
			AStarNode* parent = node->get_parent();
			if(parent){
				int h = 0, g = 0, f = 0;
				if(node->get_point().x == parent->get_point().x
						|| node->get_point().y == parent->get_point().y){
					g = parent->get_gvalue() + 10;
				}else if(table[node->get_point().y][parent->get_point().x]->get_access() == false ||
						table[parent->get_point().y][node->get_point().x]->get_access() == false){
					g = parent->get_gvalue() + 20;
				}else{
					g = parent->get_gvalue() + 14;
				}
				h = abs(goal_node->get_point().x - node->get_point().x)*10 +
					abs(goal_node->get_point().y - node->get_point().y)*10;
				f = g + h;
				node->set_value(h, g, f);
			}
		}			
	};
	
	bool is_result_ok = false;
	//main loop
	while(true){
		//choose the lowest node and remove it from the open set.
		sort(open_vec.begin(), open_vec.end(), AStarCompare);
		AStarNode* current_node = *open_vec.begin();
		open_vec.erase(open_vec.begin());

		//add the current node into the close set.
		closed_vec.push_back(current_node);
		
		//traversal the neighbor of the current node.
		vector<AStarNode*> neighbor_vec = get_neighbor_node(current_node);
		for(auto node : neighbor_vec){
			if(is_in_open_set(node)){
				int node_gvalue = node->get_gvalue();
				int current_node_gvalue = current_node->get_gvalue();
				int distance = 0;
				if(node->get_point().x == current_node->get_point().x
						|| node->get_point().y == current_node->get_point().y){
					distance = 10;
				}else{
					distance = 14;
				}
				if(node_gvalue > current_node_gvalue + distance){
					node->set_parent(current_node);
					calculate_node_value(node);
				}
			}else{
				node->set_parent(current_node);
				calculate_node_value(node);
				open_vec.push_back(node);	
			}
		}

		//judge whether the search is finish or not.
		if(is_in_open_set(goal_node)){
			is_result_ok = true;
			break;
		}	
		if(open_vec.empty()){
			break;
		}
	}

	if(is_result_ok){
		cout << "find the shortest path[" << goal_node->get_gvalue() << "]" << endl;
		AStarNode* pnode = goal_node;
		while(pnode){
			cout << "(" << pnode->get_point().x << "," << pnode->get_point().y << ")";
			pnode = pnode->get_parent();
			if(pnode){
				cout << "->";
			}
		}
		cout << endl;
	}else{
		cout << "can't find the shortest path." << endl;
	}

	//release node
	for(int i = 0; i < vscale; i++){
		for(int j = 0; j < hscale; j++){
			delete table[i][j];
		}
	}
	return 0;
}
