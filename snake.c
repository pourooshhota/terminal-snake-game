#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_SNAKE 100
#define HIGHSCORE_FILE "highscore.txt"

typedef struct {
    int x, y;
} Point;

Point snake[MAX_SNAKE];
Point food;

int length, dx, dy, score, highscore;
int game_over = 0;
int paused = 0;
int speed;

void load_highscore() {
    FILE *f = fopen(HIGHSCORE_FILE, "r");
    if (f) {
        fscanf(f, "%d", &highscore);
        fclose(f);
    } else {
        highscore = 0;
    }
}

void save_highscore() {
    if (score > highscore) {
        FILE *f = fopen(HIGHSCORE_FILE, "w");
        if (f) {
            fprintf(f, "%d", score);
            fclose(f);
        }
    }
}

void reset_game() {
    length = 3;
    dx = 1;
    dy = 0;
    score = 0;
    speed = 120000;
    paused = 0;
    game_over = 0;

    for (int i = 0; i < length; i++) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }

    food.x = rand() % (WIDTH - 2) + 1;
    food.y = rand() % (HEIGHT - 2) + 1;
}

void init_game() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    srand(time(NULL));

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);  // snake
    init_pair(2, COLOR_RED, COLOR_BLACK);    // food
    init_pair(3, COLOR_CYAN, COLOR_BLACK);   // border
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // text

    load_highscore();
    reset_game();
}

void draw_border() {
    attron(COLOR_PAIR(3));
    for (int i = 0; i <= WIDTH; i++) {
        mvaddch(0, i, '#');
        mvaddch(HEIGHT, i, '#');
    }
    for (int i = 0; i <= HEIGHT; i++) {
        mvaddch(i, 0, '#');
        mvaddch(i, WIDTH, '#');
    }
    attroff(COLOR_PAIR(3));
}

void draw() {
    clear();
    draw_border();

    attron(COLOR_PAIR(2));
    mvaddch(food.y, food.x, 'O');
    attroff(COLOR_PAIR(2));

    attron(COLOR_PAIR(1));
    for (int i = 0; i < length; i++)
        mvaddch(snake[i].y, snake[i].x, i == 0 ? 'X' : 'o');
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(4));
    mvprintw(HEIGHT + 1, 0,
             "Score: %d  High Score: %d  [P]ause  [R]estart  [Q]uit",
             score, highscore);

    if (paused)
        mvprintw(HEIGHT / 2, WIDTH / 2 - 4, "PAUSED");

    attroff(COLOR_PAIR(4));
    refresh();
}

void input() {
    int ch = getch();
    switch (ch) {
        case KEY_UP:    if (dy == 0) { dx = 0; dy = -1; } break;
        case KEY_DOWN:  if (dy == 0) { dx = 0; dy = 1; }  break;
        case KEY_LEFT:  if (dx == 0) { dx = -1; dy = 0; } break;
        case KEY_RIGHT: if (dx == 0) { dx = 1; dy = 0; }  break;
        case 'p': paused = !paused; break;
        case 'r': reset_game(); break;
        case 'q': game_over = 1; break;
    }
}

void logic() {
    if (paused) return;

    for (int i = length - 1; i > 0; i--)
        snake[i] = snake[i - 1];

    snake[0].x += dx;
    snake[0].y += dy;

    /* WALL WRAP MODE */
    if (snake[0].x <= 0) snake[0].x = WIDTH - 1;
    if (snake[0].x >= WIDTH) snake[0].x = 1;
    if (snake[0].y <= 0) snake[0].y = HEIGHT - 1;
    if (snake[0].y >= HEIGHT) snake[0].y = 1;

    for (int i = 1; i < length; i++)
        if (snake[0].x == snake[i].x &&
            snake[0].y == snake[i].y)
            game_over = 1;

    if (snake[0].x == food.x && snake[0].y == food.y) {
        length++;
        score += 10;
        if (speed > 40000) speed -= 5000;

        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
    }
}

void end_game() {
    save_highscore();
    clear();
    mvprintw(HEIGHT / 2 - 1, WIDTH / 2 - 5, "GAME OVER");
    mvprintw(HEIGHT / 2, WIDTH / 2 - 8, "Score: %d", score);
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "High Score: %d", highscore);
    mvprintw(HEIGHT / 2 + 3, WIDTH / 2 - 16, "Press any key to exit");
    nodelay(stdscr, FALSE);
    getch();
    endwin();
}

int main() {
    init_game();

    while (!game_over) {
        input();
        logic();
        draw();
        usleep(speed);
    }

    end_game();
    return 0;
}
