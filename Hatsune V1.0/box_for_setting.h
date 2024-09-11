#ifndef _BOX_SETTING_H_
#define _BOX_SETTING_H_

#include "Button.h"
#include "utilities.h"

#include <graphics.h>

extern IMAGE img_left_arrow;
extern IMAGE img_right_arrow;
extern IMAGE img_box_masking;
extern LOGFONT font_yahei;

class BoxForSetting
{
public:
	BoxForSetting() = default;
	~BoxForSetting() = default;

	// 左上锚点x、y位置，高度，长度
	void on_enter(int left, int top, int height, int width, int& num, bool isDropSpeed)
	{
		is_drop_speed = isDropSpeed;

		pos_box.top = top; pos_box.bottom = top + height; pos_box.left = left; pos_box.right = left + width;

		pos_left_arrow.x = pos_box.left;
		pos_left_arrow.y = pos_box.top + (pos_box.bottom - pos_box.top - img_left_arrow.getheight()) / 2;

		pos_right_arrow.x = pos_box.right - img_right_arrow.getwidth();
		pos_right_arrow.y = pos_left_arrow.y;

		region_box.top = pos_box.top;
		region_box.bottom = pos_box.bottom;
		region_box.left = pos_box.left;
		region_box.right = pos_box.right;

		region_left_arrow.top = pos_left_arrow.y;
		region_left_arrow.left = pos_left_arrow.x;
		region_left_arrow.right = region_left_arrow.left + img_left_arrow.getwidth();
		region_left_arrow.bottom = region_left_arrow.top + img_left_arrow.getheight();

		region_right_arrow.top = pos_right_arrow.y;
		region_right_arrow.left = pos_right_arrow.x;
		region_right_arrow.right = region_right_arrow.left + img_right_arrow.getwidth();
		region_right_arrow.bottom = region_right_arrow.top + img_right_arrow.getheight();

		str = std::to_string(num);
	
		set_text_x(num);
		pos_text.y = pos_box.top + (pos_box.bottom - pos_box.top - font_yahei.lfHeight) / 2;
	}

	void on_draw()
	{
		putimage_alpha(pos_box.left, pos_box.top, &img_box_masking, 200);
		left_arrow.on_draw(pos_left_arrow.x, pos_left_arrow.y, 255, &img_left_arrow, nullptr);
		right_arrow.on_draw(pos_right_arrow.x, pos_right_arrow.y, 255, &img_right_arrow, nullptr);
		outtextxy_shaded(pos_text.x, pos_text.y, str.c_str());
	}

	void on_input(const ExMessage& msg, int& num)
	{
		is_in_box = CheckMousePosition(region_box, msg.x, msg.y);
		is_left_arrow_clicked = left_arrow.on_input(msg, region_left_arrow);
		is_right_arrow_clicked = right_arrow.on_input(msg, region_right_arrow);

		if (msg.message == WM_MOUSEWHEEL && is_in_box)
		{
			if (wheel_msg_temp != msg.wheel)
			{
				int temp =  (msg.wheel - wheel_msg_temp) / 120;
				if(is_drop_speed)
					num += (1 * temp);
				else
					num += (10 * temp);
				str = std::to_string(num);
				set_text_x(num);
				std::cout << num << "  ";
			}
			is_in_box = false;
		}
		
		if (is_left_arrow_clicked)
		{
			if (is_drop_speed)
				num -= 1;
			else
				num -= 10;
			str = std::to_string(num);
			set_text_x(num);
			is_left_arrow_clicked = false;
		}
		if(is_right_arrow_clicked)
		{
			if (is_drop_speed)
				num += 1;
			else
				num += 10;
			str = std::to_string(num);
			set_text_x(num);
			is_right_arrow_clicked = false;
		}

		if (drop_speed > 100)
		{
			drop_speed = 100;
		}

		if (drop_speed < 1)
		{
			drop_speed = 1;
		}
	}

	int GetHeight() const
	{
		return pos_box.bottom - pos_box.top;
	}

	int GetWidth() const
	{
		return pos_box.right - pos_box.left;
	}

private:
	void set_text_x(int num)
	{
		if (num >= 10 && num < 100 || num > -10 && num < 0)
			pos_text.x = pos_left_arrow.x + img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - img_left_arrow.getwidth()) / 2 - 8;
		else if (num >= 100 && num < 1000 || num <= -10 && num > -100)
			pos_text.x = pos_left_arrow.x + img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - img_left_arrow.getwidth()) / 2 - 16;
		else if (num >= 1000 && num < 10000 || num <= -100 && num > -1000)
			pos_text.x = pos_left_arrow.x + img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - img_left_arrow.getwidth()) / 2 - 24;
		else if (num >= 10000 && num < 100000 || num <= -1000 && num > -10000)
			pos_text.x = pos_left_arrow.x + img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - img_left_arrow.getwidth()) / 2 - 32;
		else if (num <= -10000 && num > -100000)
			pos_text.x = pos_left_arrow.x + img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - img_left_arrow.getwidth()) / 2 - 40;
		else
			pos_text.x = pos_left_arrow.x + img_left_arrow.getwidth() + (pos_right_arrow.x - pos_left_arrow.x - img_left_arrow.getwidth()) / 2;
	}

private:
	RECT pos_box = { };
	RECT region_box = { };
	Button left_arrow;
	Button right_arrow;
	POINT pos_left_arrow = { };
	POINT pos_right_arrow = { };
	POINT pos_text = { };
	RECT region_left_arrow = { };
	RECT region_right_arrow = { };
	bool is_in_box = false;
	bool is_left_arrow_clicked = false;
	bool is_right_arrow_clicked = false;
	bool is_drop_speed = false;
	long wheel_msg_temp = 0;
	std::string str;
};


#endif //! _BOX_SETTING_H_
