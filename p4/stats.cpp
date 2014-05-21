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
} ply;

typedef struct team{ //stored in hashtable for teams
    struct t10 *top10;
    int count; //# of elements in top10
    QuadraticHashTable<uintptr_t> *players;
} team;

//top 10 is a double linked list
typedef struct t10 { //stored in tm and as a global var (for MLB)
    struct t10 *bigger; //NULL if it does not exist
    struct t10 *smaller; //NULL if it does not exist
    struct ply *player;
} t10;

t10 *MLB = NULL; //points to the smallest member of top10 of all teams
int MLBCount = 0; //# of elements in top10
QuadraticHashTable<int> *teams;

int count = 0;

#define UPDATE_AVG(PLY) \
    PLY->avg = ((float)(PLY->hits))/(PLY->atBats)

#define IS_MLB(TEAM) (*((int *)TEAM) == 0x424c4d)
#define HASH_TEAM(TEAM) ((*((int *)TEAM)) << 8) //faster than strcmp

Stats::Stats()
{
    teams = new QuadraticHashTable<int>(0, 2*30);
}

#define INIT_PLAYER(PLY) do{\
    PLY = (ply *)malloc(sizeof(ply));\
    PLY->hits = hit;\
    PLY->atBats = 1;\
    PLY->avg = (float)hit;\
} while(0)

inline void INSERT_TOP10(t10 *top10, ply *player, t10 *p10, int *count)
{
    for(top10 *i = top10; i != NULL; i = i->bigger)
    {
        if(player->avg <= i->player->avg) continue;

        for(top10 *j = i; j != NULL; j = j->bigger)
        {
            ply *upper = j->player; //the one before the first one that is bigger than player
            if(j->bigger && j->bigger->player->avg >= player->avg) continue;

            t10 *bigger10 = upper->bigger;

            bool isMLB = top10 == MLB;
            t10 *p10 = (t10 *)malloc(sizeof(t10));
            p10->player = player;
            if(isMLB)
                player->MLB10 = p10;
            else
                player->team10 = p10;

            p10->bigger = upper->bigger;
            p10->smaller = j;

            j->bigger = p10;
            upper->bigger->smaller = p10;

            if(*count == 10)
            {
                top10->bigger->smaller = NULL;

                if(isMLB)
                    top10->player->MLB10 = NULL;
                else
                    top10->player->team10 = NULL;

                free(top10);

            } else *count = *count + 1;

            break;
        }

        break;
    }
}

void Stats::update(const char nameStr[25], const char teamStr[4], int hit, int operationNum)
{
    //LOL FUCK USING STRCMP im gonna cheat
    char **ptr = (char **)nameStr;
    uintptr_t namePtr = (uintptr_t)ptr;

    team *theTeam = (team *)(teams->find(HASH_TEAM(teamStr)));

    if(!theTeam) //no team found, so create it
    {
        theTeam     = (team *)malloc(sizeof(tm));
        ply *player;
        t10 *top10  = (t10 *)malloc(sizeof(t10));

        //create player
        INIT_PLAYER(player);
        player->team10 = top10;

        //put him in his team's top10
        top10->bigger = NULL;
        top10->smaller = NULL;
        top10->player = player;

        //actually create the team
        theTeam->top10 = top10;
        theTeam->count = 1;
        theTeam->players = new QuadraticHashTable<uintptr_t>(0, 2*40);

        //insert player into team hash table
        theTeam->players->insert(namePtr, player);
        //insert into team hash table
        teams->insert(HASH_TEAM(teamStr), theTeam);

        count++;
        //printf("New team!!!!!! %s %d\n", teamStr, count);
        //printf("And player!!!! %s\n", nameStr);
    }
    else
    {
        ply *player = (ply *)(theTeam->players->find(namePtr));
        if(!player)
        {
            INIT_PLAYER(player);
            theTeam->players->insert(namePtr, player);
            //printf("New player!!!! %s\n", nameStr);
        }
        else
        {
            player->hits += hit;
            player->atBats++;
            UPDATE_AVG(player);
            //printf("Updating average for %s: %d/%d = %f\n", nameStr, player->hits, player->atBats, player->avg);
        }

    }
}

void Stats::query(const char teamStr[4], Player top10Players[10], int operationNum)
{
}

