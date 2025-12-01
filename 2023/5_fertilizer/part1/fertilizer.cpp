#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

enum class Element {
    SEED = 0,
    SOIL,
    FERTILIZER,
    WATER,
    LIGHT,
    TEMPERATURE,
    HUMIDITY,
    LOCATION,
};

enum class Type {
    SEED_TO_SOIL = 0,
    SOIL_TO_FERTILIZER,
    FERTILIZER_TO_WATER,
    WATER_TO_LIGHT,
    LIGHT_TO_TEMPERATURE,
    TEMPERATURE_TO_HUMIDITY,
    HUMIDITY_TO_LOCATION,
};

std::vector<Element> all_elements = {
    Element::SEED,
    Element::SOIL,
    Element::FERTILIZER,
    Element::WATER,
    Element::LIGHT,
    Element::TEMPERATURE,
    Element::HUMIDITY,
    Element::LOCATION,
};

struct Range {
    uint64_t src;
    // to
    uint64_t dst;
    uint64_t offset;

    bool validate_range(uint64_t *v) {
        if (src <= *v && *v < src + offset) {
            *v = dst + (*v - src);
            return true;
        }
        return false;
    }
};

struct Map {
    Type               type;
    std::vector<Range> ranges;
};

Type str_to_type(std::string &str) {
    Type type;

    if (str == "seed-to-soil") {
        return Type::SEED_TO_SOIL;
    } else if (str == "soil-to-fertilizer") {
        return Type::SOIL_TO_FERTILIZER;
    } else if (str == "fertilizer-to-water") {
        return Type::FERTILIZER_TO_WATER;
    } else if (str == "water-to-light") {
        return Type::WATER_TO_LIGHT;
    } else if (str == "light-to-temperature") {
        return Type::LIGHT_TO_TEMPERATURE;
    } else if (str == "temperature-to-humidity") {
        return Type::TEMPERATURE_TO_HUMIDITY;
    }
    return Type::HUMIDITY_TO_LOCATION;
}

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

std::vector<uint64_t> extract_seeds(std::string &str) {
    std::string              seeds_data = split(str, "seeds: ")[1];
    std::vector<std::string> seeds      = split(seeds_data, " ");

    std::vector<uint64_t> res;
    for (auto &s : seeds) {
        res.push_back(std::atoi(s.c_str()));
    }
    return res;
}

Map extract_map(std::string &line, std::vector<std::string> &info) {
    Map         map;
    std::string str_type = split(line, " ")[0];
    map.type             = str_to_type(str_type);

    for (auto &i : info) {
        std::vector<std::string> data = split(i, " ");

        Range r {
            .dst    = uint64_t(std::atoi(data[0].c_str())),
            .src    = uint64_t(std::atoi(data[1].c_str())),
            .offset = uint64_t(std::atoi(data[2].c_str())),
        };
        map.ranges.push_back(r);
    }
    return map;
}

void map_range(Element el_src, Element el_dst, std::map<Element, std::vector<uint64_t>> *elements, Map &map) {
    for (auto &s : (*elements)[el_src]) {
        bool found = false;
        for (auto &r : map.ranges) {
            uint64_t v = s;
            if ((found = r.validate_range(&v))) {
                (*elements)[el_dst].push_back(v);
                break;
            }
        }
        if (!found) (*elements)[el_dst].push_back(s);
    }
}

uint64_t solve(std::vector<std::string> &lines) {
    uint64_t res = UINT64_MAX;

    for (uint64_t i = 0; i < lines.size(); i++) {
        if (lines[i].empty()) {
            lines.erase(lines.begin() + i);
        }
    }

    std::map<Element, std::vector<uint64_t>> elements;
    std::map<Type, Map>                      maps;
    for (uint64_t i = 0; i < lines.size(); i++) {
        std::string str = lines[i];
        if (str[0] >= '0' && str[0] <= '9') continue;

        if (str.find("seeds:") != std::string::npos) {
            elements[Element::SEED] = extract_seeds(str);
            continue;
        }

        std::vector<std::string> info;
        for (uint64_t j = i + 1; j < lines.size(); j++) {
            if (lines[j][0] >= '0' && lines[j][0] <= '9') {
                info.push_back(lines[j]);
            } else {
                break;
            }
        }

        Map map        = extract_map(str, info);
        maps[map.type] = map;
    }

    map_range(Element::SEED, Element::SOIL, &elements, maps[Type::SEED_TO_SOIL]);
    map_range(Element::SOIL, Element::FERTILIZER, &elements, maps[Type::SOIL_TO_FERTILIZER]);
    map_range(Element::FERTILIZER, Element::WATER, &elements, maps[Type::FERTILIZER_TO_WATER]);
    map_range(Element::WATER, Element::LIGHT, &elements, maps[Type::WATER_TO_LIGHT]);
    map_range(Element::LIGHT, Element::TEMPERATURE, &elements, maps[Type::LIGHT_TO_TEMPERATURE]);
    map_range(Element::TEMPERATURE, Element::HUMIDITY, &elements, maps[Type::TEMPERATURE_TO_HUMIDITY]);
    map_range(Element::HUMIDITY, Element::LOCATION, &elements, maps[Type::HUMIDITY_TO_LOCATION]);

    for (auto &l : elements[Element::LOCATION]) {
        res = std::min(res, l);
    }

    return res;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: fertilizer <file>" << std::endl;
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
