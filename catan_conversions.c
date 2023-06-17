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
i32 identity_list[4]={0};

player_property players[4]={0};
bank_property bank={0};
card_temp cardtemp = {0};
player_property *human_player={0};
player_property *road_AI_player={0};
player_property *develop_AI_player={0};
player_property *village_AI_player={0};
i32 human_id=1;

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
void shuffle_identity_list(i32 idx)
{
    for(i32 i=0; i<4;i++)identity_list[i]=i;
    for(i32 i=0;i<100;i++)
    {
        i32 j=rand()%4;
        i32 k=rand()%4;
        i32 tmp=identity_list[j];
        identity_list[j]=identity_list[k];
        identity_list[k]=tmp;
    }
    i32 human_loc=0;
    for(i32 i=0;i<4;i++)
    {
        if(identity_list[i]==0)
        {
            human_loc=i;
            break;
        }
    }
    identity_list[human_loc]=identity_list[idx];
    identity_list[idx]=0;
    //player_property **tmp_players={&human_player,&road_AI_player,&develop_AI_player,&village_AI_player};
    for(i32 i=0;i<4;i++)
    {
        switch (identity_list[i])
        {
            case 0:
                human_player=&players[i];
                break;
            case 1:
                road_AI_player=&players[i];
                break;
            case 2:
                develop_AI_player=&players[i];
                break;
            case 3:
                village_AI_player=&players[i];
                break;
        }
    }
}
/*function LongestRoad(player):
    longest_road = 0

    for each side in all sides:
        if side's owner is not the player:
            continue
        visited_sides = empty set
        current_road = DFS(side, visited_sides)
        if current_road > longest_road:
            longest_road = current_road

    return longest_road*/


/*function DFS(side, visited_sides):
    visited_sides.add(side)
    max_depth = 0
    for each neighbor_side in side's nei_side:
        if neighbor_side is NULL or neighbor_side in visited_sides or neighbor_side's owner is not side's owner:
            continue
        current_depth = DFS(neighbor_side, visited_sides)
        if current_depth > max_depth:
            max_depth = current_depth
    visited_sides.remove(side)

    return 1 + max_depth*/
uint8_t get_longest_road(owner owner1)
{
    uint8_t longest_road=0;
    player_property *the_player=&(players[owner1-player1]);
    if(the_player->my_road_csr==1)return 1;
    for(uint8_t i=0;i<the_player->my_road_csr;i++)
    {
        obj *visited_road[15]={0};
        uint8_t visited_road_csr=0;
        uint8_t current_road=DFS(owner1 ,the_player->my_road[i],visited_road,&visited_road_csr);
        if(current_road>longest_road)longest_road=current_road;
    }
    return longest_road;
}
bool in_list(obj *the_road, obj *visited_road[15],const uint8_t *p_visited_road_csr)
{
    for(uint8_t i=0;i<*p_visited_road_csr;i++)
    {
        if(visited_road[i]==the_road)return true;
    }
    return false;
}
uint8_t DFS(owner owner1,obj *the_road,obj *visited_road[15], uint8_t *p_visited_road_csr)
{
    visited_road[*p_visited_road_csr]=the_road;
    (*p_visited_road_csr)++;
    uint8_t max_depth=0;
    for(uint8_t i=0;i<4;i++)
    {
        obj *nei_road=sprop(the_road)->nei_side[i];
        if(nei_road==NULL || sprop(nei_road)->own!=owner1||in_list(nei_road,visited_road,p_visited_road_csr)) continue;
        uint8_t current_depth=DFS(owner1,nei_road,visited_road,p_visited_road_csr);
        if(current_depth>max_depth) max_depth=current_depth;
    }
    visited_road[*p_visited_road_csr]=NULL;
    (*p_visited_road_csr)--;
    return 1 + max_depth;
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
    bank->victory_card=5;
    bank -> special_cards = bank -> knights + bank -> monopoly + bank -> year_of_plenty + bank -> road_building + bank -> victory_card;
}

void player_init(player_property *player)
{
    player -> total_resource_cards = 100;
    player -> wood = 20;
    player -> stone = 20;
    player -> brick = 20;
    player -> sheep = 20;
    player -> wheat = 20;
    player -> special_cards = 0;
    player -> knights = 0;
    player -> knights_use = 0;
    //player -> knights_get = false;
    player -> year_of_plenty = 0;
    //player -> year_of_plenty_get = false;
    player -> road_building = 0;
    //player -> road_building_get = false;
    player -> monopoly = 0;
    //player -> monopoly_get = false;
    player -> victory_card = 0;
    //player -> victory_card_get = false;
    player -> total_victory_points = 0;
    player -> max_roads = 0;
    player -> village_remain = 5;
    player -> city_remain = 4;
    player -> road_remain = 15;
    player->wood_exchange_rate=4;
    player->brick_exchange_rate=4;
    player->sheep_exchange_rate=4;
    player->stone_exchange_rate=4;
    player->wheat_exchange_rate=4;
    player->iden=identity_list[player-players];
}
void card_temp_init( card_temp *cardtemp )
{
	cardtemp -> knights = 0;
	cardtemp -> year_of_plenty = 0;
	cardtemp -> road_building = 0;
	cardtemp -> monopoly = 0;
}

