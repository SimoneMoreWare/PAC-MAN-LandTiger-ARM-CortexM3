#include "game_renderer.h"

/**
 * Enumeration for render styles used in the game.
 */
typedef enum {
    RENDER_STYLE_FULL = 0,     // Fill the entire cell
    RENDER_STYLE_BORDER = 1,   // Render only the border of the cell
    RENDER_STYLE_CIRCLE = 2,   // Render a circular style
    RENDER_STYLE_PACMAN = 3,    // Render in a Pac-Man style
		RENDER_STYLE_GHOST  = 4  	 // Render ghost style
} RenderStyle;

/** Constants defining rendering properties */
#define RENDER_CELL_SIZE (TILE_SCALE)                          // Size of a single cell
#define RENDER_CELL_OFFSET (RENDER_CELL_SIZE - 1)             // Offset for rendering within a cell
#define RENDER_CHAR_WIDTH_PIXELS 8                            // Character width in pixels
#define RENDER_CHAR_HEIGHT_PIXELS 16                          // Character height in pixels
#define WRITINGS_DISPLAY_CENTER_Y (DISPLAY_CENTER_Y - (RENDER_CHAR_HEIGHT_PIXELS / 2)) // Center Y for text
#define PAUSE_WRITING_START_Y 11                              // Start Y-coordinate for PAUSE text
#define PAUSE_WRITING_END_Y 17                                // End Y-coordinate for PAUSE text

/** Color definitions for rendering elements */
#define PACMAN_COLOR COL_YELLOW                               // Color of Pac-Man
#define GHOST_COLOR COL_RED																		// Color of Blinky Ghost
#define PACMAN_REDUCTION 1                                    // Reduction for rendering Pac-Man
#define GHOST_REDUCTION 1
#define PILL_COLOR COL_TUMBLEWEED                             // Color of standard pills
#define SPECIAL_PILL_REDUCTION 1                              // Reduction for rendering special pills
#define STANDARD_PILL_REDUCTION 3                             // Reduction for rendering standard pills

/** Boolean constants for text width handling */
#define IS_FULL_WIDTH 1                                       // Full-width text rendering
#define IS_NOT_FULL_WIDTH 0                                   // Non-full-width text rendering

/** Function prototypes for rendering different elements */
void render_map_segment(unsigned char portion_start, unsigned char portion_end);
void clear_cell(Position pos);
void render_wall(Position pos);
void render_pill(Position pos);
void render_special_pill(Position pos);
void render_character(Position pos);
void render_ghost(Position pos);
void render_ghost_door(Position pos);
void fill_line(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color);
void draw_border(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color);
void render_circle(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color);
void render_character_model(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color, Direction orientation, unsigned char animationFrame);
void render_character_right(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned char frame, unsigned short color);
void render_character_left(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned char frame, unsigned short color);
void render_character_up(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned char frame, unsigned short color);
void render_character_down(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned char frame, unsigned short color);
void render_ghost_model(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color, Direction orientation, unsigned char animationFrame);
void render_ghost_eyes(unsigned short xStart, unsigned short yStart, unsigned short colorEyes, unsigned char animationFrame, Direction orientation);
void render_ghost_body(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short colorBody);
void render_ghost_lower_body(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short colorBody, unsigned char animationFrame);
void fill_cell(Position pos, unsigned short color, unsigned char fillStyle);
void fill_cell_with_offset(Position pos, unsigned char reduction, unsigned short color, RenderStyle fillStyle);
void render_text(unsigned short xStart, unsigned short yStart, char* string, unsigned short color, unsigned short colorBackground);
void render_centered_text(char* string, unsigned short color, unsigned short colorBackground, unsigned char isFullWidth);
void render_centered_text_with_y_offset(unsigned short start_y, char* string, unsigned short color, unsigned short colorBackground, unsigned char isFullWidth);
unsigned short to_absolute_x(unsigned char coordinate_x);
unsigned short to_absolute_y(unsigned char coordinate_y);
char* int_to_string(unsigned short value);
char* int_to_padded_string(unsigned short value, unsigned char padding);
char* center_text(char* string);

/** Static buffer for storing temporary text */
static char textBuffer[32];

/** Draw the PAUSE screen */
void draw_pause_screen() {
    render_centered_text_with_y_offset(WRITINGS_DISPLAY_CENTER_Y, "PAUSED", COL_WHITE, COL_BLACK, IS_FULL_WIDTH);
}

/** Remove the PAUSE screen */
void undraw_pause_screen() {
    render_centered_text_with_y_offset(WRITINGS_DISPLAY_CENTER_Y, " ", COL_BLACK, COL_BLACK, IS_FULL_WIDTH);
}

