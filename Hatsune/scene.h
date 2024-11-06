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
	struct song_info									// 歌曲信息
	{
		int ID = 0;										// 序号
		TCHAR song_name[300] = { };						// 歌曲名
		TCHAR singer_name[300] = { };					// 歌手名
		int song_length;								// 歌曲长度
		int song_chorus_time;							// 副歌时间点
		std::vector<std::string> Level;					// 难度等级
	};

	song_info origin_info;								// 歌曲信息存储
	song_info info;										// 带介绍的歌曲信息存储
	size_t current_level = 0;							// 当前选择难度

public:
	virtual void on_enter(bool is_debug) { }

	virtual void on_update(int delta, bool is_debug) { }

	virtual void on_draw(bool is_debug) { }

	virtual void on_input(const ExMessage& msg, bool is_debug) { }

	virtual void on_exit(bool is_debug) { }

	void set_new_origininfo(const song_info& oldInfo)			// 实例化多个对象时，对象之间的变量不互通！！！(两个不同地址的实例的成员变量肯定不一样...)
	{
		origin_info = oldInfo;
	}

	void get_current_level(const unsigned short& level)			
	{
		current_level = level;
	}
};


#endif // !_SCENE_H_

