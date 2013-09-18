
#include "GemField.h"
#include "ccMacros.h"
#include <random>

#include "Gem.h"

GemField::GemField() {

	std::srand(time(NULL));

	//this->watchers = FieldWatcherDelegatePool();

	const int mask[kFieldHeight][kFieldWidth] = {
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1},
        {1,1,1,1,1,1,1,1}
	};

	std::memcpy(fieldMask, mask, sizeof(mask));
    
    const int freezers[kFieldHeight][kFieldWidth] = {
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
	};
	std::memcpy(freezeMask, freezers, sizeof(mask));

	for(int y = 0; y < kFieldHeight; y++) {
		for(int x = 0; x < kFieldWidth; x++) {
			fieldGems[y][x] = NULL;
		}
	}

	// Generate gems on appropriate tiles
	bool gotMatches = true;
	while(gotMatches)
	{
		for(int y = 0; y < kFieldHeight; y++) {
			for(int x = 0; x < kFieldWidth; x++) {
				if(fieldMask[y][x] == 1) {
					if(fieldGems[y][x] != NULL) {
						fieldGems[y][x]->reset(x, y);
					}

					fieldGems[y][x] = new Gem();
				} else {
					fieldGems[y][x] = NULL;
				}
			}
		}
		gotMatches = hasAnyMatches();
	}
	
	for(int y = 0; y < kFieldHeight; y++) {
		for(int x = 0; x < kFieldWidth; x++) {
			if(fieldMask[y][x] == 1) {
				this->addChild(fieldGems[y][x]);
				fieldGems[y][x]->init(x, y, fieldGems[y][x]->getGemColour(), fieldGems[y][x]->getType());
			} else {
//				//Fill holes with tiles
//				Sprite* tile = Sprite::create("tile.png");
//				tile->setZOrder(kZOrderTile);
//				tile->setPosition(Gem::convertCoordinatesToPixels(x, y));
//				this->addChild(tile);
			}
		}
	}

//	// Surround gameField with tiles
//	int borderWidth = 3;
//	for(int i = -borderWidth; i < kFieldHeight + borderWidth; i++) {
//		for(int j = -borderWidth; j < kFieldWidth + borderWidth; j++) {
//			if(i < 0 || i >= kFieldHeight || j < 0 || j >= kFieldWidth) {
//				Sprite* tile = Sprite::create("tile.png");
//				tile->setZOrder(kZOrderTile);
//				tile->setPosition(Gem::convertCoordinatesToPixels(j, i));
//				this->addChild(tile);
//			}
//		}
//	}

	selectedGemCoordinates = NULL;
	scheduleUpdate();

	shuffleField(false);
}

GemField::~GemField() {
	CCLOG("GemField destructor");

	SafeDelete(selectedGemCoordinates);
}

void GemField::print() {
	cocos2d::String* str = cocos2d::String::create("");
	for(int y = 1; y <= kFieldHeight; y++) {
		str->_string = "";
		for(int x = 1; x <= kFieldWidth; x++) {
			if(fieldGems[y][x]->getState() == GS_Destroyed) {
				str->appendWithFormat("%c ", "x");
			} else {
				str->appendWithFormat("%i ", fieldGems[y][x]->getType());
			}
		}
		CCLOG("%s", str->getCString());
	}
	CCLOG("\n----------------\n");
}

// Returns true if there is at least one match on the field
bool GemField::hasAnyMatches() {
	MatchList foundMatches = findMatches();
	return !foundMatches.empty();
}

// Finds and resolves all matches
void GemField::resolveMatches() {
	MatchList foundMatches = findMatches();

	// First resolve all the matches
	for(MatchList::iterator it = foundMatches.begin(); it != foundMatches.end(); it++) {
		resolveMatch(*it);
	}

	// Then mark them all as destroyed 
	for(MatchList::iterator it = foundMatches.begin(); it != foundMatches.end(); it++) {
		destroyLine((*it).fromX, (*it).fromY, (*it).toX, (*it).toY);
	}
}

