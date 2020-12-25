#include "MyTree1.h"
#include <Windows.h>

MyTree1::MyTree1()
    : root(nullptr)
{  
}

MyTree1::~MyTree1()
{
}


void MyTree1::addNode(int value)
{
    if (root) {
        addNode(value, root);
    }
    else {
        root = new Node(value);
    }
}

void MyTree1::deleteNode(int value)
{
    //root = deleteNode(value, root);
    root = remove(root, value);
    setParents();
}

void MyTree1::clearTree()
{
    root = NULL;  
}


MyTree1::Node *MyTree1::search(int value)
{
    return search(value, root);
}

void MyTree1::addNode(int value, Node *leaf)
{
    if (value < leaf->value) {
        if (leaf->left){
            addNode(value, leaf->left);
        }
        else {
            leaf->left = new Node(value);
            leaf->left->parent = leaf;
        }
    }
    else {
        if (leaf->value != value) {
            if (leaf->right) {
                addNode(value, leaf->right);
            }
            else {
                leaf->right = new Node(value);
                leaf->right->parent = leaf;
            }
        }
    }
}

MyTree1::Node* MyTree1::remove(Node* tree, int data)
 {
    if (root) {
        Node head = { 0 };
        Node* it = &head;
        Node* q, * p=NULL, *f = NULL;
        int dir = 1;    

        it->right = root;

        while (dir == 1 ? (it->right) : (it->left)) {
            if (dir == 1 && it->rightTag)
                break;
            p = it;
            if (dir) {
                it = it->right;
            }
            else {
                it = it->left;
            }
            dir = it->value <= data;
            
            if (it->value == data)
                f = it;           
        }        
        if (f) {
            bool res = it->left == NULL;
            if (res) {
                q = it->right;
            }
            else {
                q = it->left;
            }
            dir = (p->right == it);
            f->value = it->value;
            
            if (p == q) {
                if (dir) {
                    p->right = NULL;
                }
                else {
                    p->left = NULL;
                }
            }
            else if (!it->left && it->rightTag) {
                p->rightTag = true;
                p->right = it->right;               
            }
            else if (!it->left) {
                if (dir) {
                    p->right = q;
                }
                else {
                    p->left = q;
                }
                //q->rightTag = p->rightTag;
            }
            else {
                if (q->right && it->rightTag && (!q->right->rightTag||q->rightTag)) {
                    q->rightTag = it->rightTag;
                    q->right = it->right;
                }
                else {
                    q->right->right = it->right;
                    q->right->rightTag = it->rightTag;
                }
                if (dir) {
                    p->right = q;
                }
                else {
                    p->left = q;
                }   
            }
            //free(it);          
        }      
        return head.right;     
    } 
    return NULL;
}


MyTree1::Node *MyTree1::deleteNode(int value, Node* leaf)
{
    if (!leaf)
        return leaf;
    if (value < leaf->value) {
        leaf->left = deleteNode(value, leaf->left);
    }
    else if (value > leaf->value && !leaf->rightTag) {
        leaf->right = deleteNode(value, leaf->right);
    }
    else if (leaf->left && leaf->right && !leaf->rightTag) {
        Node* nodeBuffer = leaf->left;
        while (nodeBuffer->right && !nodeBuffer->rightTag) {
            nodeBuffer = nodeBuffer->right;
        }
        leaf->value = nodeBuffer->value;
        if (leaf == root) {
            leaf->parent = NULL;
        }
        else {
            leaf->parent = nodeBuffer->parent;
        }
        leaf->rightTag = false;
        leaf->left = deleteNode(leaf->value, leaf->left);
        //if (leaf == leaf->left) {
        //    leaf->left = NULL;
        //    leaf->rightTag = false;
        //}
    }
    else {
        if (leaf->left) {
            leaf->left->parent = leaf->parent;
            leaf = leaf->left;
        }
        else if (leaf->right) {
            if (leaf != root && leaf->rightTag) {
                leaf->parent->rightTag = true;
                Node* buffer = leaf;
                while (buffer != root && buffer == buffer->parent->right) {
                    buffer = buffer->parent;
                }
                if (buffer != root)
                    buffer = buffer->parent;
                leaf = buffer;           
            }
            else {               
                leaf = leaf->right;
            }
            
        }
        else {
            leaf = nullptr;
        }
    }
    return leaf;
}

