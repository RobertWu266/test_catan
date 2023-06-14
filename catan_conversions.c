//
// Created by wu on 5/18/23.
//

#include "catan_conversions.h"


obj *innerbox[13][13][3] = {0};
char *attr_names[] = {"body", "v_side", "main_side", "minor_side", "pos_vert", "neg_vert", "invalid"};
i32 resources_id[] = {HILL, HILL, HILL, MOUNTAIN, MOUNTAIN, MOUNTAIN, FIELD, FIELD, FIELD, FIELD, PASTURE, PASTURE,
                      PASTURE, PASTURE, FOREST, FOREST, FOREST, FOREST, DESERT};
//const char* resource_name[]={"HILL", "HILL", "HILL", "MOUNTAIN", "MOUNTAIN", "MOUNTAIN", "FIELD", "FIELD", "FIELD", "FIELD", "PASTURE", "PASTURE","PASTURE", "PASTURE", "FOREST", "FOREST", "FOREST", "FOREST", "DESERT"};
i32 resources_id_csr = 0;
obj* body_list[19]={0};
obj* vertice_list[54]={0};
obj* side_list[72]={0};
i32 side_list_csr=0;
i32 vertice_list_csr=0;
i32 dice_nums[18]={11,3,6,5,4,9,10,8,4,11,12,9,10,8,3,6,2,5};

player_property players[4]={0};
bank_property bank={0};

i32 abs(i32 x)
{
    if (x < 0)return -1 * x;
    return x;
}

harbor locs_harbor(i32 *locs)
{
    i32 x, y, z = 0;
    x = locs[0];
    y = locs[1];
    z = locs[2];
    if
            (x == -5 && y == -1 && z == 5 || x == -5 && y == 1 && z == 5 ||
             x == -5 && y == 3 && z == 3 || x == -5 && y == 3 && z == 1 ||
             x == -1 && y == 5 && z == -5 || x == 1 && y == 5 && z == -5 ||
             x == 5 && y == -5 && z == 1 || x == 5 && y == -5 && z == -1)
        return ALL_HARBOR;

    if (x == -3 && y == -3 && z == 5 || x == -1 && y == -3 && z == 5)return WHEAT_HARBOR;
    if (x == 1 && y == -5 && z == 3 || x == 3 && y == -5 && z == 3)return SHEEP_HARBOR;
    if (x == 5 && y == -3 && z == -3 || x == 5 && y == -1 && z == -3)return WOOD_HARBOR;
    if (x == 3 && y == 1 && z == -5 || x == 3 && y == 3 && z == -5)return STONE_HARBOR;
    if (x == -3 && y == 5 && z == -1 || x == -3 && y == 5 && z == -3)return BRICK_HARBOR;
    return Nil;
}

i32 customfindsum(i32 x, i32 y)
{
    if ((x - 0) % 2 == 0 && (y - 0) % 6 == 0 && ((x - 0) / 2 + (y - 0) / 6) % 2 == 0)return 0;//body(0,0)
    if ((x - 2) % 2 == 0 && (y - 0) % 6 == 0 && ((x - 2) / 2 + (y - 0) / 6) % 2 == 0)return 0;//v_side(2,0)
    if ((x - 1) % 2 == 0 && (y - 3) % 6 == 0 && ((x - 1) / 2 + (y - 3) / 6) % 2 == 0)return 0;//main_side(1,3)
    if ((x + 1) % 2 == 0 && (y - 3) % 6 == 0 && ((x + 1) / 2 + (y - 3) / 6) % 2 == 0)return 0;//minnor_side(-1,3)
    if ((x - 2) % 2 == 0 && (y - 2) % 6 == 0 && ((x - 2) / 2 + (y - 2) / 6) % 2 == 0)return 1;//pos_vert(2,2)
    if ((x - 0) % 2 == 0 && (y - 4) % 6 == 0 && ((x - 0) / 2 + (y - 4) / 6) % 2 == 0)return -1;//neg_vert(0,4)
    return 42;
}

