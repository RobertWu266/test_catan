#include "catan_ui.h"

extern const char* resources_name[];
extern i32 dice_nums[];
extern obj* body_list[];
extern obj* vertice_list[];
extern bank_property bank;
extern player_property players[];

void in_game_ui(MEVENT event)
{
	int trade_withbank[10] = {0};
	set_background_color_init();
	roll_and_print_dice(43,104);
	roll_and_print_dice(43,116);
	bank_property bank;
	player_property player_1;
	player_property player_2;
	player_property player_3;
	player_property player_4;
	card_temp cardtemp;
	player_init(&player_1);
	player_init(&player_2);
	player_init(&player_3);
	player_init(&player_4);
	bank_init(&bank);
	card_temp_init( &cardtemp );
	trade_init( trade_withbank );
	bool your_turn = TRUE;
	bool trade = TRUE;
	bool build_a_road = TRUE;//for test
	print_in_game_ui();
	print_bank(&bank);
	print_players_status(&player_1, &player_2, &player_3, &player_4);
	print_YOU(&player_1, &cardtemp);
	if(your_turn)
	{
		int ch;
	    while (ch = getch())
	    {
	        if (ch == KEY_MOUSE && getmouse(&event) == OK)
	        {
	            if (event.bstate & BUTTON1_PRESSED)
	            {
	                int x = event.x;
	                int y = event.y;
	                if(((x >= 105 && y >= 43) && (x <= 113 && y <= 47)) ||((x >= 117 && y >= 43) && (x <= 125 && y <= 47)))
	                {
	                	int val_by_dice = 0;
	                	val_by_dice += roll_and_print_dice(43,104);
	                	val_by_dice += roll_and_print_dice(43,116);
	                	mvprintw(50,168,"");
	                	break;
	                }
	            }
	        }
		}
		print_pass();
		while(ch = getch())
	    {
	        if (ch == KEY_MOUSE && getmouse(&event) == OK)
	        {
	            if (event.bstate & BUTTON1_PRESSED)
	            {
		        	print_pass();
	                int x = event.x;
	                int y = event.y;
	                if(((x >= 133 && y >= 41) && (x <= 167 && y <= 49)))
	                {
	                	clear_right_cornor();
		                break;
	                }
	                if(((x >= 155 && y >= 1) && (x <= 167 && y <= 9)))//trade zone
	                {
	                	print_trade_ui(&player_1, &player_1, &player_1, &player_1,  &bank, event, trade_withbank, &cardtemp);
	                }
	            }
			}
		}
	}
}

void fprintf_player(player_property player1) {
    FILE *fptr;
    fptr = fopen("catan_log.txt", "a");

    if(fptr == NULL)
    {
        fptr = fopen("catan_log.txt", "w");
    }

    fprintf(fptr, "\n\n\nTotal Resource Cards: %d\n", player1.total_resource_cards);
    fprintf(fptr, "Wood: %d\n", player1.wood);
    fprintf(fptr, "Stone: %d\n", player1.stone);
    fprintf(fptr, "Brick: %d\n", player1.brick);
    fprintf(fptr, "Sheep: %d\n", player1.sheep);
    fprintf(fptr, "Wheat: %d\n", player1.wheat);
    fprintf(fptr, "Special Cards: %d\n", player1.special_cards);
    fprintf(fptr, "Knights: %d\n", player1.knights);
    fprintf(fptr, "Year of Plenty: %d\n", player1.year_of_plenty);
    fprintf(fptr, "Road Building: %d\n", player1.road_building);
    fprintf(fptr, "Monopoly: %d\n", player1.monopoly);
    fprintf(fptr, "Victory Card: %d\n", player1.victory_card);
    fprintf(fptr, "Total Victory Points: %d\n", player1.total_victory_points);
    fprintf(fptr, "Max Roads: %d\n", player1.max_roads);
    fprintf(fptr, "Village Remain: %d\n", player1.village_remain);
    fprintf(fptr, "City Remain: %d\n", player1.city_remain);
    fprintf(fptr, "Road Remain: %d\n", player1.road_remain);
    fprintf(fptr, "Wood Exchange Rate: %d\n", player1.wood_exchange_rate);
    fprintf(fptr, "Stone Exchange Rate: %d\n", player1.stone_exchange_rate);
    fprintf(fptr, "Brick Exchange Rate: %d\n", player1.brick_exchange_rate);
    fprintf(fptr, "Sheep Exchange Rate: %d\n", player1.sheep_exchange_rate);
    fprintf(fptr, "Wheat Exchange Rate: %d\n", player1.wheat_exchange_rate);

    fclose(fptr);
}
void fprintf_bank(bank_property bank1) {
    FILE *file = fopen("catan_log.txt", "a");
    if (file == NULL) {
        file = fopen("catan_log.txt", "w");
    }

    fprintf(file, "\n\n\nBank Properties:\n");
    fprintf(file, "Wood: %u\n", bank1.wood);
    fprintf(file, "Stone: %u\n", bank1.stone);
    fprintf(file, "Brick: %u\n", bank1.brick);
    fprintf(file, "Sheep: %u\n", bank1.sheep);
    fprintf(file, "Wheat: %u\n", bank1.wheat);
    fprintf(file, "Knight: %u\n", bank1.knights);
    fprintf(file, "Year of plenty: %u\n", bank1.year_of_plenty);
    fprintf(file, "Road building: %u\n", bank1.road_building);
    fprintf(file, "Monopoly: %u\n", bank1.monopoly);
    fprintf(file, "Victory cards: %u\n", bank1.victory_card);

    fclose(file);
}
void clear_log()
{
    FILE *file = fopen("catan_log.txt", "w");
    fclose(file);
}
void draw_with_mouse_and_return_value(MEVENT event,int trade_withbank[])// for debug
{
	set_background_color_init();
    int ch;
    while ((ch = getch()) != 'q')
    {
    	card_temp cardtemp;
        if(ch=='1')fprintf_player(players[0]);
        if(ch=='2')fprintf_player(players[1]);
        if(ch=='3')fprintf_player(players[2]);
        if(ch=='4')fprintf_player(players[3]);

        if(ch=='b')fprintf_bank(bank);

        if(ch == 'j')
        {
            highlight_availible_village_beginning();
            refresh_all_status(players,players+1,players+2,players+3,&bank,&cardtemp,trade_withbank);
        }
        if(ch=='k')
        {
            highlight_availible_village(player1);
            refresh_all_status(players,players+1,players+2,players+3,&bank,&cardtemp,trade_withbank);
        }
        if(ch=='l')
        {
            clear_all_highlight();
            refresh_all_status(players,players+1,players+2,players+3,&bank,&cardtemp,trade_withbank);
        }
        if(ch=='n')
        {
            highlight_available_road(player1);
            refresh_all_status(players,players+1,players+2,players+3,&bank,&cardtemp,trade_withbank);
        }
        if(ch=='m')
        {
            highlight_available_upgrade(player1);
            refresh_all_status(players,players+1,players+2,players+3,&bank,&cardtemp,trade_withbank);
        }
        if(ch=='p')clear_log();
        if (ch == KEY_MOUSE && getmouse(&event) == OK)
        {
            if (event.bstate & BUTTON1_PRESSED)
            {
                int x = event.x;
                int y = event.y;
                attron(COLOR_PAIR(8));
                mvprintw(y,x," ");
                attroff(COLOR_PAIR(8));
                mvprintw(0, 0, "Mouse Clicked at: x=%d, y=%d", x, y);
                obj* clicked= get_obj_from_mouse(event.x,event.y);
                if(clicked)
                {

                    for(i32 i=0;i<7;i++)mvprintw(i,172,"                                                    ");
                    switch(clicked->attr)
                    {
                        case body:
                            mvprintw(0,172,"");
                            body_printw(clicked);
                            for(i32 i=0;i<6;i++)
                            {
                                mvprintw(1+i,172,"");
                                vertice_printw(bprop(clicked)->nei_vert[i]);
                            }
                            break;
                        case pos_vert:
                        case neg_vert:

                            mvprintw(0,172,"");
                            vertice_printw(clicked);
                            for(i32 i=0;i<3;i++)
                            {
                                mvprintw(1+i,172,"");
                                if(vprop(clicked)->nei_body[i])body_printw(vprop(clicked)->nei_body[i]);
                            }
                            for(i32 i=0;i<3;i++)
                            {
                                mvprintw(4+i,172,"");
                                if(vprop(clicked)->nei_vert[i])vertice_printw(vprop(clicked)->nei_vert[i]);
                            }
                            break;
                        case main_side:
                        case minor_side:
                        case v_side:
                            mvprintw(0,172,"");
                            side_printw(clicked);
                            for(i32 i=0;i<4;i++)
                            {
                                mvprintw(1+i,172,"");
                                if(sprop(clicked)->nei_side[i])side_printw(sprop(clicked)->nei_side[i]);
                            }
                            for(i32 i=0;i<2;i++)
                            {
                                mvprintw(5+i,172,"");
                                vertice_printw(sprop(clicked)->nei_vert[i]);
                            }
                            break;
                    }
                }

                refresh();
            }
            /*if (event.bstate & BUTTON2_PRESSED)
            {
                obj *clicked= get_obj_from_mouse(event.x,event.y);
                if(clicked->attr==neg_vert||clicked->attr==pos_vert)
                {
                    build_village(player1,clicked);
                    show_obj(clicked);
                }
                else if(clicked->attr!=body)
                {
                    build_road(player1,clicked);
                    show_obj(clicked);
                }
            }*/
			if (event.bstate & BUTTON3_PRESSED)
            {
                int x = event.x;
                int y = event.y;
                attron(COLOR_PAIR(7));
                mvprintw(y,x," ");
                attroff(COLOR_PAIR(7));
                mvprintw(0, 0, "Mouse Clicked at: x=%d, y=%d", x, y);
                obj *clicked= get_obj_from_mouse(event.x,event.y);
                if(clicked)
                {
                    if(clicked->attr==neg_vert||clicked->attr==pos_vert)
                    {
                        build_village(player1,clicked);
                        refresh_all_status(players,players+1,players+2,players+3,&bank,&cardtemp,trade_withbank);
                    }
                    else if(clicked->attr!=body)
                    {
                        build_road(player1,clicked);
                        refresh_all_status(players,players+1,players+2,players+3,&bank,&cardtemp,trade_withbank);
                    }
                    refresh();
                }
            }
        }
    }
}


