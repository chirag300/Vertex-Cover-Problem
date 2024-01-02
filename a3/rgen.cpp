// Parsing using getopt() referenced from the sirs code in gitlab
// Referenced the overlapping check function from stack overflow
// Declaring all important Libraries
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;



bool checker_sub_1(int x_1, int y_1, int x_2, int y_2, int x1, int y1, int x2, int y2) {
  bool condition_1 = (min(x1, x2) <= max(x_1, x_2) && max(x1, x2) >= min(x_1, x_2));
  bool condition_2 = (min(y1, y2) <= max(y_1, y_2) && max(y1, y2) >= min(y_1, y_2));
    if (condition_1 && condition_2) {
        return true;
    }
    else {
        return false;
    }
  
}

bool checker_sub_2(int x_1, int y_1, int x_2, int y_2, int x1, int y1, int x2, int y2) {
  bool condition_3 = (max(x_1, x_2) == min(x1, x2) && max(y_1, y_2) == min(y1, y2));
  bool condition_4 = (min(x_1, x_2) == max(x1, x2) && min(y_1, y_2) == max(y1, y2));
    if ( condition_3 || condition_4) {
        return true;
    }
    else {
        return false;
    }
}



             

//checker Function that checks the overlap between two  line segments
bool over_lap_check_sub(int x_1, int y_1, int x_2, int y_2, int x1, int y1,
             int x2, int y2) {
  if (abs((x_1 - x1) * (y2 - y1) - (y_1 - y1) * (x2 - x1)) < 1e-8 &&
      abs((x_2 - x1) * (y2 - y1) - (y_2 - y1) * (x2 - x1)) < 1e-8) {
    if (checker_sub_1(x_1, y_1, x_2, y_2, x1, y1, x2, y2)) {
      if (checker_sub_2(x_1, y_1, x_2, y_2, x1, y1, x2, y2)) {
        return false;
      } else {
        return true;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
}
//The function that extracts segment points from streets
bool overlapping_check_func(vector<pair<int, int>> line_coordinates,
                            vector<pair<int, int>> street) {
  // Looping in the street which is new
  for (size_t i = 0; i < line_coordinates.size() - 1; i++) {
    int x1 = line_coordinates[i].first;
    int y1 = line_coordinates[i].second;
    int x2 = line_coordinates[i + 1].first;
    int y2 = line_coordinates[i + 1].second;

    // Looping in the street which is already in DB
    for (size_t j = 0; j < street.size() - 1; j++) {
      int x_1 = street[j].first;
      int y_1 = street[j].second;
      int x_2 = street[j + 1].first;
      int y_2 = street[j + 1].second;

      // checks if the extracted point are overlapping or not
      if (over_lap_check_sub(x_1, y_1, x_2, y_2, x1, y1, x2, y2)) {
        return true;  
      }
    }
  }
  return false;  
}
// checks if the points we are adding are valid or not 
bool points_check_func(vector<pair<int, int>> line_coor, int x, int y) {
  if (line_coor.size() == 0) {
    return true;
  } else {
    for (size_t i = 0; i < line_coor.size(); i ++) {
      int x1 = line_coor[i].first;
      int y1 = line_coor[i].second;
      if (x1 == x && y1 == y) {
        return false;
      }
    }
    return true;
  }
}
// This function return random integers using urandom by giving a range
int random_gen(int a, int b) {
  std::ifstream urandom("/dev/urandom");
  if (urandom.fail()) {
    cerr << "Error: cannot open the Urandom file";
    return -1;
  }
  unsigned int num_read;
  urandom.read((char *)&num_read, sizeof(num_read));
  if (num_read) {
    if (a == b) {
      return a;
    } else {
      int gen_num = (num_read % (b - a + 1));
      gen_num = gen_num + a;
      if (gen_num <= b && gen_num >= a) {
        return gen_num;
      }
    }
  }
    urandom.close();
    return 0;
  }

// The main function
int main(int argc, char **argv) {
  int option;
  int streets = 10;
  int line_segments = 5;
  int wait_time = 5;
  int coor = 20;
  int curr_streets;
  int curr_time;
  // int curr_line;
  string t_streets;
  string t_wait;
  int coor_range;
  string t_lines;
  string t_coor;

  // While condition parses the command the line arguments (Referenced from the sirs code of using getopt)
  while ((option = getopt(argc, argv, "s:n:l:c:")) != -1) {
    switch (option) {
      // No of Streets
      case 's':
        t_streets = optarg;
        streets = atoi(t_streets.c_str());
        if (streets >= 2) {
          continue;
        } else {
          cerr << "Error: Streets should be greater than 2" << endl;
          exit(1);
          return 1;
        }
        break;

      // No of seconds to wait
      case 'l':
        t_wait = optarg;
        wait_time = atoi(t_wait.c_str());
        if (wait_time >= 5) {
          continue;
        } else {
          cerr << "Error: time should be greater than 5" << endl;
          exit(1);
          return 1;
        }
        break;

      // No of line segments
      case 'n':
        t_lines = optarg;
        line_segments = atoi(t_lines.c_str());
        if (line_segments < 1) {
          cerr << "Error: line segments should be greater than 1" << endl;
          exit(1);
          return 1;
        }
        break;

      // No. that denotes the range of coordinates 
      case 'c':
        t_coor = optarg;
        coor = atoi(t_coor.c_str());
        if (coor < 1) {
          cerr << "Error: coor range should be greater than 1" << endl;
          exit(1);
          return 1;
        }
        break;

      // Handling other conditions
      case '?':
        if (optopt == 'c' || optopt == 's' || optopt == 'n' || optopt == 'l') {
          if (optarg) {
            continue;
          } else {
            cerr << "Error: No Arguments given along with the command" << endl;
            exit(1);
            return 1;
          }
        } else {
          cerr << "Error: Unknown Command Entered" << endl;
          exit(1);
          return 1;
        }
        break;

      default:
        return 0;
        break;
    }
  }

  // Will run for unlimited counts 
  while (true) {
    // Generating random no of streets
    curr_streets = random_gen(2, streets);
    // Generating random no of seconds to wait
    curr_time = random_gen(5, wait_time);
    // Generating random coor range
    coor_range = coor;


    vector<int> a;

    // Generating random no of line segments for each street
    for (int i = 0; i < curr_streets; i++) {
      int curr_line = random_gen(1, line_segments);
      a.push_back(curr_line);
    }
    
    
    // Data dtructure that stores the total no of streets
    vector<vector<pair<int, int>>> coordinates;
    int counter = 0;

    for (int i = 0; i < curr_streets; i++) {
      vector<pair<int, int>> line_coordinates;

      int num_coordinates = a[i];
      for (int z = 0; z < num_coordinates + 1; z++) {
        int x = random_gen(-coor_range, coor_range);
        int y = random_gen(-coor_range, coor_range);


        // checking the first condition(points are valid or not)
        if (points_check_func(line_coordinates, x, y)) {
          line_coordinates.push_back(make_pair(x, y));

        } else {
          counter++;
          
          if (counter == 25) {
            // max attempts made
            cerr << "Error: Max 25 Attempts Made. Program Exited" << endl;
            exit(1);
            return 1;
          }
          z--;
        }
      }
      if (coordinates.empty()) {
        std::sort(line_coordinates.begin(), line_coordinates.end());

        coordinates.push_back(line_coordinates);
      }

      else {
        bool overlap = false;
         std::sort(line_coordinates.begin(), line_coordinates.end());

        for (auto street : coordinates) {
          // Checking the second condition that is overlapping 
          if (overlapping_check_func(line_coordinates, street)) {
            overlap = true;
            counter++;
            if (counter == 25) {
              // max attempst made 
              cerr << "Error: Max 25 Attempts Made. Program Exited" << endl;
              exit(0);
              return 1;
            }
          }
        }
        if (overlap) {
          i--;
        } else {
          coordinates.push_back(line_coordinates);
        }
      }
    }
    // printing to match the input format for a1
    for (size_t k = 0; k < coordinates.size(); k++) {
      // first adding the add command
    cout << "add \"hfhfjf " << (k + 1) << "\"";
      for (const auto &coord : coordinates[k]) {
        cout << " (" << coord.first << "," << coord.second << ")";
      }
      cout << endl;
    }
    // gg command to print the graph
    cout << "gg" << endl;
    
    // after gg wait for specified time
    sleep(curr_time);

    // rm commmands to remove the streets added
    for(size_t i = 0; i< coordinates.size(); i++){
        cout << "rm \"hfhfjf " << (i+1)<< "\"" << endl;
    }
    // clearing the DB after iteration
    coordinates.clear();
  }

  return 0;
}
