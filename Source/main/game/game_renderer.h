#ifndef __GAME_RENDERER_H
#define __GAME_RENDERER_H

#include <stdio.h>
#include <string.h>

#include "../GLCD/GLCD.h"

#include "game_engine.h"

#define COL_WHITE	0xffff
#define COL_BLACK	0x0000
#define COL_YELLOW	0xffc0
#define COL_CYAN	0x07ff
#define COL_MAGENTA	0xf814
#define COL_RED		0xf840
#define COL_GREEN	0x07e2
#define COL_BLUE	0x00bf
#define COL_PINK	0xfefb
#define COL_ORANGE	0xfd43
#define COL_LIME	0xbfec
#define COL_TUMBLEWEED 0xdd10
#define COL_GHOST_DOOR 0xfdbf

#define DURATION_FRAME 2

void draw_area_stat();
void draw_game_map();
void draw_time_stat(unsigned char timeValue);
void draw_stat_score(unsigned short scoreValue);
void draw_status_lives(unsigned char livesValue);
void draw_pause_screen();
void undraw_pause_screen();
void redraw_after_pause();
void draw_screen_victory();
void draw_screen_game_over();
void draw_screen_failure();
void draw_cell(CellType cell_type, Position pos);

#endif //__GAME_RENDERER_H
