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

#include "Dot.h"
#include "Firework.h"

namespace
{
    void palette_bitmap_bgs_scene()
    {
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

    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));

    while(true)
    {
        palette_bitmap_bgs_scene();
        bn::core::update();
    }
}
