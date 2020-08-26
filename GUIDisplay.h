#ifndef _GUI_DISPLAY_
#define _GUI_DISPLAY_
#include <vector>
#include <memory>
#include "Observer.h"
#include "BoardSubject.h"
#include "GUIBoardArea.h"
#include "coordinate.h"
#include <thread>
#include <gtkmm/window.h>

struct PImplGUIDisplay
{
    std::shared_ptr<BoardSubject> subject;
    int argc_;
    char** argv_;
    bool guiDisplayed_;
    std::shared_ptr<BoardArea> boardDisplay;
};

class GUIDisplay: public Observer 
{
    public:
        GUIDisplay(std::shared_ptr<BoardSubject> bs, int argc, char** argv);
        ~GUIDisplay();
        void update() override;
        char getBoardCharacter(int row, int col) const;

    private:
        std::shared_ptr<PImplGUIDisplay> td_;
        int argc_;
        char** argv_;
        bool guiDisplayed_;
        std::shared_ptr<BoardArea> boardDisplay;
        int displayGUI();
};

#endif