i32 *custom2cube(i32 x, i32 y)
{
    i32 sum = customfindsum(x, y);
    if (!(sum == 1 || sum == 0 || sum == -1))return NULL;
    i32 *pret = calloc(3, sizeof(i32));
    pret[0] = x;
    pret[2] = (y + x) / 2;
    i32 offset = (pret[0] + pret[2] - sum) / 3;
    pret[0] -= offset;
    pret[1] -= offset;
    pret[2] -= offset;
    return pret;
}

/*i32 *obj_custom(obj *tgt)
{
    i32 *pret= calloc(2,sizeof(i32));
    pret[0]=(tgt->locs[0]- tgt->locs[1]);
    pret[1]=(-tgt->locs[0]- tgt->locs[1]+2*tgt->locs[2]);
    return pret;
}*/
bool isvalid(i32 *locs)//check if the locs is out of bound
{
    if (locs == NULL)return false;
    i32 sum = locs[0] + locs[1] + locs[2];
    if (!(sum == 1 || sum == 0 || sum == -1))return false;
    if (sum != 0 && (abs(locs[0] + locs[1]) > 6 || abs(locs[0]) > 6 || abs(locs[1]) > 6 || abs(locs[2]) > 6))
        return false;
    if (sum == 0 && (abs(locs[0] + locs[1]) > 5 || abs(locs[0]) > 5 || abs(locs[1]) > 5 || abs(locs[2]) > 5))
        return false;
    return true;
}

attribute get_attr(i32 *locs)
{
    if (!isvalid(locs))return invalid;
    if (locs[0] % 2 == 0 && locs[1] % 2 == 0 && locs[2] % 2 == 0)return body;
    if (locs[0] % 2 != 0 && locs[1] % 2 != 0 && locs[2] % 2 == 0)return v_side;
    if (locs[0] % 2 == 0 && locs[1] % 2 != 0 && locs[2] % 2 != 0)return main_side;
    if (locs[0] % 2 != 0 && locs[1] % 2 == 0 && locs[2] % 2 != 0)return minor_side;
    if (locs[0] % 2 != 0 && locs[1] % 2 != 0 && locs[2] % 2 != 0)
    {
        if (locs[0] + locs[1] + locs[2] == 1)return pos_vert;
        return neg_vert;
    }
    return invalid;
}

obj *find_obj(i32 x, i32 y, i32 z)
{
    i32 *locs = calloc(3, sizeof(i32));
    locs[0] = x;
    locs[1] = y;
    locs[2] = z;
    obj *pret = locs_find_obj(locs);
    free(locs);
    return pret;
}

obj *locs_find_obj(i32 *locs)
{
    if (!isvalid(locs))return NULL;
    return innerbox[locs[0] + 6][locs[1] + 6][locs[0] + locs[1] + locs[2] + 1];
}

i32 *render(i32 x, i32 y, i32 z)
{
    i32 *pret = calloc(3, sizeof(i32));
    pret[0] = x;
    pret[1] = y;
    pret[2] = z;
    return pret;
}

obj *init_obj(i32 *locs)
{
    attribute attr = get_attr(locs);
    if (attr == invalid)return NULL;
    obj *pret = calloc(1, sizeof(obj));
    pret->attr = attr;
    pret->locs = locs;
    pret->prop = NULL;
    return pret;
}

void set_neighbor(obj *tgt)
{
    if (tgt == NULL)return;
    switch (tgt->attr)
    {
        case body:
            tgt->prop = calloc(1, sizeof(body_property));
            bprop(tgt)->flag = BODY_PROPERTY;//Its useless
            bprop(tgt)->nei_vert = body_neighbor_vertice(tgt);//robber's case
            bprop(tgt)->resource = resources_id[resources_id_csr];
            resources_id_csr++;
            break;
        case neg_vert:
        case pos_vert:
            tgt->prop = calloc(1, sizeof(vertice_property));
            vprop(tgt)->flag = VERTICE_PROPERTY;//Its useless
            vprop(tgt)->nei_body = vertice_neighbor_body(tgt);//for getting neighboring resource (opening)
            vprop(tgt)->nei_vert = vertice_neighbor_vertice(tgt);//for checking no neighboring building
            vprop(tgt)->harb= locs_harbor(tgt->locs);
            vertice_list[vertice_list_csr]=tgt;
            vertice_list_csr++;
            break;
        case v_side:
        case main_side:
        case minor_side:
            tgt->prop = calloc(1, sizeof(side_property));
            sprop(tgt)->flag = SIDE_PROPERTY;//Its useless
            sprop(tgt)->nei_vert = side_neighbor_vertice(tgt);
            sprop(tgt)->nei_side = side_neighbor_side(tgt);//for longest road;
            side_list[side_list_csr]=tgt;
            side_list_csr++;
            break;
    }
}

