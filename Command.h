#ifndef _COMMAND_
#define _COMMAND_

#include <map>
#include <set>

//Original Command names
const std::set<std::string> COMMANDS {
    "left", 
    "right", 
    "down", 
    "clockwise",
    "counterclockwise",
    "drop",
    "levelup",
    "leveldown",
    "norandom",
    "random",
    "sequence",
    "I", 
    "J", 
    "L", 
    "S", 
    "Z", 
    "O", 
    "T",
    "restart",
    "hint",
    "rename",
};
//Original Command Line Arguments (CLA) names
const std::set<std::string> CLAS {
    "-text",
    "-seed",
    "-scriptfile",
    "-startlevel",
    "-nobonus"
};

const std::map<std::string, int> COMMAND_ARGS = {
    {"left", 0},
    {"right", 0},
    {"down", 0},
    {"clockwise", 0},
    {"counterclockwise", 0},
    {"drop", 0},
    {"levelup", 0},
    {"leveldown", 0},
    {"norandom", 1},
    {"random", 0},
    {"sequence", 1},
    {"I", 0},
    {"J", 0},
    {"L", 0},
    {"S", 0},
    {"Z", 0},
    {"O", 0},
    {"T", 0},
    {"restart", 0},
    {"hint", 0},
    {"rename", 2}
};

const std::map<std::string, int> CLA_ARGS {
    {"-text", 0},
    {"-nobonus", 0},
    {"-seed", 1},
    {"-scriptfile", 1},
    {"-startlevel", 1},
};

enum Direction { right, left, down };
enum Rotation { cw, ccw };
enum LevelAdj { lvUp, lvDown };


#endif