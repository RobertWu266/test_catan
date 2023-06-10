//
// Created by wu on 5/18/23.
//

#include "catan_conversions.h"


obj* innerbox[13][13][3]={0};
char* attr_names[]={"body","v_side","main_side","minor_side","pos_vert","neg_vert","invalid"};

i32 abs(i32 x)
{
    if(x<0)return -1*x;
    return x;
}
i32 customfindsum(i32 x,i32 y)
{
    if((x-0)%2==0&&(y-0)%6==0&&((x-0)/2+(y-0)/6)%2==0)return 0;//body(0,0)
    if((x-2)%2==0&&(y-0)%6==0&&((x-2)/2+(y-0)/6)%2==0)return 0;//v_side(2,0)
    if((x-1)%2==0&&(y-3)%6==0&&((x-1)/2+(y-3)/6)%2==0)return 0;//main_side(1,3)
    if((x+1)%2==0&&(y-3)%6==0&&((x+1)/2+(y-3)/6)%2==0)return 0;//minnor_side(-1,3)
    if((x-2)%2==0&&(y-2)%6==0&&((x-2)/2+(y-2)/6)%2==0)return 1;//pos_vert(2,2)
    if((x-0)%2==0&&(y-4)%6==0&&((x-0)/2+(y-4)/6)%2==0)return -1;//neg_vert(0,4)
    return 42;
}
i32 *custom2cube(i32 x,i32 y)
{
    i32 sum= customfindsum(x,y);
    if(!(sum==1||sum==0||sum==-1))return NULL;
    i32 *pret=calloc(3, sizeof(i32));
    pret[0]=x;
    pret[2]=(y+x)/2;
    i32 offset=(pret[0]+pret[2]-sum)/3;
    pret[0]-=offset;pret[1]-=offset;pret[2]-=offset;
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
    if(locs==NULL)return false;
    i32 sum=locs[0]+locs[1]+locs[2];
    if(!(sum==1||sum==0||sum==-1))return false;
    if(sum!=0&&(abs(locs[0]+locs[1])>6||abs(locs[0])>6||abs(locs[1])>6||abs(locs[2])>6))return false;
    if(sum==0&&(abs(locs[0]+locs[1])>5||abs(locs[0])>5||abs(locs[1])>5||abs(locs[2])>5))return false;
    return true;
}
attribute get_attr(i32 *locs)
{
    if(!isvalid(locs))return invalid;
    if(locs[0]%2==0&&locs[1]%2==0&&locs[2]%2==0)return body;
    if(locs[0]%2!=0&&locs[1]%2!=0&&locs[2]%2==0)return v_side;
    if(locs[0]%2==0&&locs[1]%2!=0&&locs[2]%2!=0)return main_side;
    if(locs[0]%2!=0&&locs[1]%2==0&&locs[2]%2!=0)return minor_side;
    if(locs[0]%2!=0&&locs[1]%2!=0&&locs[2]%2!=0)
    {
        if(locs[0]+locs[1]+locs[2]==1)return pos_vert;
        return neg_vert;
    }
    return invalid;
}
obj* find_obj(i32 x,i32 y,i32 z)
{
    i32 *locs=calloc(3,sizeof(i32));
    locs[0]=x;
    locs[1]=y;
    locs[2]=z;
    obj *pret= locs_find_obj(locs);
    free(locs);
    return pret;
}
obj* locs_find_obj(i32 *locs)
{
    if(!isvalid(locs))return NULL;
    return innerbox[locs[0]+6][locs[1]+6][locs[0]+locs[1]+locs[2]+1];
}
i32 *render(i32 x,i32 y,i32 z)
{
    i32 *pret=calloc(3, sizeof(i32));
    pret[0]=x;
    pret[1]=y;
    pret[2]=z;
    return pret;
}
obj *init_obj(i32 *locs)
{
    attribute attr= get_attr(locs);
    if(attr==invalid)return NULL;
    obj *pret=calloc(1, sizeof(obj));
    pret->attr= attr;
    pret->locs=locs;
    pret->prop=NULL;
    return pret;
}
void set_neighbor(obj *tgt)
{
    if(tgt==NULL)return;
    switch (tgt->attr)
    {
        case body:
            tgt->prop=calloc(1,sizeof(body_property));
            bprop(tgt)->flag=BODY_PROPERTY;//Its useless
            bprop(tgt)->nei_vert= body_neighbor_vertice(tgt);//robber's case
            break;
        case neg_vert:
        case pos_vert:
            tgt->prop=calloc(1,sizeof(vertice_property));
            vprop(tgt)->flag=VERTICE_PROPERTY;//Its useless
            vprop(tgt)->nei_body= vertice_neighbor_body(tgt);//for getting neighboring resource (opening)
            vprop(tgt)->nei_vert= vertice_neighbor_vertice(tgt);//for checking no neighboring building
            break;
        case v_side:
        case main_side:
        case minor_side:
            tgt->prop=calloc(1,sizeof(side_property));
            sprop(tgt)->flag=SIDE_PROPERTY;//Its useless
            sprop(tgt)->nei_vert= side_neighbor_vertice(tgt);
            sprop(tgt)->nei_side= side_neighbor_side(tgt);//for longest road;
            break;
    }
}
void dtor_obj(obj* tgt)
{
    if(tgt==NULL)return;
    free(tgt->locs);
    switch (tgt->attr)
    {
        case body:
            free(((body_property*)tgt->prop)->nei_vert);//robber's case
            free(tgt->prop);
            break;
        case neg_vert:
        case pos_vert:
            tgt->prop=calloc(1,sizeof(vertice_property));
            free(((vertice_property*)tgt->prop)->nei_body);//for getting neighboring resource (opening)
            free(((vertice_property*)tgt->prop)->nei_vert);//for checking no neighboring building
            free(tgt->prop);
            break;
        case v_side:
        case main_side:
        case minor_side:
            free(((side_property*)tgt->prop)->nei_vert);
            free(((side_property*)tgt->prop)->nei_side);//for longest road;
            free(tgt->prop);
            break;
    }
    free(tgt);
    return;
}
void print_obj(obj* tgt)
{
    if (tgt == NULL)return;
    printf("%s\n%d,%d,%d\n", attr_names[tgt->attr], tgt->locs[0], tgt->locs[1], tgt->locs[2]);
}
void print_obj_all(obj* tgt)
{
    if(tgt==NULL)return;
    printf("%s\n%d,%d,%d\n",attr_names[tgt->attr],tgt->locs[0],tgt->locs[1],tgt->locs[2]);
    obj** neighbor=NULL;
    switch (tgt->attr)
    {
        case body:
            neighbor=bprop(tgt)->nei_vert;
            printf("Neighboring vertices:\n\n");
            for(i32 i=0;i<6;i++)
            {
                print_obj(neighbor[i]);
            }
            break;
        case neg_vert:
        case pos_vert:
            neighbor=vprop(tgt)->nei_vert;
            printf("Neighboring vertices:\n\n");
            for(i32 i=0;i<3;i++)
            {
                print_obj(neighbor[i]);
            }
            neighbor=vprop(tgt)->nei_body;
            printf("Neighboring Bodies:\n\n");
            for(i32 i=0;i<3;i++)
            {
                print_obj(neighbor[i]);
            }
            break;
        case v_side:
        case main_side:
        case minor_side:
            neighbor=sprop(tgt)->nei_vert;
            printf("Neighboring vertices:\n\n");
            for(i32 i=0;i<2;i++)
            {
                print_obj(neighbor[i]);
            }
            neighbor=sprop(tgt)->nei_side;
            printf("Neighboring sides:\n\n");
            for(i32 i=0;i<4;i++)
            {
                print_obj(neighbor[i]);
            }
            break;
    }
}

