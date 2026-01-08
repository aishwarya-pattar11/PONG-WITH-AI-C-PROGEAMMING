#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 40
#define HEIGHT 20
#define PADDLE_SIZE 3
#define WIN_SCORE 20

int ballX, ballY;
int ballDirX, ballDirY;
int playerY, aiY;
int playerScore = 0, aiScore = 0;
int aiReactionTime = 0;

void setup();
void draw();
void playerInput();
void logic();
void clearScreen();
void gameOver(const char *winner);

int main() {
    srand(time(NULL));
    setup();

    while (1) {
        playerInput();
        logic();
        clearScreen();
        draw();
        Sleep(60);
    }
    return 0;
}

void setup() {
    ballX = WIDTH / 2;
    ballY = HEIGHT / 2;

    ballDirX = (rand() % 2 == 0) ? 1 : -1;
    ballDirY = (rand() % 2 == 0) ? 1 : -1;

    playerY = HEIGHT / 2 - PADDLE_SIZE / 2;
    aiY = HEIGHT / 2 - PADDLE_SIZE / 2;
}

void clearScreen() {
    system("cls");
}

void playerInput() {
    if (_kbhit()) {
        char ch = _getch();

        if ((ch == 'w' || ch == 'W')) {
            if (playerY > 1) playerY--;
            if (playerY > 1) playerY--;   // faster movement
        }
        else if ((ch == 's' || ch == 'S')) {
            if (playerY < HEIGHT - PADDLE_SIZE - 1) playerY++;
            if (playerY < HEIGHT - PADDLE_SIZE - 1) playerY++;
        }
    }
}

void draw() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {

            if (y == 0 || y == HEIGHT - 1)
                printf("-");
            else if (x == 0 || x == WIDTH - 1)
                printf("|");
            else if (x == 2 && y >= playerY && y < playerY + PADDLE_SIZE)
                printf("|");
            else if (x == WIDTH - 3 && y >= aiY && y < aiY + PADDLE_SIZE)
                printf("|");
            else if (x == ballX && y == ballY)
                printf("O");
            else
                printf(" ");
        }
        printf("\n");
    }

    printf("\nPlayer: %d   AI: %d\n", playerScore, aiScore);
    printf("First to %d wins | Controls: W / S\n", WIN_SCORE);
}

void logic() {
    ballX += ballDirX;
    ballY += ballDirY;

    if (ballY <= 1 || ballY >= HEIGHT - 2)
        ballDirY = -ballDirY;

    /* Player paddle collision (skill-based) */
    if (ballX == 3 &&
        ballY >= playerY &&
        ballY < playerY + PADDLE_SIZE) {

        ballDirX = 1;

        if (ballY == playerY)
            ballDirY = -1;
        else if (ballY == playerY + PADDLE_SIZE - 1)
            ballDirY = 1;
    }

    /* AI paddle collision (simple) */
    if (ballX == WIDTH - 4 &&
        ballY >= aiY &&
        ballY < aiY + PADDLE_SIZE)
        ballDirX = -1;

    /* Slower AI reaction */
    aiReactionTime++;
    if (aiReactionTime % 4 == 0) {
        if (ballY > aiY + 1 && aiY < HEIGHT - PADDLE_SIZE - 1)
            aiY++;
        else if (ballY < aiY + 1 && aiY > 1)
            aiY--;
    }

    if (ballX <= 1) {
        aiScore++;
        if (aiScore >= WIN_SCORE)
            gameOver("AI");
        setup();
    }
    else if (ballX >= WIDTH - 2) {
        playerScore++;
        if (playerScore >= WIN_SCORE)
            gameOver("PLAYER");
        setup();
    }
}

void gameOver(const char *winner) {
    clearScreen();
    printf("\n=============================\n");
    printf("        %s WINS!\n", winner);
    printf("=============================\n");
    printf("Final Score -> Player: %d  AI: %d\n", playerScore, aiScore);
    printf("\nPress any key to exit...");
    _getch();
    exit(0);
}



