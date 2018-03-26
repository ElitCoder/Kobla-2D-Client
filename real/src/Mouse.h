#ifndef MOUSE_H
#define MOUSE_H

#include <SDL.h>

bool HandleMouse(SDL_Event event, bool &altf, bool &move);
void HandleMouseUp(SDL_Event event);
void HandleMouseMotion(SDL_Event event);
bool HandleKey(SDL_Event event, bool &move);

bool HandleMouseLogin(SDL_Event event);
bool HandleKeyLogin(SDL_Event event);

#endif