#ifndef VIEW_FINDER_H
#define VIEW_FINDER_H

#include "ObjectsManager.h"

typedef struct VIEW_FINDER {
    Object* rects[4];
} ViewFinder;


ViewFinder* makeViewFinder();
void drawViewFinder(ViewFinder* vf);

#endif
