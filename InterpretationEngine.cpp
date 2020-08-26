#include <iostream>
#include <assert.h>
#include <vector>
#include "InterpretationEngine.h"
#include "Command.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
////////////////////// INTERPRETATION ENGINE CLASS START //////////////////////
///////////////////////////////////////////////////////////////////////////////

//initialize singleton intEngine_ to nullptr
shared_ptr<InterpretationEngine> InterpretationEngine::inpretEngine_ = nullptr;

//constructor
InterpretationEngine::InterpretationEngine() {
    PIntEng_ = make_shared<PImplIntEngine> ();
    PIntEng_->commands_ = COMMANDS;
}

shared_ptr<InterpretationEngine> InterpretationEngine::instance() {
    if (inpretEngine_ == nullptr) {
        inpretEngine_ = shared_ptr<InterpretationEngine>(new InterpretationEngine());
    }
    return inpretEngine_;
}

//CORE LOGIC FUNCTIONS
//process input for commands. Only primitive commands atm
deque<string> InterpretationEngine::interpretCommands(istream& sin) {
    //SKIP WHITESPACE
    sin >> skipws;
    //set up
    deque <string> result;
    string cmd = "";
    sin >> cmd;
    int multiplicity = getMultiplicity(cmd);        //determine multiplicity of command
    string realCmd = getRealCommand(cmd);           //determine real command
    //check validity of cmd
    if (!assertValidCommand()) {
        cout << "Command " << realCmd << " is invalid" << endl;
        return result; 
    };
    result.push_back(realCmd);
    //check for arguments
    int subIter = 0;
    while (subIter < COMMAND_ARGS.at(realCmd)) {
        string arg = "";
        sin >> arg;
        //note no checking of validity of arg atm
        result.push_back(arg);
        subIter++;  //increment subIter (over numArgs for this command)
    }
    deque<string> mainCmds = MultiplyCommand(result, multiplicity);
    string postCmd = getPostCommand(realCmd);
    if (postCmd != "") {
        mainCmds.push_back(postCmd);
    }
    return mainCmds;
}

//process input for Command Line Arguments
//since user's cannot define compound (and recursive) clas, we can linearlly process argv
deque<string> InterpretationEngine::interpretCLAs(int argc, char** argv) {
    deque<string> result;
    int iter = 1;
    while (iter < argc) {
        string cla = argv[iter];
        //check validity of cla
        assertValidCla(cla);
        result.push_back(cla);      //valid, add to deque
        iter++; 
        //check for arguments
        int subIter = 0;
        while (subIter < CLA_ARGS.at(cla)) {
            if (iter >= argc) {
                cerr << "Illegal Number of Command Line Argument - Missing argument(s) for command \"" + cla 
                    << "\"" << endl << "Exiting program" << endl;
                exit(1);
            }
            //note no checking of validity of arg atm
            result.push_back(argv[iter]);
            subIter++;  //increment subIter (over numArgs for this command)
            iter++;     //increment Iter over all elements
        }
    }
    return result;
}

//retrive what command to execute after cmd
string InterpretationEngine::getPostCommand(string cmd) {
    //depending on command, apply any commmands that must
    //occur after that cmd (or set of those commands)
    if (cmd == "left" || cmd == "right" || cmd == "down"
        || cmd == "clockwise" || cmd == "counterclockwise") {
        //directional block move
        return "POSTMOVE";
    } 
    else if (cmd == "sequence") {
        //post execution of sequence(s)
        return "POSTSEQUENCE";
    }
    else {
        return "";
    }
}

//get the multiplicity of the command from input
int InterpretationEngine::getMultiplicity(string cmd) {
    string mult = "";
    //get all prefixed chars
    for (int i = 0; i < cmd.length(); i++) {
        if (isdigit(cmd[i])) {
            string curr(1, cmd[i]);
            mult.append(curr);
        } else break;
    }
    //return multiplicity as a number
    int res = (mult.length() == 0)? 1 : stoi(mult);
    if (mult.length() == 0) return 1;
    else return stoi(mult);
}

