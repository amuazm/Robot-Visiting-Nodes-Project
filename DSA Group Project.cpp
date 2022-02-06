#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

void readidiot(ifstream& distances, map<string, map<string, int>>& positions) {
    char s[256];

    distances.get(s, 256, '-');
    string s2(s);
    trim(s2);

    distances.ignore(1);

    distances.get(s, 256, ':');
    string s3(s);
    trim(s3);

    distances.ignore(1);

    distances.get(s, 256);
    int i = atoi(s);

    positions[s2][s3] = i;
}

int main()
{
	ifstream distances("distances.txt");

    map<string, map<string, int>> positions;

    while (distances.peek() != EOF) {
        readidiot(distances, positions);
    }

    //pDistances.push_back(map<string, int> = { {s3, i} });
    //positions.insert(pair<string, map>(s2, ));

    for (auto elem : positions) {
        cout << elem.first << ": ";
        for (auto elem2 : elem.second) {
            cout << elem2.first << elem2.second;
        }
        cout << "\n";
    }

	distances.close();
}