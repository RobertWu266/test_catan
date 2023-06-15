//
// Created by wu on 5/24/23.
//

#include "TUI.h"
#include "catan_ui.h"

extern char *attr_names[];
extern obj *innerbox[13][13][3];
i32 off_x=47;
i32 off_y=23;
#define hex_side_width 7//unused
#define hex_side_height 8//unused
#define game2custom(x,y) ((x-off_x)- (y-off_y))/ 4, (-(x-off_x) - 3 * (y-off_y))/ 4
#define custom2game(x,y)  -(x) -(y) +off_y,(3*(x)-(y) )+off_x
#define hex_a 2
#define hex_b 2


const char *resources_name[] = {"HILL", "MOUNTAIN", "FIELD", "PASTURE", "FOREST", "DESERT"};


#define FORI(X, Y) for(int32_t i=X;i<Y;i++)

void shuffle_i32(i32 *array, i32 n)
{
    if (n > 1)
    {
        for (size_t i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);//rand_between(1,n-i+1)
            int32_t temp = array[j];
            array[j] = array[i];
            array[i] = temp;
        }
    }
}
void print_array(i32 *array, i32 n)
{
    for (size_t i = 0; i < n - 1; i++)
    {
        printf("%d,",array[i]);
    }
}

obj *get_obj_from_mouse(i32 x, i32 y)
{
    i32 *tmplocs = calloc(2, sizeof(i32));
    obj *the_obj = locs_find_obj(tmplocs = custom2cube(game2custom(x,y)));
    free(tmplocs);
    return the_obj;
}
void print_polygon(obj *tgt, int y, int x)
{
    body_property *property = bprop(tgt);
    if(property -> has_robber)
    {
        attron(COLOR_PAIR(7));
        mvprintw(y, x - 3, "Robbers");
        attroff(COLOR_PAIR(7));
    }
    if(property -> resource == 0)
    {
        attron(COLOR_PAIR(1));
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int l = 7 - j; l >= 0 ; --l)
                {
                    mvprintw(y + j, x + l, " ");
                    mvprintw(y - j, x + l, " ");
                    mvprintw(y + j, x - l, " ");
                    mvprintw(y - j, x - l, " ");
                }
            }
        }
        mvprintw(y - 1, x - 2,"Hills");
        if(property -> num > 9)
        {
            int tmp = (property -> num) % 10;
            mvprintw(y + 1, x + 1,"%d",tmp);
            tmp = (property -> num - tmp) / 10;
            mvprintw(y + 1, x - 1,"%d",tmp);
        }
        else
        {
            mvprintw(y + 1, x,"%d",property -> num);
        }
        attroff(COLOR_PAIR(1));
    }
    else if(property -> resource == 1)
    {
        attron(COLOR_PAIR(8));
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int l = 7 - j; l >= 0 ; --l)
                {
                    mvprintw(y + j, x + l, " ");
                    mvprintw(y - j, x + l, " ");
                    mvprintw(y + j, x - l, " ");
                    mvprintw(y - j, x - l, " ");
                }
            }
        }
        mvprintw(y - 1, x - 4,"Mountains");
        if(property -> num > 9)
        {
            int tmp = (property -> num) % 10;
            mvprintw(y + 1, x + 1,"%d",tmp);
            tmp = (property -> num - tmp) / 10;
            mvprintw(y + 1, x - 1,"%d",tmp);
        }
        else
        {
            mvprintw(y + 1, x,"%d",property -> num);
        }
        attroff(COLOR_PAIR(8));
    }
    else if(property -> resource == 2)
    {
        attron(COLOR_PAIR(4));
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int l = 7 - j; l >= 0 ; --l)
                {
                    mvprintw(y + j, x + l, " ");
                    mvprintw(y - j, x + l, " ");
                    mvprintw(y + j, x - l, " ");
                    mvprintw(y - j, x - l, " ");
                }
            }
        }
        mvprintw(y - 1, x - 2,"Field");
        if(property -> num > 9)
        {
            int tmp = (property -> num) % 10;
            mvprintw(y + 1, x + 1,"%d",tmp);
            tmp = (property -> num - tmp) / 10;
            mvprintw(y + 1, x - 1,"%d",tmp);
        }
        else
        {
            mvprintw(y + 1, x,"%d",property -> num);
        }
        attroff(COLOR_PAIR(4));
    }
    else if(property -> resource == 3)
    {
        attron(COLOR_PAIR(5));
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int l = 7 - j; l >= 0 ; --l)
                {
                    mvprintw(y + j, x + l, " ");
                    mvprintw(y - j, x + l, " ");
                    mvprintw(y + j, x - l, " ");
                    mvprintw(y - j, x - l, " ");
                }
            }
        }
        mvprintw(y - 1, x - 3,"Pasture");
        if(property -> num > 9)
        {
            int tmp = (property -> num) % 10;
            mvprintw(y + 1, x + 1,"%d",tmp);
            tmp = (property -> num - tmp) / 10;
            mvprintw(y + 1, x - 1,"%d",tmp);
        }
        else
        {
            mvprintw(y + 1, x,"%d",property -> num);
        }
        attroff(COLOR_PAIR(5));
    }
    else if(property -> resource == 4)
    {
        attron(COLOR_PAIR(3));
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int l = 7 - j; l >= 0 ; --l)
                {
                    mvprintw(y + j, x + l, " ");
                    mvprintw(y - j, x + l, " ");
                    mvprintw(y + j, x - l, " ");
                    mvprintw(y - j, x - l, " ");
                }
            }
        }
        mvprintw(y - 1, x - 3,"Forests");
        if(property -> num > 9)
        {
            int tmp = (property -> num) % 10;
            mvprintw(y + 1, x + 1,"%d",tmp);
            tmp = (property -> num - tmp) / 10;
            mvprintw(y + 1, x - 1,"%d",tmp);
        }
        else
        {
            mvprintw(y + 1, x,"%d",property -> num);
        }
        attroff(COLOR_PAIR(3));
    }
    else if(property -> resource == 5)
    {
        attron(COLOR_PAIR(7));
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                for (int l = 7 - j; l >= 0 ; --l)
                {
                    mvprintw(y + j, x + l, " ");
                    mvprintw(y - j, x + l, " ");
                    mvprintw(y + j, x - l, " ");
                    mvprintw(y - j, x - l, " ");
                }
            }
        }
        mvprintw(y - 1, x - 3,"Deserts");
        if(property -> num > 9)
        {
            int tmp = (property -> num) % 10;
            mvprintw(y + 1, x + 1,"%d",tmp);
            tmp = (property -> num - tmp) / 10;
            mvprintw(y + 1, x - 1,"%d",tmp);
        }
        else
        {
            mvprintw(y + 1, x,"%d",property -> num);
        }
        attroff(COLOR_PAIR(7));
    }
}

