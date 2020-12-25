#include <iostream>
#include "Graph.h"

int main()
{
    Graph graph;

    char choise = 0;
    while (choise != 'e') {
        std::cout << "Choose action\n1 - Show graph\n2 - Show ways\n3 - Show center\n4 - Load graph example\n5 - Enter graph\n6 - Add new Point\n7 - Set new cost to point\ne - Exit" << std::endl;
        std::cin >> choise;
        std::cout << std::endl;
        switch (choise)
        {
        case '1': {
            graph.showGraph();
            break;
        }
        case '2': {
            int dest, src;
            std::cout << "Enter source point: ";
            std::cin >> src;
            std::cout << "Enter destination point: ";
            std::cin >> dest;
            graph.showWaysBetweenPoints(src, dest);
            break;
        }
        case '3': {
            graph.showCenter();
            break;
        }
        case '4': {
            graph.setupGraphExample();
            break;
        }
        case '5': {
            int numOfPoints = 0;
            std::cout << "Enter number of points: ";
            std::cin >> numOfPoints;
            graph.fillGraph(numOfPoints);
            break;
        }
        case '6': {
            graph.addPoint();
            break;
        }
        case '7': {
            int col, row, value;
            std::cout << "Enter row: ";
            std::cin >> col;
            std::cout << "Enter column: ";
            std::cin >> row;
            std::cout << "Enter new cost: ";
            std::cin >> value;
            graph.changeCost(col-1, row-1, value);
            break;
        }
        }
        std::cout << std::endl;
    }
}