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

void readDistances(map<string, map<string, int>>& distances) {
    ifstream fileDistances("distances.txt");
    while (fileDistances.peek() != EOF) {
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

        distances[s2][s3] = i;
    }
    fileDistances.close();
}

void readTasks(map<string, vector<string>>& tasks) {
    ifstream fileTasks("tasks.txt");
    while (fileTasks.peek() != EOF) {
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
    fileTasks.close();
}

void printDistances(map<string, map<string, int>>& distances) {
    cout << "distances:\n";
    for (auto elem : distances) {
        cout << elem.first << ": ";
        for (auto elem2 : elem.second) {
            cout << elem2.first << elem2.second << " ";
        }
        cout << "\n";
    }
}

void printTasks(map<string, vector<string>>& tasks) {
    cout << "tasks:\n";
    for (auto elem : tasks) {
        cout << elem.first << ": ";
        for (string s : elem.second) {
            cout << s << " ";
        }
        cout << "\n";
    }
}

class Robot {
public:
    vector<string> visitHistory;
    vector<string> inventory;
    map<string, map<string, int>> distances;
    map<string, vector<string>> tasks;
    string currentPos;

    Robot(map<string, map<string, int>>& d, map<string, vector<string>>& t) {
        distances = d;
        tasks = t;
        cout << "Robot created.\n";
    }

    void setPos(string s) {
        currentPos = s;
        cout << "\nMoved to " << currentPos << "\n";

        //task protocol
        //dropoff tasks
        vector<string>::iterator it = find(inventory.begin(), inventory.end(), currentPos);
        while (it != inventory.end()) {
            cout << "Dropping off one parcel...\n";
            inventory.erase(it);
            it = find(inventory.begin(), inventory.end(), currentPos);
        }
        //pickup tasks
        if (tasks[currentPos].empty()) {
            cout << "Picked up no tasks.\n";
        }
        else {
            //add tasks into inventory and clear out tasks from position
            inventory.insert(inventory.end(), tasks[currentPos].begin(), tasks[currentPos].end());
            cout << "Picked up parcels ";
            for (string i : tasks[currentPos]) {
                cout << i << " ";
            }
            cout << "\n";
            tasks[currentPos].clear();
        }
        //print inventory
        cout << "Inventory: ";
        for (string i : inventory) {
            cout << i << " ";
        }
        cout << "\n";

        //add position to history
        visitHistory.push_back(currentPos);
        //print visit history
        cout << "Visit History: ";
        for (string i : visitHistory) {
            cout << i << " ";
        }
        cout << "\n";

        nextPos();
    }

    void nextPos() {
        //find closest unvisited position
        pair<string, int> closestUnvisited("", numeric_limits<int>::max());
        for (auto elem : distances[currentPos]) {
            if (find(visitHistory.begin(), visitHistory.end(), elem.first) == visitHistory.end()) { //check if unvisited
                if (elem.second < closestUnvisited.second) { //find the closest
                    closestUnvisited.first = elem.first;
                    closestUnvisited.second = elem.second;
                }
            }
        }

        //goto closest unvisited position if any
        if (closestUnvisited.second != numeric_limits<int>::max()) {
            cout << "CLOSEST UNVISITED POSITION FROM WHERE I AM (" << currentPos << ") IS " << closestUnvisited.first << " WITH A DISTANCE OF " << closestUnvisited.second << "\n";
            setPos(closestUnvisited.first);
        }
        //if there is no unvisited position deliver closest parcel if any
        else if (inventory.empty() == false) {
            pair<string, int> closestParcel("", numeric_limits<int>::max());
            for (string parcelDestination : inventory) {
                if (distances[currentPos][parcelDestination] < closestParcel.second) {
                    closestParcel.first = parcelDestination;
                    closestParcel.second = distances[currentPos][parcelDestination];
                }
            }
            cout << "CLOSEST UNDELIVERED PARCEL FROM WHERE I AM (" << currentPos << ") IS " << closestParcel.first << " WITH A DISTANCE OF " << closestParcel.second << "\n";
            setPos(closestParcel.first);
        }
        //go home if nothing else to do
        else {
            cout << "All positions have been visited and all parcels have been delivered.\n";
            if (currentPos != "HOME") {
                cout << "Going back HOME.\n";
                setPos("HOME");
            }
            cout << "\n";
        }
    }
};

int main()
{
    map<string, map<string, int>> distances;
    map<string, vector<string>> tasks;

    readDistances(distances);
    readTasks(tasks);

    Robot myRobot(distances, tasks);
    myRobot.setPos("HOME");
}