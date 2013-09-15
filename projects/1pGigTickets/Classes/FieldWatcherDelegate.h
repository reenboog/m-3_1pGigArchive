#ifndef __FIELD_WATCHER_H__
#define __FIELD_WATCHER_H__

#include "Constants.h"
#include <list>


class FieldWatcherDelegate {
public:
	virtual void onMatch(int length, GemColour colour) = 0;
	virtual void onGemDestroyed(GemColour colour) = 0;
	virtual void onFieldShuffleStarted() = 0;
	virtual void onMovementStarted() = 0;
	virtual void onMovementEnded() = 0;
	virtual void onMove(bool legal) = 0;
};

typedef std::list<FieldWatcherDelegate *> FieldWatcherDelegatePool;


#endif // __FIELD_WATCHER_H__