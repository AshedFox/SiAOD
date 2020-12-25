#pragma once
#include <list>
#include <vector>

using namespace std;

class Queue
{
	int tickTime, inputTime;
	int totalTicks, totalTheoryProcTime;

	struct Element{
		std::list<int> processingPeriods;
		int priority;
		int waitTime;

		Element() : priority(1), waitTime(0) {}
		Element (list<int> _processingPeriods, int _priority = 1) : processingPeriods(_processingPeriods), priority(_priority), waitTime(0) {}
		Element (pair<list<int>, int> process) : processingPeriods(process.first), priority(process.second), waitTime(0) {}
	};

	list<Element> processes;

	void swapBetweenDequeue();

	void tick();

public:
	Queue() : tickTime(0), inputTime(0), totalTicks(0), totalTheoryProcTime(0) {}
	Queue(const list<pair<list<int>, int>> _processes);
	Queue(const list<pair<list<int>, int>> _processes, int tickTime, int inputTime);

	void enqueue(const pair<list<int>, int> process);
	void enqueue(const Element process);

	int dequeue();

	bool isEmpty();

	void processAllQueue();

	void getTaskOutput();
};

