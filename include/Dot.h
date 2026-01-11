#ifndef DOT_H
#define DOT_H

#include "bn_fixed.h"

class Dot {
    public:
        Dot(bn::fixed x, bn::fixed y, bn::fixed dx, bn::fixed dy, int ttc, int color_idx, bn::fixed gravity);

        void update();

        bool clearing();

        bool finished();

        int x();
        int y();
        int color_idx();
        

    private:
        bn::fixed _init_x;
        bn::fixed _init_y;
        bn::fixed _init_dx;
        bn::fixed _init_dy;
        bn::fixed _x;
        bn::fixed _y;
        bn::fixed _dx;
        bn::fixed _dy;
        int _color_idx;
        bn::fixed _gravity;
        int _state_duration;
        int _init_ttc;
        int _ttc; // frames until state change
        bool _clearing = false;
        bool _finished = false;
};

#endif