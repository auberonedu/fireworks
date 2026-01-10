/*
 * Copyright (c) 2020-2026 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include "bn_core.h"
#include "bn_math.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_blending.h"
#include "bn_bgs_mosaic.h"
#include "bn_random.h"
#include "bn_rect_window.h"
#include "bn_log.h"
#include "bn_bg_palettes.h"
#include "bn_bitmap_bg_attributes.h"
#include "bn_sprite_text_generator.h"
#include "bn_affine_bg_mat_attributes.h"
#include "bn_palette_bitmap_bg_actions.h"
#include "bn_palette_bitmap_bg_builder.h"
#include "bn_palette_bitmap_bg_painter.h"
#include "bn_palette_bitmap_bg_attributes_hbe_ptr.h"
#include "bn_palette_bitmap_bg_mat_attributes_hbe_ptr.h"
#include "bn_palette_bitmap_bg_pivot_position_hbe_ptr.h"

#include "bn_palette_bitmap_items_fw.h"

namespace
{
    class Dot {
        public:
            Dot(bn::fixed x, bn::fixed y, bn::fixed dx, bn::fixed dy, int ttc, int color_idx, bn::fixed gravity) : 
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

            void update() {
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
                            BN_LOG("DOT FIN");
                            _finished = true;
                            _clearing = false;
                        }
                    }
                }
            }

            bool clearing() {
                return _clearing;
            }

            bool finished() {   
                return _finished;
            }

            int x() { return _x.round_integer(); }
            int y() { return _y.round_integer(); }
            int color_idx() { return _color_idx; }
            

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

    const int dotcount = 20;

    class Firework {
        public:
            Firework(bn::fixed x, bn::fixed y, int color_idx, bn::palette_bitmap_bg_painter *painter, bn::random &rng) :
            _painter(painter) {
                _dots = {};
                for(int i = 0; i < dotcount; i++) {
                    _dots.push_back(Dot(x, y, rng.get_fixed(-1, 1), rng.get_fixed(-2, .5), 30, color_idx, .09));
                }
            }

            void update() {
                for(Dot &dot : _dots) {
                    if(!dot.finished()) dot.update();
                }
            }

            void draw() {
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

            void draw_and_update() {
                update();
                draw();
            }

            bool finished() {
                bool finished = true;
                BN_LOG();
                for(Dot& dot : _dots) {
                    finished &= dot.finished();
                }
                return finished;
            }

        private:
            bn::vector<Dot, dotcount> _dots;
            bn::palette_bitmap_bg_painter *_painter;
    };

    void palette_bitmap_bgs_scene()
    {

        //common::info info("Palette bitmap BGs", info_text_lines, text_generator);

        bn::palette_bitmap_bg_ptr bg = bn::palette_bitmap_bg_ptr::create(
                bn::palette_bitmap_items::fw.palette_item());

        bn::palette_bitmap_bg_painter painter(bg);
        painter.clear();
        painter.flip_page_later();

        auto rng = bn::random();

        const int fireworkcount = 10;

        bn::vector<Firework, fireworkcount> fireworks  = {};

        while(true)
        {
            if(bn::keypad::a_pressed() && fireworks.size() < fireworkcount) {
                auto x = rng.get_fixed(0, 240);
                auto y = rng.get_fixed(0, 160);
                fireworks.push_back(Firework(x, y, rng.get_int(1, 8), &painter, rng));
            }

            for(auto it = fireworks.begin(); it != fireworks.end();) {
                if(it->finished()) {
                    it = fireworks.erase(it);
                } else {
                    it->draw_and_update();
                    it++;
                }
            }
            bn::core::update();
        }
    }

}

int main()
{
    bn::core::init();

    //bn::sprite_text_generator text_generator(common::variable_8x16_sprite_font);
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));

    while(true)
    {
        palette_bitmap_bgs_scene();
        bn::core::update();
    }
}
