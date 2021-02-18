#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <queue>
#include <stack>

#include "node.h"
#include "node2.h"

using namespace std;

int DIAMETER = 0;
int VERTICES = 0;

// these were used for my first solution, but I think they violated some of the requirements of the project
map<string, node> make_connections1(string);
pair<int, vector<node*>> UCS1(node*, const string&, map<string, node>&);
void output_answers1(pair<int, vector<node*>>);

// these are for my implemented solution; I think it satisfies the requirements
vector<string> make_cities(string);
vector<vector<int>> make_connections2(string, map<string, int>);
int UCS2(int, const int, vector<vector<int>>, vector<string>);

int main(int argc, char* argv[]) {
    int method = 2;

    string start;
    string end;
    string input_file;
    if (argc == 4) {
        input_file = argv[1];
        start = argv[2];
        end = argv[3];
    } else {
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
        output_answers1(answer);
    } else if (method == 2) {
        vector<string> cities = make_cities(input_file);
        map<string, int> city_str_2_num;
        for (int i = 0; i < cities.size(); ++i) {
            city_str_2_num[cities[i]] = i;
        }
        UCS2(city_str_2_num[start], city_str_2_num[end], make_connections2(input_file, city_str_2_num), cities);
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

void output_answers1(pair<int, vector<node*>> answer) {
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
}


// METHOD 2
vector<string> make_cities(string file) {
    ifstream input(file);

    set<string> initialized_cities;

    while (!input.eof()) {
        string line;
        getline(input, line);
        if (line == "END OF INPUT") break;

        stringstream ss(line);
        string city1, city2; int dist;
        ss >> city1 >> city2 >> dist;

        initialized_cities.insert(city1);
        initialized_cities.insert(city2);

        DIAMETER += dist;
    }
    vector<string> vec;
    for (string city: initialized_cities) {
        vec.push_back(city);
    }
    return vec;
}

vector<vector<int>> make_connections2(string file, map<string, int> str_2_num) {
    ifstream input(file);

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

        stringstream ss(line);
        string city1, city2; int dist;
        ss >> city1 >> city2 >> dist;

        int num1 = str_2_num[city1];
        int num2 = str_2_num[city2];

        connects[num1][num2] = dist;
        connects[num2][num1] = dist;
    }

    return connects;
}

stack<int> node_2_stack(node2* nd, stack<int> stk) {
    if (nd == nullptr) {
        return stk;
    } else {
        stk.push(nd->get_n());
        return node_2_stack(nd->get_parent(), stk);
    }
}

void output_answers2(node2* nd2, vector<vector<int>> connections, vector<string> cities) {
    stack<int> path = node_2_stack(nd2, stack<int>());

    if (nd2 == nullptr) {
        cout << "distance: infinity\nroute:\nnone";
    } else if (nd2->get_parent() == nullptr) {
        cout << "distance: 0 km\nroute:\n" << cities[nd2->get_n()] << " to " << cities[nd2->get_n()] << ", 0 km";
    } else {
        cout << "distance: " << nd2->get_path_cost() <<  "km\nroute:";
        int prev = path.top();
        path.pop();
        while (!path.empty()) {
            cout << endl << cities[prev] << " to " << cities[path.top()];
            cout << ", " << connections[prev][path.top()] << " km";
            prev = path.top();
            path.pop();
        }
    }
}

vector<bool> preprocessing_dfs(int s, vector<vector<int>> c, vector<bool> &v) {
    v[s] = true;
    for (int i = 0; i < c[s].size(); ++i) {
        if (c[s][i] > 0 && !v[i]) {
            preprocessing_dfs(i, c, v);
        }
    }
    return v;
}

int UCS2(int start, const int destination, vector<vector<int>> connections, vector<string> cities) {
    vector<bool> vec(connections.size(), false);
    if (!preprocessing_dfs(start, connections, vec)[destination]) {
        output_answers2(nullptr, connections, cities);
        return INT64_MAX;
    }

    priority_queue<node2*, vector<node2*>, node_cmp> line;
    node2* start_node = new node2(start);
    line.push(start_node);

    while (!line.empty()) {
        node2* current = line.top();
        line.pop();

        if (current->get_path_cost() > DIAMETER) {
            output_answers2(nullptr, connections, cities);
            return INT64_MAX;
        }

        if (current->get_n() == destination) {
            output_answers2(current, connections, cities);
            return current->get_path_cost();
        }

        node2* next_node;
        for (int i = 0; i < connections[current->get_n()].size(); ++i) {
            int connection = connections[current->get_n()][i];
            if (connection == 0) continue;
            next_node = new node2(i, current->get_path_cost()+connection, current);
            line.push(next_node);
        }
    }

    output_answers2(nullptr, connections, cities);
    return INT64_MAX;
}
