#ifndef __GEMFIELD__
#define __GEMFIELD__

#include "Constants.h"
#include "cocos2d.h"
#include "FieldWatcherDelegate.h"

class Gem;
class Player;

USING_NS_CC;

struct Match {
	Match(int fromX, int fromY, int toX, int toY, GemColour colour) {
		this->fromX = fromX;
		this->fromY = fromY;
		this->toX = toX;
		this->toY = toY;
		this->length = (toX - fromX) + (toY - fromY) + 1;
		this->colour = colour;
	}

	Match();

	int fromX;
	int fromY;
	int toX;
	int toY;
	int length;
	GemColour colour;
};

typedef std::list<Match> MatchList;

struct Move {
	Move(int fromX, int fromY, int toX, int toY, MatchList matches) {
		this->fromX = fromX;
		this->fromY = fromY;
		this->toX = toX;
		this->toY = toY;

		this->matches = matches;
		this->legal = !matches.empty();
	}

	Move() { this->legal = false; }

	bool legal;

	int fromX;
	int fromY;
	int toX;
	int toY;

	MatchList matches;
};

typedef std::list<Move> MoveList;

class GemField : public Node {

public:
	GemField();
	~GemField();

	void addWatcher(FieldWatcherDelegate *watcher);

	void print();
	void printMask();

	void swipeAction(Point startCoordinates, int direction);
	void clickAction(Point clickCoordinates);
	
	MoveList getMoves();
	void showTip();
private:
	MatchList findMatches();
	MatchList findMatchesInLine(int fromX, int fromY, int toX, int toY);

	void selectGem(int x, int y);
	void deselectGem(int x, int y);

	void swapGems(int fromX, int fromY, int toX, int toY);	
	void swapGemsIndices(int fromX, int fromY, int toX, int toY);
	void moveGem(int fromX, int fromY, int toX, int toY, int rowsToWait = 0);

	void freezeGem(int x, int y, int power);
	void removeGem(int x, int y);

	void destroyGem(int x, int y);
	void destroyLine(int fromX, int fromY, int toX, int toY);

	bool hasAnyMatches();

	void resolveMatches(); 
	void resolveMatch(Match match);

	void refillLine(int lineNumber, int direction = D_Down, bool reset = true);
	void refillField(bool reset = true);

	bool gemsAreMoving();
	
    void update(float dt);
    void visit();
    
	void resetField();


	void addMoveToList(Move move, MoveList &list);
	bool checkAvailableMoves();
	void shuffleField(bool reset = true);
private:
	int fieldMask[kFieldHeight][kFieldWidth];
	int freezeMask[kFieldHeight][kFieldWidth];
	Gem *fieldGems[kFieldHeight][kFieldWidth];
	
	Point *selectedGemCoordinates;

	GamePhase gamePhase;

	int columnsWithMatches;

	MoveList getMovesForLine(int fromX, int fromY, int toX, int toY);
	Move getGemMove(int x, int y, Direction direction);

	bool gemsHaveSameColour(int firstX, int firstY, int secondX, int secondY);


	FieldWatcherDelegatePool watchers; 
};



#endif // __GEMFIELD__
