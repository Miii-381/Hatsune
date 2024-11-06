#ifndef _SCENE_H_
#define _SCENE_H_

#include "timer.h"
#include <graphics.h>
#include <vector>
#include <string>

class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	struct song_info									// ������Ϣ
	{
		int ID = 0;										// ���
		TCHAR song_name[300] = { };						// ������
		TCHAR singer_name[300] = { };					// ������
		int song_length;								// ��������
		int song_chorus_time;							// ����ʱ���
		std::vector<std::string> Level;					// �Ѷȵȼ�
	};

	song_info origin_info;								// ������Ϣ�洢
	song_info info;										// �����ܵĸ�����Ϣ�洢
	size_t current_level = 0;							// ��ǰѡ���Ѷ�

public:
	virtual void on_enter(bool is_debug) { }

	virtual void on_update(int delta, bool is_debug) { }

	virtual void on_draw(bool is_debug) { }

	virtual void on_input(const ExMessage& msg, bool is_debug) { }

	virtual void on_exit(bool is_debug) { }

	void set_new_origininfo(const song_info& oldInfo)			// ʵ�����������ʱ������֮��ı�������ͨ������(������ͬ��ַ��ʵ���ĳ�Ա�����϶���һ��...)
	{
		origin_info = oldInfo;
	}

	void get_current_level(const unsigned short& level)			
	{
		current_level = level;
	}
};


#endif // !_SCENE_H_

