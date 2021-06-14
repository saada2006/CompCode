/*
ID: shadax31
LANG: C++
TASK: beads
*/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

/*
Dear god... I'm really bad at comp code
It took me like an hour to find a good not-over-engineered solution, or even a solution at all
*/

int main() {
    std::ifstream BeadsIn("beads.in");
    std::cin.rdbuf(BeadsIn.rdbuf());

    std::ofstream BeadsOut("beads.out");
    std::cout.rdbuf(BeadsOut.rdbuf());

    size_t NumBeads;
    std::cin >> NumBeads;
    
    std::string Necklace;
    Necklace.reserve(NumBeads);
    std::cin >> Necklace;

    int Solution = 0;

    std::vector<bool> CollectionMarker(Necklace.size());

    for (int Index = 0; Index < Necklace.size(); Index++) {
        std::fill(CollectionMarker.begin(), CollectionMarker.end(), false);

        std::string FirstPortion  = Necklace.substr(0, Index);
        std::string SecondPortion = Necklace.substr(   Index);

        std::string NewNecklace = SecondPortion + FirstPortion;

        int CurrentSolution = 0;

        char BeadColor;
        constexpr char DefaultBeadColor = '\0';

        BeadColor = DefaultBeadColor;
        for (auto Iter = NewNecklace.begin(); Iter != NewNecklace.end(); Iter++) {
            size_t Offset = std::distance(NewNecklace.begin(), Iter);

            if (CollectionMarker[Offset]) { // We've run into previously collected beads
                break;
            }

            char Bead = *Iter;
            if (BeadColor == DefaultBeadColor && Bead != 'w') {
                BeadColor = Bead;
            }

            if (BeadColor == Bead || Bead == 'w') {
                CurrentSolution++;
                CollectionMarker[Offset] = true;
            }
            else {
                break;
            }
        }

        BeadColor = DefaultBeadColor;
        for (auto Iter = NewNecklace.end() - 1; Iter != NewNecklace.begin(); Iter--) {
            size_t Offset = std::distance(NewNecklace.begin(), Iter);

            if (CollectionMarker[Offset]) { // We've run into previously collected beads
                break;
            }

            char Bead = *Iter;
            if (BeadColor == DefaultBeadColor && Bead != 'w') {
                BeadColor = Bead;
            }

            if (BeadColor == Bead || Bead == 'w') {
                CurrentSolution++;
                CollectionMarker[Offset] = true;
            }
            else {
                break;
            }
        }

        Solution = std::max(Solution, CurrentSolution);
    }

    std::cout << Solution << std::endl;

	return 0;
}