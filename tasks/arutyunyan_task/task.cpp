/*  ����� ������ ����� ����� (��� int). ����� �����, ���� �� � ������� ����� �����, ��� ���������� ����� ������� ������� ����� ����� ��������� �����. ������� ����� ������ ���� ������������.
�������:
[1,1,1,3] - ����� 3 (����� 3 ����� ������ ����)
[1,3,1,1] - ����� 3
[0,3,1,2] - ����� 3 (���� ����� ������ �������, ��� ����� ������ ������ � ��� ����� ������ ������, �� ������ ������������ �� ���, ������� ����� ���)
[12,1,6] - ����� 0
[1] - ����� 0
[1,1] - ����� 0
*/

#include <functional>
#include <algorithm>
#include <vector>
#include <set>
#include <cassert>
#include <iostream>
#include "measure.h"

namespace SimpleSolution
{
	int32_t run(std::vector<int32_t> v)
	{
		if (v.size() < 2)
		{
			return 0;
		}

		std::sort(v.begin(), v.end());

		for (auto index = v.size() - 1; index > 0; --index)
		{
			if (v[index] == index && v[index - 1] != index)
			{
				return v[index];
			}
		}

		return 0;
	}
}

namespace BinaryHeapSolution
{
	int32_t run(std::vector<int32_t> v)
	{
		if (v.size() < 2)
		{
			return 0;
		}

		std::make_heap(v.begin(), v.end());

		while (v.size() > 2)
		{
			if (v[0] == v.size() - 1 && v[0] != v[1] && v[0] != v[2])
			{
				return v[0];
			}
			std::pop_heap(v.begin(), v.end());
			v.pop_back();
		}

		if ((v[0] != v[1]) && (v[0] == 1 || v[1] == 1)) return 1;

		return 0;
	}
}

namespace MultisetSolution
{
	int32_t run(const std::vector<int32_t>& values)
	{
		if (values.size() < 2)
		{
			return 0;
		}

		// O(n*log(n))
		std::multiset<int32_t, std::greater<int32_t>> sortValues(values.cbegin(), values.cend());

		// O(n)
		int prevElemCount = sortValues.size() - 1;
		for (auto it = sortValues.begin(); it != sortValues.end(); ++it, --prevElemCount)
		{
			auto isLast(prevElemCount == 0);
			auto nextIt = std::next(it);
			if (*it == prevElemCount && (isLast || *it != *nextIt))
			{
				return *it;
			}
		}
		return 0;
	}
};

namespace IndexOrientedSolution
{
	int32_t run(const std::vector<int32_t>& values)
	{
		if (values.size() < 2)
		{
			return 0;
		}

		auto size(values.size());
		auto totalCount(size);
		std::vector<int32_t> valueCounts(size);
		// O(n)
		for (auto val : values)
		{
			if (val < 0 || val < size)
			{
				val = val < 0 ? 0 : val;
				++valueCounts[val];
			}
			else
			{
				--totalCount;
			}
		}

		// O(n)
		for (auto i = valueCounts.size() - 1; i > 0; --i)
		{
			if (valueCounts[i] > 0)
			{
				totalCount -= valueCounts[i];
				if (i == totalCount)
				{
					return i;
				}
			}
		}
		return 0;
	}
};


int main()
{
	// correctness tests
	std::vector<std::vector<int32_t>> testValues = {
		{ 4, 5, 3, 3, 3, 3, 3, 7, 7 },
		{ 5, -1, 4, 7, -1, 4, 17, 6, 1, 3 },
		{ -1 },
		{ 0 },
		{ -4, -3, -2, -1, 5, 5, 5, -1 },
		{ 1, 1, 1, 1, 1 },
		{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
		{ 17, 17, 17 }
	};

	std::vector<int32_t> testResults = { 7, 4, 0, 0, 5, 0, 10, 0 };

	for (auto i = 0; i < testValues.size(); ++i)
	{
		auto s_v = SimpleSolution::run(testValues[i]);
		assert(s_v == testResults[i]);
		auto m_v = MultisetSolution::run(testValues[i]);
		assert(m_v == testResults[i]);
		auto b_v = BinaryHeapSolution::run(testValues[i]);
		assert(b_v == testResults[i]);
		auto i_v = IndexOrientedSolution::run(testValues[i]);
		assert(i_v == testResults[i]);
	}

	// performance tests
	testValues.clear();
	std::vector<int32_t> bigVector;
	for (int i = 0; i < 10000000; ++i)
		bigVector.push_back(i);

	// ascending order
	testValues.push_back(bigVector);
	
	// descending order
	std::reverse(bigVector.begin(), bigVector.end());
	testValues.push_back(bigVector);

	// random order
	std::random_shuffle(bigVector.begin(), bigVector.end());
	testValues.push_back(bigVector);

	// randor order
	std::random_shuffle(bigVector.begin(), bigVector.end());
	testValues.push_back(bigVector);

	// randor order
	std::random_shuffle(bigVector.begin(), bigVector.end());
	testValues.push_back(bigVector);

	// randor order
	std::random_shuffle(bigVector.begin(), bigVector.end());
	testValues.push_back(bigVector);
	for (auto i = 0; i <  testValues.size(); ++i)
	{
		std::cout << "\nTest #" << i << "\n";
		auto time = measure::execution(SimpleSolution::run, testValues[i]);
		std::cout << "SimpleSolution: " << time << "s \n";
		time = measure::execution(MultisetSolution::run, testValues[i]);
		std::cout << "MultisetSolution: " << time << "s \n";
		time = measure::execution(BinaryHeapSolution::run, testValues[i]);
		std::cout << "BinaryHeapSolution: " << time << "s \n";
		time = measure::execution(IndexOrientedSolution::run, testValues[i]);
		std::cout << "IndexOrientedSolution: " << time << "s \n";
	}
}