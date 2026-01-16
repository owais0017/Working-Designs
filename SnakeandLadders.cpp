#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

// ==========================================
// 1. Jumper (Abstract Base Class)
// ==========================================
class Jumper {
protected:
    int start;
    int end;
public:
    Jumper(int s, int e) : start(s), end(e) {}
    virtual ~Jumper() {}
    int getEnd() { return end; }
    virtual void info() = 0; 
};

class Snake : public Jumper {
public:
    Snake(int s, int e) : Jumper(s, e) {}
    void info() override {
        cout << "  [SNAKE] Bitten at " << start << "! Sliding down to " << end << endl;
    }
};

class Ladder : public Jumper {
public:
    Ladder(int s, int e) : Jumper(s, e) {}
    void info() override {
        cout << "  [LADDER] Climbed at " << start << "! Going up to " << end << endl;
    }
};

// ==========================================
// 2. Cell Class
// ==========================================
class Cell {
public:
    int id;
    Jumper* jumper;

    Cell(int i) : id(i), jumper(nullptr) {}
    
    bool hasJump() { return jumper != nullptr; }
};

// ==========================================
// 3. Board Class
// ==========================================
class Board {
    int sz;
    vector<Cell*> cells;

public:
    Board(int s) : sz(s) {
        for (int i = 0; i <= sz; i++) {
            cells.push_back(new Cell(i));
        }
    }

    void addJumper(int s, int e) {
        if (s > e) cells[s]->jumper = new Snake(s, e);
        else cells[s]->jumper = new Ladder(s, e);
    }

    Cell* getCell(int p) { return cells[p]; }
    int getSize() { return sz; }
};

// ==========================================
// 4. Player Class
// ==========================================
class Player {
public:
    string name;
    int pos;

    Player(string n) : name(n), pos(0) {}
};

// ==========================================
// 5. Dice Class
// ==========================================
class Dice {
    int count;
    int faces;
public:
    Dice(int c, int f) : count(c), faces(f) {
        srand(time(0)); 
    }

    int roll() {
        int sum = 0;
        for (int i = 0; i < count; i++) {
            sum += (rand() % faces) + 1;
        }
        return sum;
    }
};

// ==========================================
// 6. Game Controller (The Orchestrator)
// ==========================================
class Game {
    Board* board;
    Dice* dice;
    queue<Player*> players;
    int winScore;

public:
    Game(Board* b, Dice* d, vector<Player*> pList) {
        board = b;
        dice = d;
        winScore = b->getSize();
        for (auto p : pList) players.push(p);
    }

    void play() {
        cout << "--- Game Started! Target: " << winScore << " ---" << endl;
        
        while (true) {
            Player* curr = players.front();
            players.pop();

            int move = dice->roll();
            int nextPos = curr->pos + move;

            cout << curr->name << " rolled a " << move;

            if (nextPos > winScore) {
                cout << " but overshot! Stays at " << curr->pos << endl;
            } else {
                curr->pos = nextPos;
                Cell* cell = board->getCell(curr->pos);
                
                if (cell->hasJump()) {
                    cout << ". ";
                    cell->jumper->info();
                    curr->pos = cell->jumper->getEnd();
                }
                cout << ". Now at: " << curr->pos << endl;
            }

            if (curr->pos == winScore) {
                cout << "\n*** " << curr->name << " WINS THE GAME! ***" << endl;
                break;
            }

            players.push(curr);
        }
    }
};

// ==========================================
// 7. Main Function (Execution)
// ==========================================
int main() {
    // Setup Board
    Board* myBoard = new Board(100);
    myBoard->addJumper(14, 4);   // Snake
    myBoard->addJumper(99, 10);  // Snake
    myBoard->addJumper(3, 30);   // Ladder
    myBoard->addJumper(50, 95);  // Ladder

    // Setup Players
    Player* p1 = new Player("Alice");
    Player* p2 = new Player("Bob");
    vector<Player*> players = {p1, p2};

    // Setup Dice (1 die, 6 faces)
    Dice* myDice = new Dice(1, 6);

    // Initialize and Start Game
    Game myGame(myBoard, myDice, players);
    myGame.play();

    return 0;
}