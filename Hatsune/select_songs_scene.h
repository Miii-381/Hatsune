#ifndef _SELECT_SONGS_SCENE_H_
#define _SELECT_SONGS_SCENE_H_

#include "select_songs_scene.h"
#include "button.h"
#include "utilities.h"
#include "Atlas.h"

#include <string>
#include <fstream>

extern SceneManager scene_manager;

extern int SONGS_NUM;
extern Atlas songs_cover;								// 歌曲封面图集
extern IMAGE img_cover_shadow_16_9;						// 歌曲封面阴影
extern IMAGE img_cover_shadow_4_3;
extern IMAGE img_cover_shadow_1_1;

extern IMAGE img_background_select_songs_scene;			// 背景

extern IMAGE img_current_song_button;					// 当前歌曲封面（当作按钮处理）
extern IMAGE img_icon_setting_button_idle;				// 设置按键
extern IMAGE img_icon_setting_button_hovered;
extern IMAGE img_icon_setting_button_clicked;
extern IMAGE img_left_arrow_button_idle;				// 向左按键
extern IMAGE img_left_arrow_button_hovered;
extern IMAGE img_left_arrow_button_clicked;
extern IMAGE img_right_arrow_button_idle;				// 向右按键
extern IMAGE img_right_arrow_button_hovered;
extern IMAGE img_right_arrow_button_clicked;
extern IMAGE img_icon_pause_button_idle;				// 暂停按键
extern IMAGE img_icon_pause_button_hovered;
extern IMAGE img_icon_pause_button_clicked;
extern IMAGE img_exit_button_idle;						// 返回按键
extern IMAGE img_exit_button_hovered;
extern IMAGE img_exit_button_clicked;

class SelectSongsScene : public Scene
{
public:
	SelectSongsScene() = default;
	~SelectSongsScene() = default;

	void on_enter(bool is_debug)
	{
		if (!is_loaded)
		{
			is_loaded = true;

			// 歌曲信息文件获取
			std::ifstream file("./resources/beatmaps/songs_list.txt", std::ifstream::in);
			if (file.is_open() == false)
			{
				perror("fopen error!");
				MessageBox(GetHWnd(), _T("歌曲文件获取错误，请检查后重新启动程序\n歌曲数文件位于：.\\resources\\beatmaps\\songs_list.txt"), _T("ERROR!"), MB_OK);
				SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			}

			for (int i = 0; i < SONGS_NUM; i++)
			{
				std::string line;
				getline(file, line);
				std::cout << line << std::endl;
				songs_list.push_back(line);
			}
			file.close();

			get_info();

			add_intro();

			// 位置加载
			current_song_cover = songs_cover.get_image(current_song);

			pos_left_arrow_button.x = 20;
			pos_left_arrow_button.y = (getheight() - img_right_arrow_button_idle.getheight()) / 2;
			pos_right_arrow_button.x = getwidth() - img_right_arrow_button_idle.getwidth() - 20;
			pos_right_arrow_button.y = pos_left_arrow_button.y;
			pos_icon_setting_button.x = getwidth() - img_icon_setting_button_idle.getwidth();
			pos_icon_setting_button.y = 0;
			pos_icon_pause_button.x = pos_icon_setting_button.x - img_icon_pause_button_idle.getwidth();
			pos_icon_pause_button.y = 0;
			pos_text.x = 700;
			pos_text.y = pos_right_arrow_button.y - 30;
			if (current_song_cover->getwidth() == current_song_cover->getheight())
				pos_current_song_button.x = 100 + 512 - 420;		// 封面位置设置（1：1）
			else if((double)current_song_cover->getwidth() / current_song_cover->getheight() > 1.3332 && (double)current_song_cover->getwidth() / current_song_cover->getheight() < 1.3334)
				pos_current_song_button.x = 100 + 512 - 300;	    // 封面位置设置（4：3）
			else
				pos_current_song_button.x = 150;					// 封面位置设置（16：9）

			pos_current_song_button.y = (getheight() - current_song_cover->getheight()) / 2;

			region_current_song_button.top = pos_current_song_button.y;
			region_current_song_button.bottom = region_current_song_button.top + current_song_cover->getheight();
			region_current_song_button.left = pos_current_song_button.x;
			region_current_song_button.right = region_current_song_button.left + current_song_cover->getwidth();

			region_left_arrow_button.top = pos_left_arrow_button.y;
			region_left_arrow_button.bottom = region_left_arrow_button.top + img_left_arrow_button_idle.getheight();
			region_left_arrow_button.left = pos_left_arrow_button.x;
			region_left_arrow_button.right = region_left_arrow_button.left + img_left_arrow_button_idle.getwidth();

			region_right_arrow_button.top = pos_right_arrow_button.y;
			region_right_arrow_button.bottom = region_right_arrow_button.top + img_right_arrow_button_idle.getheight();
			region_right_arrow_button.left = pos_right_arrow_button.x;
			region_right_arrow_button.right = region_right_arrow_button.left + img_right_arrow_button_idle.getwidth();

			region_icon_setting_button.top = pos_icon_setting_button.y;
			region_icon_setting_button.bottom = region_icon_setting_button.top + img_icon_setting_button_idle.getheight();
			region_icon_setting_button.left = pos_icon_setting_button.x;
			region_icon_setting_button.right = region_icon_setting_button.left + img_icon_setting_button_idle.getwidth();

			region_icon_pause_button.top = pos_icon_pause_button.y;
			region_icon_pause_button.bottom = region_icon_pause_button.top + img_icon_pause_button_idle.getheight();
			region_icon_pause_button.left = pos_icon_pause_button.x;
			region_icon_pause_button.right = region_icon_pause_button.left + img_icon_pause_button_idle.getwidth();

			region_exit_button.top = pos_exit_button.y;
			region_exit_button.bottom = region_exit_button.top + img_exit_button_idle.getheight();
			region_exit_button.left = pos_exit_button.x;
			region_exit_button.right = region_exit_button.left + img_exit_button_idle.getwidth();
		}

		play_song(current_song, info.song_chorus_time);
	}