void GemField::resolveMatch(Match match) {
	for(FieldWatcherDelegatePool::iterator it = watchers.begin(); it != watchers.end(); it++) {
		(*it)->onMatch(match.length, match.colour);
	}

	int stepX = 1;
	int stepY = 1;

	if(match.fromX == match.toX) {
		stepX = 0;
	} else {
		stepY = 0;
	}

	int x = match.fromX;
	int y = match.fromY;

	while(y <= match.toY && x <= match.toX) {
		if(fieldGems[y][x]->getState() != GS_Matched) {
			fieldGems[y][x]->match();
		} else {
			CCLOG("OMFG IT'S A CROSS! OR AN L !!! OR WHATEVER !!!!! ");
		}
		y += stepY;
		x += stepX;
	}
}

// Destroys a line of gems (should work for diagonal ones as well)
void GemField::destroyLine(int fromX, int fromY, int toX, int toY) {
	int stepX = 1;
	int stepY = 1;
	if(fromX == toX) {
		stepX = 0;
	}
	if(fromY == toY) {
		stepY = 0;
	}
	//CCLOG("Destroy line %i, %i - %i", fromX, fromY, toY);
	int y = fromY;
	int x = fromX;
	while(y <= toY && x <= toX) {
		destroyGem(x, y);
		y += stepY;
		x += stepX;
	}
}

void GemField::destroyGem(int x, int y) {
	if(fieldMask[y][x] == 1)
	{
		if(fieldGems[y][x]->getState() != GS_Destroying) {
			if(freezeMask[y][x] > 1) {
				freezeGem(y, x, freezeMask[y][x]-1);
			} else {
				if(freezeMask[y][x] == 1) {
					freezeGem(y, x, freezeMask[y][x]-1);
				}
				if(fieldGems[y][x]->getType() != GT_Colour) {
					switch (fieldGems[y][x]->getType())
					{
					case GT_Cross: 
						fieldGems[y][x]->destroy(); 
						destroyLine(x, 0, x, kFieldHeight - 1); 
						destroyLine(0, y, kFieldWidth - 1, y); 
						break;
					case GT_LineHor : 
						fieldGems[y][x]->destroy(); 
						destroyLine(0, y, kFieldWidth - 1, y); 
						break;
					case GT_LineVer :
						fieldGems[y][x]->destroy();
						destroyLine(x, 0, x, kFieldHeight - 1); 
					default:
						fieldGems[y][x]->destroy(); 
						break;
					}
				}
				if(fieldGems[y][x]->getState() != GS_Matched) {
					for(FieldWatcherDelegatePool::iterator it = watchers.begin(); it != watchers.end(); it++) {
						(*it)->onGemDestroyed(fieldGems[y][x]->getGemColour());
					}
				}
				fieldGems[y][x]->destroy();
			}
		}
	}
}

void GemField::selectGem(int x, int y) {	
	if(selectedGemCoordinates != NULL) {
		delete(selectedGemCoordinates);
	}
	selectedGemCoordinates = new Point(x, y);
	fieldGems[y][x]->select();
}

void GemField::deselectGem(int x, int y) {
	delete(selectedGemCoordinates);
	selectedGemCoordinates = NULL;
	fieldGems[y][x]->deselect();
}


MatchList GemField::findMatches() {
	MatchList foundMatches;
	for(int y = 0; y < kFieldHeight; y++) {
		foundMatches.splice(foundMatches.begin(), findMatchesInLine(0, y, kFieldWidth - 1, y));
	}
	for(int x = 0; x < kFieldWidth; x++) {
		foundMatches.splice(foundMatches.begin(), findMatchesInLine(x, 0, x, kFieldHeight - 1));
	}
	return foundMatches;
}

