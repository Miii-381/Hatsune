#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <graphics.h>
#include <stdio.h>

class Button
{
public:
	Button() = default;
	~Button() = default;

	void on_draw(int x, int y,BYTE strength,IMAGE* img_idle, IMAGE* img_hovered, IMAGE* img_clicked)
	{
		switch (status)
		{
		case Button::Status::Idle:
			putimage_alpha(x, y, img_idle, nullptr, strength);
			if(is_debug) printf("Idle ");
			break;
		case Button::Status::Hovered:
			putimage_alpha(x, y, img_hovered, nullptr, strength);
			if (is_debug) printf("Hovered ");
			break;
		case Button::Status::Clicked:
			putimage_alpha(x, y, img_clicked, nullptr, strength);
			if (is_debug) printf("Clicked ");
			break;
		}
	}

	void on_draw(int x, int y, BYTE strength, IMAGE* img, IMAGE* img_shadow)
	{
		switch (status)
		{
		case Button::Status::Idle:
			putimage_alpha(x, y, img, img_shadow, strength);
			if (is_debug) printf("Idle ");
			break;
		case Button::Status::Hovered:
			putimage_alpha(x + 1, y + 1, img, img_shadow, strength);
			if (is_debug) printf("Hovered ");
			break;
		case Button::Status::Clicked:
			putimage_alpha(x + 1, y + 1, img, img_shadow, strength);
			if (is_debug) printf("Clicked ");
			break;
		}
	}

	bool on_input(const ExMessage& msg, RECT region)
	{
		switch (msg.message)
		{
		case WM_MOUSEMOVE:
			if (status == Status::Idle && CheckMousePosition(region, msg.x, msg.y))
				status = Status::Hovered;
			else if (status == Status::Hovered && !CheckMousePosition(region, msg.x, msg.y))
				status = Status::Idle;
			break;
		case WM_LBUTTONDOWN:
			if (CheckMousePosition(region, msg.x, msg.y))
				status = Status::Clicked;
			break;
		case WM_LBUTTONUP:
			if (status == Status::Clicked)
			{
				if (CheckMousePosition(region, msg.x, msg.y))
				{
					status = Status::Hovered;
					return true;
				}
				else
				{
					status = Status::Idle;
				}
			}
		}
		return false;
	}

private:
	enum class Status
	{
		Idle = 0,
		Hovered,
		Clicked
	};
	Status status = Status::Idle;
};


#endif // !_BUTTON_H_