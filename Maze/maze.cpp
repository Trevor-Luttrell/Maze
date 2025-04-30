#include<iostream>
#include<vector>
#include<stack>
#include<random>
#include<thread>
#include<chrono>
#include<windows.h>
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::stack;

struct coordinate {
	int xCoordinate = 0;
	int yCoordinate = 0;
};

class maze {
public:
    maze(int height, int width, char playerSymbol, char destinationSymbol, char startSymbol) {
        this->height = height;
        this->width = width;
        this->playerSymbol = playerSymbol;
        this->destinationSymbol = destinationSymbol;
        this->startSymbol = startSymbol;
        resizeMaze();
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                Maze[i][j] = char(219);
            }
        }
        buildMaze();
    }

    void printMaze() {
        for (int i = 0; i < this->height; ++i) {
            for (int j = 0; j < this->width; ++j) {
                cout << Maze[i][j];
            }
            cout << endl;
        }
    }

    void play() {
        updatePlayerPosition();

        while (true) {
            int key = getKey();

            int changeInX = 0, changeInY = 0;
            if (key == VK_LEFT) {
                changeInX = -1;
            }
            else if (key == VK_RIGHT) {
                changeInX = 1; 
            }
            else if (key == VK_UP) {
                changeInY = -1;
            }
            else if (key == VK_DOWN) {
                changeInY = 1;
            }
            
            int newXCoordinate = playerLocation.xCoordinate + changeInX;
            int newYCoordinate = playerLocation.yCoordinate + changeInY;

            
            if (newXCoordinate >= 0 && newXCoordinate < width && newYCoordinate >= 0 && newYCoordinate < height && Maze[newYCoordinate][newXCoordinate] != char(219)) {
                
                COORD oldCoordinate = {(playerLocation.xCoordinate), (playerLocation.yCoordinate) };
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), oldCoordinate);
                cout << (Maze[playerLocation.yCoordinate][playerLocation.xCoordinate] == startSymbol ? startSymbol : ' ');

                playerLocation.xCoordinate = newXCoordinate;
                playerLocation.yCoordinate = newYCoordinate;

                updatePlayerPosition();
            }

            if (Maze[playerLocation.yCoordinate][playerLocation.xCoordinate] == destinationSymbol) {
                system("cls");
                cout << "YOU WIN!!!!!";
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

    }

    void updatePlayerPosition() {
        COORD newCoord = {(playerLocation.xCoordinate), (playerLocation.yCoordinate) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), newCoord);
        cout << playerSymbol;
    }

    int getKey() {
        int result = 0;
        while (result == 0) {
            if (GetAsyncKeyState(VK_LEFT) & 1) {
                result = VK_LEFT;
            }
            else if (GetAsyncKeyState(VK_UP) & 1) {
                result = VK_UP;
            }
            else if (GetAsyncKeyState(VK_RIGHT) & 1) {
                result = VK_RIGHT;
            }
            else if (GetAsyncKeyState(VK_DOWN) & 1) {
                result = VK_DOWN;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return result;
    }

private:
    int height;
    int width;
    char playerSymbol;
    char destinationSymbol;
    char startSymbol;
    vector<vector<char>> Maze;
    stack<coordinate> backTrackContainer;
    coordinate currentLocation;
    coordinate startingLocation;
    coordinate playerLocation;
    coordinate endLocation;

    void resizeMaze() {
        Maze.resize(this->height);
        for (int i = 0; i < this->height; ++i) {
            Maze[i].resize(this->width);
        }
    }

    int generateRandomCoordinate(int maximumNumber) {
        std::random_device rd;
        std::mt19937 engine(rd());
        std::uniform_int_distribution<int> random_die(1, maximumNumber - 2);
        return random_die(engine);
    }

    void digOut() {
        do {
            bool madeProgress = false;
            if (currentLocation.xCoordinate + 3 < width) {
                if (Maze[currentLocation.xCoordinate + 2][currentLocation.yCoordinate] == char(219)) {
                    Maze[currentLocation.xCoordinate + 1][currentLocation.yCoordinate] = ' ';
                    Maze[currentLocation.xCoordinate + 2][currentLocation.yCoordinate] = ' ';
                    backTrackContainer.push(currentLocation);
                    currentLocation.xCoordinate += 2;
                    madeProgress = true;
                }
            }

            if (currentLocation.xCoordinate - 3 >= 0) {
                if (Maze[currentLocation.xCoordinate - 2][currentLocation.yCoordinate] == char(219)) {
                    Maze[currentLocation.xCoordinate - 1][currentLocation.yCoordinate] = ' ';
                    Maze[currentLocation.xCoordinate - 2][currentLocation.yCoordinate] = ' ';
                    backTrackContainer.push(currentLocation);
                    currentLocation.xCoordinate -= 2;
                    madeProgress = true;
                }
            }

            if (currentLocation.yCoordinate + 3 < height) {
                if (Maze[currentLocation.xCoordinate][currentLocation.yCoordinate + 2] == char(219)) {
                    Maze[currentLocation.xCoordinate][currentLocation.yCoordinate + 1] = ' ';
                    Maze[currentLocation.xCoordinate][currentLocation.yCoordinate + 2] = ' ';
                    backTrackContainer.push(currentLocation);
                    currentLocation.yCoordinate += 2;
                    madeProgress = true;
                }
            }

            if (currentLocation.yCoordinate - 3 >= 0) {
                if (Maze[currentLocation.xCoordinate][currentLocation.yCoordinate - 2] == char(219)) {
                    Maze[currentLocation.xCoordinate][currentLocation.yCoordinate - 1] = ' ';
                    Maze[currentLocation.xCoordinate][currentLocation.yCoordinate - 2] = ' ';
                    backTrackContainer.push(currentLocation);
                    currentLocation.yCoordinate -= 2;
                    madeProgress = true;
                }
            }

            if (!madeProgress) {
                if (!backTrackContainer.empty()) {
                    currentLocation = backTrackContainer.top();
                    backTrackContainer.pop();
                }
            }
        } while (!backTrackContainer.empty());

        endLocation.xCoordinate = generateRandomCoordinate(this->width);
        endLocation.yCoordinate = generateRandomCoordinate(this->height);
        while (Maze[endLocation.xCoordinate][endLocation.yCoordinate] != ' ') {
            endLocation.xCoordinate = generateRandomCoordinate(this->width);
            endLocation.yCoordinate = generateRandomCoordinate(this->height);
        }
        Maze[endLocation.xCoordinate][endLocation.yCoordinate] = destinationSymbol;

    }

    void buildMaze() {
        do {
            currentLocation.xCoordinate = generateRandomCoordinate(this->width);
        } while (currentLocation.xCoordinate % 2 == 0);

        do {
            currentLocation.yCoordinate = generateRandomCoordinate(this->height);
        } while (currentLocation.yCoordinate % 2 == 0);
        
        digOut();
        startingLocation = currentLocation;
        playerLocation = startingLocation;
        Maze[startingLocation.yCoordinate][startingLocation.xCoordinate] = this->startSymbol;
    }
};

int main() {
    int height = 0;
    int width = 0;
    char playerSymbol = 'P';
    char destinationSymbol = 'X';
    char startSymbol = 'S';
    char wantToCustomize = '\0';

    while (height % 2 == 0 || height == 1 || height <= 3) {
        cout << "What would you like the maze's height to be? (Must be odd, non-zero, and bigger than three)" << endl;
        cin >> height;
    }

    while (width % 2 == 0 || width == 1 || width <= 3) {
        cout << "What would you like the maze's width to be? (Must be odd, non-zero, and bigger than three)" << endl;
        cin >> width;
    }

    while (wantToCustomize != 'Y' && wantToCustomize != 'y' && wantToCustomize != 'N' && wantToCustomize != 'n') {
        cout << "Would you like to customize the maze's symbols? Input Y for yes and N for no." << endl;
        cin >> wantToCustomize;
        if (wantToCustomize == 'Y' || wantToCustomize == 'y') {
            cout << "What would you like the symbol for your player to be? (One character)" << endl;
            cin >> playerSymbol;
            cout << "What would you like the symbol for your starting location to be? (One character)" << endl;
            cin >> startSymbol;
            cout << "What would you like the symbol for your exit to be? (One character)" << endl;
            cin >> destinationSymbol;
        }
    }

    system("cls");

    maze userMaze(height, width, playerSymbol, destinationSymbol, startSymbol);
    userMaze.printMaze();
    userMaze.play();
}