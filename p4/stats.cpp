//Reed Weichler
#include "stats.h"
#include "statrunner.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "QuadraticProbing.h"
#include <stdint.h>

//struct t10;
//struct ply;
//struct tm;

typedef struct ply{ //stored in hashtable for players
    float avg;
    int hits;
    int atBats;
    struct t10 *MLB10; //entry in top10, NULL if not in top10
    struct t10 *team10; //entry in top10, NULL if not in top10
    const char *name;
    const char *team;
} ply;

typedef struct team{ //stored in hashtable for teams
    struct t10 *smallest;
    struct t10 *biggest;
    int count; //# of elements in top10
    QuadraticHashTable<uintptr_t> *players;
} team;

//top 10 is a double linked list
typedef struct t10 { //stored in tm and as a global var (for MLB)
    struct t10 *bigger; //NULL if it does not exist
    struct t10 *smaller; //NULL if it does not exist
    struct ply *player;
} t10;

t10 *MLBsmallest = NULL;
t10 *MLB = NULL; //points to the smallest member of top10 of all teams
int MLBcount = 0; //# of elements in top10
QuadraticHashTable<int> *teams;

#define UPDATE_AVG(PLY) \
    PLY->avg = ((float)(PLY->hits))/(PLY->atBats)

#define IS_MLB(TEAM) (*((int *)TEAM) == 0x424c4d)
#define HASH_TEAM(TEAM) ((*((int *)TEAM)) << 8) //faster than strcmp

Stats::Stats()
{
    teams = new QuadraticHashTable<int>(0, 2*30);
}

#define INIT_PLAYER(PLY, NAME, TEAM) do{\
    PLY = (ply *)malloc(sizeof(ply));\
    PLY->hits = hit;\
    PLY->atBats = 1;\
    PLY->avg = (float)hit;\
    PLY->name = NAME;\
    PLY->team = TEAM;\
} while(0)

#define PRINT_TOP10(T10, OP) do{\
    int __i;\
    t10 *__tmp = T10;\
    for(__i = 0; __i < 10 && __tmp != NULL; __i++, __tmp = __tmp->OP)\
    {\
        printf("%d. %s (%s) %f\n", __i, __tmp->player->name, __tmp->player->team, __tmp->player->avg);\
    }\
} while(0)

inline void INSERT_TOP10(t10 *ply10, int hit, t10 **smallestPtr, t10 **top10Ptr)
{
    if(hit)
    {
        if(!*smallestPtr)
        {
            *smallestPtr = *top10Ptr;
        }
        ply10->smaller = *top10Ptr;
        (*top10Ptr)->bigger = ply10;
        *top10Ptr = ply10;
    }
    else
    {
        if(!*smallestPtr)
        {
            *smallestPtr = ply10;
            ply10->bigger = *top10Ptr;
            (*top10Ptr)->smaller = ply10;
            return;
        }
        ply10->bigger = *smallestPtr;
        (*smallestPtr)->smaller = ply10;
        *smallestPtr = ply10;
    }
}

inline void UPDATE_TOP10(t10 *ply10, int hit, t10 **smallestPtr, t10 **top10Ptr)
{
    ply *player = ply10->player;

    if(hit)
    {
        while(ply10->bigger && player->avg > ply10->bigger->player->avg)
        {
            t10 *one = ply10->smaller;
            t10 *two = ply10->bigger;
            t10 *three = ply10;
            t10 *four = ply10->bigger->bigger;

            if(one) one->bigger = two;
            else *smallestPtr = two;
            two->smaller = one;
            two->bigger = three;
            three->smaller = two;
            three->bigger = four;
            if(four) four->smaller = three;
            else *top10Ptr = three;
        }
    }
    else
    {
        while(ply10->smaller && player->avg < ply10->smaller->player->avg)
        {
            t10 *one = ply10->smaller->smaller;
            t10 *two = ply10;
            t10 *three = ply10->smaller;
            t10 *four = ply10->bigger;

            if(one) one->bigger = two;
            else *smallestPtr = two;
            two->smaller = one;
            two->bigger = three;
            three->smaller = two;
            three->bigger = four;
            if(four) four->smaller = three;
            else *top10Ptr = three;
        }
    }
}