void trade_init( int trade_withbank[] )
{
	for( int i = 0; i < 10; i++ )
	{
		trade_withbank[i] = 0;
	}
}
void specialcard_init( int specialcard[] )
{
	for( int i = 0; i < 4; i++ )
	{
		specialcard[i] = 0;
	}
}
/*void player_card_get_init( player_property *player )
{
	player -> knights_get = false;
	player -> year_of_plenty_get = false;
	player -> road_building_get = false;
	player -> monopoly_get = false;
	player -> victory_card_get = false;
}*/

void build_road(owner owner1, obj* tobuild)
{
    player_property *the_player=&(players[owner1-player1]);
    if(the_player->road_remain)
    {
        the_player->road_remain--;
    }
    else
    {
        return;
    }
    if(tobuild->attr!=v_side&&tobuild->attr!=main_side&&tobuild->attr!=minor_side)assert(0);
    sprop(tobuild)->own=owner1;//can be reversed
    the_player->my_road[the_player->my_road_csr]=tobuild;//can be reversed
    the_player->my_road_csr++;//can be reversed
    the_player->max_roads= get_longest_road(owner1);
    clear_all_highlight();
}
void build_village(owner owner1, obj* tobuild)
{
    player_property *the_player=&(players[owner1-player1]);
    if (vprop(tobuild)->build==village)
    {
        if(the_player->city_remain)
        {
            vprop(tobuild)->build=city;
            the_player->city_remain--;
        }
        else
        {
            return;
        }
    }
    else if(vprop(tobuild)->build==empty)
    {
        if(the_player->village_remain)
        {
            vprop(tobuild)->build=village;
            the_player->village_remain--;
        }
        else
        {
            return;
        }
    }
    else
    {
        return;
    }
    if(tobuild->attr!=pos_vert&&tobuild->attr!=neg_vert)assert(0);
    vprop(tobuild)->own=owner1;

    if(vprop(tobuild)->harb==ALL_HARBOR)
    {
        if(the_player->wheat_exchange_rate>3)the_player->wheat_exchange_rate=3;
        if(the_player->wood_exchange_rate>3)the_player->wood_exchange_rate=3;
        if(the_player->stone_exchange_rate>3)the_player->stone_exchange_rate=3;
        if(the_player->brick_exchange_rate>3)the_player->brick_exchange_rate=3;
        if(the_player->sheep_exchange_rate>3)the_player->sheep_exchange_rate=3;
    }
    if(vprop(tobuild)->harb==SHEEP_HARBOR)the_player->sheep_exchange_rate=2;
    if(vprop(tobuild)->harb==STONE_HARBOR)the_player->stone_exchange_rate=2;
    if(vprop(tobuild)->harb==WHEAT_HARBOR)the_player->wheat_exchange_rate=2;
    if(vprop(tobuild)->harb==BRICK_HARBOR)the_player->brick_exchange_rate=2;
    if(vprop(tobuild)->harb==WOOD_HARBOR)the_player->wood_exchange_rate=2;
    clear_all_highlight();
}