MyTree1::Node *MyTree1::search(int value, Node *leaf)
{
    if (leaf){
        if (value > leaf->value) {
            if (!leaf->rightTag) {
                return search(value, leaf->right);
            }
            else
                return NULL;
        }
        if (value < leaf->value) {
            return search(value, leaf->left);
        }
        else
            return leaf;
    }
    else
        return NULL;
}

void MyTree1::performTree(Node* leaf)
{
    if (leaf) {
        performTree(leaf->left);
        if (!leaf->right) {
            if (leaf != root) {
                if (leaf == leaf->parent->left) {
                    leaf->right = leaf->parent;
                }
                else if (leaf == leaf->parent->right) {
                    Node* buffer = leaf;
                    while (buffer != root && buffer == buffer->parent->right) {
                        buffer = buffer->parent;
                    }
                    if (buffer != root)
                        buffer = buffer->parent;
                    leaf->right = buffer;
                }
                leaf->rightTag = true;
            }           
        }
        
        if (!leaf->rightTag)
            performTree(leaf->right);
    }
}

void MyTree1::setParents()
{
    setParents(root);
}

void MyTree1::setParents(Node* leaf)
{
    if (leaf != NULL) {
        if (leaf->left) {
            leaf->left->parent = leaf;
        }
        if (leaf->right) {
            leaf->right->parent = leaf;
        }
        setParents(leaf->left);
        if (!leaf->rightTag) {
            setParents(leaf->right);
        }
    }
}

void MyTree1::directBypass()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!root) {
        std::cout << "NO NODES" << std::endl;
    }
    else if (!root->left && !root->right) {
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << root->value << std::endl;
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
    }
    else {
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << root->value << " ";
        directBypass(root->left);
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << root->value << " ";
        directBypass(root->right);
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << root->value << " ";
    }
    SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
}

void MyTree1::reversiveBypass()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!root) {
        std::cout << "NO NODES" << std::endl;
    }
    else if (!root->left && !root->right) {
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << root->value << std::endl;
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
    }
    else {
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << root->value << " ";
        reversiveBypass(root->left);
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << root->value << " ";
        reversiveBypass(root->right);
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << root->value << " ";
    }
    SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
}

void MyTree1::symmetricBypass()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!root) {
        std::cout << "NO NODES" << std::endl;
    }
    else if (!root->left && !root->right) {
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << root->value << std::endl;
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
    }
    else {
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << root->value << " ";
        symmetricBypass(root->left);
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << root->value << " ";
        symmetricBypass(root->right);
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << root->value << " ";
            
    }
    SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
}

void MyTree1::showTree()
{
    if (!root) {
        std::cout << "Tree is empty" << std::endl;
    }
    else {
        std::cout << "\n\nTree view:------------------------------------------------------------------------------------------------------------------\n";
        std::vector<std::pair<Node*, int>> leveledNodes;
        int rightLevel = root->right ? countLevel(root->right, 0) : 0;
        int leftLevel = root->left ? countLevel(root->left, 0) : 0;
        int level = rightLevel > leftLevel ? rightLevel : leftLevel;

        leveledNodes.push_back({ root, 0 });

        for (int i = 1; i < level; i++) {
            int tempSize = leveledNodes.size();
            for (int j = pow(2, i - 1) - 1; j < tempSize; j++) {
                if (leveledNodes.at(j).first) {
                    if (leveledNodes.at(j).first->left)
                        leveledNodes.push_back({ leveledNodes.at(j).first->left, i });
                    else
                        leveledNodes.push_back({ NULL, i });
                    if (leveledNodes.at(j).first->right && !leveledNodes.at(j).first->rightTag)
                        leveledNodes.push_back({ leveledNodes.at(j).first->right, i });
                    else
                        leveledNodes.push_back({ NULL, i });
                }
                else {
                    leveledNodes.push_back({ NULL, i });
                    leveledNodes.push_back({ NULL, i });
                }
            }
        }
        printTree(&leveledNodes, level + 1);
        std::cout << "\n----------------------------------------------------------------------------------------------------------------------------" << std::endl;
    }
}

