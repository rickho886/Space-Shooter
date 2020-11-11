// [main.c]
// this template is provided for the 2D shooter game.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <math.h>

// If defined, logs will be shown on console and written to file.
// If commented out, logs will not be shown nor be saved.
#define LOG_ENABLED

/* Constants. */

// Frame rate (frame per second)
const int FPS = 60;
// Display (screen) width.
const int SCREEN_W = 800;
// Display (screen) height.
const int SCREEN_H = 600;
// At most 4 audios can be played at a time.
const int RESERVE_SAMPLES = 4;
// Same as:
// const int SCENE_MENU = 1;
// const int SCENE_START = 2;
enum {
	SCENE_MENU = 1,
	SCENE_START = 2,
	// [HACKATHON 3-7]
	// TODO: Declare a new scene id.
    SCENE_SETTINGS = 3,
    SCENE_OVER = 4,
    SCENE_WIN = 5,
    SCENE_PLANE = 6,
    SCENE_OPENING = 7,
};

/* Input states */

// The active scene id.
int active_scene;
// Keyboard state, whether the key is down or not.
bool key_state[ALLEGRO_KEY_MAX];
// Mouse state, whether the key is down or not.
// 1 is for left, 2 is for right, 3 is for middle.
bool *mouse_state;
// Mouse position.
int mouse_x, mouse_y;
// TODO: More variables to store input states such as joysticks, ...
/* Variables for allegro basic routines. */

ALLEGRO_DISPLAY* game_display;
ALLEGRO_EVENT_QUEUE* game_event_queue;
ALLEGRO_TIMER* game_update_timer;

/* Shared resources*/

ALLEGRO_FONT* font_pirulen_32;
ALLEGRO_FONT* font_pirulen_24;
// TODO: More shared resources or data that needed to be accessed
// across different scenes.

/* Menu Scene resources*/
ALLEGRO_BITMAP* main_img_background;
// [HACKATHON 3-1]
// TODO: Declare 2 variables for storing settings images.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_settings;
ALLEGRO_BITMAP* img_settings2;
ALLEGRO_BITMAP* img_pause;
ALLEGRO_BITMAP* img_pause2;
ALLEGRO_BITMAP* img_settings_bg;
ALLEGRO_BITMAP* img_settings_bg2;
ALLEGRO_BITMAP* img_game_over_bg;
ALLEGRO_BITMAP* img_game_win_bg;
ALLEGRO_BITMAP* img_boss;
ALLEGRO_BITMAP* explode[23];
ALLEGRO_SAMPLE* main_bgm;
ALLEGRO_SAMPLE_ID main_bgm_id;
ALLEGRO_SAMPLE* boss_bgm;
ALLEGRO_SAMPLE_ID boss_bgm_id;
ALLEGRO_SAMPLE* laser;
ALLEGRO_SAMPLE_ID laser_id;
ALLEGRO_SAMPLE* laser2;
ALLEGRO_SAMPLE_ID laser2_id;
ALLEGRO_SAMPLE* win;
ALLEGRO_SAMPLE_ID win_id;
ALLEGRO_SAMPLE* lose;
ALLEGRO_SAMPLE_ID lose_id;
ALLEGRO_SAMPLE* click;
ALLEGRO_SAMPLE_ID click_id;
ALLEGRO_SAMPLE* settings_bgm;
ALLEGRO_SAMPLE_ID settings_bgm_id;
ALLEGRO_SAMPLE* plane_bgm;
ALLEGRO_SAMPLE_ID plane_bgm_id;


/* Start Scene resources*/
ALLEGRO_BITMAP* start_img_background;
ALLEGRO_BITMAP* start_img_plane;
ALLEGRO_BITMAP* start_img_plane2;
ALLEGRO_BITMAP* start_img_enemy;
ALLEGRO_SAMPLE* start_bgm;
ALLEGRO_SAMPLE_ID start_bgm_id;
// [HACKATHON 2-1]
// TODO: Declare a variable to store your bullet's image.
// Uncomment and fill in the code below.
ALLEGRO_BITMAP* img_bullet;
ALLEGRO_BITMAP* img_bullet2;
ALLEGRO_BITMAP* img_bullet_enemy;
ALLEGRO_BITMAP* img_boss_bullet;
ALLEGRO_BITMAP* img_boss_bullet2;
ALLEGRO_BITMAP* img_explosion[23];

typedef struct {
	// The center coordinate of the image.
	float x, y;
	// The width and height of the object.
	float w, h;
	// The velocity in x, y axes.
	float vx, vy;
	// Should we draw this object on the screen.
	bool hidden;
	// The pointer to the object’s image.
	float hp;
	ALLEGRO_BITMAP* img;
} MovableObject;
void draw_movable_object(MovableObject obj);
#define MAX_ENEMY 3
// [HACKATHON 2-2]
// TODO: Declare the max bullet count that will show on screen.
// You can try max 4 bullets here and see if you needed more.
// Uncomment and fill in the code below.
#define MAX_BULLET 50
MovableObject plane;
MovableObject plane2;
MovableObject enemies[MAX_ENEMY];
MovableObject boss;
// [HACKATHON 2-3]
// TODO: Declare an array to store bullets with size of max bullet count.
// Uncomment and fill in the code below.
MovableObject bullets[MAX_BULLET];
MovableObject bullets2[MAX_BULLET];
MovableObject enemy_bullets[MAX_ENEMY][MAX_BULLET];
MovableObject boss_bullets[MAX_BULLET];
MovableObject boss_bullets2[MAX_BULLET];
// [HACKATHON 2-4]
// TODO: Set up bullet shooting cool-down variables.
// 1) Declare your shooting cool-down time as constant. (0.2f will be nice)
// 2) Declare your last shoot timestamp.
// Uncomment and fill in the code below.
float MAX_COOLDOWN = 0.2f;
const float MAX_COOLDOWN_ENEMIES = 1.0f;
const float MAX_COOLDOWN_WAVE = 8.0f;
double last_shoot_timestamp;
double last_shoot_timestamp_enemy;
double last_shoot_timestamp_boss;
double last_shoot_timestamp_boss2;
double last_shoot_timestamp_p2;
double last_shoot_timestamp_enemy_p2;
double last_shoot_timestamp_boss_p2;
double last_shoot_timestamp_boss2_p2;
double last_explo;
int dx[MAX_ENEMY] = {0};
int dy[MAX_ENEMY] = {0};
int dboss = 0;
int enemy_count = MAX_ENEMY;
int score = 0;
char highscore[100];
int temp;
int wave;
int pesawat = 1;
int pesawat2 = 0;
int song;
int pause;
int rpause;
int bgm = 1;
int sfx = 1;
float explosion = 15;
int p2 = 0;
int p2_count = 2;
int lastFrame = 0;
int flag = 0;
int y = 0;
int rstart = 0;


/* Declare function prototypes. */

// Initialize allegro5 library
void allegro5_init(void);
// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
void game_init(void);
// Process events inside the event queue using an infinity loop.
void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
void game_draw(void);
// Release resources.
// Free the pointers we allocated.
void game_destroy(void);
// Function to change from one scene to another.
void game_change_scene(int next_scene);
// Load resized bitmap and check if failed.
void explosive(int x, int y);
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);

/* Declare function prototypes for debugging. */

// Display error message and exit the program, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// If the program crashes unexpectedly, you can inspect "log.txt" for
// further information.
void game_abort(const char* format, ...);
// Log events for later debugging, used like 'printf'.
// Write formatted output to stdout and file from the format string.
// You can inspect "log.txt" for logs in the last run.
void game_log(const char* format, ...);
// Log using va_list.
void game_vlog(const char* format, va_list arg);
void delay(int num_of_seconds);

int main(int argc, char** argv) {
	// Set random seed for better random outcome.
	srand(time(NULL));
	allegro5_init();
	game_log("Allegro5 initialized");
	game_log("Game begin");
	// Initialize game variables.
	game_init();
	game_log("Game initialized");
	// Draw the first frame.
	game_draw();
	game_log("Game start event loop");
	// This call blocks until the game is finished.
	game_start_event_loop();
	game_log("Game end");
	game_destroy();
	return 0;
}