void print_in_game_ui()
{
	set_background_color_init();
	//other than player's property
	for (int i = 0; i < 95; ++i)
	{
		for (int j = 0; j < 49; ++j)
		{
			attron(COLOR_PAIR(2));
			mvprintw(1 + j,1 + i, " ");
			attroff(COLOR_PAIR(2));
		}
	}
	show_all_objects();
	//print stat outline
	attron(COLOR_PAIR(8));
	for (int j = 0; j < 169; ++j)
	{
		if(j == 0 || j == 168)
		{
			for (int i = 0; i < 51; ++i)
			{
				mvprintw(i,j," ");
			}
		}
		else
		{
			mvprintw(0, j, " ");
			mvprintw(50, j, " ");
		}

	}
	for (int i = 0; i < 51; ++i)
	{
		mvprintw(0 + i, 96, " ");
	}
	//print outline of dice and check area
	for (int i = 0; i < 71; ++i)
	{
		mvprintw(40, 97 + i, " ");
		if(i == 35)
		{
			for (int j = 0; j < 10; ++j)
			{
				mvprintw(40 + j, 97 + i, " ");
			}
		}
	}
	for (int i = 0; i < 71; ++i)
	{
		mvprintw(10, 97 + i, " ");
	}
	for (int i = 0; i < 10; ++i)
	{
		mvprintw(1 + i, 107, " ");
		mvprintw(1 + i, 154, " ");
	}
	//print bank's B;
	for (int j = 0; j < 7; ++j)
	{
		mvprintw(2 + j, 98 ," ");
	}
	for (int i = 0; i < 7; ++i)
	{
		if(i == 6)
		{
			mvprintw(3, 105 ," ");
			mvprintw(4, 105 ," ");
			mvprintw(6, 105 ," ");
			mvprintw(7, 105 ," ");
		}
		else
		{
			mvprintw(2, 99 + i ," ");
			mvprintw(5, 99 + i ," ");
			mvprintw(8, 99 + i ," ");
		}
	}
	for (int k = 0; k < 2; ++k)
	{
		int tmp = k * 8;
		for (int i = 0; i < 8; ++i)
		{
			mvprintw(11 + i + tmp, 132, " ");
			mvprintw(11 + i + tmp, 137, " ");
			mvprintw(11 + i + tmp, 101, " ");
			if(i == 7)
			{
				for (int j = 0; j < 36; ++j)
				{
					mvprintw(11 + i + tmp, 132 + j, " ");
					mvprintw(11 + i + tmp, 132 - j, " ");
				}
			}
		}
	}
	for (int i = 0; i < 9; ++i)
	{
		mvprintw(2, 157 + i, " ");
		if(i == 4)
		{
			for (int j = 0; j < 7; ++j)
			{
				mvprintw(2 + j, 157 + i, " ");
			}
		}
	}
	for (int i = 0; i < 13; ++i)
	{
		mvprintw(27 + i, 106, " ");
		mvprintw(27 + i, 146, " ");
	}
	for (int i = 0; i < 21; ++i)
	{
		mvprintw(30, 147 + i, " ");
		mvprintw(34, 147 + i, " ");
		mvprintw(38, 147 + i, " ");
		mvprintw(39, 147 + i, " ");
	}
	mvprintw(6, 32, "/");
	mvprintw(6, 38, "\\");
	mvprintw(6, 56, "/");
	mvprintw(6, 62, "\\");
	mvprintw(11, 76, "------");
	mvprintw(14, 80, "/");
	mvprintw(11, 14, "-----");
	mvprintw(14, 14, "\\");
	mvprintw(27, 76, "-----");
	mvprintw(24, 80, "\\");
	mvprintw(27, 14, "-----");
	mvprintw(24, 14, "/");
	mvprintw(39, 64, "-----");
	mvprintw(39, 26, "-----");
	mvprintw(36, 68, "\\");
	mvprintw(36, 26, "/");
	mvprintw(44, 50, "/");
	mvprintw(44, 44, "\\");

	attroff(COLOR_PAIR(8));
	//print boat
	print_boat(5, 3, 56);
	print_boat(4, 3, 32);
	print_boat(6, 11, 81);
	print_boat(3, 25, 81);
	print_boat(8, 37, 69);
	print_boat(6, 45, 44);
	print_boat(1, 37, 19);
	print_boat(6, 25, 7);
	print_boat(6, 11, 7);
	mvprintw(50,168,"");
}

