#ifndef _SETTING_SCENE_H_
#define _SETTING_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "utilities.h"
#include "box_for_setting.h"

#include <stdio.h>

extern int drop_speed;
extern int offset;

extern SceneManager scene_manager;

extern IMAGE img_background_setting_scene;
extern IMAGE img_setting_masking;
extern IMAGE img_exit_button_idle;
extern IMAGE img_exit_button_hovered;
extern IMAGE img_exit_button_clicked;

extern LOGFONT font_yahei;

class SettingScene : public Scene
{
public:
	SettingScene() = default;
	~SettingScene() = default;

	void on_enter(bool is_debug)
	{
		drop_speed_box.on_enter(pos_drop_speed_box.x, pos_drop_speed_box.y, 100, 200, drop_speed, true);
		offset_box.on_enter(pos_offset_box.x, pos_offset_box.y, 100, 200, offset, false);

		region_exit_button.top = pos_exit_button.y;
		region_exit_button.bottom = region_exit_button.top + img_exit_button_idle.getheight();
		region_exit_button.left = pos_exit_button.x;
		region_exit_button.right = region_exit_button.left + img_exit_button_idle.getwidth();

		pos_drop_speed_text.x = pos_drop_speed_box.x - 200;
		pos_offset_text.x = pos_offset_box.x - 200;
		pos_drop_speed_text.y = pos_drop_speed_box.y + (drop_speed_box.GetHeight() - font_yahei.lfHeight) / 2;
		pos_offset_text.y = pos_offset_box.y + (offset_box.GetHeight() - font_yahei.lfHeight) / 2;
	}

	void on_draw(bool is_debug)
	{ 
		printf("setting_scene\n");
		putimage_alpha(0, 0, &img_background_setting_scene);
		putimage_alpha(165, 93, &img_setting_masking);
		exit_button.on_draw(pos_exit_button.x, pos_exit_button.y, 255, &img_exit_button_idle, &img_exit_button_hovered, &img_exit_button_clicked);
		drop_speed_box.on_draw();
		offset_box.on_draw();
		outtextxy_shaded(pos_drop_speed_text.x, pos_drop_speed_text.y, _T("Note掉落速度"));
		outtextxy_shaded(pos_offset_text.x, pos_offset_text.y, _T("判定偏移量(ms)"));
		outtextxy_shaded(pos_tips.x, pos_tips.y + font_yahei.lfHeight + 5, _T("判定偏移量：调小Note出现更快，调大Note出现更慢"));
		outtextxy_shaded(pos_tips.x, pos_tips.y, _T("可按下箭头调节，也可将鼠标移至对应位置滚动滚轮调节"));
	}

	void on_input(const ExMessage& msg, bool is_debug)
	{ 
		is_exit_clicked = exit_button.on_input(msg, region_exit_button);
		if (msg.message == WM_KEYDOWN || is_exit_clicked)
		{
			if (msg.vkcode == VK_ESCAPE || is_exit_clicked)
				scene_manager.switch_to_last_scene(is_debug);
			is_exit_clicked = false;
		}
		
		drop_speed_box.on_input(msg, drop_speed);
		offset_box.on_input(msg, offset);
	}

private:
	BoxForSetting drop_speed_box;
	BoxForSetting offset_box;

	POINT pos_drop_speed_box = { 670,200 };
	POINT pos_offset_box = { 670,320 };

	Button exit_button = { };
	POINT pos_exit_button = { };
	RECT region_exit_button = { };

	POINT pos_drop_speed_text = { };
	POINT pos_offset_text = { };
	POINT pos_tips = { 370,475 };

	bool is_exit_clicked = false;
};








#endif // !_SETTING_SCENE_H_
