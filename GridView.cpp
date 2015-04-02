/*
 * =====================================================================================
 *
 *       Filename:  GridView.cpp
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
#include "GridView.h"

USING_NS_CC;
USING_NS_CC_EXT;

GridView* GridView::create()
{
    return GridView::create(nullptr, Size::ZERO);
}

GridView* GridView::create(TableViewDataSource* dataSource, Size size)
{
    return GridView::create(dataSource, size, nullptr);
}

GridView* GridView::create(TableViewDataSource* dataSource, Size size, Node *container)
{
    GridView *table = new GridView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();

    return table;
}

bool GridView::initWithViewSize(Size size, Node* container/* = nullptr*/)
{
    if (ScrollView::initWithViewSize(size,container))
    {
        CC_SAFE_DELETE(_indices);
        _indices        = new std::set<ssize_t>();
        _vordering      = VerticalFillOrder::BOTTOM_UP;
        this->setDirection(Direction::VERTICAL);

        ScrollView::setDelegate(this);
        return true;
    }
    // Assume that the size of each cell is equal
    _cellSize =  _dataSource->tableCellSizeForIndex(this, 0);
    _cellsCount = _dataSource->numberOfCellsInTableView(this);
    return false;
}

GridView::GridView()
: TableView()
, _cellSize(Size::ZERO)
, _cellsCount(0)
, _rowNum(0)
, _colNum(0)
{

}

GridView::~GridView()
{
    CC_SAFE_DELETE(_indices);
}

/*-----------------------------------------------------------------------------
 *  override
 *-----------------------------------------------------------------------------*/
void GridView::_updateContentSize()
{
    // Assume that the size of each cell is equal
    _cellSize =  _dataSource->tableCellSizeForIndex(this, 0);
    _cellsCount = _dataSource->numberOfCellsInTableView(this);

    Size size = Size::ZERO;

    if (_rowNum == 0 && _colNum == 0) {
        return ;
    } 

    if (_cellsCount > 0)
    {
        switch (this->getDirection())
        {
            case Direction::HORIZONTAL:
            {
                CCASSERT(_rowNum != 0, "_rowNum != 0");
                int colNum = (_cellsCount + _rowNum - 1) / _rowNum;
                size.width = std::max(colNum * _cellSize.width, _viewSize.width);
                size.height = _cellSize.height * _rowNum;
                break;
            }
            default:
            {
                CCASSERT(_colNum != 0, "_colNum != 0");
                int rowNum = (_cellsCount + _colNum - 1) / _colNum;
                size.width = _cellSize.width * _colNum;
                size.height = std::max(rowNum * _cellSize.height, _viewSize.height);
                break;
            }
        }
    }

    this->setContentSize(size);

	if (_oldDirection != _direction)
	{
		if (_direction == Direction::HORIZONTAL)
		{
			this->setContentOffset(Vec2(0,0));
		}
		else
		{
			this->setContentOffset(Vec2(0,this->minContainerOffset().y));
		}
		_oldDirection = _direction;
	}

}

long GridView::_indexFromOffset(Vec2 offset)
{
    long index = 0;
    const long maxIdx = _dataSource->numberOfCellsInTableView(this) - 1;

    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y;
    }
    index = this->__indexFromOffset(offset);
    if (index != -1)
    {
        index = MAX(0, index);
        if (index > maxIdx)
        {
            index = CC_INVALID_INDEX;
        }
    }

    return index;
}

/*-----------------------------------------------------------------------------
 *  override
 *-----------------------------------------------------------------------------*/
Vec2 GridView::__offsetFromIndex(ssize_t index)
{
    return _vCellsPositions[index];
}

Vec2 GridView::_offsetFromIndex(ssize_t index)
{
    Vec2 offset = this->__offsetFromIndex(index);

    const Size cellSize = _dataSource->tableCellSizeForIndex(this, index);
    if (_vordering == VerticalFillOrder::TOP_DOWN)
    {
        offset.y = this->getContainer()->getContentSize().height - offset.y - cellSize.height;
    }
    return offset;
}

