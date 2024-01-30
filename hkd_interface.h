#ifndef _HKD_INTERFACE_H_
#define _HKD_INTERFACE_H_

typedef bool (*QUIT_GAME_PFN)(void);

struct hkdInterface {
    QUIT_GAME_PFN QuitGame;
};


#endif
