#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void HeapSort(vector<vector<int>> &A, int size);
void BuildMaxHeap(vector<vector<int>> &A, int size);
void MaxHeapify(vector<vector<int>> &A, int i, int size);
int FindMaxVal(vector<vector<int>> &A);
int LatestNconflict(vector<vector<int>> &A, int index);

void HeapSort(vector<vector<int>> &A, int size){
    BuildMaxHeap(A, size);
    for(int i = A.size()-1; i > 0; i--){
        vector<int> temp = A[0];
        A[0] = A[i];
        A[i] = temp;
        size--;
        MaxHeapify(A, 0, size);
    }

}

void BuildMaxHeap(vector<vector<int>> &A, int size){
    for(int i = A.size()/2 - 1; i >= 0; i--)
        MaxHeapify(A, i, size);
}

void MaxHeapify(vector<vector<int>> &A, int i, int size){
    int l = 2*i + 1;
    int r = 2*i + 2;
    int largest;
    if(l <= size-1 && A[l][1] > A[i][1])
        largest = l;
    else
        largest = i;
    if(r <= size-1 && A[r][1] > A[largest][1])
        largest = r;
    if(largest != i){
        vector<int> temp = A[i];
        A[i] = A[largest];
        A[largest] = temp;
        MaxHeapify(A, largest, size);
    }
    
}

int FindMaxVal(vector<vector<int>> &A){
    int last;
    vector<int> maxVal;
    for(int i = 0; i < A.size(); i++)
        maxVal.push_back(0);
    maxVal[0] = A[0][2];
    for(int i = 1;i < maxVal.size(); i++){
        last = LatestNconflict(A, i);
        if(last == -1)
            maxVal[i] = max(maxVal[i-1], A[i][2]);
        else
            maxVal[i] = max(maxVal[i-1], A[i][2] + maxVal[last]);
    }
    return maxVal[maxVal.size()-1];
}
int LatestNconflict(vector<vector<int>> &A, int index){
    int left = 0, right = index - 1, mid;

    while(left <= right){
        mid = (left + right) / 2;
        if(A[mid][1] < A[index][0])
            if(A[mid+1][1] < A[index][0])
                left = mid + 1;
            else
                return mid;
        else
            right = mid - 1;
    }
    return -1;
}

int main() {
    int m, n, exp_max;
    vector<vector<int>> books;
    vector<int> maxVal;
    cin >> n >> m;

    for(int i = 0; i < m; i++){
        int s, e, ex;
        cin >> s >> e >> ex;
        books.push_back({s, e, ex});
    }

    HeapSort(books, books.size());
    int ans = FindMaxVal(books);
    cout << ans;

    return 0;
}