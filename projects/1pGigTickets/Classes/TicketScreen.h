//
//  TicketScreen.h
//  1pGigTickets
//
//  Created by Alex Gievsky on 15.10.13.
//
//

#ifndef ___pGigTickets__TicketScreen__
#define ___pGigTickets__TicketScreen__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

using std::string;

class TicketScreen: public Layer, public TableViewDataSource, public TableViewDelegate {
public:
    ~TicketScreen();
    TicketScreen();
    // cocos2d stuff
    virtual bool init();
    CREATE_FUNC(TicketScreen);
    
    // table delegates
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, unsigned int idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, unsigned int idx);
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::TableView *table);
    
    static cocos2d::Scene * scene();
    
    // callbacks
    void onListBtnPressed(Object *sender);
    void onShareBtnPressed(Object *sender);
    
    void populateTickets();
private:
    Sprite *back;
    MenuItem *listBtn;
    LabelBMFont *plectrumsLabel;
    MenuItem *shareBtn;
    
    // table
    int numberOfItems;
    
    int itemWidth;
	int itemHeight;
    
	TableView *tableView;
    // test cells
    int testTableCells[5];
};

// table cell
class TicketTableViewCell: public TableViewCell
{
public:
	TicketTableViewCell(int ticketId);
	virtual ~TicketTableViewCell();
private:
	Sprite *background;
    int ticketId;
};


#endif /* defined(___pGigTickets__TicketScreen__) */
