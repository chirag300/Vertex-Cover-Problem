//BFS Implementation learnt and referenced by "Intro to algorithms" and "GEEKS FOR GEEKS" web 
//sstream referenced from the sir's code of calc.cpp given in the gitlab
// Declaring all the Important libraries
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <algorithm>
#include <queue>
#include <sstream>

// #include <bits/stdc++.h>

using namespace std;

// Creating a function to create a adjacency list using edge list and Vertex count
vector<vector<int>> createAdjacencyList(vector<vector<int>>& unique_pairs_list, int value) {
vector<vector<int>> adj_list(value + 1);

if(unique_pairs_list.size() != 0){


for (size_t i = 0; i < unique_pairs_list.size(); i++) {
adj_list[unique_pairs_list[i][0]].push_back(unique_pairs_list[i][1]);
adj_list[unique_pairs_list[i][1]].push_back(unique_pairs_list[i][0]);
}
}

return adj_list;
}


// Creating a function that returns the shortest path between two points
pair<vector<int>, int> bfs_shortest_path(vector<vector<int>>& unique_pairs_list, int value, int src, int dest) {

if(src == dest){
return make_pair(vector<int>(), 1000);
}
// calling the adj_listacency list function and saving here
vector<vector<int>> adj_list = createAdjacencyList(unique_pairs_list, value);
//Creating two vectors one for saving wheather the node is node is visited or not and second for finding the parent node
vector <bool> visited_array(value+1);
vector <int> parent_array(value+1);


//Initialising both the vector with specified values
for(int i=0; i<value+1; i++) {
visited_array[i] = false;
parent_array[i] = -1;
}
// Creating a Queue and pushing the source or start_point 
queue<int> tracker;
tracker.push(src);
visited_array[src]=1;

//Looping till queue is empty and all nodes are traversed
while(!tracker.empty()){
int f = tracker.front();
if(f!= 0){
tracker.pop();
}
else{
continue;
}


//Looping till the front is traversed and marked as the true 
for(size_t i = 0; i< adj_list[f].size(); i++){
int c = adj_list[f][i]; 
// checking that this node is already visited before or not
if(visited_array[c] == true){
    continue;
    }
    else{
    //marking as true after visting
    visited_array[c] = true;
    parent_array[c] = f;
    if(parent_array[c]!= 0){
        tracker.push(c);

    }
    else{
        continue;
    }
}
}
}
//If the visited array is false returning the empty vertex and -1    
if (visited_array[dest] == false) {
return make_pair(vector<int>(), -1);
}

//Initailising the final ans vector
vector<int> shortest_path_ans;
int curr = dest;
while(curr != src){

shortest_path_ans.push_back(curr);
if(shortest_path_ans.size() != 0){
curr = parent_array[curr];

}
}

// adding the source in the last
shortest_path_ans.push_back(src);

// returning the result in form of vector and visited array
return make_pair(shortest_path_ans, visited_array[dest]);
}


// Creating global varibales to use for parser 
int value = 0;
int max_number = 0;
int max_point = 0;
int start_point = 0;
int end_point = 0;
vector<vector<int>> unique_pairs_list;


int main() {

string line;
while (!cin.eof()) {
getline(cin, line);

// processing the V command
if (line[0] == 'V' || line[0] == 'v') {
    cout << line << endl;
istringstream substring(line);
vector<string> element_string;

// splitting the two elements using istringstream 
    int num;
    string elements;
    substring >> elements;
    substring >> num;

    value = num;
    unique_pairs_list.clear();
    max_number = 0;
    max_point = 0;
    start_point = 0;
    end_point = 0;
    
    //  else {
    //     cerr << "Error: Value should be greater than 0" << endl;
    // }

}

// Processing the E command 
else if (line[0] == 'E'|| line[0] == 'e') {
    cout << line << endl;
if (unique_pairs_list.size() == 0){
    
    regex pattern_edges(R"(<(\d+),(\d+)>)");
    //regex method referenced and learnt from stackoverflow

        for (sregex_iterator i(line.cbegin(), line.cend(), pattern_edges); i != sregex_iterator(); ++i) {
            smatch c = *i;
            if (c.length()!=0){
            int x = stoi(c[1].str());
            int y = stoi(c[2].str());
            if (x && y){
                unique_pairs_list.push_back({x, y});

            }
            }
            
}
// finding the maximum number from the by traversing the whole vector 
for (auto pair : unique_pairs_list) {
    
    max_number = max(max_number, max(pair[0], pair[1]));
}
// checking the condition if maximum of edge should be smaller or equal than the total vertices
if (max_number <= value) {

    continue;
} 
else {
    
    cerr << "Error: The Edge in the input is greater than the total Vertices" << endl;
    unique_pairs_list.clear();
    max_number = 0;
    
}}

else{
    // cerr << "Edges already given" << endl;
    continue;
}

} 

// Processing the S command
else if (line[0] == 's' || line[0] == 'S') {
istringstream substring(line);
vector<string> parts;
string part;

// traversing the part by part and saving it in the vector parts
while (substring >> part) {
    parts.push_back(part);
}

// acessing the start and end point of the src and dest given
if (parts.size() == 3 && parts[0] == "s") {
    start_point = stoi(parts[1]);
    end_point = stoi(parts[2]);
    
    if(start_point != 0 && end_point != 0){
        max_point = max(start_point,end_point);

        // checking the condition that if maximum of src and dest is smaller or equal than the E given
        if (max_point <=  value) {
            pair<vector<int>, int> result = bfs_shortest_path(unique_pairs_list, value, start_point, end_point);
            
            if (result.second == -1) {
                cerr << "Error: No path exists between the source and destination." << endl;;
            } 
            else if(result.second ==1000){
                cout << start_point << endl;
            }
            else {
                //Printing out the results in reversed format to match the desired output
                for (int i = result.first.size()-1; i >= 0; --i) {
                    cout << result.first[i];
                    if (i > 0){
                        cout << "-";
                    }
                }
                cout << endl;

            }
        }
        else {
            cerr << "Error: The input in S is greater than Edges given" << endl;
        }}
        else{
            cerr << "Error: S can't be 0" << endl;
        }
    } 
    else {
        cerr << "Error: Invalid input format for start and end points" << endl;
    }
}
}

return 0;
}