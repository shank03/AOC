#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

struct game_set {
    int r;
    int g;
    int b;

    bool valid_set(game_set &game) {
        return game.r - r >= 0 && game.g - g >= 0 && game.b - b >= 0;
    }

    int power() {
        return r * g * b;
    }
};

game_set                 possible_game { .r = 12, .g = 13, .b = 14 };
std::vector<std::string> colors = { "red", "blue", "green" };

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

int extract_initial_value(std::string &str) {
    int value = 0;
    for (auto &c : str) {
        if (c == ' ') {
            break;
        }
        value *= 10;
        value += (c - 48);
    }
    return value;
}

void parse(std::string &line, std::vector<game_set> *games) {
    std::vector<std::string> data = split(line, ": ");
    std::vector<std::string> sets = split(data[1], "; ");
    for (auto &i : sets) {
        std::vector<std::string> g = split(i, ", ");

        game_set game { 0, 0, 0 };
        for (auto &cube : g) {
            int value = extract_initial_value(cube);
            if (cube.find("red") != std::string::npos) {
                game.r = value;
            }
            if (cube.find("green") != std::string::npos) {
                game.g = value;
            }
            if (cube.find("blue") != std::string::npos) {
                game.b = value;
            }
        }
        games->emplace_back(game);
    }
}

int solve(std::vector<std::string> &lines) {
    int res = 0;

    std::vector<std::vector<game_set>> games(lines.size(), std::vector<game_set>());

    for (int i = 0; i < lines.size(); i++) {
        parse(lines[i], &games[i]);

        auto     set = games[i];
        game_set playable { 0, 0, 0 };
        for (auto &s : set) {
            playable.r = std::max(playable.r, s.r);
            playable.g = std::max(playable.g, s.g);
            playable.b = std::max(playable.b, s.b);
        }

        res += playable.power();
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: conundrum <file>" << std::endl;
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
