### PointCloudWidget 代码阅读
主要目的是为了熟悉QVTKWidget,方便进一步地修改

1. 使用了PointCloudWidget的有哪些地方(内部函数如何和外部功能进行交互的)?
2. PointCloudWidget 主要的流程函数是什么?
3. 一些回调函数主要做了什么?
4. vtk的内部组件主要有哪些?

-------

### 使用了 PointCloudWidget 的主要有哪些地方

1. MainWindow CTOR 创建和规定位置
2. MainWindow bindSignalToSlots
      
   signal to slot
   对应在MainWindow 进行的
   是否进行框选
   得到车体状态
   选中的反光住
   signal_editFlagChanged
   signal_reflectorSelected
   signal_vehiclePosUpdated

   用于显示当前选中KeyFramePointCloud的信息
   signal_KeyFramePointCloudInfo -> 
   KeyFramePointCloudInfoWidget::slot_updateKeyFramePointCloudInfo

   slot to signal
   确定修改的参数
   slot_ajustKeyFramePointCloudInfo -> KeyFramePointCloudInfoWidget:on_pushButton_ajust_clicked
   |____ showPointCloudMapForLoopClosure 进行点云和轨迹的展示
   |____ updateReflectorMap 展示反光住的轨迹

3. MainWindow loadDefaultMap 

    在非地图编辑的模式下，展示GlobalMap 和 CurrentScan
    在地图编辑的模式下，仅展示GlobalMap
    updatePointCloudMap
    
    updateReflectorMap

4. MainWindow disconnectProcess

    是否进入建图模式
    m_ptrPointCloudWidget->setBuildingFlag(false)

5. MainWindow slot_buildMap

    清空 PCLViewer
    m_ptrPointCloudWidget->clearAll();	
    是否进入建图模式
    m_ptrPointCloudWidget->setBuildingFlag(false);

6. MainWindow setCurrentWorkMode
   
   m_ptrPointCloudWidget->setCurrentWorkMode(workMode);

7. MainWindow setCurrentMapEditType

    m_ptrPointCloudWidget->setCurrentMapEditType(m_currentMapEditType);

8. MainWindow enterbuildMapStatus
   
    清空 PCLViewer
   	m_ptrPointCloudWidget->clearAll();
    是否进入建图模式
	m_ptrPointCloudWidget->setBuildingFlag(true);

9. MainWindow exitbuildMapStatus
    
    是否进入建图模式
    m_ptrPointCloudWidget->setBuildingFlag(false);
	m_ptrPointCloudWidget->updatePointCloudMap();
	m_ptrPointCloudWidget->updateReflectorMap();

10. MainWindow slot_deleteSelectedPointCloud

    deleteSelectedPointCloud();

11. MainWindow slot_setWeightofSelectedPointCloud
    
    setWeightofSelectedPointCloud()

12. MainWindow slot_startOrExitManualLoopClosure
    
    startManualLoopClosure
    |____ showPointCloudMapForLoopClosure 进行点云和轨迹的展示
    |____ updateReflectorMap 展示反光住的轨迹
    exitManualLoopClosure

13. 多种模式

    Work_Mode
    |____ Work_Mode_MapBuild 建图模式
    |____ Work_Mode_MapEdit  地图编辑
    
    MapEdit_Type
    |____ MapEdit_Type_PointCloud 点云编辑模式
    |____ MapEdit_Type_Reflector  反光柱编辑模式

### PCLViewer 
1. 多种回调
    
    registerAreaPickingCallback
    registerPointPickingCallback
    registerMouseCallback
    registerKeyboardCallback

2. 显示设置
    
    addPointCloud
    addCube
    setShapeRenderingProperties


### KeyFramePointCloudInfoWidget

1. 主要是为了进行信息的输入


经验分享:
1. 找关键词
2. 看背后的基类

### 具体开发逻辑和准备内容

1. 进入加载之后，显示keyframe 和 相关的点云      addcube addpointcloud              ok
2. 点击相关keyframe点，确认点云，进行加粗显示    mouse_callback pointpick 点云修改    ok
3. wasd 进行点云平移和旋转                     需要固定Camera视角和点云位置           ok
4. 'o' 确认输出这个坐标位置                    keyboard_callback                  ok
5. 整理出相关的Widget 内容

3 4 的代码都比较容易进行编写
但是2 的交互问题是需要进行思考的部分

3  先确定最后一个点云是目标控制点云，进行wasd 的控制

------

### 数据准备

1. 点云和相关的位姿                        ok
2. 一个简单的容器用于交互和保存              ok
