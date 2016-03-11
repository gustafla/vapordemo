#pragma once

// Macros
#define DEMO_T() (Demo::singleton().getTime())

// Error values
#define ERR_SYNC  -1
#define ERR_AUDIO -2

// Video
#define DEMO_W 320
#define DEMO_H 240

#define DEMO_POST_SIZE_MULT 2

// Misc
#define DEMO_BPS 1 //Later!
#define DEMO_MUSIC_FILE "data/audio/music.wav"
#define DEMO_PI 3.14159265

// Palette
#define DEMO_C_BLACK vec3(0.0,0.0,0.0)
#define DEMO_C_WHITE vec3(1.0,1.0,1.0)
#define DEMO_C_VPINK vec3(0.9,0.2,0.4)
#define DEMO_C_DBLUE vec3(0.1,0.2,1.0)
#define DEMO_C_LBLUE vec3(0.6,0.65,1.0)

// Sync channel names
#define SYNC_PART               			0
#define SYNC_BRIGHTNESS         			1
#define SYNC_CONTRAST           			2
#define SYNC_BLUR               			3
#define SYNC_GLITCHINESS                    4

#define SYNC_PART_TRIANGLES_VIEW_X			5
#define SYNC_PART_TRIANGLES_VIEW_Y			6
#define SYNC_PART_TRIANGLES_MAIN_ROTATION	7
#define SYNC_PART_TRIANGLES_TUNNEL_DEPTH    8
#define SYNC_PART_TRIANGLES_TUNNEL_DEPTHPAN 9
#define SYNC_PART_TRIANGLES_TUNNEL_DEPTHROT 10

#define SYNC_PART_LOGO_VIEW_X               11
#define SYNC_PART_LOGO_VIEW_Y               12
#define SYNC_PART_LOGO_VIEW_Z               13
#define SYNC_PART_LOGO_VIEW_RX              14
#define SYNC_PART_LOGO_VIEW_RY              15
#define SYNC_PART_LOGO_VIEW_RZ              16
#define SYNC_PART_LOGO_X                    17
#define SYNC_PART_LOGO_X_SEPARATION         18
