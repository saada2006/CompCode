#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <stack>

/*
Possible optimizations: use previous splits
Also use bools instead of chars since D and K can be replaced with true or false
*/

// Pair: D - first, K - second

std::pair<uint32_t, uint32_t> GetRatio(const std::string& Str, uint32_t Begin, uint32_t End) {
	std::pair<uint32_t, uint32_t> Ratio;

	Ratio.first = Ratio.second = 0;

	for (uint32_t Idx = Begin; Idx < End; Idx++) {
		char C = Str[Idx];
		if (C == 'D') {
			Ratio.first++;
		} else {
			Ratio.second++;
		}
	}

	return Ratio;
}

bool CheckRatios(const std::pair<uint32_t, uint32_t> LHS, const std::pair<uint32_t, uint32_t> RHS) {
	return ((RHS.first * LHS.second) == (RHS.second * LHS.first));
}

// TODO: previous split caching
uint32_t ValidateSplits(const std::string& Str, const std::vector<uint32_t>& Splits) {
	/*
	Test consecutive pairs 
	*/

	if (Str.size() == 0) {
		return 0;
	}

	uint32_t PreviousSplit = 0;
	for (uint32_t SplitIndex = 0; SplitIndex < Splits.size(); SplitIndex++) {
		uint32_t CurrentSplitStart = Splits[SplitIndex], CurrentSplitEnd;
		if (SplitIndex + 1 == Splits.size()) {
			CurrentSplitEnd = Str.size();
		}
		else {
			CurrentSplitEnd = Str[SplitIndex + 1];
		}

		std::pair<uint32_t, uint32_t> R0 = GetRatio(Str, PreviousSplit    , CurrentSplitStart);
		std::pair<uint32_t, uint32_t> R1 = GetRatio(Str, CurrentSplitStart, CurrentSplitEnd  );

		if (!CheckRatios(R0, R1)) {
			return 0;
		}
	}

	return 1;
}

int main() {
	std::ifstream in("Input.txt");
	std::streambuf* cinbuf = std::cin.rdbuf(); //save old buf
	std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!


	// 1. Get the number of test cases
	uint32_t NumTestCases = 0;
	std::cin >> NumTestCases;

	// 2. Get each test case
	std::vector<std::string> TestCases;
	TestCases.reserve(NumTestCases);
	for (uint32_t Counter = 0; Counter < NumTestCases; Counter++) {
		uint32_t WoodSize = 0;
		std::cin >> WoodSize;

		std::string WoodString;
		WoodString.reserve(WoodSize);
		std::getline(std::cin, WoodString);

		TestCases.emplace_back(std::move(WoodString));

		std::cout << WoodString << '\n';
	}

	// 3. Compute the solution and exit 
	for (const std::string& Test : TestCases) {
		size_t NumSolutions = 1;

		std::string PrefixedString;
		PrefixedString.reserve(Test.size());

		for (const char C : Test) {
			PrefixedString += C;

			/*
			Test each split

			A basic splitting algorithm works like this:
			We can first iterate through the boundries of the string chars to split it
			Then we can recursively split the right hand string formed by the split to get unique splits
			For each split string we can 

			To avoid using recursive function calls I can use a stack
			After splitting and testing each string (assuming it has unsplit boundies) is put on a stack
			During each iteration a new string is popped off the stack, split, and then the split strings are put back on the stack
			I used a similair strategy when doing recursive spatial partitioning in my BVH (see BVH.cpp in my GPURayTracer repo)

			To keep memory usage low I keep track of integer indexs into PrefixedString to represent the split
			*/

			std::stack<std::vector<uint32_t>> Splits;

			// Before going into the splitting loop lets create the initial splits 
			uint32_t SplitCounter = 0;
			for (; SplitCounter < PrefixedString.size() - 1;) {
				SplitCounter++;

				std::vector<uint32_t> Vec(1);
				Vec[0] = SplitCounter;
			}

			while (!Splits.empty()) {
				// Get a split and then test it
				std::vector<uint32_t> CurrentSplit = Splits.top();
				Splits.pop();

				// Test 
				NumSolutions += ValidateSplits(PrefixedString, CurrentSplit);

				// Generate more splits
				for (size_t NewSplitIndex = CurrentSplit.back(); NewSplitIndex < PrefixedString.size() - 1; NewSplitIndex++) {
					std::vector<uint32_t> NewSplit;
					NewSplit.reserve(CurrentSplit.size() + 1);
					NewSplit.insert(NewSplit.end(), CurrentSplit.begin(), CurrentSplit.end());
					NewSplit.push_back(NewSplitIndex);
					Splits.push(NewSplit);
				}
			}
			
			std::cout << NumSolutions << ' ';
		}

		std::cout << '\n';
	}

	return 0;
}