/** Draw the victory screen */
void draw_screen_victory() {
    render_centered_text("A VIND!", COL_YELLOW, COL_BLACK, IS_NOT_FULL_WIDTH);
}

/** Draw the GAME OVER screen */
void draw_screen_game_over() {
    LCD_Clear(COL_BLACK);
    render_centered_text_with_y_offset(WRITINGS_DISPLAY_CENTER_Y , "GAME OVER!", COL_RED, COL_BLACK, IS_FULL_WIDTH);
}

/** Draw the failure screen */
void draw_screen_failure() {
    render_centered_text_with_y_offset(WRITINGS_DISPLAY_CENTER_Y, "FAIL! TRY AGAIN", COL_RED, COL_BLACK, IS_FULL_WIDTH);
}

/** Draw statistics on the screen */
void draw_area_stat() {
    render_text(TIMER_AREA_START_X+16, PLAYER_GUI_START_Y , "TIME", COL_WHITE, COL_BLACK);
    render_text(SCORE_AREA_START_X+16, PLAYER_GUI_START_Y , "SCORE", COL_WHITE, COL_BLACK);

    draw_time_stat(gameRunning.remainingTime);
    draw_stat_score(gameRunning.score);
    draw_status_lives(gameRunning.remainingLives);
}

/** Draw the remaining time statistic */
void draw_time_stat(unsigned char timeValue) {
    render_text(TIMER_VALUE_START_X, TIMER_VALUE_START_Y , int_to_padded_string(timeValue, 2), COL_WHITE, COL_BLACK);
}

/** Draw the score statistic */
void draw_stat_score(unsigned short scoreValue) {
    render_text(SCORE_VALUE_START_X, SCORE_VALUE_START_Y, int_to_padded_string(scoreValue, 4), COL_WHITE, COL_BLACK);
}

/** Draw the player's remaining lives */
void draw_status_lives(unsigned char livesValue) {
    const unsigned char DISPLAY_PADDING__LIVES = RENDER_CELL_SIZE / 2;
    unsigned char i;
    for(i = 0; i < GAME_CONFIG.maxLives; i++) {
        unsigned short temp = ((RENDER_CELL_SIZE + DISPLAY_PADDING__LIVES) * i);
        if(i < livesValue) {
            render_character_model(LIVES_VALUE_START_X + temp, LIVES_VALUE_START_Y, LIVES_VALUE_START_X + temp + RENDER_CELL_OFFSET, LIVES_VALUE_START_Y + RENDER_CELL_OFFSET, PACMAN_COLOR, DIRECTION_LEFT, 0);
        } else {
            fill_line(LIVES_VALUE_START_X + temp, LIVES_VALUE_START_Y, LIVES_VALUE_START_X + temp + RENDER_CELL_OFFSET, LIVES_VALUE_START_Y + RENDER_CELL_OFFSET, COL_BLACK);
        }
    }
}

/** Draw the entire game map */
void draw_game_map() {
    render_map_segment(0, GAME_MAP_HEIGHT);
}

/** Redraw the map portion obscured by the PAUSE screen */
void redraw_after_pause() {
    render_map_segment(PAUSE_WRITING_START_Y, PAUSE_WRITING_END_Y);
}

/** Render a segment of the map */
void render_map_segment(unsigned char portion_start, unsigned char portion_end) {
    unsigned char i, j;
    CellType temp;
    Position temp_pos;

    for(i = portion_start; i < portion_end; i++) {
        for(j = 0; j < GAME_MAP_WIDTH; j++) {
            temp = (CellType) gameRunning.gameMap[i][j];
            temp_pos = (Position) {.x = j, .y = i};
            draw_cell(temp, temp_pos);
        }
    }
}

/** Render a single cell based on its type */
void draw_cell(CellType cell_type, Position cell_pos) {
    switch(cell_type) {
        case CELL_FREE:
            clear_cell(cell_pos);
            break;
        case CELL_WALL:
            render_wall(cell_pos);
            break;
        case CELL_TELEPORT_LEFT:
        case CELL_TELEPORT_RIGHT:
            clear_cell(cell_pos);
            break;
        case CELL_STANDARD_PILL:
            clear_cell(cell_pos);  // Prevent lingering pill artifacts
            render_pill(cell_pos);
            break;
        case CELL_SPECIAL_PILL:
            render_special_pill(cell_pos);
            break;
        case CELL_PACMAN:
            render_character(cell_pos);
            break;
        case CELL_GHOST_DOOR:
            render_ghost_door(cell_pos);
            break;
				case CELL_GHOST:
						render_ghost(cell_pos);
						break;
        default:
            clear_cell(cell_pos);
            break;
    }
}

