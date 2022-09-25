### 主要pipeline

1. 主要pipeline
   入口: svo_node
   |___ ctor 订阅 imu stereo/mono 和 remotekey
   |___ initThirdParty
   |___ run() -> ros.spin()