#include <iostream>
#include "trie.hpp"
struct Data {
    std::string key;
    double someContent;

};

int main(int, char**) {
    Data testData[] = {
        {"fuu", 36.6},
         {"fuub", 13},
         {"fuublo", 1010},
         {"fuubla", 66},
         {"fuuh", 1488}
    };

    Trie<decltype (Data::key), Data> trie;

    for(const auto & d: testData) {
        trie.insert(d.key, d);
    }


    auto searchResult = trie.search("fuublo");

    for(std::size_t i = 0; i < searchResult.size(); i++) {
        std::cout << searchResult[i].someContent << std::endl;
    }
    return 0;
}