MatchList GemField::findMatchesInLine(int fromX, int fromY, int toX, int toY) {
	MatchList foundMatches;
	
	int stepX = 1;
	int stepY = 1;

	if(fromX == toX) {
		stepX = 0;
	}
	if(fromY == toY) {
		stepY = 0;
	}

	int x = fromX;
	int y = fromY;

	while(fieldMask[y][x] == 0) {
		x += stepX;
		y += stepY;
	}

	if(x > toX || y > toY) {
		return foundMatches;
	}

	int currentValue = fieldGems[y][x]->getGemColour();
	int chainLength = 1;

	x += stepX;
	y += stepY;

	while(x <= toX && y <= toY) {
		while ((x <= toX && y <= toY) && fieldMask[y][x] == 1 && freezeMask[y][x] <= 1 && (fieldGems[y][x]->getGemColour() == currentValue || (fieldGems[y][x]->getGemColour() == GC_Wild  && chainLength >= 2) || currentValue == GC_Wild))
		{
			x += stepX;
			y += stepY;
			chainLength++;
		}
		if(chainLength >= 3) {
			if(fromX == toX) {
				foundMatches.push_front(Match(x, y - chainLength, x, y - 1, fieldGems[y - 1][x]->getGemColour()));
			} else {
				foundMatches.push_front(Match(x - chainLength, y, x - 1, y, fieldGems[y][x - 1]->getGemColour()));
			}
		}

		if(x > toX || y > toY) {
			return foundMatches;
		}

		while(fieldMask[y][x] == 0)
		{
			x += stepX;
			y += stepY;
		}

		if(x > toX || y > toY) {
			return foundMatches;
		}

		currentValue = fieldGems[y][x]->getGemColour();
		chainLength = 1;
		x += stepX;
		y += stepY;
	}

	return foundMatches;
}


void GemField::swapGems(int fromX, int fromY, int toX, int toY) {
	Gem *first;
	Gem *second;

	//Pointers for gems we will swap
	first = fieldGems[fromY][fromX];
	second = fieldGems[toY][toX];

	swapGemsIndices(fromX, fromY, toX, toY);

	bool hasMatches = hasAnyMatches();

	//Change gems' positions - if there is no match, they move forward then backward
	first->swapTo(toX, toY, !hasMatches);
	second->swapTo(fromX, fromY, !hasMatches);

	// If there was no match swap indices again
	if(!hasMatches) {
		swapGemsIndices(fromX, fromY, toX, toY);
	}

	for(FieldWatcherDelegatePool::iterator it = watchers.begin(); it != watchers.end(); it++) {
		(*it)->onMove(hasMatches);
		(*it)->onMovementStarted();
	}
	state = FS_Moving;
}

void GemField::swapGemsIndices(int fromX, int fromY, int toX, int toY) {
	Gem *tmp;
	//Swap gems in the array
	tmp = fieldGems[fromY][fromX];
	fieldGems[fromY][fromX] = fieldGems[toY][toX];
	fieldGems[toY][toX] = tmp;
}

void GemField::refillField(bool reset) {
	columnsWithMatches = 0;
	for(int x = 0; x < kFieldWidth; x++) {
		refillLine(x, D_Down, reset);
	}
}

void GemField::swipeAction(Point startCoordinates, int direction) {
	int fromX = startCoordinates.x;
	int fromY = startCoordinates.y;
	
	int toX = fromX;
	int toY = fromY;

	if(fromX >=0 && fromX < kFieldWidth && fromY >= 0 && fromY < kFieldHeight) {
		switch (direction)
		{
			case D_Up : 
				if(fromY > 0) {
					toY -= 1;
				}
				break;
			case D_Down :
				if(fromY < kFieldHeight - 1) {
					toY += 1;
				}
				break;
			case D_Left :
				if(fromX > 0) {
					toX -= 1;
				}
				break;
			case D_Right :
				if(fromX < kFieldWidth - 1) {
					toX += 1;
				}
				break;
			default:
				break;
		}
	}
	if((toX - fromX) + (toY - fromY) != 0) {
		if(selectedGemCoordinates != NULL) {
			deselectGem(selectedGemCoordinates->x, selectedGemCoordinates->y);
		}
		if(fieldMask[fromY][fromX] == 1 && freezeMask[fromY][fromX] == 0 && fieldMask[toY][toX] == 1  && freezeMask[toY][toX] == 0) {
			swapGems(fromX, fromY, toX, toY);
		}
	}
}