obj **body_neighbor_vertice(obj* tgt)
{
    obj **pret=calloc(6,sizeof(obj*));
    for(uint8_t i=1;i<7;i++)
    {
        pret[i-1]=find_obj(tgt->locs[0]+1-2*((i)&1),tgt->locs[1]+1-2*((i>>1)&1),tgt->locs[2]+1-2*((i>>2)&1));
    }
    return pret;
}

obj** side_neighbor_vertice(obj* tgt)
{
    obj **pret=NULL;
    obj *toput=NULL;
    switch (tgt->attr)
    {
        case minor_side:
            pret= calloc(2, sizeof(obj*));
            toput=find_obj(tgt->locs[0],tgt->locs[1]+1,tgt->locs[2]);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1]-1,tgt->locs[2]);
            if(toput!=NULL)pret[1]=toput;
            return pret;

        case main_side:
            pret= calloc(2, sizeof(obj*));
            toput=find_obj(tgt->locs[0]+1,tgt->locs[1],tgt->locs[2]);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0]-1,tgt->locs[1],tgt->locs[2]);
            if(toput!=NULL)pret[1]=toput;
            return pret;
        case v_side:
            pret= calloc(2, sizeof(obj*));
            toput=find_obj(tgt->locs[0],tgt->locs[1],tgt->locs[2]+1);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1],tgt->locs[2]-1);
            if(toput!=NULL)pret[1]=toput;
            return pret;
        default:
            return NULL;
    }

}
obj** vertice_neighbor_vertice(obj* tgt)
{
    obj **pret=NULL;
    obj *toput=NULL;
    switch (tgt->attr)
    {
        case pos_vert:
            pret= calloc(3, sizeof(obj*));
            toput=find_obj(tgt->locs[0]-2,tgt->locs[1],tgt->locs[2]);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1]-2,tgt->locs[2]);
            if(toput!=NULL)pret[1]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1],tgt->locs[2]-2);
            if(toput!=NULL)pret[2]=toput;
            return pret;

        case neg_vert:
            pret= calloc(3, sizeof(obj*));
            toput=find_obj(tgt->locs[0]+2,tgt->locs[1],tgt->locs[2]);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1]+2,tgt->locs[2]);
            if(toput!=NULL)pret[1]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1],tgt->locs[2]+2);
            if(toput!=NULL)pret[2]=toput;
            return pret;
        default:
            return NULL;
    }

}
obj** vertice_neighbor_side(obj* tgt)
{
    obj **pret=NULL;
    obj *toput=NULL;
    switch (tgt->attr)
    {
        case pos_vert:
            pret= calloc(3, sizeof(obj*));
            toput=find_obj(tgt->locs[0]-1,tgt->locs[1],tgt->locs[2]);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1]-1,tgt->locs[2]);
            if(toput!=NULL)pret[1]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1],tgt->locs[2]-1);
            if(toput!=NULL)pret[2]=toput;
            return pret;

        case neg_vert:
            pret= calloc(3, sizeof(obj*));
            toput=find_obj(tgt->locs[0]+1,tgt->locs[1],tgt->locs[2]);
            if(toput!=NULL)pret[0]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1]+1,tgt->locs[2]);
            if(toput!=NULL)pret[1]=toput;
            toput=find_obj(tgt->locs[0],tgt->locs[1],tgt->locs[2]+1);
            if(toput!=NULL)pret[2]=toput;
            return pret;
        default:
            return NULL;
    }
}
obj** vertice_neighbor_body(obj* tgt)
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
}
obj **side_neighbor_side(obj *tgt)
{
    if(!(tgt->attr==v_side||tgt->attr==main_side||tgt->attr==minor_side))return NULL;
    obj **nei_vert= side_neighbor_vertice(tgt);
    obj **pret=calloc(4,sizeof(obj*));
    obj* toput=NULL;
    i32 csr=0;
    for(i32 i=0;i<2;i++)
    {
        obj **vert_nei= vertice_neighbor_side(nei_vert[i]);

        for(i32 j=0;j<3;j++)
        {
            toput=vert_nei[j];

            if(toput==NULL||toput->attr==tgt->attr)continue;
            pret[csr]=toput;
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
void box_set()
{
    for(i32 i=0;i<13;i++)
    {
        for(i32 j=0;j<13;j++)
        {
            for(i32 k=0;k<3;k++)
            {
                i32 *locs=calloc(3,sizeof(i32));
                //index to cube
                locs[0]=i-6;
                locs[1]=j-6;
                locs[2]=k-(i-6)-(j-6)-1;

                innerbox[i][j][k]=init_obj(locs);
                if(innerbox[i][j][k]==NULL)free(locs);
            }
        }
    }
    for(i32 i=0;i<13;i++)
    {
        for(i32 j=0;j<13;j++)
        {
            for(i32 k=0;k<3;k++)
            {
                set_neighbor(innerbox[i][j][k]);
            }
        }
    }
}
void box_dtor()
{
    for(i32 i=0;i<13;i++)
    {
        for(i32 j=0;j<13;j++)
        {
            for(i32 k=0;k<3;k++)
            {
                dtor_obj(innerbox[i][j][k]);
            }
        }
    }
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