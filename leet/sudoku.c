#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef unsigned char uchar;
typedef enum {row, col, sqr} segment_type;

#define pos_row(p) ((p) / (uchar)9)
#define pos_col(p) ((p) % (uchar)9)
#define get_pos(row, col) (uchar)((row) * 9 + (col))

#define pos_sqr(p) (get_pos(pos_row(p) % 3, pos_col(p) % 3))

typedef unsigned short pos_state;

static const uchar test_board[] = "53..7....6..195....98....6.8...6...34..8.3..17...2...6.6....28....419..5....8..79";
static const uchar test_board_solution[] = "534678912672195348198342567859761423426853791713924856961537284287419635345286179";


uchar* Generate(void)
{
    uchar* puzzle = calloc(81, 1);

    srand(9);

    uchar pos_state[9] = { 0 };

    puzzle[0] = rand() % 9 + 1;

    return puzzle;
}

void SqrState(const uchar* board, uchar pos, uchar* state) 
{
    uchar topLeft = pos - pos_sqr(pos);

    for (uchar row = 0; row < 3; row++)
    {
        for (uchar col = 0; col < 3; col++)
        {
            uchar index = topLeft + (row * 9 + col);
            if (board[index] != 0) state[board[index] - 1] = 1;
        }        
    }    
}

void ColState(const uchar* board, uchar pos, uchar* state) 
{
    uchar col_start = pos_col(pos);

    for (uchar i = 0; i < 9; i++)
    {
        uchar index = col_start + (i * 9);
        if (board[index] != 0) state[board[index] - 1] = 1;
    }
}

void RowState(const uchar* board, uchar pos, uchar* state)
{
    uchar row = pos_row(pos);

    for (uchar i = 0; i < 9; i++)
    {
        uchar index = row * 9 + i;
        if (board[index] != 0) state[board[index] - 1] = 1;
    }
}

void PositionOptions(const uchar* board, uchar pos, uchar* solutions)
{
    RowState(board, pos, solutions);    
    ColState(board, pos, solutions);
    SqrState(board, pos, solutions); 
}

uchar NextPos(const uchar* board, uchar pos, const uint* trail)
{    
    uchar rv = 81;    
    uint min_trail = 959595;

    uchar row = pos_row(pos) * 9;
    for (uchar i = row; i < row + 9; i++)
    {
        if (pos == i) continue;
        if (board[i] == 0 && trail[i] < min_trail)
        {
            min_trail = trail[i];
            rv = i;
        }
    }

    uchar col = pos_col(pos);
    for (uchar i = col; i < 81; i += 9)
    {
        if (pos == i) continue;
        if (board[i] == 0 && trail[i] < min_trail)
        {
            min_trail = trail[i];
            rv = i;
        }
    }   

    uchar sqr = pos - pos_sqr(pos);
    for (uchar i = 0; i < 3; i++)
    {
        for (uchar j = 0; j < 3; j++)
        {
            uchar index = sqr + get_pos(i, j);
            if (pos == index) continue;

            if (board[index] == 0 && trail[index] < min_trail)
            {
                min_trail = trail[index];
                rv = index;
            }
        }
    }

    // if (rv == 81) rv = NextPos2(board, pos, trail);

    return rv;
}


size_t steps = 0;

void Solve(uchar* board, uchar pos, uint* trail)
{
    ++steps;
    if (81 == pos) return;
    ++trail[pos];

    static uchar position_state[9] = { 0 };
        
    memset(position_state, 0, 9);
    PositionOptions(board, pos, position_state);

    uchar pos_solution = 0;

    for (uchar i = 0; i < 9; i++)
    {        
        if (position_state[i] == 0)
        {
            if (pos_solution != 0) 
            {
                pos_solution = 0;
                break;
            }
            pos_solution = i + 1;
        }
    }

    board[pos] = pos_solution;

    Solve(board, NextPos(board, pos, trail), trail);
}


// board is a standard 9x9 
uchar* solveSudoku(const uchar* _board){

    // translate
    uchar* board = calloc(81, sizeof(uchar));
    uint* trail = calloc(81, sizeof(uint));

    for (uchar i = 0; i < 81; i++)
    {
        uchar val = _board[i] == '.' ? 0 : _board[i] - '0';
        board[i] = val;
    }

    Solve(board, 0, trail);
    free(trail);

    return board;
}


int main(int argc, char const *argv[])
{
    uchar* solved = solveSudoku(test_board);

    for (uchar i = 0; i < 81; i++)
    {
        solved[i] = solved[i] + '0';
    }    

    printf("%d\n", strncmp(test_board_solution, solved, 81));
    printf("in %lu steps\n", steps);

    free(solved);

    return 0;
}
