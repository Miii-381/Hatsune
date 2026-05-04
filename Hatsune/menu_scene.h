#ifndef _MAIN_SCREEN_H_
#define	_MAIN_SCREEN_H_

#include "ResourcesManager.h"
#include "scene_manager.h"
#include "utilities.h"
#include "button.h"

#include <stdio.h>

extern SceneManager scene_manager;

// 主菜单场景类：游戏的主界面，包含开始、设置、关于三个按钮
class MenuScene : public Scene
{
public:
	MenuScene()
	{
		// 配置淡入动画定时器
		timer_trans_control.restart();
		timer_trans_control.set_wait_time(5);        // 每5毫秒更新一次
		timer_trans_control.set_one_shot(false);     // 循环触发
		timer_trans_control.set_callback([&]()
			{
				trans_strength = min(trans_strength + 1, 255);  // 逐渐增加透明度
				if (trans_strength >= 255)
					timer_trans_control.pause();  // 淡入完成后暂停定时器
			});

		// 计算各按钮的交互区域
		region_start_button.top = pos_img_start_button.y;
		region_start_button.bottom = region_start_button.top + ResourceMgr.img_start_button_idle.getheight();
		region_start_button.left = pos_img_start_button.x;
		region_start_button.right = region_start_button.left + ResourceMgr.img_start_button_idle.getwidth();

		region_setting_button.top = pos_img_setting_button.y;
		region_setting_button.bottom = region_setting_button.top + ResourceMgr.img_setting_button_idle.getheight();
		region_setting_button.left = pos_img_setting_button.x;
		region_setting_button.right = region_setting_button.left + ResourceMgr.img_setting_button_idle.getwidth();

		region_about_button.top = pos_img_about_button.y;
		region_about_button.bottom = region_about_button.top + ResourceMgr.img_about_button_idle.getheight();
		region_about_button.left = pos_img_about_button.x;
		region_about_button.right = region_about_button.left + ResourceMgr.img_about_button_idle.getwidth();
	};

	~MenuScene() = default;

	// 进入主菜单时播放背景音乐
	void on_enter(bool is_debug)
	{
		if (!is_music_playing)
		{
			mciSendString(_T("play menu_scene_BGM repeat from 0"), NULL, 0, NULL);
			is_music_playing = true;
		}
	}

	// 绘制主菜单界面（背景、标题、三个按钮）
	void on_draw(bool is_debug)
	{
		putimage_alpha(0, 0, &ResourceMgr.img_background_menu_scene, nullptr, trans_strength);
		putimage_alpha(pos_img_titie.x, pos_img_titie.y, &ResourceMgr.img_title, nullptr, trans_strength);
		
		// 绘制三个按钮（根据透明度渐变显示）
		start_button.on_draw(pos_img_start_button.x, pos_img_start_button.y, trans_strength,
			&ResourceMgr.img_start_button_idle, &ResourceMgr.img_start_button_hovered, &ResourceMgr.img_start_button_clicked);
		setting_button.on_draw(pos_img_setting_button.x, pos_img_setting_button.y, trans_strength,
			&ResourceMgr.img_setting_button_idle, &ResourceMgr.img_setting_button_hovered, &ResourceMgr.img_setting_button_clicked);
		about_button.on_draw(pos_img_about_button.x, pos_img_about_button.y, trans_strength,
			&ResourceMgr.img_about_button_idle, &ResourceMgr.img_about_button_hovered, &ResourceMgr.img_about_button_clicked);
	}

	// 处理用户输入（按钮点击和ESC键退出）
	void on_input(const ExMessage& msg, bool is_debug)
	{
		// 检测按钮点击状态
		is_start_clicked = start_button.on_input(msg, region_start_button);
		is_setting_clicked = setting_button.on_input(msg, region_setting_button);
		is_about_clicked = about_button.on_input(msg, region_about_button);

		// 处理ESC键退出
		if (msg.message == WM_KEYDOWN)
		{
			if (msg.vkcode == VK_ESCAPE)
			{
				mciSendString(_T("pause menu_scene_BGM"), NULL, 0, NULL);  // 暂停音乐
				int status = MessageBox(GetHWnd(), _T("确认要退出游戏？"), _T("提示~"), MB_OKCANCEL);
				if (status == IDOK)
					SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);  // 关闭窗口
				else
					mciSendString(_T("resume menu_scene_BGM"), NULL, 0, NULL);  // 恢复音乐
			}
		}

		// 处理开始按钮点击：切换到选歌场景
		if (is_start_clicked)
		{
			mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);  // 播放按钮音效
			is_switch_to_setting_scene = false;
			scene_manager.switch_to(SceneManager::SceneType::select_songs_scene);
			is_start_clicked = false;
		}
		
		// 处理设置按钮点击：切换到设置场景
		if (is_setting_clicked)
		{
			mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);  // 播放按钮音效
			is_switch_to_setting_scene = true;
			scene_manager.switch_to(SceneManager::SceneType::setting_scene);
			is_setting_clicked = false;
		}
		
		// 处理关于按钮点击：打开GitHub页面
		if (is_about_clicked)
		{
			mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);  // 播放按钮音效
			system("start https://github.com/Miii-381/Hatsune");
			is_about_clicked = false;
		}
	}

	// 更新主菜单逻辑（处理淡入动画）
	void on_update(int delta, bool is_debug)
	{
		timer_trans_control.on_update(delta);
	}

	// 退出主菜单时停止背景音乐
	void on_exit(bool is_debug)
	{
		trans_strength = 255;  // 重置透明度
		
		// 如果不是切换到设置场景（说明是离开主菜单），则停止音乐
		if (!is_switch_to_setting_scene)
		{
			mciSendString(_T("stop menu_scene_BGM"), NULL, 0, NULL);
			is_music_playing = false;
		}
	}

private:
	BYTE trans_strength = 0;           // 当前透明度（0-255）
	Timer timer_trans_control;         // 淡入动画定时器

	POINT pos_img_titie = { 10, 50 };          // 标题图片位置
	POINT pos_img_start_button = { 0, 300 };   // 开始按钮位置
	POINT pos_img_setting_button = { 0, 420 }; // 设置按钮位置
	POINT pos_img_about_button = { 0, 540 };   // 关于按钮位置

	Button start_button;               // 开始按钮对象
	Button setting_button;             // 设置按钮对象
	Button about_button;               // 关于按钮对象

	RECT region_start_button;          // 开始按钮交互区域
	RECT region_setting_button;        // 设置按钮交互区域
	RECT region_about_button;          // 关于按钮交互区域

	bool is_start_clicked = false;         // 开始按钮是否被点击
	bool is_setting_clicked = false;       // 设置按钮是否被点击
	bool is_about_clicked = false;         // 关于按钮是否被点击
	bool is_switch_to_setting_scene = false;  // 是否切换到设置场景
	bool is_music_playing = false;         // 背景音乐是否正在播放
};

#endif // !_MAIN_SCREEN_H_