void dtor_obj(obj *tgt)
{
    if (tgt == NULL)return;
    free(tgt->locs);
    switch (tgt->attr)
    {
        case body:
            free(((body_property *) tgt->prop)->nei_vert);//robber's case
            free(tgt->prop);
            break;
        case neg_vert:
        case pos_vert:
            tgt->prop = calloc(1, sizeof(vertice_property));
            free(((vertice_property *) tgt->prop)->nei_body);//for getting neighboring resource (opening)
            free(((vertice_property *) tgt->prop)->nei_vert);//for checking no neighboring building
            free(tgt->prop);
            break;
        case v_side:
        case main_side:
        case minor_side:
            free(((side_property *) tgt->prop)->nei_vert);
            free(((side_property *) tgt->prop)->nei_side);//for longest road;
            free(tgt->prop);
            break;
    }
    free(tgt);
    return;
}

void print_obj(obj *tgt)
{
    if (tgt == NULL)return;
    printf("%s\n%d,%d,%d\n", attr_names[tgt->attr], tgt->locs[0], tgt->locs[1], tgt->locs[2]);
}

void print_obj_all(obj *tgt)
{
    if (tgt == NULL)return;
    printf("%s\n%d,%d,%d\n", attr_names[tgt->attr], tgt->locs[0], tgt->locs[1], tgt->locs[2]);
    obj **neighbor = NULL;
    switch (tgt->attr)
    {
        case body:
            neighbor = bprop(tgt)->nei_vert;
            printf("Neighboring vertices:\n\n");
            for (i32 i = 0; i < 6; i++)
            {
                print_obj(neighbor[i]);
            }
            break;
        case neg_vert:
        case pos_vert:
            neighbor = vprop(tgt)->nei_vert;
            printf("Neighboring vertices:\n\n");
            for (i32 i = 0; i < 3; i++)
            {
                print_obj(neighbor[i]);
            }
            neighbor = vprop(tgt)->nei_body;
            printf("Neighboring Bodies:\n\n");
            for (i32 i = 0; i < 3; i++)
            {
                print_obj(neighbor[i]);
            }
            break;
        case v_side:
        case main_side:
        case minor_side:
            neighbor = sprop(tgt)->nei_vert;
            printf("Neighboring vertices:\n\n");
            for (i32 i = 0; i < 2; i++)
            {
                print_obj(neighbor[i]);
            }
            neighbor = sprop(tgt)->nei_side;
            printf("Neighboring sides:\n\n");
            for (i32 i = 0; i < 4; i++)
            {
                print_obj(neighbor[i]);
            }
            break;
    }
}

obj **body_neighbor_vertice(obj *tgt)
{
    obj **pret = calloc(6, sizeof(obj *));
    for (uint8_t i = 1; i < 7; i++)
    {
        pret[i - 1] = find_obj(tgt->locs[0] + 1 - 2 * ((i) & 1), tgt->locs[1] + 1 - 2 * ((i >> 1) & 1),
                               tgt->locs[2] + 1 - 2 * ((i >> 2) & 1));
    }
    return pret;
}

obj **side_neighbor_vertice(obj *tgt)
{
    obj **pret = NULL;
    obj *toput = NULL;
    switch (tgt->attr)
    {
        case minor_side:
            pret = calloc(2, sizeof(obj *));
            toput = find_obj(tgt->locs[0], tgt->locs[1] + 1, tgt->locs[2]);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1] - 1, tgt->locs[2]);
            if (toput != NULL)pret[1] = toput;
            return pret;

        case main_side:
            pret = calloc(2, sizeof(obj *));
            toput = find_obj(tgt->locs[0] + 1, tgt->locs[1], tgt->locs[2]);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0] - 1, tgt->locs[1], tgt->locs[2]);
            if (toput != NULL)pret[1] = toput;
            return pret;
        case v_side:
            pret = calloc(2, sizeof(obj *));
            toput = find_obj(tgt->locs[0], tgt->locs[1], tgt->locs[2] + 1);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1], tgt->locs[2] - 1);
            if (toput != NULL)pret[1] = toput;
            return pret;
        default:
            return NULL;
    }

}

