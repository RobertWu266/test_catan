//
// Created by wu on 5/24/23.
//

#include "TUI.h"
#include "catan_ui.h"

extern char *attr_names[];
extern obj *innerbox[13][13][3];
static i32 off_x=47;
static i32 off_y=25;
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
            //mvprintw(y, x,"X");
            mvprintw(y, x,"X%d",bprop(tgt)->num);
            break;
        case neg_vert:
        case pos_vert:
            attron(COLOR_PAIR(8));
            mvprintw(y, x,"X");
            attroff(COLOR_PAIR(8));
            if(vprop(tgt) -> own != 0)
            {
                switch(vprop(tgt) -> own)
                {
                    case 1:
                        attron(COLOR_PAIR(21));
                        mvprintw(y, x,"X");
                        attroff(COLOR_PAIR(21));
                        break;
                    case 2:
                        attron(COLOR_PAIR(22));
                        mvprintw(y, x,"X");
                        attroff(COLOR_PAIR(22));
                        break;
                    case 3:
                        attron(COLOR_PAIR(23));
                        mvprintw(y, x,"X");
                        attroff(COLOR_PAIR(23));
                        break;
                    case 4:
                        attron(COLOR_PAIR(24));
                        mvprintw(y, x,"X");
                        attroff(COLOR_PAIR(24));
                        break;
                }
            }
            break;
        case v_side:
            attron(COLOR_PAIR(8));
            mvprintw(y, x,"\\");
            mvprintw(y-1, x-1,"\\");
            mvprintw(y+1, x+1,"\\");
            attroff(COLOR_PAIR(8));
            if(vprop(tgt) -> own != 0)
            {
                switch(vprop(tgt) -> own)
                {
                    case 1:
                        attron(COLOR_PAIR(21));
                        mvprintw(y, x,"\\");
                        mvprintw(y-1, x-1,"\\");
                        mvprintw(y+1, x+1,"\\");
                        attroff(COLOR_PAIR(21));
                        break;
                    case 2:
                        attron(COLOR_PAIR(22));
                        mvprintw(y, x,"\\");
                        mvprintw(y-1, x-1,"\\");
                        mvprintw(y+1, x+1,"\\");
                        attroff(COLOR_PAIR(22));
                        break;
                    case 3:
                        attron(COLOR_PAIR(23));
                        mvprintw(y, x,"\\");
                        mvprintw(y-1, x-1,"\\");
                        mvprintw(y+1, x+1,"\\");
                        attroff(COLOR_PAIR(23));
                        break;
                    case 4:
                        attron(COLOR_PAIR(24));
                        mvprintw(y, x,"\\");
                        mvprintw(y-1, x-1,"\\");
                        mvprintw(y+1, x+1,"\\");
                        attroff(COLOR_PAIR(24));
                        break;
                }
            }
            break;
        case main_side:
            attron(COLOR_PAIR(8));
            mvprintw(y, x-3,"--------");
            attroff(COLOR_PAIR(8));
            if(vprop(tgt) -> own != 0)
            {
                switch(vprop(tgt) -> own)
                {
                    case 1:
                        attron(COLOR_PAIR(21));
                        mvprintw(y, x-3,"-------");
                        attroff(COLOR_PAIR(1));
                        break;
                    case 2:
                        attron(COLOR_PAIR(22));
                        mvprintw(y, x-3,"-------");
                        attroff(COLOR_PAIR(22));
                        break;
                    case 3:
                        attron(COLOR_PAIR(23));
                        mvprintw(y, x-3,"-------");
                        attroff(COLOR_PAIR(23));
                        break;
                    case 4:
                        attron(COLOR_PAIR(24));
                        mvprintw(y, x-3,"-------");
                        attroff(COLOR_PAIR(24));
                        break;
                }
            }
            break;
        case minor_side:
            attron(COLOR_PAIR(8));
            mvprintw(y, x,"/");
            mvprintw(y-1, x+1,"/");
            mvprintw(y+1, x-1,"/");
            attroff(COLOR_PAIR(8));
            if(vprop(tgt) -> own != 0)
            {
                switch(vprop(tgt) -> own)
                {
                    case 1:
                        attron(COLOR_PAIR(1));
                        mvprintw(y, x,"/");
                        mvprintw(y-1, x+1,"/");
                        mvprintw(y+1, x-1,"/");
                        attroff(COLOR_PAIR(1));
                        break;
                    case 2:
                        attron(COLOR_PAIR(2));
                        mvprintw(y, x,"/");
                        mvprintw(y-1, x+1,"/");
                        mvprintw(y+1, x-1,"/");
                        attroff(COLOR_PAIR(2));
                        break;
                    case 3:
                        attron(COLOR_PAIR(3));
                        mvprintw(y, x,"/");
                        mvprintw(y-1, x+1,"/");
                        mvprintw(y+1, x-1,"/");
                        attroff(COLOR_PAIR(3));
                        break;
                    case 4:
                        attron(COLOR_PAIR(4));
                        mvprintw(y, x,"/");
                        mvprintw(y-1, x+1,"/");
                        mvprintw(y+1, x-1,"/");
                        attroff(COLOR_PAIR(4));
                        break;
                }
            }
            //To be complete
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
