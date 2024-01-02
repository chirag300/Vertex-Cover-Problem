// Declaring important libraries
// Minisat referenced from Sirs Code in GItlab and (https://dwheeler.com/essays/minisat-user-guide.html)
// Sort function used as per the assignment
// Learnt The Binary Search: Geeks For Geeks
#include <algorithm>
#include <iostream>
#include <memory>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"

// #include <bits/stdc++.h>

using namespace std;
using namespace Minisat;

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


// Function to create the matrix for traversing
vector<vector<Lit>> create_Vector_X(int num, int mid, Solver& solver) {
    vector<vector<Lit>> x(num);

    for (int i = 0; i < num; i++) for (int j = 0; j < mid; j++) x[i].push_back(Lit(mkLit(solver.newVar())));

    return x;
}

// Main vertex cover function to calculate the vertex cover
vector<int> vertex_cover_func(int num, vector<vector<int>> edges) {

  // Creating the vector to store the answer  
  vector<int> out;
  int mid;
  int l = 0;
  int h = num;
 // Implementing Binary Search to find the minimum vertex cover 


  while (l <= h) {
    int t = l + h;
    mid = t / 2;

    Solver solver;
    // Calling the vector creator function
    vector<vector<Lit>> x = create_Vector_X(num, mid, solver);

    // First Clause
    for (int i = 0; i < mid; i++) {vec<Lit> yy; for (const auto& column : x) yy.push(column[i]); solver.addClause(yy); yy.clear();}
    // for (int i = 0; i < mid; i++) { vec<Lit> yy; for (int j = 0; j < num; j++) yy.push(x[j][i]); solver.addClause(yy); yy.clear(); }

    // Second Clause
   for (int i = 0; i < num; ++i){
    for (int j = 0; j < mid - 1; ++j){
        for (int t = j + 1; t < mid; ++t) {
            Lit a1 = x[i][j];
            Lit a2 = x[i][t];
            solver.addClause(~a1, ~a2);
        }
    }
   }

        
    // Third Clause
    for (int i = 0; i < mid; i++){ 
    for (int j = 0; j < num - 1; j++) {
    for (int t = j+1; t < num; t++) {
    Lit a1 = x[j][i];
    Lit a2 = x[t][i];
    solver.addClause(~a1, ~a2);
    }
    }
    }

    // Fourth Clause
    for (auto edge : edges) { int x1 = edge[0], x2 = edge[1]; vec<Lit> yo; for (int i = 0; i < mid; i++) yo.push(x[x1][i]), yo.push(x[x2][i]); solver.addClause(yo); yo.clear(); }



    // Checking if after the condition check we get the valid output or not
    if (solver.solve()) {
      out.clear();
      h = mid - 1;
      for (int i = 0; i < num; i++) {
         for (int j = 0; j < mid; j++) {
            if (solver.modelValue(x[i][j]) == l_False) {
              continue;
            }
            else{
                out.push_back(i);
            }
        }
    }
}
    
    else {
      l = mid + 1;
    }
  }

  // Sorting if we get the result
  std::sort(out.begin(), out.end());

  return out;
}

// Creating global varibales to use for parser
int value = 0;
int max_number = 0;
int max_point = 0;
vector<vector<int>> unique_pairs_list;

int main() {
  string line;
  while (!cin.eof()) {
    getline(cin, line);

    // processing the V command
    if (line[0] == 'V' || line[0] == 'v') {
      istringstream substring(line);
      vector<string> element_string;

      // splitting the two elements using istringstream
      int num;
      string elements;
      substring >> elements;
      substring >> num;

      if (num > 0) {
        value = num;
        unique_pairs_list.clear();
        max_number = 0;
        max_point = 0;
      } else {
        cerr << "Error: Value should be greater than 0" << endl;
      }

    }

    // Processing the E command
    else if (line[0] == 'E' || line[0] == 'e') {
      // cout << line << endl;
      if (unique_pairs_list.size() == 0) {
        regex pattern_edges(R"(<(\d+),(\d+)>)");
        // regex method referenced and learnt from stackoverflow

        for (sregex_iterator i(line.cbegin(), line.cend(), pattern_edges);
             i != sregex_iterator(); ++i) {
          smatch c = *i;
          if (c.length() != 0) {
            int x = stoi(c[1].str());
            int y = stoi(c[2].str());
            if (x && y) {
              unique_pairs_list.push_back({x, y});
            }
          }
        }
        // finding the maximum number from the by traversing the whole vector
        for (auto pair : unique_pairs_list) {
          max_number = max(max_number, max(pair[0], pair[1]));
        }
        // checking the condition if maximum of edge should be smaller or equal
        // than the total vertices
        if (max_number <= value) {
          // storing the result  
          vector<int> out = vertex_cover_func(value + 1, unique_pairs_list);
          // printing the result 
          for (int num : out) {

            cout << num << " ";
          }

          cout << endl;
        }
         else {

          cerr << "Error: The Edge in the input is greater than the total Vertices"<< endl;

          unique_pairs_list.clear();
          max_number = 0;
        }
      }
    }
  }

  return 0;
}