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
		timer_trans_control.restart();				// ������ͼ�õ��뵭��Ҳ��֡���ѹ�������GetTickCount()�ļ��޾���15~18ms,��ѭ��ʱ��С��16ms��Ҳ����֡�ʳ���60FPS������£�Ҳ��ֻ�и���֡������......
		timer_trans_control.set_wait_time(5);		// ��������ʹ��chrono�ѽ��
		timer_trans_control.set_one_shot(false);
		timer_trans_control.set_callback([&]()		// lambda�����������̣�ʹ�÷��㣬�ͱ����⴮���룬ʹ�ó��溯���Ļ���д�ü������lambda����ֱ�Ӳ����˵�ǰ������Ҳ�������class���ڵĳ�Ա������������
			{										// lambda������[�����б�](lambda������Ҫ�Ĳ����б�"��������ͨ�����Ĳ����б�") mutable -> return_type{������}
				if (is_fade_in_progress)			// mutable�����ϾͿ�����lambda�����޸�ֵ���ݱ����Ŀ���������Ӱ��ԭ������
				{									// -> return_type��ָ��lambda�����ķ������ͣ�����int��double֮��ģ�һ�㲻��д�������������㶨��lambda�������û��return��䲻��ָ���������ͣ���Ϊ�䷵�������Ѿ�����ʽ����Ϊ��void���ͣ������溯��һ����
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
	POINT pos_img_title = { 0 };				// ����λ��
	BYTE trans_strength = 0;					// ͼƬ͸����ǿ��
	bool is_fade_in_progress = true;			// ����
	bool is_waiting_for_fade_out = false;		// �ȴ�����
	int fade_out_delay_counter = 100;			// �ȴ������ļ���������ϼ�ʱ��ʹ��)(��ǰscene��ѭ��ִ��ʱ������7ms���ң�7*�������������Ǵ�ŵ��ӳ�ʱ��)
	Timer timer_trans_control;
	
};



#endif //!_OPENING_SCENE_H_