#ifndef MOTION_H
#define MOTION_H

bool ObjectInfoWindowFunction(int x, int y, int lastX, int lastY);
void CheckMonsterInfo(int x, int y, bool hit, int type, Monster *mCheck, Character *cCheck, bool showLvl);

#endif