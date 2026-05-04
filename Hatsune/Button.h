#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <graphics.h>

// 按钮类，用于处理界面上的可交互按钮
// 支持三种状态：空闲、悬停、点击
class Button
{
public:
	Button() = default;
	~Button() = default;

	// 绘制按钮（带阴影版本）
	// x, y: 按钮位置坐标
	// strength: 透明度（0-255）
	// img_idle: 空闲状态图片
	// img_hovered: 悬停状态图片
	// img_clicked: 点击状态图片
	void on_draw(int x, int y, BYTE strength, IMAGE* img_idle, IMAGE* img_hovered, IMAGE* img_clicked)
	{
		switch (status)
		{
		case Button::Status::Idle:
			putimage_alpha(x, y, img_idle, nullptr, strength);
			break;
		case Button::Status::Hovered:
			putimage_alpha(x, y, img_hovered, nullptr, strength);
			break;
		case Button::Status::Clicked:
			putimage_alpha(x, y, img_clicked, nullptr, strength);
			break;
		}
	}

	// 绘制按钮（简化版本，使用同一张图片）
	// x, y: 按钮位置坐标
	// strength: 透明度（0-255）
	// img: 按钮图片
	// img_shadow: 阴影图片
	void on_draw(int x, int y, BYTE strength, IMAGE* img, IMAGE* img_shadow)
	{
		switch (status)
		{
		case Button::Status::Idle:
			putimage_alpha(x, y, img, img_shadow, strength);
			break;
		case Button::Status::Hovered:
			// 悬停和点击时稍微偏移，产生按压效果
			putimage_alpha(x + 1, y + 1, img, img_shadow, strength);
			break;
		case Button::Status::Clicked:
			putimage_alpha(x + 1, y + 1, img, img_shadow, strength);
			break;
		}
	}

	// 处理用户输入（鼠标移动和点击）
	// msg: 消息结构体
	// region: 按钮的交互区域
	// 返回值：如果按钮被点击返回true，否则返回false
	bool on_input(const ExMessage& msg, RECT region)
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			// 检测鼠标是否进入或离开按钮区域
			if (status == Status::Idle && CheckMousePosition(region, msg.x, msg.y))
				status = Status::Hovered;  // 鼠标进入，变为悬停状态
			else if (status == Status::Hovered && !CheckMousePosition(region, msg.x, msg.y))
				status = Status::Idle;     // 鼠标离开，恢复空闲状态
			break;
		case WM_LBUTTONDOWN:
			// 鼠标左键按下，如果在按钮区域内则变为点击状态
			if (CheckMousePosition(region, msg.x, msg.y))
				status = Status::Clicked;
			break;
		case WM_LBUTTONUP:
			// 鼠标左键释放
			if (status == Status::Clicked)
			{
				if (CheckMousePosition(region, msg.x, msg.y))
				{
					// 在按钮区域内释放，视为有效点击
					status = Status::Hovered;
					return true;  // 返回true表示按钮被成功点击
				}
				else
				{
					// 在按钮区域外释放，取消点击
					status = Status::Idle;
				}
			}
		}
		return false;  // 未发生有效点击
	}

private:
	// 按钮的状态枚举
	enum class Status
	{
		Idle = 0,      // 空闲状态：鼠标未在按钮上
		Hovered,       // 悬停状态：鼠标在按钮上但未点击
		Clicked        // 点击状态：鼠标按下
	};
	Status status = Status::Idle;  // 当前按钮状态，默认为空闲
};

#endif // !_BUTTON_H_
