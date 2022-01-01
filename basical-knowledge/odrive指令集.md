<!--
 * @Description: ODrive学习
 * @Version: 1.0
 * @Author: Amamiya
 * @Date: 2021-11-03 22:10:31
 * @LastEditors: Amamiya
 * @LastEditTime: 2021-11-06 22:07:49
-->
# ODrive学习
- [ODrive学习](#odrive学习)
  - [资料](#资料)
  - [ODrive从零开始配置代码（完整版/SUNNY SKY）](#odrive从零开始配置代码完整版sunny-sky)
  - [ODrive电机和编码器校准](#odrive电机和编码器校准)5
  - [问题记录](#问题记录)
  - [其他代码](#其他代码)
  - [电机与编码器参数参考](#电机与编码器参数参考)
## 资料
[ODrive入门指南](https://blog.csdn.net/abf1234444/category_9555991.html)
[ODrive踩坑记录](https://blog.csdn.net/mark_md/category_11133983.html)
[ODrive创客基地](https://blog.csdn.net/gjy_skyblue/category_10813011.html)

## ODrive从零开始配置代码（完整版/SUNNY SKY）
<details>
<summary>STEP 1：恢复出厂配置</summary>

odrv0.erase_configuration() # 恢复出厂配置
</details>
<details>
<summary>STEP 2：配置主板参数</summary>

odrv0.config.brake_resistance = 0 # 配置AUX接口上的制动电阻值（常见的为0.47、2.0Ω），如没接则配置为0

odrv0.config.dc_bus_undervoltage_trip_level = 8.0 # 配置低压保护阈值（V）

odrv0.config.dc_bus_overvoltage_trip_level = 56.0 # 配置高压保护阈值（V）

odrv0.config.dc_max_positive_current = 50.0 # 配置过流保护阈值（A）

odrv0.config.dc_max_negative_current = -5.0 # 配置反向电流阈值（电机制动产生的反向电流）（A）

odrv0.config.max_regen_current = 0 # 配置回充电流值（根据供电电池的参数配置，开关电源供电配置为0）（A）

odrv0.save_configuration() # 保存参数
</details>
<details>
<summary>STEP 3：配置电机参数</summary>

odrv0.axis0.motor.config.pole_pairs = 7 # 配置电机0极对数，根据博客开篇的介绍，自己去数磁极个数，然后/2

odrv0.axis0.motor.config.current_lim = 35 # 配置电机0的限制电流（A）

odrv0.axis0.motor.config.requested_current_range = 60 # 配置电机0的电流采样阈值（A）

odrv0.axis0.motor.config.calibration_current = 10 # 配置电机0校准时的电流阈值（根据自己电机的负载状况酌情配置）（A）

odrv0.axis0.motor.config.motor_type = MOTOR_TYPE_HIGH_CURRENT # 配置电机0类型。目前支持两种电机：大电流电机（MOTOR_TYPE_HIGH_CURRENT）和云台电机（MOTOR_TYPE_GIMBAL）

odrv0.save_configuration() # 保存参数

odrv0.axis1.motor.config.pole_pairs = 21 # 配置电机0极对数，根据博客开篇的介绍，自己去数磁极个数，然后/2

odrv0.axis1.motor.config.current_lim = 35 # 配置电机0的限制电流（A）

odrv0.axis1.motor.config.requested_current_range = 60 # 配置电机0的电流采样阈值（A）

odrv0.axis1.motor.config.calibration_current = 10 # 配置电机0校准时的电流阈值（根据自己电机的负载状况酌情配置）（A）

odrv0.axis1.motor.config.motor_type = MOTOR_TYPE_HIGH_CURRENT # 配置电机0类型。目前支持两种电机：大电流电机（MOTOR_TYPE_HIGH_CURRENT）和云台电机（MOTOR_TYPE_GIMBAL）

odrv0.save_configuration() # 保存参数
</details>
<details>
<summary>STEP 4：配置编码器参数</summary>

odrv0.axis0.encoder.config.mode = ENCODER_MODE_SPI_ABS_AMS # 配置电机0编码器类型。ENCODER_MODE_INCREMENTAL 使用的是ABI正交（增量）编码器。ENCODER_MODE_SPI_ABS_AMS 使用AMS磁编码器-AS5047/AS5048。

odrv0.axis0.encoder.config.abs_spi_cs_gpio_pin = 4 # 设置CSn片选的引脚，ODrive J3接口GPIO3-8任选一个作为CS，这里我使用GPIO4

odrv0.axis0.encoder.config.cpr = 2**14 # 配置电机0编码器CPR（每转一圈，编码器的计数），AS5047P的最大分辨率为14位（16384）

odrv0.axis0.encoder.config.bandwidth = 3000 # 编码器带宽设置，CPR值越高带宽设置的也越高

odrv0.axis0.encoder.config.calib_range = 10 #编码器精度，类型为 `float`，单位为 `圆周角度∠` （这个值可以适当的大一些，避免环境干扰）。电机实际转动角度和开环移动距离之间允许的最大误差，超过此误差将报错ERROR_CPR_OUT_OF_RANGE。

odrv0.save_configuration() # 保存参数

odrv0.axis1.encoder.config.mode = ENCODER_MODE_SPI_ABS_AMS # 配置电机0编码器类型。ENCODER_MODE_INCREMENTAL 使用的是ABI正交（增量）编码器。ENCODER_MODE_SPI_ABS_AMS 使用AMS磁编码器-AS5047/AS5048。

odrv0.axis1.encoder.config.abs_spi_cs_gpio_pin = 4 # 设置CSn片选的引脚，ODrive J3接口GPIO3-8任选一个作为CS，这里我使用GPIO4

odrv0.axis1.encoder.config.cpr = 2**14 # 配置电机0编码器CPR（每转一圈，编码器的计数），AS5047P的最大分辨率为14位（16384）

odrv0.axis1.encoder.config.bandwidth = 3000 # 编码器带宽设置，CPR值越高带宽设置的也越高

odrv0.axis1.encoder.config.calib_range = 10 #编码器精度，类型为 `float`，单位为 `圆周角度∠` （这个值可以适当的大一些，避免环境干扰）。电机实际转动角度和开环移动距离之间允许的最大误差，超过此误差将报错ERROR_CPR_OUT_OF_RANGE。

odrv0.save_configuration() # 保存参数

</details>
<details>
<summary>STEP 5：配置控制器参数（位置闭环模式、配置PID参数）</summary>

odrv0.axis0.controller.config.control_mode = CONTROL_MODE_POSITION_CONTROL # 配置电机0控制模式，为位置闭环控制

odrv0.axis0.controller.config.vel_limit = 120 # 配置电机0最大转速（转/秒）（电机kv值 * 电压 / 60）

odrv0.axis0.controller.config.pos_gain = 20 # 配置位置环增益：20

odrv0.axis0.controller.config.vel_gain = 0.05 # 配置速度环增益：0.05

odrv0.axis0.controller.config.vel_integrator_gain = 0.02 # 配置积分增益：0.02

odrv0.axis0.controller.config.input_mode = INPUT_MODE_TRAP_TRAJ # 配置输入模式为：梯形轨迹模式

odrv0.axis0.trap_traj.config.vel_limit = 50 # 配置梯形模式下的电机转速阈值（转/秒）

odrv0.axis0.trap_traj.config.accel_limit = 10 # 配置梯形运动模式下的加速加速度；数值大小影响动作跟随效果，大则跟随快；小则跟随慢。

odrv0.axis0.trap_traj.config.decel_limit = 10 # 配置梯形运动模式下的减速加速度

odrv0.save_configuration() # 保存参数

odrv0.reboot() # 重启驱动器

</details>

## ODrive电机和编码器校准
<details>
<summary>CASE 1：每次上电后都自动校准编码器</summary>

odrv0.axis0.requested_state = AXIS_STATE_MOTOR_CALIBRATION # 进行电机参数校准（运行后电机会发出哔~的一声）

odrv0.axis0.motor.config.pre_calibrated = True # 设置电机预校准。（不用每次上电都哔~的一声）驱动器会将本次校准值保存，避免上电启动后自动校准，以加快启动速度。

odrv0.axis0.requested_state = AXIS_STATE_ENCODER_OFFSET_CALIBRATION # 进行编码器校准（运行后，电机会正转一圈再反转一圈）

odrv0.axis0.error # 查看错误，如果为0，则为无错。否则请断电后重启，重试校准。

odrv0.axis0.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL # 配置电机为闭环模式

odrv0.axis0.controller.input_pos = 10 # （测试运动）控制电机运行到10圈的位置

odrv0.axis0.controller.input_pos = 0 # （测试运动）控制电机运行到0圈的位置

odrv0.axis0.config.startup_encoder_offset_calibration = True # 设置ODrive上电启动时，自动校准编码器

odrv0.axis0.config.startup_closed_loop_control = True # 设置ODrive上电启动时，自动进入闭环模式

odrv0.save_configuration() # 保存参数

odrv0.reboot() # 重启驱动器

</details>

<details>
<summary>CASE 2：每次上电后，只自动校准电机，不校准编码器</summary>

odrv0.axis0.requested_state = AXIS_STATE_MOTOR_CALIBRATION # 进行电机参数校准（运行后电机会发出哔~的一声）

odrv0.axis0.config.startup_motor_calibration = True # 设置ODrive上电启动后，自动校准电机

odrv0.axis0.requested_state = AXIS_STATE_ENCODER_OFFSET_CALIBRATION # 进行编码器校准（运行后，电机会正转一圈再反转一圈）

odrv0.axis0.encoder.config.pre_calibrated = True # 设置编码器预校准。（不用每次上电都右转一圈又左转一圈）驱动器会将本次校准值保存，避免上电启动后自动校准，以加快启动速度。

odrv0.axis0.config.startup_encoder_offset_calibration = False # 关闭ODrive上电启动时，自动校准编码器

odrv0.axis0.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL # 配置电机为闭环模式

odrv0.axis0.config.startup_closed_loop_control = True # 设置ODrive上电启动时，自动进入闭环模式

odrv0.save_configuration() # 保存参数

odrv0.reboot() # 重启驱动器

odrv0.axis0.encoder.config.calib_range = 0.1 #当因编码器精度误差，而导致失步时，考虑增加以下选项
odrv0.axis0.encoder.config.calib_range = 10  #编码器精度，类型为 `float`，单位为 `圆周角度∠`  电机实际转动角度和开环移动距离之间允许的最大误差，超过此误差将报错ERROR_CPR_OUT_OF_RANGE。

odrv0.save_configuration() # 保存参数

</details>
<details>
<summary>CASE 3：每次上电后，不需任何校准，直接自动进入闭环</summary>

odrv0.axis0.requested_state = AXIS_STATE_MOTOR_CALIBRATION # 进行电机参数校准（运行后电机会发出哔~的一声）
 
odrv0.axis0.motor.config.pre_calibrated = True # 设置电机预校准（不用每次上电都哔~的一声）驱动器会将本次校准值保存，避免上电启动后自动校准，以加快启动速度。

odrv0.axis0.config.startup_motor_calibration = False # 关闭ODrive上电启动后，自动校准电机

odrv0.axis0.requested_state = AXIS_STATE_ENCODER_OFFSET_CALIBRATION # 进行编码器校准（运行后，电机会正转一圈再反转一圈）

odrv0.axis0.encoder.config.pre_calibrated = True # 设置编码器预校准（不用每次上电都右转一圈又左转一圈） 驱动器会将本次校准值保存，避免上电启动后自动校准，以加快启动速度。

odrv0.save_configuration() # 保存参数

odrv0.reboot() # 重启驱动器

</details>

## 问题记录
|问题|解决方案|
|:--:|:--:|
|GUI进不去|安装/重装python3.9.1|

## 其他代码
<details>
<summary>常用ODrive命令</summary>

odrivetool #启动odrivetool

odrv0.vbus_voltage  # 电压检测

odrv0.axis0.error  # 错误检查

dump_errors(odrv0) #查看错误

dump_errors(odrv0, True) #清除错误（如果报错ODrive不会继续执行电机旋转指令）

odrv0.reboot() #重启ODrive

odrv0.axis0.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE #校准电机和编码器

odrv0.axis0.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL #进入闭环控制

odrv0.axis0.motor #检查电机参数设置

odrv0.axis0.encoder #检查编码器参数设置

odrv0.axis0.encoder.shadow_count #读取当前编码器度数
odrv0.axis1.encoder.shadow_count 

quit #退出odrivetool
</details>

<details>
<summary>ODrive0配置代码</summary>
odrv0.axis0.motor.config.current_lim = 30
odrv0.axis0.motor.config.requested_current_range = 70
odrv0.axis0.motor.config.calibration_current = 20
odrv0.axis0.controller.config.vel_limit = 2000
odrv0.config.brake_resistance = 0
odrv0.axis0.motor.config.pole_pairs = 21
odrv0.axis0.motor.config.motor_type = 0
odrv0.axis0.encoder.config.cpr = 4000

odrv0.axis1.motor.config.current_lim = 30
odrv0.axis1.motor.config.requested_current_range = 70
odrv0.axis1.motor.config.calibration_current = 20
odrv0.axis1.controller.config.vel_limit = 2000
odrv0.config.brake_resistance = 0
odrv0.axis1.motor.config.pole_pairs = 21
odrv0.axis1.motor.config.motor_type = 0
odrv0.axis1.encoder.config.cpr = 16384
</details>

<details>
<summary>ODrive0无感模式代码</summary>
<!-- odrv0.axis0.controller.config.vel_gain = 0.01
odrv0.axis0.controller.config.vel_integrator_gain = 0.05
odrv0.axis0.controller.config.control_mode = CONTROL_MODE_VELOCITY_CONTROL
odrv0.axis0.controller.config.vel_limit = <a value greater than axis.config.sensorless_ramp.vel / (2pi * <pole_pairs>)>
odrv0.axis0.motor.config.current_lim = 2 * odrv0.axis0.config.sensorless_ramp.current
odrv0.axis0.sensorless_estimator.config.pm_flux_linkage = 5.51328895422 / (<pole pairs> * <motor kv>)
odrv0.axis0.config.enable_sensorless_mode = True -->

odrv0.axis0.controller.config.vel_gain = 0.01
odrv0.axis0.controller.config.vel_integrator_gain = 0.05
odrv0.axis0.controller.config.control_mode = CONTROL_MODE_VELOCITY_CONTROL
odrv0.axis0.controller.config.vel_limit = 2000
odrv0.axis0.motor.config.current_lim = 2 * odrv0.axis0.config.sensorless_ramp.current
odrv0.axis0.sensorless_estimator.config.pm_flux_linkage = 5.51328895422 / 2100
odrv0.axis0.config.enable_sensorless_mode = True
</details>

## 电机与编码器参数参考
|电机型号|KV值|极对数|
|:--:|:--:|:--:|
|U10|100|21|
|SUNNY SKY|380|7|