void GemField::clickAction(Point clickCoordinates) {
	int toX = clickCoordinates.x;
	int toY = clickCoordinates.y;
	if(toX >= 0 && toX < kFieldWidth && toY >= 0 && toY < kFieldHeight) {
		if(fieldMask[toY][toX] == 1 && freezeMask[toY][toX] == 0) 
		{
			if(selectedGemCoordinates == NULL) {
				selectGem(toX, toY);
				//CCLOG("Selected %i, %i", toX, toY); 
			} else {
				int selectedX = selectedGemCoordinates->x;
				int selectedY = selectedGemCoordinates->y;
				// Same gem
				if(selectedGemCoordinates->getDistance(clickCoordinates) == 0)
				{
					//CCLOG("Deselected %f, %f", selectedGemCoordinates->x, selectedGemCoordinates->y); 
					deselectGem(selectedX, selectedY);
				} else {
				// Different gems 
					// Nearby gem
					if(selectedGemCoordinates->getDistance(clickCoordinates) == 1) {
						if(toX >= 0 && toX < kFieldWidth && toY >= 0 && toY < kFieldHeight) {
							//CCLOG("Swap %f, %f with %f, %f", selectedGemCoordinates->x, selectedGemCoordinates->y, clickCoordinates.x, clickCoordinates.y); 
							deselectGem(selectedX, selectedY);
							swapGems(selectedX, selectedY, toX, toY);
						}
					} else {
					// Faraway gem
						//CCLOG("Deselected %f, %f - selected %f, %f", selectedGemCoordinates->x, selectedGemCoordinates->y, clickCoordinates.x, clickCoordinates.y); 
						deselectGem(selectedX, selectedY);
						selectGem(toX, toY);
					}
				}
			}
		}
	}
}

bool GemField::gemsAreMoving() {
	for(int y = 0; y < kFieldHeight; y++) {
		for(int x = 0; x < kFieldWidth; x++) {
			if(fieldMask[y][x] == 1) {
				if(fieldGems[y][x]->getState() == GS_Moving || fieldGems[y][x]->getState() == GS_Destroying) {
					return true;
				}
			}
		}
	}
	return false;
}

void GemField::visit() {
    glEnable(GL_SCISSOR_TEST);
    
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Point pos = this->getPosition();
    int x = 0, y = 0;
    
    y = abs(pos.y - kTileSize * kFieldHeight / 4);

    glScissor(x, y, visibleSize.width, kTileSize * (kFieldHeight + 2.2));

    Node::visit();
    
    glDisable(GL_SCISSOR_TEST);
}

void GemField::update(float dt)
{
	switch(state) {
		case FS_Ready:
			if(checkAvailableMoves()) {
				for(FieldWatcherDelegatePool::iterator it = watchers.begin(); it != watchers.end(); it++) {
					(*it)->onMovementEnded();
					state = FS_Waiting;
				}
			} else {
				shuffleField(true);
				for(FieldWatcherDelegatePool::iterator it = watchers.begin(); it != watchers.end(); it++) {
					(*it)->onFieldShuffleStarted();
				}
			}
			break;
		case FS_Moving:
			if(!gemsAreMoving()) {
				//CCLOG("-GP_Moving");
				state = FS_Searching;
			}
			break;
		case FS_Searching:
			//CCLOG("-GP_Searching");
			if(hasAnyMatches()) {
				// If we do - find and resolve them
				resolveMatches();	
				state = FS_Destroying;
			} else {
				state = FS_Ready;
			}
			break;
		case FS_Destroying:
			if(!gemsAreMoving()) {
				//CCLOG("-GP_Destroying");
				refillField();
				state = FS_Refilling;
			}
			break;
		case FS_Refilling:
			if(!gemsAreMoving()) {
				//CCLOG("-GP_Refilling");
				state = FS_Searching;
			}
			break;
		case FS_Shuffling:
			if(!gemsAreMoving()) {
				//CCLOG("-GP_Shuffling");
				state = FS_Searching;
			}
			break;
		default:
			break;
	}
}

