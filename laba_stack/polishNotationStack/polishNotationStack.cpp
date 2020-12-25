#include <iostream>
#include <string>
#include "stack.h"

struct ExprNode {
    char symbol;
    int priority;
    int stackPriority;
    ExprNode(char _symbol, int _priority, int _stackPriority)
        : symbol(_symbol),
          priority(_priority),
          stackPriority(_stackPriority) {}
};

Stack<char> transformToBackPolish(Stack<char> input) {
    Stack<char> result;
    std::vector<ExprNode> tempExpr;
    while (input.getSize() !=0) {
        ExprNode temp = { input.Pop(), 7 , 8};
        switch (temp.symbol)
        {
        case '+':
        case '-': {
            temp.priority = 1;
            temp.stackPriority = 2;
            break;
        }
        case '*':
        case '/': {
            temp.priority = 3;
            temp.stackPriority = 4;
            break;
        }
        case '^': {
            temp.priority = 6;
            temp.stackPriority = 5;
            break;
        }
        case '(': {
            temp.priority = 9;
            temp.stackPriority = 0;
            break;
        }
        case ')': {
            temp.priority = 0;
            temp.stackPriority = 0;
            break;
        }
        }
        if (tempExpr.size() != 0) {
            if (temp.symbol == ')') {
                int j = tempExpr.size()-1;
                while (j >= 0 && tempExpr.at(j).symbol != '(') {
                    result.Push(tempExpr.at(j).symbol);
                    tempExpr.pop_back();
                    j--;
                }
                if (!tempExpr.empty())
                    tempExpr.pop_back();
            }
            else {
                if (temp.priority < tempExpr.back().stackPriority) {
                    while (!tempExpr.empty() && temp.priority < tempExpr.back().stackPriority){
                        result.Push(tempExpr.back().symbol);
                        tempExpr.pop_back();
                    }
                    tempExpr.push_back(temp);
                }
                else {
                    tempExpr.push_back(temp);
                }
            }
        }
        else
            tempExpr.push_back(temp);
    }
    while (!tempExpr.empty()) {
        if (tempExpr.back().symbol != '(') {
            result.Push(tempExpr.back().symbol);
        }
        tempExpr.pop_back();
    }
    return result;
}

int CountExprRank(std::vector<char> input) {
    int rank = 0;
    for (int i = 0; i < input.size(); i++) {
        switch (input.at(i)) {
        case'+':
        case'-':
        case'*':
        case'/':
        case'^': {
            rank--;
            break;
        }
        default: {
            rank++;
            break;
        }
        }
    }
    return rank;
}

int main()
{
    std::string input;

    std::cout << "Enter expression: ";
    std::cin >> input;

    "((a+b+c)*d)^f^g";
    "(a+b+c*(y-g*d)^n^k+s*l)/(x-f*t*p+w)";

    std::vector<char> buff{input.crbegin(), input.crend()};

    Stack<char> stack{ buff };

    Stack<char> result = transformToBackPolish(stack);

    buff = result.getValues();

    std::cout << "Expression: ";
    for (int i = 0; i < buff.size(); i++) {
        std::cout << buff.at(i);
    }
    int rank = CountExprRank(buff);
    std::cout << "\nRank: " << rank << std::endl;

    return 0;
}

