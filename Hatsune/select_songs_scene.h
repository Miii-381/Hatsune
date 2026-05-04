#ifndef _SELECT_SONGS_SCENE_H_
#define _SELECT_SONGS_SCENE_H_

#include "ResourcesManager.h"
#include "scene_manager.h"
#include "utilities.h"
#include "button.h"
#include "Atlas.h"

#include <string>
#include <fstream>

extern SceneManager scene_manager;

// 选歌场景类：显示歌曲列表，允许玩家选择要游玩的歌曲和难度
class SelectSongsScene : public Scene
{
public:
	SelectSongsScene() = default;
	~SelectSongsScene() = default;

	// 进入选歌场景时加载歌曲信息并播放音乐
	void on_enter(bool is_debug)
	{
		if (!is_loaded)
		{
			is_loaded = true;

			// 读取歌曲信息文件
			std::ifstream file("./resources/beatmaps/songs_list.txt", std::ifstream::in);
			if (file.is_open() == false)
			{
				perror("fopen error!");
				MessageBox(GetHWnd(), _T("歌曲列表读取失败，请检查该文件是否存在！\n文件位于：.\\resources\\beatmaps\\songs_list.txt"), _T("ERROR!"), MB_OK | MB_ICONERROR);
				SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			}

			// 逐行读取歌曲信息
			for (int i = 0; i < ResourceMgr.SONGS_NUM; i++)
			{
				std::string line;
				getline(file, line);
				songs_list.push_back(line);
			}
			file.close();

			get_info();      // 解析当前歌曲信息
			add_intro();     // 添加显示前缀

			// 获取当前歌曲封面
			current_song_cover = ResourceMgr.songs_cover.get_image(current_song);

			// 计算各按钮位置
			pos_left_arrow_button.x = 20;
			pos_left_arrow_button.y = (getheight() - ResourceMgr.img_right_arrow_button_idle.getheight()) / 2;
			pos_right_arrow_button.x = getwidth() - ResourceMgr.img_right_arrow_button_idle.getwidth() - 20;
			pos_right_arrow_button.y = pos_left_arrow_button.y;
			pos_icon_setting_button.x = getwidth() - ResourceMgr.img_icon_setting_button_idle.getwidth();
			pos_icon_setting_button.y = 0;
			pos_icon_pause_button.x = pos_icon_setting_button.x - ResourceMgr.img_icon_pause_button_idle.getwidth();
			pos_icon_pause_button.y = 0;
			pos_text.x = 700;
			pos_text.y = pos_right_arrow_button.y - 30;
			
			// 根据封面比例调整按钮位置
			if (current_song_cover->getwidth() == current_song_cover->getheight())
				pos_current_song_button.x = 100 + 512 - 420;
			else if ((double)current_song_cover->getwidth() / current_song_cover->getheight() > 1.3332 && (double)current_song_cover->getwidth() / current_song_cover->getheight() < 1.3334)
				pos_current_song_button.x = 100 + 512 - 300;
			else
				pos_current_song_button.x = 150;

			pos_current_song_button.y = (getheight() - current_song_cover->getheight()) / 2;

			// 计算各按钮的交互区域
			region_current_song_button.top = pos_current_song_button.y;
			region_current_song_button.bottom = region_current_song_button.top + current_song_cover->getheight();
			region_current_song_button.left = pos_current_song_button.x;
			region_current_song_button.right = region_current_song_button.left + current_song_cover->getwidth();

			region_left_arrow_button.top = pos_left_arrow_button.y;
			region_left_arrow_button.bottom = region_left_arrow_button.top + ResourceMgr.img_left_arrow_button_idle.getheight();
			region_left_arrow_button.left = pos_left_arrow_button.x;
			region_left_arrow_button.right = region_left_arrow_button.left + ResourceMgr.img_left_arrow_button_idle.getwidth();

			region_right_arrow_button.top = pos_right_arrow_button.y;
			region_right_arrow_button.bottom = region_right_arrow_button.top + ResourceMgr.img_right_arrow_button_idle.getheight();
			region_right_arrow_button.left = pos_right_arrow_button.x;
			region_right_arrow_button.right = region_right_arrow_button.left + ResourceMgr.img_right_arrow_button_idle.getwidth();

			region_icon_setting_button.top = pos_icon_setting_button.y;
			region_icon_setting_button.bottom = region_icon_setting_button.top + ResourceMgr.img_icon_setting_button_idle.getheight();
			region_icon_setting_button.left = pos_icon_setting_button.x;
			region_icon_setting_button.right = region_icon_setting_button.left + ResourceMgr.img_icon_setting_button_idle.getwidth();

			region_icon_pause_button.top = pos_icon_pause_button.y;
			region_icon_pause_button.bottom = region_icon_pause_button.top + ResourceMgr.img_icon_pause_button_idle.getheight();
			region_icon_pause_button.left = pos_icon_pause_button.x;
			region_icon_pause_button.right = region_icon_pause_button.left + ResourceMgr.img_icon_pause_button_idle.getwidth();

			region_exit_button.top = pos_exit_button.y;
			region_exit_button.bottom = region_exit_button.top + ResourceMgr.img_exit_button_idle.getheight();
			region_exit_button.left = pos_exit_button.x;
			region_exit_button.right = region_exit_button.left + ResourceMgr.img_exit_button_idle.getwidth();
		}

		play_song(current_song);  // 播放当前歌曲
	}

