#include "TextDisplay.h"
#include "coordinate.h"
#include <iostream>

using namespace std;

TextDisplay::TextDisplay(shared_ptr<BoardSubject> bs)
{
    td_ = make_shared<PImplTextDisplay> ();
    td_->subject = bs;
    td_->subject->attach(this);
}

TextDisplay::~TextDisplay(){
    td_->subject->detach(this);
}

void TextDisplay::update() {
    cout << *this;
}

unsigned int TextDisplay::getNumRows() const {
    return td_->subject->getNumRows();
}
unsigned int TextDisplay::getNumCols() const {
    return td_->subject->getNumCols();
}
unsigned int TextDisplay::getNumResRows() const {
    return td_->subject->getNumResRows();
}

char TextDisplay::getBoardCharacter(int row, int col) const
{
    return td_->subject->getBoardCharacter(row, col);
}

void TextDisplay::printTopInfo(ostream& sout) const {
    //build top sectino of board display and return it
    sout << "    Level: " << td_ -> subject -> getLevelNum() << endl;
    sout << "    Score: " << td_ -> subject -> getScore() << endl;
    sout << "    HiScore: " << td_ -> subject -> getHiScore() << endl;
}

ostream& operator<<(ostream& sout, const TextDisplay& u)
{
    //Add level, hiscore, and score
    //print top -------
    u.printTopInfo(sout);
    //4 empty space
    sout << "   |";
    for (unsigned int i = 0; i < u.getNumCols(); i++){
        sout << "-";
    }
    sout << "|\n";

    //print reserve rows
    for(unsigned int i = 0; i < u.getNumResRows(); i++)
    {
        sout << (i+1) << "  |";
        for(unsigned int j = 0; j < u.getNumCols(); j++)
        {
            if(u.td_->subject->floatingBlockIsHere(i+1, j+1))
            {
                sout << u.td_->subject->getFloatingBlockType();
            }
            else
            {
                sout << " ";
            }
        }
        sout << "|\n";
    }

    //print actual rows
    for(unsigned int i = 0; i < u.getNumRows(); i++)
    {
        sout << (i+1+u.getNumResRows());
        if(i+1+u.getNumResRows() < 10)//extra space if single digit row number
        {
            sout << " ";
        }
        sout << " |";

        for(unsigned int j = 0; j < u.getNumCols(); j++)
        {
            if(u.td_->subject->floatingBlockIsHere(i+1+u.getNumResRows(), j+1))
            {
                sout << u.td_->subject->getFloatingBlockType();
            }
            else
            {
                sout << u.getBoardCharacter(i, j);
            }
        }
        sout << "|\n";
    }

    //print bottom -----
    //4 empty space
    sout << "    ";
    for (unsigned int i = 0; i < u.getNumCols(); i++){
        sout << "-";
    }
    sout << '\n';
    
    //print next block
    int spaceCounter = 0;
    char type = u.td_->subject->getUpcomingBlockType();
    sout << "    Next:\n";
    vector<shared_ptr<coordinate>> nextBlock = u.td_->subject->getUpcomingBlock();
    //Next block's row value will be from 4-6 (initial position of blocks)
    for(int i = 4; i <= 6; i++)
    {
        sout << "    ";
        spaceCounter = 0;
        //For each row, print all block parts within that row
        for(int j = 0; j < nextBlock.size(); j++)
        {
            if(nextBlock[j]->row_ == i)
            {
                for(int k = 1; k < nextBlock[j]->col_-spaceCounter; k++)//print spaces
                {
                    sout << " ";
                }
                sout << type;
                spaceCounter += nextBlock[j]->col_-spaceCounter;
            }
        }
        sout << "\n";
    }

    return sout;
}
