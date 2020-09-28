#include <iostream>
#include "trie.hpp"
#include <boost/property_tree/ptree.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <string>

using namespace std::chrono_literals;
struct Data {
    std::string key;
};

int main(int, char**) {
    namespace pt = boost::property_tree;

    Trie<std::string, Data> trie;

    {
        pt::ptree tree;
        pt::read_json(std::getenv("PATH_TO_JSON"), tree);
        for(const auto & [_,ch]: tree.get_child("")) {
            auto city_name = ch.get<std::string>("name");
            std::string lowerCase = city_name;
            std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(),
                [](unsigned char c){ return std::tolower(c); });
            trie.insert(lowerCase, {city_name});
        }
    }

    auto searchResult = trie.search("ivan");

    for(std::size_t i = 0; i < searchResult.size(); i++) {
        auto dlist = searchResult[i];
        std::cout << i << std::endl;
        for(auto d: dlist) {
            std::cout << '\t'<<d.key << std::endl;
        }

    }
    std::cout << "waiting" << std::endl;
    std::this_thread::sleep_for(10000s);
    return 0;
}
