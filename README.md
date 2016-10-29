cocos2d-x-GridView
==================

GridView (2D TableView) in cocos2d-x

NOTE:
- Some methods of the `TableView` class should be declared as `virtual`, you'd better replace or merge _CCTableView.h_ in your cocos2d-x source code directory with the one in the repo.
- Either of the following methods of the `GridView` must be called:
    - `setRowNum`
    - `setColNum`
- To run `GridViewTest`, you can copy the _GridViewTest_ folder to `${COCOS_X_ROOT}/tests/cpp-tests/Classes/ExtensionsTest` and replace the _ExtensionsTest.cpp_.
- You can refer to [an article in my blog (in Chinese)](http://galoisplusplus.coding.me/blog/2014/09/20/gridview-in-cocos2d-x/) for further details.