	// 绘制选歌界面（背景、封面、按钮、歌曲信息）
	void on_draw(bool is_debug)
	{
		if (is_debug)
		{
			std::cout << info.ID << info.singer_name << info.song_name << info.song_length << info.Level[current_level] << std::endl;
		}

		putimage_alpha(0, 0, &ResourceMgr.img_background_select_songs_scene, nullptr, 200);

		// 绘制左右箭头按钮
		left_arrow_button.on_draw(pos_left_arrow_button.x, pos_left_arrow_button.y, 255,
			&ResourceMgr.img_left_arrow_button_idle, &ResourceMgr.img_left_arrow_button_hovered, &ResourceMgr.img_left_arrow_button_clicked);

		right_arrow_button.on_draw(pos_right_arrow_button.x, pos_right_arrow_button.y, 255,
			&ResourceMgr.img_right_arrow_button_idle, &ResourceMgr.img_right_arrow_button_hovered, &ResourceMgr.img_right_arrow_button_clicked);

		// 绘制设置和暂停图标按钮
		icon_setting_button.on_draw(pos_icon_setting_button.x, pos_icon_setting_button.y, 255,
			&ResourceMgr.img_icon_setting_button_idle, &ResourceMgr.img_icon_setting_button_hovered, &ResourceMgr.img_icon_setting_button_clicked);

		icon_pause_button.on_draw(pos_icon_pause_button.x, pos_icon_pause_button.y, 255,
			&ResourceMgr.img_icon_pause_button_idle, &ResourceMgr.img_icon_pause_button_hovered, &ResourceMgr.img_icon_pause_button_clicked);

		exit_button.on_draw(pos_exit_button.x, pos_exit_button.y, 255,
			&ResourceMgr.img_exit_button_idle, &ResourceMgr.img_exit_button_hovered, &ResourceMgr.img_exit_button_clicked);

		// 根据封面比例选择合适的阴影图片
		if (current_song_cover->getwidth() == current_song_cover->getheight())
			current_song_button.on_draw(pos_current_song_button.x, pos_current_song_button.y, 255, current_song_cover, &ResourceMgr.img_cover_shadow_1_1);
		else if ((double)current_song_cover->getwidth() / current_song_cover->getheight() > 1.32 && (double)current_song_cover->getwidth() / current_song_cover->getheight() < 1.34)
			current_song_button.on_draw(pos_current_song_button.x, pos_current_song_button.y, 255, current_song_cover, &ResourceMgr.img_cover_shadow_4_3);
		else if ((double)current_song_cover->getwidth() / current_song_cover->getheight() >= 1.76 && (double)current_song_cover->getwidth() / current_song_cover->getheight() <= 1.78)
			current_song_button.on_draw(pos_current_song_button.x, pos_current_song_button.y, 255, current_song_cover, &ResourceMgr.img_cover_shadow_16_9);

		// 绘制歌曲信息文本
		outtextxy_shaded(pos_text.x, pos_text.y, current_song_display);        // 歌曲编号
		outtextxy_shaded(pos_text.x, pos_text.y + 35, info.song_name);         // 歌曲名称
		outtextxy_shaded(pos_text.x, pos_text.y + 70, info.singer_name);       // 歌手名称
		outtextxy_shaded(pos_text.x, pos_text.y + 105, song_length);           // 歌曲时长
		outtextxy_shaded(pos_text.x, pos_text.y + 140, info.Level[current_level].c_str());  // 难度等级
	}

