#ifndef FIREWORK_H
#define FIREWORK_H

#include "bn_vector.h"
#include "bn_random.h"
#include "bn_palette_bitmap_bg_painter.h"
#include "Dot.h"



class Firework {
    public:
        Firework(bn::fixed x, bn::fixed y, int color_idx, bn::palette_bitmap_bg_painter *painter, bn::random &rng);

        void update();

        void draw();

        void draw_and_update();

        bool finished();

    private:
        static inline constexpr int DOTS_PER_FIREWORK = 20;
        static inline constexpr int DOT_TTC = 30;
        static inline constexpr bn::fixed GRAVITY = bn::fixed(9) / 100;
        bn::vector<Dot, DOTS_PER_FIREWORK> _dots;
        bn::palette_bitmap_bg_painter *_painter;
};

#endif