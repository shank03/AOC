#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <thread>

#define MAX_RANK 5

enum Type {
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_KIND,
    FULL_HOUSE,
    FOUR_KIND,
    FIVE_KIND,
    NONE,
};

std::map<char, char> card_strength_map = {
    {'A', 'E'},
    {'K', 'D'},
    {'Q', 'C'},
    {'J', 'B'},
    {'T', 'A'},
};

struct Hand {
    std::string hand;
    std::string psuedo_hand;
    int         bid;
    Type        type = HIGH_CARD;
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

bool n_kind(std::string &hnd, int n) {
    std::map<char, int> mp;
    for (auto &s : hnd) mp[s]++;

    bool found = false;
    for (auto &it : mp) {
        found |= it.second == n;
    }
    return found && mp.size() == MAX_RANK - n + 1;
}

bool n_pair(std::string &hnd, int n) {
    std::map<char, int> mp;
    for (auto &s : hnd) mp[s]++;

    int p = 0;
    for (auto &it : mp) {
        if (it.second == 2) {
            p++;
        }
    }
    return p == n && mp.size() == MAX_RANK - p;
}

bool full_house(std::string &hnd) {
    std::map<char, int> mp;
    for (auto &s : hnd) mp[s]++;

    bool tf = false, sf = false;
    for (auto &it : mp) {
        tf |= it.second == 3;
        sf |= it.second == 2;
    }
    return mp.size() == 2 && tf && sf;
}

Type validate_hand(std::string &hnd) {
    if (n_kind(hnd, 5)) {
        return Type::FIVE_KIND;
    }
    if (n_kind(hnd, 4)) {
        return Type::FOUR_KIND;
    }
    if (full_house(hnd)) {
        return Type::FULL_HOUSE;
    }
    if (n_kind(hnd, 3)) {
        return Type::THREE_KIND;
    }
    if (n_pair(hnd, 2)) {
        return Type::TWO_PAIR;
    }
    if (n_pair(hnd, 1)) {
        return Type::ONE_PAIR;
    }
    return Type::HIGH_CARD;
}

Hand extract_hand(std::string &str) {
    std::vector<std::string> data = split(str, " ");

    std::string cards = data[0];
    for (int i = 0; i < cards.length(); i++) {
        if (cards[i] >= '0' && cards[i] <= '9') continue;
        cards[i] = card_strength_map[cards[i]];
    }

    Hand hnd;
    hnd.psuedo_hand = cards;
    hnd.hand        = data[0];
    hnd.bid         = std::atoi(data[1].c_str());
    hnd.type        = validate_hand(hnd.hand);
    return hnd;
}

int solve(std::vector<std::string> &lines) {
    int res = 0;

    std::vector<Hand> hands;
    for (auto &str : lines) {
        hands.push_back(extract_hand(str));
    }

    std::sort(hands.begin(), hands.end(), [](const Hand &a, const Hand &b) -> bool {
        return std::make_pair(a.type, a.psuedo_hand) < std::make_pair(b.type, b.psuedo_hand);
    });

    for (int i = 0; i < hands.size(); i++) {
        Hand h = hands[i];
        res += (h.bid * (i + 1));
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: camel <file>" << std::endl;
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