obj **vertice_neighbor_vertice(obj *tgt)
{
    obj **pret = NULL;
    obj *toput = NULL;
    switch (tgt->attr)
    {
        case pos_vert:
            pret = calloc(3, sizeof(obj *));
            toput = find_obj(tgt->locs[0] - 2, tgt->locs[1], tgt->locs[2]);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1] - 2, tgt->locs[2]);
            if (toput != NULL)pret[1] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1], tgt->locs[2] - 2);
            if (toput != NULL)pret[2] = toput;
            return pret;

        case neg_vert:
            pret = calloc(3, sizeof(obj *));
            toput = find_obj(tgt->locs[0] + 2, tgt->locs[1], tgt->locs[2]);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1] + 2, tgt->locs[2]);
            if (toput != NULL)pret[1] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1], tgt->locs[2] + 2);
            if (toput != NULL)pret[2] = toput;
            return pret;
        default:
            return NULL;
    }

}

obj **vertice_neighbor_side(obj *tgt)
{
    obj **pret = NULL;
    obj *toput = NULL;
    switch (tgt->attr)
    {
        case pos_vert:
            pret = calloc(3, sizeof(obj *));
            toput = find_obj(tgt->locs[0] - 1, tgt->locs[1], tgt->locs[2]);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1] - 1, tgt->locs[2]);
            if (toput != NULL)pret[1] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1], tgt->locs[2] - 1);
            if (toput != NULL)pret[2] = toput;
            return pret;

        case neg_vert:
            pret = calloc(3, sizeof(obj *));
            toput = find_obj(tgt->locs[0] + 1, tgt->locs[1], tgt->locs[2]);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1] + 1, tgt->locs[2]);
            if (toput != NULL)pret[1] = toput;
            toput = find_obj(tgt->locs[0], tgt->locs[1], tgt->locs[2] + 1);
            if (toput != NULL)pret[2] = toput;
            return pret;
        default:
            return NULL;
    }
}

obj **vertice_neighbor_body(obj *tgt)
{
    obj **pret = NULL;
    obj *toput = NULL;
    switch (tgt->attr)
    {
        case pos_vert:
            pret = calloc(3, sizeof(obj *));
            toput = find_obj(tgt->locs[0] + 1, tgt->locs[1] - 1, tgt->locs[2] - 1);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0] - 1, tgt->locs[1] + 1, tgt->locs[2] - 1);
            if (toput != NULL)pret[1] = toput;
            toput = find_obj(tgt->locs[0] - 1, tgt->locs[1] - 1, tgt->locs[2] + 1);
            if (toput != NULL)pret[2] = toput;
            return pret;

        case neg_vert:
            pret = calloc(3, sizeof(obj *));
            toput = find_obj(tgt->locs[0] - 1, tgt->locs[1] + 1, tgt->locs[2] + 1);
            if (toput != NULL)pret[0] = toput;
            toput = find_obj(tgt->locs[0] + 1, tgt->locs[1] - 1, tgt->locs[2] + 1);
            if (toput != NULL)pret[1] = toput;
            toput = find_obj(tgt->locs[0] + 1, tgt->locs[1] + 1, tgt->locs[2] - 1);
            if (toput != NULL)pret[2] = toput;
            return pret;
        default:
            return NULL;
    }
}

obj **side_neighbor_side(obj *tgt)
{
    if (!(tgt->attr == v_side || tgt->attr == main_side || tgt->attr == minor_side))return NULL;
    obj **nei_vert = side_neighbor_vertice(tgt);
    obj **pret = calloc(4, sizeof(obj *));
    obj *toput = NULL;
    i32 csr = 0;
    for (i32 i = 0; i < 2; i++)
    {
        obj **vert_nei = vertice_neighbor_side(nei_vert[i]);

        for (i32 j = 0; j < 3; j++)
        {
            toput = vert_nei[j];

            if (toput == NULL || toput->attr == tgt->attr)continue;
            pret[csr] = toput;
            csr++;
        }
        free(vert_nei);
    }
    free(nei_vert);
    return pret;

}

