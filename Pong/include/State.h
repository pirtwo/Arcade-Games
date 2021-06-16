#ifndef STATE_H_INCLUDE
#define STATE_H_INCLUDE

struct State
{
    int playerOneScore = 0;
    int playerTwoScore = 0;
    bool hasKickoff = false;
    bool ballTouched = false;
};

#endif