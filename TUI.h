//
// Created by wu on 5/22/23.
//

#ifndef ROCKET_TUI_H
#define ROCKET_TUI_H

#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//#include "mymmap.h"
#include "catan_conversions.h"
#define i32 int32_t

obj *get_obj_from_mouse(i32 x,i32 y);
void show_all_objects();
#endif //ROCKET_TUI_H