/**
 * Clear a cell on the screen by filling it with the background color (black).
 * @param pos The position of the cell to be cleared.
 */
void clear_cell(Position pos) {
    fill_cell(pos, COL_BLACK, RENDER_STYLE_FULL);
}

/**
 * Render a wall cell with a blue border style.
 * @param pos The position of the wall cell.
 */
void render_wall(Position pos) {
    fill_cell(pos, COL_BLUE, RENDER_STYLE_BORDER);
}

/**
 * Render a standard pill in a circular style with a defined reduction offset.
 * @param pos The position of the pill cell.
 */
void render_pill(Position pos) {
    fill_cell_with_offset(pos, STANDARD_PILL_REDUCTION, PILL_COLOR, RENDER_STYLE_CIRCLE);
}

/**
 * Render a special pill in a circular style with a smaller reduction offset.
 * @param pos The position of the special pill cell.
 */
void render_special_pill(Position pos) {
    fill_cell_with_offset(pos, SPECIAL_PILL_REDUCTION, PILL_COLOR, RENDER_STYLE_CIRCLE);
}

/**
 * Render the Pac-Man character at a specified position.
 * @param pos The position of the Pac-Man cell.
 */
void render_character(Position pos) {
    fill_cell_with_offset(pos, PACMAN_REDUCTION, PACMAN_COLOR, RENDER_STYLE_PACMAN);
}

void render_ghost(Position pos) {
	fill_cell_with_offset(pos, GHOST_REDUCTION, GHOST_COLOR, RENDER_STYLE_GHOST);
}

/**
 * Render the ghost door with a solid style and a unique color.
 * @param pos The position of the ghost door cell.
 */
void render_ghost_door(Position pos) {
    fill_cell(pos, COL_GHOST_DOOR, RENDER_STYLE_FULL);
}

/**
 * Fill a cell with a given color and style.
 * This is a wrapper for `fill_cell_with_offset` with no reduction.
 * @param pos The position of the cell.
 * @param color The color to fill the cell with.
 * @param fillStyle The rendering style to use.
 */
void fill_cell(Position pos, unsigned short color, RenderStyle fillStyle) {
    fill_cell_with_offset(pos, 0, color, fillStyle);
}

/**
 * Fill a cell with an offset for rendering styles.
 * Handles different styles like FULL, BORDER, CIRCLE, and PACMAN.
 * @param pos The position of the cell.
 * @param reduction The reduction offset to shrink the rendered area.
 * @param color The color to use for rendering.
 * @param fillStyle The rendering style to apply.
 */
void fill_cell_with_offset(Position pos, unsigned char reduction, unsigned short color, RenderStyle fillStyle) {
    unsigned short x_screenStart = to_absolute_x(pos.x) + reduction;
    unsigned short y_screenStart = to_absolute_y(pos.y) + reduction;
    unsigned short x_screenEnd = x_screenStart + RENDER_CELL_OFFSET - (reduction * 2);
    unsigned short y_screenEnd = y_screenStart + RENDER_CELL_OFFSET - (reduction * 2);

    switch (fillStyle) {
        case RENDER_STYLE_FULL:
            fill_line(x_screenStart, y_screenStart, x_screenEnd, y_screenEnd, color);
            break;
        case RENDER_STYLE_BORDER:
            draw_border(x_screenStart, y_screenStart, x_screenEnd, y_screenEnd, color);
            break;
        case RENDER_STYLE_CIRCLE:
            render_circle(x_screenStart, y_screenStart, x_screenEnd, y_screenEnd, color);
            break;
        case RENDER_STYLE_PACMAN:
            render_character_model(x_screenStart, y_screenStart, x_screenEnd, y_screenEnd, color, gameRunning.pacman.sprite.direction, gameRunning.pacman.sprite.animationFrame);
            break;
				case RENDER_STYLE_GHOST:
						render_ghost_model(x_screenStart, y_screenStart, x_screenEnd, y_screenEnd, color, gameRunning.ghost.sprite.direction, gameRunning.ghost.sprite.animationFrame);
						break;
    }
}

/**
 * Fill a rectangle defined by start and end coordinates with a color.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param xEnd Ending X-coordinate.
 * @param yEnd Ending Y-coordinate.
 * @param color The color to fill with.
 */
void fill_line(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color) {
    unsigned short i;
    const short lineWidth = ((yStart - yEnd) * -1) + 1;

    for (i = 0; i < lineWidth; i++) {
        LCD_DrawLine(xStart, (yStart + i), xEnd, (yStart + i), color);
    }
}