void allegro5_init(void) {
	if (!al_init())
		game_abort("failed to initialize allegro");
	// Initialize add-ons.
	if (!al_init_primitives_addon())
		game_abort("failed to initialize primitives add-on");
	if (!al_init_font_addon())
		game_abort("failed to initialize font add-on");
	if (!al_init_ttf_addon())
		game_abort("failed to initialize ttf add-on");
	if (!al_init_image_addon())
		game_abort("failed to initialize image add-on");
	if (!al_install_audio())
		game_abort("failed to initialize audio add-on");
	if (!al_init_acodec_addon())
		game_abort("failed to initialize audio codec add-on");
	if (!al_reserve_samples(RESERVE_SAMPLES))
		game_abort("failed to reserve samples");
	if (!al_install_keyboard())
		game_abort("failed to install keyboard");
	if (!al_install_mouse())
		game_abort("failed to install mouse");
	// TODO: Initialize other addons such as video, ...

	// Setup game display.
	game_display = al_create_display(SCREEN_W, SCREEN_H);
	if (!game_display)
		game_abort("failed to create display");
	al_set_window_title(game_display, "I2P(I)_2018_Yang Final Project <107062361>");

	// Setup update timer.
	game_update_timer = al_create_timer(1.0f / FPS);
	if (!game_update_timer)
		game_abort("failed to create timer");

	// Setup event queue.
	game_event_queue = al_create_event_queue();
	if (!game_event_queue)
		game_abort("failed to create event queue");

	// Malloc mouse buttons state according to button counts.
	const unsigned m_buttons = al_get_mouse_num_buttons();
	game_log("There are total %u supported mouse buttons", m_buttons);
	// mouse_state[0] will not be used.
	mouse_state = malloc((m_buttons + 1) * sizeof(bool));
	memset(mouse_state, false, (m_buttons + 1) * sizeof(bool));

	// Register display, timer, keyboard, mouse events to the event queue.
	al_register_event_source(game_event_queue, al_get_display_event_source(game_display));
	al_register_event_source(game_event_queue, al_get_timer_event_source(game_update_timer));
	al_register_event_source(game_event_queue, al_get_keyboard_event_source());
	al_register_event_source(game_event_queue, al_get_mouse_event_source());
	// TODO: Register other event sources such as timer, video, ...

	// Start the timer to update and draw the game.
	al_start_timer(game_update_timer);
}

void game_init(void) {
	/* Shared resources*/
	font_pirulen_32 = al_load_font("pirulen.ttf", 32, 0);
	if (!font_pirulen_32)
		game_abort("failed to load font: pirulen.ttf with size 32");

	font_pirulen_24 = al_load_font("pirulen.ttf", 24, 0);
	if (!font_pirulen_24)
		game_abort("failed to load font: pirulen.ttf with size 24");

	/* Menu Scene resources*/
	main_img_background = load_bitmap_resized("main-bg.jpg", SCREEN_W, SCREEN_H);

	main_bgm = al_load_sample("menu.ogg");
	if (!main_bgm)
		game_abort("failed to load audio: menu.ogg");
    boss_bgm = al_load_sample("boss.ogg");
    if(!boss_bgm) {
        game_abort("failed to load audio: boss.ogg");
    }

    laser = al_load_sample("laser.ogg");
    if(!laser) {
        game_abort("failed to load audio: laser.ogg");
    }

    laser2 = al_load_sample("laser2.ogg");
    if(!laser2) {
        game_abort("failed to load audio: laser2.ogg");
    }

    win = al_load_sample("win.ogg");
    if(!win) {
        game_abort("failed to load audio: win.ogg");
    }

    lose = al_load_sample("lose.ogg");
    if(!lose) {
        game_abort("failed to load audio: lose.ogg");
    }

    click = al_load_sample("click.ogg");
    if(!click) {
        game_abort("failed to load audio: click.ogg");
    }

    settings_bgm = al_load_sample("settings_bgm.ogg");
    if(!settings_bgm) {
        game_abort("failed to load audio: settings_bgm.ogg");
    }

    plane_bgm = al_load_sample("plane_bgm.ogg");
    if(!plane_bgm) {
        game_abort("failed to load audio: plane_bgm.ogg");
    }

	img_settings = al_load_bitmap("settings.png");
	if (!img_settings) {
		game_abort("failed to load image: settings.png");
	}
	img_settings2 = al_load_bitmap("settings2.png");
	if (!img_settings2)
		game_abort("failed to load image: settings2.png");

    img_pause = al_load_bitmap("pause.png");
	if (!img_settings) {
		game_abort("failed to load image: pause.png");
	}
	img_pause2 = al_load_bitmap("pause2.png");
	if (!img_settings2)
		game_abort("failed to load image: pause2.png");


    img_settings_bg = al_load_bitmap("settings-bg.jpg");
	if (!img_settings_bg) {
		game_abort("failed to load image: settings-bg.jpg");
	}

	img_settings_bg2 = load_bitmap_resized("settings-bg1.png", 800, 600);
	if (!img_settings_bg2) {
		game_abort("failed to load image: settings-bg1.png");
	}

    img_game_over_bg = al_load_bitmap("game-over.jpg");
    if (!img_game_over_bg) {
        game_abort("failed to load image: game-over.jpg");
    }
    img_game_win_bg = al_load_bitmap("game-win.png");
    if(!img_game_win_bg) {
        game_abort("failed to load image: game-win.png");
    }

    img_boss = al_load_bitmap("boss.png");
    if (!img_boss) {
        game_abort("failed to load image: boss.png");
    }
	/* Start Scene resources*/
	start_img_background = load_bitmap_resized("start-bg.jpg", SCREEN_W, SCREEN_H);


	start_img_plane = al_load_bitmap("plane.png");
	if (!start_img_plane)
		game_abort("failed to load image: plane.png");

    start_img_plane2 = al_load_bitmap("plane2.png");
	if (!start_img_plane2)
		game_abort("failed to load image: plane2.png");

	start_img_enemy = al_load_bitmap("smallfighter0006.png");
	if (!start_img_enemy)
		game_abort("failed to load image: smallfighter0006.png");

	start_bgm = al_load_sample("bgm.ogg");
	if (!start_bgm)
		game_abort("failed to load audio: bgm.ogg");

	img_bullet = al_load_bitmap("image13.png");
	if (!img_bullet)
		game_abort("failed to load image: image13.png");

    img_bullet2 = al_load_bitmap("image14.png");
    if (!img_bullet2)
        game_abort("failed to load image: image14.png");

    img_explosion[0] = al_load_bitmap("explosion/0.png");
    img_explosion[1] = al_load_bitmap("explosion/1.png");
    img_explosion[2] = al_load_bitmap("explosion/2.png");
    img_explosion[3] = al_load_bitmap("explosion/3.png");
    img_explosion[4] = al_load_bitmap("explosion/4.png");
    img_explosion[5] = al_load_bitmap("explosion/5.png");
    img_explosion[6] = al_load_bitmap("explosion/6.png");
    img_explosion[7] = al_load_bitmap("explosion/7.png");
    img_explosion[8] = al_load_bitmap("explosion/8.png");
    img_explosion[9] = al_load_bitmap("explosion/9.png");
    img_explosion[10] = al_load_bitmap("explosion/10.png");
    img_explosion[11] = al_load_bitmap("explosion/11.png");
    img_explosion[12] = al_load_bitmap("explosion/12.png");
    img_explosion[13] = al_load_bitmap("explosion/13.png");
    img_explosion[14] = al_load_bitmap("explosion/14.png");
    img_explosion[15] = al_load_bitmap("explosion/15.png");
    img_explosion[16] = al_load_bitmap("explosion/16.png");
    img_explosion[17] = al_load_bitmap("explosion/17.png");
    img_explosion[18] = al_load_bitmap("explosion/18.png");
    img_explosion[19] = al_load_bitmap("explosion/19.png");
    img_explosion[20] = al_load_bitmap("explosion/20.png");
    img_explosion[21] = al_load_bitmap("explosion/21.png");
    img_explosion[22] = al_load_bitmap("explosion/22.png");


    img_bullet_enemy = al_load_bitmap("bullet-enemy.png");
	if (!img_bullet_enemy)
		game_abort("failed to load image: bullet-enemy.png");

    img_boss_bullet = al_load_bitmap("beam.png");
	if (!img_boss_bullet)
		game_abort("failed to load image: beam.png");

    img_boss_bullet2 = al_load_bitmap("beam2.png");
	if (!img_boss_bullet2)
		game_abort("failed to load image: beam2.png");

	// Change to first scene.
	game_change_scene(SCENE_MENU);
}

