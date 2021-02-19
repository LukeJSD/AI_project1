#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <queue>
#include <stack>

#include "node.h"

using namespace std;

vector<string> make_cities(string);
vector<vector<int>> make_connections2(string, map<string, int>);
int UCS2(int, int, vector<vector<int>>, vector<string>);

int main(int argc, char* argv[]) {
    string start;
    string end;
    string input_file;
    if (argc == 4) {
        input_file = argv[1];
        start = argv[2];
        end = argv[3];
    } else {    // exit if there are not 4 args (program call, input file name, start city, end city)
        cerr << "Incorrect number of arguments; 4 args required\n";
        exit(-1);
    }

    vector<string> cities = make_cities(input_file);

    // create a map to convert from a city's name as a string to its int value
    map<string, int> city_str_2_num;
    for (int i = 0; i < cities.size(); ++i) {
        city_str_2_num[cities[i]] = i;
    }

    // make sure the cities entered from the args are actually in the graph
    if (city_str_2_num.count(start) < 1 || city_str_2_num.count(end) < 1) {
        cerr << "One of the input cities is not present in the graph\n";
        exit(-2);
    }

    UCS2(city_str_2_num[start], city_str_2_num[end], make_connections2(input_file, city_str_2_num), cities);

    return 0;
}

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
 * @parameters: a node* of the final node in the route/path, and a stack that should be empty on the intial call, add
 * will be added to on recursive calls.
 * @returns: a stack (the finalized stk parameter) of the nodes where the top of the stack is the starting city's int,
 * going along the path as the stack goes further down.
 * Because the node* are connected by pointer to each parent, we can only in one direction from that - end to start. To
 * put this in a useful form where we can output the path in the correct order, the order needs to be reversed.  Stacks
 * are FILO/LIFO, so a stack populated from the node* will be popped in the true order followed by the path.
 */
stack<int> node_2_stack(node* nd, stack<int> stk = stack<int>()) {
    if (nd == nullptr) {    // The only node* with a parent of nullptr is the starting node28/starting city
        return stk;
    } else {    // Recursively go backwards in the path
        stk.push(nd->get_n());
        return node_2_stack(nd->get_parent(), stk);
    }
}

/*
 * outputs the path and cost in the desired format
 */
void output_answers2(node* nd, vector<vector<int>> connections, vector<string> cities) {
    stack<int> path = node_2_stack(nd);

    if (nd == nullptr) {
        cout << "distance: infinity\nroute:\nnone\n";
    } else if (nd->get_parent() == nullptr) {
        cout << "distance: 0 km\nroute:\n" << cities[nd->get_n()] << " to " << cities[nd->get_n()] << ", 0 km\n";
    } else {
        cout << "distance: " << nd->get_path_cost() << "km\nroute:\n";
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

    // Create priority queue of node*. Comparisons are made with node_cmp in node.h to make sure the node* with the
    // lowest path cost is at the front
    priority_queue<node*, vector<node*>, node_cmp> line;
    node* start_node = new node(start);
    line.push(start_node);  // Initialize line with a node* of the starting state

    while (!line.empty()) {
        // get the path with the lowest total cost and pop it from the queue
        node* current = line.top();
        line.pop();

        // if the current node* is the goal state, then we have found the optimal path. Output and return the path cost
        if (current->get_n() == destination) {
            output_answers2(current, connections, cities);
            return current->get_path_cost();
        }

        // expand to path from current
        node* next_node;
        for (int i = 0; i < connections[current->get_n()].size(); ++i) {
            int connection = connections[current->get_n()][i];  // cost from current node*'s state to state i
            if (connection == 0) continue;  // no connection, continue loop
            // create new node for state i with parent current
            next_node = new node(i, current->get_path_cost() + connection, current);
            line.push(next_node);   // push new expanded path to priority queue
        }
    }

    // this block of code if probably useless because of the pre-processing; only leftover for thoroughness
    output_answers2(nullptr, connections, cities);
    return INT16_MAX;
}
