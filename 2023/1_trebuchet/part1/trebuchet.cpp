#include <fstream>
#include <iostream>
#include <vector>

int solve(std::vector<std::string> &lines) {
    int res = 0;
    for (auto &str : lines) {
        int x = -1;
        for (auto &c : str) {
            int d = c - 48;
            if (d >= 0 && d <= 9) {
                x = d;
                break;
            }
        }

        for (int i = str.length(); i >= 0; i--) {
            int d = str[i] - 48;
            if (d >= 0 && d <= 9) {
                x *= 10;
                x += d;
                break;
            }
        }
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
