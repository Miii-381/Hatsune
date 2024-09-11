#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include <ctime>
#include <functional>


class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void restart()										//定时器的状态重置
	{
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(int val)							// 定时器等待时间
	{
		wait_time = val;
	}

	void set_one_shot(bool flag)						// 定时器是否单次触发
	{
		one_shot = flag;
	}

	void set_callback(std::function<void()> callback)	// 设置自定义的回调函数（这里使用std::function函数接受一个无参，返回值为void类型的任何可调用对象，并将其存放在callback成员变量中等待调用）
	{
		this->callback = callback;	// 此处使用this是为了明确指出操作的是当前对象的callback成员变量，而不是形参
	}

	void set_callback(std::function<void(int)> callback)	// 设置自定义的回调函数（这里使用std::function函数接受有一个参数，返回值为void类型的任何可调用对象，并将其存放在callback成员变量中等待调用）
	{
		this->callback_int = callback;	// 此处使用this是为了明确指出操作的是当前对象的callback成员变量，而不是形参
	}

	void pause()										// 计时器的暂停
	{
		paused = true;
	}

	void resume()										// 计时器的恢复运行
	{
		paused = false;
	}

	bool get_status() const
	{
		return paused;
	}

	void on_update(int delta)							// 计时器更新
	{
		if (paused)					// 检查是否暂停，暂停就不执行计时器时间更新
			return;

		int time = 0;
		pass_time += delta;			// 累加帧更新实际过去的时间
		time += delta / wait_time; 	// 计算在过去的时间里需要重复几次定时器

		if (pass_time >= wait_time) // 计时器时间与期望定时进行比较
		{
			if ((!one_shot || (one_shot && !shotted)) && callback)
				for (int i = 0; i < time; i++)
				{
					callback();
				}  // 如果不是一次性的，或一次性但未触发（总之就是定时器是否能被触发），并且callback不为空（std::function对象可被隐式转换为bool值），则尝试按照重复次数执行赋予的回调函数
			else if ((!one_shot || (one_shot && !shotted)) && callback_int)
				callback_int(time);// 如果不是一次性的，或一次性但未触发（总之就是定时器是否能被触发），并且callback_int不为空（std::function对象可被隐式转换为bool值），则尝试按照重复次数执行赋予的回调函数
			shotted = true;	 // 触发标记
			pass_time = 0;   // 重置定时器
		}
	}

private:
	int pass_time = 0;					// 已过时间
	int wait_time = 0;					// 等待时间
	bool paused = false;				// 是否暂停
	bool shotted = false;				// 是否触发
	bool one_shot = false;				// 单次触发
	std::function<void()> callback;		// 存储程序员手动赋予的回调函数（std::function其实可以看作是一个函数指针的高级和灵活的替代品（虽然它不只可以存放函数），设置一个变量，变量类型属于function模板，
										// 这个变量现在可被认为是一个能够存储任何符合其模板参数指定签名（即参数类型和返回类型）的可调用对象的容器，等待接受一个可调用对象，并在需要时使用这个变量名调用存放的对象）
	std::function<void(int)> callback_int;		
};

#endif // !_TIMER_H_