void GemField::refillLine(int lineNumber, int direction, bool reset) {
	if(direction == D_Down) {
		int y = kFieldHeight - 1;
		int destroyedGems = 0;
		int emptyBlocks = 0;
		int timesToFall = 0;
		int distanceBetweenEmpty = 0;
		int lastDistance = 0;


		// Until we reach the top of the line 
		while(y >= 0) {
			// If there is a gem
			if(fieldMask[y][lineNumber] == 1 && fieldGems[y][lineNumber]->getState() != GS_Immovable) {
				// If a gem is destroyed - account for it
				if(fieldGems[y][lineNumber]->getState() == GS_Destroyed) {
					distanceBetweenEmpty++;
					destroyedGems++;			
				} else {
				// If the gem is not destroyed - move it down
					distanceBetweenEmpty++;
					moveGem(lineNumber, y, lineNumber, y + destroyedGems + emptyBlocks, columnsWithMatches);
		
					if(emptyBlocks > 0) {
						timesToFall--;
						while(timesToFall <= 0 && emptyBlocks > 0) {
							emptyBlocks--;
							timesToFall = lastDistance;
						}
					}
				}
			} else {
			// If there is no gem - count that as a non-existing block		
				if(destroyedGems > 0) {
					if(timesToFall > 0) {
						emptyBlocks++;
					} else {
						timesToFall = destroyedGems;
						emptyBlocks = 1;
					}
					lastDistance = distanceBetweenEmpty;
					distanceBetweenEmpty = 0;
				}
			}
			y--;
		}
		y += destroyedGems + emptyBlocks;
		int gemsFallen = 0;
		while(y >= 0) {
			if(fieldMask[y][lineNumber] == 1 && fieldGems[y][lineNumber]->getState() != GS_Immovable) {
				GemType type;
				GemColour colour;
				if(reset) {				
					if(CCRANDOM_0_1() < kBonusChance) {
						type = (GemType) ((int) (CCRANDOM_0_1() * 3));
						colour = GC_Random;
					} else {
						type = GT_Colour;
						colour = GC_Random;
					}
				} else {
					type = fieldGems[y][lineNumber]->getType();
					colour = fieldGems[y][lineNumber]->getGemColour();
				}
				fieldGems[y][lineNumber]->reset(lineNumber, y - destroyedGems - emptyBlocks, colour, type);
				fieldGems[y][lineNumber]->fallTo(lineNumber, y, gemsFallen, columnsWithMatches);
				gemsFallen++;
				if(emptyBlocks > 0) {
					timesToFall--;
					if(timesToFall <= 0) {
						emptyBlocks--;
						y--;
						timesToFall = lastDistance;
					}
				}	
			} else {
				emptyBlocks--;
			}
			y--;
		}
		if(destroyedGems > 0) {
			columnsWithMatches++;
		}
	}
	
}

void GemField::moveGem(int fromX, int fromY, int toX, int toY, int rowsToWait) {
	if(fromX != toX || fromY != toY)
	{
		swapGemsIndices(fromX, fromY, toX, toY);
		fieldGems[toY][toX]->fallTo(toX, toY, 0, rowsToWait);
	}
}

// Freezes a gem in place. It acts like a hole
// - if power == 1 - cant'be moved, may be matched with others or destroyed by explosion
// - if power == 2 - must be hit by explosion in order to become matchable
void GemField::freezeGem(int x, int y, int power) {
	freezeMask[y][x] = power;
	fieldGems[y][x]->setFreeze(power);
}