	// 处理用户输入（切换歌曲、选择难度、开始游戏等）
	void on_input(const ExMessage& msg, bool is_debug)
	{
		// 检测各按钮点击状态
		is_left_arrow_button_clicked = left_arrow_button.on_input(msg, region_left_arrow_button);
		is_right_arrow_button_clicked = right_arrow_button.on_input(msg, region_right_arrow_button);
		is_current_song_button_clicked = current_song_button.on_input(msg, region_current_song_button);
		is_icon_setting_button_clicked = icon_setting_button.on_input(msg, region_icon_setting_button);
		is_icon_pause_button_clicked = icon_pause_button.on_input(msg, region_icon_pause_button);
		is_exit_button_clicked = exit_button.on_input(msg, region_exit_button);

		if (msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONUP || msg.message == WM_MOUSEWHEEL)
		{
			// 左箭头或左方向键：切换到上一首歌曲
			if (is_left_arrow_button_clicked || msg.vkcode == VK_LEFT)
			{
				if (current_song <= 1)
				{
					stop_song(current_song);
					current_song = ResourceMgr.SONGS_NUM;  // 循环到最后一首
					play_song(current_song);
				}
				else
				{
					stop_song(current_song);
					current_song -= 1;
					play_song(current_song);
				}
				is_need_switch_song = true;
				current_level = 0;  // 重置难度选择
			}

			// 右箭头或右方向键：切换到下一首歌曲
			if (is_right_arrow_button_clicked || msg.vkcode == VK_RIGHT)
			{
				if (current_song >= ResourceMgr.SONGS_NUM)
				{
					stop_song(current_song);
					current_song = 1;  // 循环到第一首
					play_song(current_song);
				}
				else
				{
					stop_song(current_song);
					current_song += 1;
					play_song(current_song);
				}
				is_need_switch_song = true;
				current_level = 0;  // 重置难度选择
			}

			// 上方向键或滚轮向上：提高难度等级
			if (msg.vkcode == VK_UP || msg.wheel > 0)
			{
				if (current_level == info.Level.size() - 1)
					current_level = 0;  // 循环到第一个难度
				else
					current_level += 1;
			}

			// 下方向键或滚轮向下：降低难度等级
			if (msg.vkcode == VK_DOWN || msg.wheel < 0)
			{
				if (current_level == 0)
					current_level = info.Level.size() - 1;  // 循环到最后一个难度
				else
					current_level -= 1;
			}

			// ESC键或退出按钮：返回上一场景
			if (msg.vkcode == VK_ESCAPE || is_exit_button_clicked)
			{
				stop_song(current_song);
				scene_manager.switch_to_last_scene(is_debug);
			}

			// 回车键或点击封面：开始游戏
			if (is_current_song_button_clicked || msg.vkcode == VK_RETURN)
			{
				stop_song(current_song);
				mciSendString("play song_selected from 0", NULL, 0, NULL);  // 播放选择音效
				scene_manager.switch_to(SceneManager::SceneType::gaming_scene);
			}

			// 设置图标按钮：进入设置场景
			if (is_icon_setting_button_clicked)
			{
				stop_song(current_song);
				scene_manager.switch_to(SceneManager::SceneType::setting_scene);
			}

			// 如果需要切换歌曲，更新封面和信息
			if (is_need_switch_song)
			{
				current_song_cover = ResourceMgr.songs_cover.get_image(current_song);

				// 根据新封面比例调整位置
				if (current_song_cover->getwidth() == current_song_cover->getheight())
					pos_current_song_button.x = 100 + 512 - 384;
				else if ((double)current_song_cover->getwidth() / current_song_cover->getheight() > 1.3332 && (double)current_song_cover->getwidth() / current_song_cover->getheight() < 1.3334)
					pos_current_song_button.x = 100 + 512 - 420;
				else
					pos_current_song_button.x = 150;

				pos_current_song_button.y = (getheight() - current_song_cover->getheight()) / 2;

				// 重新计算交互区域
				region_current_song_button.top = pos_current_song_button.y;
				region_current_song_button.bottom = region_current_song_button.top + current_song_cover->getheight();
				region_current_song_button.left = pos_current_song_button.x;
				region_current_song_button.right = region_current_song_button.left + current_song_cover->getwidth();

				get_info();    // 解析新歌曲信息
				add_intro();   // 添加显示前缀

				is_need_switch_song = false;
			}

			// 暂停图标按钮：暂停/恢复播放
			if (is_icon_pause_button_clicked)
			{
				if (is_paused)
				{
					is_paused = false;
					resume_song(current_song);  // 恢复播放
				}
				else
				{
					is_paused = true;
					pause_song(current_song);   // 暂停播放
				}
			}
		};
	}