//get the parsed command (ie without numbers)
string InterpretationEngine::getRealCommand(string cmd) {
    //remove any prefixed numbers
    int numNumbers = 0;
    for (int i = 0; i < cmd.length(); i++) {
        if (isdigit(cmd[i])) {
            numNumbers++;
        } else break;
    }
    //get full form of command if prefixed
    return getFullCommand(cmd.substr(numNumbers));
}

//get the real, recognized command from input
string InterpretationEngine::getFullCommand(string cmd) {
    //if cmd is already a full command name, return it
    //need to consider if it's an aliased name as well
    if (assertRecognizedCommand(cmd)) {
        setValidCommand();
        return decodeAliasCommand(cmd);
    }
    //char by char compare with other commands until a unique 
    vector<string> possibleCmds;
    for (int i = 0; i < cmd.length(); i++) {
        for (auto &realCmd : PIntEng_->commands_) {
            //if real command and cmd are the same up to i, add realCommand
            //to list of possible commands determined from this prefix
            if (cmd.substr(0, i+1) == realCmd.substr(0, i+1) && cmd.length() <= realCmd.length()) {
                possibleCmds.push_back(realCmd);
            }
        }
        //if only one possible command determined, then we have unique rep
        if (possibleCmds.size() == 1) {
            setValidCommand();
            //in case of an aliased command, must return real command
            return decodeAliasCommand(possibleCmds.front());
        }
        //else clear possible commands and check again by increasing prefix depth
        possibleCmds.clear();
    }
    //if nothing returned upon termination of loop, either nothing is represented by cmd
    //or cmd represents multiple commands so we cannot perform either -> returning protected command for failure
    setInvalidCommand();
    return cmd;
}

//if recognized command is an alias, decode its mapping to get the real command
string InterpretationEngine::decodeAliasCommand(string cmd) {
    //if cmd is an alias command, return real command, otherwise return cmd
    for (auto &aliasCmd : PIntEng_->cmdAlias) {
        if (cmd == aliasCmd.first) {
            //it's an alias command, return real command
            return aliasCmd.second;
        }
    }
    //not an alias command
    //since this is only called on recognized commands, it must be a primitive command name
    return cmd;
}

//multiply a command by given multiple
deque<std::string> InterpretationEngine::MultiplyCommand(std::deque<std::string> cmds, int mult) {
    deque<std::string> result;
    for (int i = 0; i < mult; i++) {
        //repeatedly append cmmds to result to get "repeating commands" effect
        result.insert(result.end(), cmds.begin(), cmds.end());
    }
    return result;
}

//UTILITY METHODS
bool InterpretationEngine::assertRecognizedCommand(string cmd) {
    //check that cmd is in list of recognized comands in InterpretaionEngine instant
    return (PIntEng_->commands_.find(cmd) != PIntEng_->commands_.end());
}

bool InterpretationEngine::assertValidCommand() {
    //check that the command was processed to be a recognized command
    return PIntEng_->isValidCmd;
}

void InterpretationEngine::assertValidCla(string cla) {
    //check that cla is in list of valid clas
    if(CLAS.find(cla) != CLAS.end()) {
        return;
    }
    cerr << "Illegal Command Line Argument: \"" + cla << "\"" << endl << "Exiting program" << endl;
    exit(1);
}

//MUTATORS
void InterpretationEngine::addAlias (string newCmd, string cmd) {
    PIntEng_->cmdAlias.insert(pair<string, string>(newCmd, cmd));
} 

void InterpretationEngine::removeAlias (string cmd) {
    //check if cmd is key to an alias, and remove it
    map<string, string>::iterator it;
    it = PIntEng_->cmdAlias.find(cmd);
    if (it != PIntEng_->cmdAlias.end()) {
        //delete alias
        PIntEng_->cmdAlias.erase(it);
    }
} 

void InterpretationEngine::addCommand (string newCmd) {
    //adds cmd to recognized commands
    PIntEng_->commands_.insert(newCmd);
} 

void InterpretationEngine::removeCommand (string cmd) {
    //reomves cmd from recognized commands
    PIntEng_->commands_.erase(cmd);
} 

void InterpretationEngine::setValidCommand() {
    PIntEng_->isValidCmd = true;
}
void InterpretationEngine::setInvalidCommand() {
    PIntEng_->isValidCmd = false;
}
