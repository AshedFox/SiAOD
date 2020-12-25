#include "Queue.h"
//#include <algorithm>
#include <iostream>


Queue::Queue(const list<pair<list<int>, int>> _processes) 
	: tickTime(0), inputTime(0), totalTicks(0), totalTheoryProcTime(0)
{
	for (auto& el : _processes) {
		for (auto subel : el.first) {
			totalTheoryProcTime += subel;
		}
		enqueue(el);
	}
}

Queue::Queue(const list<pair<list<int>, int>> _processes, int tickTime, int inputTime)
	: tickTime(tickTime), inputTime(inputTime), totalTicks(0), totalTheoryProcTime(0)
{
	for (auto& el : _processes) {
		for (auto subel : el.first) {
			totalTheoryProcTime += subel;
		}
		enqueue(el);
	}
}

void Queue::enqueue(const pair<list<int>, int > process)
{
	auto insertPos = processes.begin();
	if (!processes.empty()) {
		for (auto& el : processes) {
			if (process.second < el.priority) {
				break;
			}
			insertPos++;
		}
		processes.insert(insertPos, process);
	}
	else
		processes.push_back(process);
}

void Queue::enqueue(const Element process)
{
	auto insertPos = processes.begin();
	if (!processes.empty()) {
		for (auto& el : processes) {
			if (process.priority < el.priority) {
				break;
			}
			insertPos++;
		}
		processes.insert(insertPos, process);
	}
	else
		processes.push_back(process);
}

int Queue::dequeue()
{
	int result = 0;
	if (!processes.front().waitTime) {
		if (processes.front().processingPeriods.front() > tickTime) {
			result = tickTime;
			/*printf("|%d", processes.front().priority);
			for (int i = 0; i < result; i++) {
				printf("L");
			}
			printf("|");*/
			processes.front().processingPeriods.front() -= tickTime;
		}
		else {
			result = processes.front().processingPeriods.front();
			/*printf("|%d", processes.front().priority);
			for (int i = 0; i < result; i++) {
				printf("L");
			}*/
			processes.front().waitTime = inputTime;
			processes.front().processingPeriods.pop_front();
		}
	}
	return result;
}

void Queue::swapBetweenDequeue() {
	if (processes.size() > 1) {
		auto bIter = processes.begin();
		for (auto iter = ++processes.begin(); iter != processes.end(); ++iter) {
			if (((*iter).priority <= (*bIter).priority || (*bIter).waitTime != 0) && ((*iter).waitTime == 0)) {
				swap(*bIter, *iter);
				auto el = *iter;
				if ((*iter).waitTime == 0 || (*iter).priority == (*bIter).priority) {
					processes.erase(iter);
					enqueue(el);
				}
				break;
			}
		}
	}
}

bool Queue::isEmpty()
{
	return processes.empty();
}

void Queue::tick()
{
	int input = dequeue();
	bool isAnyProcessed = false;
	if (processes.front().waitTime) {
		if (processes.front().waitTime - (tickTime - input) > 0) {
			/*for (int i = 0; i < (tickTime - input); i++) {
				printf("b");
			}
			printf("|");*/
			processes.front().waitTime = (processes.front().waitTime - (tickTime - input));
		}
		else {
			/*printf("|");
			for (int i = 0; i < tickTime; i++) {
				printf("b");
			}
			printf("|");*/
			processes.front().waitTime = 0;
		}
	}
	for (auto it = ++processes.begin(); it != processes.end(); it++) {
		if ((*it).waitTime > 0) {
			if ((*it).waitTime <= tickTime) {
				(*it).waitTime = 0;
			}
			else {
				(*it).waitTime -= tickTime;
			}
		}
	}
	if (processes.front().processingPeriods.empty()) {
		processes.pop_front();
		if (!isEmpty()) {
			if (processes.front().waitTime) {
				swapBetweenDequeue();
			}
		}
	}
	else
		swapBetweenDequeue();
	++totalTicks;

}

void Queue::processAllQueue()
{
	while (!isEmpty()) {
		tick();
	}
}

void Queue::getTaskOutput()
{
	printf("%3d|%3d|", totalTheoryProcTime, totalTicks * tickTime);
	printf("%4.2f%%", (float)totalTheoryProcTime*100 / (totalTicks * tickTime));
}