	void on_draw(bool is_debug)
	{
		if (is_debug)
		{
			std::cout << info.ID << info.singer_name << info.song_name << info.song_length << info.Level[current_level] << std::endl;
			printf("select_songs_scene\n");
		}

		putimage_alpha(0, 0, &img_background_select_songs_scene, 200);

		left_arrow_button.on_draw(pos_left_arrow_button.x, pos_left_arrow_button.y, 255, &img_left_arrow_button_idle, &img_left_arrow_button_hovered, &img_left_arrow_button_clicked);

		right_arrow_button.on_draw(pos_right_arrow_button.x, pos_right_arrow_button.y, 255, &img_right_arrow_button_idle, &img_right_arrow_button_hovered, &img_right_arrow_button_clicked);

		icon_setting_button.on_draw(pos_icon_setting_button.x, pos_icon_setting_button.y, 255, &img_icon_setting_button_idle, &img_icon_setting_button_hovered, &img_icon_setting_button_clicked);

		icon_pause_button.on_draw(pos_icon_pause_button.x, pos_icon_pause_button.y, 255, &img_icon_pause_button_idle, &img_icon_pause_button_hovered, &img_icon_pause_button_clicked);

		exit_button.on_draw(pos_exit_button.x, pos_exit_button.y, 255, &img_exit_button_idle, &img_exit_button_hovered, &img_exit_button_clicked);

		if (current_song_cover->getwidth() == current_song_cover->getheight())
			current_song_button.on_draw(pos_current_song_button.x, pos_current_song_button.y, 255, current_song_cover, &img_cover_shadow_1_1);		// 绘制带阴影的封面（1：1）
		else if ((double)current_song_cover->getwidth() / current_song_cover->getheight() > 1.32 && (double)current_song_cover->getwidth() / current_song_cover->getheight() < 1.34)
			current_song_button.on_draw(pos_current_song_button.x, pos_current_song_button.y, 255, current_song_cover, &img_cover_shadow_4_3);		// 绘制带阴影的封面（4：3）																				
		else if ((double)current_song_cover->getwidth() / current_song_cover->getheight() >= 1.76 && (double)current_song_cover->getwidth() / current_song_cover->getheight() <= 1.78)																																		
			current_song_button.on_draw(pos_current_song_button.x, pos_current_song_button.y, 255, current_song_cover, &img_cover_shadow_16_9);		// 绘制带阴影的封面（16：9）

		outtextxy_shaded(pos_text.x, pos_text.y, current_song_display);
		outtextxy_shaded(pos_text.x, pos_text.y + 35, info.song_name);
		outtextxy_shaded(pos_text.x, pos_text.y + 70, info.singer_name);
		outtextxy_shaded(pos_text.x, pos_text.y + 105, song_length);
		outtextxy_shaded(pos_text.x, pos_text.y + 140, info.Level[current_level].c_str());
	}

