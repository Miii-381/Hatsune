#ifndef _SCENE_H_
#define _SCENE_H_

#include "timer.h"
#include <graphics.h>
#include <vector>
#include <string>

// 场景基类：所有游戏场景的父类，定义了场景的基本接口
class Scene
{
public:
	Scene() = default;
	~Scene() = default;
	
	// 歌曲信息结构体
	struct song_info
	{
		int ID = 0;                                    // 歌曲编号
		TCHAR song_name[300] = { };                    // 歌曲名称
		TCHAR singer_name[300] = { };                  // 歌手名称
		int song_length = 0;                           // 歌曲时长（毫秒）
		int song_chorus_time = 0;                      // 副歌时间点（毫秒）
		std::vector<std::string> Level = { };          // 难度等级列表
	};

	song_info origin_info = { };      // 原始歌曲信息存储（用于传递）
	song_info info = { };             // 当前处理的歌曲信息存储
	size_t current_level = 0;         // 当前选择的难度索引

public:
	// 场景进入时调用（初始化）
	virtual void on_enter(bool is_debug) { }

	// 每帧更新场景逻辑
	// delta: 帧时间间隔（毫秒）
	virtual void on_update(int delta, bool is_debug) { }

	// 绘制场景内容
	virtual void on_draw(bool is_debug) { }

	// 处理用户输入
	// msg: 消息结构体
	virtual void on_input(const ExMessage& msg, bool is_debug) { }

	// 场景退出时调用（清理资源）
	virtual void on_exit(bool is_debug) { }

	// 设置新的原始信息（用于场景间传递数据）
	// oldInfo: 要设置的歌曲信息
	void set_new_origininfo(const song_info& oldInfo)
	{
		origin_info = oldInfo;
	}

	// 获取当前选择的难度等级
	// level: 难度等级索引
	void get_current_level(const unsigned short& level)
	{
		current_level = level;
	}
};


#endif // !_SCENE_H_
