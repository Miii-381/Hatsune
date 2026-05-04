#ifndef _BOX_SETTING_H_
#define _BOX_SETTING_H_

#include "ResourcesManager.h"
#include "Button.h"
#include "utilities.h"

#include <graphics.h>

// 设置界面中的数值调节框类
// 用于在设置界面中调节各种数值参数（如下落速度、偏移量等）
class BoxForSetting
{
public:
	BoxForSetting() = default;
	~BoxForSetting() = default;

	// 初始化调节框的位置和参数
	// left: 左边界位置, top: 上边界位置, height: 高度, width: 宽度
	// num: 要显示的数值引用, isDropSpeed: 是否为下落速度类型（影响调节步长和范围）
	void on_enter(int left, int top, int height, int width, int& num, bool isDropSpeed)
	{
		is_drop_speed = isDropSpeed;

		// 设置调节框的边界坐标
		pos_box.top = top; pos_box.bottom = top + height; pos_box.left = left; pos_box.right = left + width;

		// 计算左箭头按钮的位置（居中对齐）
		pos_left_arrow.x = pos_box.left;
		pos_left_arrow.y = pos_box.top + (pos_box.bottom - pos_box.top - ResourceMgr.img_left_arrow.getheight()) / 2;

		// 计算右箭头按钮的位置（靠右对齐）
		pos_right_arrow.x = pos_box.right - ResourceMgr.img_right_arrow.getwidth();
		pos_right_arrow.y = pos_left_arrow.y;

		// 设置整个调节框的交互区域
		region_box.top = pos_box.top;
		region_box.bottom = pos_box.bottom;
		region_box.left = pos_box.left;
		region_box.right = pos_box.right;

		// 设置左箭头的交互区域
		region_left_arrow.top = pos_left_arrow.y;
		region_left_arrow.left = pos_left_arrow.x;
		region_left_arrow.right = region_left_arrow.left + ResourceMgr.img_left_arrow.getwidth();
		region_left_arrow.bottom = region_left_arrow.top + ResourceMgr.img_left_arrow.getheight();

		// 设置右箭头的交互区域
		region_right_arrow.top = pos_right_arrow.y;
		region_right_arrow.left = pos_right_arrow.x;
		region_right_arrow.right = region_right_arrow.left + ResourceMgr.img_right_arrow.getwidth();
		region_right_arrow.bottom = region_right_arrow.top + ResourceMgr.img_right_arrow.getheight();

		// 将数值转换为字符串显示
		str = std::to_string(num);

		// 根据数值位数设置文本的水平位置
		set_text_x(num);
		// 计算文本的垂直位置（居中显示）
		pos_text.y = pos_box.top + (pos_box.bottom - pos_box.top - ResourceMgr.font_yahei.lfHeight) / 2;
	}

	// 绘制调节框及其内容
	void on_draw()
	{
		// 绘制半透明的背景遮罩
		putimage_alpha(pos_box.left, pos_box.top, &ResourceMgr.img_box_masking, nullptr, 200);
		// 绘制左箭头按钮
		left_arrow.on_draw(pos_left_arrow.x, pos_left_arrow.y, 255, &ResourceMgr.img_left_arrow, nullptr);
		// 绘制右箭头按钮
		right_arrow.on_draw(pos_right_arrow.x, pos_right_arrow.y, 255, &ResourceMgr.img_right_arrow, nullptr);
		// 绘制带阴影效果的数值文本
		outtextxy_shaded(pos_text.x, pos_text.y, str.c_str());
	}

