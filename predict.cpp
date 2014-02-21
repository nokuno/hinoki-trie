#include "trie.hpp"

int main(int argc, char **argv) {
    if (argc < 2 || string(argv[1]) == "-h") {
        cerr << "Usage: hinoki-predict trie\n";
        return 0;
    }

    Trie trie;
    trie.load(argv[1]);
    string query;
    while (cin >> query) {
        vector<string> results = trie.predict(query);
        for (string s : results)
            cout << s << ", ";
        cout << endl;
    }
    return 0;
}
