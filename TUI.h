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
obj* wait_until_get_obj_from_mouse();
obj* get_highlighted();
resources get_highlighted_resource(MEVENT event);
void shuffle_i32(i32 *array, i32 n);
void show_obj(obj* tgt);
void print_polygon(obj *tgt, int y, int x);
#define body_printw(tgt) printw("BODY:resource:%s  num:%d,robber=%d,locs:(%d,%d,%d)",resources_name[bprop(tgt)->resource],bprop(tgt)->num,bprop(tgt)->has_robber,tgt->locs[0],tgt->locs[1],tgt->locs[2])
#define vertice_printw(tgt) printw("VERTICE:owner:%d,building=%d,harbor:%dlocs:(%d,%d,%d)",vprop(tgt)->own,vprop(tgt)->build,vprop(tgt)->harb,tgt->locs[0],tgt->locs[1],tgt->locs[2])
#define side_printw(tgt) printw("SIDE:owner:%d,locs:(%d,%d,%d)",sprop(tgt)->own,tgt->locs[0],tgt->locs[1],tgt->locs[2])
void show_all_objects();

#endif //ROCKET_TUI_H
