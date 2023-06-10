#ifndef CATAN_UI_H
#define CATAN_UI_H

#define ROW 51
#define COL 169

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "catan_conversions.h"
#include "TUI.h"

typedef struct
{
    uint8_t wood;
    uint8_t stone;
    uint8_t brick;
    uint8_t sheep;
    uint8_t wheat;
    uint8_t special_cards;

}bank_property;

typedef struct
{
    uint8_t total_resource_cards;
    uint8_t wood;
    uint8_t stone;
    uint8_t brick;
    uint8_t sheep;
    uint8_t wheat;
    uint8_t special_cards;
    uint8_t knights;
    bool year_of_plenty;
    bool road_building;
    bool monopoly;
    bool victory_card;
    uint8_t totoal_victory_points;
    uint8_t max_roads;
    uint8_t village_remain;
    uint8_t city_remain;
    uint8_t road_remain;

}player_property;


bool agree_or_disagree(int y, int x, char *deal , MEVENT event);
int start_screen();
int roll_and_print_dice();
void clear_right_cornor();
void set_background_color_init();
void in_game_ui();
void check_ui_size();
void start_ui();
void print_pass();
void draw_with_mouse_and_return_value();
void print_in_game_ui();
void button_play_and_quit();
void print_c(int y, int x);
void print_a(int y, int x);
void print_t(int y, int x);
void print_n(int y, int x);
void bank_init(bank_property *bank);
void player_init(player_property *player);
void print_bank(bank_property *bank);
void _print_player(player_property *player ,int y, int x , int color);
void print_players_status(player_property *player_1, player_property *player_2, player_property *player_3, player_property *player_4);
void print_YOU(player_property *player_1);
//player_1 is red at top left cornor, player_2 blue is at top right cornor
//player_3 is green at down left cornor, player_4 yellow is at down right cornor 

/* background colors:
    init_pair(1,COLOR_WHITE,COLOR_RED);
    init_pair(2,COLOR_BLACK,COLOR_CYAN);
    init_pair(3,COLOR_WHITE,COLOR_GREEN);
    init_pair(4,COLOR_WHITE,COLOR_YELLOW);
    init_pair(5,COLOR_WHITE,COLOR_BLUE);//this will present as light green
    init_pair(6,COLOR_WHITE,COLOR_MAGENTA);//this is purple
    init_pair(7,COLOR_WHITE,COLOR_BLACK);
    init_pair(8,COLOR_BLACK,COLOR_WHITE);
    //those are for roads and vertice
    init_pair(21,COLOR_RED,COLOR_WHITE);
    init_pair(22,COLOR_CYAN,COLOR_WHITE);
    init_pair(23,COLOR_GREEN,COLOR_WHITE);
    init_pair(24,COLOR_YELLOW,COLOR_WHITE);
*/



#endif