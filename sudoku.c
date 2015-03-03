#include <stdio.h>
#include <sys/time.h>

char worst_case[9][9] =
{
    {0, 0, 0,   0, 0, 0,   0, 0, 0},
    {0, 0, 0,   0, 0, 3,   0, 8, 5},
    {0, 0, 1,   0, 2, 0,   0, 0, 0},

    {0, 0, 0,   5, 0, 7,   0, 0, 0},
    {0, 0, 4,   0, 0, 0,   1, 0, 0},
    {0, 9, 0,   0, 0, 0,   0, 0, 0},

    {5, 0, 0,   0, 0, 0,   0, 7, 3},
    {0, 0, 2,   0, 1, 0,   0, 0, 0},
    {0, 0, 0,   0, 4, 0,   0, 0, 9}
};

void print(char matrix[9][9])
{
    int i;
    char *row;

    for (i = 0; i < 9; i++) {
        row = matrix[i];
        printf("%d, %d, %d,   %d, %d, %d,   %d, %d, %d\n",
            row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8]);
        if ((i > 0) && (i % 3 == 2)) {
            printf("\n");
        }
    }
    printf("\n");
}

int solve(char matrix[9][9])
{
    /* Find an empty spot. */
    int x, y, i, j, s = 0;
    char nums[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    for (i = 0; i < 9 && !s; i++) {
        for (j = 0; j < 9 && !s; j++) {
            if (matrix[i][j] == 0) {
                x = i; y = j; s = 1;
            }
        }
    }

    /* No empty spots, we found a solution! */
    if (!s)
        return 1;

    /* Determine legal numbers for this spot. */
    for (i = 0, j = y; i < 9; i++) {
        nums[(int) matrix[i][j]] = 0;   /* Vertically */
    }
    for (i = x, j = 0; j < 9; j++) {
        nums[(int) matrix[i][j]] = 0;   /* Horizontally */
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            nums[(int) matrix[i + ((int) (x / 3)) * 3]
                             [j + ((int) (y / 3)) * 3]
                ] = 0;          /* Within the partition */
        }
    }

    /* Try each possible number and recurse for each. */
    for (i = 1; i < 10; i++) {
        if (nums[i] > 0) {
            matrix[x][y] = i;
            if (solve(matrix))
                return 1;
        }
    }

    /* Each attempt failed: reset this position and report failure. */
    matrix[x][y] = 0;

    return 0;
}

int load(char matrix[9][9])
{
    int i, j;
    char *row;

    for (i = 0; i < 9; i++) {
        printf("Row %d: ", (i + 1));
        row = matrix[i];
        for (j = 0; j < 9; j++) {
            scanf("%d", &row[j]);
            if (row[j] > 9 || row[j] < 0) {
                return 0;
            }
        }
    }

    return 1;
}

int main()
{
    int success;
    unsigned int elapse;
    struct timeval start, end;
    char matrix[9][9];

    printf("Input Puzzle:\n");
    success = load(matrix);
    if (!success) {
        return -1;
    }
    printf("Sudoku Puzzle:\n");
    print(matrix);
    gettimeofday(&start, NULL);
    success = solve(matrix);
    gettimeofday(&end, NULL);

    if (end.tv_usec < start.tv_usec) {
        end.tv_sec -= 1;
        end.tv_usec += 1000000;
    }
    elapse = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

    if (success) {
        printf("Solved (%u.%03u sec):\n", elapse / 1000, elapse % 1000);
        print(matrix);
    } else {
        printf("Cannot be solved.\n");
    }

    return 0;
}

