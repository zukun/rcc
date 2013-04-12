#ifndef INCLUDES_H
#define INCLUDES_H



#define REFP(p) REF((p).x, (p).y)

/* (half of) the neighborhood system
   the full neighborhood system is edges in NEIGHBORS
   plus reversed edges in NEIGHBORS */
const struct Coord NEIGHBORS[] = { Coord(1, 0), Coord(0, -1) };
#define NEIGHBOR_NUM (sizeof(NEIGHBORS) / sizeof(Coord))


#endif
