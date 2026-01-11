#ifndef FIREWORK_H
#define FIREWORK_H

#include "bn_vector.h"
#include "bn_random.h"
#include "bn_palette_bitmap_bg_painter.h"
#include "Dot.h"

const int dotcount = 20;

class Firework {
    public:
        Firework(bn::fixed x, bn::fixed y, int color_idx, bn::palette_bitmap_bg_painter *painter, bn::random &rng);

        void update();

        void draw();

        void draw_and_update();

        bool finished();

    private:
        bn::vector<Dot, dotcount> _dots;
        bn::palette_bitmap_bg_painter *_painter;
};

#endif