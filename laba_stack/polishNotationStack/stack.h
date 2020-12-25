#pragma once
#include <vector>

template<typename T>
class Stack
{
	std::vector<T> values;
	int top;

public:
	Stack() : top{ 0 } {}

	Stack(std::vector<T> _values) : values{_values}, top{(int)_values.size()} {}

	void Push(T& value) {
		values.push_back(value);
		top++;
	}

	T Pop() {
		if (!isEmpty()) {
			T buff = values.back();
			values.pop_back();
			top--;
			return buff;
		}
		return NULL;
	}

	T Top() {
		return isEmpty() ? NULL
							  : values.back();
	}

	int getSize() {
		return values.size();
	}

	std::vector<T> getValues() {
		return values;
	}

	bool isEmpty() {
		return values.empty();
	}
};