/**
 * Draw a border around a rectangular area with the specified color.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param xEnd Ending X-coordinate.
 * @param yEnd Ending Y-coordinate.
 * @param color The color of the border.
 */
void draw_border(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color) {
    unsigned short i;
    const short lineWidth = ((yStart - yEnd) * -1) + 1;

    i = 0;
    LCD_DrawLine(xStart, (yStart + i), xEnd, (yStart + i), color);
    while (i < lineWidth - 1) {
        LCD_SetPoint(xStart, (yStart + i), color);
        LCD_SetPoint(xEnd, (yStart + i), color);
        i++;
    }
    LCD_DrawLine(xStart, (yStart + i), xEnd, (yStart + i), color);
}

/**
 * Render a circle within a rectangular area.
 * The circle is defined by the radius, calculated from the rectangle.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param xEnd Ending X-coordinate.
 * @param yEnd Ending Y-coordinate.
 * @param color The color of the circle.
 */
void render_circle(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color) {
    unsigned short i;
    const short radius = ((xStart - xEnd) * -1) / 2;
    unsigned char isEven = (radius % 2 == 0) ? 1 : 0;

    for (i = 0; i <= radius; i++) {
        LCD_DrawLine(xStart + i, (yStart + radius - i), xEnd - i, (yStart + radius - i), color);
        LCD_DrawLine(xStart + i, (yStart + radius + i + isEven), xEnd - i, (yStart + radius + i + isEven), color);
    }
}

/**
 * Render a character model with orientation and animation.
 * Handles four directions: UP, DOWN, LEFT, and RIGHT.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param xEnd Ending X-coordinate.
 * @param yEnd Ending Y-coordinate.
 * @param color The color of the character.
 * @param orientation Direction the character is facing.
 * @param animationFrame Frame index for animation.
 */
void render_character_model(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color, Direction orientation, unsigned char animationFrame) {
    animationFrame = (animationFrame >= DURATION_FRAME);

    switch (orientation) {
        case DIRECTION_STILL:
        case DIRECTION_RIGHT:
            render_character_right(xStart, yStart, xEnd, yEnd, animationFrame, color);
            break;
        case DIRECTION_LEFT:
            render_character_left(xStart, yStart, xEnd, yEnd, animationFrame, color);
            break;
        case DIRECTION_UP:
            render_character_up(xStart, yStart, xEnd, yEnd, animationFrame, color);
            break;
        case DIRECTION_DOWN:
            render_character_down(xStart, yStart, xEnd, yEnd, animationFrame, color);
            break;
    }
}

/**
 * Render Pac-Man facing right.
 * The appearance changes depending on the animation frame.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param xEnd Ending X-coordinate.
 * @param yEnd Ending Y-coordinate.
 * @param frame Current animation frame (0 or 1).
 * @param color Color used for rendering.
 */
void render_character_right(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned char frame, unsigned short color) {
    if (frame == 0) { // First animation frame
        LCD_DrawLine(xStart + 1, yStart + 0, xEnd - 1, yStart + 0, color);
        LCD_DrawLine(xStart + 0, yStart + 1, xEnd - 2, yStart + 1, color);
        LCD_DrawLine(xStart + 0, yStart + 2, xEnd - 3, yStart + 2, color);
        LCD_DrawLine(xStart + 0, yStart + 3, xEnd - 4, yStart + 3, color);
        LCD_DrawLine(xStart + 0, yStart + 4, xEnd - 3, yStart + 4, color);
        LCD_DrawLine(xStart + 0, yStart + 5, xEnd - 2, yStart + 5, color);
        LCD_DrawLine(xStart + 1, yStart + 6, xEnd - 1, yStart + 6, color);
    } else { // Second animation frame
        LCD_DrawLine(xStart + 1, yStart + 0, xEnd - 1, yStart + 0, color);
        LCD_DrawLine(xStart + 0, yStart + 1, xEnd - 1, yStart + 1, color);
        LCD_DrawLine(xStart + 0, yStart + 2, xEnd - 2, yStart + 2, color);
        LCD_DrawLine(xStart + 0, yStart + 3, xEnd - 3, yStart + 3, color);
        LCD_DrawLine(xStart + 0, yStart + 4, xEnd - 2, yStart + 4, color);
        LCD_DrawLine(xStart + 0, yStart + 5, xEnd - 1, yStart + 5, color);
        LCD_DrawLine(xStart + 1, yStart + 6, xEnd - 1, yStart + 6, color);
    }
}

/**
 * Render Pac-Man facing left.
 * The appearance changes depending on the animation frame.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param xEnd Ending X-coordinate.
 * @param yEnd Ending Y-coordinate.
 * @param frame Current animation frame (0 or 1).
 * @param color Color used for rendering.
 */
