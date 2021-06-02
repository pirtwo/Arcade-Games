#ifndef KEYBOARD_H_INCLUDE
#define KEYBOARD_H_INCLUDE

struct Keyboard
{
    bool isDown = false;
    bool isUp = true;
    void press()
    {
        isDown = true;
        isUp = false;
    }
    void release()
    {
        isDown = false;
        isUp = true;
    }
}

#endif