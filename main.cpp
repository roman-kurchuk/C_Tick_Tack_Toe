// tictactoe.c
// Single-file tic-tac-toe with Human vs Human and Human vs Computer (Minimax)
// Compile: gcc -std=c11 -O2 -o tictactoe tictactoe.c
// Run: ./tictactoe

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SIZE 9

void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void print_board(const char b[SIZE]) {
    puts("");
    for (int r = 0; r < 3; ++r) {
        printf(" %c | %c | %c \n", b[r*3 + 0], b[r*3 + 1], b[r*3 + 2]);
        if (r < 2) puts("---+---+---");
    }
    puts("");
}

int check_winner(const char b[SIZE]) {
    // Return 'X' or 'O' ascii code for winner, 'D' for draw, 0 for game ongoing
    const int lines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // rows
        {0,3,6}, {1,4,7}, {2,5,8}, // cols
        {0,4,8}, {2,4,6}           // diags
    };
    for (int i = 0; i < 8; ++i) {
        int a = lines[i][0], b1 = lines[i][1], c = lines[i][2];
        if (b[a] != ' ' && b[a] == b[b1] && b[a] == b[c]) return b[a];
    }
    // check draw
    for (int i = 0; i < SIZE; ++i) if (b[i] == ' ') return 0;
    return 'D';
}

int minimax(char board[SIZE], int depth, int isMax) {
    int res = check_winner(board);
    if (res == 'X') return 10 - depth;   // prefer faster win
    if (res == 'O') return -10 + depth;  // prefer slower loss
    if (res == 'D') return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < SIZE; ++i) {
            if (board[i] == ' ') {
                board[i] = 'X';
                int val = minimax(board, depth + 1, 0);
                board[i] = ' ';
                if (val > best) best = val;
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < SIZE; ++i) {
            if (board[i] == ' ') {
                board[i] = 'O';
                int val = minimax(board, depth + 1, 1);
                board[i] = ' ';
                if (val < best) best = val;
            }
        }
        return best;
    }
}

int computer_move(char board[SIZE]) {
    // Computer plays 'O' and assumes human is 'X'. We choose move minimizing score.
    int bestVal = 1000;
    int bestMove = -1;
    for (int i = 0; i < SIZE; ++i) {
        if (board[i] == ' ') {
            board[i] = 'O';
            int moveVal = minimax(board, 0, 1); // next is maximizer
            board[i] = ' ';
            if (moveVal < bestVal) {
                bestVal = moveVal;
                bestMove = i;
            }
        }
    }
    if (bestMove != -1) board[bestMove] = 'O';
    return bestMove;
}

void human_move(char board[SIZE], char symbol) {
    int pos = -1;
    while (1) {
        printf("Player %c, enter your move (1-9): ", symbol);
        int ret = scanf("%d", &pos);
        if (ret != 1) {
            printf("Invalid input. Please enter a number 1-9.\n");
            clear_stdin();
            continue;
        }
        clear_stdin();
        if (pos < 1 || pos > 9) {
            printf("Number out of range. Choose 1-9.\n");
            continue;
        }
        pos -= 1; // zero-based
        if (board[pos] != ' ') {
            printf("Cell already taken. Choose another.\n");
            continue;
        }
        board[pos] = symbol;
        break;
    }
}

void show_instructions(void) {
    puts("Tic-Tac-Toe board positions:");
    puts(" 1 | 2 | 3 ");
    puts("---+---+---");
    puts(" 4 | 5 | 6 ");
    puts("---+---+---");
    puts(" 7 | 8 | 9 ");
    puts("");
    puts("X always goes first.");
    puts("");
}

int play_game(int mode) {
    // mode: 1 = H vs H, 2 = H vs CPU (human X)
    char board[SIZE];
    for (int i = 0; i < SIZE; ++i) board[i] = ' ';
    char turn = 'X';
    print_board(board);
    while (1) {
        int result = check_winner(board);
        if (result) {
            if (result == 'D') {
                print_board(board);
                puts("It's a draw!");
            } else {
                print_board(board);
                printf("Player %c wins!\n", result);
            }
            return 0;
        }

        if (mode == 2) {
            if (turn == 'X') {
                print_board(board);
                human_move(board, 'X');
            } else {
                // computer's turn
                int mv = computer_move(board);
                // if something unexpected, choose first free
                if (mv == -1) {
                    for (int i = 0; i < SIZE; ++i) if (board[i] == ' ') { board[i] = 'O'; break; }
                }
                printf("Computer (O) played.\n");
            }
        } else {
            // Human vs Human
            print_board(board);
            human_move(board, turn);
        }

        // switch turn
        turn = (turn == 'X') ? 'O' : 'X';
    }
}

int main(void) {
    puts("Welcome to Tic-Tac-Toe (single-file C)");
    show_instructions();

    while (1) {
        puts("Choose mode:");
        puts(" 1. Human vs Human");
        puts(" 2. Human (X) vs Computer (O)");
        puts(" Q. Quit");
        printf("Select: ");
        int ch = getchar();
        clear_stdin();
        if (ch == EOF) break;
        ch = toupper(ch);
        if (ch == '1' || ch == '2') {
            int mode = ch - '0';
            play_game(mode);
            // ask to play again
            printf("Play again (y/n)? ");
            int c = getchar();
            clear_stdin();
            if (c == EOF) break;
            if (tolower(c) == 'y') continue;
            else break;
        } else if (ch == 'Q') {
            break;
        } else {
            puts("Invalid choice. Try again.");
        }
    }

    puts("Goodbye!");
    return 0;
}