/*-----------------------------------------------------------------------------
 *  override
 *-----------------------------------------------------------------------------*/
long GridView::__indexFromOffset(Vec2 offset)
{
    // Assume that the size of each cell is equal
    _cellSize =  _dataSource->tableCellSizeForIndex(this, 0);
    _cellsCount = _dataSource->numberOfCellsInTableView(this);

    long index = -1;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
            {
                CCASSERT(_rowNum != 0, "_rowNum != 0");
                index = (int) (offset.y / _cellSize.height) + _rowNum * (int) (offset.x / _cellSize.width);
                break;
            }
        default:
            {
                CCASSERT(_colNum != 0, "_colNum != 0");
//                int rowNum = (_cellsCount + _colNum - 1) / _colNum;
//                int maxHeight = std::max(rowNum * _cellSize.height, _viewSize.height);
//                index = _colNum * (int) ((maxHeight - offset.y) / _cellSize.height) + (int) (offset.x / _cellSize.width);
                index = _colNum * (int) (offset.y / _cellSize.height) + (int) (offset.x / _cellSize.width);
                break;
            }
    }
    return index;
}

/*-----------------------------------------------------------------------------
 *  override
 *-----------------------------------------------------------------------------*/
void GridView::_setIndexForCell(ssize_t index, TableViewCell *cell)
{
    cell->setAnchorPoint(Vec2(0.0f, 0.0f));
    cell->setPosition(_offsetFromIndex(index));
    cell->setIdx(index);
}

/*-----------------------------------------------------------------------------
 *  override
 *-----------------------------------------------------------------------------*/
void GridView::_updateCellPositions()
{
    // Assume that the size of each cell is equal
    _cellSize =  _dataSource->tableCellSizeForIndex(this, 0);
    _cellsCount = _dataSource->numberOfCellsInTableView(this);

    _vCellsPositions.resize(_cellsCount, Point(0, 0));

    if (_rowNum == 0 && _colNum == 0) {
        return ;
    } 

    if (_cellsCount > 0)
    {
        float currentPosX;
        float currentPosY;
        switch (this->getDirection())
        {
            case Direction::HORIZONTAL:
            {
                currentPosX = 0.0f;
                currentPosY = 0.0f;
                CCASSERT(_rowNum != 0, "_rowNum != 0");
                for (int i = 0; i < _cellsCount; i++) {
                    if (i != 0) {
                        if (i % _rowNum == 0) {
                            currentPosY = 0.0f;
                            currentPosX += _cellSize.width;
                        } else {
                            currentPosY += _cellSize.height;
                        }
                    }
                    _vCellsPositions[i] = Point(currentPosX, currentPosY);
                }
                break;
            }
            default:
            {
                currentPosX = 0.0f;
                CCASSERT(_colNum != 0, "_colNum != 0");
                currentPosY = 0.0f;
                for (int i = 0; i < _cellsCount; i++) {
                    if (i != 0) {
                        if (i % _colNum == 0) {
                            currentPosX = 0.0f;
                            currentPosY += _cellSize.height;
                        } else {
                            currentPosX += _cellSize.width;
                        }
                    }
                    _vCellsPositions[i] = Point(currentPosX, currentPosY);
                }
                break;
            }
        }
    }
}

int GridView::rowOfIndex(const long index) const
{
    int row = 0;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
        {
            CCASSERT(_rowNum != 0, "_rowNum != 0");
            row = index % _rowNum;
            break;
        }
        default:
        {
            row = index / _colNum;
            break;
        }
    }
    return row;
}

int GridView::colOfIndex(const long index) const
{
    int col = 0;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
        {
            CCASSERT(_rowNum != 0, "_rowNum != 0");
            col = index / _rowNum;
            break;
        }
        default:
        {
            col = index % _colNum;
            break;
        }
    }
    return col;
}

