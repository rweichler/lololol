#include "stats.h"
#include "statrunner.h"
#include <stdio.h>

static inline int hash_team(const char team[4])
{
    return *((int *)team);
}

#define HASH_TEAM(TEAM) (*((int *)team))

Stats::Stats()
{
    printf("OH YEAH INITIALIZATION BABY\n\n");
}

void Stats::update(const char name[25], const char team[4], int hit, int operationNum)
{
    int x = ((((((((int)(team[0])) << 0x4) + (int)(team[1])) << 0x4) + (int)(team[2])) << 0x4) + (int)(team[3])) << 0x4;
    printf("UPDATE: %s (%x %x)\n", team, HASH_TEAM(team), x);
}

void Stats::query(const char team[4], Player top10[10], int operationNum)
{
    //printf("QUERY:  %s (%d)\n", team, HASH_TEAM(team));
    //printf("QUERY: %s\n", team);
}

