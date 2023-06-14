#include "catan_ui.h"

extern const char* resources_name[];
extern i32 dice_nums[];
extern obj* body_list[];
extern obj* vertice_list[];
extern bank_property bank;
extern player_property players[];

void in_game_ui(MEVENT event)
{
	bank_property bank;
	player_property player_1;
	player_init(&player_1);
	bank_init(&bank);
	bool your_turn = TRUE;
	bool trade = TRUE;
	bool build_a_road = TRUE;//for test
	print_in_game_ui();
	print_bank(&bank);
	print_players_status(&player_1, &player_1, &player_1, &player_1);
	print_YOU(&player_1);
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
	                //in real playing following code will not proceed. it is just for test
					if(trade)
				    {
				    	agree_or_disagree(43, 140, "trade with blablabla?", event);
				    }
				    if(build_a_road)
				    {
				    	agree_or_disagree(43, 140, "build a road?", event);
				    }
	            }
	        }
		}
		while(ch = getch())
	    {
	        print_pass();
	        if (ch == KEY_MOUSE && getmouse(&event) == OK)
	        {
	            if (event.bstate & BUTTON1_PRESSED)
	            {
	                int x = event.x;
	                int y = event.y;
	                if(((x >= 133 && y >= 41) && (x <= 167 && y <= 49)))
	                {
	                	clear_right_cornor();
		                break;
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
void draw_with_mouse_and_return_value(MEVENT event)// for debug
{
	set_background_color_init();
    int ch;
    while ((ch = getch()) != 'q')
    {
        if(ch=='1')fprintf_player(players[0]);
        if(ch=='2')fprintf_player(players[1]);
        if(ch=='3')fprintf_player(players[2]);
        if(ch=='4')fprintf_player(players[3]);

        if(ch=='b')fprintf_bank(bank);

        if(ch=='p')clear_log();
        if (ch == KEY_MOUSE && getmouse(&event) == OK)
        {
            if (event.bstate & BUTTON1_PRESSED)
            {
                int x = event.x;
                int y = event.y;
                attron(COLOR_PAIR(8));
                //mvprintw(y,x," ");
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
			if (event.bstate & BUTTON3_PRESSED)
            {
                int x = event.x;
                int y = event.y;
                attron(COLOR_PAIR(7));
                mvprintw(y,x," ");
                attroff(COLOR_PAIR(7));
                mvprintw(0, 0, "Mouse Clicked at: x=%d, y=%d", x, y);
                refresh();
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
	attroff(COLOR_PAIR(8));
	roll_and_print_dice(43,104);
	roll_and_print_dice(43,116);
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
	set_background_color_init();
	print_c(5,19);
    print_a(16,37);
    print_t(5,67);
    print_a(16,90);
    print_n(16,123);
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
            }
        }
    }
}

void print_bank(bank_property *bank)
{
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
			mvprintw(7, 117 + (i * 5), "x%d",bank ->knights+bank->year_of_plenty+bank->road_building+bank->monopoly+bank->victory_card);
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
	mvprintw(c_y + 4, c_x + 2, "x%d",player -> knights );
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

void print_YOU(player_property *player)
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
	mvprintw(38, 111, "x%d", player -> knights);
	mvprintw(38, 118, "x%d", player -> year_of_plenty);
	mvprintw(38, 125, "x%d", player -> road_building);
	mvprintw(38, 132, "x%d", player -> monopoly);
	mvprintw(38, 139, "x%d", player -> victory_card);
	mvprintw(28, 149, "Village Remain: %d", player -> village_remain);
	mvprintw(32, 149, "City Remain: %d", player -> city_remain);
	mvprintw(36, 149, "Road Remain: %d", player -> road_remain);

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
    init_pair(21,COLOR_RED,COLOR_BLACK);
    init_pair(22,COLOR_CYAN,COLOR_BLACK);
    init_pair(23,COLOR_GREEN,COLOR_BLACK);
    init_pair(24,COLOR_YELLOW,COLOR_BLACK);
    init_color(COLOR_MAGENTA,800,0,800);
    init_color(COLOR_BLUE,0,1000,0);
    init_color(COLOR_RED,800,0,0);
    init_color(COLOR_GREEN,0,500,0);
	init_color(COLOR_YELLOW,900,900,0);
    init_color(COLOR_BLACK,200,200,200);
    init_color(COLOR_CYAN,0,200,750);
}