#include "GUIDisplay.h"
#include "GUIBoardArea.h"
#include "coordinate.h"
#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <thread>
#include <chrono>

using namespace std;

GUIDisplay::GUIDisplay(shared_ptr<BoardSubject> bs, int argc, char** argv)
{
    td_ = make_shared<PImplGUIDisplay> ();
    td_->subject = bs;
    td_->subject->attach(this);
    td_->argc_ = argc;
    td_->argv_ = argv;
    td_->guiDisplayed_ = false;
}

GUIDisplay::~GUIDisplay()
{
    td_->subject->detach(this);
}

//Open the GUI application and run it
int GUIDisplay::displayGUI()
{
    auto gtkApp = Gtk::Application::create(td_->argc_, td_->argv_, "org.gtkmm.examples.base");
    Gtk::Window mainWindow;
    mainWindow.set_default_size(390, 700);
    td_->boardDisplay = shared_ptr<BoardArea>(new BoardArea(td_->subject));
    mainWindow.add(*td_->boardDisplay);
    td_->boardDisplay->show();
    return gtkApp->run(mainWindow);
}

void GUIDisplay::update() 
{
    //Open window
    if(!td_->guiDisplayed_)
    {
        //Separate thread for GUI
        thread guiThread_(&GUIDisplay::displayGUI, this);
        td_->guiDisplayed_ = true;
        //Allow initial board to be displayed
        this_thread::sleep_for(chrono::seconds(5));
        guiThread_.detach();//detach GUI thread
    }
    else
    {
        //Redraw the board
        td_->boardDisplay->hide();
        td_->boardDisplay->show();
    }
}
