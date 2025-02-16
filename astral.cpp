#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

using namespace std;
using namespace sf;

const int WIDTH = 800;
const int ROWS = 50;
const int CELL_SIZE = WIDTH / ROWS;

// Colors
const Color RED(255, 0, 0);
const Color GREEN(0, 255, 0);
const Color BLUE(0, 255, 0);
const Color YELLOW(255, 255, 0);
const Color WHITE(255, 255, 255);
const Color BLACK(0, 0, 0);
const Color PURPLE(128, 0, 128);
const Color ORANGE(255, 165, 0);
const Color GREY(128, 128, 128);
const Color TURQUOISE(64, 224, 208);

class Spot {
public:
    int row, col;
    float x, y;
    Color color;
    vector<Spot*> neighbors;
    
    Spot(int r, int c) : row(r), col(c), color(WHITE) {
        x = col * CELL_SIZE;
        y = row * CELL_SIZE;
    }

    void draw(RenderWindow& window) {
        RectangleShape rect(Vector2f(CELL_SIZE, CELL_SIZE));
        rect.setPosition(x, y);
        rect.setFillColor(color);
        rect.setOutlineColor(GREY);
        rect.setOutlineThickness(1);
        window.draw(rect);
    }

    void add_neighbors(vector<vector<Spot>>& grid) {
        neighbors.clear();
        if (row < ROWS - 1 && grid[row + 1][col].color != BLACK) neighbors.push_back(&grid[row + 1][col]);
        if (row > 0 && grid[row - 1][col].color != BLACK) neighbors.push_back(&grid[row - 1][col]);
        if (col < ROWS - 1 && grid[row][col + 1].color != BLACK) neighbors.push_back(&grid[row][col + 1]);
        if (col > 0 && grid[row][col - 1].color != BLACK) neighbors.push_back(&grid[row][col - 1]);
    }
};

float heuristic(Spot* a, Spot* b) {
    return abs(a->row - b->row) + abs(a->col - b->col);
}

void reconstruct_path(unordered_map<Spot*, Spot*>& came_from, Spot* current, RenderWindow& window, vector<vector<Spot>>& grid) {
    while (came_from.find(current) != came_from.end()) {
        current = came_from[current];
        current->color = PURPLE;
        window.clear(WHITE);
        for (auto& row : grid)
            for (auto& spot : row)
                spot.draw(window);
        window.display();
    }
}

bool a_star(RenderWindow& window, vector<vector<Spot>>& grid, Spot* start, Spot* end) {
    priority_queue<pair<float, Spot*>, vector<pair<float, Spot*>>, greater<>> open_set;
    unordered_map<Spot*, Spot*> came_from;
    unordered_map<Spot*, float> g_score, f_score;
    
    for (auto& row : grid)
        for (auto& spot : row)
            g_score[&spot] = f_score[&spot] = INFINITY;

    g_score[start] = 0;
    f_score[start] = heuristic(start, end);
    open_set.push({0, start});

    while (!open_set.empty()) {
        Spot* current = open_set.top().second;
        open_set.pop();

        if (current == end) {
            reconstruct_path(came_from, end, window, grid);
            end->color = TURQUOISE;
            return true;
        }

        for (Spot* neighbor : current->neighbors) {
            float temp_g_score = g_score[current] + 1;

            if (temp_g_score < g_score[neighbor]) {
                came_from[neighbor] = current;
                g_score[neighbor] = temp_g_score;
                f_score[neighbor] = temp_g_score + heuristic(neighbor, end);
                open_set.push({f_score[neighbor], neighbor});
                neighbor->color = GREEN;
            }
        }

        if (current != start)
            current->color = RED;

        window.clear(WHITE);
        for (auto& row : grid)
            for (auto& spot : row)
                spot.draw(window);
        window.display();
    }

    return false;
}

int main() {
    RenderWindow window(VideoMode(WIDTH, WIDTH), "A* Pathfinding Algorithm");

    vector<vector<Spot>> grid(ROWS, vector<Spot>(ROWS, Spot(0, 0)));
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < ROWS; j++)
            grid[i][j] = Spot(i, j);

    Spot* start = nullptr;
    Spot* end = nullptr;
    
    bool running = true;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (Mouse::isButtonPressed(Mouse::Left)) {
                Vector2i pos = Mouse::getPosition(window);
                int row = pos.y / CELL_SIZE;
                int col = pos.x / CELL_SIZE;
                if (row >= 0 && row < ROWS && col >= 0 && col < ROWS) {
                    Spot* spot = &grid[row][col];
                    if (!start) {
                        start = spot;
                        start->color = ORANGE;
                    } else if (!end && spot != start) {
                        end = spot;
                        end->color = TURQUOISE;
                    } else if (spot != start && spot != end) {
                        spot->color = BLACK;
                    }
                }
            }

            if (Mouse::isButtonPressed(Mouse::Right)) {
                Vector2i pos = Mouse::getPosition(window);
                int row = pos.y / CELL_SIZE;
                int col = pos.x / CELL_SIZE;
                if (row >= 0 && row < ROWS && col >= 0 && col < ROWS) {
                    Spot* spot = &grid[row][col];
                    spot->color = WHITE;
                    if (spot == start) start = nullptr;
                    if (spot == end) end = nullptr;
                }
            }

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Space && start && end) {
                    for (auto& row : grid)
                        for (auto& spot : row)
                            spot.add_neighbors(grid);

                    a_star(window, grid, start, end);
                }

                if (event.key.code == Keyboard::C) {
                    start = end = nullptr;
                    grid.assign(ROWS, vector<Spot>(ROWS, Spot(0, 0)));
                    for (int i = 0; i < ROWS; i++)
                        for (int j = 0; j < ROWS; j++)
                            grid[i][j] = Spot(i, j);
                }
            }
        }

        window.clear(WHITE);
        for (auto& row : grid)
            for (auto& spot : row)
                spot.draw(window);
        window.display();
    }

    return 0;
}