void render_character_left(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned char frame, unsigned short color) {
    if (frame == 0) { // First animation frame
        LCD_DrawLine(xStart + 1, yStart + 0, xEnd - 1, yStart + 0, color);
        LCD_DrawLine(xStart + 2, yStart + 1, xEnd - 0, yStart + 1, color);
        LCD_DrawLine(xStart + 3, yStart + 2, xEnd - 0, yStart + 2, color);
        LCD_DrawLine(xStart + 4, yStart + 3, xEnd - 0, yStart + 3, color);
        LCD_DrawLine(xStart + 3, yStart + 4, xEnd - 0, yStart + 4, color);
        LCD_DrawLine(xStart + 2, yStart + 5, xEnd - 0, yStart + 5, color);
        LCD_DrawLine(xStart + 1, yStart + 6, xEnd - 1, yStart + 6, color);
    } else { // Second animation frame
        LCD_DrawLine(xStart + 1, yStart + 0, xEnd - 1, yStart + 0, color);
        LCD_DrawLine(xStart + 1, yStart + 1, xEnd - 0, yStart + 1, color);
        LCD_DrawLine(xStart + 2, yStart + 2, xEnd - 0, yStart + 2, color);
        LCD_DrawLine(xStart + 3, yStart + 3, xEnd - 0, yStart + 3, color);
        LCD_DrawLine(xStart + 2, yStart + 4, xEnd - 0, yStart + 4, color);
        LCD_DrawLine(xStart + 1, yStart + 5, xEnd - 0, yStart + 5, color);
        LCD_DrawLine(xStart + 1, yStart + 6, xEnd - 1, yStart + 6, color);
    }
}

/**
 * Render Pac-Man facing up.
 * The appearance changes depending on the animation frame.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param xEnd Ending X-coordinate.
 * @param yEnd Ending Y-coordinate.
 * @param frame Current animation frame (0 or 1).
 * @param color Color used for rendering.
 */
void render_character_up(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned char frame, unsigned short color) {
    if (frame == 0) { // First animation frame
        LCD_SetPoint(xStart, yStart + 1, COL_YELLOW);
        LCD_SetPoint(xEnd, yStart + 1, COL_YELLOW);
        LCD_DrawLine(xStart + 0, yStart + 2, xStart + 1, yStart + 2, color);
        LCD_DrawLine(xEnd - 0, yStart + 2, xEnd - 1, yStart + 2, color);
        LCD_DrawLine(xStart + 0, yStart + 3, xStart + 2, yStart + 3, color);
        LCD_DrawLine(xEnd - 0, yStart + 3, xEnd - 2, yStart + 3, color);
        LCD_DrawLine(xStart + 0, yStart + 4, xEnd - 0, yStart + 4, color);
        LCD_DrawLine(xStart + 0, yStart + 5, xEnd - 0, yStart + 5, color);
        LCD_DrawLine(xStart + 1, yStart + 6, xEnd - 1, yStart + 6, color);
    } else { // Second animation frame
        LCD_DrawLine(xStart + 0, yStart + 1, xStart + 1, yStart + 1, color);
        LCD_DrawLine(xEnd - 0, yStart + 1, xEnd - 1, yStart + 1, color);
        LCD_DrawLine(xStart + 0, yStart + 2, xStart + 2, yStart + 2, color);
        LCD_DrawLine(xEnd - 0, yStart + 2, xEnd - 2, yStart + 2, color);
        LCD_DrawLine(xStart + 0, yStart + 3, xEnd - 0, yStart + 3, color);
        LCD_DrawLine(xStart + 0, yStart + 4, xEnd - 0, yStart + 4, color);
        LCD_DrawLine(xStart + 0, yStart + 5, xEnd - 0, yStart + 5, color);
        LCD_DrawLine(xStart + 1, yStart + 6, xEnd - 1, yStart + 6, color);
    }
}

/**
 * Render Pac-Man facing down.
 * The appearance changes depending on the animation frame.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param xEnd Ending X-coordinate.
 * @param yEnd Ending Y-coordinate.
 * @param frame Current animation frame (0 or 1).
 * @param color Color used for rendering.
 */
