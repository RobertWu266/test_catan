#include "catan_ui.h"
extern i32 resources_id[];
extern i32 winner;
extern i32 human_id;
int main()
{
    srand(time(NULL));
    shuffle_i32(resources_id,19);
    human_id=rand()%4;
    box_set();
	check_ui_size();
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();


    set_background_color_init();

    MEVENT event;
    mousemask(ALL_MOUSE_EVENTS, NULL);
    mouseinterval(0);
    mouse_trafo(&event.y,&event.x, FALSE);

    if(start_screen() == -1)
    {
        endwin();
        return 0;

    }

    in_game_ui_2(&event);//actuall middle point:(25,47)
    /*card_temp cardtemp={0};
    i32 trade_withbank[10]={0};

    draw_with_mouse_and_return_value(event,&cardtemp);//for debug*/



	endwin();
    printf("The winner is player %d!\n",winner);
	return 0;
}
