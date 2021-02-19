//
// Created by luke on 2/17/2021.
//

#ifndef PP1_NODE_H
#define PP1_NODE_H


class node {
int n;  // state/city number
node* parent;   // pointer to the parent node
public:
    int path_cost;  // cost of the path through this node (public for the node_cmp struct)

    node(int n, int pc = 0, node* n2 = nullptr) {
        this->n = n;
        this->path_cost = pc;
        this->parent = n2;
    }
    int get_n() {
        return this->n;
    }
    int get_path_cost() {
        return this->path_cost;
    }
    node* get_parent() {
        return this->parent;
    }
};

// this is used so the priority queue sorts correctly; idea from stackoverflow
struct node_cmp {
    bool operator()(const node* a, const node* b) {
        return a->path_cost > b->path_cost;
    }
};


#endif //PP1_NODE_H
