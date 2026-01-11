#include "Dot.h"

Dot::Dot(bn::fixed x, bn::fixed y, bn::fixed dx, bn::fixed dy, int ttc, int color_idx, bn::fixed gravity) : 
    _init_x(x),
    _init_y(y),
    _init_dx(dx),
    _init_dy(dy),
    _x(x),
    _y(y),
    _dx(dx),
    _dy(dy),
    _color_idx(color_idx),
    _gravity(gravity),
    _state_duration(ttc),
    _init_ttc(ttc),
    _ttc(ttc){};

void Dot::update() {
    if(!_finished) {
        // TODO: air resistance
        _dy += _gravity;
        _x +=_dx;
        _y += _dy;
        _ttc--;
        if(_ttc == 0) {
            if (!_clearing) {
                _x = _init_x;
                _y = _init_y;
                _dx = _init_dx;
                _dy = _init_dy;
                _ttc = _init_ttc;
                _clearing = true;
            }  else {
                _finished = true;
                _clearing = false;
            }
        }
    }
}

bool Dot::clearing() {
    return _clearing;
}

bool Dot::finished() {   
    return _finished;
}

int Dot::x() { return _x.round_integer(); }
int Dot::y() { return _y.round_integer(); }
int Dot::color_idx() { return _color_idx; }