void game_start_event_loop(void) {
	bool done = false;
	ALLEGRO_EVENT event;
	int redraws = 0;
	while (!done) {
		al_wait_for_event(game_event_queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			// Event for clicking the window close button.
			game_log("Window close button clicked");
			done = true;
		} else if (event.type == ALLEGRO_EVENT_TIMER) {
			// Event for redrawing the display.
			if (event.timer.source == game_update_timer)
				// The redraw timer has ticked.
				redraws++;
		} else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
			// Event for keyboard key down.
			game_log("Key with keycode %d down", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = true;
			on_key_down(event.keyboard.keycode);
		} else if (event.type == ALLEGRO_EVENT_KEY_UP) {
			// Event for keyboard key up.
			game_log("Key with keycode %d up", event.keyboard.keycode);
			key_state[event.keyboard.keycode] = false;
		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			// Event for mouse key down.
			game_log("Mouse button %d down at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = true;
			on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
		} else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			// Event for mouse key up.
			game_log("Mouse button %d up at (%d, %d)", event.mouse.button, event.mouse.x, event.mouse.y);
			mouse_state[event.mouse.button] = false;
		} else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
			if (event.mouse.dx != 0 || event.mouse.dy != 0) {
				// Event for mouse move.
				game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
				mouse_x = event.mouse.x;
				mouse_y = event.mouse.y;
			} else if (event.mouse.dz != 0) {
				// Event for mouse scroll.
				game_log("Mouse scroll at (%d, %d) with delta %d", event.mouse.x, event.mouse.y, event.mouse.dz);
			}
		}
		// TODO: Process more events and call callbacks by adding more
		// entries inside Scene.

		// Redraw
		if (redraws > 0 && al_is_event_queue_empty(game_event_queue)) {
			if (redraws > 1)
				game_log("%d frame(s) dropped", redraws - 1);
			// Update and draw the next frame.
			game_update();
			game_draw();
			redraws = 0;
		}
	}
}

