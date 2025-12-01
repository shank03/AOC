#include <algorithm>
#include <fstream>
#include <iostream>

struct num_data {
    int num;
    int start;
    int end;
};

bool is_symbol(char c) {
    return c != '.' && (c < '0' || c > '9');
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

std::vector<num_data> extract_nums(std::string &str) {
    std::vector<num_data> v_idx;

    int d = -1, s = -1;
    for (int i = 0; i < str.length(); i++) {
        if (is_digit(str[i])) {
            if (s == -1) s = std::max(i - 1, 0);
            if (d == -1) d = 0;
            d *= 10;
            d += str[i] - 48;
            continue;
        }
        if (d != -1) {
            v_idx.push_back({ d, s, i });
            d = s = -1;
        }
    }
    if (d != -1) {
        v_idx.push_back({ d, s, (int) str.length() - 1 });
        d = s = -1;
    }
    return v_idx;
}

bool part_numbers(num_data *t, std::string &line) {
    for (int i = t->start; i <= t->end; i++) {
        if (is_symbol(line[i])) {
            return true;
        }
    }
    return false;
}

int solve(std::vector<std::string> &lines) {
    int res = 0;

    for (int i = 0; i < lines.size(); i++) {
        auto v = extract_nums(lines[i]);
        for (auto &t : v) {
            int u = std::max(i - 1, 0);
            int d = std::min(i + 1, (int) lines.size() - 1);

            bool valid = false;
            valid |= part_numbers(&t, lines[i]);
            valid |= part_numbers(&t, lines[u]);
            valid |= part_numbers(&t, lines[d]);

            if (valid) {
                res += t.num;
            }
        }
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: gear <file>" << std::endl;
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