bool agree_or_disagree(int y, int x, char *deal, MEVENT event)
{
	set_background_color_init();
	clear_right_cornor();
	mvprintw(41,133,"Are you sure you want to: ");
	mvprintw(42,133,deal);
	bool decision;
	attron(COLOR_PAIR(3));
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			mvprintw(y + j, x + i, " ");
		}
	}
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(1));
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			mvprintw(y + j, x + i + 12, " ");
		}
	}
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(8));
	mvprintw(y + 1, x + 14, " ");
	mvprintw(y + 3, x + 14, " ");
	mvprintw(y + 3, x + 18, " ");
	mvprintw(y + 1, x + 18, " ");
	mvprintw(y + 2, x + 16, " ");
	mvprintw(y + 2, x + 2, " ");
	mvprintw(y + 3, x + 3, " ");
	mvprintw(y + 3, x + 4, " ");
	mvprintw(y + 2, x + 5, " ");
	mvprintw(y + 1, x + 6, " ");
	mvprintw(50,168,"");
	attroff(COLOR_PAIR(8));
	int ch;
	while (ch = getch())
	{
        if (ch == KEY_MOUSE && getmouse(&event) == OK)
        {
            if (event.bstate & BUTTON1_PRESSED)
            {
                int x = event.x;
                int y = event.y;
                if(((x >= 140 && y >= 43) && (x <= 148 && y <= 47)))
                {
                	decision = TRUE;
                	break;
                }
                else if(((x >= 152 && y >= 43) && (x <= 160 && y <= 47)))
                {
                	decision = FALSE;
                	break;
                }
            }
        }
    }
    clear_right_cornor();
	mvprintw(50,168,"");
	return decision;
}

int roll_and_print_dice(int y, int x)
{
	set_background_color_init();
	int val = rand() % 6 + 1;
	attron(COLOR_PAIR(8));
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			mvprintw(y + j, x + i, " ");
		}
	}
	if(val == 1)
	{
			attron(COLOR_PAIR(1));
			mvprintw(y + 2, x + 4, " ");
			attroff(COLOR_PAIR(1));
	}
	else if(val == 2)
	{
		attron(COLOR_PAIR(7));
		mvprintw(y + 2, x + 2, " ");
		mvprintw(y + 2, x + 6, " ");
		attroff(COLOR_PAIR(7));
	}
	else if(val == 3)
	{
		attron(COLOR_PAIR(7));
		mvprintw(y + 1, x + 2, " ");
		mvprintw(y + 2, x + 4, " ");
		mvprintw(y + 3, x + 6, " ");
		attroff(COLOR_PAIR(7));
	}
	else if(val == 4)
	{
		attron(COLOR_PAIR(1));
		mvprintw(y + 1, x + 2, " ");
		mvprintw(y + 3, x + 2, " ");
		mvprintw(y + 3, x + 6, " ");
		mvprintw(y + 1, x + 6, " ");
		attroff(COLOR_PAIR(1));
	}
	else if(val == 5)
	{
		attron(COLOR_PAIR(7));
		mvprintw(y + 1, x + 2, " ");
		mvprintw(y + 3, x + 2, " ");
		mvprintw(y + 3, x + 6, " ");
		mvprintw(y + 1, x + 6, " ");
		mvprintw(y + 2, x + 4, " ");
		attroff(COLOR_PAIR(7));
	}
	else
	{
		attron(COLOR_PAIR(7));
		mvprintw(y + 1, x + 2, " ");
		mvprintw(y + 3, x + 2, " ");
		mvprintw(y + 3, x + 6, " ");
		mvprintw(y + 1, x + 6, " ");
		mvprintw(y + 1, x + 4, " ");
		mvprintw(y + 3, x + 4, " ");
		attroff(COLOR_PAIR(7));
	}

	attroff(COLOR_PAIR(8));
	return val;
}

void print_pass()
{
	set_background_color_init();
	clear_right_cornor();
	int c_y = 43;
	int c_x = 137;
	//print p
	attron(COLOR_PAIR(8));
	for (int i = 0; i < 4; ++i)
	{
		if(i == 0)
		{
			for (int j = 0; j < 5; ++j)
			{
				mvprintw(c_y + j, c_x ," ");
			}
		}
		else if(i == 3)
		{
			mvprintw(c_y + 1, c_x, " ");
		}
		else
		{
			mvprintw(c_y, c_x, " ");
			mvprintw(c_y + 2, c_x, " ");
		}
		c_x ++;
	}
	//print a
	c_x += 3;
	c_y += 4;
	for (int i = 0; i < 2; ++i)
	{
		if(i == 0)
		{
			for (int j = 0; j < 3; ++j)
			{
				mvprintw(c_y - j,c_x, " ");
			}
			c_y -= 3;
		}
		c_x ++;
		mvprintw(c_y, c_x, " ");
		c_y --;
	}
	c_x ++;
	c_y ++;
	for (int i = 0; i < 3; ++i)
	{
		if(i == 2)
		{
			for (int j = 0; j < 3; ++j)
			{
				mvprintw(c_y + j,c_x, " ");
			}
		}
		mvprintw(c_y, c_x, " ");
		c_y ++;
		c_x ++;
	}
	for (int i = 0; i < 4; ++i)
	{
		mvprintw(c_y - 1, c_x - i - 2, " ");
	}
	//print s
	c_x += 3;
	c_y -= 3;
	for (int i = 0; i < 4; ++i)
	{
		mvprintw(c_y, c_x + i, " ");
		mvprintw(c_y + 4, c_x + i, " ");
		if(i == 0)
		{
			mvprintw(c_y + 1, c_x + i, " ");
		}
		else if(i == 3)
		{
			mvprintw(c_y + 3, c_x + i, " ");
		}
		else
		{
			mvprintw(c_y + 2, c_x + i, " ");
		}
	}
	c_x += 7;
	for (int i = 0; i < 4; ++i)
	{
		mvprintw(c_y, c_x + i, " ");
		mvprintw(c_y + 4, c_x + i, " ");
		if(i == 0)
		{
			mvprintw(c_y + 1, c_x + i, " ");
		}
		else if(i == 3)
		{
			mvprintw(c_y + 3, c_x + i, " ");
		}
		else
		{
			mvprintw(c_y + 2, c_x + i, " ");
		}
	}
	attroff(COLOR_PAIR(8));
	mvprintw(50,168,"");
}