void game_update(void) {
    int i, j;
    if(pause == 0) {
    if(wave == 10) {
        if(boss.hp > 0) {
            if(dboss == 0) {
                if(boss.x > SCREEN_W - 220) {
                    boss.x = SCREEN_W - 220;
                    dboss = 1;
                }
                boss.x += boss.vx;

            } else if(dboss == 1) {
                if(boss.x < 80) {
                    boss.x = 80;
                    dboss = 0;
                }
                boss.x -= boss.vx;
            }
        } else if(boss.hp < 0) {
            FILE *f = fopen("highscore.txt", "w+");
            if (f == NULL)
            {
                printf("Error opening file!\n");
                exit(1);
            }
            temp = atoi(highscore);
            if(temp < score) {
                fprintf(f, "%d", score);
            } else {
                fprintf(f, "%d", temp);
            }
            fclose(f);
            boss.hp = 100;
            temp = score;
            score = 0;
            wave = 1;
            al_stop_sample(&boss_bgm_id);
            if(bgm == 1)
                if (!al_play_sample(win, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &win_id));
            game_change_scene(5);
        }
        if(song % 2 == 0 && wave == 10) {
            song++;
            al_stop_sample(&start_bgm_id);
            game_log("stop audio (bgm)");
            if(bgm == 1)
                if (!al_play_sample(boss_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &boss_bgm_id))
                    game_log("failed to play audio (bgm)");
        }
    }

        for (i = 0; i < MAX_ENEMY; i++) {
                    if(enemies[i].hp > 0) {
                        if(dx[i] == 0) {
                            if(enemies[i].x > SCREEN_W - 14) {
                                enemies[i].x = SCREEN_W - 14;
                                dx[i] = 1;
                            }
                            enemies[i].x += (enemies[i].vx + wave*0.25);
                        } else if(dx[i] == 1) {
                            if(enemies[i].x < 14) {
                                enemies[i].x = 14;
                                dx[i] = 0;
                            }
                            enemies[i].x -= (enemies[i].vx + wave*0.25);
                        }
                        if(dy[i] == 0) {
                            if(enemies[i].y > SCREEN_H - 350) {
                                enemies[i].y = SCREEN_H - 350;
                                dy[i] = 1;
                            }
                            enemies[i].y += (enemies[i].vy + wave*0.25);
                        } else if(dy[i] == 1) {
                            if(enemies[i].y < 34) {
                                enemies[i].y = 34;
                                dy[i] = 0;
                            }
                            enemies[i].y -= (enemies[i].vy + wave*0.25);
                        }
                    } else if(enemies[i].hp == 0) {
                        enemies[i].hp = -1;
                        enemies[i].x = -30;
                        enemies[i].y = -30;
                    }
                }
        if (active_scene == SCENE_START) {
            if(pesawat == 1) {
                plane.vx = plane.vy = 0;
                if(p2 == 1) {
                    if (key_state[ALLEGRO_KEY_UP])
                        plane.vy -= 1;
                    if (key_state[ALLEGRO_KEY_DOWN])
                        plane.vy += 1;
                    if (key_state[ALLEGRO_KEY_LEFT])
                        plane.vx -= 1;
                    if (key_state[ALLEGRO_KEY_RIGHT])
                        plane.vx += 1;
                } else {
                    if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
                        plane.vy -= 1;
                    if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
                        plane.vy += 1;
                    if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
                        plane.vx -= 1;
                    if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
                        plane.vx += 1;
                }
                // 0.71 is (1/sqrt(2)).
                plane.y += plane.vy * 4 * (plane.vx ? 0.71f : 1);
                plane.x += plane.vx * 4 * (plane.vy ? 0.71f : 1);
                // [HACKATHON 1-1]
                // TODO: Limit the plane's collision box inside the frame.
                //       (x, y axes can be separated.)
                // Uncomment and fill in the code below.
                if (plane.x < 26)
                    plane.x = 26;
                else if (plane.x > SCREEN_W-26)
                    plane.x = SCREEN_W-26;
                if (plane.y < 38)
                    plane.y = 38;
                else if (plane.y > SCREEN_H-38)
                    plane.y = SCREEN_H-38;

                for (i = 0; i < MAX_BULLET; i++) {
                    if (bullets[i].hidden)
                        continue;
                    bullets[i].x += bullets[i].vx;
                    bullets[i].y += bullets[i].vy;
                    if(wave == 10) {
                        if(bullets[i].x < boss.x + 230 && bullets[i].x > boss.x - 80 && bullets[i].y < boss.y + 125 && bullets[i].y > boss.y - 50) {
                            bullets[i].hidden = true;
                            boss.hp -= 1;
                            score += 2;
                        }
                    }
                    for(j = 0; j < MAX_ENEMY; j++) {
                        if(bullets[i].x < enemies[j].x + 18 && bullets[i].x > enemies[j].x - 18 && bullets[i].y < enemies[j].y + 34 && bullets[i].y > enemies[j].y - 34) {
                            bullets[i].hidden = true;
                            enemies[j].hp -= 1;
                            score++;
                            if(enemies[j].hp == 0) {
                                bullets[i].x = enemies[j].x;
                                bullets[i].y = enemies[j].y;
                                if(game_update_timer) {
                                    al_draw_bitmap_region(img_explosion[lastFrame], 0, 0, 64, 64, bullets[i].x, bullets[i].y, 0);
                                    lastFrame++;
                                }
                                if(lastFrame == 23) {
                                    lastFrame = 0;
                                    bullets[i].x = -100;
                                    bullets[i].y = -100;
                                }
                                enemies[j].hidden = true;
                                enemy_count -= 1;
                            }
                        }
                    }
                    if (bullets[i].y < 0)
                        bullets[i].hidden = true;
                }
                for(j = 0; j < MAX_ENEMY; j++) {
                    for (i = 0; i < MAX_BULLET; i++) {
                        if (enemy_bullets[j][i].hidden)
                            continue;
                        enemy_bullets[j][i].x += enemy_bullets[j][i].vx;
                        enemy_bullets[j][i].y += enemy_bullets[j][i].vy + (wave*0.1);
                        if(enemy_bullets[j][i].x < plane.x + 26 && enemy_bullets[j][i].x > plane.x - 26 && enemy_bullets[j][i].y < plane.y + 38 && enemy_bullets[j][i].y > plane.y - 38 ) {
                            enemy_bullets[j][i].hidden = true;
                            if(p2 == 1 && p2_count == 2)
                                plane.hp -= 0.5;
                            else
                                plane.hp -= 1;
                        }
                        if (enemy_bullets[j][i].y > 600)
                            enemy_bullets[j][i].hidden = true;
                    }
                }
                if(wave == 10) {
                    for (i = 0; i < MAX_BULLET; i++) {
                        if (boss_bullets[i].hidden)
                            continue;
                        boss_bullets[i].y += boss_bullets[i].vy;
                        if(boss_bullets[i].x < plane.x + 26 && boss_bullets[i].x > plane.x - 26 && boss_bullets[i].y < plane.y + 38 && boss_bullets[i].y > plane.y - 38 ) {
                            boss_bullets[i].hidden = true;
                            if(p2 == 1 && p2_count == 2)
                                plane.hp -= 0.5;
                            else
                                plane.hp -= 1;
                        }
                        if (boss_bullets[i].y > 600)
                            boss_bullets[i].hidden = true;
                    }

                    for (i = 0; i < MAX_BULLET; i++) {
                        if (boss_bullets2[i].hidden)
                            continue;
                        boss_bullets2[i].y += boss_bullets2[i].vy;
                        if(boss_bullets2[i].x < plane.x + 26 && boss_bullets2[i].x > plane.x - 26 && boss_bullets2[i].y < plane.y + 38 && boss_bullets2[i].y > plane.y - 38 ) {
                            boss_bullets2[i].hidden = true;
                            if(p2 == 1 && p2_count == 2)
                                plane.hp -= 0.5;
                            else
                                plane.hp -= 1;
                        }
                        if (boss_bullets2[i].y > 600)
                            boss_bullets2[i].hidden = true;
                    }
                }

                double now = al_get_time();
                if(p2 == 1) {
                    if (key_state[ALLEGRO_KEY_RCTRL] && now - last_shoot_timestamp >= MAX_COOLDOWN - (wave*0.005)) {
                        if (sfx == 1)
                            if (!al_play_sample(laser, 1, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, &laser_id));
                        for (i = 0; i < MAX_BULLET; i++) {
                            if(bullets[i].hidden == true)
                                break;
                        }
                        if (i == MAX_BULLET)
                            return;
                        last_shoot_timestamp = now;
                        bullets[i].hidden = false;
                        bullets[i].x = plane.x;
                        bullets[i].y = plane.y;
                    }
                } else {
                    if (key_state[ALLEGRO_KEY_SPACE] && now - last_shoot_timestamp >= MAX_COOLDOWN - (wave*0.001)) {
                        if (sfx == 1)
                            if (!al_play_sample(laser, 1, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, &laser_id));
                        for (i = 0; i < MAX_BULLET; i++) {
                            if(bullets[i].hidden == true)
                                break;
                        }
                        if (i == MAX_BULLET)
                            return;
                        last_shoot_timestamp = now;
                        bullets[i].hidden = false;
                        bullets[i].x = plane.x;
                        bullets[i].y = plane.y;
                    }
                }

                double now_enemy = al_get_time();
                if(now_enemy - last_shoot_timestamp_enemy >= MAX_COOLDOWN_ENEMIES - (wave*0.05)) {
                    for(j = 0; j < MAX_ENEMY; j++) {
                        for (i = 0; i < MAX_BULLET; i++) {
                            if(enemy_bullets[j][i].hidden == true)
                                break;
                        }
                        if (i == MAX_BULLET)
                            return;
                        last_shoot_timestamp_enemy = now_enemy;
                        enemy_bullets[j][i].hidden = false;
                        enemy_bullets[j][i].x = enemies[j].x;
                        enemy_bullets[j][i].y = enemies[j].y+(0.1*wave);
                    }
                }

                if(wave == 10) {
                    double now_boss = al_get_time();
                        if(now_boss - last_shoot_timestamp_boss >= MAX_COOLDOWN_ENEMIES) {
                            for (i = 0; i < MAX_BULLET; i++) {
                                if(boss_bullets[i].hidden == true)
                                    break;
                            }
                            if (i == MAX_BULLET)
                                return;
                            last_shoot_timestamp_boss = now_boss;
                            boss_bullets[i].hidden = false;
                            boss_bullets[i].x = boss.x;
                            boss_bullets[i].y = boss.y;
                        }


                    double now_boss2 = al_get_time();
                        if(now_boss2 - last_shoot_timestamp_boss2 >= MAX_COOLDOWN_ENEMIES) {
                            for (i = 0; i < MAX_BULLET; i++) {
                                if(boss_bullets2[i].hidden == true)
                                    break;
                            }
                            if (i == MAX_BULLET)
                                return;
                            last_shoot_timestamp_boss2 = now_boss2;
                            boss_bullets2[i].hidden = false;
                            boss_bullets2[i].x = boss.x + 150;
                            boss_bullets2[i].y = boss.y;
                        }
                }
                if(p2 == 1) {
                    if(plane.hp <= 0) {
                        pesawat = 0;
                        p2_count--;
                    }
                } else {
                    if(plane.hp <= 0) {
                        FILE *f = fopen("highscore.txt", "w+");
                        if (f == NULL)
                        {
                            printf("Error opening file!\n");
                            exit(1);
                        }
                        temp = atoi(highscore);
                        if(temp < score) {
                            fprintf(f, "%d", score);
                        } else {
                            fprintf(f, "%d", temp);
                        }
                        fclose(f);
                        temp = score;
                        score = 0;
                        wave = 1;
                        al_stop_sample(&boss_bgm_id);
                        if(bgm == 1)
                            if (!al_play_sample(lose, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &lose_id));
                        game_change_scene(SCENE_OVER);
                    }
                }


            }
            if(pesawat2 == 1) {
                plane2.vx = plane2.vy = 0;
                if(p2 == 1) {
                    if (key_state[ALLEGRO_KEY_W])
                        plane2.vy -= 1;
                    if (key_state[ALLEGRO_KEY_S])
                        plane2.vy += 1;
                    if (key_state[ALLEGRO_KEY_A])
                        plane2.vx -= 1;
                    if (key_state[ALLEGRO_KEY_D])
                        plane2.vx += 1;
                } else {
                    if (key_state[ALLEGRO_KEY_UP] || key_state[ALLEGRO_KEY_W])
                        plane2.vy -= 1;
                    if (key_state[ALLEGRO_KEY_DOWN] || key_state[ALLEGRO_KEY_S])
                        plane2.vy += 1;
                    if (key_state[ALLEGRO_KEY_LEFT] || key_state[ALLEGRO_KEY_A])
                        plane2.vx -= 1;
                    if (key_state[ALLEGRO_KEY_RIGHT] || key_state[ALLEGRO_KEY_D])
                        plane2.vx += 1;
                }

                plane2.y += plane2.vy * 4 * (plane2.vx ? 0.71f : 1);
                plane2.x += plane2.vx * 4 * (plane2.vy ? 0.71f : 1);

                if (plane2.x < 25)
                    plane2.x = 25;
                else if (plane2.x > SCREEN_W-25)
                    plane2.x = SCREEN_W-25;
                if (plane2.y < 23)
                    plane2.y = 23;
                else if (plane2.y > SCREEN_H-23)
                    plane2.y = SCREEN_H-23;

                for (i = 0; i < MAX_BULLET; i++) {
                    if (bullets2[i].hidden)
                        continue;
                    bullets2[i].x += bullets2[i].vx;
                    bullets2[i].y += bullets2[i].vy;
                    if(wave == 10) {
                        if(bullets2[i].x < boss.x + 230 && bullets2[i].x > boss.x - 80 && bullets2[i].y < boss.y + 125 && bullets2[i].y > boss.y - 50) {
                            bullets2[i].hidden = true;
                            boss.hp -= 1;
                            score += 2;
                        }
                    }
                    for(j = 0; j < MAX_ENEMY; j++) {
                        if(bullets2[i].x < enemies[j].x + 18 && bullets2[i].x > enemies[j].x - 18 && bullets2[i].y < enemies[j].y + 34 && bullets2[i].y > enemies[j].y - 34) {
                            bullets2[i].hidden = true;
                            enemies[j].hp -= 1;
                            score++;
                            if(enemies[j].hp == 0) {
                                enemies[j].hidden = true;
                                enemies[j].x = -30;
                                enemies[j].y = -30;
                                enemy_count -= 1;
                            }
                        }
                    }
                    if (bullets2[i].y < 0)
                        bullets2[i].hidden = true;
                }

                for(j = 0; j < MAX_ENEMY; j++) {
                    for (i = 0; i < MAX_BULLET; i++) {
                        if (enemy_bullets[j][i].hidden)
                            continue;
                        enemy_bullets[j][i].x += enemy_bullets[j][i].vx;
                        enemy_bullets[j][i].y += enemy_bullets[j][i].vy + (wave*0.1);
                        if((enemy_bullets[j][i].x < plane2.x + 26) && (enemy_bullets[j][i].x > plane2.x - 26) && (enemy_bullets[j][i].y < plane2.y + 24) && (enemy_bullets[j][i].y > plane2.y - 24)) {
                            enemy_bullets[j][i].hidden = true;
                            if(p2 == 1 && p2_count == 2)
                                plane2.hp -= 0.5;
                            else
                                plane2.hp -= 1;
                        }
                        if (enemy_bullets[j][i].y > 600)
                            enemy_bullets[j][i].hidden = true;
                    }
                }

                if(wave == 10) {
                    for (i = 0; i < MAX_BULLET; i++) {
                        if (boss_bullets[i].hidden)
                            continue;
                        boss_bullets[i].y += boss_bullets[i].vy;
                        if(boss_bullets[i].x < plane2.x + 26 && boss_bullets[i].x > plane2.x - 26 && boss_bullets[i].y < plane2.y + 38 && boss_bullets[i].y > plane2.y - 38 ) {
                            boss_bullets[i].hidden = true;
                            if(p2 == 1 && p2_count == 2)
                                plane2.hp -= 0.5;
                            else
                                plane2.hp -= 1;
                        }
                        if (boss_bullets[i].y > 600)
                            boss_bullets[i].hidden = true;
                    }

                    for (i = 0; i < MAX_BULLET; i++) {
                        if (boss_bullets2[i].hidden)
                            continue;
                        boss_bullets2[i].y += boss_bullets2[i].vy;
                        if(boss_bullets2[i].x < plane2.x + 26 && boss_bullets2[i].x > plane2.x - 26 && boss_bullets2[i].y < plane2.y + 38 && boss_bullets2[i].y > plane2.y - 38 ) {
                            boss_bullets2[i].hidden = true;
                            if(p2 == 1 && p2_count == 2)
                                plane2.hp -= 0.5;
                            else
                                plane2.hp -= 1;
                        }
                        if (boss_bullets2[i].y > 600)
                            boss_bullets2[i].hidden = true;
                    }
                }

                double now_p2 = al_get_time();
                if (key_state[ALLEGRO_KEY_SPACE] && now_p2 - last_shoot_timestamp_p2 >= MAX_COOLDOWN) {
                    if(sfx == 1)
                        if (!al_play_sample(laser2, 1, 0.0, 2.0, ALLEGRO_PLAYMODE_ONCE, &laser2_id));
                    for (i = 0; i < MAX_BULLET; i++) {
                        if(bullets2[i].hidden == true)
                            break;
                    }
                    if (i == MAX_BULLET)
                        return;
                    last_shoot_timestamp_p2 = now_p2;
                    bullets2[i].hidden = false;
                    bullets2[i].x = plane2.x;
                    bullets2[i].y = plane2.y;
                }

                double now_enemy_p2 = al_get_time();
                if(now_enemy_p2 - last_shoot_timestamp_enemy_p2 >= MAX_COOLDOWN_ENEMIES - (wave*0.05)) {
                    for(j = 0; j < MAX_ENEMY; j++) {
                        for (i = 0; i < MAX_BULLET; i++) {
                            if(enemy_bullets[j][i].hidden == true)
                                break;
                        }
                        if (i == MAX_BULLET)
                            return;
                        last_shoot_timestamp_enemy_p2 = now_enemy_p2;
                        enemy_bullets[j][i].hidden = false;
                        enemy_bullets[j][i].x = enemies[j].x;
                        enemy_bullets[j][i].y = enemies[j].y+(0.1*wave);;
                    }
                }

                if(wave == 10) {
                    double now_boss_p2 = al_get_time();
                        if(now_boss_p2 - last_shoot_timestamp_boss_p2 >= MAX_COOLDOWN_ENEMIES) {
                            for (i = 0; i < MAX_BULLET; i++) {
                                if(boss_bullets[i].hidden == true)
                                    break;
                            }
                            if (i == MAX_BULLET)
                                return;
                            last_shoot_timestamp_boss_p2 = now_boss_p2;
                            boss_bullets[i].hidden = false;
                            boss_bullets[i].x = boss.x;
                            boss_bullets[i].y = boss.y;
                        }


                    double now_boss2_p2 = al_get_time();
                        if(now_boss2_p2 - last_shoot_timestamp_boss2_p2 >= MAX_COOLDOWN_ENEMIES) {
                            for (i = 0; i < MAX_BULLET; i++) {
                                if(boss_bullets2[i].hidden == true)
                                    break;
                            }
                            if (i == MAX_BULLET)
                                return;
                            last_shoot_timestamp_boss2_p2 = now_boss2_p2;
                            boss_bullets2[i].hidden = false;
                            boss_bullets2[i].x = boss.x + 150;
                            boss_bullets2[i].y = boss.y;
                        }
                }
                if(p2 == 1) {
                    if(plane2.hp <= 0) {
                        pesawat2 = 0;
                        p2_count--;
                    }
                } else {
                    if(plane2.hp <= 0) {
                        FILE *f = fopen("highscore.txt", "w+");
                        if (f == NULL)
                        {
                            printf("Error opening file!\n");
                            exit(1);
                        }
                        temp = atoi(highscore);
                        if(temp < score) {
                            fprintf(f, "%d", score);
                        } else {
                            fprintf(f, "%d", temp);
                        }
                        fclose(f);
                        temp = score;
                        score = 0;
                        wave = 1;
                        al_stop_sample(&boss_bgm_id);
                        if(bgm == 1)
                            if (!al_play_sample(lose, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &lose_id));
                        game_change_scene(SCENE_OVER);
                    }
                }
            }
        }
    }
    if(enemy_count == 0 && wave < 10 && rstart == 1) {
                    pause = 1;
                    if(y < SCREEN_H / 2) {
                        al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 0), SCREEN_W / 2 , y, ALLEGRO_ALIGN_CENTER, "WAVE %d", wave+1);
                    }
                    y++;
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    al_flip_display();
                    if(y == SCREEN_H / 2) {
                        wave++;
                        if(pesawat != 0) {
                            plane.hp = 10;
                        }
                        if(pesawat2 != 0) {
                            plane2.hp = 20;
                        }
                        for(i = 0; i < MAX_ENEMY; i++) {
                            enemies[i].hidden = false;
                            enemies[i].hp = 5;
                            enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
                            enemies[i].y = 80;
                        }
                        enemy_count = MAX_ENEMY;
                        pause = 0;
                        y = 0;
                    }
            }
            if(enemy_count < 3 && wave == 10) {
                for(i = 0; i < MAX_ENEMY; i++) {
                        enemies[i].hidden = false;
                        enemies[i].hp = 5;
                        enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
                        enemies[i].y = 80;
                    }
                enemy_count = MAX_ENEMY;
            }
    if(p2_count <= 0) {
        FILE *f = fopen("highscore.txt", "w+");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        temp = atoi(highscore);
        if(temp < score) {
            fprintf(f, "%d", score);
        } else {
            fprintf(f, "%d", temp);
        }
        fclose(f);
        temp = score;
        score = 0;
        wave = 1;
        al_stop_sample(&boss_bgm_id);
        if(bgm == 1)
            if (!al_play_sample(lose, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &lose_id));
        game_change_scene(SCENE_OVER);
        p2_count = 2;
    }
}

