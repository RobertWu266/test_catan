//
// Created by robert_wu on 5/18/23.
//
#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#define i32 int32_t
#define TOL 100

#define bprop(tgt) ((body_property*)tgt->prop)
#define vprop(tgt) ((vertice_property*)tgt->prop)
#define sprop(tgt) ((side_property*)tgt->prop)
#define obj_custom_x(tgt) (tgt->locs[0]- tgt->locs[1])
#define obj_custom_y(tgt) (-tgt->locs[0]- tgt->locs[1]+2*tgt->locs[2])
typedef enum _resources
{
    HILL, MOUNTAIN, FIELD, PASTURE, FOREST, DESERT
} resources;
//Hill will be red, mountain will be grey, field will be yellow, pasture will be green, forest will be deep green, desert will be deep yellow.
typedef enum _attribute{body=0,v_side,main_side,minor_side,pos_vert,neg_vert,invalid}attribute;
typedef enum _color{NONE, WHITE, RED, ORANGE, YELLOW, BLUE}color;
typedef enum _property{BODY_PROPERTY,VERTICE_PROPERTY,SIDE_PROPERTY}property;
typedef enum _owner{None,player1,player2,player3,player4}owner;
//player 1 is red follow as blue green yellow.
typedef enum _building{empty,village,city}building;
typedef enum _harbor{Nil, ALL_HARBOR,WHEAT_HARBOR,SHEEP_HARBOR,WOOD_HARBOR,BRICK_HARBOR,STONE_HARBOR}harbor;
typedef enum _identity{human,road_AI,develop_AI,village_AI}identity;


void shuffle_identity_list(i32 idx);
/*
 * body: Represents the center of the hexagon
 * v_side: The side of the hexagon that is vertical
 * main_side: The side of the hexagon shaped like '\'
 * minor_side: The side of the hexagon shaped like '/'
 * pos_vert: The vertices where the sum of the cube coordinates x+y+z equals 1
 * neg_vert: The vertices where the sum of the cube coordinates x+y+z equals -1
 */
struct _obj;
typedef struct
{
    uint8_t wood;
    uint8_t stone;
    uint8_t brick;
    uint8_t sheep;
    uint8_t wheat;
    uint8_t special_cards;
    uint8_t knights;
    uint8_t year_of_plenty;
    uint8_t road_building;
    uint8_t monopoly;
    uint8_t victory_card;
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
    uint8_t knights_use;
    //bool knights_get;
    uint8_t year_of_plenty;
    //bool year_of_plenty_get;
    uint8_t road_building;
    //bool road_building_get;
    uint8_t monopoly;
    //bool monopoly_get;
    uint8_t victory_card;
    //bool victory_card_get;
    uint8_t total_victory_points;
    uint8_t max_roads;
    uint8_t village_remain;
    uint8_t city_remain;
    uint8_t road_remain;
    uint8_t wood_exchange_rate;
    uint8_t stone_exchange_rate;
    uint8_t brick_exchange_rate;
    uint8_t sheep_exchange_rate;
    uint8_t wheat_exchange_rate;
    struct _obj* my_road[15];
    i32 my_road_csr;
    identity iden;

}player_property;

typedef struct
{
    uint8_t knights;
    uint8_t year_of_plenty;
    uint8_t road_building;
    uint8_t monopoly;
}card_temp;

typedef struct _obj
{
    attribute attr;
    int32_t *locs;
    void *prop;
    bool highlighted;
}obj;

typedef struct _body_property
{
    property flag;
    resources resource;
    uint8_t num;
    obj **nei_vert;
    bool has_robber;

}body_property;

typedef struct _side_property
{
    property flag;
    owner own;
    obj **nei_vert;
    obj **nei_side;
}side_property;

typedef struct _vertice_property
{
    property flag;
    owner own;
    building build;
    obj **nei_vert;
    obj **nei_body;
    harbor harb;

}vertice_property;
/*
 *要加什麼自己加
 */
/*
 * Rules:
 * innerbox[x+6][y+6][x+y+z+1] is the object at (x,y,z) (cube coordinates)
 */
void box_set();
i32 abs(i32 x);
i32 customfindsum(i32 x,i32 y);
i32 *custom2cube(i32 x,i32 y);//return a calloc'd i32* like this {x,y,z} (cube coordinate)
bool isvalid(i32 *locs);
/*
 * check if the locs is out of bound, but it doesn't check if the find(location) is NULL
 */
attribute get_attr(i32 *locs);
obj* find_obj(i32 x,i32 y,i32 z);//if the locs is invalid, NULL is returned.
obj* locs_find_obj(i32 *locs);
obj *init_obj(i32 *locs);
void set_neighbor(obj *tgt);
void dtor_obj(obj* tgt);
void print_obj(obj* tgt);// if tgt==NULL, nothing will happen.
obj** side_neighbor_body(obj* tgt);
/*return 2 obj* of the bodies adjacent to given side obj tgt
 * if tgt->attr is not a side, NULL is returned
 * if there's only 1 adjacent body, the returned value might be
 * {NULL,(body)} or {(body),NULL}*/

void bank_init(bank_property *bank);
void player_init(player_property *player);
void card_temp_init( card_temp *cardtemp );

obj** side_neighbor_vertice(obj* tgt);
obj** vertice_neighbor_side(obj* tgt);
obj **side_neighbor_side(obj *tgt);
obj **body_neighbor_vertice(obj* tgt);
obj** vertice_neighbor_body(obj* tgt);
obj** vertice_neighbor_vertice(obj* tgt);
harbor locs_harbor(i32 *locs);
void build_village(owner owner1, obj* tobuild);
void build_road(owner owner1, obj* tobuild);
obj** highlight_availible_village(owner);
obj** highlight_availible_village_beginning();
obj** highlight_available_road(owner owner1);
obj** highlight_available_upgrade(owner owner1);
void clear_all_highlight();

void trade_init( int trade_withbank[] );
void tradewithbank( player_property *player, bank_property *bank, int trade_withbank[] );
void robber( obj *robber );
void specialcard_init( int specialcard[] );
void specialcard_get( card_temp *cardtemp, player_property *player, bank_property *bank, int count );
//void player_card_get_init( player_property *player );
void specialcard_use( player_property *player1, player_property *player2, player_property *player3, player_property *player4, bank_property *bank, int specialcard[], int trade[], owner owner, obj* tobuild );
uint8_t get_longest_road(owner owner1);
uint8_t DFS(owner owner1,obj *the_road,obj *visited_road[15], uint8_t *p_visited_road_csr);
void discard_half_deck_action(player_property *the_player,uint8_t brick_discard,uint8_t sheep_discard,uint8_t stone_discard,uint8_t wheat_discard,uint8_t wood_discard);
