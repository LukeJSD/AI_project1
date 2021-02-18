#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <queue>
#include <stack>

#include "node.h"   // used for method 1
#include "node2.h"  // used for method 2; this is the on actually used

using namespace std;

// Global variables; more useful in the unused method1
int DIAMETER = 0;
int VERTICES = 0;

// METHOD 1
// these were used for my first solution, but I think they violated some of the requirements of the project
map<string, node> make_connections1(string);
pair<int, vector<node*>> UCS1(node*, const string&, map<string, node>&);
int output_answers1(pair<int, vector<node*>>);

// METHOD 2
// these are for my implemented solution; I think it satisfies the requirements
vector<string> make_cities(string);
vector<vector<int>> make_connections2(string, map<string, int>);
int UCS2(int, int, vector<vector<int>>, vector<string>);

int main(int argc, char* argv[]) {
    int method = 2;

    string start;
    string end;
    string input_file;
    if (argc == 4) {
        input_file = argv[1];
        start = argv[2];
        end = argv[3];
    } else {    // if there aren't enough args, then the program will run will run for manual testing
        input_file = "input1.txt";
        start = "Bremen";
        end = "Frankfurt";
    }

    if (method == 1) {
        map<string, node> node_map = make_connections1(input_file);

        pair<int, vector<node *>> answer;
        if (start == end) {
            answer = pair<int, vector<node *>>(0, vector<node *>{&node_map[start]});
        } else {
            answer = UCS1(&node_map[start], end, node_map);
        }
        return output_answers1(answer);
    } else if (method == 2) {
        vector<string> cities = make_cities(input_file);

        // create a map to convert from a city's name as a string to its int value
        map<string, int> city_str_2_num;
        for (int i = 0; i < cities.size(); ++i) {
            city_str_2_num[cities[i]] = i;
        }

        return UCS2(city_str_2_num[start], city_str_2_num[end], make_connections2(input_file, city_str_2_num), cities);
    }
    return 0;
}

// METHOD 1
map<string, node> make_connections1(string file) {
    ifstream input(file);

    map<string, node> ret_map;
    set<string> initialized_cities;

    while (!input.eof()) {
        string line;
        getline(input, line);
        if (line == "END OF INPUT") break;

        stringstream ss(line);
        string city1, city2; int dist;
        ss >> city1 >> city2 >> dist;

        node* nd_ptr1;
        node* nd_ptr2;
        node nd1;
        node nd2;
        if (initialized_cities.count(city1) < 1) {
            nd1 = node(city1);
            nd_ptr1 = &nd1;
            ret_map[city1] = *nd_ptr1;
            initialized_cities.insert(city1);
        } else {
            nd_ptr1 = &ret_map[city1];
        }
        if (initialized_cities.count(city2) < 1) {
            nd2 = node(city2);
            nd_ptr2 = &nd2;
            ret_map[city2] = *nd_ptr2;
            initialized_cities.insert(city2);
        } else {
            nd_ptr2 = &ret_map[city2];
        }

        ret_map[city1].add_neighbor(city2, nd_ptr2, dist);
        ret_map[city2].add_neighbor(city1, nd_ptr1, dist);

        DIAMETER += dist;
    }
    VERTICES = initialized_cities.size();

    input.close();
    return ret_map;
}

pair<int, vector<node*>> UCS1(node* start, const string& destination, map<string, node> &cities) {
    priority_queue<pair<int, vector<node*>>, vector<pair<int, vector<node*>>>, greater<>> line;
    vector<node*> init_vec = {start};
    line.push(pair<int, vector<node*>>(0, init_vec));

    while (!line.empty()) {
        pair<int, vector<node*>> current = line.top();
        line.pop();

        if (current.first > DIAMETER || current.second.size() > VERTICES) {
            return pair<int, vector<node*>>(DIAMETER+1, vector<node*>());
        }

        node* last_city = current.second[current.second.size()-1];

        // cout << current.first << " " << last_city->get_city() << " - " << DIAMETER << endl;

        if (last_city->get_city() == destination) {
            return current;
        }

        for (string city: last_city->get_neighbors()) {
            vector<node*> updated(current.second);
            updated.push_back(&cities[city]);
            line.push(pair<int, vector<node*>>(current.first+last_city->get_distance(city), updated));
        }
    }

    return pair<int, vector<node*>>(DIAMETER+1, vector<node*>());
}

