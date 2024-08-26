#pragma once

/// @brief 读取 D 变量
/// @param index
/// @return
int DD(int index);

/// @brief 设置 D 变量
/// @param index
/// @param value
void DD(int index, int value);

float DF(int index);
void DF(int index, float value);

/// @brief 读取伺服参数
/// @param group 组号。例如对于参数 P1.01 ，组号就是 1 ，而不是 0 。
/// @param index 索引。例如对于参数 P1.01 ，索引就是 1 ，而不是 0 。
/// @return
int SRV_PARA(int group, int index);

/// @brief 设置伺服参数
/// @param group 组号
/// @param index 索引
/// @param value 要设置的值
void SRV_PARA(int group, int index, int value);

/// @brief 设置伺服参数
/// @param group 组号
/// @param index 索引
/// @param value 要设置的值
/// @param write_to_eerom 是否将值写入 EEROM
void SRV_PARA(int group, int index, int value, bool write_to_eerom);

/// @brief 读取伺服报警值。
/// @return
int SRV_ALM();

/// @brief 读取伺服监控数据
/// @param index 要读取的监控数据的索引。索引从 0 开始。对于说明书上的监控数据 1，这里需要传入 0.
/// @return 监控数据。
int SRV_MON(int index);

/// @brief 设置 EI 的值。
/// @param index EI 的索引。例如 EI1 的索引是 1.
/// @param value 要设置给 EI 的值。true 表示让 EI 开，false 表示让 EI 关。
void SRV_EI(int index, bool value);

void AXIS_SPEED(double value);

void DELAY(int ms);
