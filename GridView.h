/*
 * =====================================================================================
 *
 *       Filename:  GridView.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014/09/18 
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuai Yuan (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __GRID_VIEW_H__
#define __GRID_VIEW_H__ 

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

#include <set>
#include <vector>

class GridView : public cocos2d::extension::TableView
{
public:
    
    /** Empty contructor of GridView */
    static GridView* create();
    
    /**
     * An intialized table view object
     *
     * @param dataSource data source
     * @param size view size
     * @return table view
     * @code
     * when this function bound to js or lua,the input params are changed
     * in js:var create(var jsObject,var size)
     * in lua:local create(var size)
     * in lua:
     * @endcode
     */
    static GridView* create(cocos2d::extension::TableViewDataSource* dataSource, cocos2d::Size size);
    /**
     * An initialized table view object
     *
     * @param dataSource data source;
     * @param size view size
     * @param container parent object for cells
     * @return table view
     * @code
     * when this function bound to js or lua,the input params are changed
     * in js:var create(var jsObject,var size,var container)
     * in lua:local create(var size, var container)
     * in lua:
     * @endcode
     */
    static GridView* create(cocos2d::extension::TableViewDataSource* dataSource, cocos2d::Size size, cocos2d::Node *container);
    /**
     * @js ctor
     */
    GridView();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~GridView();

    bool initWithViewSize(cocos2d::Size size, Node* container = NULL);

    // Overrides
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view) override;
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view)  override {}
    virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;
    virtual void onTouchCancelled(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) override;

protected:
    virtual long _indexFromOffset(cocos2d::Vec2 offset) override;
    virtual long __indexFromOffset(cocos2d::Vec2 offset) override;
    virtual cocos2d::Vec2 _offsetFromIndex(ssize_t index) override;
    virtual cocos2d::Vec2 __offsetFromIndex(ssize_t index) override;

    virtual void _setIndexForCell(ssize_t index, cocos2d::extension::TableViewCell *cell) override;

    virtual void _updateCellPositions();

    /*-----------------------------------------------------------------------------
     *  
     *-----------------------------------------------------------------------------*/
protected:
    std::vector<cocos2d::Vec2> _vCellsPositions;
    cocos2d::Size _cellSize;
    ssize_t _cellsCount;
    int _rowNum;
    int _colNum;

public:
    int getRowNum() const { return _rowNum; }
    void setRowNum(const int rowNum) { _rowNum = rowNum; }
    int getColNum() const { return _colNum; }
    void setColNum(const int colNum) { _colNum = colNum; }

public:
    int maxRowIdx() const;
    int maxColIdx() const;

protected:
    int rowOfIndex(const long index) const;
    int colOfIndex(const long index) const;
    int rowFromOffset(const cocos2d::Vec2 offset) const;
    int colFromOffset(const cocos2d::Vec2 offset) const;

public:
    virtual void _updateContentSize();

};

#endif /* __GRID_VIEW_H__  */
