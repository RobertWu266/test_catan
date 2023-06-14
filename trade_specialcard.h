#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
    uint8_t year_of_plenty;
    uint8_t road_building;
    uint8_t monopoly;
    uint8_t victory_card;
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
}player_property;

void trade( player_property *player, bank_property *bank );
void robber( player_property *player, );
void specialcard_get( player_property *player, bank_property *bank );

