#ifndef KEY_H_INCLUDE
#define KEY_H_INCLUDE

struct Key
{
    bool isDown = false;
    void onPress()
    {
        isDown = true;
    }
    void onRelease()
    {
        isDown = false;
    }
};

#endif