void print_c(int y ,int x)
{
	init_pair(1,COLOR_WHITE,COLOR_YELLOW);
	int c_y = y;
	int c_x = x;
	attron(COLOR_PAIR(1));
	for (int i = 0; i < 10; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	c_x -= 1;
	c_y += 1;
	for (int i = 0; i < 11; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	c_x -= 1;
	c_y += 1;
	for (int i = 0; i < 2; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	c_x -= 1;
	c_y += 1;
	for (int i = 0; i < 2; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	c_x -= 1;
	c_y += 1;
	for (int i = 0; i < 2; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	c_y += 1;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y + i,c_x + j," ");
		}
	}
	c_x += 1;
	c_y += 3;
	for (int i = 0; i < 2; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	c_x += 1;
	c_y += 1;
	for (int i = 0; i < 2; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	c_x += 1;
	c_y += 1;
	for (int i = 0; i < 11; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	c_x += 1;
	c_y += 1;
	for (int i = 0; i < 10; ++i)
	{
		mvprintw(c_y,c_x + i," ");
	}
	attroff(COLOR_PAIR(1));
}


void print_a(int y ,int x)
{
	init_pair(1,COLOR_WHITE,COLOR_YELLOW);
	int c_y = y;
	int c_x = x;
	attron(COLOR_PAIR(1));

	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y - i,c_x + j," ");
		}
		c_x ++;
		if(i == 11)
		{
			c_y -= i;
		}
	}
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y + i,c_x + j," ");
		}
		c_x ++;
		if(i == 6)
		{
			for (int k = 0; k < 11; ++k)
			{
				mvprintw(c_y + i, c_x - 12 + k, " ");
			}
		}
	}

	attroff(COLOR_PAIR(1));
}

void print_t(int y ,int x)
{
	init_pair(1,COLOR_WHITE,COLOR_YELLOW);
	int c_y = y;
	int c_x = x;
	attron(COLOR_PAIR(1));

	for (int i = 0; i < 18; ++i)
	{
		mvprintw(c_y,c_x + i," ");
		if(i == 8)
		{
			for (int j = 0; j < 12; ++j)
			{
				mvprintw(c_y + j,c_x + i," ");
				mvprintw(c_y + j,c_x + i + 1," ");
			}
		}
	}

	attroff(COLOR_PAIR(1));
}

void print_n(int y, int x)
{
	init_pair(1,COLOR_WHITE,COLOR_YELLOW);
	int c_y = y;
	int c_x = x;
	attron(COLOR_PAIR(1));

	for (int i = 0; i < 12; ++i)
	{
		mvprintw(c_y,c_x," ");
		mvprintw(c_y,c_x + 1," ");
		c_y --;
	}
	c_x += 2;
	c_y ++;
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y,c_x + j, " ");
		}
		c_x ++;
		c_y ++;
	}
	c_x ++;
	c_y --;
	for (int i = 0; i < 12; ++i)
	{
		mvprintw(c_y,c_x," ");
		mvprintw(c_y,c_x + 1," ");
		c_y --;
	}

	attroff(COLOR_PAIR(1));
}

void print_boat(int color, int y, int x)
{
	attron(COLOR_PAIR(color));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			mvprintw(y + i, x + j, " ");
		}
	}
	if(color == 6)
	{
		mvprintw(y + 1, x + 1, "3 : 1");
	}
	else
	{
		mvprintw(y + 1, x + 1, "2 : 1");
	}
	attroff(COLOR_PAIR(color));
}

void button_play_and_quit(int y, int x)
{
	init_pair(2,COLOR_WHITE,COLOR_WHITE);
	attron(COLOR_PAIR(2));
	int c_y = y;
	int c_x = x;
	for (int i = 0; i < 8; ++i)
	{
		if(i == 0 || i == 7)
		{
			for (int j = 0; j < 33; ++j)
			{
				mvprintw(c_y,c_x + j," ");
			}
		}
		else
		{
			mvprintw(c_y,c_x," ");
			mvprintw(c_y,c_x + 32," ");
		}
		c_y ++;
	}
	c_y = y + 2;
	c_x = x + 3;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y + i,c_x + j, " ");
		}
	}
	c_x += 2;
	for (int i = 0; i < 3; ++i)
	{
		if(i != 1)
		{
			mvprintw(c_y + i,c_x," ");
			mvprintw(c_y + i,c_x + 1," ");
		}
		else
		{
			mvprintw(c_y + i,c_x + 2," ");
		}
	}
	c_y = y + 2;
	c_x = x + 9;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y + i,c_x + j, " ");
		}
		if(i == 3)
		{
			mvprintw(c_y + i,c_x + 1, " ");
			mvprintw(c_y + i,c_x + 2, " ");
			mvprintw(c_y + i,c_x + 3, " ");
			mvprintw(c_y + i,c_x + 4, " ");
		}
	}
	c_y = y + 5;
	c_x = x + 15;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y - j,c_x + i," ");
		}
		c_y --;
	}
	c_x += 2;
	mvprintw(c_y,c_x + 1," ");
	c_x += 2;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y + j,c_x + i," ");
		}
		if(i == 2)
		{
			for (int k = 0; k < 5; ++k)
			{
				mvprintw(c_y,c_x - 4 + k, " ");
			}
		}
		c_y ++;
	}
	c_y -= 3;
	c_x += 4;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y + i,c_x + j, " ");
		}
		c_x ++;
		if(i == 1)
		{
			c_x ++;
			for (int k = 0; k < 2; ++k)
			{
				mvprintw(c_y + i + k + 1,c_x, " ");
				mvprintw(c_y + i + k + 1,c_x + 1, " ");
			}
		}
	}
	c_y += 1;
	c_x += 2;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y,c_x + j, " ");
		}
		c_y --;
		c_x ++;
	}
	c_x = x;
	c_y = y + 10;
	for (int i = 0; i < 8; ++i)
	{
		if(i == 0 || i == 7)
		{
			for (int j = 0; j < 33; ++j)
			{
				mvprintw(c_y,c_x + j," ");
			}
		}
		else
		{
			mvprintw(c_y,c_x," ");
			mvprintw(c_y,c_x + 32," ");
		}
		c_y ++;
	}
	c_x = x + 4;
	c_y = y + 12;
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			mvprintw(c_y,c_x + i," ");
			mvprintw(c_y + 3,c_x + i," ");
		}
		if(i != 2 && i != 3)
		{
			mvprintw(c_y + 1,c_x + i," ");
			mvprintw(c_y + 2,c_x + i," ");
		}
		if(i == 3)
		{
			mvprintw(c_y + 2,c_x + i," ");
		}
		if(i == 5)
		{
			mvprintw(c_y + 3,c_x + i + 1," ");
		}
	}
	c_x = x + 12;
	c_y = y + 13;
	for (int i = 0; i < 6; ++i)
	{
		if(i != 2 && i != 3)
		{
			for (int j = 0; j < 2; ++j)
			{
				mvprintw(c_y + j, c_x + i, " ");
			}
		}
		if(i != 0 && i != 5)
		{
			mvprintw(c_y + 2, c_x + i, " ");
		}
	}
	c_x = x + 20;
	c_y = y + 12;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if(j != 1)
			{
				mvprintw(c_y + j, c_x + i, " ");
			}
		}
	}
	c_x = x + 23;
	c_y = y + 13;
	for (int i = 0; i < 6; ++i)
	{
		mvprintw(c_y, c_x + i, " ");
		if(i == 2 || i == 3)
		{
			mvprintw(c_y - 1, c_x + i, " ");
			for (int j = 0; j < 3; ++j)
			{
				mvprintw(c_y + j, c_x + i, " ");
			}
		}
	}
	attroff(COLOR_PAIR(2));
}

void clear_right_cornor()
{
	set_background_color_init();
	attron(COLOR_PAIR(7));
	for (int i = 0; i < 35; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			mvprintw(41 + j, 133 + i, " ");
		}
	}
	attroff(COLOR_PAIR(7));
	mvprintw(50,168,"");
}