	// 处理用户输入（鼠标点击和滚轮）
	// msg: 消息结构体, num: 要修改的数值引用
	void on_input(const ExMessage& msg, int& num)
	{
		// 检测鼠标是否在调节框区域内
		is_in_box = CheckMousePosition(region_box, msg.x, msg.y);
		// 检测左右箭头是否被点击
		is_left_arrow_clicked = left_arrow.on_input(msg, region_left_arrow);
		is_right_arrow_clicked = right_arrow.on_input(msg, region_right_arrow);

		// 处理鼠标滚轮事件（仅在鼠标位于调节框内时响应）
		if (msg.message == WM_MOUSEWHEEL && is_in_box)
		{
			// 防止重复处理同一滚轮事件
			if (wheel_msg_temp != msg.wheel)
			{
				// 计算滚轮滚动方向和幅度（120为一格）
				int temp = (msg.wheel - wheel_msg_temp) / 120;
				// 根据类型决定调节步长：下落速度每次调1，其他每次调10
				if (is_drop_speed)
					num += (1 * temp);
				else
					num += (10 * temp);
				// 更新显示的文本
				str = std::to_string(num);
				set_text_x(num);
				std::cout << num << "  ";
			}
			is_in_box = false;
		}

		// 处理左箭头点击（减小数值）
		if (is_left_arrow_clicked)
		{
			// 根据类型决定减小步长
			if (is_drop_speed)
				num -= 1;
			else
				num -= 10;
			str = std::to_string(num);
			set_text_x(num);
			is_left_arrow_clicked = false;
		}
		
		// 处理右箭头点击（增加数值）
		if (is_right_arrow_clicked)
		{
			// 根据类型决定增加步长
			if (is_drop_speed)
				num += 1;
			else
				num += 10;
			str = std::to_string(num);
			set_text_x(num);
			is_right_arrow_clicked = false;
		}

		// 检查数值是否超出范围并进行限制
		bounds_check(num);
	}

	// 获取调节框的高度
	int GetHeight() const
	{
		return pos_box.bottom - pos_box.top;
	}

	// 获取调节框的宽度
	int GetWidth() const
	{
		return pos_box.right - pos_box.left;
	}

private:
	// 检查并限制数值在有效范围内
	void bounds_check(int& num)
	{
		// 下落速度的范围：1-100
		if (is_drop_speed)
		{
			if (num > 100) num = 100;
			if (num < 1) num = 1;
		}
		// 其他参数的范围：-2000到2000
		else
		{
			if (num > 2000) num = 2000;
			if (num < -2000) num = -2000;
		}
	}

	// 根据数值的位数调整文本的水平位置，使其居中显示
	void set_text_x(int num)
	{
		// 根据数值位数动态调整文本位置，确保视觉居中
		if (num >= 10 && num < 100 || num > -10 && num < 0)
			pos_text.x = pos_left_arrow.x + ResourceMgr.img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - ResourceMgr.img_left_arrow.getwidth()) / 2 - 8;
		else if (num >= 100 && num < 1000 || num <= -10 && num > -100)
			pos_text.x = pos_left_arrow.x + ResourceMgr.img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - ResourceMgr.img_left_arrow.getwidth()) / 2 - 16;
		else if (num >= 1000 && num < 10000 || num <= -100 && num > -1000)
			pos_text.x = pos_left_arrow.x + ResourceMgr.img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - ResourceMgr.img_left_arrow.getwidth()) / 2 - 24;
		else if (num >= 10000 && num < 100000 || num <= -1000 && num > -10000)
			pos_text.x = pos_left_arrow.x + ResourceMgr.img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - ResourceMgr.img_left_arrow.getwidth()) / 2 - 32;
		else if (num <= -10000 && num > -100000)
			pos_text.x = pos_left_arrow.x + ResourceMgr.img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - ResourceMgr.img_left_arrow.getwidth()) / 2 - 40;
		else
			pos_text.x = pos_left_arrow.x + ResourceMgr.img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - ResourceMgr.img_left_arrow.getwidth()) / 2;
	}

private:
	RECT pos_box = {};                    // 调节框的位置坐标
	RECT region_box = {};                 // 调节框的交互区域
	Button left_arrow;                    // 左箭头按钮对象
	Button right_arrow;                   // 右箭头按钮对象
	POINT pos_left_arrow = {};            // 左箭头的位置坐标
	POINT pos_right_arrow = {};           // 右箭头的位置坐标
	POINT pos_text = {};                  // 文本的位置坐标
	RECT region_left_arrow = {};          // 左箭头的交互区域
	RECT region_right_arrow = {};         // 右箭头的交互区域
	bool is_in_box = false;               // 鼠标是否在调节框内
	bool is_left_arrow_clicked = false;   // 左箭头是否被点击
	bool is_right_arrow_clicked = false;  // 右箭头是否被点击
	bool is_drop_speed = false;           // 是否为下落速度类型
	long wheel_msg_temp = 0;              // 上次滚轮消息的值（用于防抖）
	std::string str;                      // 当前显示的数值字符串
};

#endif //! _BOX_SETTING_H_
