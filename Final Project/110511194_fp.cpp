#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <algorithm>

using namespace std;

const int MIN_VALUE = -2147483648;

unordered_map<string, int> gate_and_its_delay = {
    {"NAND", 2}, {"AND", 3}, {"NOR", 2}, {"OR", 3},
    {"XOR", 4}, {"XNOR", 4}, {"INV", 1}, {"input", 0},
    {"USD01", 5}, {"USD02", 2}, {"USD03", 3}, {"USD04", 4},
    {"USD05", 5}, {"USD06", 2}, {"USD07", 3}, {"USD08", 5},
    {"USD09", 2}, {"USD10", 4}
};

void DFS(vector<vector<pair<int, string>>> &graph, int v, vector<bool> &visited, stack<int> &s){
    visited[v] = true;

    if(v >= graph.size()){
        s.push(v);
        return;
    }

    for(int i = 0; i < graph[v].size(); i++){
        int w = graph[v][i].first;
        if(!visited[w])
            DFS(graph, w, visited, s);
    }

    s.push(v);
}

vector<int> topological_sort(vector<vector<pair<int, string>>> &graph){
    vector<bool> visited(graph.size(), false);
    stack<int> s;

    for(int i = 0; i < graph.size(); i++){
        if(!visited[i])
            DFS(graph, i, visited, s);
    }

    vector<int> topo_order;
    topo_order.push_back(graph.size());
    while(!s.empty()){
        topo_order.push_back(s.top());
        s.pop();
    }

    return topo_order;
}

int find_max_delay(vector<vector<pair<int, string>>> &graph, vector<int> &input, unordered_map<int, int> numToIndexMap, vector<int> &topo_order){
    topo_order = topological_sort(graph);
    
    vector<pair<int, string>> virtual_input;
    for(int i = 0; i < input.size(); i++)
        virtual_input.push_back(make_pair(numToIndexMap[input[i]], "input"));
    graph.push_back(virtual_input);

    vector<int> delay(graph.size(), MIN_VALUE);
    delay[topo_order[0]] = 0;

    for(int i = 0; i < topo_order.size(); i++){
        for(int j = 0; j < graph[topo_order[i]].size(); j++){
            int v = graph[topo_order[i]][j].first; // adjacent vertex
            int w = gate_and_its_delay[graph[topo_order[i]][j].second]; // edge weight
            if(delay[v] < delay[topo_order[i]] + w)
                delay[v] = delay[topo_order[i]] + w;
        }        
    }

    graph.pop_back();
    return *max_element(delay.begin(), delay.end());
}

bool gate_output(bool input1, bool input2, string gate_type){
    if(gate_type == "NAND")
        return !(input1 && input2);
    else if (gate_type == "AND")
        return input1 && input2;
    else if(gate_type == "NOR")
        return !(input1 || input2);
    else if(gate_type == "OR")
        return input1 || input2;
    else if(gate_type == "XOR")
        return input1 ^ input2;
    else if(gate_type == "XNOR")
        return !(input1 ^ input2);
    else if(gate_type == "USD01")
        return false;
    else if(gate_type == "USD02")
        return input1 && !(input2);
    else if(gate_type == "USD03")
        return input1;
    else if(gate_type == "USD04")
        return !(input1) && input2;
    else if(gate_type == "USD05")
        return input2;
    else if(gate_type == "USD06")
        return !input2;
    else if(gate_type == "USD07")
        return input1 || !(input2);
    else if(gate_type == "USD08")
        return !input1;
    else if(gate_type == "USD09")
        return !(input1) || input2;
    else if(gate_type == "USD10")
        return true;
    else
        return false;
}

