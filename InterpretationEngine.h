#ifndef _INTERPRETATION_ENGINE_
#define _INTERPRETATION_ENGINE_

#include <deque> 
#include <memory>
#include "Command.h"

/******************* INTERPRETATION ENGINE *******************/

//PImpl to store keys of accepted commands
struct PImplIntEngine {
    std::set<std::string> commands_;
    std::map<std::string, std::string> cmdAlias;    //future -> adapt command renaming
    bool isValidCmd;
};

class InterpretationEngine {
    private:
        InterpretationEngine();
        static std::shared_ptr<InterpretationEngine> inpretEngine_;     //singleton global obj
        std::shared_ptr<PImplIntEngine> PIntEng_;                       //private impl

        //utility methods
        bool assertValidCommand();
        void assertValidCla(std::string);
        //Interpretation specific
        int getMultiplicity(std::string);
        std::string getPostCommand(std::string);
        std::string getRealCommand(std::string);
        std::string getFullCommand(std::string);
        std::deque<std::string> MultiplyCommand(std::deque<std::string>, int);

        //Special Member Functions
        InterpretationEngine(const InterpretationEngine&) = delete;                 //prevent copying the class         
        InterpretationEngine& operator= (const InterpretationEngine&) = delete;     //prevent copying the class

    public:
        //get global Interpretation Engine instance
        static std::shared_ptr<InterpretationEngine> instance();        
        std::deque<std::string> interpretCommands(std::istream&);
        std::deque<std::string> interpretCLAs(int argc, char** argv);

        //mutators
        void removeAlias(std::string);
        void addAlias(std::string, std::string);
        void addCommand(std::string);
        void removeCommand(std::string);
        void setValidCommand();
        void setInvalidCommand();

        //utility methods
        bool assertRecognizedCommand(std::string);
        std::string decodeAliasCommand(std::string);
};

#endif