void render_character_down(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned char frame, unsigned short color) {
	
	if (frame == 0) { // First animation frame
				LCD_DrawLine(xStart+1, yStart+0, xEnd-1, yStart+0, color);
				LCD_DrawLine(xStart+0, yStart+1, xEnd-0, yStart+1, color);
				LCD_DrawLine(xStart+0, yStart+2, xEnd-0, yStart+2, color);
				LCD_DrawLine(xStart+0, yStart+3, xStart+2, yStart+3, color);
				LCD_DrawLine(xEnd-0, yStart+3, xEnd-2, yStart+3, color);
				LCD_DrawLine(xStart+0, yStart+4, xStart+1, yStart+4, color);
				LCD_DrawLine(xEnd-0, yStart+4, xEnd-1, yStart+4, color);
				LCD_SetPoint(xStart, yStart+5, COL_YELLOW);
				LCD_SetPoint(xEnd, yStart+5, COL_YELLOW);
	} else { // Second animation frame
				LCD_DrawLine(xStart+1, yStart+0, xEnd-1, yStart+0, color);
				LCD_DrawLine(xStart+0, yStart+1, xEnd-0, yStart+1, color);
				LCD_DrawLine(xStart+0, yStart+2, xEnd-0, yStart+2, color);
				LCD_DrawLine(xStart+0, yStart+3, xEnd-0, yStart+3, color);
				LCD_DrawLine(xStart+0, yStart+4, xStart+2, yStart+4, color);
				LCD_DrawLine(xEnd-0, yStart+4, xEnd-2, yStart+4, color);
				LCD_DrawLine(xStart+0, yStart+5, xStart+1, yStart+5, color);
				LCD_DrawLine(xEnd-0, yStart+5, xEnd-1, yStart+5, color);
	}
	
}

void render_ghost_model(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short color, Direction orientation, unsigned char animationFrame) {
    // Determine the body and eye colors based on the ghost's vulnerability
    const unsigned short colorBody = (gameRunning.ghost.status.isVulnerable) ? COL_BLUE : color;
    const unsigned short colorEyes = (gameRunning.ghost.status.isVulnerable) ? COL_PINK : COL_CYAN;

    // Adjust animation frame duration for better visibility
    animationFrame = (animationFrame >= 2); // A frame lasts 2 ticks to make the animation more visible, especially on physical board

    // Render the main body of the ghost
    render_ghost_body(xStart, yStart, xEnd, yEnd, colorBody);

    // Render the lower body with animation effect
    render_ghost_lower_body(xStart, yStart, xEnd, yEnd, colorBody, animationFrame);

    // Render the eyes of the ghost
    render_ghost_eyes(xStart, yStart, colorEyes, animationFrame, orientation);
}

// Renders the main body of the ghost
void render_ghost_body(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short colorBody) {
    // Top of the ghost body
    LCD_DrawLine(xStart + 1, yStart + 0, xEnd - 1, yStart + 0, colorBody);
    
    // Body rows
    LCD_SetPoint(xStart + 0, yStart + 1, colorBody);
    LCD_SetPoint(xStart + 3, yStart + 1, colorBody);
    LCD_SetPoint(xStart + 6, yStart + 1, colorBody);
    
    LCD_SetPoint(xStart + 0, yStart + 2, colorBody);
    LCD_SetPoint(xStart + 3, yStart + 2, colorBody);
    LCD_SetPoint(xStart + 6, yStart + 2, colorBody);
    
    // Bottom of the ghost body
    LCD_DrawLine(xStart + 0, yStart + 3, xEnd - 0, yStart + 3, colorBody);
    LCD_DrawLine(xStart + 0, yStart + 4, xEnd - 0, yStart + 4, colorBody);
}

// Renders the lower part of the ghost body, applying animation effect
void render_ghost_lower_body(unsigned short xStart, unsigned short yStart, unsigned short xEnd, unsigned short yEnd, unsigned short colorBody, unsigned char animationFrame) {
    if (animationFrame == 0) {
        LCD_SetPoint(xStart + 0, yStart + 5, colorBody);
        LCD_DrawLine(xStart + 2, yStart + 5, xEnd - 2, yStart + 5, colorBody);
        LCD_SetPoint(xEnd - 0, yStart + 5, colorBody);
        
        LCD_SetPoint(xStart + 0, yStart + 6, colorBody);
        LCD_SetPoint(xStart + 2, yStart + 6, colorBody);
        LCD_SetPoint(xEnd - 2, yStart + 6, colorBody);
        LCD_SetPoint(xEnd - 0, yStart + 6, colorBody);
    } else if (animationFrame == 1) {
        LCD_DrawLine(xStart + 0, yStart + 5, xStart + 2, yStart + 5, colorBody);
        LCD_DrawLine(xEnd - 2, yStart + 5, xEnd - 0, yStart + 5, colorBody);
        
        LCD_SetPoint(xStart + 0, yStart + 6, colorBody);
        LCD_SetPoint(xStart + 2, yStart + 6, colorBody);
        LCD_SetPoint(xEnd - 2, yStart + 6, colorBody);
        LCD_SetPoint(xEnd - 0, yStart + 6, colorBody);
    }
}

