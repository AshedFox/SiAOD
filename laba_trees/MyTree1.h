#ifndef MyTree1_H
#define MyTree1_H

#include <iostream>
#include <vector>


class MyTree1
{
public:
    struct Node {
        int value;
        Node* left;
        Node* right;
        Node* parent;
        bool rightTag;

        Node(int _value) : value(_value), left(nullptr), right(nullptr), parent(nullptr), rightTag(false) {}
    };

    MyTree1();
    ~MyTree1();

    void addNode(int value);
    void deleteNode(int value);
    void clearTree();

    Node* search(int value);

    void directBypass();
    void reversiveBypass();
    void symmetricBypass();

    void showTree();

    Node* getTree();

    void performTree();

private:

    Node* root;

    void addNode(int value, Node* leaf);
    Node* deleteNode(int value, Node* leaf);
    Node* search(int value, Node* leaf);

    Node* remove(Node* tree, int data);

    int countLevel(Node* leaf, int level);
    void printTree(std::vector<std::pair<Node*, int>>* leveledArray, int level);

    void performTree(Node* leaf);
    void setParents();
    void setParents(Node* leaf);

    void directBypass(Node* leaf);
    void reversiveBypass(Node* leaf);
    void symmetricBypass(Node* leaf);
};

#endif // MyTree1_H
