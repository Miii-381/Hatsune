#ifndef _OPENING_SCENE_H_
#define _OPENING_SCENE_H_

#include "scene.h"
#include "scene_manager.h"
#include "utilities.h"
#include "button.h"

#include <stdio.h>

extern SceneManager scene_manager;

extern IMAGE img_background_opening_scene;

class OpeningScene : public Scene
{
public:
	OpeningScene()
	{
		timer_trans_control.restart();				// 我在试图让淡入淡出也与帧率脱钩，但是GetTickCount()的极限就是15~18ms,在循环时长小于16ms，也就是帧率超过60FPS的情况下，也就只有跟着帧率来了......
		timer_trans_control.set_wait_time(5);		// 上述问题使用chrono已解决
		timer_trans_control.set_one_shot(false);
		timer_trans_control.set_callback([&]()		// lambda函数：定义简短，使用方便，就比如这串代码，使用常规函数的话会写得极其抽象，lambda函数直接捕获了当前作用域（也就是这个class）内的成员变量，简单优雅
			{										// lambda函数：[捕获列表](lambda函数需要的参数列表"类似于普通函数的参数列表") mutable -> return_type{函数体}
				if (is_fade_in_progress)			// mutable：加上就可以让lambda函数修改值传递变量的拷贝（不会影响原变量）
				{									// -> return_type：指定lambda函数的返回类型，比如int，double之类的，一般不用写，编译器会帮你搞定（lambda函数如果没有return语句不能指定返回类型，因为其返回类型已经被隐式设置为了void类型，跟常规函数一样）
					trans_strength = min(trans_strength + 1, 255);
					if (trans_strength >= 255)
					{
						is_fade_in_progress = false;
						is_waiting_for_fade_out = true;
					}
				}
				else if (is_waiting_for_fade_out)
				{
					fade_out_delay_counter --;
					if (fade_out_delay_counter <= 0)
					{
						is_waiting_for_fade_out = false;
					}
				}
				else 
				{
					trans_strength = max(trans_strength - 1, 0);
				}
			});
	};

	~OpeningScene() = default;

	void on_draw(bool is_debug)
	{
		putimage_alpha(pos_img_title.x, pos_img_title.y, &img_background_opening_scene, nullptr, trans_strength);
	}

	void on_update(int delta, bool is_debug)
	{
		timer_trans_control.on_update(delta);
		printf("%d %d\n", trans_strength, delta);
		if (!is_fade_in_progress && trans_strength == 0)
			scene_manager.switch_to(SceneManager::SceneType::menu_scene);
	}

	void on_input(const ExMessage& msg, bool is_debug)
	{
		if(msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN)
			scene_manager.switch_to(SceneManager::SceneType::menu_scene);
	}

private:
	POINT pos_img_title = { 0 };				// 标题位置
	BYTE trans_strength = 0;					// 图片透明化强度
	bool is_fade_in_progress = true;			// 淡入
	bool is_waiting_for_fade_out = false;		// 等待淡出
	int fade_out_delay_counter = 100;			// 等待淡出的计数器（配合计时器使用)(当前scene的循环执行时间大概在7ms左右，7*计数器个数就是大概的延迟时间)
	Timer timer_trans_control;
	
};



#endif //!_OPENING_SCENE_H_