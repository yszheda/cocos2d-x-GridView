#ifndef __GRIDVIEWTESTSCENE_H__
#define __GRIDVIEWTESTSCENE_H__

#include "cocos2d.h"
#include "GridView.h"

void runGridViewTest();

class GridViewTestLayer : public cocos2d::Layer, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate
{
public:
    virtual bool init();  
   
	void toExtensionsMainLayer(cocos2d::Ref *sender);

    CREATE_FUNC(GridViewTestLayer);
    
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) {};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view) {}
    virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
    virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
};

#endif // __GRIDVIEWTESTSCENE_H__
