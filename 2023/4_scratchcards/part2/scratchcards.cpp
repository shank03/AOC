#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

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

std::map<int, int> extract_wnums(std::string &nums) {
    std::map<int, int> res;

    std::vector<std::string> tokens = split(nums, " ");
    for (auto &t : tokens) {
        if (t.empty()) continue;
        res[std::atoi(t.c_str())]++;
    }
    return res;
}

std::vector<int> extract_mnums(std::string &nums) {
    std::vector<int> res;

    std::vector<std::string> tokens = split(nums, " ");
    for (auto &t : tokens) {
        if (t.empty()) continue;
        res.emplace_back(std::atoi(t.c_str()));
    }
    return res;
}

int solve(std::vector<std::string> &lines) {
    int res = 0;

    std::vector<int> match;
    for (auto &str : lines) {
        std::vector<std::string> data = split(str, ": ");
        std::string              card = data[1];

        std::vector<std::string> numbers = split(card, " | ");
        std::string              wn = numbers[0], mn = numbers[1];

        std::map<int, int> winning_nums = extract_wnums(wn);
        std::vector<int>   my_nums      = extract_mnums(mn);

        int matches = 0;
        for (auto &n : my_nums) {
            if (winning_nums.find(n) != winning_nums.end()) {
                matches++;
            }
        }
        match.push_back(matches);
    }

    std::vector<int> copies(match.size(), 1);
    for (int i = 0; i < match.size(); i++) {
        int m = match[i];
        for (int c = 0; c < copies[i]; c++) {
            int x = m;
            for (int j = i + 1; j < match.size() && x--; j++) {
                copies[j]++;
            }
        }
    }

    for (auto &i : copies) res += i;
    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: scratchcards <file>" << std::endl;
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
