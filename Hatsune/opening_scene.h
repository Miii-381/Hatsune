#ifndef _OPENING_SCENE_H_
#define _OPENING_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "ResourcesManager.h"
#include "utilities.h"
#include "button.h"

#include <stdio.h>

extern SceneManager scene_manager;

// 开场场景类：游戏启动时显示的淡入淡出动画场景
class OpeningScene : public Scene
{
public:
	OpeningScene()
	{
		// 配置过渡动画定时器
		timer_trans_control.restart();
		timer_trans_control.set_wait_time(5);        // 每5毫秒更新一次
		timer_trans_control.set_one_shot(false);     // 循环触发
		timer_trans_control.set_callback([&]()
			{
				// 淡入阶段：逐渐增加透明度
				if (is_fade_in_progress)
				{
					trans_strength = min(trans_strength + 1, 255);
					if (trans_strength >= 255)
					{
						is_fade_in_progress = false;    // 淡入完成
						is_waiting_for_fade_out = true; // 开始等待淡出
					}
				}
				// 等待阶段：倒计时
				else if (is_waiting_for_fade_out)
				{
					fade_out_delay_counter--;
					if (fade_out_delay_counter <= 0)
					{
						is_waiting_for_fade_out = false; // 等待结束，开始淡出
					}
				}
				// 淡出阶段：逐渐减少透明度
				else
				{
					trans_strength = max(trans_strength - 1, 0);
				}
			});
	};

	~OpeningScene() = default;

	// 绘制开场场景（显示背景图和标题）
	void on_draw(bool is_debug)
	{
		putimage_alpha(pos_img_title.x, pos_img_title.y, &ResourceMgr.img_background_opening_scene, nullptr, trans_strength);
	}

	// 更新开场场景逻辑（处理淡入淡出动画）
	void on_update(int delta, bool is_debug)
	{
		timer_trans_control.on_update(delta);
		
		// 如果淡出完成（透明度为0），切换到主菜单场景
		if (!is_fade_in_progress && trans_strength == 0)
			scene_manager.switch_to(SceneManager::SceneType::menu_scene);
	}

	// 处理用户输入（任意按键或点击可跳过动画）
	void on_input(const ExMessage& msg, bool is_debug)
	{
		if (msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN)
			scene_manager.switch_to(SceneManager::SceneType::menu_scene);
	}

private:
	POINT pos_img_title = { 0 };   // 标题图片位置
	BYTE trans_strength = 0;       // 当前透明度（0-255）
	bool is_fade_in_progress = true;  // 是否正在淡入
	bool is_waiting_for_fade_out = false;  // 是否在等待淡出
	int fade_out_delay_counter = 100;      // 淡出前的等待计数器
	Timer timer_trans_control;             // 过渡动画定时器
};

#endif //!_OPENING_SCENE_H_
