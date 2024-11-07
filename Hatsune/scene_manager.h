#ifndef _SCENE_SELECTOR_H_
#define _SCENE_SELECTOR_H_

#include "scene.h"

extern Scene* opening_scene;
extern Scene* menu_scene;
extern Scene* gaming_scene;
extern Scene* setting_scene;
extern Scene* select_songs_scene;
extern std::vector<Scene*> last_scene;

extern bool is_debug;

class SceneManager
{
public:
	enum class SceneType
	{
		opening_scene,
		menu_scene,
		setting_scene,
		select_songs_scene,
		gaming_scene
	};

public:
	SceneManager() = default;
	~SceneManager() = default;

	// ���õ�ǰ֡�ĳ���
	void set_current_scene(Scene* scene)
	{
		current_scene = scene;
		current_scene->on_enter(is_debug);
	}

	void switch_to(SceneType type)
	{
		if(current_scene != opening_scene)
			last_scene.push_back(current_scene); // �洢����ʵ���ĵ�ַ����û�и���һ���³���ʵ����
		current_scene->on_exit(is_debug);
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
			current_scene->set_new_origininfo(select_songs_scene->origin_info);
			current_scene->get_current_level(select_songs_scene->current_level);
			break;
		default:
			MessageBox(GetHWnd(), _T("�����л�����"), _T("��ʼ֮��"), MB_OK | MB_ICONERROR);
			SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			break;
		}
		current_scene->on_enter(is_debug);
	}

	void switch_to_last_scene(bool is_debug)
	{
		if (current_scene == gaming_scene)
			current_scene->on_exit(is_debug);
		current_scene = last_scene.back();
		last_scene.pop_back();
		current_scene->on_enter(is_debug);
	}

	void on_update(int delta, bool is_debug)
	{
		current_scene->on_update(delta, is_debug);
	}

	void on_draw(bool is_debug)
	{
		current_scene->on_draw(is_debug);
	}

	void on_input(const ExMessage& msg,bool is_debug)
	{
		current_scene->on_input(msg, is_debug);
	}

private:
	Scene* current_scene = nullptr;
	std::vector<Scene*> last_scene;		// ��¼�ϸ�������ʲô��������ݣ���ǰѡ��ͽ�ָ��ѹ��last_scene��ջ�������˾���ָ���ջ�����current_scene��������˿������~
};

#endif // !_SCENE_SELECTOR_H_
