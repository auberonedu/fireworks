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
    const int MAX_FIREWORKS = 10;
    const int MAX_FW_X = bn::display::width();
    const int MAX_FW_Y = bn::display::height();

    // TODO: Is there a way to get this dynamically from the palette item?
    const int COLOR_COUNT = 8;

    void fireworks_scene()
    {
        bn::palette_bitmap_bg_ptr bg = bn::palette_bitmap_bg_ptr::create(
                bn::palette_bitmap_items::fw.palette_item());

        bn::palette_bitmap_bg_painter painter(bg);
        painter.clear();

        auto rng = bn::random();

        bn::vector<Firework, MAX_FIREWORKS> fireworks  = {};

        while(true)
        {
            // If aren't at capactiy for fireworks, create a new one when A is pressed
            if(bn::keypad::a_pressed() && fireworks.size() < MAX_FIREWORKS) {
                // Firework has random color from full palette, and random location from full screen
                auto x = rng.get_fixed(0, MAX_FW_X);
                auto y = rng.get_fixed(0, MAX_FW_Y);
                fireworks.push_back(Firework(x, y, rng.get_int(1, COLOR_COUNT), &painter, rng));
            }

            for(auto it = fireworks.begin(); it != fireworks.end();) {
                // Remove finished fireworks from vector so new ones can be made
                if(it->finished()) {
                    it = fireworks.erase(it);
                } else {
                    // Update unfinished ones
                    it->draw_and_update();
                    it++;
                }
            }

            rng.update();
            bn::core::update();
        }
    }

}

int main()
{
    bn::core::init();
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));
    fireworks_scene();
}
