//Reed Weichler

#ifndef STATS_H
#define	STATS_H

#include "statrunner.h"

class Stats {
public:
  Stats();
  void update(const char name[25], const char team[4], int hit, int operationNum);
  void query(const char team[4], Player top10[10], int operationNum);
};

#endif	/* STATS_H */

