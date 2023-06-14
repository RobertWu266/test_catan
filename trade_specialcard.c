#include "trade_specialcard.h"

void trade( player_property *player, bank_property *bank )
{
	if()
}

void robber()
{
	
}

void specialcard( player_property *player1, player_property *player2, player_property *player3, player_property *player4, bank_property *bank ) //player1 為當回合玩家
{
	if( player1 -> stone < 1 || player1 -> sheep < 1 || player1 -> wheat < 1 )
		return;
	player1 -> stone--;
	player1 -> sheep--;
	player1 -> wheat--;
	int card = rand() % 5 + 1;
	switch( card )
	{
		case 1: //騎士
			player1 -> knights++;
			//robber
			break;
		case 2: //豐收之年
			
			break;
		case 3: //道路建造
			
			break;
		case 4: //壟斷市場
			
			break;
		case 5: //分數卡
			player1 -> totoal_victory_points++;
			break;
		default:
			break:
	}
}








