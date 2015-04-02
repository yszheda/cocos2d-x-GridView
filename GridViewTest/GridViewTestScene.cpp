#include "GridViewTestScene.h"
// Use the following header if you put the test folder under {cocos2d-x root}/tests/cpp-tests/Classes/ExtensionsTest/
// In that case, you don't have to use the source code of CustomTableViewCell
// #include "../TableViewTest/CustomTableViewCell.h"
// Use the following header otherwises
#include "CustomTableViewCell.h"
#include "../ExtensionsTest.h"

USING_NS_CC;
USING_NS_CC_EXT;

void runGridViewTest()
{
	Scene *scene = Scene::create();
	GridViewTestLayer *layer = GridViewTestLayer::create();
	scene->addChild(layer);
	Director::getInstance()->replaceScene(scene);
}

// on "init" you need to initialize your instance
bool GridViewTestLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	Size winSize = Director::getInstance()->getWinSize();

    int colNum = 2;
    GridView* tableView = GridView::create(this, Size(120, 240));
	tableView->setDirection(ScrollView::Direction::VERTICAL);
	tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setPosition(Vec2(20, 20));
    tableView->setColNum(colNum);
    tableView->setDelegate(this);
    this->addChild(tableView);
    tableView->reloadData();

    int rowNum = 2;
	tableView = GridView::create(this, Size(240, 120));
    tableView->setDirection(ScrollView::Direction::HORIZONTAL);
	tableView->setPosition(Vec2(200, 20));
    tableView->setRowNum(rowNum);
	tableView->setDelegate(this);
	this->addChild(tableView);
	tableView->reloadData();

	// Back Menu
	MenuItemFont *itemBack = MenuItemFont::create("Back", CC_CALLBACK_1(GridViewTestLayer::toExtensionsMainLayer, this));
	itemBack->setPosition(Vec2(VisibleRect::rightBottom().x - 50, VisibleRect::rightBottom().y + 25));
	Menu *menuBack = Menu::create(itemBack, nullptr);
	menuBack->setPosition(Vec2::ZERO);
	addChild(menuBack);

    return true;
}

void GridViewTestLayer::toExtensionsMainLayer(cocos2d::Ref *sender)
{
	ExtensionsTestScene *scene = new (std::nothrow) ExtensionsTestScene();
	scene->runThisTest();
	scene->release();
}

void GridViewTestLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", cell->getIdx());
}

Size GridViewTestLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(60, 60);
}

TableViewCell* GridViewTestLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto string = String::createWithFormat("%ld", idx);
    TableViewCell *cell = table->dequeueCell();
    if (!cell) {
        cell = new (std::nothrow) CustomTableViewCell();
        cell->autorelease();
        auto sprite = Sprite::create("Images/Icon.png");
        sprite->setAnchorPoint(Vec2::ZERO);
        sprite->setPosition(Vec2(0, 0));
        cell->addChild(sprite);

        auto label = Label::createWithSystemFont(string->getCString(), "Helvetica", 20.0);
        label->setPosition(Vec2::ZERO);
		label->setAnchorPoint(Vec2::ZERO);
        label->setTag(123);
        cell->addChild(label);
    }
    else
    {
        auto label = (Label*)cell->getChildByTag(123);
        label->setString(string->getCString());
    }


    return cell;
}

ssize_t GridViewTestLayer::numberOfCellsInTableView(TableView *table)
{
    return 20;
}
