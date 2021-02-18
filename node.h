//
// Created by luke on 2/16/2021.
//

#ifndef PP1_NODE_H
#define PP1_NODE_H

#include <map>
#include <set>
#include <string>

using namespace std;

class node {
public:
    node() {
        this->city = "NONE";
    }
    node(string name) {
        this->city = name;
    }
    void add_neighbor(string c, node* n, int d) {
        this->neighbors[c] = pair<int, node*>(d, n);
    }
    string get_city() {
        return this->city;
    }
    int get_distance(string c) {
        return this->neighbors[c].first;
    }
    node* get_neighbor(string c) {
        return this->neighbors[c].second;
    }
    set<string> get_neighbors() {
        set<string> ret_set;
        for (map<string, pair<int, node*>>::iterator it = this->neighbors.begin(); it != this->neighbors.end(); ++it) {
            ret_set.insert(it->first);
        }
        return ret_set;
    }

    node& operator=(const node& nd) {
        if (this == &nd) return *this;
        this->city = nd.city;
        this->neighbors = nd.neighbors;
        return *this;
    }
private:
    string city;
    map<string, pair<int, node*>> neighbors;
};


#endif //PP1_NODE_H
