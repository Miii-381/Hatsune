#ifndef _SETTING_SCENE_H_
#define _SETTING_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "ResourcesManager.h"
#include "utilities.h"
#include "box_for_setting.h"

#include <stdio.h>

extern SceneManager scene_manager;

// 设置场景类：提供游戏参数调节功能（下落速度、判定偏移量）
class SettingScene : public Scene
{
public:
	SettingScene() = default;
	~SettingScene() = default;

	// 进入设置场景时初始化调节框
	void on_enter(bool is_debug)
	{
		// 初始化下落速度调节框
		drop_speed_box.on_enter(pos_drop_speed_box.x, pos_drop_speed_box.y, 100, 200, ResourceMgr.drop_speed, true);
		
		// 初始化判定偏移量调节框
		offset_box.on_enter(pos_offset_box.x, pos_offset_box.y, 100, 200, ResourceMgr.offset, false);

		// 计算退出按钮的交互区域
		region_exit_button.top = pos_exit_button.y;
		region_exit_button.bottom = region_exit_button.top + ResourceMgr.img_exit_button_idle.getheight();
		region_exit_button.left = pos_exit_button.x;
		region_exit_button.right = region_exit_button.left + ResourceMgr.img_exit_button_idle.getwidth();

		// 计算文本标签位置（在调节框左侧）
		pos_drop_speed_text.x = pos_drop_speed_box.x - 200;
		pos_offset_text.x = pos_offset_box.x - 200;
		pos_drop_speed_text.y = pos_drop_speed_box.y + (drop_speed_box.GetHeight() - ResourceMgr.font_yahei.lfHeight) / 2;
		pos_offset_text.y = pos_offset_box.y + (offset_box.GetHeight() - ResourceMgr.font_yahei.lfHeight) / 2;
	}

	// 绘制设置界面（背景、遮罩、调节框、文本标签）
	void on_draw(bool is_debug)
	{
		putimage_alpha(0, 0, &ResourceMgr.img_background_setting_scene);
		putimage_alpha(165, 93, &ResourceMgr.img_setting_masking);
		
		// 绘制退出按钮
		exit_button.on_draw(pos_exit_button.x, pos_exit_button.y, 255, 
			&ResourceMgr.img_exit_button_idle, &ResourceMgr.img_exit_button_hovered, &ResourceMgr.img_exit_button_clicked);
		
		// 绘制两个数值调节框
		drop_speed_box.on_draw();
		offset_box.on_draw();
		
		// 绘制文本标签和提示信息
		outtextxy_shaded(pos_drop_speed_text.x, pos_drop_speed_text.y, _T("Note下落速度"));
		outtextxy_shaded(pos_offset_text.x, pos_offset_text.y, _T("判定偏移量(ms)"));
		outtextxy_shaded(pos_tips.x, pos_tips.y + ResourceMgr.font_yahei.lfHeight + 5, 
			_T("判定偏移量越小Note出现越快，越大Note出现越慢"));
		outtextxy_shaded(pos_tips.x, pos_tips.y, 
			_T("可点击箭头按钮，也可将鼠标放在相应位置滚动滚轮调节数值"));
	}

	// 处理用户输入（调节数值、退出设置）
	void on_input(const ExMessage& msg, bool is_debug)
	{
		// 检测退出按钮点击
		is_exit_clicked = exit_button.on_input(msg, region_exit_button);
		
		if (msg.message == WM_KEYDOWN || is_exit_clicked)
		{
			// ESC键或点击退出按钮返回上一场景
			if (msg.vkcode == VK_ESCAPE || is_exit_clicked)
				scene_manager.switch_to_last_scene(is_debug);
			is_exit_clicked = false;
		}

		// 处理两个调节框的输入
		drop_speed_box.on_input(msg, ResourceMgr.drop_speed);
		offset_box.on_input(msg, ResourceMgr.offset);
	}

private:
	BoxForSetting drop_speed_box;  // 下落速度调节框
	BoxForSetting offset_box;      // 判定偏移量调节框

	POINT pos_drop_speed_box = { 670, 200 };  // 下落速度调节框位置
	POINT pos_offset_box = { 670, 320 };      // 判定偏移量调节框位置

	Button exit_button = {};        // 退出按钮对象
	POINT pos_exit_button = {};     // 退出按钮位置
	RECT region_exit_button = {};   // 退出按钮交互区域

	POINT pos_drop_speed_text = {}; // 下落速度文本位置
	POINT pos_offset_text = {};     // 判定偏移量文本位置
	POINT pos_tips = { 370, 475 };  // 提示信息位置

	bool is_exit_clicked = false;   // 退出按钮是否被点击
};

#endif // !_SETTING_SCENE_H_