void game_draw(void) {
	if (active_scene == SCENE_MENU) {
        FILE *f = fopen("highscore.txt", "r");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        fgets(highscore, sizeof(highscore), f);
        for(int i = 0; i < MAX_ENEMY; i++) {
            enemies[i].hidden = false;
        }
		al_draw_bitmap(main_img_background, 0, 0, 0);
		al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2, 30, ALLEGRO_ALIGN_CENTER, "Space Shooter");
		al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 20, SCREEN_H - 50, 0, "Press enter key to start");
		al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 0), 20, SCREEN_H - 100, 0, "HIGHSCORE : %d", atoi(highscore));

		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
			al_draw_bitmap(img_settings2, SCREEN_W - 48, 10, 0);
		else
			al_draw_bitmap(img_settings, SCREEN_W - 48, 10, 0);
        fclose(f);

	} else if (active_scene == SCENE_START) {
		int i, j;
		al_draw_bitmap(start_img_background, 0, 0, 0);
        al_draw_textf(font_pirulen_24, al_map_rgb(255, 255, 255), 593, 20, ALLEGRO_ALIGN_CENTER, "SCORE : %05d", score);
        al_draw_textf(font_pirulen_24, al_map_rgb(255, 255, 255), 565, 50, ALLEGRO_ALIGN_CENTER, "WAVE : %02d", wave);
        if(p2 == 1) {
            al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 70, 35, ALLEGRO_ALIGN_CENTER, "PLANE");
            al_draw_rectangle(10, 10, 160, 30, al_map_rgb(0, 0, 0), 4);
            al_draw_filled_rectangle(10, 10, 160, 30, al_map_rgb(128, 128, 128));
            al_draw_filled_rectangle(10, 10, (16*plane.hp), 30, al_map_rgb(255, 0, 0));
            al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 10, 100, 0, "PLANE 2");
            al_draw_rectangle(10, 75, 320, 95, al_map_rgb(0, 0, 0), 4);
            al_draw_filled_rectangle(10, 75, 320, 95, al_map_rgb(128, 128, 128));
            al_draw_filled_rectangle(10, 75, (16*plane2.hp), 95, al_map_rgb(255, 0, 0));
        } else {
            al_draw_text(font_pirulen_24, al_map_rgb(255, 255, 255), 70, 35, ALLEGRO_ALIGN_CENTER, "PLANE");
            if(pesawat == 1) {
                al_draw_rectangle(10, 10, 160, 30, al_map_rgb(0, 0, 0), 4);
                al_draw_filled_rectangle(10, 10, 160, 30, al_map_rgb(128, 128, 128));
                al_draw_filled_rectangle(10, 10, (16*plane.hp), 30, al_map_rgb(255, 0, 0));
            }
            else if(pesawat2 == 1) {
                al_draw_rectangle(10, 10, 320, 30, al_map_rgb(0, 0, 0), 4);
                al_draw_filled_rectangle(10, 10, 320, 30, al_map_rgb(128, 128, 128));
                al_draw_filled_rectangle(10, 10, (16*plane2.hp), 30, al_map_rgb(255, 0, 0));
            }
        }


        if(pesawat == 1) {
            draw_movable_object(plane);
            for (i = 0; i < MAX_BULLET; i++) {
                draw_movable_object(bullets[i]);
                for(j = 0; j < MAX_ENEMY; j++) {
                        if(bullets[i].x < enemies[j].x + 18 && bullets[i].x > enemies[j].x - 18 && bullets[i].y < enemies[j].y + 34 && bullets[i].y > enemies[j].y - 34) {
                            bullets[i].x = enemies[j].x;
                            bullets[i].y = enemies[j].y;
                            if(game_update_timer) {
                                al_draw_bitmap_region(img_explosion[lastFrame], 0, 0, 64, 64, bullets[i].x, bullets[i].y, 0);
                                lastFrame++;
                            }
                            if(lastFrame == 23) {
                                lastFrame = 0;
                                bullets[i].x = -100;
                                bullets[i].y = -100;
                            }
                        }
                }
                if(wave == 10) {
                    if(bullets[i].x < boss.x + 230 && bullets[i].x > boss.x - 80 && bullets[i].y < boss.y + 125 && bullets[i].y > boss.y - 50) {
                        bullets[i].x = boss.x;
                        bullets[i].y = boss.y;
                        if(game_update_timer) {
                            al_draw_bitmap_region(img_explosion[lastFrame], 0, 0, 64, 64, bullets[i].x, bullets[i].y, 0);
                            lastFrame++;
                        }
                        if(lastFrame == 23) {
                            lastFrame = 0;
                            bullets[i].x = -100;
                            bullets[i].y = -100;
                        }
                    }
                }
            }
        }
        if(pesawat2 == 1) {
            draw_movable_object(plane2);
            for (i = 0; i < MAX_BULLET; i++) {
                draw_movable_object(bullets2[i]);
                for(j = 0; j < MAX_ENEMY; j++) {
                        if(bullets2[i].x < enemies[j].x + 18 && bullets2[i].x > enemies[j].x - 18 && bullets2[i].y < enemies[j].y + 34 && bullets2[i].y > enemies[j].y - 34) {
                            bullets2[i].x = enemies[j].x;
                            bullets2[i].y = enemies[j].y;
                            if(game_update_timer) {
                                al_draw_bitmap_region(img_explosion[lastFrame], 0, 0, 64, 64, bullets2[i].x, bullets2[i].y, 0);
                                lastFrame++;
                            }
                            if(lastFrame == 23) {
                                lastFrame = 0;
                                bullets2[i].x = -100;
                                bullets2[i].y = -100;
                            }
                        }
                }
                if(wave == 10) {
                    if(bullets2[i].x < boss.x + 230 && bullets2[i].x > boss.x - 80 && bullets2[i].y < boss.y + 125 && bullets2[i].y > boss.y - 50) {
                        bullets2[i].x = boss.x;
                        bullets2[i].y = boss.y;
                        if(game_update_timer) {
                            al_draw_bitmap_region(img_explosion[lastFrame], 0, 0, 64, 64, bullets2[i].x, bullets2[i].y, 0);
                            lastFrame++;
                        }
                        if(lastFrame == 23) {
                            lastFrame = 0;
                            bullets2[i].x = -100;
                            bullets2[i].y = -100;
                        }
                    }
                }
            }
        }
        if(wave == 10) {
            draw_movable_object(boss);
            for(i = 0; i < MAX_BULLET; i++) {
                draw_movable_object(boss_bullets[i]);
            }
            for(i = 0; i < MAX_BULLET; i++) {
                draw_movable_object(boss_bullets2[i]);
            }
        }
		for (i = 0; i < MAX_ENEMY; i++) {
			draw_movable_object(enemies[i]);
			for(j = 0; j < MAX_BULLET; j++)
                draw_movable_object(enemy_bullets[i][j]);
		}

		if (pnt_in_rect(mouse_x, mouse_y, SCREEN_W - 48, 10, 38, 38))
			al_draw_bitmap(img_pause2, SCREEN_W - 48, 10, 0);
		else
			al_draw_bitmap(img_pause, SCREEN_W - 48, 10, 0);

        if(pause == 1 && rpause == 1) {
            if(pnt_in_rect(mouse_x, mouse_y, 225, 175, 375, 50)) {
                al_draw_rectangle(225, 175, 600, 225, al_map_rgb(255, 255, 255), 3 );
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 240 , 180, 0, "BACK TO MENU");
            } else {
                al_draw_rectangle(225, 175, 600, 225, al_map_rgb(0, 0, 255), 3 );
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 240 , 180, 0, "BACK TO MENU");
            }

            if(pnt_in_rect(mouse_x, mouse_y, 300, 275, 215, 50)) {
                al_draw_rectangle(300, 275, 515, 325, al_map_rgb(255, 255, 255), 3 );
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 315 , 280, 0, "RESUME");
            } else {
                al_draw_rectangle(300, 275, 515, 325, al_map_rgb(0, 0, 255), 3 );
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 315 , 280, 0, "RESUME");
            }

            if(pnt_in_rect(mouse_x, mouse_y, 335, 375, 130, 50)) {
                al_draw_rectangle(335, 375, 465, 425, al_map_rgb(255, 255, 255), 3 );
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), SCREEN_W / 2 - 50, 380, 0, "QUIT");
            } else {
                al_draw_rectangle(335, 375, 465, 425, al_map_rgb(0, 0, 255), 3 );
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), SCREEN_W / 2 - 50, 380, 0, "QUIT");
            }
        }

	} else if (active_scene == SCENE_SETTINGS) {
	    al_draw_bitmap(img_settings_bg2, 0, 0, 0);
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 220 , 150, 0, "BGM : ");
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 220 , 200, 0, "SFX : ");
        al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 220 , 300, 0, "2P MODE : ");

        if(bgm == 1) {
            if(pnt_in_rect(mouse_x, mouse_y, 380 , 155, 65, 25))
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 380 , 150, 0, "ON");
            else
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 380 , 150, 0, "ON");
        } else if(bgm == 0) {
            if(pnt_in_rect(mouse_x, mouse_y, 380 , 155, 83, 25))
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 380 , 150, 0, "OFF");
            else
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 380 , 150, 0, "OFF");
        }
        if(sfx == 1) {
            if(pnt_in_rect(mouse_x, mouse_y, 380 , 205, 65, 25))
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 380 , 200, 0, "ON");
            else
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 380 , 200, 0, "ON");
        } else if(sfx == 0) {
            if(pnt_in_rect(mouse_x, mouse_y, 380 , 205, 83, 25))
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 380 , 200, 0, "OFF");
            else
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 380 , 200, 0, "OFF");
        }
        if(p2 == 1) {
            if(pnt_in_rect(mouse_x, mouse_y, 480 , 305, 65, 25))
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 480 , 300, 0, "ON");
            else
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 480 , 300, 0, "ON");
        } else if(p2 == 0) {
            if(pnt_in_rect(mouse_x, mouse_y, 480 , 305, 83, 25))
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 480 , 300, 0, "OFF");
            else
                al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 480 , 300, 0, "OFF");
        }

        if(pnt_in_rect(mouse_x, mouse_y, 225, 255, 150, 28))
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 225 , 250, 0, "PLANE");
        else
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 225 , 250, 0, "PLANE");

        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 75, SCREEN_H - 175, 150, 50)) {
            al_draw_rectangle(SCREEN_W / 2 - 75, SCREEN_H - 175, SCREEN_W/2 + 75, SCREEN_H - 125, al_map_rgb(255, 255, 255), 3 );
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 340 , 430, 0, "BACK");
        } else {
            al_draw_rectangle(SCREEN_W / 2 - 75, SCREEN_H - 175, SCREEN_W/2 + 75, SCREEN_H - 125, al_map_rgb(0, 0, 255), 3 );
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 340 , 430, 0, "BACK");
        }
	}


	else if (active_scene == SCENE_OVER) {
        al_draw_bitmap(img_game_over_bg, 0, 0, 0);
        al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 0), SCREEN_W / 2 , 355, ALLEGRO_ALIGN_CENTER, "YOUR SCORE : %d", temp);
        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 75, SCREEN_H - 175, 150, 50)) {
            al_draw_rectangle(SCREEN_W / 2 - 75, SCREEN_H - 175, SCREEN_W/2 + 75, SCREEN_H - 125, al_map_rgb(255, 255, 255), 3 );
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 340 , 430, 0, "BACK");
        } else {
            al_draw_rectangle(SCREEN_W / 2 - 75, SCREEN_H - 175, SCREEN_W/2 + 75, SCREEN_H - 125, al_map_rgb(0, 0, 255), 3 );
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 340 , 430, 0, "BACK");
        }
	}
	else if (active_scene == SCENE_WIN) {
        al_draw_bitmap(img_game_win_bg, 0, 0, 0);

        al_draw_textf(font_pirulen_32, al_map_rgb(255, 255, 0), SCREEN_W / 2 , 340, ALLEGRO_ALIGN_CENTER, "YOUR SCORE : %d", temp);
        if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 75, SCREEN_H - 175, 150, 50)) {
            al_draw_rectangle(SCREEN_W / 2 - 75, SCREEN_H - 175, SCREEN_W/2 + 75, SCREEN_H - 125, al_map_rgb(255, 255, 255), 3 );
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 340 , 430, 0, "BACK");
        } else {
            al_draw_rectangle(SCREEN_W / 2 - 75, SCREEN_H - 175, SCREEN_W/2 + 75, SCREEN_H - 125, al_map_rgb(0, 0, 255), 3 );
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 340 , 430, 0, "BACK");
        }
	}

	else if (active_scene == SCENE_PLANE) {
        al_draw_bitmap(img_settings_bg, 0, 0, 0);
		al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 200 , 100, 0, "PICK YOUR PLANE");
		if(pesawat == 1) {
            al_draw_bitmap(start_img_plane, 243, 270, 0);
            al_draw_rectangle(200, 230, 340, 380, al_map_rgb(255, 0, 0), 3 );
        }  else {
            al_draw_bitmap(start_img_plane, 243, 270, 0);
            al_draw_rectangle(200, 230, 340, 380, al_map_rgb(255, 255, 255), 3 );
        }
        if(pnt_in_rect(mouse_x, mouse_y, 200, 230, 140, 150)) {
            al_draw_bitmap(start_img_plane, 243, 270, 0);
            al_draw_rectangle(200, 230, 340, 380, al_map_rgb(255, 255, 0), 3 );
        }


        if(pesawat2 == 1) {
            al_draw_bitmap(start_img_plane2, 503, 290, 0);
            al_draw_rectangle(460, 230, 600, 380, al_map_rgb(255, 0, 0), 3 );
        } else {
            al_draw_bitmap(start_img_plane2, 503, 290, 0);
            al_draw_rectangle(460, 230, 600, 380, al_map_rgb(255, 255, 255), 3 );
        }
        if(pnt_in_rect(mouse_x, mouse_y, 460, 230, 140, 150)){
            al_draw_bitmap(start_img_plane2, 503, 290, 0);
            al_draw_rectangle(460, 230, 600, 380, al_map_rgb(255, 255, 0), 3 );
        }


		if(pnt_in_rect(mouse_x, mouse_y, SCREEN_W / 2 - 45, SCREEN_H - 125, 90, 50)) {
            al_draw_rectangle(SCREEN_W / 2 - 45, SCREEN_H - 125, SCREEN_W/2 + 45, SCREEN_H - 75, al_map_rgb(255, 255, 255), 3 );
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 0), 370 , 480, 0, "OK");
        } else {
            al_draw_rectangle(SCREEN_W / 2 - 45, SCREEN_H - 125, SCREEN_W/2 + 45, SCREEN_H - 75, al_map_rgb(0, 0, 255), 3 );
            al_draw_text(font_pirulen_32, al_map_rgb(255, 255, 255), 370 , 480, 0, "OK");
        }
	}
	al_flip_display();
}

