

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

//Pour mettre de la couleur
void Color(int couleurDuTexte, int couleurDeFond) // fonction d'affichage de couleurs
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}

//On initialise les variables globales
int rows, cols, mines;
int** grid;
int** revealed;
int** flags;

//Propose au joueur de sélectionner la difficulté
int setDifficulty(){
    int dif;
    printf("Choisir la difficulte (1, 2, 3) : ");
    scanf_s("%d", &dif);
    while (getchar() != '\n');
    if (dif == 1) {
        rows = 9;
        cols = 11;
        mines = 10;
        grid = malloc(sizeof(int*) * rows);
        for (int k = 0; k < rows; k++) {
            grid[k] = malloc(sizeof(int) * cols);
        }
        revealed = malloc(sizeof(int*) * rows);
        for (int i = 0; i < rows; i++) {
            revealed[i] = malloc(sizeof(int) * cols);
        }
        flags = malloc(sizeof(int*) * rows);
        for (int j = 0; j < rows; j++) {
            flags[j] = malloc(sizeof(int) * cols);
        }
    }
    else if (dif == 2) {
        rows = 15;
        cols = 19;
        mines = 40;
        grid = malloc(sizeof(int*) * rows);
        for (int k = 0; k < rows; k++) {
            grid[k] = malloc(sizeof(int) * cols);
        }
        revealed = malloc(sizeof(int*) * rows);
        for (int i = 0; i < rows; i++) {
            revealed[i] = malloc(sizeof(int) * cols);
        }
        flags = malloc(sizeof(int*) * rows);
        for (int j = 0; j < rows; j++) {
            flags[j] = malloc(sizeof(int) * cols);
        }
    }
    else if(dif == 3){
        rows = 21;
        cols = 25;
        mines = 99;
        grid = malloc(sizeof(int*) * rows);
        for (int k = 0; k < rows; k++) {
            grid[k] = malloc(sizeof(int) * cols);
        }
        revealed = malloc(sizeof(int*) * rows);
        for (int i = 0; i < rows; i++) {
            revealed[i] = malloc(sizeof(int) * cols);
        }
        flags = malloc(sizeof(int*) * rows);
        for (int j = 0; j < rows; j++) {
            flags[j] = malloc(sizeof(int) * cols);
        }
    }
}


void generateMines() {
    int i, j, count = 0;

    while (count < mines) {
        i = rand() % rows;
        j = rand() % cols;

        if (grid[i][j] != -1) {
            grid[i][j] = -1;
            if (i > 0) {
                if (grid[i - 1][j] != -1){
                    grid[i - 1][j] ++;
                }
                if (j > 0) {
                    if (grid[i - 1][j - 1] != -1) {
                        grid[i - 1][j - 1] ++;
                    }
                }
                if (j < cols - 1) {
                    if (grid[i - 1][j + 1] != -1) {
                        grid[i - 1][j + 1]++;
                    }
                }
            }

            if (j > 0) {
                if (grid[i][j - 1] != -1) {
                    grid[i][j - 1]++;
                }
                if (i < rows - 1) {
                    if (grid[i + 1][j - 1] != -1) {
                        grid[i + 1][j - 1]++;
                    }
                }
            }

            if (i < rows - 1) {
                if (grid[i + 1][j] != -1) {
                    grid[i + 1][j]++;
                }
                if (j < cols - 1) {
                    if (grid[i + 1][j + 1] != -1) {
                        grid[i + 1][j + 1]++;
                    }
                }
            }

            if (j < cols - 1) {
                if (grid[i][j + 1] != -1) {
                    grid[i][j + 1]++;
                }
            }
            count++;
        }
    }
}

void printGrid() {
    int i, j;

    printf("   ");
    for (j = 0; j < cols; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    for (i = 0; i < rows; i++) {
        printf("%2d ", i);

        for (j = 0; j < cols; j++) {
            if (flags[i][j] == 1 && revealed[i][j] == 0) {
                printf(" ? ");
            }
            else if (revealed[i][j] == 0) {
                printf(" . ");
            }
            else if (grid[i][j]== -1) {
                printf(" * ");
            }
            else {
                Color(grid[i][j], 0);
                printf(" %d ", grid[i][j]);
                Color(15, 0);
            }
        }

        printf("\n");
    }
}

void reveal(int i, int j) {
    if (revealed[i][j] == 1) {
        return;
    }

    revealed[i][j] = 1;

    if (grid[i][j] == -1) {
        return;
    }

    if (grid[i][j] > 0) {
        return;
    }

    if (i > 0) {
        reveal(i - 1, j);
        if (j > 0) {
            reveal(i - 1, j - 1);
        }
        if (j < cols - 1) {
            reveal(i - 1, j + 1);
        }
    }

    if (j > 0) {
        reveal(i, j - 1);
        if (i < rows - 1) {
            reveal(i + 1, j - 1);
        }
    }

    if (i < rows - 1) {
        reveal(i + 1, j);
        if (j < cols - 1) {
            reveal(i + 1, j + 1);
        }
    }

    if (j < cols - 1) {
        reveal(i, j + 1);
    }
}
int main() {
    int x, y;
    char action;

    srand(time(NULL));
    setDifficulty();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            grid[i][j] = 0;
            revealed[i][j] = 0;
            flags[i][j] = 0;
        }
    }

    generateMines();
    while (1) {
        int victory = 1;
        system("CLS");
        printGrid();

        printf(" Entrer les coordonnees (avec un espace entre les deux): ");
        scanf_s("%d %d", &x, &y);
        while (getchar() != '\n');

        if (x < 0 || x >= cols || y < 0 || y >= rows) {
            printf("Valeurs incorrect\n");
            continue;
        }

        printf("Taper r pour reveler la case, et f pour planter un drapeau: ");
        scanf_s("%c", &action, 1);
        while (getchar() != '\n');

        if (action == 'r') {
            if (grid[y][x] == -1) {
                system("CLS");
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        if (grid[i][j] == -1) {
                            revealed[i][j] = 1;
                        }
                    }
                }
                printGrid();
                printf("Une mine a exploser, c'est une defaite\n");
                break;
            }

            reveal(y, x);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (victory == 1) {
                        if (revealed[i][j] == 0 && grid[i][j] != -1) {
                            victory = 0;
                            break;
                        }
                    }
                }
            }
            if (victory == 1) {
                system("CLS");
                printGrid();
                printf("Victoire, aucune mine n'a exploser !\n");
                break;
            }
        }
        if (action == 'f') {
            if (flags[y][x] == 0) {
                flags[y][x] = 1;
            }
            else {
                flags[y][x] = 0;
            }
        }
    }
    for (int k = 0; k < rows; k++) {
        free(grid[k]);
    }
    free(grid);
    for (int i = 0; i < rows; i++) {
        free(revealed[i]);
    }
    free(revealed);
    for (int j = 0; j < rows; j++) {
        free(flags[j]);
    }
    free(flags);
}