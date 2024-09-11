#ifndef _MAIN_SCREEN_H_
#define	_MAIN_SCREEN_H_

#include "scene_manager.h"
#include "utilities.h"
#include "button.h"

#include <stdio.h>

extern SceneManager scene_manager;

extern IMAGE img_background_menu_scene;
extern IMAGE img_title;

extern IMAGE img_start_button_idle;
extern IMAGE img_start_button_hovered;
extern IMAGE img_start_button_clicked;
extern IMAGE img_setting_button_idle;
extern IMAGE img_setting_button_hovered;
extern IMAGE img_setting_button_clicked;
extern IMAGE img_about_button_idle;
extern IMAGE img_about_button_hovered;
extern IMAGE img_about_button_clicked;

class MenuScene : public Scene
{
public:
	MenuScene()
	{
		timer_trans_control.restart();				
		timer_trans_control.set_wait_time(5);		
		timer_trans_control.set_one_shot(false);
		timer_trans_control.set_callback([&]()		
			{	
				trans_strength = min(trans_strength + 1, 255);
				if (trans_strength >= 255)
					timer_trans_control.pause();
			});

		region_start_button.top = pos_img_start_button.y;
		region_start_button.bottom = region_start_button.top + img_start_button_idle.getheight();
		region_start_button.left = pos_img_start_button.x;
		region_start_button.right = region_start_button.left + img_start_button_idle.getwidth();

		region_setting_button.top = pos_img_setting_button.y;
		region_setting_button.bottom = region_setting_button.top + img_setting_button_idle.getheight();
		region_setting_button.left = pos_img_setting_button.x;
		region_setting_button.right = region_setting_button.left + img_setting_button_idle.getwidth();

		region_about_button.top = pos_img_about_button.y;
		region_about_button.bottom = region_about_button.top + img_about_button_idle.getheight();
		region_about_button.left = pos_img_about_button.x;
		region_about_button.right = region_about_button.left + img_about_button_idle.getwidth();
	};

	~MenuScene() = default;

	void on_enter(bool is_debug)
	{
		if (!is_music_playing)
		{
			mciSendString(_T("play menu_scene_BGM repeat from 0"), NULL, 0, NULL);
			is_music_playing = true;
		}
	}

	void on_draw(bool is_debug)
	{ 
		putimage_alpha(0, 0, &img_background_menu_scene, trans_strength);
		putimage_alpha(pos_img_titie.x, pos_img_titie.y, &img_title, trans_strength);
		start_button.on_draw(pos_img_start_button.x, pos_img_start_button.y, trans_strength,
			&img_start_button_idle, &img_start_button_hovered, &img_start_button_clicked);
		setting_button.on_draw(pos_img_setting_button.x, pos_img_setting_button.y, trans_strength,
			&img_setting_button_idle, &img_setting_button_hovered, &img_setting_button_clicked);
		about_button.on_draw(pos_img_about_button.x,pos_img_about_button.y,trans_strength,
			&img_about_button_idle, &img_about_button_hovered, &img_about_button_clicked);
		printf("menu_scene\n");
	}

	void on_input(const ExMessage& msg, bool is_debug)
	{
		is_start_clicked = start_button.on_input(msg, region_start_button);
		is_setting_clicked = setting_button.on_input(msg, region_setting_button);
		is_about_clicked = about_button.on_input(msg, region_about_button);

		if (msg.message == WM_KEYDOWN)
		{
			if (msg.vkcode == VK_ESCAPE)
			{
				MessageBox(GetHWnd(), _T("确定要退出吗？"), _T("呜呜呜~"), MB_OKCANCEL);
				if (IDOK)
					SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
				if (IDCANCEL)
					DestroyWindow(GetHWnd());
			}
		}
		
		if (is_start_clicked)
		{
			mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);
			is_switch_to_setting_scene = false;
			scene_manager.switch_to(SceneManager::SceneType::select_songs_scene);
			is_start_clicked = false;
		}
		if (is_setting_clicked)
		{
			mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);
			is_switch_to_setting_scene = true;
			scene_manager.switch_to(SceneManager::SceneType::setting_scene);
			is_setting_clicked = false;
		}
		if (is_about_clicked)
		{
			mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);
			// 调用系统命令以默认浏览器打开URL
			system("start https://github.com/Miii-381/Hatsune");
			is_about_clicked = false;
		}
	}

	void on_update(int delta, bool is_debug)
	{
		printf("%d %d ", delta, trans_strength);
		timer_trans_control.on_update(delta);
	}

	void on_exit(bool is_debug)
	{
		trans_strength = 255;
		if (!is_switch_to_setting_scene)
		{
			mciSendString(_T("stop menu_scene_BGM"), NULL, 0, NULL); 
			is_music_playing = false;
		}
	}

private:
	BYTE trans_strength = 0;					// 背景透明度控制
	Timer timer_trans_control;

	POINT pos_img_titie = { 10,50 };			// 标题位置
	POINT pos_img_start_button = { 0,300 };		// 开始按键位置
	POINT pos_img_setting_button = { 0,420 };	// 设置按键位置
	POINT pos_img_about_button = { 0,540 };		// 关于按键位置

	Button start_button;						// 开始按键
	Button setting_button;						// 设置按键
	Button about_button;						// 关于按键

	RECT region_start_button;					// 开始按键判定范围 
	RECT region_setting_button;					// 设置按键判定范围
	RECT region_about_button;					// 关于按键判定范围

	bool is_start_clicked = false;				// 开始按键是否被按下
	bool is_setting_clicked = false;			// 设置按键是否被按下
	bool is_about_clicked = false;				// 关于按键是否被按下
	bool is_switch_to_setting_scene = false;	// 是否切换到设置界面
	bool is_music_playing = false;
};


#endif // !_MAIN_SCREEN_H_

