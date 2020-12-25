#include "Graph.h"
#include <iostream>
#include <algorithm>

const int inf = 999;

Graph::Graph()
{
}

void Graph::findFloyd()
{
    int n = graphMatrix.size();
    shortestGraphWays = graphMatrix;

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (shortestGraphWays.at(i).at(k) && shortestGraphWays.at(k).at(j) && i != j) {
                    if (shortestGraphWays.at(i).at(j) > shortestGraphWays.at(i).at(k) + shortestGraphWays.at(k).at(j) || !shortestGraphWays.at(i).at(j)) {
                        shortestGraphWays.at(i).at(j) = shortestGraphWays.at(i).at(k) + shortestGraphWays.at(k).at(j);
                    }
                }
            }
        }
    }
   
}

void Graph::showGraph()
{
    if (graphMatrix.size()) {
        std::cout << "   ||";
        for (int i = 0; i < graphMatrix.size(); i++) {
            printf(" %3d|", i + 1);
        }
        std::cout << std::endl;
        for (int i = 0; i < graphMatrix.size()+1; i++) {
            std::cout << "-----";
        }
        std::cout << std::endl;
        for (int i = 0; i < graphMatrix.size(); i++) {
            printf(" %2d||", i + 1);

            for (int j = 0; j < graphMatrix.size(); j++) {
                if (graphMatrix.at(i).at(j) != inf) {
                    printf(" %3d|", graphMatrix.at(i).at(j));
                }
                else {
                    std::cout << " inf|";
                }
            }
            std::cout << std::endl;
        }
    }
    else {
        std::cout << "Graph is empty";
    }
    std::cout << std::endl;
}

void Graph::showCenter()
{
    if (graphMatrix.size()) {
        if (shortestGraphWays.size() != graphMatrix.size()) {
            findFloyd();
        }
        findCenter();
    }
    else {
        std::cout << "Graph is empty" << std::endl;
    }
}

void Graph::findCenter()
{
    if (graphMatrix.size()) {
        int n = shortestGraphWays.size();
        std::vector<int> extr;
        extr.resize(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (shortestGraphWays.at(i).at(j) != 0) {
                    if (shortestGraphWays.at(i).at(j) > extr.at(j)) {
                        extr.at(j) = shortestGraphWays.at(i).at(j);
                    }
                }
            }
        }
        int minExtr = 0;
        for (int i = 0; i < extr.size(); i++) {
            if (extr.at(i) < extr.at(minExtr)) {
                minExtr = i;
            }
        }
        showFloyd();
        std::cout << "Center: " << minExtr + 1 << " with eccentricity = " << extr.at(minExtr) << std::endl;
    }
    else {
        std::cout << "Graph is empty";
    }
}

void Graph::showFloyd()
{
    if (shortestGraphWays.size() != graphMatrix.size()) {
        findFloyd();
    }
    int n = shortestGraphWays.size();
    for (int i = 0; i < n; i++) {
        std::cout << i + 1 << '\t';
    }
    std::cout << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << "-------";
    }
    std::cout << std::endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) {
            if (shortestGraphWays.at(i).at(j) < inf) {
                std::cout << shortestGraphWays.at(i).at(j) << "\t";
            }
            else {
                std::cout << "inf\t";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Graph::findRoute(int n, Way* way, int& dest)
{
   if (n == dest) {
        ways.push_back(way);
   }
   else {
       for (int i = 0; i < graphMatrix.size(); i++) {
           if (graphMatrix.at(n).at(i) != inf && way->used.find(i) == way->used.end()) {
               Way newWay = { way };
               newWay.name += ", " + std::to_string(i + 1);
               newWay.used.insert(i);
               newWay.cost += graphMatrix.at(n).at(i);
               findRoute(i, &newWay, dest);
           }
       }
   }
}

void Graph::sortWays()
{
    if (!ways.empty()) {
        for (int i = 0; i < ways.size()-1; i++) {
            for (int j = i + 1; j < ways.size(); j++) {
                if (ways.at(j).cost < ways.at(i).cost) {
                    std::swap(ways.at(j), ways.at(i));
                }
            }
        }
    }
}

void Graph::showWaysBetweenPoints(int src, int dest)
{
    if (src <= graphMatrix.size() && dest <= graphMatrix.size() && src > 0 && dest > 0) {
        ways.clear();
        src--;
        dest--;
        Way baseWay{ std::to_string(src + 1), {src}, 0 };
        findRoute(src, &baseWay, dest);

        sortWays();
        src++;
        dest++;
        if (!ways.empty()) {
            std::cout << "ALL WAYS FROM " << src << " TO " << dest << std::endl;
            for (int i = 0; i < ways.size(); i++) {
                std::cout << "   Way " << i + 1 << ": " << ways.at(i).name << " with cost = " << ways.at(i).cost << std::endl;
            }
            std::cout << "SHORTEST WAY FROM " << src << " TO " << dest << " is " << ways.front().name << " with cost = " << ways.front().cost << std::endl;
            std::cout << "LONGEST WAY FROM " << src << " TO " << dest << " is " << ways.back().name << " with cost = " << ways.back().cost << std::endl;
        }
        else {
            std::cout << "NO WAYS FROM " << src << " TO " << dest << std::endl;
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "POINT(S) OUT OF GRAPH" << std::endl;
    }
}

void Graph::setupGraphExample()
{
    graphMatrix = { {  0,    inf,  4,inf,  6},
                    {  inf,    0,  5,  7,inf},
                    {    2,    3,  0,inf,  4},
                    {  inf,    4,  3,  0,inf},
                    {  inf,    9,inf,  2,  0} };
}

void Graph::fillGraph(int numOfPoints)
{
    graphMatrix.clear();
    graphMatrix.resize(numOfPoints);
    int buff;
    std::cout << "To set infinity way enter 999" << std::endl;
    for (int i = 0; i < numOfPoints; i++) {
        for (int j = 0; j < numOfPoints; j++) {
            std::cout << "Enter cost of way from " << i + 1 << " to " << j + 1 << ": ";
            std::cin >> buff;
            graphMatrix.at(i).push_back(buff);
        }
    }
}

void Graph::addPoint()
{
    graphMatrix.resize(graphMatrix.size() + 1);
    int buff;
    std::cout << "To set infinity way enter 999" << std::endl;
    for (int i = 0; i < graphMatrix.size()-1; i++) {
        std::cout << "Enter cost of way from " << i + 1 << " to " << graphMatrix.size() << ": ";
        std::cin >> buff;
        graphMatrix.at(i).push_back(buff);
    }
    for (int i = 0; i < graphMatrix.size(); i++) {
        std::cout << "Enter cost of way from " << graphMatrix.size() << " to "  << i + 1 << ": ";
        std::cin >> buff;
        graphMatrix.at(graphMatrix.size()-1).push_back(buff);
    }
}

void Graph::changeCost(int col, int row, int value)
{
    if (col < graphMatrix.size() && row < graphMatrix.size()) {
        graphMatrix.at(col).at(row) = value;
    }
}
