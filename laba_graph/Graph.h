#pragma once
#include <vector>
#include <list>
#include <string>
#include <set>

class Graph
{	
	struct Way {
		std::string name;
		std::set<int> used;
		int cost;
		Way() : name(""), used(), cost(999) {}
		Way(std::string _name, std::set<int> _used, int _cost) : name{ _name }, used{ _used }, cost{ _cost } {}
		Way(Way* _way) : name{ _way->name }, used{ _way->used }, cost{ _way->cost } {}
	};

	std::vector<std::vector<int>> graphMatrix;
	std::vector<std::vector<int>> shortestGraphWays;
	std::vector<Way> ways;

public:
	Graph();

	void showGraph();
	void showCenter();
	void showFloyd();
	void showWaysBetweenPoints(int src, int dest);
	void setupGraphExample();
	void fillGraph(int numOfPoints);
	void addPoint();
	void changeCost(int col, int row, int value);

private:
	void findFloyd();
	void findCenter();
	void findRoute(int n, Way* way, int &dest);
	void sortWays();

};