void check_ui_size()
{
    struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    printf("Current terminal：%d rows x %d columns\n", size.ws_row, size.ws_col);

    while(size.ws_row < 51 && size.ws_col < 169)
    {
    	printf("Please use 'Ctrl' + '-' or other methods to resize your terminal until it reaches row and column as 51 and 169.\n");
    	printf("If you done resizing, press enter.\n");
    	getchar();
	    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	    printf("Current terminal：%d rows x %d columns\n", size.ws_row, size.ws_col);
    }

    return;
}

int start_screen(MEVENT event)
{
	clear();
	set_background_color_init();
	print_c(5,19);
    print_a(16,37);
    print_t(5,67);
    print_a(16,90);
    print_n(16,123);
	attron(COLOR_PAIR(8));
	for (int i = 0; i < 9; ++i)
	{
		if(i == 0 || i == 8)
		{
			for (int j = 0; j < 10; ++j)
			{
				mvprintw(41 + j, 160 + i, " ");
			}
		}
		else
		{
			mvprintw(41, 160 + i, " ");
			mvprintw(50, 160 + i, " ");
		}
	}
	mvprintw(44, 162, " ");
	mvprintw(43, 163, " ");
	mvprintw(43, 164, " ");
	mvprintw(43, 165, " ");
	mvprintw(44, 166, " ");
	mvprintw(44, 162, " ");
	mvprintw(45, 165, " ");
	mvprintw(46, 164, " ");
	mvprintw(48, 164, " ");
	attroff(COLOR_PAIR(8));
    button_play_and_quit(24,61);
   	mvprintw(50,168,"");
	int ch;
    while (ch = getch())
    {
        if (ch == KEY_MOUSE && getmouse(&event) == OK)
        {
            if (event.bstate & BUTTON1_PRESSED)
            {
                int x = event.x;
                int y = event.y;
                // attron(COLOR_PAIR(1));
                // mvprintw(y,x," ");
                // attroff(COLOR_PAIR(1));
                // mvprintw(0, 0, "Mouse Clicked at: x=%d, y=%d", x, y);
                if((x >= 62 && y >= 25) && (x <= 92 && y <= 30))
                {
                	clear();
                	return 1;
                }
                if((x >= 62 && y >= 35) && (x <= 92 && y <= 45))
                {
                	clear();
                	return -1;
                }
                if((x >= 161 && y >= 42) && (x <= 167 && y <= 49))
                {
                	clear();
                	help();
                	set_background_color_init();
					print_c(5,19);
				    print_a(16,37);
				    print_t(5,67);
				    print_a(16,90);
				    print_n(16,123);
					attron(COLOR_PAIR(8));
					for (int i = 0; i < 9; ++i)
					{
						if(i == 0 || i == 8)
						{
							for (int j = 0; j < 10; ++j)
							{
								mvprintw(41 + j, 160 + i, " ");
							}
						}
						else
						{
							mvprintw(41, 160 + i, " ");
							mvprintw(50, 160 + i, " ");
						}
					}
					mvprintw(44, 162, " ");
					mvprintw(43, 163, " ");
					mvprintw(43, 164, " ");
					mvprintw(43, 165, " ");
					mvprintw(44, 166, " ");
					mvprintw(44, 162, " ");
					mvprintw(45, 165, " ");
					mvprintw(46, 164, " ");
					mvprintw(48, 164, " ");
					attroff(COLOR_PAIR(8));
				    button_play_and_quit(24,61);
				   	mvprintw(50,168,"");
                }
            }
        }
    }
}

void print_bank(bank_property *bank)
{
	//clean
	attron(COLOR_PAIR(7));
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 46; ++j)
		{
			mvprintw(1 + i, 108 + j, " ");
		}
	}
	attroff(COLOR_PAIR(7));
	//storage
	for (int i = 0; i < 6; ++i)
	{
		int tmp = 0;
		if (i == 0)
		{
			tmp = 3;
			mvprintw(7, 117 + (i * 5), "x%d",bank -> wood);
		}
		else if(i == 1)
		{
			tmp = 1;
			mvprintw(7, 117 + (i * 5), "x%d",bank -> brick);
		}
		else if(i == 2)
		{
			tmp = 5;
			mvprintw(7, 117 + (i * 5), "x%d",bank -> sheep);
		}
		else if(i == 3)
		{
			tmp = 4;
			mvprintw(7, 117 + (i * 5), "x%d",bank -> wheat);
		}
		else if(i == 4)
		{
			tmp = 8;
			mvprintw(7, 117 + (i * 5), "x%d",bank -> stone);
		}
		else
		{
			tmp = 6;
			mvprintw(7, 117 + (i * 5), "x%d",bank -> special_cards);
		}
		attron(COLOR_PAIR(tmp));
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				mvprintw(3 + j, 117 + (i * 5) + k, " ");
			}
		}
		attroff(COLOR_PAIR(tmp));
	}
}

void print_players_status(player_property *player_1, player_property *player_2, player_property *player_3, player_property *player_4)
{
	_print_player(player_1, 11, 97, 1);
	_print_player(player_2, 11, 133, 2);
	_print_player(player_3, 19, 97 ,3);
	_print_player(player_4, 19, 133, 4);
}

void _print_player(player_property *player ,int y, int x , int color)
{
	int c_x = x;
	int c_y = y;
	attron(COLOR_PAIR(color));
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			mvprintw(c_y + j, c_x + i, " ");
		}
	}
	attroff(COLOR_PAIR(color));
	attron(COLOR_PAIR(color + 20));
	mvprintw(c_y + 5, c_x +1, "%d",player -> total_victory_points);
	attroff(COLOR_PAIR(color + 20));
	//clean
	attron(COLOR_PAIR(7));
	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 30; ++j)
		{
			mvprintw(c_y + i, c_x + 5 + j, " ");
		}
	}
	attroff(COLOR_PAIR(7));
	//print total cards
	c_x += 7;
	c_y += 1;
	attron(COLOR_PAIR(2));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(c_y + j, c_x + i, " " );
		}
	}
	attroff(COLOR_PAIR(2));
	mvprintw(c_y + 4 , c_x, "x%d", player -> total_resource_cards);
	//print total special cards
	c_x += 6;
	attron(COLOR_PAIR(6));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(c_y + j, c_x + i, " " );
		}
	}
	attroff(COLOR_PAIR(6));
	mvprintw(c_y + 4 , c_x, "x%d", player -> special_cards);
	//print knights
	c_x += 6;
	attron(COLOR_PAIR(8));
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(c_y + j, c_x + i, " ");
		}

	}
	attroff(COLOR_PAIR(8));
	attron(COLOR_PAIR(7));
	for (int i = 0; i < 3; ++i)
	{
		mvprintw(c_y + 1, c_x + 1 + i, " ");
		if(i == 1)
		{
			mvprintw(c_y + 2, c_x + 1 + i, " ");
		}
	}
	attroff(COLOR_PAIR(7));
	mvprintw(c_y + 4, c_x + 2, "x%d",player -> knights_use );
	//print roads
	c_y += 2;
	c_x += 7;
	attron(COLOR_PAIR(8));
	for (int i = 0; i < 7; ++i)
	{
		mvprintw(c_y ,c_x + i, " ");
		if(i < 2)
		{
			c_y --;
		}
		else if(i > 3)
		{
			c_y ++;
		}
	}
	attroff(COLOR_PAIR(8));
	mvprintw(c_y + 1, c_x + 2, "x%d",player -> max_roads);
}

