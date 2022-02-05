#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class Position {
public:
    vector<string> tasks;
    map<string, int> cons;
    string name;

    Position(string n) {
        name = n;
    }

    void addTask(string c) {
        tasks.push_back(c);
    }

    void setCons(int n1, int n2, int n3, int n4, int n5) {
        cons["HOME"] = n1;
        cons["A"] = n2;
        cons["B"] = n3;
        cons["C"] = n4;
        cons["D"] = n5;
    }
};

class Robot {
public:
    Position* currentPosition;
    vector<string> visitHistory;
    vector<string> inventory;
    map<string, Position*> positions;

    Robot(map<string, Position*> p) {
        positions = p;
        cout << "Robot created.\n";
    }

    void setPos(string n) { //ideally pass string and refer to positions map
        currentPosition = positions[n];
        cout << "\nMoved to " << currentPosition->name << "\n";

        //task protocol
        //dropoff tasks
        vector<string>::iterator it = find(inventory.begin(), inventory.end(), currentPosition->name);
        while (it != inventory.end()) {
            cout << "Dropping off one parcel...\n";
            inventory.erase(it);
            it = find(inventory.begin(), inventory.end(), currentPosition->name);
        }
        //print out tasks available at position
        if (currentPosition->tasks.empty() == true) {
            //if there are no tasks
            cout << "Picked up no tasks.\n";
        }
        else {
            //if there are tasks
            //pick up tasks
            inventory.insert(inventory.end(), currentPosition->tasks.begin(), currentPosition->tasks.end());
            cout << "Picked up parcels ";
            for (string i : currentPosition->tasks) {
                cout << i << " ";
            }
            cout << "\n";
            //remove all tasks from position
            currentPosition->tasks.clear();
        }
        //print inventory
        cout << "Inventory: ";
        for (string i : inventory) {
            cout << i << " ";
        }
        cout << "\n";
        

        //add position to history
        visitHistory.push_back(currentPosition->name);
        //print visit history
        cout << "Visit History: ";
        for (string i : visitHistory) {
            cout << i << " ";
        }
        cout << "\n";
    }

    void nextPos() {

    }
};

int main()
{
    map<string, Position*> positions;

    Position pHOME("HOME");
    pHOME.setCons(0, 6, 8, 5, 10);
    positions[pHOME.name] = &pHOME;

    Position pA("A");
    pA.setCons(6, 0, 4, 7, 8);
    positions[pA.name] = &pA;
    pA.addTask("B");
    pA.addTask("B");

    Position pB("B");
    pB.setCons(8, 4, 0, 6, 3);
    positions[pB.name] = &pB;

    Position pC("C");
    pC.setCons(5, 7, 6, 0, 7);
    positions[pC.name] = &pC;

    Position pD("D");
    pD.setCons(10, 8, 3, 7, 0);
    positions[pD.name] = &pD;

    Robot myRobot(positions);
    cout << "Positions loaded into positions map.\nPosition Name\tPosition Address\n";
    for (const auto& elem : myRobot.positions) {
        cout << elem.first << "\t\t" << elem.second << "\n";
    }
    cout << "\n";
    myRobot.setPos("HOME");
    myRobot.setPos("A");
    myRobot.setPos("B");

    //TODO:
    //make setPos take strings instead
    //make a loop for creating positions and put them into a vector
}