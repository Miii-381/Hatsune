#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include <ctime>
#include <functional>

// 定时器类：用于处理延迟执行、周期性回调等定时任务
class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	// 重启定时器（重置已过去时间）
	void restart()
	{
		pass_time = 0;
		shotted = false;
	}

	// 设置等待时间（毫秒）
	// val: 目标等待时间
	void set_wait_time(int val)
	{
		wait_time = val;
	}

	// 设置是否为单次触发模式
	// flag: true为单次触发，false为循环触发
	void set_one_shot(bool flag)
	{
		one_shot = flag;
	}

	// 设置无参数回调函数
	// callback: 要执行的回调函数
	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	// 设置带整数参数的回调函数（参数表示触发的次数）
	// callback: 要执行的回调函数
	void set_callback(std::function<void(int)> callback)
	{
		this->callback_int = callback;
	}

	// 暂停定时器
	void pause()
	{
		paused = true;
	}

	// 恢复定时器
	void resume()
	{
		paused = false;
	}

	// 获取定时器的暂停状态
	// 返回值：true表示已暂停，false表示运行中
	bool get_status() const
	{
		return paused;
	}

	// 每帧更新定时器状态
	// delta: 帧时间间隔（毫秒）
	void on_update(int delta)
	{
		if (paused)
			return;

		int time = 0;
		pass_time += delta;           // 累加已过时间
		time += delta / wait_time;    // 计算触发次数

		// 如果已过时间达到等待时间，则触发回调
		if (pass_time >= wait_time)
		{
			// 检查是否应该触发（非单次模式或单次模式但未触发过）
			if ((!one_shot || (one_shot && !shotted)) && callback)
				for (int i = 0; i < time; i++)
				{
					callback();       // 调用无参数回调
				}
			else if ((!one_shot || (one_shot && !shotted)) && callback_int)
				callback_int(time);   // 调用带参数回调（传入触发次数）
			
			shotted = true;           // 标记为已触发
			pass_time = 0;            // 重置已过时间
		}
	}

private:
	int pass_time = 0;                          // 已经过去的时间（毫秒）
	int wait_time = 0;                          // 目标等待时间（毫秒）
	bool paused = false;                        // 是否已暂停
	bool shotted = false;                       // 是否已触发过（用于单次模式）
	bool one_shot = false;                      // 是否为单次触发模式
	std::function<void()> callback;             // 无参数回调函数
	std::function<void(int)> callback_int;      // 带整数参数的回调函数
};

#endif
