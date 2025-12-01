#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

int solve(std::vector<std::string> &lines) {
    std::map<std::string, int> mp = {
        {  "one", 1},
        {  "two", 2},
        {"three", 3},
        { "four", 4},
        { "five", 5},
        {  "six", 6},
        {"seven", 7},
        {"eight", 8},
        { "nine", 9},
        { "zero", 0},
    };

    std::vector<std::string> nums = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
        "zero",
    };

    int res = 0;

    for (auto &str : lines) {
        int x   = 0;
        int idx = -1;
        for (int i = 0; i < str.length(); i++) {
            int d = str[i] - 48;
            if (d >= 0 && d <= 9) {
                x   = d;
                idx = i;
                break;
            }
        }
        for (auto &it : nums) {
            size_t pos = str.find(it);
            if (pos != std::string::npos) {
                if (pos < idx) {
                    idx = pos;
                    x   = mp[it];
                }
            }
        }

        int num = x;
        idx     = 0;

        for (int i = str.length(); i >= 0; i--) {
            int d = str[i] - 48;
            if (d >= 0 && d <= 9) {
                num = d;
                idx = i;
                break;
            }
        }

        for (auto &it : nums) {
            size_t pos = str.rfind(it);
            if (pos != std::string::npos) {
                if (pos > idx) {
                    idx = pos;
                    num = mp[it];
                }
            }
        }
        x *= 10;
        x += num;
        res += x;
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: trebuchet <file>" << std::endl;
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