/*obj** vertice_neighbor_body(obj* tgt)
{
    obj **pret=NULL;
    obj *toput=NULL;
    switch (tgt->attr)
    {
        case pos_vert:
            pret= calloc(3, sizeof(obj*));
            toput=find_obj(tgt->locs[0]+1,tgt->locs[1]-1,tgt->locs[2]-1);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0]-1,tgt->locs[1]+1,tgt->locs[2]-1);
            if(toput!=NULL)pret[1]=toput;
            toput=find_obj(tgt->locs[0]-1,tgt->locs[1]-1,tgt->locs[2]+1);
            if(toput!=NULL)pret[2]=toput;
            return pret;

        case neg_vert:
            pret= calloc(3, sizeof(obj*));
            toput=find_obj(tgt->locs[0]-1,tgt->locs[1]+1,tgt->locs[2]+1);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0]+1,tgt->locs[1]-1,tgt->locs[2]+1);
            if(toput!=NULL)pret[1]=toput;
            toput=find_obj(tgt->locs[0]+1,tgt->locs[1]+1,tgt->locs[2]-1);
            if(toput!=NULL)pret[2]=toput;
            return pret;
        default:
            return NULL;
    }
}*/
obj* body_astep_foward(obj* tgt,i32 dir)
{
    if(!tgt)assert(0);
    switch (dir%6)
    {
        case 0:
            return find_obj(tgt->locs[0]+2,tgt->locs[1]-2,tgt->locs[2]);
        case 1:
            return find_obj(tgt->locs[0]+2,tgt->locs[1],tgt->locs[2]-2);
        case 2:
            return find_obj(tgt->locs[0],tgt->locs[1]+2,tgt->locs[2]-2);
        case 3:
            return find_obj(tgt->locs[0]-2,tgt->locs[1]+2,tgt->locs[2]);
        case 4:
            return find_obj(tgt->locs[0]-2,tgt->locs[1],tgt->locs[2]+2);
        case 5:
            return find_obj(tgt->locs[0],tgt->locs[1]-2,tgt->locs[2]+2);
    }
}
void fill_body_list()
{
    i32 dir=rand()%6;
    body_list[0]= find_obj(0,0,0);
    body_list[1]= body_astep_foward(body_list[0],dir);
    dir+=2;
    for(i32 i=0;i<5;i++)
    {
        body_list[i+2]= body_astep_foward(body_list[i+1],dir);
        dir++;
    }
    dir--;
    for(i32 i=0;i<6;i++)
    {
        body_list[2*i+7]= body_astep_foward(body_list[2*i+6],dir);
        if(i==0)dir++;
        body_list[2*i+8]= body_astep_foward(body_list[2*i+7],dir);
        dir++;
    }
}
void bank_init(bank_property *bank)
{
    bank -> wood = 19;
    bank -> stone = 19;
    bank -> brick = 19;
    bank -> sheep = 19;
    bank -> wheat = 19;
    bank -> knights=14;
    bank->monopoly=2;
    bank->year_of_plenty=2;
    bank->road_building=2;
    bank->victory_card=6;
}

void player_init(player_property *player)
{
    player -> total_resource_cards = 0;
    player -> wood = 0;
    player -> stone = 0;
    player -> brick = 0;
    player -> sheep = 0;;
    player -> wheat = 0;;
    player -> special_cards = 0;
    player -> knights = 0;
    player -> year_of_plenty = 0;
    player -> road_building = 0;
    player -> monopoly = 0;
    player -> victory_card = 0;
    player -> total_victory_points = 0;
    player -> max_roads = 0;
    player -> village_remain = 4;
    player -> city_remain = 5;
    player -> road_remain = 15;
    player->wood_exchange_rate=4;
    player->brick_exchange_rate=4;
    player->sheep_exchange_rate=4;
    player->stone_exchange_rate=4;
    player->wheat_exchange_rate=4;

}