void game_destroy(void) {
	// Destroy everything you have created.
	// Free the memories allocated by malloc or allegro functions.
	// Destroy shared resources.
	al_destroy_font(font_pirulen_32);
	al_destroy_font(font_pirulen_24);

	/* Menu Scene resources*/
	al_destroy_bitmap(main_img_background);
	al_destroy_sample(main_bgm);
	al_destroy_sample(boss_bgm);
	al_destroy_sample(laser);
	al_destroy_sample(start_bgm);
	al_destroy_sample(lose);
	al_destroy_sample(win);
	al_destroy_sample(click);
	al_destroy_sample(settings_bgm);
	al_destroy_sample(plane_bgm);
	// [HACKATHON 3-6]
	// TODO: Destroy the 2 settings images.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_settings);
	al_destroy_bitmap(img_settings2);

	/* Start Scene resources*/
	al_destroy_bitmap(start_img_background);
	al_destroy_bitmap(img_game_over_bg);
	al_destroy_bitmap(img_settings_bg);
	al_destroy_bitmap(img_settings_bg2);
	al_destroy_bitmap(img_game_win_bg);
	al_destroy_bitmap(img_boss);
	al_destroy_bitmap(start_img_plane);
	al_destroy_bitmap(start_img_plane2);
	al_destroy_bitmap(start_img_enemy);
	// [HACKATHON 2-9]
	// TODO: Destroy your bullet image.
	// Uncomment and fill in the code below.
	al_destroy_bitmap(img_bullet);
	al_destroy_bitmap(img_bullet2);
	al_destroy_bitmap(img_bullet_enemy);
	al_destroy_bitmap(img_boss_bullet);
	al_destroy_bitmap(img_boss_bullet2);

	al_destroy_timer(game_update_timer);
	al_destroy_event_queue(game_event_queue);
	al_destroy_display(game_display);
	free(mouse_state);
}