// Renders the eyes of the ghost based on the ghost's orientation and animation frame
void render_ghost_eyes(unsigned short xStart, unsigned short yStart, unsigned short colorEyes, unsigned char animationFrame, Direction orientation) {
    // Draw the basic eye shapes (white eyes)
    LCD_DrawLine(xStart + 1, yStart + 1, xStart + 2, yStart + 1, COL_WHITE);
    LCD_DrawLine(xStart + 4, yStart + 1, xStart + 5, yStart + 1, COL_WHITE);
    LCD_DrawLine(xStart + 1, yStart + 2, xStart + 2, yStart + 2, COL_WHITE);
    LCD_DrawLine(xStart + 4, yStart + 2, xStart + 5, yStart + 2, COL_WHITE);

    // Draw pupils (black pixels) and adjust position based on orientation and animation frame
    switch (orientation) {
        case DIRECTION_STILL:
        case DIRECTION_RIGHT:
            // Pupils slightly to the right
            LCD_SetPoint(xStart + 2, yStart + animationFrame + 1, colorEyes); // Blue pixel (iris)
            LCD_SetPoint(xStart + 2, yStart + animationFrame + 1, COL_BLACK); // Black pixel (pupil)
            LCD_SetPoint(xStart + 5, yStart + animationFrame + 1, colorEyes); // Blue pixel (iris)
            LCD_SetPoint(xStart + 5, yStart + animationFrame + 1, COL_BLACK); // Black pixel (pupil)
            break;
        case DIRECTION_LEFT:
            // Pupils slightly to the left
            LCD_SetPoint(xStart + 1, yStart + animationFrame + 1, colorEyes); // Blue pixel (iris)
            LCD_SetPoint(xStart + 1, yStart + animationFrame + 1, COL_BLACK); // Black pixel (pupil)
            LCD_SetPoint(xStart + 4, yStart + animationFrame + 1, colorEyes); // Blue pixel (iris)
            LCD_SetPoint(xStart + 4, yStart + animationFrame + 1, COL_BLACK); // Black pixel (pupil)
            break;
        case DIRECTION_UP:
            // Pupils shifted upward
            LCD_SetPoint(xStart + 1 + animationFrame, yStart + 1, colorEyes); // Blue pixel (iris)
            LCD_SetPoint(xStart + 1 + animationFrame, yStart + 1, COL_BLACK); // Black pixel (pupil)
            LCD_SetPoint(xStart + 4 + animationFrame, yStart + 1, colorEyes); // Blue pixel (iris)
            LCD_SetPoint(xStart + 4 + animationFrame, yStart + 1, COL_BLACK); // Black pixel (pupil)
            break;
        case DIRECTION_DOWN:
            // Pupils shifted downward
            LCD_SetPoint(xStart + 1 + animationFrame, yStart + 2, colorEyes); // Blue pixel (iris)
            LCD_SetPoint(xStart + 1 + animationFrame, yStart + 2, COL_BLACK); // Black pixel (pupil)
            LCD_SetPoint(xStart + 4 + animationFrame, yStart + 2, colorEyes); // Blue pixel (iris)
            LCD_SetPoint(xStart + 4 + animationFrame, yStart + 2, COL_BLACK); // Black pixel (pupil)
            break;
    }
}


/**
 * Render text on the screen at the specified coordinates with a given foreground and background color.
 * @param xStart Starting X-coordinate.
 * @param yStart Starting Y-coordinate.
 * @param string Pointer to the text string to be rendered.
 * @param color Foreground color of the text.
 * @param colorBackground Background color of the text.
 */
void render_text(unsigned short xStart, unsigned short yStart, char* string, unsigned short color, unsigned short colorBackground) {
    GUI_Text(xStart, yStart, (uint8_t*) string, color, colorBackground);
}

/**
 * Render text that is horizontally centered on the screen, with optional full-width background.
 * This function assumes the text should be centered vertically on the display.
 * @param string Pointer to the text string to be rendered.
 * @param color Foreground color of the text.
 * @param colorBackground Background color of the text.
 * @param isFullWidth Flag to indicate whether the text spans the full width of the screen.
 */
void render_centered_text(char* string, unsigned short color, unsigned short colorBackground, unsigned char isFullWidth) {
    render_centered_text_with_y_offset(WRITINGS_DISPLAY_CENTER_Y, string, color, colorBackground, isFullWidth);
}

