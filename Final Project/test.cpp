#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

class Chain;
class Node
{
    friend class Chain;
public:
    Node(int e = 0, int d = 0, Node* next = 0);
private:
    int vertex;
    int duration;
    Node *link;
};

class Chain
{
public:
    Chain();
    void Insert(int,int);
    Node* getFirst();
    Node* getLast();
    void setIter(Node*);
    void moveIter();
    Node* getIter();
    int getIterVal();
    int getIterDur();
private:
    Node *first;
    Node *last;
    Node *iter;
};
Node::Node(int e, int d, Node* next)
{
    vertex = e;
    duration = d;
    link = next;
}
Chain::Chain()
{
    first = 0;
}
void Chain::Insert(int e,int d)
{
    if(first)
    {
        last -> link = new Node(e,d);
        last = last -> link;
    }
    else
        first = last = new Node(e,d);
}
Node* Chain::getFirst()
{
    return first;
}
Node* Chain::getLast()
{
    return last->link;
}
void Chain::setIter(Node* i)
{
    iter = i;
}
void Chain::moveIter()
{
    iter = iter ->link;
}
Node* Chain::getIter()
{
    return iter;
}
int Chain::getIterVal()
{
    int s = iter -> vertex;
    return s;
}
int Chain::getIterDur()
{
    int d = iter ->duration;
    return d;
}

char cri(int,int);
int slack(int,int);
void graph_path(vector<int> &temp,int path[][2],int k,int p,int i,int g,int o, ostream& oup);


