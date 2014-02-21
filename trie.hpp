#ifndef TRIE_HPP
#define TRIE_HPP

#include <vector>
#include <queue>
#include <set>
#include <fstream>
#include <iostream>
#include "node.hpp"

typedef char Label;
const Label LEAF = '\0';

struct Trie {
    vector<Node> nodes;
    vector<Label> labels;

    Trie() {
        // super root
        nodes.push_back(Node());
        labels.push_back(LEAF);
    }

    size_t size() const { return nodes.size(); }
    size_t nodeSize() const { return sizeof(Node) + sizeof(Label); }

    void addNode(Label label, Index next, Index child) {
        labels.push_back(label);
        nodes.push_back(Node(next, child));
    }

    bool insert(const string &key) {
        string key_(key);
        key_.push_back(LEAF);
        return insert(key_.begin(), key_.end(), 0);
    }

    bool insert(string::const_iterator begin, string::const_iterator end, Index current) {
        // search prefix
        while (begin != end) {

            // linear search
            while (true) {
                // Found
                if (labels[current] == *begin) {
                    break;
                }
                // Not found
                if (nodes[current].next == NOP) {
                    goto NEXT;
                }
                // Go to next
                current = nodes[current].next;
            }

            begin ++;
            current = nodes[current].child;
        }
NEXT:

        // Duplicate entry!
        if (begin == end)
            return false;

        // create new branch
        if ((size_t)current < nodes.size()) {
            addNode(*begin, nodes[current].next, NOP);
            nodes[current].next = nodes.size()-1;
            current = nodes.size() - 1;
            begin ++;
        }

        // insert suffix
        while (begin != end) {
            addNode(*begin, NOP, NOP);
            if ((size_t)current < nodes.size()) {
                nodes[current].child = nodes.size()-1;
            }
            current = nodes.size() - 1;
            begin ++;
        }

        return true;
    }

    Index lookup(const string &key) {
        string key_(key);
        key_.push_back(LEAF);
        return lookup(key_.begin(), key_.end(), 0);
    }

    Index lookup(string::const_iterator begin, string::const_iterator end, Index current) {
        // search prefix
        while (begin != end) {

            // linear search
            while (true) {
                // Found
                if (labels[current] == *begin) {
                    break;
                }
                // Not found
                if (nodes[current].next == NOP) {
                    return NOP;
                }
                // Go to next
                current = nodes[current].next;
            }

            begin ++;
            if (begin == end)
                return current;
            current = nodes[current].child;
        }
        return NOP;
    }

    vector<string> predict(const string &key) {
        vector<string> results = predict(key.begin(), key.end(), 0);

        for (string &result: results) {
            result.insert(result.begin(), key.begin(), key.end());
        }

        return results;
    }

    vector<string> predict(string::const_iterator begin, string::const_iterator end, Index current) {
        // search prefix
        while (begin != end) {

            // linear search
            while (true) {
                // Found
                if (labels[current] == *begin) {
                    break;
                }
                // Not found
                if (nodes[current].next == NOP) {
                    return vector<string>();
                }
                // Go to next
                current = nodes[current].next;
            }

            begin ++;

            current = nodes[current].child;
        }

        return predict(current);
    }

    vector<string> predict(Index current) {
        vector<string> results;

        if (nodes[current].child == NOP) {
            results.push_back("");
        }

        if (nodes[current].child != NOP) {
            vector<string> temp = predict(nodes[current].child);
            for (auto s : temp) {
                s.insert(s.begin(), labels[current]);
                results.push_back(s);
            }
        }

        if (nodes[current].next != NOP) {
            vector<string>temp = predict(nodes[current].next);
            results.insert(results.end(), temp.begin(), temp.end());
        }

        return results;
    }

    void traverse(Index current = 0, string str="") {
        char label = labels.at(current);

        // print terminal
        if (label == LEAF)
            cout << str << endl;

        // traverse first child
        if (nodes.at(current).child != NOP)
            traverse(nodes.at(current).child, str+label);

        // traverse next sibling
        if (nodes.at(current).next != NOP)
            traverse(nodes.at(current).next, str);
    }

    struct Path {
        string str;
        string::const_iterator position;
        Index current;
        int cost;

        Path(string str, string::const_iterator position, Index current, int cost)
            : str(str), position(position), current(current), cost(cost) {}
    };

    struct Compare {
        bool operator()(const Path &a, const Path &b) {
            return a.cost < b.cost;
        }
    };

    set<string> correct(const string &key, int threshold) {
        set<string> results;
        priority_queue<Path, vector<Path>, Compare> que;
        que.push(Path("", key.begin(), 0, 0));

        while (!que.empty()) {
            Path path = que.top();
            que.pop();
            Index i = path.current;

            while (i != NOP) {
                if (nodes[i].child == NOP && path.position == key.end()) {
                    results.insert(path.str);
                    i = nodes[i].next;
                    continue;
                }

                // Replace
                {
                    Path new_path(path);
                    new_path.cost += (labels[i] != *path.position);
                    new_path.position++;
                    new_path.current = nodes[i].child;
                    new_path.str.push_back(labels[i]);

                    if (new_path.cost <= threshold)
                        que.push(new_path);
                }

                // Insert
                {
                    Path new_path(path);
                    new_path.cost += 1;
                    new_path.current = nodes[i].child;
                    new_path.str.push_back(labels[i]);

                    if (new_path.cost <= threshold)
                        que.push(new_path);
                }

                i = nodes[i].next;
            }

            // Delete
            Path new_path(path);
            new_path.cost += 1;
            new_path.position++;

            if (new_path.cost <= threshold)
                que.push(new_path);
        }

        return results;
    }

    void save(const string &filename) {
        ofstream ofs(filename, ios::binary);
        ofs.write(labels.data(), labels.size() * sizeof(Label));
        ofs.write((Label*)nodes.data(), nodes.size() * sizeof(Node));
        ofs.close();
    }

    void load(const string &filename) {
        labels.clear();
        nodes.clear();
        ifstream ifs(filename, ios::binary);
        ifs.seekg (0, ifs.end);
        size_t size  = ifs.tellg() / nodeSize();
        ifs.seekg (0, ifs.beg);

        labels.resize(size);
        nodes.resize(size);

        ifs.read(labels.data(), size * sizeof(Label));
        ifs.read((Label*)nodes.data(), size * sizeof(Node));
        ifs.close();
    }

    void read(const string &filename) {
        ifstream ifs(filename);
        string str;
        while (getline(ifs, str)) {
            insert(str);
        }
        ifs.close();
    }

    bool operator==(const Trie &trie) const {
        if (size() != trie.size())
            return false;

        for (size_t i = 0; i < size(); ++i) {
            if (labels[i] != trie.labels[i] ||
                nodes[i] != trie.nodes[i]) {
                return false;
            }
        }
        return true;
    }
};

inline ostream& operator<<(ostream& out, const Trie& trie) {
    out << "size: " << trie.size() << endl;
    for (size_t i = 0; i < trie.size(); ++i) {
        out << i << ": " << trie.labels[i] << " : "
            << trie.nodes[i] << endl;
    }
    return out;
}

#endif
