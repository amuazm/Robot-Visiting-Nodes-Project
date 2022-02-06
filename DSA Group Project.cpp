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

void readidiot(ifstream& fileDistances, map<string, map<string, int>>& positions) {
    char s[256];

    fileDistances.get(s, 256, '-');
    string s2(s);
    trim(s2);

    fileDistances.ignore(1);

    fileDistances.get(s, 256, ':');
    string s3(s);
    trim(s3);

    fileDistances.ignore(1);

    fileDistances.get(s, 256);
    int i = atoi(s);

    positions[s2][s3] = i;
}

void readDistances(ifstream& fileTasks, map<string, vector<string>>& tasks) {
    char s[256];

    fileTasks.get(s, 256, '-');
    string s2(s);
    trim(s2);

    fileTasks.ignore(1);

    fileTasks.get(s, 256);
    string s3(s);
    trim(s3);

    tasks[s2].push_back(s3);
}

int main()
{
	ifstream fileDistances("distances.txt");

    map<string, map<string, int>> positions;

    while (fileDistances.peek() != EOF) {
        readidiot(fileDistances, positions);
    }

    for (auto elem : positions) {
        cout << elem.first << ": ";
        for (auto elem2 : elem.second) {
            cout << elem2.first << elem2.second << " ";
        }
        cout << "\n";
    }

	fileDistances.close();

    cout << "\n\n\n";
    ifstream fileTasks("tasks.txt");

    map<string, vector<string>> tasks;

    while (fileTasks.peek() != EOF) {
        readDistances(fileTasks, tasks);
    }

    for (auto elem : tasks) {
        cout << elem.first << ": ";
        for (string s : elem.second) {
            cout << s << " ";
        }
        cout << "\n";
    }

    fileTasks.close();
}