void MyTree1::printTree(std::vector<std::pair<Node*, int>>* leveledArray, int level) {
    std::vector<int> prevAmounts;
    prevAmounts.push_back(level * level * 2);

    for (int j = 0; j < prevAmounts.at(0); j++) {
        std::cout << " ";
    }
    
    printf("%d", leveledArray->at(0).first->value);
    int balance = 0;

    for (int i = 1; i < leveledArray->size(); i++) {

        if (leveledArray->at(i - 1).second < leveledArray->at(i).second) {
            std::cout << "\n\n\n";
            balance = 0;
            
            prevAmounts.push_back(prevAmounts.at(leveledArray->at(i).second-1)/2);
            for (int j = 0; j < prevAmounts.at(leveledArray->at(i).second); j++) {
                std::cout << " ";
            }
        }
       
        else {
            auto buff = pow(2, leveledArray->at(i - 1).second);
            double num = i;
            if ((num - 1 <= round(buff * 3 / 4) && num <= round(buff * 3 / 4))  || (num - 1 > round(buff * 3 / 4) && num > round(buff * 3 / 4))) {
                    for (int j = 0; j < prevAmounts.at(leveledArray->at(i).second-1)-balance; j++) {
                        std::cout << " ";
                    }
            }
            else {
                    for (int j = 0; j < prevAmounts.at(leveledArray->at(i).second-1)-balance; j++) {
                        std::cout << " ";
                    }

            }
            balance = 0;
        }

        if (leveledArray->at(i).first) {
            printf("%d", leveledArray->at(i).first->value);
            if (leveledArray->at(i).first->rightTag) {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
                printf("-(%d)", leveledArray->at(i).first->right->value);
                SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
                balance = 4;
            }
        }
        else {
            printf(" ");
        }

    }
}

int MyTree1::countLevel(Node* leaf,  int level) {
    if (leaf) {
        int rightLevel = 0; int leftLevel = 0;
        if (!leaf->rightTag)
            rightLevel = countLevel(leaf->right, level+1)+1;
        leftLevel = countLevel(leaf->left, level+1)+1;
        return rightLevel > leftLevel ? rightLevel : leftLevel;
    }
    else {
        return 1;
    }
}

MyTree1::Node* MyTree1::getTree()
{
    return root;
}

void MyTree1::performTree()
{
    performTree(root);
}

void MyTree1::directBypass(Node* leaf)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
    if (leaf) {
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << leaf->value << " ";
        //if (leaf->rightTag) {
        //    std::cout << "|(" << leaf->right->value << ") ";
        //    SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        //}
        directBypass(leaf->left);
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << leaf->value << " ";
        if (!leaf->rightTag)
            directBypass(leaf->right);
        else
            std::cout << 0 << " ";
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << leaf->value << " ";
    }
    else {
        std::cout << 0 << " ";
    }
}

void MyTree1::reversiveBypass(Node* leaf)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
    if (leaf) {
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << leaf->value << " ";
        reversiveBypass(leaf->left);
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << leaf->value << " ";
        if (!leaf->rightTag)
            reversiveBypass(leaf->right);        
        else {           
            std::cout << 0 << " ";
        }
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << leaf->value << " ";
        //if (leaf->rightTag) {
        //    std::cout << "|(" << leaf->right->value << ") ";
        //    SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        //}
    }
    else {
        std::cout << 0 << " ";
    }
}

void MyTree1::symmetricBypass(Node* leaf)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
    if (leaf != NULL) {
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << leaf->value << " ";
        symmetricBypass(leaf->left);
        SetConsoleTextAttribute(hConsole, (WORD)(0 << 4 | 4));
        std::cout << leaf->value << " ";
        if (!leaf->rightTag) {
            symmetricBypass(leaf->right);
        }
        else {
            std::cout << "-(" << leaf->right->value << ") ";
            SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
            std::cout << 0 << " ";
        }
        SetConsoleTextAttribute(hConsole, (WORD)(0 | 15));
        std::cout << leaf->value << " ";
    }
    else {
        std::cout << 0 << " ";
    }
}
