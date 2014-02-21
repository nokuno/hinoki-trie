#include "trie.hpp"

int main(int argc, char **argv) {
    if (argc < 2 || string(argv[1]) == "-h") {
        cerr << "Usage: hinoki-correct trie\n";
        return 0;
    }

    Trie trie;
    trie.load(argv[1]);
    string query;
    while (cin >> query) {
        set<string> results = trie.correct(query, 2);
        for (string s : results)
            cout << s << ", ";
        cout << endl;
    }
    return 0;
}