	void on_input(const ExMessage& msg, bool is_debug)
	{
		is_left_arrow_button_clicked = left_arrow_button.on_input(msg,region_left_arrow_button);
		is_right_arrow_button_clicked = right_arrow_button.on_input(msg, region_right_arrow_button);
		is_current_song_button_clicked = current_song_button.on_input(msg,region_current_song_button);
		is_icon_setting_button_clicked = icon_setting_button.on_input(msg, region_icon_setting_button);
		is_icon_pause_button_clicked = icon_pause_button.on_input(msg, region_icon_pause_button);
		is_exit_button_clicked = exit_button.on_input(msg, region_exit_button);

		if (msg.message == WM_KEYDOWN || msg.message == WM_LBUTTONUP || msg.message == WM_MOUSEWHEEL)
		{
			if (is_left_arrow_button_clicked || msg.vkcode == VK_LEFT)
			{
				if (current_song <= 1)
				{
					stop_song(current_song);
					current_song = SONGS_NUM;
					play_song(current_song, info.song_chorus_time);
				}
				else
				{
					stop_song(current_song);
					current_song -= 1;
					play_song(current_song, info.song_chorus_time);
				}
				is_need_switch_song = true;
				current_level = 0;
			}

			if (is_right_arrow_button_clicked || msg.vkcode == VK_RIGHT)
			{
				if (current_song >= SONGS_NUM)
				{
					stop_song(current_song);
					current_song = 1;
					play_song(current_song, info.song_chorus_time);
				}
				else
				{
					stop_song(current_song);
					current_song += 1;
					play_song(current_song, info.song_chorus_time);
				}
				is_need_switch_song = true;
				current_level = 0;
			}

			if (msg.vkcode == VK_UP || msg.wheel > 0)
			{
				if (current_level == info.Level.size() - 1)
					current_level = 0;
				else
					current_level += 1;
			}

			if (msg.vkcode == VK_DOWN || msg.wheel < 0)
			{
				if (current_level == 0)
					current_level = info.Level.size() - 1;
				else
					current_level -= 1;
			}

			if (msg.vkcode == VK_ESCAPE || is_exit_button_clicked)
			{
				stop_song(current_song);
				scene_manager.switch_to_last_scene(is_debug);
			}

			if (is_current_song_button_clicked || msg.vkcode == VK_RETURN)
			{
				stop_song(current_song);
				mciSendString("play song_selected from 0", NULL, 0, NULL);
				scene_manager.switch_to(SceneManager::SceneType::gaming_scene);
			}

			if (is_icon_setting_button_clicked)
			{
				stop_song(current_song);
				scene_manager.switch_to(SceneManager::SceneType::setting_scene);
			}

			if (is_need_switch_song)
			{
				current_song_cover = songs_cover.get_image(current_song);

				if (current_song_cover->getwidth() == current_song_cover->getheight())
					pos_current_song_button.x = 100 + 512 - 384;		// 封面位置设置（1：1）
				else if ((double)current_song_cover->getwidth() / current_song_cover->getheight() > 1.3332 && (double)current_song_cover->getwidth() / current_song_cover->getheight() < 1.3334)
					pos_current_song_button.x = 100 + 512 - 420;	    // 封面位置设置（4：3）
				else
					pos_current_song_button.x = 150;					// 封面位置设置（16：9）

				pos_current_song_button.y = (getheight() - current_song_cover->getheight()) / 2;

				region_current_song_button.top = pos_current_song_button.y;
				region_current_song_button.bottom = region_current_song_button.top + current_song_cover->getheight();
				region_current_song_button.left = pos_current_song_button.x;
				region_current_song_button.right = region_current_song_button.left + current_song_cover->getwidth();

				get_info();
				add_intro();

				is_need_switch_song = false;
			}

			if ((is_icon_pause_button_clicked))
			{
				if (is_paused)
				{
					is_paused = false;
					resume_song(current_song);
				}
				else
				{
					is_paused = true;
					pause_song(current_song);
				}
			}
		};
	}

	// 获取歌曲信息
	void get_info()
	{
		info = { 0 };
		std::string str = songs_list[current_song - 1];
		int count = 0;
		size_t now = 0, next = 0, length = 0;
		while (next != std::string::npos)
		{	
			std::string temp;
			next = str.find(_T(" - "), now);
			length = next - now;
			temp = str.substr(now, length);
			switch (count)
			{
			case 0:											// 歌曲ID
				info.ID = std::stoi(temp);
				Song_length_get();
				if (is_debug) std::cout << "ID complete" << std::endl;
				break;
			case 1:											// 歌曲名
				std::copy(temp.begin(), temp.end(), info.song_name);
				if (is_debug) std::cout << "song_name complete" << std::endl;
				break;
			case 2:											// 歌手名
				std::copy(temp.begin(), temp.end(), info.singer_name);
				if (is_debug) std::cout << "singer_name complete" << std::endl;
				break;
			case 3:											// 歌曲副歌时间点
				info.song_chorus_time = std::stoi(temp);
				if (is_debug) std::cout << "song_length complete" << std::endl;
				break;
			};
			if (count > 3)
			{
				info.Level.push_back(temp);					// 难度
				if (is_debug) std::cout << "song_level complete" << std::endl;
			}
			now = next + 3;
			count++;
		}
		origin_info = info;
	}

