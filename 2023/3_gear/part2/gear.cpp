#include <algorithm>
#include <fstream>
#include <iostream>

struct num_data {
    int num;
    int y;
    int start;
    int end;
};

struct coord {
    int x;
    int y;
};

bool is_symbol(char c) {
    return c == '*';
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

std::vector<num_data> extract_nums(std::string &str, int y) {
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
            v_idx.push_back({ d, y, s, i });
            d = s = -1;
        }
    }
    if (d != -1) {
        v_idx.push_back({ d, y, s, (int) str.length() - 1 });
        d = s = -1;
    }
    return v_idx;
}

std::vector<coord> extract_symbols(std::vector<std::string> &lines) {
    std::vector<coord> res;

    for (int y = 0; y < lines.size(); y++) {
        for (int x = 0; x < lines[y].size(); x++) {
            if (is_symbol(lines[y][x])) {
                coord c = { .x = x, .y = y };
                res.emplace_back(c);
            }
        }
    }
    return res;
}

int part_numbers(coord *symbol, std::vector<num_data> *nums) {
    int res   = 1;
    int found = 0;
    for (auto &num : *nums) {
        int adj = abs(symbol->y - num.y);
        if (adj > 1) continue;

        if (symbol->x >= num.start && symbol->x <= num.end) {
            found++;
            res *= num.num;
        }
    }
    return found >= 2 ? res : 0;
}

int solve(std::vector<std::string> &lines) {
    int res = 0;

    std::vector<coord>    symbols = extract_symbols(lines);
    std::vector<num_data> nums;

    for (int i = 0; i < lines.size(); i++) {
        auto v = extract_nums(lines[i], i);
        nums.insert(nums.end(), v.begin(), v.end());
    }

    for (auto &c : symbols) {
        res += part_numbers(&c, &nums);
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
