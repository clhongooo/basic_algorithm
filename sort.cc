/*************************************************************************
	> File Name: sort.cc
	> Author: clhongooo
	> Mail: clhongooo@163.com 
	> Created Time: Tue 11 Sep 2018 03:41:11 PM CST
 ************************************************************************/

#include <vector>
#include <iostream>
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

/**
 * \brief bubble sort
 * \the best time complexity: O(n)
 * \the worst time complexity: O(n^2)
 * \the space complexity: O(1)
 */
vector<int> BubbleSort(vector<int> array){
	int sz = array.size();
	for(int i = 0; i < sz; i++){
		bool is_swap = false;
		for(int j = 0; j < sz - 1 - i; j++){
			if(array[j] > array[j+1]){
				swap(array[j], array[j+1]);
				is_swap = true;
			}
		}
		if(is_swap == false){
			break;
		}
	}
	return move(array);
}

/**
 * \brief insert sort
 * \the best time complexity: O(n)
 * \the worst time complexity: O(n^2)
 * \the space complexity: O(1)
 */
vector<int> InsertSort(vector<int> array){
	int sz = array.size();
	for(int i = 1; i < sz && sz > 1; i++){
		int idx = i - 1, val = array[i];
		while(idx >= 0){
			if(array[idx] > val){
				array[idx + 1] = array[idx];
				idx--;
			}else{
				break;
			}
		}
		array[idx+1] = val;
	}
	return move(array);
}

/**
 * \brief select sort, compare with bubble sort, it takes less times to call swap.
 * \the best time complexity: O(n^2)
 * \the worst time complexity: O(n^2)
 * \the space complexity: O(1)
 */
vector<int> SelectSort(vector<int> array){
	int sz = array.size();	
	for(int i = 0; i < sz-1 && sz > 1; i++){
		int idx = i, val = array[i];
		for(int j = i+1; j < sz; j++){
			if(array[j] < val){
				idx = j, val = array[j];
			}
		}
		if(idx != i){
			swap(array[i], array[idx]);
		}
	}
	return move(array);
}

/**
 * \brief quick sort
 * \the best time complexity: O(n*log(n))
 * \the worst time complexity: O(n^2)
 * \the space complexity: O(n*log(n))
 */ 
void QuickSortImp(vector<int>& array, int left, int right){
	int l = left, r = right;
	int pivot = array[(left+right)/2];
	while(l <= r){
		while(array[l] < pivot) l++;
		while(array[r] > pivot) r--;
		if(l <= r){
			swap(array[l], array[r]);
			l++, r--;
		}
	}
	if(left < r){
		QuickSortImp(array, left, r);
	}
	if(l < right){
		QuickSortImp(array, l, right);
	}
}
vector<int> QuickSort(vector<int> array){	
	int sz = array.size();
	if(sz > 1){
		QuickSortImp(array, 0, sz - 1);
	}
	return move(array);
}

/**
 * test function
 */
#define TEST_SORT(x, y, z) \
	if(CompareVec(x, y)){ \
		cout << z << " succeed!" << endl; \
	}else{ \
		cout << z << " failed!"; \
		for(auto i : x){ \
			cout << i << " "; \
		} \
		cout << endl; \
	}

int main(){
	vector<int> test_vec = {1, 4, 3, 5, 2, 9, 8, 6, 7};
	vector<int> cmp_vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	TEST_SORT(BubbleSort(test_vec), cmp_vec, "bubble sort");
	TEST_SORT(InsertSort(test_vec), cmp_vec, "insert sort");
	TEST_SORT(SelectSort(test_vec), cmp_vec, "select_sort");
	TEST_SORT(QuickSort(test_vec), cmp_vec, "quick_sort");
	return 0;
}
