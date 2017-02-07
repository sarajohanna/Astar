//
//  main.cpp
//  Astar
//
//  Created by Sara Lindström on 2017-02-04.
//  Copyright © 2017 Sara Lindström. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Node {
    int x;
    int y;
    double costG;
    double costF;
    int parentX;
    int parentY;
    bool operator == (const Node node2)
    {
        return (x == node2.x && y == node2.y);
    }
};

struct Map {
    
    int rows =  sizeof _mapdata / sizeof _mapdata[0];
    int cols = sizeof _mapdata[0] / sizeof(int);
    
    const int getMap(int x, int y) {
        int value;
        if ((x < 0 || x >= cols) || (y < 0 || y >= rows))
            value = -1;
        else
            value = _mapdata[y][x];
        return value;
    }
    
    void setMap(int x, int y) {
        _mapdata[y][x] = 8;
    }
    
    void printMap() {
        
        for (int y = 0; y < 6; ++y) {
            
            for (int x = 0; x < 6; ++x) {
                cout << setw(2) << _mapdata[y][x] << " ";
            }
            cout << std::endl;
        }
    }
    
    int _mapdata[6][6] = {
        { 1, 1, 1,  1, 1, 1},
        { 1, 1, 1, -1, 1, 1},
        { 1, 1, 1, -1, 1, 1},
        { 1, 1, 1, -1, 1, 1},
        { 1, 1, 1, -1, 1, 1},
        { 1, 1, 1,  1, 1, 1}
    };
};

struct BigMap {
    
    int rows =  sizeof _mapdata / sizeof _mapdata[0];
    int cols = sizeof _mapdata[0] / sizeof(int);
    
    const int getMap(int x, int y) {
        int value;
        if ((x < 0 || x >= cols) || (y < 0 || y >= rows))
            value = -1;
        else
            value = _mapdata[y][x];
        return value;
    }
    
    void setMap(int x, int y) {
        _mapdata[y][x] = 33;
    }
    
    void printMap() {
        
        for (int y = 0; y < rows; ++y) {
            
            for (int x = 0; x < cols; ++x) {
                cout << setw(2) << _mapdata[y][x] << " ";
            }
            cout << std::endl;
        }
    }
    
    int _mapdata[16][16] = {
        { 1, 1, 1, -1, 1, 1, 1, 1,  1, 1, 1,  1, 1, 1, 1, 1},
        { 1, 1, 1, -1, 1, 1, 1, 1,  1, 1, 1,  1, 1, 1, 1, 1},
        { 1, 1, 1, -1, 1, 1, 1, 1,  1, 1, 1,  1, 1, 1, 1, 1},
        { 1, 1, 1, -1, 1, 1, 1, 1,  1, 1, 1,  1, 1, 1, 1, 1},
        { 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 1},
        { 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 1},
        { 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 1},
        { 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 1},
        { 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 1},
        { 1, 1, 1,  1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 1},
        { 1, 1, 1,  1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 1},
        { 1, 1, 1,  1, 1, 1, 1, 1, -1, 1, 1, -1, 1, 1, 1, 1},
        { 1, 1, 1,  1, 1, 1, 1, 1, -1, 1, 1,  1, 1, 1, 1, 1},
        { 1, 1, 1,  1, 1, 1, 1, 1, -1, 1, 1,  1, 1, 1, 1, 1},
        { 1, 1, 1,  1, 1, 1, 1, 1, -1, 1, 1,  1, 1, 1, 1, 1},
        { 1, 1, 1,  1, 1, 1, 1, 1, -1, 1, 1,  1, 1, 1, 1, 1},
    };
};

double calcCostH(Node endNode, int x, int y) {
    return (std::abs(endNode.x - x) + std::abs(endNode.y - y)) * 10;
}

bool isOnClosedList(Node node, vector<Node>& closedList) {
    bool val =  false;
    for (int i = 0; i < closedList.size(); ++i) {
        if (closedList[i] == node) {
            val = true;
            break;
        }
    }
    return val;
}