void game_change_scene(int next_scene) {
	game_log("Change scene from %d to %d", active_scene, next_scene);
	// TODO: Destroy resources initialized when creating scene.
	if (active_scene == SCENE_MENU) {
		al_stop_sample(&main_bgm_id);
		al_stop_sample(&boss_bgm_id);
		enemy_count = 0;
		score = 0;
		song = 0;
        pause = 1;
        rpause = 0;
        rstart = 0;
        wave = 0;
        plane.hp = 10;
        plane2.hp = 20;
        p2_count = 2;
        if(p2 == 1) {
            pesawat = 1;
            pesawat2 = 1;
        }
		game_log("stop audio (bgm)");
	} else if (active_scene == SCENE_START) {
		al_stop_sample(&start_bgm_id);
		song++;
		game_log("stop audio (bgm)");
	} else if (active_scene == SCENE_SETTINGS) {
        al_stop_sample(&settings_bgm_id);
	} else if (active_scene == SCENE_PLANE) {
        al_stop_sample(&plane_bgm_id);
	}
	active_scene = next_scene;
	// TODO: Allocate resources before entering scene.
	if (active_scene == SCENE_MENU) {
		if(bgm == 1)
            if (!al_play_sample(main_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &main_bgm_id))
                game_log("failed to play audio (bgm)");
	} else if (active_scene == SCENE_START) {
		int i, j;
		rstart = 1;
		if(pesawat == 1) {
            plane.img = start_img_plane;
            plane.x = 400;
            plane.y = 500;
            plane.w = 51;
            plane.h = 76;
            plane.hp = 10;

            for (i = 0; i < MAX_BULLET; i++) {
                bullets[i].w = 5;
                bullets[i].h = 40;
                bullets[i].img = img_bullet;
                bullets[i].vx = 0;
                bullets[i].vy = -20;
                bullets[i].hidden = true;
            }
		}
		if(pesawat2 == 1) {
            plane2.img = start_img_plane2;
            plane2.x = 400;
            plane2.y = 500;
            plane2.w = 51;
            plane2.h = 47;
            plane2.hp = 20;

            for (i = 0; i < MAX_BULLET; i++) {
                bullets2[i].w = 5;
                bullets2[i].h = 40;
                bullets2[i].img = img_bullet2;
                bullets2[i].vx = 0;
                bullets2[i].vy = -5;
                bullets2[i].hidden = true;
            }
		}

            boss.img = img_boss;
            boss.x = SCREEN_W / 2;
            boss.y = 100;
            boss.w = 150;
            boss.h = 158;
            boss.vx = 1;
            boss.hp = 100;
            for (i = 0; i < MAX_BULLET; i++) {
                    boss_bullets[i].w = 15;
                    boss_bullets[i].h = 61;
                    boss_bullets[i].img = img_boss_bullet;
                    boss_bullets[i].vx = 0;
                    boss_bullets[i].vy = 2;
                    boss_bullets[i].hidden = true;
                    boss_bullets2[i].w = 15;
                    boss_bullets2[i].h = 61;
                    boss_bullets2[i].img = img_boss_bullet2;
                    boss_bullets2[i].vx = 0;
                    boss_bullets2[i].vy = 2;
                    boss_bullets2[i].hidden = true;
            }




		for (i = 0; i < MAX_ENEMY; i++) {
			enemies[i].w = 28;
			enemies[i].h = 68;
			enemies[i].x = enemies[i].w / 2 + (float)rand() / RAND_MAX * (SCREEN_W - enemies[i].w);
			enemies[i].y = 80;
			enemies[i].img = start_img_enemy;
			enemies[i].vx = 1;
			enemies[i].vy = 1;
			enemies[i].hp = 5;
		}

		for (i = 0; i < MAX_ENEMY; i++) {
            for (j = 0; j < MAX_BULLET; j++) {
                enemy_bullets[i][j].w = 2;
                enemy_bullets[i][j].h = 20;
                enemy_bullets[i][j].img = img_bullet_enemy;
                enemy_bullets[i][j].vx = 0;
                enemy_bullets[i][j].vy = 1;
                enemy_bullets[i][j].hidden = true;
            }
        }
        if(bgm == 1)
            if (!al_play_sample(start_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &start_bgm_id))
                game_log("failed to play audio (bgm)");

	} else if(active_scene == SCENE_SETTINGS) {
        if(bgm == 1)
            if(!al_play_sample(settings_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &settings_bgm_id))
                game_log("failed to play audio (bgm)");

	} else if(active_scene == SCENE_PLANE) {
        if(bgm == 1)
            if(!al_play_sample(plane_bgm, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &plane_bgm_id))
                game_log("failed to play audio (bgm)");
	}


}

