#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

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

std::vector<int> extract_seq(std::string &str) {
    std::vector<std::string> data = split(str, " ");
    std::vector<int>         res;
    for (auto &d : data) {
        res.push_back(std::atoi(d.c_str()));
    }
    return res;
}

bool all_zeros(std::vector<int> &seq) {
    for (auto &i : seq) {
        if (i != 0) return false;
    }
    return true;
}

int extrapolate(std::vector<int> &seq) {
    std::vector<std::vector<int>> exp = { seq };

    std::vector<int> diff;
    for (int y = 0; y < exp.size(); y++) {
        for (int i = 0; i < exp[y].size() - 1; i++) {
            diff.push_back(exp[y][i + 1] - exp[y][i]);
        }
        exp.push_back(diff);
        diff.clear();
        if (all_zeros(exp[y + 1])) {
            break;
        }
    }

    for (int y = exp.size() - 1; y > 0; y--) {
        exp[y - 1].push_back(exp[y].back() + exp[y - 1].back());
    }

    return exp[0].back();
}

int solve(std::vector<std::string> &lines) {
    int res = 0;

    std::vector<std::vector<int>> sequences;
    for (auto &l : lines) {
        sequences.push_back(extract_seq(l));
    }

    for (auto &seq : sequences) {
        res += extrapolate(seq);
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: mirage <file>" << std::endl;
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
