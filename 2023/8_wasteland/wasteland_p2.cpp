#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

enum Direction {
    R,
    L
};

struct Node {
    Node       *l;
    Node       *r;
    std::string v;
};

struct Conn {
    std::string l;
    std::string r;
};

std::vector<std::string> split(std::string &s, std::string delimiter) {
    size_t                   pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string              token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token     = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::vector<Direction> extract_directions(std::string &str) {
    std::vector<Direction> res;
    for (auto &c : str) {
        switch (c) {
            case 'R': res.push_back(Direction::R); break;
            case 'L': res.push_back(Direction::L); break;
            default: break;
        }
    }
    return res;
}

std::pair<Node, Conn> extract_node(std::string &str) {
    std::pair<Node, Conn> res;

    std::vector<std::string> data  = split(str, " = (");
    std::vector<std::string> conns = split(data[1], ", ");

    res.first     = Node { .l = nullptr, .r = nullptr, .v = data[0] };
    std::string l = conns[0], r = conns[1];
    r.pop_back();
    res.second = Conn { .l = l, .r = r };
    return res;
}

std::vector<uint64_t> get_cycles(std::vector<Node> *nodes, std::vector<Direction> &dir) {
    std::vector<uint64_t> res;

    for (auto &n : *nodes) {
        Node    *curr  = &n;
        uint64_t steps = 0;
        int      di    = 0;
        while (curr != nullptr && curr->v[2] != 'Z') {
            switch (dir[di]) {
                case L: curr = curr->l; break;
                case R: curr = curr->r; break;
                default: break;
            }
            steps++;
            di = (di + 1) % dir.size();
        }
        res.push_back(steps);
    }
    return res;
}

uint64_t lcm(std::vector<uint64_t> &nums) {
    uint64_t res = nums[0];
    for (auto &n : nums) {
        res = (res * n) / std::__algo_gcd(res, n);
    }
    return res;
}

bool all_reached(std::vector<Node *> *n) {
    for (auto &p : *n) {
        if (p->v[2] != 'Z') return false;
    }
    return true;
}

uint64_t solve(std::vector<std::string> &lines) {
    int res = 0;

    std::vector<Direction> directions = extract_directions(lines[0]);

    std::vector<std::pair<Node, Conn>> edges;
    std::map<std::string, Node>        nodes;
    for (int i = 2; i < lines.size(); i++) {
        auto p           = extract_node(lines[i]);
        nodes[p.first.v] = p.first;
        edges.push_back(p);
    }

    for (auto &e : edges) {
        if (nodes.find(e.second.l) != nodes.end()) {
            nodes[e.first.v].l = &nodes[e.second.l];
        }
        if (nodes.find(e.second.r) != nodes.end()) {
            nodes[e.first.v].r = &nodes[e.second.r];
        }
    }

    std::vector<Node>   s_nodes;
    std::vector<Node *> c_nodes;
    for (auto &it : nodes) {
        if (it.first[2] == 'A') {
            s_nodes.push_back(it.second);
            c_nodes.push_back(&it.second);
        }
    }

    std::vector<uint64_t> nums = get_cycles(&s_nodes, directions);
    return lcm(nums);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: wasteland <file>" << std::endl;
        return -1;
    }

    std::ifstream file(argv[1], std::ios::in);
    if (!file.is_open()) {
        std::cout << "Unable to open sample file" << std::endl;
    }

    std::vector<std::string> lines;
    std::string              line;
    while (std::getline(file, line)) {
        lines.emplace_back(line);
    }
    file.close();

    std::cout << solve(lines) << std::endl;

    return 0;
}
