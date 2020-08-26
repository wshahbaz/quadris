#ifndef _TEXT_DISPLAY_
#define _TEXT_DISPLAY_

#include "Observer.h"
#include "BoardSubject.h"
#include "coordinate.h"
#include <vector>
#include <memory>

struct PImplTextDisplay{
    std::shared_ptr<BoardSubject> subject;
};

class TextDisplay: public Observer {
    std::shared_ptr<PImplTextDisplay> td_;
    public:
    TextDisplay(std::shared_ptr<BoardSubject> bs);
    ~TextDisplay();
    void update() override;
    friend std::ostream& operator<<(std::ostream& sout, const TextDisplay& u);
    unsigned int getNumRows() const;
    unsigned int getNumCols() const;
    unsigned int getNumResRows() const;
    char getBoardCharacter(int row, int col) const;

    private:
        //helper methods
        //print current level, score, etc
        void printTopInfo(std::ostream&) const;
};

#endif
