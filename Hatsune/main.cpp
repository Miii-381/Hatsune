#include "utilities.h"
#include "atlas.h"
#include "button.h"
#include "click.h"
#include "ResourcesManager.h"
#include "scene_manager.h"
#include "opening_scene.h"
#include "menu_scene.h"
#include "setting_scene.h"
#include "select_songs_scene.h"
#include "gaming_scene.h"

#include <iostream>
#include <thread>

// 游戏窗口基本配置
const int WINDOW_WIDTH = 1280;   // 窗口宽度
const int WINDOW_HEIGHT = 720;   // 窗口高度
const int FPS = 144;             // 目标帧率

bool is_debug = false;           // 是否开启调试模式
bool load_all_songs = true;      // 是否加载所有歌曲

// 资源管理器全局实例
ResourcesManager ResourceMgr;

// 场景管理器及各个场景实例
SceneManager scene_manager;
Scene* opening_scene = nullptr;    // 开场场景
Scene* menu_scene = nullptr;       // 主菜单场景
Scene* select_songs_scene = nullptr; // 选歌场景
Scene* gaming_scene = nullptr;     // 游戏场景
Scene* setting_scene = nullptr;    // 设置场景

int main()
{
	ExMessage msg;

	// 选择启动模式（是否开启调试）
	switch (MessageBox(GetHWnd(), _T("是否进入debug模式？"), _T("初始之音"), MB_YESNO))
	{
	case IDYES:
		initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EX_SHOWCONSOLE);  // 显示控制台窗口
		SetWindowText(GetHWnd(), _T("初始之音"));
		is_debug = true;
		
		// 选择是否加载所有歌曲
		switch (MessageBox(GetHWnd(), _T("是否加载全部歌曲？"), _T("初始之音"), MB_YESNO))
		{
		case IDYES:
			load_all_songs = true;
			break;
		case IDNO:
			load_all_songs = false;
			break;
		};
		break;
	case IDNO:
		initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);  // 不显示控制台
		SetWindowText(GetHWnd(), _T("初始之音"));
		break;
	};

	// 显示加载界面
	loadimage(&ResourceMgr.img_loading, _T("./resources/others/img_loading.png"));
	putimage_alpha(0, 0, &ResourceMgr.img_loading);
	FlushBatchDraw();

	// 加载游戏资源
	ResourceMgr.load_songs(load_all_songs);     // 加载歌曲音频
	ResourceMgr.load_game_resources();          // 加载图片资源
	ResourceMgr.set_font();                     // 设置字体

	// 创建所有场景实例
	opening_scene = new OpeningScene();
	menu_scene = new MenuScene();
	setting_scene = new SettingScene();
	select_songs_scene = new SelectSongsScene();
	gaming_scene = new GamingScene();
	
	// 设置当前场景为开场场景
	scene_manager.set_current_scene(opening_scene);

	// 设置绘图模式
	setbkmode(TRANSPARENT);   // 透明背景模式
	settextcolor(WHITE);      // 白色文字
	settextstyle(&ResourceMgr.font_yahei);  // 使用微软雅黑字体

	BeginBatchDraw();  // 开始批量绘制

	// 游戏主循环
	while (true)
	{
		auto frame_start_time = std::chrono::system_clock::now();

		// 处理用户输入消息
		while (peekmessage(&msg))
			scene_manager.on_input(msg, is_debug);

		// 计算帧时间间隔
		static auto last_tick_time = std::chrono::system_clock::now();
		auto current_tick_time = std::chrono::system_clock::now();
		auto delta_tick = std::chrono::duration_cast<std::chrono::milliseconds>(current_tick_time - last_tick_time).count();
		
		// 更新场景逻辑
		scene_manager.on_update(delta_tick, is_debug);
		last_tick_time = current_tick_time;

		// 清空屏幕并绘制当前场景
		cleardevice();
		scene_manager.on_draw(is_debug);
		FlushBatchDraw();

		// 帧率控制：确保不超过目标FPS
		auto frame_end_time = std::chrono::system_clock::now();
		auto frame_delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end_time - frame_start_time).count();
		if (frame_delta_time < 1000 / FPS)
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS - frame_delta_time));
	}

	EndBatchDraw();  // 结束批量绘制

	return 0;
}
