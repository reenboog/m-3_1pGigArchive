//
//  TicketScreen.cpp
//  1pGigTickets
//
//  Created by Alex Gievsky on 15.10.13.
//
//

#include "TicketScreen.h"
#include "GameConfig.h"
#include "GameScene.h"

#define kTicketCellWidth 596
#define kTicketCellHeight 201
#define kTicketCellBorderHeight 0

TicketScreen::~TicketScreen() {
}

TicketScreen::TicketScreen(): Layer() {
    back = nullptr;
    listBtn = nullptr;
    plectrumsLabel = nullptr;
    shareBtn = nullptr;
    
    itemWidth = kTicketCellWidth;
    itemHeight = kTicketCellHeight;
}

#pragma mark - init

Scene* TicketScreen::scene() {
    Scene *scene = Scene::create();
    
    TicketScreen *layer = TicketScreen::create();
    scene->addChild(layer);
    
    return scene;
}

bool TicketScreen::init() {
    if(!Layer::init()) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    back = Sprite::create("ticketBack.png");
    back->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    this->addChild(back, 0);
    
    float listMenuDisp = 0.96;
    float plectrumsDisp = 0.953;
    
    int tableHeight = 800;
    int tableDisp = 40;
    
    float shareMenuDisp = 0.857;
    
    if(visibleSize.height == 1136) {
        listMenuDisp = 0.95;
        plectrumsDisp = 0.957;
        shareMenuDisp = 0.869;
        
        tableHeight = 935;
        tableDisp = 30;
    }

    // list btn
    listBtn = MenuItemImage::create("tickets/ticketListBtn.png", "tickets/ticketListBtnOn.png", "tickets/ticketListBtnOn.png", CC_CALLBACK_1(TicketScreen::onListBtnPressed, this));
    listBtn->setPosition({0, 0});
    
    Menu *listMenu = Menu::create(listBtn, NULL);
    listMenu->setPosition({visibleSize.width * 0.063, visibleSize.height * listMenuDisp});
    
    this->addChild(listMenu);
    
    // plectrums
    
    Sprite *plectrumsMount = Sprite::create("tickets/ticketPlectrums.png");
    plectrumsMount->setPosition({visibleSize.width * 0.88, visibleSize.height * plectrumsDisp});
    this->addChild(plectrumsMount);
    
    plectrumsLabel = LabelBMFont::create("0", "allerWhiteScores.fnt");
    
    String *plectrums = String::createWithFormat("%i", GameConfig::sharedInstance()->currentPlectrums);

    plectrumsLabel->setString(plectrums->getCString());
    plectrumsLabel->setPosition({plectrumsMount->getContentSize().width * 0.5, plectrumsMount->getContentSize().height * 0.5});
    plectrumsLabel->setScale(0.95);
    plectrumsMount->addChild(plectrumsLabel);

    // table view
    
    testTableCells[0] = 0;
    testTableCells[1] = 0;
    testTableCells[2] = 1;
    testTableCells[3] = 0;
    testTableCells[4] = 2;
    
    numberOfItems = sizeof(testTableCells) / sizeof(int);
    
    tableView = TableView::create(this, {itemWidth, tableHeight});
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setPosition(Point((visibleSize.width - itemWidth) / 2.0, tableDisp));
	tableView->setDelegate(this);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);

	this->addChild(tableView);
	
    tableView->reloadData();


    // share button
    shareBtn = MenuItemImage::create("tickets/ticketShareToWinBtn.png", "tickets/ticketShareToWinBtnOn.png",
                                     "tickets/ticketListBtnOn.png", CC_CALLBACK_1(TicketScreen::onShareBtnPressed, this));
    listBtn->setPosition({0, 0});
    
    Menu *shareMenu = Menu::create(shareBtn, NULL);
    shareMenu->setPosition({visibleSize.width * 0.5, visibleSize.height * shareMenuDisp});
    
    this->addChild(shareMenu);
    
    populateTickets();
    
    return true;
}

void TicketScreen::populateTickets() {
    
}

#pragma mark - callbacks

void TicketScreen::onListBtnPressed(Object *sender) {
    CCLOG("list btn pressed");
}

void TicketScreen::onShareBtnPressed(Object *sender) {
    CCLOG("share btn pressed");
}

#pragma mark - table view

void TicketScreen::tableCellTouched(TableView* table, TableViewCell* cell) {
    CCLOG("cell touched at index: %i", cell->getIdx());
    
    Director::getInstance()->replaceScene(TransitionFade::create(0.3, GameScene::scene()));
}

Size TicketScreen::tableCellSizeForIndex(TableView *table, unsigned int idx) {
	return Size(itemWidth, itemHeight + kTicketCellBorderHeight);
}

TableViewCell* TicketScreen::tableCellAtIndex(TableView *table, unsigned int idx) {
    String *string = String::createWithFormat("%d", idx);
	
	TableViewCell *cell = table->cellAtIndex(idx);
    if(!cell) {
		int index = testTableCells[idx];
        
		cell = new TicketTableViewCell(index);
        cell->autorelease();
    } else {
//        LabelTTF *label = (LabelTTF*)cell->getChildByTag(123);
//        label->setString(string->getCString());
        1 / 0;
    }
    
	return cell;
}

unsigned int TicketScreen::numberOfCellsInTableView(TableView *table) {
	//int n = GlobalConfig::getInstance().getNumberOfEquipmentWithParameters();
	return numberOfItems;
}

#pragma mark - custom table cells

TicketTableViewCell::~TicketTableViewCell() {
    
}

TicketTableViewCell::TicketTableViewCell(int ticketId) {
    this->ticketId = ticketId;
    
    string fileName = "";
    
    switch(ticketId) {
        case 0:
            fileName = "tickets/ticketNormal.png";
            break;
        case 1:
            fileName = "tickets/ticketGold.png";
            break;
        case 2:
            fileName = "tickets/ticketUnavailable.png";
            break;
    }
    
    background = Sprite::create(fileName.c_str());
    background->setPosition(Point::ZERO);
	background->setAnchorPoint(Point::ZERO);
    
    this->addChild(background);
}