void on_key_down(int keycode) {
	if (active_scene == SCENE_MENU) {
		if (keycode == ALLEGRO_KEY_ENTER)
			game_change_scene(SCENE_START);
	}
}

void explosive(int x, int y)
{

    //al_draw_bitmap(img_explosion, x - 15, y - 15, explosion);
    //al_draw_scaled_bitmap(img_explosion, 0 , 0 , 60, 60, x - (60 - explosion) / 2, y - (60 - explosion) / 2, 40, 40 , 0);
}

void on_mouse_down(int btn, int x, int y) {
	// [HACKATHON 3-8]
	// TODO: When settings clicked, switch to settings scene.
	// Uncomment and fill in the code below.
	if (active_scene == SCENE_MENU) {
		if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38)) {
				if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
				game_change_scene(3);
			}
		}
	}
	if (active_scene == SCENE_START) {
        if (btn == 1) {
			if (pnt_in_rect(x, y, SCREEN_W - 48, 10, 38, 38)) {
                if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
				pause = 1;
				rpause = 1;
			}
            if(pause == 1 && rpause == 1) {
                if (pnt_in_rect(x, y, 225, 175, 375, 50)) {
                    if(sfx == 1)
                        al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    game_change_scene(1);
                }

                if (pnt_in_rect(x, y, 300, 275, 215, 50)) {
                    if(sfx == 1)
                        al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    pause = 0;
                    rpause = 0;
                }

                if (pnt_in_rect(x, y, 335, 375, 130, 50)) {
                    if(sfx == 1)
                        al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    exit(1);
                }
            }

		}
	}

    if (active_scene == SCENE_SETTINGS) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, SCREEN_W / 2 - 75, SCREEN_H - 175, 150, 50)) {
                if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                game_change_scene(1);
            }
            if (pnt_in_rect(x, y, 225, 255, 150, 28)) {
                if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                game_change_scene(6);
            }
            if(bgm == 0) {
                if (pnt_in_rect(x, y, 380, 155, 83, 25)) {
                    if(sfx == 1)
                        al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    bgm = 1;
                }
            } else if(bgm == 1) {
                if (pnt_in_rect(x, y, 380, 155, 65, 25)) {
                    if(sfx == 1)
                        al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    bgm = 0;
                }
            }

            if(sfx == 0) {
                if (pnt_in_rect(x, y, 380, 205, 83, 25)) {
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    sfx = 1;
                }
            } else if(sfx == 1) {
                if (pnt_in_rect(x, y, 380, 205, 65, 25)) {
                    if(sfx == 1)
                        al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    sfx = 0;
                }
            }

            if(p2 == 0) {
                if (pnt_in_rect(x, y, 480 , 305, 83, 25)) {
                    if(sfx == 1)
                        al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    p2 = 1;
                    pesawat = 1;
                    pesawat2 = 1;
                }
            } else if(p2 == 1) {
                if (pnt_in_rect(x, y, 480 , 305, 65, 25)) {
                    if(sfx == 1)
                        al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                    p2 = 0;
                    pesawat = 1;
                    pesawat2 = 0;
                }
            }


        }
    }


	if (active_scene == SCENE_OVER) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, SCREEN_W / 2 - 75, SCREEN_H - 175, 150, 50)) {
                if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                game_change_scene(1);
            }
        }
	}

	if (active_scene == SCENE_WIN) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, SCREEN_W / 2 - 75, SCREEN_H - 175, 150, 50)) {
                if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                game_change_scene(1);
            }
        }
	}

	if (active_scene == SCENE_PLANE) {
        if (btn == 1) {
            if (pnt_in_rect(x, y, SCREEN_W / 2 - 45, SCREEN_H - 125, 150, 50)) {
                if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                game_change_scene(3);
            }
            if (pnt_in_rect(x, y, 200, 230, 140, 150)) {
                if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                pesawat = 1;
                pesawat2 = 0;
            }
            if (pnt_in_rect(x, y, 460, 230, 140, 150)) {
                if(sfx == 1)
                    al_play_sample(click, 1, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &click_id);
                pesawat = 0;
                pesawat2 = 1;
            }
        }
	}

}

void draw_movable_object(MovableObject obj) {
	if (obj.hidden)
		return;
	al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}

ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
	ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
	if (!loaded_bmp)
		game_abort("failed to load image: %s", filename);
	ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
	ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

	if (!resized_bmp)
		game_abort("failed to create bitmap when creating resized image: %s", filename);
	al_set_target_bitmap(resized_bmp);
	al_draw_scaled_bitmap(loaded_bmp, 0, 0,
		al_get_bitmap_width(loaded_bmp),
		al_get_bitmap_height(loaded_bmp),
		0, 0, w, h, 0);
	al_set_target_bitmap(prev_target);
	al_destroy_bitmap(loaded_bmp);

	game_log("resized image: %s", filename);

	return resized_bmp;
}

// [HACKATHON 3-3]
// TODO: Define bool pnt_in_rect(int px, int py, int x, int y, int w, int h)
// Uncomment and fill in the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h) {
	return ((x+w) >= px && px >=x) && ((y+h) >= py && py >= y);
}

// +=================================================================+
// | Code below is for debugging purpose, it's fine to remove it.    |
// | Deleting the code below and removing all calls to the functions |
// | doesn't affect the game.                                        |
// +=================================================================+

void game_abort(const char* format, ...) {
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
	fprintf(stderr, "error occured, exiting after 2 secs");
	// Wait 2 secs before exiting.
	al_rest(2);
	// Force exit program.
	exit(1);
}

void game_log(const char* format, ...) {
#ifdef LOG_ENABLED
	va_list arg;
	va_start(arg, format);
	game_vlog(format, arg);
	va_end(arg);
#endif
}

void game_vlog(const char* format, va_list arg) {
#ifdef LOG_ENABLED
	static bool clear_file = true;
	vprintf(format, arg);
	printf("\n");
	// Write log to file for later debugging.
	FILE* pFile = fopen("log.txt", clear_file ? "w" : "a");
	if (pFile) {
		vfprintf(pFile, format, arg);
		fprintf(pFile, "\n");
		fclose(pFile);
	}
	clear_file = false;
#endif
}
