#ifndef BOARDAREA_H
#define BOARDAREA_H
#include <memory>
#include <map>
#include <gtkmm/drawingarea.h>
#include "BoardSubject.h"

struct PImplBoardArea
{
    std::shared_ptr<BoardSubject> subject_;//allows for access to boardSubject data
    int sideLength_;//side length of a square
    int borderSize_;//distance between the board and the edge of the window
    int textAreaSize_;//length of space where the level #, score, etc. are shown
};

//This GUI element stores the actual game board 
//It consists of 18x11 squares that will be filled in
class BoardArea : public Gtk::DrawingArea
{
    public:
        BoardArea(std::shared_ptr<BoardSubject> bs);//Ctor
        virtual ~BoardArea();
        //Maps from chars ('T', 'L', etc.) to RGB colour values (0-255)
        //Each block's colour is the same as its colour in any
        //official Tetris game (e.g. purple for T)
        //Bonus blocks are: Pink (W Block), Lime Green (V Block), Turquoise (X Block)
        static std::map<char, int> redValue;
        static std::map<char, int> greenValue;
        static std::map<char, int> blueValue;

    protected:
        //Override default signal handler:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    private:
        std::shared_ptr<PImplBoardArea> ba_;
        void fillSquare(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y, char type);
        void drawSquareBorder(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y);
};

#endif
