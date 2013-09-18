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

#define kPlayersAmount 2

#define kEquipmentSlotsAmount 5

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

//enum GamePhase {
//	GP_Ready,
//	GP_Waiting,
//	GP_Moving,
//	GP_Searching,
//	GP_Destroying,
//	GP_Refilling,
//	GP_Shuffling,
//};

#define SafeDelete(p) {if(p) {delete p; p = NULL;}}

#endif // __CONSTANTS_H__