hinoki-trie
===========

Basic pointer-based implementation of Trie data structure.

Install
-----------

    ./waf configure
    ./waf build --check
    sudo ./waf install

Usage Example
-----------
    % head words A
    A's
    AA's
    AB's
    ABM's
    AC's
    ACTH's
    AI's
    AIDS's AM's 
    % wc words
    99171  99171 938848 words

    % hinoki-build words words.hinoki
    Node number: 327150
    Node size: 9
    Total size: 2944350 bytes

    % hinoki-lookup words.hinoki
    beautiful
    80156

    % hinoki-predict words.hinoki
    beautiful
    beautiful, beautifully,

    % hinoki-correct words.hinoki
    beaitiful
    beautiful,

    % hinoki-traverse words.hinoki | head

    A
    A's
    AA's
    AB's
    ABM's
    AC's
    ACTH's
    AI's
    AIDS's

What is Hinoki?
-----------

Hinoki (檜) means cypress tree in Japanese language.

![Hinoki](http://upload.wikimedia.org/wikipedia/commons/7/7c/Chamaecyparis_obtusa2.jpg)
