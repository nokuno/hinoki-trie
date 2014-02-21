#include "trie.hpp"

int main(int argc, char **argv) {
    if (argc < 2 || string(argv[1]) == "-h") {
        cerr << "Usage: hinoki-traverse trie [times]\n";
        return 0;
    }
    int times = argc > 2 ? atoi(argv[2]) : 1;

    Trie trie;
    trie.load(argv[1]);

    for (int i = 0; i < times; ++i)
        trie.traverse();
    return 0;
}
