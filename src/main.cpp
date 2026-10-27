#include <bn_core.h>
#include <bn_backdrop.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_sprite_ptr.h>
#include <bn_display.h>
#include <bn_random.h>
#include <bn_vector.h>
#include <bn_log.h>

#include "bn_sprite_items_dot.h"

static bn::random random;

// Set max/min x position to be the edges of the display
static constexpr int HALF_SCREEN_WIDTH = bn::display::width() / 2;
static constexpr bn::fixed MIN_X = -HALF_SCREEN_WIDTH;
static constexpr bn::fixed MAX_X = HALF_SCREEN_WIDTH;

static constexpr int HALF_SCREEN_HEIGHT = bn::display::height() / 2;
static constexpr bn::fixed MIN_Y = -HALF_SCREEN_HEIGHT;
static constexpr bn::fixed MAX_Y = HALF_SCREEN_HEIGHT;

// Bouncer speed limits
static constexpr int MIN_SPEED = 1;
static constexpr int MAX_SPEED = 5;

// Maximum number of bouncers on screen at once
static constexpr int MAX_BOUNCERS = 20;

bn::fixed y_spawn = 0;

class Bouncer {
    public:
        bn::sprite_ptr sprite;
        bn::fixed x_speed;
        bn::fixed y_speed;

        Bouncer(bn::fixed x_start, bn::fixed y_start) : sprite(bn::sprite_items::dot.create_sprite(x_start, y_start)), x_speed(random.get_int(MIN_SPEED, MAX_SPEED)), y_speed(random.get_int(MIN_SPEED, MAX_SPEED)) {

        }

        void update() {
            bn::fixed x = sprite.x();
            bn::fixed y = sprite.y();

            // Update x position by adding speed
            x += x_speed;
            y += y_speed;

            // If we've gone off the screen
            if(x > MAX_X) {
                // Snap back to screen and reverse direction
                x = MAX_X;
                x_speed *= -1;
            }
            if (x < MIN_X) {
                x = MIN_X;
                x_speed *= -1;
            }
            if (y > MAX_Y) {
                y = MAX_Y;
                y_speed *= -1;
            }
            if (y < MIN_Y) {
                y = MIN_Y;
                y_speed *= -1;
            }

            sprite.set_x(x);
            sprite.set_y(y);
        }
    private:
};

// Return the average of all Bouncers' coordinates as a point
bn::fixed_point get_average_position(bn::vector<Bouncer, MAX_BOUNCERS>& bouncers){
     // Add all x positions together
    bn::fixed x_sum = 0;
    bn::fixed y_sum = 0;
    for(Bouncer bouncer : bouncers) {
        x_sum += bouncer.sprite.x();
        y_sum += bouncer.sprite.y();
    }

    bn::fixed x_average = x_sum;
    bn::fixed y_average = y_sum;

    // Only divide if we have 1 or more
    // Prevents division by 0
    int size = bouncers.size();
    if(size > 0) {
        x_average /= size;
        y_average /= size;
    }

    return bn::fixed_point(x_average, y_average);
}

void add_bouncer(bn::vector<Bouncer, MAX_BOUNCERS>& bouncers, bn::fixed start_x, bn::fixed start_y){
    // Only add if we're below the maximum
    if(bouncers.size() < bouncers.max_size()) {
        bouncers.push_back(Bouncer(start_x, start_y));
    }
}

int main() {
    bn::core::init();

    // All bouncers that currently exist
    bn::vector<Bouncer, MAX_BOUNCERS> bouncers = {};
    // Current average bouncer coordinates
    bn::fixed_point current_average = bn::fixed_point(0, 0);
    // Dot sprite indicating average bouncer position
    bn::sprite_ptr average_indicator = bn::sprite_items::dot.create_sprite(current_average);

    while(true) {
        current_average = get_average_position(bouncers);
        average_indicator.set_position(current_average);

        // if A is pressed add a new bouncer
        if(bn::keypad::a_pressed()) {
            add_bouncer(bouncers, 20, y_spawn);
        }

        // if B is pressed print the average to the console
        if(bn::keypad::b_pressed()) {
            BN_LOG("Average x: ", current_average.x());
        }

        for (Bouncer& b : bouncers) {
            b.update();
        }

        y_spawn += 1;
        if (y_spawn > MAX_Y) {
            y_spawn = MIN_Y;
        }

        bn::core::update();
    }
}