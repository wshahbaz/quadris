#ifndef _COORDINATE_H_
#define _COORDINATE_H_

struct coordinate 
{
    int row_;
    int col_;
    coordinate(int r, int c);
    void shiftRow(int n);
    void shiftCol(int n);
};

#endif