/**
 * Render text that is horizontally centered on the screen, with a vertical offset.
 * Calculates the left padding required to center the text based on its length.
 * @param start_y Y-coordinate where the text should be vertically centered.
 * @param string Pointer to the text string to be rendered.
 * @param color Foreground color of the text.
 * @param colorBackground Background color of the text.
 * @param isFullWidth Flag to indicate whether the text spans the full width of the screen.
 */
void render_centered_text_with_y_offset(unsigned short start_y, char* string, unsigned short color, unsigned short colorBackground, unsigned char isFullWidth) {
    if (isFullWidth) {
        // Render the text fully centered, with padding to span the entire width of the screen.
        render_text(0, start_y, center_text(string), color, colorBackground);
    } else {
        // Calculate the left padding required to center the text horizontally.
        const unsigned short calculated_left_padding = ((DISPLAY_WIDTH / RENDER_CHAR_WIDTH_PIXELS) - strlen(string)) / 2;
        const unsigned short calculated_start_x = (calculated_left_padding + (strlen(string) % 2)) * RENDER_CHAR_WIDTH_PIXELS;
        render_text(calculated_start_x, start_y, string, color, colorBackground);
    }
}

/**
 * Convert a grid-based X-coordinate to its absolute screen X-coordinate.
 * @param coordinate_x The grid X-coordinate to be converted.
 * @return The absolute X-coordinate on the screen.
 */
unsigned short to_absolute_x(unsigned char coordinate_x) {
    return GAME_MAP_START_X + (coordinate_x * RENDER_CELL_SIZE);
}

/**
 * Convert a grid-based Y-coordinate to its absolute screen Y-coordinate.
 * @param coordinate_y The grid Y-coordinate to be converted.
 * @return The absolute Y-coordinate on the screen.
 */
unsigned short to_absolute_y(unsigned char coordinate_y) {
    return GAME_MAP_START_Y + (coordinate_y * RENDER_CELL_SIZE);
}

/**
 * Convert an integer value to a string representation.
 * The result is stored in the static `textBuffer` for reuse.
 * @param value The integer value to convert.
 * @return A pointer to the string representation of the value.
 */
char* int_to_string(unsigned short value) {
    memset(textBuffer, 0, sizeof(textBuffer)); // Clear the buffer before writing.
    sprintf(textBuffer, "%d", value);         // Convert the integer to a string.
    return textBuffer;
}

/**
 * Convert an integer value to a string and pad it with spaces to a specified length.
 * The padding is added to the end of the string to meet the required width.
 * @param value The integer value to convert.
 * @param padding The total number of characters the output string should contain.
 * @return A pointer to the padded string representation of the value.
 */
char* int_to_padded_string(unsigned short value, unsigned char padding) {
    unsigned char index;
    unsigned char i;

    int_to_string(value); // Convert the integer to a string.

    index = strlen(textBuffer); // Get the current length of the string.
    for (i = 0; i < padding; i++) {
        textBuffer[index + i] = ' '; // Add spaces to the end of the string.
    }

    textBuffer[index + i] = '\0'; // Null-terminate the string.
    return textBuffer;
}

/**
 * Center a text string by adding padding spaces to the left and right.
 * The total padding is calculated based on the screen width and string length.
 * @param string Pointer to the original text string to be centered.
 * @return A pointer to the centered text string.
 */
char* center_text(char* string) {
    const unsigned short STR_LEN = strlen(string); // Length of the input string.
    const unsigned short TOT_STR_DISPLAY_PADDING_ = (DISPLAY_WIDTH / 8) - STR_LEN; // Total padding needed.
    const unsigned short LEFT_STR_DISPLAY_PADDING_ = TOT_STR_DISPLAY_PADDING_ / 2; // Padding on the left.
    const unsigned short RIGHT_STR_DISPLAY_PADDING_ = TOT_STR_DISPLAY_PADDING_ - LEFT_STR_DISPLAY_PADDING_; // Padding on the right.
    const unsigned char isOdd = STR_LEN % 2; // Check if the string length is odd.
    unsigned char len;
    unsigned char i;

    memset(textBuffer, 0, sizeof(textBuffer)); // Clear the text buffer.

    // Add padding to the left.
    for (i = 0; i < (LEFT_STR_DISPLAY_PADDING_ + isOdd); i++) {
        textBuffer[i] = ' ';
    }
    textBuffer[i] = '\0'; // Null-terminate after left padding.

    strcat(textBuffer, string); // Append the original string to the buffer.
    len = strlen(textBuffer); // Get the length of the new string.

    // Add padding to the right.
    for (i = len; i < (len + RIGHT_STR_DISPLAY_PADDING_ - isOdd); i++) {
        textBuffer[i] = ' ';
    }
    textBuffer[i] = '\0'; // Null-terminate after right padding.

    return textBuffer;
}



