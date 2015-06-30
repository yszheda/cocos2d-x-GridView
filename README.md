cocos2d-x-GridView
==================

GridView (2D TableView) in cocos2d-x

NOTE:
- Some methods of the `TableView` class should be declared as virtual, you'd better replace or merge CCTableView.h in your cocos2d-x source code directory with the one in the repo.
- Either of the following methods of the `GridView` must be called:
    - `setRowNum`
    - `setColNum`
- To run `GridViewTest`, you can copy the #GridViewTest# folder to `${COCOS_X_ROOT}/tests/cpp-tests/Classes/ExtensionsTest` and replace the `ExtensionsTest.cpp`.
- You can refer to [an article in my blog (in Chinese)](http://galoisplusplus.gitcafe.io/blog/2014/09/20/gridview-in-cocos2d-x/) for further details.
