#include "lang.h"
#pragma once

#include "util.h"
#include "bsptypes.h"

// if lightmap extent exceeds 16, the map will not be able to load in 'Software' renderer and HLDS.
// #define MAX_SURFACE_EXTENT  64
// max pixels in a single lightmap
#define MAX_LUXELS 1600

#define MAX_SINGLEMAP ((MAX_SURFACE_EXTENT+1)*(MAX_SURFACE_EXTENT+1))
// sky or slime or null, no lightmap or 256 subdivision

#define TEX_SPECIAL				1U << ( 0 )	// sky or slime, no lightmap or 256 subdivision

//XASH
#define TEX_WORLD_LUXELS		1U << ( 1 )	// alternative lightmap matrix will be used (luxels per world units instead of luxels per texels)
#define TEX_AXIAL_LUXELS		1U << ( 2 )	// force world luxels to axial positive scales
#define TEX_EXTRA_LIGHTMAP		1U << ( 3 )	// bsp31 legacy - using 8 texels per luxel instead of 16 texels per luxel
#define TEX_SCROLL				1U << ( 6 )	// Doom special FX


#define assume(exp, message) {if (!(exp)) {print_log(get_localized_string(LANG_0001),#exp,__FILE__,__LINE__,message); }}
#define hlassume(exp, message) {if (!(exp)) {print_log(get_localized_string(LANG_1016),#exp,__FILE__,__LINE__,#message); }}

#define qmax(a,b)            (((a) > (b)) ? (a) : (b)) // changed 'max' to 'qmax'. --vluzacn
#define qmin(a,b)            (((a) < (b)) ? (a) : (b)) // changed 'min' to 'qmin'. --vluzacn

// HLCSG_HLBSP_DOUBLEPLANE: We could use smaller epsilon for hlcsg and hlbsp (hlcsg and hlbsp use double as float), which will totally eliminate all epsilon errors. But we choose this big epsilon to tolerate the imprecision caused by Hammer. Basically, this is a balance between precision and flexibility.

//
// Vector Math
//

#define DotProduct(x,y) ( (x)[0] * (y)[0] + (x)[1] * (y)[1]  +  (x)[2] * (y)[2])
#define CrossProduct(a, b, dest) \
{ \
    (dest)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1]; \
    (dest)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2]; \
    (dest)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0]; \
}
#define VectorSubtract(a,b,c)    { (c)[0]=(a)[0]-(b)[0]; (c)[1]=(a)[1]-(b)[1]; (c)[2]=(a)[2]-(b)[2]; }
#define VectorAdd(a,b,c)         { (c)[0]=(a)[0]+(b)[0]; (c)[1]=(a)[1]+(b)[1]; (c)[2]=(a)[2]+(b)[2]; }
#define VectorScale(a,b,c)       { (c)[0]=(a)[0]*(b);(c)[1]=(a)[1]*(b);(c)[2]=(a)[2]*(b); }
#define VectorCopy(a,b) { (b)[0]=(a)[0]; (b)[1]=(a)[1]; (b)[2]=(a)[2]; }
#define VectorMA(a, scale, b, dest) { (dest)[0] = (a)[0] + (scale) * (b)[0]; (dest)[1] = (a)[1] + (scale) * (b)[1];  (dest)[2] = (a)[2] + (scale) * (b)[2]; }

struct BSPEDGE32;
struct BSPTEXTUREINFO;
struct BSPPLANE;
struct BSPFACE32;
class Winding;

typedef enum
{
	LightOutside,                                          // Not lit
	LightShifted,                                          // used HuntForWorld on 100% dark face
	LightShiftedInside,                                    // moved to neighbhor on 2nd cleanup pass
	LightNormal,                                           // Normally lit with no movement
	LightPulledInside,                                     // Pulled inside by bleed code adjustments
	LightSimpleNudge,                                      // A simple nudge 1/3 or 2/3 towards center along S or T axist
}
light_flag_t;

typedef enum
{
	plane_x = 0,
	plane_y,
	plane_z,
	plane_anyx,
	plane_anyy,
	plane_anyz
}
planetypes;

typedef struct
{
	int             texmins[2], texsize[2];
	int             surfnum;
	BSPFACE32* face;
}
lightinfo_t;

typedef struct
{
	BSPPLANE planes[4];
}
samplefragrect_t;

typedef struct samplefrag_s
{
	int facenum; // facenum
	samplefragrect_t rect; // original rectangle that forms the boundary
	Winding* mywinding; // relative to the texture coordinate on that face

	samplefrag_s()
	{
		facenum = 0;
		rect = samplefragrect_t();
		mywinding = NULL;
	}
}
samplefrag_t;

// for a single face
struct LIGHTMAP
{
	int width, height;
	int layers; // for when multiple lights hit the same face (nStyles[0-3] != 255)
	int face;
	LIGHTMAP()
	{
		width = height = layers = 0;
		face = -1;
	}
};


float CalculatePointVecsProduct(const volatile float* point, const volatile float* vecs);
bool CanFindFacePosition(Bsp* bsp, int facenum, int imins[2], int imaxs[2]);