int output_answers1(pair<int, vector<node*>> answer) {
    if (answer.first > DIAMETER) {
        cout << "distance: infinity\nroute:\nnone";
    } else if (answer.first == 0) {
        cout << "distance: 0 km\nroute:\n" << answer.second[0] << " to " << answer.second[0] << ", 0 km";
    } else {
        cout << "distance: " << answer.first <<  "km\nroute:";
        node* prev_node = answer.second[0];
        for (int i = 1; i < answer.second.size(); ++i) {
            cout << endl << prev_node->get_city() << " to " << answer.second[i]->get_city();
            cout << ", " << prev_node->get_distance(answer.second[i]->get_city()) << " km";
            prev_node = answer.second[i];
        }
    }
    return answer.first;
}


// METHOD 2
/*
 * @parameters: string of the input file's name
 * @returns: 1-D vector of strings where each city is at the index corresponding to its int identification
 */
vector<string> make_cities(string file) {
    ifstream input(file);

    // Use set so cities are not assigned more than once
    set<string> initialized_cities;

    while (!input.eof()) {
        string line;
        getline(input, line);
        if (line == "END OF INPUT") break;  // end of the file, break

        // Each piece of data in the line is seperated by spaces, so convert the string of the line to stringstream
        stringstream ss(line);
        string city1, city2; int dist;
        ss >> city1 >> city2 >> dist;

        initialized_cities.insert(city1);
        initialized_cities.insert(city2);

        DIAMETER += dist;
    }

    // convert the set of cities to the vector, thus assigning each city an int between 0 and NUM_OF_CITIES-1
    vector<string> vec;
    for (string city: initialized_cities) {
        vec.push_back(city);
    }
    return vec;
}

/*
 * NOTE: In the documentation for this function, connection is defined as a one step path between states at a cost.
 * @parameters: string for input file, map the returns a cities int when given its string name
 * @returns:  2-D vector representing the connections between cities.  Both dimensions are of equal size to the number
 * of cities.
 * Example: if city 3 and city 7 are connected at a cost of 100, and our 2-D vector is named "connects,"
 * connects[3][7] = 100 and connects[7][3] = 100. Also, if there is no connection between cities 1 and 2,
 * connects[1][2] = 0 and connects[2][1] = 0.
 */
vector<vector<int>> make_connections2(string file, map<string, int> str_2_num) {
    ifstream input(file);

    // initialize 2-D vector
    vector<vector<int>> connects;
    for (int i = 0; i < str_2_num.size(); ++i) {
        vector<int> adj;
        connects.push_back(adj);
        for (int j = 0; j < str_2_num.size(); ++j) {
            connects[i].push_back(0);
        }
    }

    while (!input.eof()) {
        string line;
        getline(input, line);
        if (line == "END OF INPUT") break;

        // Same process to retrieve data as in make_cities
        stringstream ss(line);
        string city1, city2; int dist;
        ss >> city1 >> city2 >> dist;

        // Get city ints
        int num1 = str_2_num[city1];
        int num2 = str_2_num[city2];

        // Assign connection with cost dist
        connects[num1][num2] = dist;
        connects[num2][num1] = dist;
    }

    return connects;
}

/*
 * @parameters: a node2* of the final node in the route/path, and a stack that should be empty on the intial call, add
 * will be added to on recursive calls.
 * @returns: a stack (the finalized stk parameter) of the nodes where the top of the stack is the starting city's int,
 * going along the path as the stack goes further down.
 * Because the node2* are connected by pointer to each parent, we can only in one direction from that - end to start. To
 * put this in a useful form where we can output the path in the correct order, the order needs to be reversed.  Stacks
 * are FILO/LIFO, so a stack populated from the node2* will be popped in the true order followed by the path.
 */
