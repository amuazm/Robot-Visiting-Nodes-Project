#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

#define MYMAX numeric_limits<int>::max()

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
    map<string, map<string, int>> distances;
    map<string, vector<string>> tasks;

    vector<string> visitHistory;
    vector<string> inventory;
    string currentPos = "";
    int distanceTravelled = 0;

    Robot(map<string, map<string, int>>& d, map<string, vector<string>>& t) {
        distances = d;
        tasks = t;
        cout << "Robot created.\n";
    }

    void setPos(string s) {

        //calculate distance travelled
        if (currentPos != "") {
            distanceTravelled += distances[currentPos][s];
        }

        currentPos = s;

        //add position to history
        visitHistory.push_back(currentPos);
        //print visit history
        cout << "\nVisit History: ";
        for (string i : visitHistory) {
            cout << i << " | ";
        }
        cout << "\n";

        cout << "Distance travelled: " << distanceTravelled << "\n";

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
            cout << "Picked up parcels: ";
            for (string i : tasks[currentPos]) {
                cout << i << " | ";
            }
            cout << "\n";
            tasks[currentPos].clear();
        }
        //print inventory
        cout << "Inventory: ";
        for (string i : inventory) {
            cout << i << " | ";
        }
        cout << "\n";

        nextPos();
    }

    bool gotoClosestUnvisited() {
        //find closest unvisited position
        pair<string, int> closestUnvisited("", MYMAX);
        for (auto elem : distances[currentPos]) { //use the current position's distances map as a reference
            if (find(visitHistory.begin(), visitHistory.end(), elem.first) == visitHistory.end()) { //check if unvisited
                if (elem.second < closestUnvisited.second) { //find the closest
                    closestUnvisited.first = elem.first;
                    closestUnvisited.second = elem.second;
                }
            }
        }
        if (closestUnvisited.first != "") {
            cout << "Going to closest unvisited position.\n";
            setPos(closestUnvisited.first);
            return true;
        }
        else {
            return false;
        }
    }

    bool deliverClosestParcel() {
        pair<string, int> closestParcelDelivery("", MYMAX);
        if (inventory.empty() == false) {
            for (string parcelDestination : inventory) {
                if (distances[currentPos][parcelDestination] < closestParcelDelivery.second) {
                    closestParcelDelivery.first = parcelDestination;
                    closestParcelDelivery.second = distances[currentPos][parcelDestination];
                }
            }
        }
        if (closestParcelDelivery.first != "") {
            cout << "Delivering closest parcel.\n";
            setPos(closestParcelDelivery.first);
            return true;
        }
        else {
            return false;
        }
    }

    void goHOME() {
        cout << "Jobs done!\n";
        if (currentPos != "HOME") {
            cout << "Going back HOME.\n";
            setPos("HOME");
        }
    }

    void nextPos() {
        if (gotoClosestUnvisited());
        else if (deliverClosestParcel());
        else goHOME();
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