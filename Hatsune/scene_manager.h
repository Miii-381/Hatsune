#ifndef _SCENE_SELECTOR_H_
#define _SCENE_SELECTOR_H_

#include "scene.h"
#include "ResourcesManager.h"

// 外部声明的场景实例
extern Scene* opening_scene;
extern Scene* menu_scene;
extern Scene* gaming_scene;
extern Scene* setting_scene;
extern Scene* select_songs_scene;

extern bool is_debug;

// 场景管理器类：负责管理游戏场景的切换和生命周期
class SceneManager
{
public:
	// 场景类型枚举
	enum class SceneType
	{
		opening_scene,      // 开场场景
		menu_scene,         // 主菜单场景
		setting_scene,      // 设置场景
		select_songs_scene, // 选歌场景
		gaming_scene        // 游戏场景
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	// 设置当前活动场景
	// scene: 要设置的场景指针
	void set_current_scene(Scene* scene)
	{
		current_scene = scene;
		current_scene->on_enter(is_debug);  // 调用场景的进入方法
	}

	// 切换到指定类型的场景
	// type: 目标场景类型
	void switch_to(SceneType type)
	{
		// 将当前场景加入历史栈（开场场景除外）
		if (current_scene != opening_scene)
			last_scene.push_back(current_scene);
		
		// 退出当前场景
		current_scene->on_exit(is_debug);
		
		// 根据类型切换到新场景
		switch (type)
		{
		case SceneType::opening_scene:
			current_scene = opening_scene;
			break;
		case SceneType::menu_scene:
			current_scene = menu_scene;
			break;
		case SceneType::setting_scene:
			current_scene = setting_scene;
			break;
		case SceneType::select_songs_scene:
			current_scene = select_songs_scene;
			break;
		case SceneType::gaming_scene:
			current_scene = gaming_scene;
			// 传递选歌场景的信息给游戏场景
			current_scene->set_new_origininfo(select_songs_scene->origin_info);
			current_scene->get_current_level(select_songs_scene->current_level);
			break;
		default:
			MessageBox(GetHWnd(), _T("场景切换失败"), _T("初始之音"), MB_OK | MB_ICONERROR);
			SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			break;
		}
		
		// 进入新场景
		current_scene->on_enter(is_debug);
	}

	// 返回上一个场景
	// is_debug: 是否处于调试模式
	void switch_to_last_scene(bool is_debug)
	{
		// 如果当前是游戏场景，先退出
		if (current_scene == gaming_scene)
			current_scene->on_exit(is_debug);
		
		// 从历史栈中取出上一个场景
		current_scene = last_scene.back();
		last_scene.pop_back();
		
		// 进入上一个场景
		current_scene->on_enter(is_debug);
	}

	// 更新当前场景逻辑
	// delta: 帧时间间隔（毫秒）
	// is_debug: 是否处于调试模式
	void on_update(int delta, bool is_debug)
	{
		current_scene->on_update(delta, is_debug);
	}

	// 绘制当前场景
	// is_debug: 是否处于调试模式
	void on_draw(bool is_debug)
	{
		current_scene->on_draw(is_debug);
	}

	// 处理当前场景的输入
	// msg: 消息结构体
	// is_debug: 是否处于调试模式
	void on_input(const ExMessage& msg, bool is_debug)
	{
		current_scene->on_input(msg, is_debug);
	}

private:
	Scene* current_scene = nullptr;       // 当前活动场景
	std::vector<Scene*> last_scene;       // 场景历史栈（用于返回）
};

#endif // !_SCENE_SELECTOR_H_