void trade_init( int trade[] )
{
	for( int i = 0; i < 10; i++ )
	{
		trade[i] = 0;
	}
}

void build_road(owner owner1, obj* tobuild)
{
    player_property the_player=players[owner1-player1];
    the_player.road_remain--;
    if(tobuild->attr!=v_side&&tobuild->attr!=main_side&&tobuild->attr!=minor_side)assert(0);
    sprop(tobuild)->own=owner1;
    the_player.my_road[the_player.my_road_csr]=tobuild;
    the_player.my_road_csr++;

}
void build_village(owner owner1, obj* tobuild)
{
    player_property the_player=players[owner1-player1];
    the_player.road_remain--;
    if(tobuild->attr!=pos_vert&&tobuild->attr!=neg_vert)assert(0);
    vprop(tobuild)->own=owner1;
    if(vprop(tobuild)->harb==ALL_HARBOR)
    {
        if(the_player.wheat_exchange_rate>3)the_player.wheat_exchange_rate=3;
        if(the_player.wood_exchange_rate>3)the_player.wood_exchange_rate=3;
        if(the_player.stone_exchange_rate>3)the_player.stone_exchange_rate=3;
        if(the_player.brick_exchange_rate>3)the_player.brick_exchange_rate=3;
        if(the_player.sheep_exchange_rate>3)the_player.sheep_exchange_rate=3;
    }
    if(vprop(tobuild)->harb==SHEEP_HARBOR)the_player.sheep_exchange_rate=2;
    if(vprop(tobuild)->harb==STONE_HARBOR)the_player.stone_exchange_rate=2;
    if(vprop(tobuild)->harb==WHEAT_HARBOR)the_player.wheat_exchange_rate=2;
    if(vprop(tobuild)->harb==BRICK_HARBOR)the_player.brick_exchange_rate=2;
    if(vprop(tobuild)->harb==WOOD_HARBOR)the_player.wood_exchange_rate=2;
}

void box_set()
{
    for(i32 i=0;i<4;i++)player_init(players+i);
    bank_init(&bank);
    int trade[10] = {0};
    for (i32 i = 0; i < 13; i++)
    {
        for (i32 j = 0; j < 13; j++)
        {
            for (i32 k = 0; k < 3; k++)
            {
                i32 *locs = calloc(3, sizeof(i32));
                //index to cube
                locs[0] = i - 6;
                locs[1] = j - 6;
                locs[2] = k - (i - 6) - (j - 6) - 1;

                innerbox[i][j][k] = init_obj(locs);
                if (innerbox[i][j][k] == NULL)free(locs);
            }
        }
    }
    for (i32 i = 0; i < 13; i++)
    {
        for (i32 j = 0; j < 13; j++)
        {
            for (i32 k = 0; k < 3; k++)
            {
                set_neighbor(innerbox[i][j][k]);
            }
        }
    }
    fill_body_list();
    i32 csr=0;
    for(i32 i=0;i<19;i++)
    {
        if(bprop(body_list[i])->resource!=DESERT)
        {
            bprop(body_list[i])->num=dice_nums[csr];
            csr++;
        }
    }
}
void highlight_availible_village_beginning()
{
    for(i32 i=0;i<54;i++)
    {
        for(i32 j=0;j<3;j++)
        {
            obj *tgt=vprop(vertice_list[i])->nei_vert[j];
            if(NULL==tgt)continue;
            if(vprop(tgt)->own!=None)continue;
        }
        vertice_list[i]->highlighted=1;
    }
}void highlight_availible_village()
{

    for(i32 i=0;i<54;i++)
    {

        for(i32 j=0;j<3;j++)
        {
            obj *tgt=vprop(vertice_list[i])->nei_vert[j];
            if(NULL==tgt||vprop(tgt)->own!=None)continue;
        }
        vertice_list[i]->highlighted=1;
    }
}

void box_dtor()
{
    for (i32 i = 0; i < 13; i++)
    {
        for (i32 j = 0; j < 13; j++)
        {
            for (i32 k = 0; k < 3; k++)
            {
                dtor_obj(innerbox[i][j][k]);
            }
        }
    }
}