int GridView::maxRowIdx() const
{
    int maxRowIdx = 0;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
        {
            CCASSERT(_rowNum != 0, "_rowNum != 0");
            maxRowIdx = _rowNum - 1;
            break;
        }
        default:
        {
            CCASSERT(_colNum != 0, "_colNum != 0");
            if (_cellsCount < _colNum) {
                maxRowIdx = 0;
            } else {
                maxRowIdx = (_cellsCount + _colNum - 1) / _colNum - 1;
            }
            break;
        }
    }
    return maxRowIdx;
}

int GridView::maxColIdx() const
{
    int maxColIdx = 0;
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
        {
            CCASSERT(_rowNum != 0, "_rowNum != 0");
            if (_cellsCount < _rowNum) {
                maxColIdx = 0;
            } else {
                maxColIdx = (_cellsCount + _rowNum - 1) / _rowNum - 1;
            }
            break;
        }
        default:
        {
            CCASSERT(_colNum != 0, "_colNum != 0");
            maxColIdx = _colNum - 1;
            break;
        }
    }
    return maxColIdx;
}

int GridView::rowFromOffset(const cocos2d::Vec2 offset) const
{
    int rowNum = (_cellsCount + _colNum - 1) / _colNum;
    int maxHeight = std::max(rowNum * _cellSize.height, _viewSize.height);
    int row = (maxHeight - offset.y) / _cellSize.height;
    if (row < 0) {
        row = 0;
    } else if (row > this->maxRowIdx()) {
        row = this->maxRowIdx();
    }
    return row;
}

int GridView::colFromOffset(const cocos2d::Vec2 offset) const
{
    int col = offset.x / _cellSize.width;
    if (col < 0) {
        col = 0;
    } else if (col > this->maxColIdx()) {
        col = this->maxColIdx();
    }
    return col;
}

/*-----------------------------------------------------------------------------
 *  override
 *-----------------------------------------------------------------------------*/
