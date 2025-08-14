#include <ncurses.h>
#include <stdio.h>

#define X_SIZE 80
#define Y_SIZE 25
#define CELL 'o'
#define SPACE ' '

int input_matrix_fix(int array[Y_SIZE][X_SIZE], int M, int N);
void output_matrix_fix(int array[Y_SIZE][X_SIZE]);

void evolve_matrix(int array_start[Y_SIZE][X_SIZE], int array_target[Y_SIZE][X_SIZE], int M, int N);
void fill_surrounding_marix(int start[Y_SIZE][X_SIZE], int target[3][3], int i, int j);
int sum_matrix(int array[3][3]);
int update_value(int sum, int initial_value);
int change_speed(char control_btn, int *flag, int *speed);
int count_of_survivors(int matrix[Y_SIZE][X_SIZE]);
FILE *freop();

int main() {
    int array_start[Y_SIZE][X_SIZE];
    int array_target[Y_SIZE][X_SIZE];
    input_matrix_fix(array_start, Y_SIZE, X_SIZE);
    freop();

    int speed = 51;  // в милисекундах
    int stop = 0;    // флаг

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    while (stop != 1) {
        // проверка на вымирание
        if (count_of_survivors(array_start) == 0) {
            stop = 1;
        }

        clear();

        evolve_matrix(array_start, array_target, Y_SIZE, X_SIZE);
        output_matrix_fix(array_start);

        // управление
        char control_btn = getch();
        if (control_btn != ERR) {
            change_speed(control_btn, &stop, &speed);
        }
        refresh();

        napms(speed);
    }
    attroff(COLOR_PAIR(1));
    endwin();
    return 0;
}

int input_matrix_fix(int array[Y_SIZE][X_SIZE], int M, int N) {
    int err_res = 0;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (scanf("%d", &array[i][j]) != 1) err_res = 3;
        }
    }
    return err_res;
}

// Функция отрисовывает поле в терминаде
void output_matrix_fix(int array[Y_SIZE][X_SIZE]) {
    for (int i = 0; i < Y_SIZE; i++) {
        for (int j = 0; j < X_SIZE; j++) {
            move(i, j);
            printw("%c", array[i][j] ? CELL : SPACE);
        }
        printw("\n");
    }
    refresh();
}

void evolve_matrix(int array_start[Y_SIZE][X_SIZE], int array_target[Y_SIZE][X_SIZE], int M, int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            int surrounding_matrix[3][3];
            fill_surrounding_marix(array_start, surrounding_matrix, i, j);
            int sum = sum_matrix(surrounding_matrix);
            array_target[i][j] = update_value(sum, array_start[i][j]);
        }
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            array_start[i][j] = array_target[i][j];
        }
    }
}

void fill_surrounding_marix(int start[Y_SIZE][X_SIZE], int target[3][3], int i, int j) {
    for (int y = -1; y < 2; y++) {
        for (int x = -1; x < 2; x++) {
            target[y + 1][x + 1] = start[(i + y + Y_SIZE) % Y_SIZE][(j + x + X_SIZE) % X_SIZE];
        }
    }
    target[1][1] = 0;
}

int sum_matrix(int array[3][3]) {
    int sum = 0;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            sum += array[y][x];
        }
    }
    return sum;
}

int update_value(int sum, int initial_value) {
    int new_value = 0;
    if (initial_value == 0) {
        if (sum == 3) {
            new_value = 1;
        }
    } else if (initial_value == 1) {
        if (sum == 3 || sum == 2) {
            new_value = 1;
        } else {
            new_value = 0;
        }
    }
    return new_value;
}

int change_speed(char control_btn, int *flag, int *speed) {
    if (control_btn == 'a' || control_btn == 'A') {
        if (*speed >= 11) *speed -= 10;
    }
    if (control_btn == 'z' || control_btn == 'Z') {
        *speed += 10;
    } else if (control_btn == ' ') {
        *flag = 1;
    }
    return 0;
}

int count_of_survivors(int matrix[Y_SIZE][X_SIZE]) {
    int count = 0;
    for (int i = 0; i < Y_SIZE; i++) {
        for (int j = 0; j < X_SIZE; j++) {
            count += matrix[i][j];
        }
    }
    return count;
}

FILE *freop() {
    FILE *t = freopen("/dev/tty", "r", stdin);
    return t;
}
