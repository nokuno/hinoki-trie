#include <gtest/gtest.h>
#include "trie.hpp"

class TrieTest : public ::testing::Test {
 protected:
    void SetUp() {
        trie.insert("a");
        trie.insert("ab");
        trie.insert("acd");
        trie.insert("ac");
    }
    Trie trie;
};

TEST(Trie, Build) {
    Trie trie;
    Trie expected;

    // Initialized with super root
    expected.labels = {
        LEAF,
    };
    expected.nodes = {
        {NOP,NOP},
    };
    EXPECT_EQ(expected, trie);

    // First node
    trie.insert("a");
    expected.labels = {
        LEAF,
        'a',
        LEAF,
    };
    expected.nodes = {
        {1,NOP},
        {NOP,2},
        {NOP,NOP},
    };
    EXPECT_EQ(expected, trie);

    // Extend suffix
    trie.insert("ab");
    expected.labels = {
        LEAF,
        'a',
        LEAF,
        'b',
        LEAF,
    };
    expected.nodes = {
        {1,NOP},
        {NOP,2},
        {3,NOP},
        {NOP,4},
        {NOP,NOP},
    };
    EXPECT_EQ(expected, trie);

    // Create branch
    trie.insert("acd");
    expected.labels = {
        LEAF,
        'a',
        LEAF,
        'b',
        LEAF,
        'c',
        'd',
        LEAF,
    };
    expected.nodes = {
        {1,NOP},
        {NOP,2},
        {3,NOP},
        {5,4},
        {NOP,NOP},
        {NOP,6},
        {NOP,7},
        {NOP,NOP},
    };
    EXPECT_EQ(expected, trie);

    // Add prefix
    trie.insert("ac");
    expected.labels = {
        LEAF,
        'a',
        LEAF,
        'b',
        LEAF,
        'c',
        'd',
        LEAF,
        LEAF,
    };
    expected.nodes = {
        {1,NOP},
        {NOP,2},
        {3,NOP},
        {5,4},
        {NOP,NOP},
        {NOP,6},
        {8,7},
        {NOP,NOP},
        {NOP,NOP},
    };
    EXPECT_EQ(expected, trie);

    // Duplicate entry
    trie.insert("a");
    EXPECT_EQ(expected, trie);
}

TEST_F(TrieTest, Lookup) {
    Index actual;

    actual = trie.lookup(string("a"));
    EXPECT_EQ(2, actual);

    actual = trie.lookup(string("ab"));
    EXPECT_EQ(4, actual);

    actual = trie.lookup(string("ac"));
    EXPECT_EQ(8, actual);

    actual = trie.lookup(string("abc"));
    EXPECT_EQ(NOP, actual);

    actual = trie.lookup(string("ad"));
    EXPECT_EQ(NOP, actual);

    actual = trie.lookup(string("acd"));
    EXPECT_EQ(7, actual);
}

TEST_F(TrieTest, Predict) {
    vector<string> actual, expected;

    actual = trie.predict(string("a"));
    expected = {"a", "ab", "acd", "ac"};
    EXPECT_EQ(expected, actual);

    actual = trie.predict(string("ab"));
    expected = {"ab"};
    EXPECT_EQ(expected, actual);

    actual = trie.predict(string("ac"));
    expected = {"acd", "ac"};
    EXPECT_EQ(expected, actual);

    actual = trie.predict(string("b"));
    expected = {};
    EXPECT_EQ(expected, actual);
}

TEST_F(TrieTest, Correct) {
    set<string> actual, expected;

    // replace
    actual = trie.correct(string("axd"), 1);
    expected = {"acd"};
    EXPECT_EQ(expected, actual);

    // insert
    actual = trie.correct(string("b"), 1);
    expected = {"a", "", "ab"};
    EXPECT_EQ(expected, actual);

    // delete
    actual = trie.correct(string("acxd"), 1);
    expected = {"acd"};
    EXPECT_EQ(expected, actual);

    // empty
    actual = trie.correct(string("adc"), 0);
    expected = {};
    EXPECT_EQ(expected, actual);
}

TEST_F(TrieTest, SaveLoad) {
    trie.save("temp.trie");

    Trie actual;
    actual.load("temp.trie");
    EXPECT_EQ(trie, actual);
}