void box_set()
{
    shuffle_identity_list(human_id);
    for(i32 i=0;i<4;i++)player_init(players+i);
    bank_init(&bank);
    card_temp_init( &cardtemp );


    int trade_withbank[10] = {0};
    int specialcard[4] = {0};
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
obj** highlight_availible_village_beginning()
{
    obj **ret=calloc(54,sizeof(obj*));
    i32 ret_csr=0;
    for(i32 i=0;i<54;i++)
    {
        if(vprop(vertice_list[i])->own!=None)continue;
        for(i32 j=0;j<3;j++)
        {
            obj *tgt=vprop(vertice_list[i])->nei_vert[j];
            if(tgt != NULL && vprop(tgt)->build != empty)
            {
                goto failure;
            }
        }
        vertice_list[i]->highlighted=1;
        ret[ret_csr]=vertice_list[i];
        ret_csr++;
        failure:;
    }
    return ret;
}
obj** highlight_availible_village(owner owner1)
{
    player_property *the_player=&(players[owner1-player1]);
    obj **ret=calloc(54,sizeof(obj*));
    i32 ret_csr=0;
    for(i32 i=0; i<the_player->my_road_csr;i++)
    {

        for(i32 j=0;j<2;j++)
        {
            obj *tgt=sprop(the_player->my_road[i])->nei_vert[j];//get vert around my road
            if(tgt->highlighted)continue;
            for(i32 k=0;k<3;k++)
            {
                obj* test=vprop(tgt)->nei_vert[k];//check if the nei_vert of "the vert by my road" is empty
                if(test != NULL && vprop(test)->build != empty)
                {
                    goto failure;
                }
            }
            tgt->highlighted=1;
            ret[ret_csr]=tgt;
            ret_csr++;
            failure:;

        }
    }
    return ret;
}
obj** highlight_available_road(owner owner1)
{
    obj **ret=calloc(72,sizeof(obj*));
    i32 ret_csr=0;
    for (i32 i=0;i<72;i++)
    {
        obj* tgt=side_list[i];
        if(sprop(tgt)->own!=None)continue;
        if(tgt->highlighted)continue;
        for(i32 j=0;j<2;j++)
        {
            obj *test=sprop(tgt)->nei_vert[j];
            if(vprop(test)->own==owner1)
            {
                tgt->highlighted=1;
                ret[ret_csr]=tgt;
                ret_csr++;
                break;
            }
        }
        for(i32 j=0;j<4;j++)
        {
            obj *test=sprop(tgt)->nei_side[j];
            if(test!=NULL&&sprop(test)->own==owner1)
            {
                tgt->highlighted=1;
                ret[ret_csr]=tgt;
                ret_csr++;
                break;
            }
        }
    }
    return ret;
}
obj** highlight_available_upgrade(owner owner1)
{
    obj **ret=calloc(54,sizeof(obj*));
    i32 ret_csr=0;
    for(i32 i=0;i<54;i++)
    {
        obj *tgt=vertice_list[i];
        if(vprop(tgt)->build==village && vprop(tgt)->own==owner1)
        {
            tgt->highlighted=1;
            ret[ret_csr]=tgt;
            ret_csr++;
        }
    }
    return ret;
}
void clear_all_highlight()
{
    for(i32 i=0;i<19;i++)body_list[i]->highlighted=0;
    for(i32 i=0;i<54;i++)vertice_list[i]->highlighted=0;
    for(i32 i=0;i<72;i++)side_list[i]->highlighted=0;
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

void tradewithbank( player_property *player, bank_property *bank, int trade_withbank[] )
{
	/*if( player -> wood < player -> wood_exchange_rate * trade[0] || 
	    player -> stone < player -> stone_exchange_rate * trade[1] ||
	    player -> brick < player -> brick_exchange_rate * trade[2] ||
	    player -> sheep < player -> sheep_exchange_rate * trade[3] ||
	    player -> wheat < player -> wheat_exchange_rate * trade[4] ||
	    bank -> wood < trade[5] ||
	    bank -> stone < trade[6] ||
	    bank -> brick < trade[7] ||
	    bank -> sheep < trade[8] ||
	    bank -> wheat < trade[9] )
	    	return;*/
	//player pay
	player -> wood -= player -> wood_exchange_rate * trade_withbank[0];
	bank -> wood += player -> wood_exchange_rate * trade_withbank[0];
	player -> stone -= player -> stone_exchange_rate * trade_withbank[1];
	bank -> stone += player -> stone_exchange_rate * trade_withbank[1];
	player -> brick -= player -> brick_exchange_rate * trade_withbank[2];
	bank -> brick += player -> brick_exchange_rate * trade_withbank[2];
	player -> sheep -= player -> sheep_exchange_rate * trade_withbank[3];
	bank -> sheep += player -> sheep_exchange_rate * trade_withbank[3];
	player -> wheat -= player -> wheat_exchange_rate * trade_withbank[4];
	bank -> wheat += player -> wheat_exchange_rate * trade_withbank[4];
	//player get
	player -> wood += trade_withbank[5];
	bank -> wood -= trade_withbank[5];
	player -> stone += trade_withbank[6];
	bank -> stone -= trade_withbank[6];
	player -> brick += trade_withbank[7];
	bank -> brick -= trade_withbank[7];
	player -> sheep += trade_withbank[8];
	bank -> sheep -= trade_withbank[8];
	player -> wheat += trade_withbank[9];
	bank -> wheat -= trade_withbank[9];
	return;
}

void robber( obj *robber )
{

}

void specialcard_get( card_temp *cardtemp, player_property *player, bank_property *bank, int count )
{
	for( int i = count; i > 0; i-- )
	{
	int card = rand() % bank -> special_cards + 1;
	int remain[5] = { bank -> knights, bank -> year_of_plenty, bank -> road_building, bank -> monopoly, bank -> victory_card };
	int c = 0;
	while( c == 0 )
	{
		c = 1;
		if( 0 < card && card <= remain[0] )
			card = 1;
		else if( remain[0] < card && card <= remain[0] + remain[1])
			card = 2;
		else if( remain[0] + remain[1] < card && card <= remain[0] + remain[1] + remain[2])
			card = 3;
		else if( remain[0] + remain[1] + remain[2] < card && card <= remain[0] + remain[1] + remain[2] + remain[3] )
			card = 4;
		else
			card = 5;
		switch( card )
		{
			case 1: 
				if( bank -> knights == 0 )
				{
					c = 0;
					card = rand() % bank -> special_cards + 1;
				}
				break;
			case 2: 
				if( bank -> year_of_plenty == 0 )
				{
					c = 0;
					card = rand() % bank -> special_cards + 1;
				}
				break;
			case 3: 
				if( bank -> road_building == 0 )
				{
					c = 0;
					card = rand() % bank -> special_cards + 1;
				}
				break;
			case 4: 
				if( bank -> monopoly == 0 )
				{
					c = 0;
					card = rand() % bank -> special_cards + 1;
				}
				break;
			case 5: 
				if( bank -> victory_card == 0 )
				{
					c = 0;
					card = rand() % bank -> special_cards + 1;
				}
				break;
			default:
				break;
		}
	}
	switch( card )
	{
		case 1: 
			cardtemp -> knights++;
			bank -> knights--;
			//player -> knights_get = true;
			break;
		case 2: 
			cardtemp -> year_of_plenty++;
			bank -> year_of_plenty--;
			//player -> year_of_plenty_get = true;
			break;
		case 3: 
			cardtemp -> road_building++;
			bank -> road_building--;
			//player -> road_building_get = true;
			break;
		case 4: 
			cardtemp -> monopoly++;
			bank -> monopoly--;
			//player -> monopoly_get = true;
			break;
		case 5: 
			player -> victory_card++;
			player -> total_victory_points++;
			bank -> victory_card--;
			//player -> victory_card_get = true;
			break;
		default:
			break;
	}
	}
}
void specialcard_use( player_property *player1, player_property *player2, player_property *player3, player_property *player4, bank_property *bank, int specialcard[], int trade[], owner owner, obj* tobuild )
{
	if( specialcard[0] == 1 ) //knights
	{
		player1 -> knights--;
		player1 -> knights_use++;
	}
	else if( specialcard[1] == 1 ) //year_of_plenty
	{
		player1 -> year_of_plenty--;
		//use trade[0] - trade[4] to represent resource ; remember to trade_init after this
		player1 -> wood += trade[0];
		bank -> wood -= trade[0];
		player1 -> stone += trade[1];
		bank -> stone -= trade[1];
		player1 -> brick += trade[2];
		bank -> brick -= trade[2];
		player1 -> sheep += trade[3];
		bank -> sheep -= trade[3];
		player1 -> wheat += trade[4];
		bank -> wheat -= trade[4];
	}
	else if( specialcard[2] == 1 ) //road_building
	{
		player1 -> road_building--;
		build_road(owner, tobuild);
		build_road(owner, tobuild);
	}
	else if( specialcard[3] == 1 ) //monopoly
	{
		player1 -> monopoly--;
		//use trade[0] - trade[4] to represent resource ; remember to trade_init after this
		if( trade[0] == 1 )
		{
			player1 -> wood = player2 -> wood + player3 -> wood + player4 -> wood;
			player2 -> wood = 0;
			player3 -> wood = 0;
			player4 -> wood = 0;
		}
		else if( trade[1] == 1 )
		{
			player1 -> stone = player2 -> stone + player3 -> stone + player4 -> stone;
			player2 -> stone = 0;
			player3 -> stone = 0;
			player4 -> stone = 0;
		}
		else if( trade[2] == 1 )
		{
			player1 -> brick = player2 -> brick + player3 -> brick + player4 -> brick;
			player2 -> brick = 0;
			player3 -> brick = 0;
			player4 -> brick = 0;
		}
		else if( trade[3] == 1 )
		{
			player1 -> sheep = player2 -> sheep + player3 -> sheep + player4 -> sheep;
			player2 -> sheep = 0;
			player3 -> sheep = 0;
			player4 -> sheep = 0;
		}
		else if( trade[4] == 1 )
		{
			player1 -> wheat = player2 -> wheat + player3 -> wheat + player4 -> wheat;
			player2 -> wheat = 0;
			player3 -> wheat = 0;
			player4 -> wheat = 0;
		}
	}
}
void discard_half_deck_action(player_property *the_player,uint8_t brick_discard,uint8_t sheep_discard,uint8_t stone_discard,uint8_t wheat_discard,uint8_t wood_discard)
{
    the_player->wheat-=wheat_discard;bank.wheat+=wheat_discard;
    the_player->wood-=wood_discard;bank.wood+=wood_discard;
    the_player->brick-=brick_discard;bank.brick+=brick_discard;
    the_player->sheep-=sheep_discard;bank.sheep+=sheep_discard;
    the_player->stone-=stone_discard;bank.stone+=stone_discard;
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
