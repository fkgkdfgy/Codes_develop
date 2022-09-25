# ROS2 tutorial --- Service Action
(具体内容来自 turtlesim 和nav2)

## service-server 编写
### 节点实例化
  <pre><blockcode> 
    //对应ros1 里面的初始化两行代码
    rclcpp::init(argc, argv);
    auto nh_ = rclcpp::Node::make_shared("turtlesim"); 
  </blockcode></pre>

### 创建server
  <pre><blockcode> 
    // 以下为示例
    // 需要注意placeholders 可用可不用，这里只要保证只有两个参数即可
    teleport_relative_srv_ = nh_->create_service<turtlesim::srv::TeleportRelative>(real_name + "/teleport_relative",   
    //‘service’ 名字
    std::bind(&Turtle::teleportRelativeCallback, this, std::placeholders::_1, std::placeholders::_2));
    // 绑定的callback函数
  </blockcode></pre>

### service 的callback 函数编写
  <pre><blockcode> 
    // 以下为示例
    // request 和 response 同时作为函数参数
   bool Turtle::teleportRelativeCallback(const turtlesim::srv::TeleportRelative::Request::SharedPtr req, turtlesim::srv::TeleportRelative::Response::SharedPtr)
    {
    teleport_requests_.push_back(TeleportRequest(0, 0, req->angular, req->linear, true));
    return true;
    }
  </blockcode></pre>

## service-client 编写

### 节点实例化
  <pre><blockcode> 
    //对应ros1 里面的初始化两行代码
    rclcpp::init(argc, argv);
    auto nh_ = rclcpp::Node::make_shared("add_two_ints_client"); 
  </blockcode></pre>

### 创建client
  <pre><blockcode> 
    // 以下为示例
    auto client =
    node->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");
  </blockcode></pre>

### 填充、发布、等待结果
<pre><blockcode> 
// 以下为示例 填充
  auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
  request->a = atoll(argv[1]);
  request->b = atoll(argv[2]);
  </blockcode></pre>
  <pre><blockcode> 
// 以下为示例 发布
 auto result = client->async_send_request(request);
  </blockcode></pre>
  <pre><blockcode> 
// 以下为示例 等待结果
if (rclcpp::spin_until_future_complete(node, result) ==
    rclcpp::executor::FutureReturnCode::SUCCESS)
    {...}
  </blockcode></pre>

## Action-server 编写
### 节点初始化
    和service一致

### 创建server
  <pre><blockcode> 
  // 来自 rclcpp 的原始action
  // 方法一：
  create_server<ActionT>(
    node->get_node_base_interface(),      // 以下node->都是对于节点的依赖项
    node->get_node_clock_interface(),
    node->get_node_logging_interface(),
    node->get_node_waitables_interface(),
    name,
    handle_goal,                          // 判断是否接受这个goal的callback
    handle_cancel,                        // 判断是否取消一个goal 返回的值
                                          // 表示会尝试取消，而不是已经取消
    handle_accepted,                      // 接受goal之后运行的回调函数
    options,                              // options和group有默认值可以不填
    group);
// 补充说明 handle_cancel handle_accpeted 的函数参数都是单个的 rclcpp_action::ServerGoalHandle<>
// 方法二：
// 和方法一基本一致只是给方法一对节点加了一层包装
  create_server(                          
    NodeT node,
    const std::string & name,
    GoalCallback handle_goal,
    CancelCallback handle_cancel,
    AcceptedCallback handle_accepted,
    const rcl_action_server_options_t & options =      
    rcl_action_server_get_default_options(),
    rclcpp::CallbackGroup::SharedPtr group = nullptr)
  </blockcode></pre>
  

<pre><blockcode> 
  // 相关的例子
    rotate_absolute_action_server_ = rclcpp_action::create_server<turtlesim::action::RotateAbsolute>(
    nh,
    real_name + "/rotate_absolute",
    [](const rclcpp_action::GoalUUID &, std::shared_ptr<const turtlesim::action::RotateAbsolute::Goal>)
    {
      // Accept all goals
      return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    },
    [](const std::shared_ptr<RotateAbsoluteGoalHandle>)
    {
      // Accept all cancel requests
      return rclcpp_action::CancelResponse::ACCEPT;
    },
    std::bind(&Turtle::rotateAbsoluteAcceptCallback, this, std::placeholders::_1));
  </blockcode></pre>


## Action-client 编写

### 创建client
<pre><blockcode> 
// 应该是可以和server一致的
  self_client_ = rclcpp_action::create_client<nav2_msgs::action::NavigateToPose>(
  client_node_, "navigate_to_pose");
  </blockcode></pre>
### 发送goal 填充部分省略
<pre><blockcode> 
  auto future_result_handle = self_client_->async_send_goal(goal);
  </blockcode></pre>
### 等待结果
<pre><blockcode> 
  auto goal_handle = future_goal_handle.get();
  auto future_result =  self_client_->async_get_result(goal_handle);
  </blockcode></pre>

--------------------------

## nav2 包装下的action 示例 Fibonacci
### 创建server 

<pre><blockcode> 
// 只保留了 node name 和handle_accepted
  action_server_ = std::make_shared<nav2_util::SimpleActionServer<Fibonacci>>(
      shared_from_this(),
      "fibonacci",
      std::bind(&FibonacciServerNode::execute, this));
  </blockcode></pre>