/*
ID: shadax31
LANG: C++
TASK: milk2
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

std::ifstream Input;
std::ofstream Output;

int main() {
	Input .open("milk2.in" );
	Output.open("milk2.out");

	using Timing = std::pair<uint32_t, uint32_t>;

	std::vector<Timing> Schedule;

	size_t NumCows = 0;
	Input >> NumCows;
	Schedule.resize(NumCows);

	for (Timing& Time : Schedule) {
		Input >> Time.first >> Time.second;
	}
	
	std::sort(Schedule.begin(), Schedule.end(), [](const Timing& T0, const Timing& T1) -> bool {return T0.first < T1.first; });

	uint32_t MaxTimeMilked   = 0;
	uint32_t MaxTimeUnmilked = 0;

	bool Reduction = true;
	while (Reduction) {
		Reduction = false;

		std::vector<Timing> ReducedSchedule;
		for (const Timing& Time : Schedule) {
			if (ReducedSchedule.empty()) {
				ReducedSchedule.push_back(Time);
			} else {
				Timing& Back = ReducedSchedule.back();

				if (Back.second >= Time.first) {
					if (Back.second < Time.second) {
						Back.second = Time.second;
					}
					Reduction = true;
				} else {
					ReducedSchedule.push_back(Time);
				}
			}
		}

		Schedule = ReducedSchedule;
	}

	Timing PreviousTime;
	PreviousTime.first = PreviousTime.second = Schedule.front().first;

	for (const Timing& Time : Schedule) {
		MaxTimeMilked   = std::max(MaxTimeMilked  , Time.second         - Time.first);
		MaxTimeUnmilked = std::max(MaxTimeUnmilked, Time.first - PreviousTime.second);

		PreviousTime = Time;
	}

	Output << MaxTimeMilked << ' ' << MaxTimeUnmilked << std::endl;

	return 0;
}