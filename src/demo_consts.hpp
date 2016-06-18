#pragma once

// Macros
#define DEMO_T() (Demo::singleton().getTime(bps, -start))

// Error values
#define ERR_SYNC  -1
#define ERR_AUDIO -2

// Video
#define DEMO_W 320.0
#define DEMO_H 240.0

#define DEMO_POST_SIZE_MULT 2

// Misc
#define DEMO_BPS (86.97f/60.0f)
#define DEMO_MUSIC_FILE "data/audio/music.wav"
#define DEMO_PI 3.14159265

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
#define SYNC_PART_VAPOR1_PILLAR_XSEP        19
#define SYNC_PART_LOGO_STARFIELD            20
#define SYNC_PART_LOGO_FLARE1_OPACITY       21
#define SYNC_PART_LOGO_FLARE2_OPACITY       22
#define SYNC_PART_LOGO_FLARE3_OPACITY       23
#define SYNC_PART_LOGO_PYRAMID_Y            24
#define SYNC_PART_LOGO_PYRAMID_RECURSION    25
#define SYNC_PART_LOGO_PYRAMID_EXPLOSION    26
#define SYNC_PART_VAPOR1_SPEED              27
