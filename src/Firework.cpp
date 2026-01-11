#include "Firework.h"

Firework::Firework(bn::fixed x, bn::fixed y, int color_idx, bn::palette_bitmap_bg_painter *painter, bn::random &rng) :
_painter(painter) {
    _dots = {};
    for(int i = 0; i < DOTS_PER_FIREWORK; i++) {
        _dots.push_back(Dot(x, y, 
                            rng.get_fixed(-1, 1), rng.get_fixed(-2, .5), // random initial vector
                            DOT_TTC,
                            color_idx,
                            GRAVITY));
    }
}

void Firework::update() {
    for(Dot &dot : _dots) {
        if(!dot.finished()) dot.update();
    }
}

void Firework::draw() {
    for(Dot &dot : _dots) {
        if(!dot.finished()) {
            int pal_color = dot.clearing() ? 0 : dot.color_idx();
            _painter->plot(dot.x(), dot.y(), pal_color);
            _painter->flip_page_now();
            _painter->plot(dot.x(), dot.y(), pal_color);
            dot.update();
        }
    }
} 

void Firework::draw_and_update() {
    update();
    draw();
}

bool Firework::finished() {
    bool finished = true;
    for(Dot& dot : _dots) {
        finished &= dot.finished();
    }
    return finished;
}