	// 解析歌曲信息（从字符串中提取ID、名称、歌手、时长、难度等）
	void get_info()
	{
		info = { 0 };
		std::string str = songs_list[current_song - 1];
		int count = 0;
		size_t now = 0, next = 0, length = 0;
		
		// 按" - "分隔符解析字符串
		while (next != std::string::npos)
		{
			std::string temp;
			next = str.find(_T(" - "), now);
			length = next - now;
			temp = str.substr(now, length);
			
			switch (count)
			{
			case 0:
				info.ID = std::stoi(temp);  // 提取歌曲ID
				Song_length_get();          // 获取歌曲时长
				break;
			case 1:
				std::copy(temp.begin(), temp.end(), info.song_name);  // 提取歌曲名称
				break;
			case 2:
				std::copy(temp.begin(), temp.end(), info.singer_name);  // 提取歌手名称
				break;
			case 3:
				info.song_chorus_time = std::stoi(temp);  // 提取副歌时间
				break;
			};
			
			// 第4个及以后的字段为难度等级
			if (count > 3)
			{
				info.Level.push_back(temp);
			}
			now = next + 3;
			count++;
		}
		origin_info = info;  // 保存原始信息
	}

	// 为显示文本添加前缀（如"歌曲名称："等）
	void add_intro()
	{
		TCHAR temp[512] = {};
		
		// 为歌曲名称添加前缀
		strncpy_s(temp, info.song_name, _countof(info.song_name));
		sprintf_s(info.song_name, "歌曲名称：%s", temp);
		memset(temp, '\0', sizeof(temp));

		// 为歌手名称添加前缀
		strncpy_s(temp, info.singer_name, _countof(info.singer_name));
		sprintf_s(info.singer_name, "歌手名称：%s", temp);
		memset(temp, '\0', sizeof(temp));

		// 格式化歌曲时长
		int s = 0, m = 0, h = 0;
		msToTime(info.song_length, s, m, h);
		if (h == 0)
		{
			if (m < 10)
				_stprintf_s(song_length, "歌曲时长：%d:%02d", m, s);
			else
				_stprintf_s(song_length, "歌曲时长：%02d:%02d", m, s);
		}
		else
		{
			if (m < 10)
				_stprintf_s(song_length, "歌曲时长：%d:%d:%02d", h, m, s);
			else
				_stprintf_s(song_length, "歌曲时长：%d:%02d:%02d", h, m, s);
		}

		// 为难度等级添加前缀
		for (int i = 0; i < info.Level.size(); i++)
		{
			TCHAR temp_1[128] = {};
			strcpy_s(temp_1, info.Level[i].c_str());
			sprintf_s(temp, "难度等级：%s", temp_1);
			info.Level[i] = temp;
		}

		// 格式化当前歌曲显示
		sprintf_s(current_song_display, "当前歌曲：%d / %d", info.ID, ResourceMgr.SONGS_NUM);
	}

	// 通过MCI命令获取歌曲时长
	void Song_length_get()
	{
		TCHAR temp[64];
		TCHAR temp_length[256];
		_stprintf_s(temp, _T("status song_%d length"), info.ID);
		mciSendString(temp, temp_length, sizeof(temp_length), NULL);
		info.song_length = strtol(temp_length, NULL, 10);
	}

private:
	bool is_loaded = false;  // 是否已加载歌曲列表

	IMAGE* current_song_cover;  // 当前歌曲封面图片指针
	
	// 按钮对象
	Button left_arrow_button;       // 左箭头按钮
	Button right_arrow_button;      // 右箭头按钮
	Button current_song_button;     // 当前歌曲封面按钮
	Button icon_setting_button;     // 设置图标按钮
	Button icon_pause_button;       // 暂停图标按钮
	Button exit_button;             // 退出按钮

	// 按钮交互区域
	RECT region_left_arrow_button = {};
	RECT region_right_arrow_button = {};
	RECT region_current_song_button = {};
	RECT region_icon_setting_button = {};
	RECT region_icon_pause_button = {};
	RECT region_exit_button = {};

	// 按钮位置坐标
	POINT pos_left_arrow_button = {};
	POINT pos_right_arrow_button = {};
	POINT pos_current_song_button = {};
	POINT pos_icon_setting_button = {};
	POINT pos_icon_pause_button = {};
	POINT pos_exit_button = {};
	POINT pos_text = {};

	// 按钮点击状态标志
	bool is_left_arrow_button_clicked = false;
	bool is_right_arrow_button_clicked = false;
	bool is_current_song_button_clicked = false;
	bool is_icon_setting_button_clicked = false;
	bool is_icon_pause_button_clicked = false;
	bool is_exit_button_clicked = false;
	
	bool is_paused = false;              // 是否已暂停
	bool is_need_switch_song = false;    // 是否需要切换歌曲

	std::vector<std::string> songs_list; // 歌曲信息列表
	TCHAR current_song_display[20] = {}; // 当前歌曲显示文本
	int current_song = 1;                // 当前歌曲编号
	TCHAR song_length[128];              // 歌曲时长文本
};

#endif // !_SELECT_SONGS_SCENE_H_
