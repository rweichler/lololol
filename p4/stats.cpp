#include "stats.h"
#include "statrunner.h"
#include <stdio.h>
#include <string.h>

typedef struct t10 t10;
typedef struct pl pl;

struct pl{
    float avg;
    int hits;
    int atBats;
    t10 *MLB; //entry in top10, NULL if not in top10
    t10 *team; //entry in top10, NULL if not in top10
};

//top 10 is a double linked list
struct t10 {
    t10 *bigger; //NULL if it does not exist
    t10 *smaller; //NULL if it does not exist
    pl *player;
};

t10 *MLB = NULL; //points to the smallest member of top10 of all teams

#define IS_MLB(TEAM) (*((int *)TEAM) == 0x424c4d)
#define HASH_TEAM(TEAM) ((*((int *)TEAM)) << 8) //faster than strcmp

Stats::Stats()
{
    printf("OH YEAH INITIALIZATION BABY\n\n");
}

void Stats::update(const char name[25], const char team[4], int hit, int operationNum)
{
    return;
    if(strcmp(team, "KC") != 0) return;
    //if(strcmp(name, "Heath Hembree") != 0) return;
    char **ptr = (char **)&name;

    printf("UPDATE: Team(%s HASH(%x) <%p>), Name(%s == %s<%p>\n", team, HASH_TEAM(team), &team, name, *ptr, ptr);
}

void Stats::query(const char team[4], Player top10[10], int operationNum)
{
    //if(strcmp(team, "MLB") != 0) return;
    printf("\nQUERY: Team(%s HASH(%x) <%p> %d)\n\n", team, HASH_TEAM(team), &team, IS_MLB(team));
    //printf("QUERY:  %s (%d)\n", team, HASH_TEAM(team));
    //printf("QUERY: %s\n", team);
}