int main(int argc, char *argv[])
{
    int i = 0, g = 0, o = 0;

    ifstream inp;
    ofstream oup;
    inp.open(argc[1]);

    while(!inp.eof()) //find the number of I,G,O
    {
        char tempi,tempo;
        int vali,valo;
        string s;
        inp >> tempi >> vali >> tempo >> valo;
        getline(inp,s);

        if(tempi == 'I')
        {
            if(vali > i)
                i = vali;
        }
        else if(tempi == 'G')
        {
            if(vali > g)
                g = vali;
        }

        if(tempo == 'G')
        {
            if(valo > g)
                g = valo;
        }
        else if(tempo == 'O')
        {
            if(valo > o)
                o = valo;
        }
    }
    Chain *G = new Chain [i + g + o];
    Chain *inv = new Chain [i + g + o];

    inp.close();
    inp.open(argc[1]);

    while(!inp.eof()) // insert the data into the graph
    {
        char a,b;
        int c,d,e;
        string s;
        inp >> a >> c >> b >> d >> e;
        getline(inp,s);
        if(a == 'I')
        {
            if(b == 'G')
            {
                G[c-1].Insert(d+i-1,e);
                inv[d+i-1].Insert(c-1,e);
            }
            else if(b == 'O')
            {
                G[c-1].Insert(d+i+g-1,e);
                inv[d+i+g-1].Insert(c-1,e);
            }

        }
        else if(a == 'G')
        {
            if(b == 'G')
            {
                G[c+i-1].Insert(d+i-1,e);
                inv[d+i-1].Insert(c+i-1,e);
            }
            else if(b == 'O')
            {
                G[c+i-1].Insert(d+i+g-1,e);
                inv[d+i+g-1].Insert(c+i-1,e);
            }
        }
    }

    int *ee = new int [i+g+o]; // earliest event
    int *le = new int [i+g+o]; // latest event

    for(int j = 0;j < i+g+o; j++)
        ee[j] = 0;

    for(int j = 0;j < i+g+o; j++) // find the earliest event
    {
        if(j < i)
            continue; // skip the input
        else
        {
            inv[j].setIter(inv[j].getFirst());
            while(inv[j].getIter() != 0)
            {
                if(ee[inv[j].getIterVal()] + inv[j].getIterDur() > ee[j])
                    ee[j] = ee[inv[j].getIterVal()] + inv[j].getIterDur();
                inv[j].moveIter();
            }
        }
    }

    int ml = 0; // max latency
    for(int j = 0;j < i+g+o; j++)
        if(ee[j] > ml)
            ml = ee[j];

    for(int j = 0;j < i+g+o; j++) 
        le[j] = ml;

    for(int j = i+g+o-1; j >=0; j--) // find the latest event
    {
        if(j >= i+g)
            le[j] = ml;
        else
        {
            G[j].setIter(G[j].getFirst());
            while(G[j].getIter() != 0)
            {
                if(le[G[j].getIterVal()]-G[j].getIterDur() < le[j])
                    le[j] = le[G[j].getIterVal()]-G[j].getIterDur();
                G[j].moveIter();
            }
        }
    }

    oup.open(argv[2]);

    int path[50][2];
    for(int j = 0;j < 50;j++)
        for(int k = 0;k < 2;k++)
            path[j][k] = 0;
    bool tra[50] = {false};
    int p = -1;

    for(int j = i;j < i+g; j++) // find the critical path
    {
        inv[j].setIter(inv[j].getFirst());
        while(inv[j].getIter() != 0)
        {
            if(inv[j].getIterVal() < i)
            {
                cout << "I" << inv[j].getIterVal()+1 << " G" << j-i+1 << " " << ee[inv[j].getIterVal()] << " " << le[j]-inv[j].getIterDur() << " " << slack(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) << " " << cri(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) << endl;
                if(cri(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) == 'Y')
                {
                    p++;
                    path[p][0] = inv[j].getIterVal();
                    path[p][1] = j;
                }
            }
            else
            {
                cout << "G" << inv[j].getIterVal()-i+1 << " G" << j-i+1 << " " << ee[inv[j].getIterVal()] << " " << le[j]-inv[j].getIterDur() << " " << slack(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) << " " << cri(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) << endl;
                if(cri(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) == 'Y')
                {
                    p++;
                    path[p][0] = inv[j].getIterVal();
                    path[p][1] = j;
                }
            }
            inv[j].moveIter();
        }
    }

    for(int j = i+g; j < i+g+o; j++)
    {
        inv[j].setIter(inv[j].getFirst());
        while(inv[j].getIter() != 0)
        {
            if(inv[j].getIterVal() < i)
            {
                oup << "I" << inv[j].getIterVal()+1 << " O" << j-i-g+1 << " " << ee[inv[j].getIterVal()] << " " << le[j]-inv[j].getIterDur() << " " << slack(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) << " " << cri(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) << endl;
                if(cri(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) == 'Y')
                {
                    p++;
                    path[p][0] = inv[j].getIterVal();
                    path[p][1] = j;
                }
            }
            else
            {
                oup << "G" << inv[j].getIterVal()-i+1 << " O" << j-i-g+1 << " " << ee[inv[j].getIterVal()] << " " << le[j]-inv[j].getIterDur() << " " << slack(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) << " " << cri(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) << endl;
                if(cri(ee[inv[j].getIterVal()], le[j]-inv[j].getIterDur()) == 'Y')
                {
                    p++;
                    path[p][0] = inv[j].getIterVal();
                    path[p][1] = j;
                }
            }
            inv[j].moveIter();
        }
    }

    oup << "max latency: " << ml << endl;
    oup << "critical paths:" << endl;

    vector<int> temp;

    for(int j = 0;j < i;j++)
    {
        for(int k = 0;k <= p;k++)
        {
            if(path[k][0] == j)
            {
                temp.push_back(path[k][0]);
                graph_path(temp,path,k,p,i,g,o,oup);
            }
        }
    }

    return 0;
}
char cri(int a,int b)
{
    if(a == b)
        return 'Y';
    else
        return 'N';
}
int slack(int a,int b)
{
    return b-a;
}
void graph_path(vector<int> &temp,int path[][2],int k,int p,int i,int g,int o, ostream& oup)
{
    int m = path[k][1];
    for(int j = k;j <= p;j++)
    {
        if(path[j][0] == m && m < i+g)
        {
            if(temp.back() != path[j][0])
                temp.push_back(path[j][0]);
            graph_path(temp,path,j,p,i,g,o);
        }
        else if(m >= i+g)
        {
            temp.push_back(m);
            for(int r = 0;r < temp.size(); r++)
            {
                if(temp.at(r) < i)
                    oup << "I" << temp.at(r)+1 << " ";
                else if(temp.at(r) >= i+g)
                    oup << "O" << temp.at(r)-i-g+1 << endl;
                else
                    oup << "G" << temp.at(r)-i+1 << " ";
            }
            temp.pop_back();
            return;
        }
    }
    temp.pop_back();
    return;

}