MoveList GemField::getMovesForLine(int fromX, int fromY, int toX, int toY) {
	MoveList availableMoves;

	int stepX = 1;
	int stepY = 1;

	if(fromX == toX) {
		stepX = 0;
		//CCLOG("Checking line %i", fromX);
	}
	if(fromY == toY) {
		stepY = 0;
		//CCLOG("Checking line %i", fromY);
	}

	int x = fromX;
	int y = fromY;

	while(fieldMask[y][x] == 0) {
		x += stepX;
		y += stepY;
	}

	if(x > toX || y > toY) {
		return availableMoves;
	}

	int currentValue = fieldGems[y][x]->getGemColour();
	int chainLength = 1;

	x += stepX;
	y += stepY;


	while(x <= toX + 1 && y <= toY + 1) {
		while ((x <= toX && y <= toY) && fieldMask[y][x] == 1 && freezeMask[y][x] <= 1 && (fieldGems[y][x]->getGemColour() == currentValue || (fieldGems[y][x]->getGemColour() == GC_Wild  && chainLength >= 2) || currentValue == GC_Wild))
		{
			x += stepX;
			y += stepY;
			chainLength++;
		}
		//CCLOG("Stopped at %i,%i with length %i", x, y, chainLength);
		

		// Find turns for 3s with gaps
		if(chainLength == 1) {
			if(stepY == 0) {
				if(x - 3 >= fromX) {
					if(gemsHaveSameColour(x - 1, y, x - 3, y)) {
						addMoveToList(getGemMove(x - 2, y, D_Up), availableMoves);
						addMoveToList(getGemMove(x - 2, y, D_Down), availableMoves);
					}
				}
			} else {
				if(y - 3 >= fromY) {
					if(gemsHaveSameColour(x, y - 1, x, y - 3)) {
						addMoveToList(getGemMove(x, y - 2, D_Left), availableMoves);
						addMoveToList(getGemMove(x, y - 2, D_Right), availableMoves);
					}
				}
			}
		}
		
		// Find couples and look for 3s, 4s, 5s
		if(chainLength == 2) {
			// If we're checking a horizontal line
			if(stepY == 0) {
				// If there is a gem to the left of the pair
				if(x - 3 >= fromX) {
					addMoveToList(getGemMove(x - 3, y, D_Up), availableMoves); 
					addMoveToList(getGemMove(x - 3, y, D_Down), availableMoves);
					addMoveToList(getGemMove(x - 3, y, D_Left), availableMoves);
				}
				// If there is a gem to the right of the pair
				if(x <= toX) {
					addMoveToList(getGemMove(x, y, D_Up), availableMoves); 
					addMoveToList(getGemMove(x, y, D_Down), availableMoves); 
					addMoveToList(getGemMove(x, y, D_Right), availableMoves);
				}
			} else {
			// If we're checking a vertical line	
				// If there is a gem above the couple
				if(y - 3 >= fromY) {
					addMoveToList(getGemMove(x, y - 3, D_Left), availableMoves);
					addMoveToList(getGemMove(x, y - 3, D_Right), availableMoves); 
					addMoveToList(getGemMove(x, y - 3, D_Up), availableMoves); 
				}
				// If there is a gem below the couple
				if(y <= toY) {
					addMoveToList(getGemMove(x, y, D_Left), availableMoves);
					addMoveToList(getGemMove(x, y, D_Right), availableMoves);
					addMoveToList(getGemMove(x, y, D_Down), availableMoves); 
				}
			}
		}
		
		if(x > toX || y > toY) {
			return  availableMoves;
		}

		while(fieldMask[y][x] == 0)
		{
			x += stepX;
			y += stepY;
		}

		if(x > toX || y > toY) {
			return  availableMoves;
		}

		currentValue = fieldGems[y][x]->getGemColour();
		chainLength = 1;
		x += stepX;
		y += stepY;
	}
	return  availableMoves;
}

void GemField::addMoveToList(Move move, MoveList &list) {
	if(move.legal) {
		list.push_front(move);
	}
}

