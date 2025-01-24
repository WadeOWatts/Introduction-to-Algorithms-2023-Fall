#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

void merge(vector<vector<int>> &A, int p, int q, int r);
void mergeSort(vector<vector<int>> &A, int p, int r);

void merge(vector<vector<int>> &A, int p, int q, int r){
    int n1 = q - p + 1;
    int n2 = r - q;
    vector<vector<int>> left, right;
    for(int i = 0; i < n1; i++)
        left.push_back({A[p + i][0], A[p + i][1], A[p + i][2]});
    for(int j = 0; j < n2; j++)
        right.push_back({A[q+j+1][0], A[q+j+1][1], A[q+j+1][2]});

    left.push_back({101, 0, 101});
    right.push_back({101, 0, 101});
    int i = 0, j = 0;
    for(int k = p; k <= r; k++){
        if(left[i][2] < right[j][2]){
            A[k][0] = left[i][0];
            A[k][1] = left[i][1];
            A[k][2] = left[i][2];
            i++;
        }
        else if(left[i][2] == right[j][2]){
            if(left[i][0] < right[j][0]){
                A[k][0] = left[i][0];
                A[k][1] = left[i][1];
                A[k][2] = left[i][2];
            i++;
            }
            else{
                A[k][0] = right[j][0];
                A[k][1] = right[j][1];
                A[k][2] = right[j][2];
            j++;
            }
        }
        else{
            A[k][0] = right[j][0];
            A[k][1] = right[j][1];
            A[k][2] = right[j][2];
            j++;
        }
    }
}

void mergeSort(vector<vector<int>> &A, int p, int r){
    if(p < r){
        int q = (p + r) / 2;
        mergeSort(A, p, q);
        mergeSort(A, q+1, r);
        merge(A, p, q, r);
    }
}

int main() {
    int n, k, total = 0;
    vector<vector<int>> books;
    cin >> n >> k;

    for(int i = 0; i < n; i++){
        int c1, c2, diff;
        cin >> c1 >> c2;
        diff = c1 - c2;
        books.push_back({c1, c2, diff});
    }

    mergeSort(books, 0, n-1);
    for(int i = 0; i < k; i++)
        total += books[i][0];
    for(int i = k; i < n; i++)
        total += books[i][1];
    cout << total;
    return 0;
}