int trade( player_property *player, bank_property *bank, int trade[] )
{
	if( player -> wood < player -> wood_exchange_rate * trade[0] || 
	    player -> stone < player -> stone_exchange_rate * trade[1] ||
	    player -> brick < player -> brick_exchange_rate * trade[2] ||
	    player -> sheep < player -> sheep_exchange_rate * trade[3] ||
	    player -> wheat < player -> wheat_exchange_rate * trade[4] ||
	    bank -> wood < trade[5] ||
	    bank -> stone < trade[6] ||
	    bank -> brick < trade[7] ||
	    bank -> sheep < trade[8] ||
	    bank -> wheat < trade[9] )
	    	return -1;
	//player pay
	player -> wood -= player -> wood_exchange_rate * trade[0];
	bank -> wood += player -> wood_exchange_rate * trade[0];
	player -> stone -= player -> stone_exchange_rate * trade[1];
	bank -> stone += player -> stone_exchange_rate * trade[1];
	player -> brick -= player -> brick_exchange_rate * trade[2];
	bank -> brick += player -> brick_exchange_rate * trade[2];
	player -> sheep -= player -> sheep_exchange_rate * trade[3];
	bank -> sheep += player -> sheep_exchange_rate * trade[3];
	player -> wheat -= player -> wheat_exchange_rate * trade[4];
	bank -> wheat += player -> wheat_exchange_rate * trade[4];
	//player get
	player -> wood += trade[5];
	bank -> wood -= trade[5];
	player -> stone += trade[6];
	bank -> stone -= trade[6];
	player -> brick += trade[7];
	bank -> brick -= trade[7];
	player -> sheep += trade[8];
	bank -> sheep -= trade[8];
	player -> wheat += trade[9];
	bank -> wheat -= trade[9];
	return 0;
}


int specialcard_get( player_property *player, bank_property *bank )
{
	if( player -> stone < 1 || player -> sheep < 1 || player -> wheat < 1 )
		return -1;
	player -> stone--;
	player -> sheep--;
	player -> wheat--;
	int card = rand() % 5 + 1;
	int c = 0;
	while( c == 0 )
	{
		c = 1;
		switch( card )
		{
			case 1: 
				if( bank -> knights == 0 )
				{
					c = 0;
					card = rand() % 5 + 1;
				}
				break;
			case 2: 
				if( bank -> year_of_plenty == 0 )
				{
					c = 0;
					card = rand() % 5 + 1;
				}
				break;
			case 3: 
				if( bank -> road_building == 0 )
				{
					c = 0;
					card = rand() % 5 + 1;
				}
				break;
			case 4: 
				if( bank -> monopoly == 0 )
				{
					c = 0;
					card = rand() % 5 + 1;
				}
				break;
			case 5: 
				if( bank -> victory_card == 0 )
				{
					c = 0;
					card = rand() % 5 + 1;
				}
				break;
			default:
				break;
		}
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
			break;
	}
	return 0;
}

/*int main()
{
    box_set();
    i32 *tmplocs=NULL;
    obj* the_obj=find_obj(3,-5,2);
    //free(tmplocs);
    print_obj_all(the_obj);
    box_dtor();
}*/
/*obj** side_neighbor_body(obj* tgt)
{
    obj **pret=NULL;
    obj *toput=NULL;
    switch (tgt->attr)
    {
        case minor_side:
            pret= calloc(2, sizeof(obj*));
            toput=find_obj(tgt->locs[0]+1,tgt->locs[1],tgt->locs[2]-1);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0]-1,tgt->locs[1],tgt->locs[2]+1);
            if(toput!=NULL)pret[1]=toput;
            return pret;

        case main_side:
            pret= calloc(2, sizeof(obj*));
            toput=find_obj(tgt->locs[0],tgt->locs[1]+1,tgt->locs[2]-1);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1]-1,tgt->locs[2]+1);
            if(toput!=NULL)pret[1]=toput;
            return pret;
        case v_side:
            pret= calloc(2, sizeof(obj*));
            toput=find_obj(tgt->locs[0]-1,tgt->locs[1]+1,tgt->locs[2]);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0]+1,tgt->locs[1]-1,tgt->locs[2]);
            if(toput!=NULL)pret[1]=toput;
            return pret;
        default:
            return NULL;
    }

}*/
