#include "coordinate.h"

coordinate::coordinate(int r, int c)
{
    row_ = r;
    col_ = c;
}

void coordinate::shiftRow(int n)
{
    row_+=n;
}

void coordinate::shiftCol(int n)
{
    col_+=n;
}