int main()
{
    vector<int> input, output, wire;
    string temp;

    while(temp.back() != ';') // read the first line
        cin >> temp;
    
    for(int i = 0; i < 3; i++){ // read input, output, wire, and check the number of them
        cin >> temp;
        if(temp == "input"){
            cin >> temp;
            while(temp.back() != ';'){
                input.push_back(stoi(temp.substr(1, temp.length() - 2)));
                cin >> temp;
            }  
            
            input.push_back(stoi(temp.substr(1, temp.length() - 2)));
        }
        else if(temp == "output"){
            cin >> temp;
            while(temp.back() != ';'){
                output.push_back(stoi(temp.substr(1, temp.length() - 2)));
                cin >> temp;
            }
            
            output.push_back(stoi(temp.substr(1, temp.length() - 2)));
        }
        else if(temp == "wire"){
            cin >> temp;
            while(temp.back() != ';'){
                wire.push_back(stoi(temp.substr(1, temp.length() - 2)));
                cin >> temp;
            }
            
            wire.push_back(stoi(temp.substr(1, temp.length() - 2)));
        }
    }

    unordered_map<int, int> numToIndexMap; // construct the map
    for(int i = 0; i < input.size(); ++i) 
        numToIndexMap[input[i]] = i;
    for(int i = 0; i < wire.size(); ++i)
        numToIndexMap[wire[i]] = i + input.size();
    for(int i = 0; i < output.size(); ++i)
        numToIndexMap[output[i]] = i + input.size() + wire.size();
    
    vector<vector<pair<int, string>>> graph(input.size() + wire.size()); // construct the graph
    vector<vector<pair<int, string>>> graph_transpose(input.size() + output.size() + wire.size()); // construct the transpose graph

    cin >> temp;
    while(temp != "endmodule"){ // read gates and construct the graph
        if(temp == "INV"){ // temp is the gate type
            string gate_name, input_name, output_name;
            cin >> gate_name >> input_name >> output_name;
            input_name = input_name.substr(5, input_name.length() - 3);
            output_name = output_name.substr(5, output_name.length() - 4);

            auto it = numToIndexMap.find(stoi(input_name));
            if(it != numToIndexMap.end()){
                graph[it->second].push_back(make_pair(numToIndexMap[stoi(output_name)], temp));
                graph_transpose[numToIndexMap[stoi(output_name)]].push_back(make_pair(it->second, temp));
            }
        }
        else{
            string gate_name, input_name1, input_name2, output_name;
            cin >> gate_name >> input_name1 >> input_name2 >> output_name;
            input_name1 = input_name1.substr(6, input_name1.length() - 3);
            input_name2 = input_name2.substr(5, input_name2.length() - 3);
            output_name = output_name.substr(5, output_name.length() - 4);

            auto it1 = numToIndexMap.find(stoi(input_name1));
            auto it2 = numToIndexMap.find(stoi(input_name2));
            if(it1 != numToIndexMap.end() && it2 != numToIndexMap.end()){
                graph[it1->second].push_back(make_pair(numToIndexMap[stoi(output_name)], temp));
                graph_transpose[numToIndexMap[stoi(output_name)]].push_back(make_pair(it1->second, temp));
                graph[it2->second].push_back(make_pair(numToIndexMap[stoi(output_name)], temp));
                graph_transpose[numToIndexMap[stoi(output_name)]].push_back(make_pair(it2->second, temp));
            }

        }

        cin >> temp;
    }

    vector<int> topo_order; // topo_order[0] is the virtual input
    cout << find_max_delay(graph, input, numToIndexMap, topo_order) << endl;
    topo_order[0] = -1;

//----------------------------------------------------------------------------------------//

    cin >> temp;
    cin >> temp;
    vector<int> input_values(input.size());
    for(int i = 0; i < input.size() - 1; i++){
        input_values[i] = numToIndexMap[stoi(temp.substr(1, temp.length() - 2))];
        cin >> temp;
    }
    input_values[input.size() - 1] = numToIndexMap[stoi(temp.substr(1, temp.length() - 1))];

    cin >> temp;
    vector<bool> output_values(topo_order.size() - 1, false);
    while(temp != ".end"){
        for(int i = 0; i < input_values.size(); i++){ // fill the input_values into output_values
            vector<int>::iterator iter = find(topo_order.begin(), topo_order.end(), input_values[i]);
            if(temp == "0")
                output_values[iter - topo_order.begin() - 1] = false;
            else if(temp == "1")
                output_values[iter - topo_order.begin() - 1] = true;
            cin >> temp;
        }

        for(int i = 0; i < output_values.size(); i++){ // construct the output_values
            if(topo_order[i + 1] < input.size())
                continue;
            else{
                if(graph_transpose[topo_order[i + 1]].size() == 1){
                    vector<int>::iterator iter = find(topo_order.begin(), topo_order.end(), graph_transpose[topo_order[i + 1]][0].first);
                    output_values[i] = !(output_values[iter - topo_order.begin() - 1]);
                }
                else{
                    vector<int>::iterator iter1 = find(topo_order.begin(), topo_order.end(), graph_transpose[topo_order[i + 1]][0].first);
                    vector<int>::iterator iter2 = find(topo_order.begin(), topo_order.end(), graph_transpose[topo_order[i + 1]][1].first);
                    output_values[i] = gate_output(output_values[iter1 - topo_order.begin() - 1], output_values[iter2 - topo_order.begin() - 1], graph_transpose[topo_order[i + 1]][0].second);
                }
            }
        }

        for(int i = input.size() + wire.size(); i < topo_order.size() - 1; i++){ // print the output
            vector<int>::iterator iter = find(topo_order.begin(), topo_order.end(), i);
            cout << output_values[iter - topo_order.begin() - 1] << " ";
        }
        cout << endl;

        for(int i = 0; i < output_values.size(); i++) // reset the output_values
            output_values[i] = false;

    }

    return 0;
}