stack<int> node_2_stack(node2* nd, stack<int> stk = stack<int>()) {
    if (nd == nullptr) {    // The only node2* with a parent of nullptr is the starting node28/starting city
        return stk;
    } else {    // Recursively go backwards in the path
        stk.push(nd->get_n());
        return node_2_stack(nd->get_parent(), stk);
    }
}

/*
 * outputs the path and cost in the desired format
 */
void output_answers2(node2* nd2, vector<vector<int>> connections, vector<string> cities) {
    stack<int> path = node_2_stack(nd2);

    if (nd2 == nullptr) {
        cout << "distance: infinity\nroute:\nnone\n";
    } else if (nd2->get_parent() == nullptr) {
        cout << "distance: 0 km\nroute:\n" << cities[nd2->get_n()] << " to " << cities[nd2->get_n()] << ", 0 km\n";
    } else {
        cout << "distance: " << nd2->get_path_cost() <<  "km\nroute:\n";
        int prev = path.top();
        path.pop();
        while (!path.empty()) {
            cout << cities[prev] << " to " << cities[path.top()];
            cout << ", " << connections[prev][path.top()] << " km\n";
            prev = path.top();
            path.pop();
        }
    }
}

/*
 * @parameters: int of the current city, 2-D vector of the graph, and boolean vector to keep track of visited cities
 * visited vector is initialize outside of function to be of size of the number of cities and to all false values
 * @returns: vector of which cities were able to be visited from s
 * This is used to verify whether or not there is a path from the start city to the destination city.
 */
vector<bool> preprocessing_dfs(int s, vector<vector<int>> c, vector<bool> &v) {
    v[s] = true;    // we have now visited city s
    for (int i = 0; i < c[s].size(); ++i) {
        if (c[s][i] > 0 && !v[i]) { // check that there is a connection from s to i and that i hasn't been visited yet
            preprocessing_dfs(i, c, v);
        }
    }
    return v;
}

/*
 * @parameters: int start - the starting state's int value; int destination - the goal state's int value;
 * vector<vector<int>> connections - the 2-D vector as described and constructed in make_connections();
 * vector<string> cities - the vector of string city names that will be used to convert state int values back to strings
 * for the output
 * @returns: the total cost of the path.  If there is no path, the maximum int16 value is returned
 */
int UCS2(int start, int destination, vector<vector<int>> connections, vector<string> cities) {
    // PRE-PROCESSING: verify a path does exist
    vector<bool> vec(connections.size(), false);
    if (!preprocessing_dfs(start, connections, vec)[destination]) {
        output_answers2(nullptr, connections, cities);
        return INT16_MAX;
    }

    // Create priority queue of node2*. Comparisons are made with node_cmp in node2.h to make sure the node* with the
    // lowest path cost is at the front
    priority_queue<node2*, vector<node2*>, node_cmp> line;
    node2* start_node = new node2(start);
    line.push(start_node);  // Initialize line with a node2* of the starting state

    while (!line.empty()) {
        // get the path with the lowest total cost and pop it from the queue
        node2* current = line.top();
        line.pop();

        // if the path exceeds the DIAMETER than it is not possible for there to be a path from start to destination
        // this block of code if probably useless because of the pre-processing; only leftover for thoroughness
        if (current->get_path_cost() > DIAMETER) {
            output_answers2(nullptr, connections, cities);
            return INT16_MAX;
        }

        // if the current node2* is the goal state, then we have found the optimal path. Output and return the path cost
        if (current->get_n() == destination) {
            output_answers2(current, connections, cities);
            return current->get_path_cost();
        }

        // expand to path from current
        node2* next_node;
        for (int i = 0; i < connections[current->get_n()].size(); ++i) {
            int connection = connections[current->get_n()][i];  // cost from current node2*'s state to state i
            if (connection == 0) continue;  // no connection, continue loop
            // create new node for state i with parent current
            next_node = new node2(i, current->get_path_cost()+connection, current);
            line.push(next_node);   // push new expanded path to priority queue
        }
    }

    // this block of code if probably useless because of the pre-processing; only leftover for thoroughness
    output_answers2(nullptr, connections, cities);
    return INT16_MAX;
}
