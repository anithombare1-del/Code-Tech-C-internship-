#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

using namespace std;

#ifdef _WIN32
bool keyPressed() {
    DWORD numEvents;
    GetNumberOfConsoleInputEvents(GetStdHandle(STD_INPUT_HANDLE), &numEvents);
    return numEvents > 0;
}
char readKey() {
    INPUT_RECORD ir;
    DWORD read;
    ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &ir, 1, &read);
    if (ir.EventType == KEY_EVENT && ir.Event.KeyEvent.bKeyDown) {
        return ir.Event.KeyEvent.uChar.AsciiChar;
    }
    return 0;
}
#else
struct termios orig_termios;
void enableRawMode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
bool keyPressed() {
    char c;
    return read(STDIN_FILENO, &c, 1) > 0;
}
char readKey() {
    char c = 0;
    read(STDIN_FILENO, &c, 1);
    return c;
}
#endif

const int WIDTH = 40;
const int HEIGHT = 20;
const int MIN_DELAY_MS = 80;
const int MAX_DELAY_MS = 220;

enum Direction { UP, DOWN, LEFT, RIGHT, NONE };

class SnakeGame {
private:
    vector<pair<int, int>> snake;
    pair<int, int> food;
    Direction dir = RIGHT;
    bool gameOver = false;
    int score = 0;

    void spawnFood() {
        food.first = rand() % (WIDTH - 2) + 1;
        food.second = rand() % (HEIGHT - 2) + 1;
        for (const auto& seg : snake) {
            if (seg.first == food.first && seg.second == food.second) {
                spawnFood();
                return;
            }
        }
    }

    bool collision(int x, int y) const {
        if (x <= 0 || x >= WIDTH - 1 || y <= 0 || y >= HEIGHT - 1) return true;
        for (size_t i = 1; i < snake.size(); ++i) {
            if (snake[i].first == x && snake[i].second == y) return true;
        }
        return false;
    }

public:
    SnakeGame() {
        srand(static_cast<unsigned>(time(nullptr)));
        snake.push_back({WIDTH / 2, HEIGHT / 2});
        spawnFood();
    }

    ~SnakeGame() {
#ifndef _WIN32
        disableRawMode();
#endif
    }

    void draw() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                bool isSnake = false;
                for (size_t i = 0; i < snake.size(); ++i) {
                    if (snake[i].first == x && snake[i].second == y) {
                        cout << (i == 0 ? 'O' : 'o');
                        isSnake = true;
                        break;
                    }
                }
                if (!isSnake) {
                    if (x == food.first && y == food.second) cout << '*';
                    else if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) cout << '#';
                    else cout << ' ';
                }
            }
            cout << '\n';
        }
        cout << "\n  Score: " << score << "    (W/A/S/D)\n";
        cout << "  Press X to quit\n\n";
    }

    void input() {
        if (keyPressed()) {
            char key = readKey();
            switch (key) {
                case 'w': case 'W': if (dir != DOWN) dir = UP; break;
                case 's': case 'S': if (dir != UP)   dir = DOWN; break;
                case 'a': case 'A': if (dir != RIGHT) dir = LEFT; break;
                case 'd': case 'D': if (dir != LEFT)  dir = RIGHT; break;
                case 'x': case 'X': gameOver = true; break;
            }
        }
    }

    void update() {
        auto head = snake[0];
        pair<int, int> newHead = head;
        switch (dir) {
            case UP:    newHead.second--; break;
            case DOWN:  newHead.second++; break;
            case LEFT:  newHead.first--;  break;
            case RIGHT: newHead.first++;  break;
            default: return;
        }
        if (collision(newHead.first, newHead.second)) {
            gameOver = true;
            return;
        }
        snake.insert(snake.begin(), newHead);
        if (newHead.first == food.first && newHead.second == food.second) {
            score++;
            spawnFood();
        } else {
            snake.pop_back();
        }
    }

    void run() {
#ifdef _WIN32
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
#else
        enableRawMode();
#endif
        while (!gameOver) {
            draw();
            input();
            int delay = MAX_DELAY_MS - (score * 8);
            if (delay < MIN_DELAY_MS) delay = MIN_DELAY_MS;
#ifdef _WIN32
            Sleep(delay);
#else
            usleep(delay * 1000);
#endif
            update();
        }
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        cout << "  ╔══════════════════════╗\n";
        cout << "  ║     GAME OVER!       ║\n";
        cout << "  ║   Final Score: " << score << "   ║\n";
        cout << "  ╚══════════════════════╝\n\n";
        cout << "  Press Enter to exit...\n";
        cin.get();
    }
};

int main() {
    cout << "Snake Game\nPress any key to start...\n";
    cin.get();
    SnakeGame game;
    game.run();
    return 0;
}