#define YEAH -1

t10 *INIT_TOP10(ply *player)
{
    t10 *top10 = (t10 *)malloc(sizeof(t10));
    top10->bigger = NULL;
    top10->smaller = NULL;
    top10->player = player;
    return top10;
}

void Stats::update(const char nameStr[25], const char teamStr[4], int hit, int operationNum)
{
    //printf("-----------MLB---------\ntop to bottom:\n");
    //printf("bottom to top:\n");
    //PRINT_TOP10(MLBsmallest, bigger);
    //printf("Smallest ptr is %p\n", MLBsmallest);
    if(operationNum == YEAH) exit(1);
    //printf("Operation num: %d\n", operationNum);
    //LOL FUCK USING STRCMP im gonna cheat
    char **ptr = (char **)nameStr;
    uintptr_t namePtr = (uintptr_t)ptr;

    team *theTeam = (team *)(teams->find(HASH_TEAM(teamStr)));

    if(!theTeam) //no team found, so create it
    {
        theTeam     = (team *)malloc(sizeof(tm));
        ply *player;
        //t10 *top10  = (t10 *)malloc(sizeof(t10));

        //create player
        INIT_PLAYER(player, nameStr, teamStr);
        player->team10 = INIT_TOP10(player);//top10;
        player->MLB10 = INIT_TOP10(player);

        //actually create the team
        theTeam->smallest = NULL;
        theTeam->biggest = player->team10;
        theTeam->count = 1;
        theTeam->players = new QuadraticHashTable<uintptr_t>(0, 2*40);

        //insert player into team hash table
        theTeam->players->insert(namePtr, player);
        //insert into team hash table
        teams->insert(HASH_TEAM(teamStr), theTeam);

        //MLBcount++;
        //printf("New team!!!!!! %s\n", teamStr);
        //printf("And player!!!! %s %s\n", nameStr, hit ? "hit":"missed");

        if(!MLB)
        {
            MLBsmallest = NULL;
            MLB = player->MLB10;
            MLBcount = 1;
        }
        else
        {
            INSERT_TOP10(player->MLB10, hit, &MLBsmallest, &MLB);
        }

        //INSERT_TOP10(&top10, player, &(theTeam->count));
    }
    else
    {
        ply *player = (ply *)(theTeam->players->find(namePtr));
        if(!player)
        {
            INIT_PLAYER(player, nameStr, teamStr);
            player->team10 = INIT_TOP10(player);//top10;
            player->MLB10 = INIT_TOP10(player);
            theTeam->players->insert(namePtr, player);
            //printf("New player!!!! %s (%s) %s\n", nameStr, teamStr, hit?"hit":"missed");

            INSERT_TOP10(player->team10, hit, &(theTeam->smallest), &(theTeam->biggest));
            INSERT_TOP10(player->MLB10, hit, &MLBsmallest, &MLB);
        }
        else
        {
            player->hits += hit;
            player->atBats++;
            UPDATE_AVG(player);
            //printf("Updating average for %s: %d/%d = %f\n", nameStr, player->hits, player->atBats, player->avg);
            UPDATE_TOP10(player->team10, hit, &(theTeam->smallest), &(theTeam->biggest));
            UPDATE_TOP10(player->MLB10, hit, &MLBsmallest, &MLB);
        }

    }
}

void Stats::query(const char teamStr[4], Player top10Players[10], int operationNum)
{
    //printf("\nNEED TOP 10 for %s!!!! %d\n", teamStr, operationNum);
    t10 *top10;
    if(IS_MLB(teamStr))
        top10 = MLB;
    else
        top10 = ((team *)teams->find(HASH_TEAM(teamStr)))->biggest;

    int i;
    for(i = 0; i < 10; i++, top10 = top10->smaller)
    {
        strcpy(top10Players[i].name, top10->player->name);
        strcpy(top10Players[i].team, top10->player->team);
        //top10Players[i].name = top10->player->name;
        //top10Players[i].team = top10->player->team;
    }
}

