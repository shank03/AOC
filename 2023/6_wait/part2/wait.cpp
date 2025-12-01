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

uint64_t extract_data(std::string &line, std::string token) {
    std::string data = split(line, token)[1];

    std::vector<std::string> nums = split(data, " ");

    uint64_t res = 0;
    for (auto &s : nums) {
        if (s.empty()) continue;
        uint64_t i = std::atoi(s.c_str());
        if (i == 0) continue;
        res *= pow<uint64_t>(10, s.length());
        res += i;
    }
    return res;
}

uint64_t solve(std::vector<std::string> &lines) {
    uint64_t res = 0;

    uint64_t times     = extract_data(lines[0], "Time:");
    uint64_t distances = extract_data(lines[1], "Distance:");

    uint64_t hold = 0, wins = 0;
    while (hold != times) {
        uint64_t travelled = hold * (times - hold);
        if (travelled > distances) {
            res++;
        }
        hold++;
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: wait <file>" << std::endl;
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