void GridView::scrollViewDidScroll(ScrollView* view)
{
    long countOfItems = _dataSource->numberOfCellsInTableView(this);
    if (0 == countOfItems)
    {
        return;
    }

    if (_isUsedCellsDirty)
    {
        _isUsedCellsDirty = false;
        std::sort(_cellsUsed.begin(), _cellsUsed.end(), [](TableViewCell *a, TableViewCell *b) -> bool{
            return a->getIdx() < b->getIdx();
        });
    }
    
    if(_tableViewDelegate != nullptr) {
        _tableViewDelegate->scrollViewDidScroll(this);
    }

    ssize_t idx = 0;
    ssize_t maxIdx = 0;
    Vec2 offset = this->getContentOffset() * -1;
    maxIdx = MAX(countOfItems-1, 0);
    int startRow = 0;
    int endRow = 0;
    int maxRowIdx = this->maxRowIdx();
    int maxColIdx = this->maxColIdx();
    int startCol = 0;
    int endCol = 0;
    
    switch (this->getDirection())
    {
        case Direction::HORIZONTAL:
        {
            CCASSERT(_rowNum != 0, "_rowNum != 0");
            startCol = this->colFromOffset(offset);
            offset.x += _viewSize.width / this->getContainer()->getScaleX();
            endCol = this->colFromOffset(offset);
            
            if (!_cellsUsed.empty())
            {
                auto cell = _cellsUsed.at(0);
                idx = cell->getIdx();

                while(colOfIndex(idx) < startCol)
                {
                    this->_moveCellOutOfSight(cell);
                    if (!_cellsUsed.empty())
                    {
                        cell = _cellsUsed.at(0);
                        idx = cell->getIdx();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (!_cellsUsed.empty())
            {
                auto cell = _cellsUsed.back();
                idx = cell->getIdx();

                while(colOfIndex(idx) <= maxColIdx && colOfIndex(idx) > endCol)
                {
                    this->_moveCellOutOfSight(cell);
                    if (!_cellsUsed.empty())
                    {
                        cell = _cellsUsed.back();
                        idx = cell->getIdx();
                    }
                    else
                    {
                        break;
                    }
                }
            }

            for (int i = 0; i < _rowNum; i++) {
                for (int j = startCol; j <= endCol; j++) {
                    long index = i + j * _rowNum;
                    if (_indices->find(index) != _indices->end()) {
                        continue;
                    }
                    this->updateCellAtIndex(index);
                }
            }
            return;
//            break;
        }
        default:
        {
            CCASSERT(_colNum != 0, "_colNum != 0");
            startRow = this->rowFromOffset(offset);
            offset.y = offset.y + _viewSize.height / this->getContainer()->getScaleY();
            endRow = this->rowFromOffset(offset);
            if (_vordering == VerticalFillOrder::TOP_DOWN) {
                std::swap(startRow, endRow);
            }
            
            if (!_cellsUsed.empty())
            {
                auto cell = _cellsUsed.at(0);
                idx = cell->getIdx();

                while(rowOfIndex(idx) < startRow)
                {
                    this->_moveCellOutOfSight(cell);
                    if (!_cellsUsed.empty())
                    {
                        cell = _cellsUsed.at(0);
                        idx = cell->getIdx();
                    }
                    else
                    {
                        break;
                    }
                }
            }
            if (!_cellsUsed.empty())
            {
                auto cell = _cellsUsed.back();
                idx = cell->getIdx();

                while(rowOfIndex(idx) <= maxRowIdx && rowOfIndex(idx) > endRow)
                {
                    this->_moveCellOutOfSight(cell);
                    if (!_cellsUsed.empty())
                    {
                        cell = _cellsUsed.back();
                        idx = cell->getIdx();
                    }
                    else
                    {
                        break;
                    }
                }
            }

            for (int i = startRow; i <= endRow; i++) {
                for (int j = 0; j < _colNum; j++) {
                    long index = i * _colNum + j;
                    if (_indices->find(index) != _indices->end()) {
                        continue;
                    }
                    this->updateCellAtIndex(index);
                }
            }
            return;
//            break;
        }
    }
}

void GridView::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible()) {
        return;
    }

    if (_touchedCell){
		Rect bb = this->getBoundingBox();
		bb.origin = _parent->convertToWorldSpace(bb.origin);

		if (bb.containsPoint(pTouch->getLocation()) && _tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
            _tableViewDelegate->tableCellTouched(this, _touchedCell);
        }

        _touchedCell = nullptr;
    }

    ScrollView::onTouchEnded(pTouch, pEvent);
}

bool GridView::onTouchBegan(Touch *pTouch, Event *pEvent)
{
    if (!this->isVisible())
    {
        return false;
    }

    bool touchResult = ScrollView::onTouchBegan(pTouch, pEvent);

    if(_touches.size() == 1)
    {
        long index;
        Vec2 point;

        point = this->getContainer()->convertTouchToNodeSpace(pTouch);

        index = this->_indexFromOffset(point);
		if (index == CC_INVALID_INDEX)
		{
			_touchedCell = nullptr;
		}
        else
		{
			_touchedCell  = this->cellAtIndex(index);
		}

        if (_touchedCell && _tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellHighlight(this, _touchedCell);
        }
    }
    else if (_touchedCell)
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }

        _touchedCell = nullptr;
    }

    return touchResult;
}

void GridView::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchMoved(pTouch, pEvent);

    if (_touchedCell && isTouchMoved())
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }

        _touchedCell = nullptr;
    }
}

void GridView::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    ScrollView::onTouchCancelled(pTouch, pEvent);

    if (_touchedCell)
    {
        if(_tableViewDelegate != nullptr)
        {
            _tableViewDelegate->tableCellUnhighlight(this, _touchedCell);
        }

        _touchedCell = nullptr;
    }
}




