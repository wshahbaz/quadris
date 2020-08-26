#include "GUIBoardArea.h"
#include "coordinate.h"
#include <cairomm/context.h>
#include <iostream>
#include <string>

using namespace std;

//Instantiate colour maps 
std::map<char, int> BoardArea::redValue = {
{'I',66},{'J',0},{'L',245},{'O',255},{'S',0}, {'Z',255}, {'T',144}, {'*',158}, {'?',0},{'W',255},{'X',0},{'V',187}
};
std::map<char, int> BoardArea::greenValue = {
{'I',245},{'J',0},{'L',170},{'O',255},{'S',255}, {'Z',0}, {'T',66}, {'*',95}, {'?',0},{'W',0},{'X',255},{'V',255}
};
std::map<char, int> BoardArea::blueValue = {
{'I',245},{'J',255},{'L',66},{'O',0},{'S',0}, {'Z',0}, {'T',245}, {'*',22}, {'?',0},{'W',174},{'X',183},{'V',0}
};

BoardArea::BoardArea(shared_ptr<BoardSubject> bs) 
{
    ba_ = make_shared<PImplBoardArea>();
    ba_->subject_ = bs;
    ba_->sideLength_ = 30;
    ba_->borderSize_ = 30;
    ba_->textAreaSize_ = 100;
}

BoardArea::~BoardArea() {}

//Fills in a square with the colour corresponding to type
//Coordinate given is the top left corner
void BoardArea::fillSquare(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y, char type)
{
    cr->set_source_rgb(double(redValue[type])/255, double(greenValue[type])/255, double(blueValue[type])/255);
    cr->rectangle(x, y, ba_->sideLength_, ba_->sideLength_);
    cr->fill();
}

//Draw border of a single square given coordinates of the top left corner
void BoardArea::drawSquareBorder(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y)
{
    cr->set_source_rgb(0, 0, 0);
    cr->move_to(x, y);
    cr->line_to(x, y + ba_->sideLength_);
    cr->stroke();
    cr->move_to(x, y);
    cr->line_to(x + ba_->sideLength_, y);
    cr->stroke();
    cr->move_to(x + ba_->sideLength_, y + ba_->sideLength_);
    cr->line_to(x, y + ba_->sideLength_);
    cr->stroke();
    cr->move_to(x + ba_->sideLength_, y + ba_->sideLength_);
    cr->line_to(x + ba_->sideLength_, y);
    cr->stroke();
}

bool BoardArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    Gtk::Allocation allocation = get_allocation();
    //get dimensions of window
    const int width = allocation.get_width();
    const int height = allocation.get_height();
    int x = 0;
    int y = 0;

    //Draw border colours
    cr->set_source_rgb(1.0, 1.0, 1.0);//set colour to white
    cr->rectangle(0, 0, ba_->borderSize_, height);
    cr->rectangle(0, 0, width, ba_->textAreaSize_ + ba_->borderSize_);
    cr->rectangle(width - ba_->borderSize_, 0, ba_->borderSize_, height);
    cr->rectangle(0, height-ba_->borderSize_, width, ba_->borderSize_);
    cr->fill();

    //Draw blocks in the reserve rows
    for(unsigned int i = 0; i < ba_->subject_->getNumResRows(); i++)
    {
        for(unsigned int j = 0; j < ba_->subject_->getNumCols(); j++)
        {
            if(ba_->subject_->floatingBlockIsHere(i+1, j+1))
            {
                x = ba_->borderSize_ + j*ba_->sideLength_;//square coordinates
                y = ba_->borderSize_ + i*ba_->sideLength_ + ba_->textAreaSize_;
                fillSquare(cr, x, y, ba_->subject_->getFloatingBlockType());
            }
        }
    }

    //Draw blocks in the board rows
    for(unsigned int i = 0; i < ba_->subject_->getNumRows(); i++)
    {
        for(unsigned int j = 0; j < ba_->subject_->getNumCols(); j++)
        {
            //coordinate of top left corner of square
            x = ba_->borderSize_ + j*ba_->sideLength_;
            y = ba_->borderSize_ + (i+ba_->subject_->getNumResRows())*ba_->sideLength_ + ba_->textAreaSize_;
            if(ba_->subject_->floatingBlockIsHere(i+1+ba_->subject_->getNumResRows(), j+1))
            {
                fillSquare(cr, x, y, ba_->subject_->getFloatingBlockType());
            }
            else if(ba_->subject_->getBoardCharacter(i, j) != ' ')
            {
                fillSquare(cr, x, y, ba_->subject_->getBoardCharacter(i, j));
            }
        }
    }

    //Draw grid (black lines in between blocks)
    cr->set_line_width(1.5);
    cr->set_source_rgb(0, 0, 0);//set colour to black

    //draw vertical lines
    for(int i = 0; i < ba_->subject_->getNumCols() + 1; i++)
    {
        //top of board
        cr->move_to(ba_->borderSize_ + i*ba_->sideLength_, ba_->borderSize_ + ba_->textAreaSize_);
        //bottom of board
        cr->line_to(ba_->borderSize_ + i*ba_->sideLength_, height - ba_->borderSize_);
        cr->stroke();
    }

    //draw horizontal lines
    for(int i = 0; i < ba_->subject_->getNumRows() + 1 + ba_->subject_->getNumResRows(); i++)
    {
        //left side of board
        cr->move_to(ba_->borderSize_, ba_->borderSize_ + i*ba_->sideLength_ + ba_->textAreaSize_);
        //right side of board
        cr->line_to(width - ba_->borderSize_, ba_->borderSize_ + i*ba_->sideLength_ + ba_->textAreaSize_);
        cr->stroke();
    }

    //Text(Level, Score, HiScore)
    Pango::FontDescription font;
    font.set_family("Calibri");
    font.set_size(20000);

    auto layout = create_pango_layout("Level: " + to_string(ba_->subject_->getLevelNum()));
    cr->move_to(30, 15);
    layout->set_font_description(font);
    layout->show_in_cairo_context(cr);
    layout = create_pango_layout("Score: " + to_string(ba_->subject_->getScore()));
    cr->move_to(30, 50);
    layout->set_font_description(font);
    layout->show_in_cairo_context(cr);
    layout = create_pango_layout("HiScore: " + to_string(ba_->subject_->getHiScore()));
    cr->move_to(30, 85);
    layout->set_font_description(font);
    layout->show_in_cairo_context(cr);

    //Next Block
    layout = create_pango_layout("Next:");//Next label
    cr->move_to(177, 15);
    layout->set_font_description(font);
    layout->show_in_cairo_context(cr);

    vector<shared_ptr<coordinate>> nextBlock = ba_->subject_->getUpcomingBlock();
    for(int i = 0; i < nextBlock.size(); i++)//draw the next block
    {
        x = 240 + (nextBlock[i]->col_-1)*ba_->sideLength_;
        y = 15 + (nextBlock[i]->row_-4)*ba_->sideLength_;
        //Draw block part colour
        fillSquare(cr, x, y, ba_->subject_->getUpcomingBlockType());
        //Draw block part border
        drawSquareBorder(cr, x, y);
    }

    return true;
}
