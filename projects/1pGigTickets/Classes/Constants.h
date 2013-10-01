#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define kFieldWidth 8
#define kFieldHeight 8

#define kGemTypeAmount 6

#define kTileSize 80.0f
#define kSwapTime 0.1f
#define kFallTime 0.1f
#define kDestructionTime 0.1f

#define kBonusChance 0.0f

#define zGem 2
#define zTile 3
#define zLighting 90
#define zMatchScore 100

#define kAITurnDelay 0.5f
#define kTipDelay 1.f

#define kTransformationTime 0.3f

#define kColumnsFallDelay 0
#define kRowsFallDelay 0

#define kPlayersAmount 2

#define kSpawnBonuses 1

const bool kPreloadField = true;

enum GemColour {
	GC_Random,
	GC_Guitar,
	GC_Keyboard,
	GC_Microphone,
	GC_Saxophone,
	GC_Plectrum,
    GC_Question,
	GC_Note
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
    GS_AboutToDestroyByNote,
    GS_AboutToTurnIntoBomb,
    GS_AboutToExplodeWithCross,
	GS_Immovable
};

enum GemType {
	GT_Colour,
	GT_Cross,
	GT_LineHor,
	GT_LineVer,
	GT_Explosion,
    GT_NoteMaker
};

enum Direction {
	D_Down,
	D_Up,
	D_Left,
	D_Right,
};

enum MatchType {
    MT_Horizontal,
    MT_Vertical,
    MT_None
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
    FS_SwappingNoteWithNormalIcon,
    FS_SwappingTwoFourInRowIcons,
    FS_SwappingNoteWithFourInRowIcon,
    FS_DestroyingFourInRowIcons,
};

#if(kSpawnBonuses == 1)
    const GemType kVerticalMatchFourBonus = GT_Explosion;
    const GemType kHorizontalMatchFourBonus = GT_Explosion;
    const GemType kVerticalMatchFiveBonus = GT_NoteMaker;
    const GemType kHorizontalMatchFiveBonus = GT_NoteMaker;
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