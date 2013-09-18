#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define kFieldWidth 8
#define kFieldHeight 8

#define kGemTypeAmount 6

#define kTileSize 78.0f
#define kSwapTime 0.1f
#define kFallTime 0.1f
#define kDestructionTime 0.1f

#define kBonusChance 0.05f

#define kZOrderGem 2
#define kZOrderTile 3

#define kAITurnDelay 0.5f
#define kTipDelay 1.f

#define kTransformationTime 0.25f

#define kPlayersAmount 2

#define kSpawnBonuses 0

const bool kPreloadField = false;

enum GemColour {
	GC_Random,
	GC_Guitar,
	GC_Keyboard,
	GC_Microphone,
	GC_Saxophone,
	GC_Plectrum,
    GC_Question,
	GC_Wild
};

enum GemState {
	GS_Idle,
	GS_Moving,
	GS_Moved,
	GS_Selected,
	GS_Matched,
	GS_Transforming,
	GS_Transformed,
	GS_Destroying,
	GS_Destroyed,
	GS_Immovable
};

enum GemType {
	GT_Colour,
	GT_Cross,
	GT_LineHor,
	GT_LineVer,
	GT_Explosion
};

enum Direction {
	D_Down,
	D_Up,
	D_Left,
	D_Right,
};

enum FieldState {
	FS_Ready,
	FS_Waiting,
	FS_Moving,
	FS_Searching,
	FS_Transforming,
	FS_Destroying,
	FS_Refilling,
	FS_Shuffling,
};

#if(kSpawnBonuses == 1)
    const GemType kVerticalMatchFourBonus = GT_LineVer;
    const GemType kHorizontalMatchFourBonus = GT_LineHor;
    const GemType kVerticalMatchFiveBonus = GT_LineVer;
    const GemType kHorizontalMatchFiveBonus = GT_LineHor;
    const GemType kCrossMatchBonus = GT_Cross;
#else
    const GemType kVerticalMatchFourBonus = GT_Colour;
    const GemType kHorizontalMatchFourBonus = GT_Colour;
    const GemType kVerticalMatchFiveBonus = GT_Colour;
    const GemType kHorizontalMatchFiveBonus = GT_Colour;
    const GemType kCrossMatchBonus = GT_Colour;
#endif

#define SafeDelete(p) {if(p) {delete p; p = NULL;}}

#endif // __CONSTANTS_H__