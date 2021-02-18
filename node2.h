//
// Created by luke on 2/17/2021.
//

#ifndef PP1_NODE2_H
#define PP1_NODE2_H


class node2 {
int n;
node2* parent;
public:
    int path_cost;

    node2(int n, int pc = 0, node2* n2 = nullptr) {
        this->n = n;
        this->path_cost = pc;
        this->parent = n2;
    }
    bool operator<(const node2& nd2) {
        return this->path_cost < nd2.path_cost;
    }
    bool operator>(const node2& nd2) {
        return this->path_cost > nd2.path_cost;
    }
    int get_n() {
        return this->n;
    }
    int get_path_cost() {
        return this->path_cost;
    }
    node2* get_parent() {
        return this->parent;
    }
};

struct node_cmp {
    bool operator()(const node2* a, const node2* b) {
        return a->path_cost > b->path_cost;
    }
};


#endif //PP1_NODE2_H
