#include <iostream>
#include "MyTree1.h"

int main()
{
    std::system("chcp 1251");
    MyTree1 tree;

    char choise = 0;

    while (choise!='0') {
        std::cout << "\nВыберите действие:\n1 - Добавить элемент\n2 - Удалить элемент\n3 - Найти элемент\n4 - Прямой обход\n5 - Реверсивный обход \n6 - Симметричный обход"
            "\n7 - Вывести дерево\n8 - Прошить дерево\n9 - Загрузить тестовое дерево\n0 - Выход" << std::endl;
        
        std::cin >> choise;
        std::cout << std::endl;
        
        switch (choise)
        {
        case '0': { 
            break;
        }
        case '1': {
            int value;
            std::cout << "Введите значение: ";
            std::cin >> value;
            tree.addNode(value);
            break;
        }
        case '2': {
            int value;
            std::cout << "Введите значение: ";
            std::cin >> value;
            tree.deleteNode(value);
            break;
        }
        case '3': {
            int value;
            std::cout << "Введите значение: ";
            std::cin >> value;
            if (tree.search(value)) {
                std::cout << "Значение найдено\n\n";
            }
            else {
                std::cout << "Значение не найдено\n\n";
            }
            break;
        }
        case '4': {
            std::cout << "Прямой обход: ";
            tree.directBypass();
            break;
        }
        case '5': {
            std::cout << "Реверсивный обход: ";
            tree.reversiveBypass();
            break;
        }
        case '6': {
            std::cout << "Симметричный обход: ";
            tree.symmetricBypass();
            break;
        }
        case '7': {
            tree.showTree();
            break;
        }
        case '8': {
            tree.performTree();
            break;
        }
        case '9': {
            tree.clearTree();
            tree.addNode(6);
            tree.addNode(4);
            tree.addNode(3);
            tree.addNode(1);
            tree.addNode(5);
            tree.addNode(2);
            tree.addNode(8);
            tree.addNode(9);
            tree.addNode(7);
            break;
        }
        }
    }
    
    return 0;
}
