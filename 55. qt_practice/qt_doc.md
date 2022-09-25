### ui_*.h 文件内部
1. 进行UI 初始化
2. 使用QMetaObject::connectSlotsByName(MainWindow) 进行信号和槽之间的连接
   
   这也是为什么在 mainwindow.h 里面，看不到信号和连接过程，只能看到槽的原因

### 如何没有自动连接的话，应该如何做呢
1. 自行进行连接 主要参考 SLAMVisualTool 里面的可视化

    所以目前的SLAMVisualizationTool 一部分是使用qt designer 做的

    一部分是使用qt desinger 做的，这是为什么呢

### 杂乱的笔记
1. label button 等一行类的内容都是从Qwidget 继承过来的
2. QWidget 中 hide show SetHidden 都是在调用setVisible
   只有close 是真正会去杀死内存

### Qt 5.14.2 进行开发(VS2017)
1. 更换IDE原因:
   目前认为 Qt 5.14.2 的默认IDE 是Qt 4.11 直接导致VTK插件无法使用，所以这里采用Visual Studio 2017 替代方案
2. Qt Designer 使用

   因为不能够像 Qt Creator 那样自动生成 ui_*.h 所以部分功能需要自己去实现