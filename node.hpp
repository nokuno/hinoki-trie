#ifndef NODE_HPP
#define NODE_HPP

using namespace std;

typedef uint32_t Index;
const Index NOP = -1;

struct Node {
    Index next;
    Index child;

    Node() : next(NOP), child(NOP) {}
    Node(Index next, Index child) :
        next(next), child(child) {}

    bool operator==(const Node &node) const {
        return next == node.next
            && child == node.child;
    }

    bool operator!=(const Node &node) const {
        return !(*this == node);
    }
};

inline ostream& operator<<(ostream& out, const Node& node) {
    out << node.next << ", "
        << node.child;
    return out;
}

#endif