void print_YOU(player_property *player, card_temp *cardtemp)
{
	attron(COLOR_PAIR(1));
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 13; ++j)
		{
			mvprintw(27 + j, 97 + i, " ");
		}
	}
	attroff(COLOR_PAIR(2));
	//clean
	attron(COLOR_PAIR(7));
	for (int i = 0; i < 13; ++i)
	{
		for (int j = 0; j < 39; ++j)
		{
			mvprintw(27 + i, 107 + j, " ");
		}
	}
	attroff(COLOR_PAIR(7));
	//print cards
	attron(COLOR_PAIR(3));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(28 + i, 111 + j, " ");
		}
	}
	attroff(COLOR_PAIR(3));
	mvprintw(32, 111, "x%d", player -> wood);
	attron(COLOR_PAIR(1));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(28 + i, 118 + j, " ");
		}
	}
	attroff(COLOR_PAIR(1));
	mvprintw(32, 118, "x%d", player -> brick);
	attron(COLOR_PAIR(5));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(28 + i, 125 + j, " ");
		}
	}
	attroff(COLOR_PAIR(5));
	mvprintw(32, 125, "x%d", player -> sheep);
	attron(COLOR_PAIR(4));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(28 + i, 132 + j, " ");
		}
	}
	attroff(COLOR_PAIR(4));
	mvprintw(32, 132, "x%d", player -> wheat);
	attron(COLOR_PAIR(8));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(28 + i, 139 + j, " ");
		}
	}
	attroff(COLOR_PAIR(8));
	mvprintw(32, 139, "x%d", player -> stone);
	attron(COLOR_PAIR(6));
	for (int i = 0; i < 5; ++i)
	{
		for (int a = 0; a < 3; ++a)
		{
			for (int b = 0; b < 3; ++b)
			{
				mvprintw(34 + a, 111 + b + (i * 7), " ");
			}
		}
	}
	attroff(COLOR_PAIR(6));
	attron(COLOR_PAIR(1));
	mvprintw(35, 112 ," ");
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(5));
	mvprintw(35, 119 ," ");
	attroff(COLOR_PAIR(5));
	attron(COLOR_PAIR(8));
	mvprintw(35, 126 ," ");
	attroff(COLOR_PAIR(8));
	attron(COLOR_PAIR(2));
	mvprintw(35, 133 ," ");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(4));
	mvprintw(35, 140 ," ");
	attroff(COLOR_PAIR(4));
	mvprintw(38, 111, "x%d+%d", player -> knights, cardtemp -> knights);
	mvprintw(38, 118, "x%d+%d", player -> year_of_plenty, cardtemp -> year_of_plenty);
	mvprintw(38, 125, "x%d+%d", player -> road_building, cardtemp -> road_building);
	mvprintw(38, 132, "x%d+%d", player -> monopoly, cardtemp -> monopoly);
	mvprintw(38, 139, "x%d", player -> victory_card);
	mvprintw(28, 149, "Village Remain: %d", player -> village_remain);
	mvprintw(32, 149, "City Remain: %d", player -> city_remain);
	mvprintw(36, 149, "Road Remain: %d ", player -> road_remain);
	player -> knights += cardtemp -> knights;
	player -> year_of_plenty += cardtemp -> year_of_plenty;
	player -> road_building += cardtemp -> road_building;
	player -> monopoly += cardtemp -> monopoly;
	card_temp_init( cardtemp );
}