Move GemField::getGemMove(int x, int y, Direction direction) {
	int stepX = 0;
	int stepY = 0;
	switch (direction)
	{
		case D_Down: stepY = 1;
			break;
		case D_Up: stepY = -1;
			break;
		case D_Left: stepX = -1;
			break;
		case D_Right: stepX = 1;
			break;
		default:
			break;
	}
	if(x >= 0 && x < kFieldWidth && y >=0 && y < kFieldHeight && x + stepX >= 0 && x + stepX < kFieldWidth && y + stepY >=0 && y + stepY < kFieldHeight) {
		if(fieldMask[y + stepY][x + stepX] == 1 && fieldMask[y][x] == 1 && freezeMask[y + stepY][x + stepX] == 0 && freezeMask[y][x] == 0) {
			swapGemsIndices(x, y, x + stepX, y + stepY);
		
			MatchList foundMatches = findMatches();
			swapGemsIndices(x, y, x + stepX, y + stepY);
			return Move(x, y, x + stepX, y + stepY, foundMatches);
		}
	}
	return Move();
}

bool GemField::gemsHaveSameColour(int firstX, int firstY, int secondX, int secondY) {
	if(firstX >= 0 && firstX < kFieldWidth && firstY >= 0 && firstY < kFieldHeight && secondX >= 0 && secondX < kFieldWidth && secondY >= 0 && secondY < kFieldHeight) {
		return fieldMask[firstY][firstX] == 1 && fieldMask[secondY][secondX] == 1 && fieldGems[firstY][firstX]->getGemColour() == fieldGems[secondY][secondX]->getGemColour();
	}
	return false;
}



void GemField::resetField() {	
	bool gotMatches = true;
	bool hasTurns = false;
	while(gotMatches || !hasTurns) {
		for(int x = 0; x < kFieldWidth; x++) {
			for(int y = kFieldHeight - 1; y >= 0; y--) {
				if(fieldMask[y][x] == 1) {
					fieldGems[y][x]->reset(x, y);
					removeGem(x, y);
				}
			} 
		}
		gotMatches = hasAnyMatches();
		hasTurns = checkAvailableMoves();
	}
}

void GemField::shuffleField(bool reset) {
	state = FS_Shuffling;
	if(reset) {
		resetField();
	} else {
		for(int x = 0; x < kFieldWidth; x++) {
			for(int y = kFieldHeight - 1; y >= 0; y--) {
				if(fieldMask[y][x] == 1) {
					fieldGems[y][x]->reset(x, y, fieldGems[y][x]->getGemColour(), fieldGems[y][x]->getType());
					removeGem(x, y);
				}
			} 
		}
	}
	refillField(false);
}


void GemField::removeGem(int x, int y) {
	if(fieldMask[y][x] == 1) {
		fieldGems[y][x]->remove();
	}
}


MoveList GemField::getMoves() {
	MoveList availableMoves;
	for(int y = 0; y < kFieldHeight; y++) {
		availableMoves.splice(availableMoves.begin(), getMovesForLine(0, y, kFieldWidth - 1, y));
	}
	for(int x = 0; x < kFieldWidth; x++) {
		availableMoves.splice(availableMoves.begin(), getMovesForLine(x, 0, x, kFieldHeight - 1));
	}
	return availableMoves;
}

void GemField::showTip() {
	MoveList moves = getMoves();

	int moveNumber = moves.size() / 2;
	MoveList::iterator it = moves.begin();
	while(moveNumber > 0) {
		it++;
		moveNumber--;
	}

	fieldGems[(*it).fromY][(*it).fromX]->swapTo((*it).toX, (*it).toY, true);
	fieldGems[(*it).toY][(*it).toX]->swapTo((*it).fromX, (*it).fromY, true);
}

bool GemField::checkAvailableMoves() {
	MoveList availableMoves = getMoves();
	return !availableMoves.empty();
}

void GemField::addWatcher(FieldWatcherDelegate *watcher) {
	watchers.push_front(watcher);
}