Node findNodeInClosedList(vector<Node>& closedList, int x, int y) {
    Node invalidNode = {-1, -1, -1, -1, -1, -1};
    for (Node i : closedList) {
        if (i.x == x && i.y == y) {
            return i;
        }
    }
    return invalidNode;
}

//Prints the parents of the nodes on the closed list - for debugging
void printClosedList(vector<Node>& closedList) {
    for (int y = 0; y < 16; ++y) {
        for (int x = 0; x < 16; ++x) {
            Node currentNode = findNodeInClosedList(closedList, x, y);
            if (currentNode.x < 0) {
                cout << "XX ";
            }
            else
                cout << currentNode.parentX << currentNode.parentY << " ";
        }
        cout << endl;
    }
}

// Finds path by going from end node to its parent, and from that to its parent and so on, then reverses the path
vector<Node> findPath(vector<Node>& closedList, int endX, int endY) {
    vector<Node> path;
    Node current = findNodeInClosedList(closedList, endX, endY);
    
    while(!(current.parentX == current.x && current.parentY == current.y)) {
        path.push_back(current);
        current = findNodeInClosedList(closedList, current.parentX, current.parentY);
    }
    
    path.push_back(current);
    reverse(path.begin(), path.end());
    return path;
}

// Get gCost for child node depending on diagonal step or not
double childCostG(int dx, int dy) {
    double childCostG;
    
    if (dx == 0 || dy == 0) {
        childCostG = 10;
    }
    else {
        childCostG = 14;
    }
    return childCostG;
}

int main(int argc, const char * argv[]) {
    
    BigMap bigMap;
    
    // Create start and end node
    Node endNode = {15, 15, 10, 0};
    double costHStart = calcCostH(endNode, 1, 1);
    Node startNode = {1, 1, 0, costHStart, 1, 1};
    
    // Create lists needed
    vector<Node> openList = {startNode};
    vector<Node> closedList;
    
    Node current = openList[0];
    
    while (openList.size() != 0) {
        
        // Get node with lowest costF
        int index = 0;
        for(int i = 0; i < openList.size(); ++i) {
            if (openList[i].costF < openList[index].costF) {
                index = i;
            }
        }
        
        // If current is the same as end, print path on map and break
        if (current == endNode) {
            for (Node i : findPath(closedList, endNode.x, endNode.y)) {
                bigMap.setMap(i.x, i.y);
            }
            bigMap.printMap();
            break;
       }
        
        // Update current node, openList and closedList
        current = openList[index];
        openList.erase(openList.begin() + index);
        closedList.push_back(current);

        for(int x = (current.x - 1); x <= current.x + 1; ++x) {
            for(int y = current.y - 1; y <= current.y + 1; ++y) {
                
                // Creates the child node
                double costH = calcCostH(endNode, x, y);
                double costG = childCostG(x - current.x, y - current.y) + current.costG;
                double costF = costG + costH;
                Node child = {x, y, costG, costF, current.x, current.y};
                
                if (isOnClosedList(child, closedList)) {
                    continue;
                }
                
                // If it's a wall or outside the map
                if (bigMap.getMap(x, y) == -1) {
                    continue;
                }
                
                // First time the openList will be empty so we will push the first child directly
                if (openList.size() == 0) {
                    openList.push_back(child);
                }
                
                // Check if it's on open list and return index if it is
                bool val = false;
                int indexO = 0;
                
                for (int i = 0; i < openList.size(); ++i) {
                    if ((openList[i] == child)) {
                        indexO = i;
                        break;
                    }
                }
                
                // If it's on the open list and the costG is lower, update it
                if (val && costG < openList[indexO].costG) {
                    openList[indexO].costG = costG;
                    openList[indexO].costF = costF;
                    openList[indexO].parentX = current.x;
                    openList[indexO].parentY = current.y;
                }
                
                // If it's not on the open list, add it
                else if (!val) {
                    openList.push_back(child);
                }
            }
        }
    }
    
    return 0;
}