void print_trade_ui(player_property *player, player_property *player_2, player_property *player_3, player_property *player_4, bank_property *bank, MEVENT event, int trade_withbank[], card_temp *cardtemp)
{
	//clean map
	attron(COLOR_PAIR(7));
	for (int i = 0; i < 95; ++i)
	{
		for (int j = 0; j < 49; ++j)
		{
			mvprintw(1 + j, 1 + i, " ");
		}
	}
	attroff(COLOR_PAIR(7));
	attron(COLOR_PAIR(8));
	for (int i = 0; i < 95; ++i)
	{
		mvprintw(26, 1 + i, " ");
	}
	clear_right_cornor();
	attroff(COLOR_PAIR(8));
	attron(COLOR_PAIR(7));
	mvprintw(12, 1, "Bank's Storage: ");
	mvprintw(28, 1, "Player's Resource: ");
	mvprintw(32, 1, "Player's Wood Exchange Rate: %d : 1", player -> wood_exchange_rate);
	mvprintw(36, 1, "Player's Brick Exchange Rate: %d : 1", player -> brick_exchange_rate);
	mvprintw(40, 1, "Player's Sheep Exchange Rate: %d : 1", player -> sheep_exchange_rate);
	mvprintw(44, 1, "Player's Wheat Exchange Rate: %d : 1", player -> wheat_exchange_rate);
	mvprintw(48, 1, "Player's Stone Exchange Rate: %d : 1", player -> stone_exchange_rate);
	attroff(COLOR_PAIR(7));
	print_a_card(31, 44, 3);
	mvprintw(35, 44, "x%d",player -> wood);
	print_a_card(37, 44, 1);
	mvprintw(41, 44, "x%d", player -> brick);
	print_a_card(43, 44, 5);
	mvprintw(47, 44, "x%d", player -> sheep);
	print_a_card(31, 60, 4);
	mvprintw(35, 60, "x%d",player -> wheat);
	print_a_card(37, 60, 8);
	mvprintw(41, 60, "x%d", player -> stone);
	print_a_card(5, 44, 3);
	mvprintw(9, 44, "x%d", bank -> wood);
	print_a_card(11, 44, 1);
	mvprintw(15, 44, "x%d", bank -> brick);
	print_a_card(17, 44, 5);
	mvprintw(21, 44, "x%d", bank -> sheep);
	print_a_card(5, 60, 4);
	mvprintw(9, 60, "x%d", bank -> wheat);
	print_a_card(11, 60, 8);
	mvprintw(15, 60, "x%d", bank -> stone);
	print_a_card(17, 60, 6);
	mvprintw(21, 60, "x%d",bank ->knights+bank->year_of_plenty+bank->road_building+bank->monopoly+bank->victory_card);
	attron(COLOR_PAIR(3));
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			mvprintw(45 + j, 87 + i, " ");
		}
	}
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(8));
	mvprintw(45 + 2, 87 + 2, " ");
	mvprintw(45 + 3, 87 + 3, " ");
	mvprintw(45 + 3, 87 + 4, " ");
	mvprintw(45 + 2, 87 + 5, " ");
	mvprintw(45 + 1, 87 + 6, " ");
	attroff(COLOR_PAIR(8));
	int ch;
	int tmp_trade_point = 0;
	int player_wood_reduction = 0;
	int player_brick_reduction = 0;
	int player_sheep_reduction = 0;
	int player_wheat_reduction = 0;
	int player_stone_reduction = 0;
	int bank_wood_reduction = 0;
	int bank_brick_reduction = 0;
	int bank_sheep_reduction = 0;
	int bank_wheat_reduction = 0;
	int bank_stone_reduction = 0;
	int bank_special_card_reduction = 0;
	int special_card_count = 0;
	int trade_pay = 0, trade_get = 0;
    mvprintw(50, 168," ");
	attron(COLOR_PAIR(31));
	while (ch = getch())
	{
        if (ch == KEY_MOUSE && getmouse(&event) == OK)
        {
            if (event.bstate & BUTTON1_PRESSED)
            {
                int x = event.x;
                int y = event.y;
                if(((x >= 44 && y >= 31) && (x <= 46 && y <= 33)))
                {
                	/*player_wood_reduction ++;
                	mvprintw(35, 47, "-%d",player_wood_reduction);
                	tmp_trade_point += player_wood_reduction / player -> wood_exchange_rate;*/
                	trade_withbank[0] ++;
                	mvprintw(35, 47, "-%d",trade_withbank[0]);
                	trade_pay++;
                }
                else if(((x >= 44 && y >= 37) && (x <= 46 && y <= 39)))
                {
                	/*player_brick_reduction ++;
                	mvprintw(41, 47, "-%d",player_brick_reduction);
                	tmp_trade_point += player_brick_reduction / player -> brick_exchange_rate;*/
                	trade_withbank[2] ++;
                	mvprintw(41, 47, "-%d",trade_withbank[2]);
                	trade_pay++;
                }
                else if(((x >= 44 && y >= 43) && (x <= 46 && y <= 45)))
                {
                	/*player_sheep_reduction ++;
                	mvprintw(47, 47, "-%d", player_sheep_reduction);
                	tmp_trade_point += (player_sheep_reduction - bank_special_card_reduction) / player -> sheep_exchange_rate;*/
                	trade_withbank[3] ++;
                	mvprintw(47, 47, "-%d",trade_withbank[3]+player_sheep_reduction);
                	trade_pay++;
                }
                else if(((x >= 60 && y >= 31) && (x <= 62 && y <= 33)))
                {
                	/*player_wheat_reduction ++;
                	mvprintw(35, 63, "-%d", player_wheat_reduction);
                	tmp_trade_point += (player_wheat_reduction - bank_special_card_reduction) / player -> wheat_exchange_rate;*/
                	trade_withbank[4] ++;
                	mvprintw(35, 63, "-%d",trade_withbank[4]+player_wheat_reduction);
                	trade_pay++;
                	
                }
                else if(((x >= 60 && y >= 37) && (x <= 62 && y <= 39)))
                {
                	/*player_stone_reduction ++;
                	mvprintw(41, 63, "-%d", player_stone_reduction);
                	tmp_trade_point += (player_stone_reduction - bank_special_card_reduction) / player -> stone_exchange_rate;*/
                	trade_withbank[1] ++;
                	mvprintw(41, 63, "-%d",trade_withbank[1]+player_stone_reduction);
                	trade_pay++;
                }
                else if(((x >= 87 && y >= 45) && (x <= 95 && y <= 49)))
                {
                	break;
                }
                else if(((x >= 44 && y >= 5) && (x <= 46 && y <= 7)) /*&& tmp_trade_point > 0*/)
                {
                	/*bank_wood_reduction ++;
                	mvprintw(9, 47, "-%d", bank_wood_reduction);*/
                	trade_withbank[5] ++;
                	mvprintw(9, 47, "-%d",trade_withbank[5]);
                	trade_get++;
                }
                else if(((x >= 44 && y >= 11) && (x <= 46 && y <= 13)) /*&& tmp_trade_point > 0*/)
                {
                	/*bank_brick_reduction ++;
                	mvprintw(15, 47, "-%d", bank_brick_reduction);*/
                	trade_withbank[7] ++;
                	mvprintw(15, 47, "-%d", trade_withbank[7]);
                	trade_get++;
                }
                else if(((x >= 44 && y >= 17) && (x <= 46 && y <= 19)) /*&& tmp_trade_point > 0*/)
                {
                	/*bank_sheep_reduction ++;
                	mvprintw(21, 47, "-%d", bank_sheep_reduction);*/
                	trade_withbank[8] ++;
                	mvprintw(21, 47, "-%d", trade_withbank[8]);
                	trade_get++;
                }
                else if(((x >= 60 && y >= 5) && (x <= 62 && y <= 7)) /*&& tmp_trade_point > 0*/)
                {
                	/*bank_wheat_reduction ++;
                	mvprintw(9, 63, "-%d", bank_wheat_reduction);*/
                	trade_withbank[9] ++;
                	mvprintw(9, 63, "-%d", trade_withbank[9]);
                	trade_get++;
                }
                else if(((x >= 60 && y >= 11) && (x <= 62 && y <= 13)) /*&& tmp_trade_point > 0*/)
                {
                	/*bank_stone_reduction ++;
                	mvprintw(15, 63, "-%d", bank_stone_reduction);*/
                	trade_withbank[6] ++;
                	mvprintw(15, 63, "-%d", trade_withbank[6]);
                	trade_get++;
                }
                else if(((x >= 60 && y >= 17) && (x <= 62 && y <= 19)))
                {
                	special_card_count++;
                	bank_special_card_reduction ++;
                	player_sheep_reduction ++;
                	player_stone_reduction ++;
                	player_wheat_reduction ++;
                	mvprintw(41, 63, "-%d", player_stone_reduction);
                	mvprintw(47, 47, "-%d", player_sheep_reduction);
                	mvprintw(35, 63, "-%d", player_wheat_reduction);
                	mvprintw(21, 63, "-%d", bank_special_card_reduction);
                }
            }
        }
    }
    /*if((player -> wood - player_wood_reduction < 0) || (player -> brick - player_brick_reduction < 0) || (player -> sheep - player_sheep_reduction < 0) || (player -> wheat - player_wheat_reduction < 0) || (player -> stone - player_stone_reduction < 0) ||(bank -> wood - bank_wood_reduction < 0 ) || (bank -> brick - bank_brick_reduction < 0) ||(bank -> sheep - bank_sheep_reduction < 0) ||(bank -> wheat - bank_wheat_reduction < 0) ||(bank -> stone - bank_stone_reduction < 0) ||(bank -> special_cards - bank_special_card_reduction < 0))*/
    if( (player -> wood < player -> wood_exchange_rate * trade_withbank[0]) || 
	    (player -> stone < player -> stone_exchange_rate * trade_withbank[1] + player_stone_reduction) ||
	    (player -> brick < player -> brick_exchange_rate * trade_withbank[2]) ||
	    (player -> sheep < player -> sheep_exchange_rate * trade_withbank[3] + player_sheep_reduction) ||
	    (player -> wheat < player -> wheat_exchange_rate * trade_withbank[4] + player_wheat_reduction) ||
	    (bank -> wood < trade_withbank[5]) ||
	    (bank -> stone < trade_withbank[6]) ||
	    (bank -> brick < trade_withbank[7]) ||
	    (bank -> sheep < trade_withbank[8]) ||
	    (bank -> wheat < trade_withbank[9]) ||
	    (trade_get != trade_pay) )
    {
    	mvprintw(45,138, "Transaction Failed!");
    }
    else
    {
    	if(agree_or_disagree(43, 140, "Trade like this?", event))
    	{
    		tradewithbank( player, bank, trade_withbank );
    		player -> wood -= player_wood_reduction;
    		player -> brick -= player_brick_reduction;
    		player -> sheep -= player_sheep_reduction;
    		player -> wheat -= player_wheat_reduction;
    		player -> stone -= player_stone_reduction;
    		player -> special_cards += bank_special_card_reduction;
    		bank -> wood -= bank_wood_reduction;
    		bank -> brick -= bank_brick_reduction;
    		bank -> sheep -= bank_sheep_reduction;
    		bank -> wheat -= bank_wheat_reduction;
    		bank -> stone -= bank_stone_reduction;
    		player -> wood += bank_wood_reduction;
    		player -> brick += bank_brick_reduction;
    		player -> sheep += bank_sheep_reduction;
    		player -> wheat += bank_wheat_reduction;
    		player -> stone += bank_stone_reduction;
    		bank -> wood += player_wood_reduction;
    		bank -> brick += player_brick_reduction;
    		bank -> sheep += player_sheep_reduction;
    		bank -> wheat += player_wheat_reduction;
    		bank -> stone += player_stone_reduction;
    		bank -> special_cards -= bank_special_card_reduction;
    		specialcard_get( cardtemp, player, bank, special_card_count );
    		
    		player -> total_resource_cards = player -> wood + player -> brick + player -> sheep + player -> wheat + player -> stone;
    	}
    }
    trade_init(trade_withbank);
   	refresh_all_status(player,player_2,player_3,player_4,bank,cardtemp,trade_withbank);
    attroff(COLOR_PAIR(31));
    //feature : u can discard card like this way!
    mvprintw(50, 168," ");
}