void show_obj(obj* tgt)
{
    set_background_color_init();
    if(!tgt)return;

    wmove(stdscr,custom2game(obj_custom_x(tgt), obj_custom_y(tgt)));
    i32 y,x;
    getyx(stdscr,y,x);
    switch (tgt->attr)
    {
        case body:
            print_polygon(tgt, y, x);
            //mvprintw(y, x,"X%d",bprop(tgt)->num);
            break;
        case neg_vert:
        case pos_vert:
            if(tgt -> highlighted)
            {
                attron(COLOR_PAIR(6));
                mvprintw(y, x,"X");
                attroff(COLOR_PAIR(6));
            }
            else
            {
                attron(COLOR_PAIR(8));
                mvprintw(y, x,"X");
                attroff(COLOR_PAIR(8));
            }
            if(vprop(tgt) -> own != 0)
            {
                char tmp;
                if(vprop(tgt) -> build == 1)
                {
                    tmp = 'V';
                }
                else
                {
                    tmp = 'C';
                }
                switch(vprop(tgt) -> own)
                {
                    case 1:
                        attron(COLOR_PAIR(21));
                        mvprintw(y, x,"%c",tmp);
                        attroff(COLOR_PAIR(21));
                        mvprintw(50, 168, " ");
                        break;
                    case 2:
                        attron(COLOR_PAIR(22));
                        mvprintw(y, x,"%c",tmp);
                        attroff(COLOR_PAIR(22));
                        mvprintw(50, 168, " ");
                        break;
                    case 3:
                        attron(COLOR_PAIR(23));
                        mvprintw(y, x,"%c",tmp);
                        attroff(COLOR_PAIR(23));
                        mvprintw(50, 168, " ");
                        break;
                    case 4:
                        attron(COLOR_PAIR(24));
                        mvprintw(y, x,"%c",tmp);
                        attroff(COLOR_PAIR(24));
                        mvprintw(50, 168, " ");
                        break;
                }
            }
            break;
        case v_side:
            if(tgt -> highlighted)
            {
                attron(COLOR_PAIR(6));
                mvprintw(y, x,"\\");
                mvprintw(y-1, x-1,"\\");
                mvprintw(y+1, x+1,"\\");
                attroff(COLOR_PAIR(6));
            }
            else
            {
                attron(COLOR_PAIR(8));
                mvprintw(y, x,"\\");
                mvprintw(y-1, x-1,"\\");
                mvprintw(y+1, x+1,"\\");
                attroff(COLOR_PAIR(8));
            }
            if(sprop(tgt) -> own != 0)
            {
                switch(sprop(tgt) -> own)
                {
                    case 1:
                        attron(COLOR_PAIR(21));
                        mvprintw(y, x,"\\");
                        mvprintw(y-1, x-1,"\\");
                        mvprintw(y+1, x+1,"\\");
                        attroff(COLOR_PAIR(21));
                        mvprintw(50, 168, " ");
                        break;
                    case 2:
                        attron(COLOR_PAIR(22));
                        mvprintw(y, x,"\\");
                        mvprintw(y-1, x-1,"\\");
                        mvprintw(y+1, x+1,"\\");
                        attroff(COLOR_PAIR(22));
                        mvprintw(50, 168, " ");
                        break;
                    case 3:
                        attron(COLOR_PAIR(23));
                        mvprintw(y, x,"\\");
                        mvprintw(y-1, x-1,"\\");
                        mvprintw(y+1, x+1,"\\");
                        attroff(COLOR_PAIR(23));
                        mvprintw(50, 168, " ");
                        break;
                    case 4:
                        attron(COLOR_PAIR(24));
                        mvprintw(y, x,"\\");
                        mvprintw(y-1, x-1,"\\");
                        mvprintw(y+1, x+1,"\\");
                        attroff(COLOR_PAIR(24));
                        mvprintw(50, 168, " ");
                        break;
                }
            }
            break;
        case main_side:
            if(tgt -> highlighted)
            {
                attron(COLOR_PAIR(6));
                mvprintw(y, x-3,"-------");
                attroff(COLOR_PAIR(6));
            }
            else
            {
                attron(COLOR_PAIR(8));
                mvprintw(y, x-3,"-------");
                attroff(COLOR_PAIR(8));
            }
            if(sprop(tgt) -> own != 0)
            {
                switch(sprop(tgt) -> own)
                {
                    case 1:
                        attron(COLOR_PAIR(21));
                        mvprintw(y, x-3,"-------");
                        attroff(COLOR_PAIR(21));
                        mvprintw(50, 168, " ");
                        break;
                    case 2:
                        attron(COLOR_PAIR(22));
                        mvprintw(y, x-3,"-------");
                        attroff(COLOR_PAIR(22));
                        mvprintw(50, 168, " ");
                        break;
                    case 3:
                        attron(COLOR_PAIR(23));
                        mvprintw(y, x-3,"-------");
                        attroff(COLOR_PAIR(23));
                        mvprintw(50, 168, " ");
                        break;
                    case 4:
                        attron(COLOR_PAIR(24));
                        mvprintw(y, x-3,"-------");
                        attroff(COLOR_PAIR(24));
                        mvprintw(50, 168, " ");
                        break;
                }
            }
            break;
        case minor_side:
            if(tgt -> highlighted)
            {
                attron(COLOR_PAIR(6));
                mvprintw(y, x,"/");
                mvprintw(y-1, x+1,"/");
                mvprintw(y+1, x-1,"/");
                attroff(COLOR_PAIR(6));
            }
            else
            {
                attron(COLOR_PAIR(8));
                mvprintw(y, x,"/");
                mvprintw(y-1, x+1,"/");
                mvprintw(y+1, x-1,"/");
                attroff(COLOR_PAIR(8));
            }
            if(sprop(tgt) -> own != 0)
            {
                switch(sprop(tgt) -> own)
                {
                    case 1:
                        attron(COLOR_PAIR(21));
                        mvprintw(y, x,"/");
                        mvprintw(y-1, x+1,"/");
                        mvprintw(y+1, x-1,"/");
                        attroff(COLOR_PAIR(21));
                        mvprintw(50, 168, " ");
                        break;
                    case 2:
                        attron(COLOR_PAIR(22));
                        mvprintw(y, x,"/");
                        mvprintw(y-1, x+1,"/");
                        mvprintw(y+1, x-1,"/");
                        attroff(COLOR_PAIR(22));
                        mvprintw(50, 168, " ");
                        break;
                    case 3:
                        attron(COLOR_PAIR(23));
                        mvprintw(y, x,"/");
                        mvprintw(y-1, x+1,"/");
                        mvprintw(y+1, x-1,"/");
                        attroff(COLOR_PAIR(23));
                        mvprintw(50, 168, " ");
                        break;
                    case 4:
                        attron(COLOR_PAIR(24));
                        mvprintw(y, x,"/");
                        mvprintw(y-1, x+1,"/");
                        mvprintw(y+1, x-1,"/");
                        attroff(COLOR_PAIR(24));
                        mvprintw(50, 168, " ");
                        break;
                }
            }
            //To be complete
            //wood: complete, i think
            mvprintw(50, 168, " ");
            break;
    }
    set_background_color_init();
}
void show_all_objects()
{
    for(i32 i=0;i<13;i++)
    {
        for(i32 j=0;j<13;j++)
        {
            for(i32 k=0;k<3;k++)
            {
                show_obj(innerbox[i][j][k]);
            }
        }
    }
}
// int main()
// {
//     shuffle_i32(resources_cards,19);
//     srand(time(NULL));
//     box_set();
//     initscr();          // Initialize the window
//     start_color();
//     MEVENT event;
//     getmaxyx(stdscr, off_y, off_x);
//     off_y /= 2;
//     off_x /= 2;
//     mvprintw(10,0,"off_x:%d,off_y:%d",off_x,off_y);
//     noecho();
//     cbreak();

