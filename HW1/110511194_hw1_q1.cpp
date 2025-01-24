#include <iostream>
#include <vector>
#include <string>

using namespace std;

int partition(vector<string> &A, int p, int r);
void quickSort(vector<string> &A, int p, int r);
int Compare(string a, string b);

int partition(vector<string> &A, int p, int r){
    string x = A[r], temp;
    int i = p-1;
    for(int j = p; j < r; j++){
        if(Compare(A[j], x) <= 0){
            i++;
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    }
    temp = A[i+1];
    A[i+1] = A[r];
    A[r] = temp;

    return i+1;
}

void quickSort(vector<string> &A, int p, int r){
    if(p < r){
        int q = partition(A, p, r);
        quickSort(A, p, q-1);
        quickSort(A, q+1, r);
    }
}

int Compare(string a, string b){
    string x = a + b;
    string y = b + a;
    if(x.compare(y) >= 0)
        return 1;
    else
        return -1;
}

int main() {

    int line;
    vector<string> paper_pieces;
    cin >> line;

    for(int i = 0; i < line; i++){
        string temp;
        cin >> temp;
        paper_pieces.push_back(temp);
    }

    quickSort(paper_pieces, 0, paper_pieces.size()-1);

    for(int i = paper_pieces.size()-1; i >= 0; i--){
        cout << paper_pieces[i];
    }

    return 0;
}