	// 加介绍
	void add_intro()
	{
		TCHAR temp[512];
		TCHAR temp_str[512];
		strncpy_s(temp, info.song_name, _countof(info.song_name));
		sprintf_s(info.song_name, "曲名：%s", temp);
		memset(temp, '\0', sizeof(temp));

		strncpy_s(temp, info.singer_name, _countof(info.singer_name));
		sprintf_s(info.singer_name, "歌手名：%s", temp);
		memset(temp, '\0', sizeof(temp));

		int s = 0, m = 0, h = 0;
		msToTime(info.song_length, s, m, h);
		if (h == 0)
		{
			if (m < 10)
				_stprintf_s(song_length, "歌曲长度：%d:%02d", m, s);
			else
				_stprintf_s(song_length, "歌曲长度：%02d:%02d", m, s);
		}
		else
		{
			if (m < 10) 
				_stprintf_s(song_length, "歌曲长度：%d:%d:%02d", h, m, s);
			else
				_stprintf_s(song_length, "歌曲长度：%d:%02d:%02d", h, m, s);
		}

		for (int i = 0; i < info.Level.size(); i++)
		{
			TCHAR temp_1[128] = { };
			strcpy_s(temp_1, info.Level[i].c_str());
			sprintf_s(temp, "当前难度：%s", temp_1);
			info.Level[i] = temp;
		}
		
		sprintf_s(current_song_display, "当前歌曲：%d / %d", info.ID, SONGS_NUM);
	}

	// 音乐时间显示转换
	void msToTime(long long milliseconds, int& s, int& m, int& h)
	{
		int seconds = milliseconds / 1000;
		h = seconds / 3600;
		m = (seconds % 3600) / 60;
		s = seconds % 60;
	}

	void Song_length_get()
	{
		TCHAR temp[64];
		TCHAR temp_length[256];
		_stprintf_s(temp, _T("status song_%d length"), info.ID);
		mciSendString(temp, temp_length , sizeof(temp_length), NULL);
		info.song_length = strtol(temp_length, NULL, 10); // 暂时不考虑字符集问题
	}

private:
	FILE* song_list_ptr;									// 读取谱面文件的指针
	bool is_loaded = false;									// 是否加载过本场景

	TCHAR song_time[64];									// 歌曲时长
	IMAGE* current_song_cover;								// 当前歌曲封面图片
	Button left_arrow_button;								// 向左切歌按钮
	Button right_arrow_button;								// 向右切歌按钮
	Button current_song_button;								// 当前歌曲封面（当做按钮处理）
	Button icon_setting_button;								// 设置按钮
	Button icon_pause_button;								// 暂停按钮
	Button exit_button;										// 返回按键
	Button play_songs_button;								// 歌曲从头开始播放按键
	RECT region_left_arrow_button = { };					// 向左切歌按钮判定范围
	RECT region_right_arrow_button = { };					// 向右切歌按钮判定范围
	RECT region_current_song_button = { };					// 当前歌曲封面判定范围
	RECT region_icon_setting_button = { };					// 设置按钮判定范围
	RECT region_icon_pause_button = { };					// 暂停按钮判定范围
	RECT region_exit_button = { };							// 返回按钮判定范围
	RECT region_play_songs_button = { };					// 歌曲从头开始播放按键判定范围
	POINT pos_left_arrow_button = { };						// 向左切歌按钮位置
	POINT pos_right_arrow_button = { };						// 向右切歌按钮位置
	POINT pos_current_song_button = { };					// 当前歌曲封面位置（当做按钮处理）
	POINT pos_icon_setting_button = { };					// 设置按钮位置
	POINT pos_icon_pause_button = { };						// 暂停按钮位置
	POINT pos_exit_button = { };							// 返回按键位置
	POINT pos_text = { };									// 文字位置
	POINT pos_play_songs_button = { };						// 歌曲从头开始播放按键位置
	bool is_left_arrow_button_clicked = false;				// 向左切歌按钮是否按下
	bool is_right_arrow_button_clicked = false; 			// 向右切歌按钮
	bool is_current_song_button_clicked = false;			// 当前歌曲封面按钮
	bool is_icon_setting_button_clicked = false;			// 设置按钮是否按下
	bool is_icon_pause_button_clicked = false;				// 暂停按钮是否按下
	bool is_exit_button_clicked = false;					// 返回按钮是否按下
	bool is_play_songs_button = false;						// 歌曲从头开始播放按键是否按下
	bool is_paused = false;									// 是否按下暂停键
	bool is_need_switch_song = false;						// 是否按键切换了歌曲

	std::vector<std::string> songs_list;					// 原始歌曲信息列表
	TCHAR current_song_display[20] = { };					// 当前歌曲显示
	int current_song = 1;									// 当前选择歌曲
	TCHAR song_length[128];									// 歌曲长度显示
};

#endif // !_SELECT_SONGS_SCENE_H_
