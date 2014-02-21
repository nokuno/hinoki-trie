#include "trie.hpp"

int main(int argc, char **argv) {
    if (argc < 3 || string(argv[1]) == "-h") {
        cerr << "Usage: hinoki-build input output\n";
        return 0;
    }

    Trie trie;
    trie.read(argv[1]);
    trie.save(argv[2]);
    cout << "Node number: " << trie.size() << endl;
    cout << "Node size: " << trie.nodeSize() << endl;
    cout << "Total size: " << trie.size() * trie.nodeSize() << " bytes" << endl;
    return 0;
}