//     keypad(stdscr, TRUE);
//     mousemask(ALL_MOUSE_EVENTS, NULL);
//     show_all_objects();
//     while (1)
//     {
//         int c = getch();
//         // If a mouse event was detected
//         if (c == KEY_MOUSE)
//         {
//             if (getmouse(&event) == OK)
//             { // Get the details of the mouse event
//                 if (event.bstate & BUTTON1_CLICKED)
//                 { // If left button clicked
//                     mvprintw(0, 0, "                           ");
//                     mvprintw(0, 0, "x=%d,y=%d", event.x - off_x, off_y - event.y);
//                     mvprintw(1, 0, "                           ");
//                     obj *tgt = get_obj_from_mouse(event.x, event.y);
//                     if (tgt != NULL)
//                     {
//                         mvprintw(1, 0, "%s\t%d,%d,%d", attr_names[tgt->attr], tgt->locs[0], tgt->locs[1], tgt->locs[2]);
//                         show_obj(tgt);
//                     }
//                 }
//             }
//         }
//         // Refresh the screen
//         refresh();
//     }
//     refresh();          // Refresh the screen to match what's in memory
//     getch();            // Wait for user input
//     endwin();           // Close the window

//     return 0;
// }
/*void printhex(i32 y, i32 x, i32 dice_num, const char *resource_name)
{
    //const char *resource="";
    mvprintw(off_y + y - 4, off_x + x + 2 - 6, "X-------X");
    mvprintw(off_y + y - 3, off_x + x + 1 - 6, "/         \\");
    mvprintw(off_y + y - 2, off_x + x + 0 - 6, "/    %2d     \\", dice_num);
    mvprintw(off_y + y - 1, off_x + x - 1 - 6, "/             \\");
    mvprintw(off_y + y + 0, off_x + x - 2 - 6, "X  %8s     X", resource_name);
    mvprintw(off_y + y + 1, off_x + x - 1 - 6, "\\             /");
    mvprintw(off_y + y + 2, off_x + x + 0 - 6, "\\           /");
    mvprintw(off_y + y + 3, off_x + x + 1 - 6, "\\         /");
    mvprintw(off_y + y + 4, off_x + x + 2 - 6, "X-------X");
}*/
/*int32_t board_locs[19][2] =
        {
                {0,   0},
                {-8,  0},
                {-4,  12},
                {4,   12},
                {8,   0},
                {4,   -12},
                {-4,  -12},
                {-12, -12},
                {-16, 0},
                {-12, 12},
                {-8,  24},
                {0,   24},
                {8,   24},
                {12,  12},
                {16,  0},
                {12,  -12},
                {8,   -24},
                {0,   -24},
                {-8,  -24}
        };
i32 j=0;

FORI(0, 19)
{

    const char* res_name=resources_name[resources_cards[i]];
    if(strcmp(res_name,"DESERT")==0)
    {
        printhex(board_locs[i][0], board_locs[i][1], 88, "DESERT");
    }
    else
    {
        printhex(board_locs[i][0], board_locs[i][1], dice_nums[j], res_name);
        j++;
    }
}*/
