#include "trie.hpp"

int main(int argc, char **argv) {
    if (argc < 2 || string(argv[1]) == "-h") {
        cerr << "Usage: hinoki-lookup trie\n";
        return 0;
    }

    Trie trie;
    trie.load(argv[1]);
    string query;
    while (cin >> query) {
        Index result = trie.lookup(query);
        cout << result << endl;
    }
    return 0;
}
