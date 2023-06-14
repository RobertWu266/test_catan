#include "trade_specialcard.h"

void trade( player_property *player, bank_property *bank )
{
	if()
}

void robber()
{
	
}

void specialcard_get( player_property *player, bank_property *bank )
{
	if( player -> stone < 1 || player -> sheep < 1 || player -> wheat < 1 )
		return;
	player -> stone--;
	player -> sheep--;
	player -> wheat--;
	int card = rand() % 5 + 1;
	while( )
	{
		card = rand() % 5 + 1;
	}
	switch( card )
	{
		case 1: 
			player -> knights++;
			bank -> knights--;
			break;
		case 2: 
			player -> year_of_plenty++;
			bank -> year_of_plenty--;
			break;
		case 3: 
			player -> road_building++;
			bank -> road_building--;
			break;
		case 4: 
			player -> monopoly++;
			bank -> monopoly--;
			break;
		case 5: 
			player -> victory_card++;
			bank -> victory_card--;
			break;
		default:
			break:
	}
}








