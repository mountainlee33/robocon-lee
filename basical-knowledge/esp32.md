## TWAI（CAN）

 采用收发buffer，控制器硬件将接受到的消息放入buffer中，软件驱动再把buffer中的报文处理过后放入queue，用户应用读取queue获取报文，发送同理 

## 蓝牙BLE

GAP：GAP定义了设备的**广播**行为， 把设备分成两种：中心设备（Central）、外围设备（Peripheral），外围设备对外不断广播，中心设备扫描、接收广播。发现后进而建立连接，再利用GATT协议进行数据传输。
GATT: 利用GAP发现并连接相应设备后，就可以开始传输数据了。蓝牙BLE的数据传输建立在GATT协议上，它定义了BLE设备之间如何传输数据。GATT把设备分为Client和Server，其中命令与请求由Client主动发起，Server被动接受。 