void refresh_all_status(player_property *player_1, player_property *player_2, player_property *player_3, player_property *player_4, bank_property *bank, card_temp *cardtemp, int trade_withbank[])
{
	print_in_game_ui();
	print_players_status(player_1 ,player_2 ,player_3 ,player_4);
	print_YOU(player_1, cardtemp);
	_print_player(player_1, 11, 97, 1);
	print_bank(bank);
	show_all_objects();
	card_temp_init( cardtemp );
	trade_init( trade_withbank );
}

void help(MEVENT event)
{
	set_background_color_init();
	int ch;
    while (ch = getch())
    {
		mvprintw(3, 5, "There are 12 kinds of cards that as follow:");
		print_a_card(6, 5, 3);
		mvprintw(7, 9, "This is Wood");
		print_a_card(12, 5, 1);
		mvprintw(13, 9, "This is Brick");
		print_a_card(18, 5, 5);
		mvprintw(19, 9, "This is Sheep");
		print_a_card(24, 5, 4);
		mvprintw(25, 9, "This is Wheat");
		print_a_card(30, 5, 8);
		mvprintw(31, 9, "This is Stone");
		print_a_card(36, 5, 2);
		mvprintw(37, 9, "This means all resource cards");
		print_a_card(6, 53, 6);
		mvprintw(7, 57, "This is Knight");
		attron(COLOR_PAIR(1));
		mvprintw(7, 54, " ");
		attroff(COLOR_PAIR(1));
		print_a_card(12, 53, 6);
		attron(COLOR_PAIR(5));
		mvprintw(13, 54, " ");
		attroff(COLOR_PAIR(5));
		mvprintw(13, 57, "This is Year of Plenty");
		print_a_card(18, 53, 6);
		attron(COLOR_PAIR(8));
		mvprintw(19, 54, " ");
		attroff(COLOR_PAIR(8));
		mvprintw(19, 57, "This is Road Building");
		print_a_card(24, 53, 6);
		attron(COLOR_PAIR(2));
		mvprintw(25, 54, " ");
		attroff(COLOR_PAIR(2));
		mvprintw(25, 57, "This is Monopoly");
		print_a_card(30, 53, 6);
		attron(COLOR_PAIR(4));
		mvprintw(31, 54, " ");
		attroff(COLOR_PAIR(4));
		mvprintw(31, 57, "This is Victory Card");
		print_a_card(36, 53, 6);
		mvprintw(37, 57, "This means all development cards");
		attron(COLOR_PAIR(8));
		int c_y = 6;
		int c_x = 104;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				mvprintw(c_y + j, c_x + i, " ");
			}

		}
		attroff(COLOR_PAIR(8));
		attron(COLOR_PAIR(7));
		for (int i = 0; i < 3; ++i)
		{
			mvprintw(c_y + 1, c_x + 1 + i, " ");
			if(i == 1)
			{
				mvprintw(c_y + 2, c_x + 1 + i, " ");
			}
		}
		attroff(COLOR_PAIR(7));
		mvprintw(7, 110, "This is the knight player has");
		c_y = 18;
		c_x = 103;
		attron(COLOR_PAIR(8));
		for (int i = 0; i < 7; ++i)
		{
			mvprintw(c_y ,c_x + i, " ");
			if(i < 2)
			{
				c_y --;
			}
			else if(i > 3)
			{
				c_y ++;
			}
		}
		attroff(COLOR_PAIR(8));
		mvprintw(17, 113, "This is the maxium road player has");
		mvprintw(45, 4, "If you done reading, press 'q' to back to menu.");
		//draw_with_mouse_and_return_value(event);
		if(ch == 'q')
		{
			clear();
			return;
		}
	}
}

void print_a_card(int y, int x, int color)
{
	attron(COLOR_PAIR(color));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			mvprintw(y + i, x + j, " ");
		}
	}
	attroff(COLOR_PAIR(color));
}

int check_winner(player_property *player_1, player_property *player_2, player_property *player_3, player_property *player_4)
{
	if(player_1 -> total_victory_points >= 10)
	{
		return 1;
	}
	else if(player_2 -> total_victory_points >= 10)
	{
		return 2;
	}
	else if(player_3 -> total_victory_points >= 10)
	{
		return 3;
	}
	else if(player_4 -> total_victory_points >= 10)
	{
		return 4;
	}
}

void set_background_color_init()
{
	init_pair(1,COLOR_WHITE,COLOR_RED);
    init_pair(2,COLOR_WHITE,COLOR_CYAN);
    init_pair(3,COLOR_WHITE,COLOR_GREEN);
    init_pair(4,COLOR_WHITE,COLOR_YELLOW);
    init_pair(5,COLOR_WHITE,COLOR_BLUE);//this will present as light green
    init_pair(6,COLOR_WHITE,COLOR_MAGENTA);//this is purple
    init_pair(7,COLOR_WHITE,COLOR_BLACK);
    init_pair(8,COLOR_BLACK,COLOR_WHITE);
    init_pair(21,COLOR_RED,COLOR_WHITE);
    init_pair(22,COLOR_CYAN,COLOR_WHITE);
    init_pair(23,COLOR_GREEN,COLOR_WHITE);
    init_pair(24,COLOR_YELLOW,COLOR_WHITE);
    init_pair(31,COLOR_RED,COLOR_BLACK);
    init_color(COLOR_WHITE,850,850,850);
    init_color(COLOR_MAGENTA,800,0,800);
    init_color(COLOR_BLUE,0,750,0);
    init_color(COLOR_RED,800,0,0);
    init_color(COLOR_GREEN,0,400,0);
	init_color(COLOR_YELLOW,800,800,0);
    init_color(COLOR_BLACK,200,200,200);
    init_color(COLOR_CYAN,0,200,750);
}
