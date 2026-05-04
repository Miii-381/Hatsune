#ifndef _GAMING_SCENE_H_
#define _GAMING_SCENE_H_

#include "ResourcesManager.h"
#include "scene.h"
#include "scene_manager.h"
#include "Atlas.h"
#include "utilities.h"
#include "note.h"
#include "click.h"
#include "hold.h"
#include "Button.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

extern SceneManager scene_manager;

class GamingScene : public Scene
{
public:
	// 跟踪每个轨道按键（D、F、J、K）当前是否被按下
	struct KeyStatus
	{
		bool status_D_now = false;
		bool status_F_now = false;
		bool status_J_now = false;
		bool status_K_now = false;
	} key_status;

public:
	GamingScene() : count_set(6)
	{
		timer_blur.restart();
		timer_blur.set_one_shot(false);
		timer_blur.set_wait_time(2);
		timer_blur.set_callback([&](int delta)
			{
				// 逐渐淡入场景过渡效果
				trans_strength_transition = max(trans_strength_transition - 1, 0);
				if (trans_strength_transition == 0)
				{
					transision_complete = true;
					if (transision_complete)
					{
						// 游戏正式开始前的倒计时
						game_start_wait_counter--;
						if (!game_start_wait_counter)
						{
							game_start = true;
							settextstyle(&ResourceMgr.font_miku);
						}
					}
				}
			});
		timer_blur.pause();
	};

	~GamingScene() = default;

	void on_enter(bool is_debug)
	{
		setfillcolor(0x7bbff8);
		srand(static_cast<unsigned int>(time(0)));

		enter_time = std::chrono::system_clock::now();

		// 使用更大的字体显示歌曲信息
		ResourceMgr.font_yahei.lfHeight = 35;
		settextstyle(&ResourceMgr.font_yahei);

		img_current_song_bg = ResourceMgr.songs_background.get_image(origin_info.ID);
		img_current_song_cover = ResourceMgr.songs_cover.get_image(origin_info.ID);
		if (ResourceMgr.songs_cover.get_scale(origin_info.ID) == ResourceMgr.songs_cover.scale_1_1)
			img_current_song_shadow = &ResourceMgr.img_cover_shadow_1_1;
		else if (ResourceMgr.songs_cover.get_scale(origin_info.ID) == ResourceMgr.songs_cover.scale_4_3)
			img_current_song_shadow = &ResourceMgr.img_cover_shadow_4_3;
		else if (ResourceMgr.songs_cover.get_scale(origin_info.ID) == ResourceMgr.songs_cover.scale_16_9)
			img_current_song_shadow = &ResourceMgr.img_cover_shadow_16_9;

		pos_current_song_cover.x = (getwidth() - img_current_song_cover->getwidth()) / 2;
		if (ResourceMgr.songs_cover.get_scale(origin_info.ID) == ResourceMgr.songs_cover.scale_1_1)
			pos_current_song_cover.y = 100;
		else if (ResourceMgr.songs_cover.get_scale(origin_info.ID) == ResourceMgr.songs_cover.scale_4_3)
			pos_current_song_cover.y = 150;
		else if (ResourceMgr.songs_cover.get_scale(origin_info.ID) == ResourceMgr.songs_cover.scale_16_9)
			pos_current_song_cover.y = 150;
		
		pos_info_name_text.x = (getwidth() - textwidth(origin_info.song_name)) / 2;
		pos_info_name_text.y = pos_current_song_cover.y + img_current_song_cover->getheight() + 10;
		pos_info_singer_text.x = (getwidth() - textwidth(origin_info.singer_name)) / 2;
		pos_info_singer_text.y = pos_info_name_text.y + ResourceMgr.font_yahei.lfHeight + 1;

		// 定义继续按钮的可点击区域
		region_continue_button.top = pos_continue_button.y;
		region_continue_button.left = pos_continue_button.x;
		region_continue_button.bottom = region_continue_button.top + ResourceMgr.img_continue_button_idle.getheight();
		region_continue_button.right = region_continue_button.left + ResourceMgr.img_continue_button_idle.getwidth();

		// 定义结束按钮的可点击区域
		region_end_button.top = pos_end_button.y;
		region_end_button.left = pos_end_button.x;
		region_end_button.bottom = region_end_button.top + ResourceMgr.img_end_button_idle.getheight();
		region_end_button.right = region_end_button.left + ResourceMgr.img_end_button_idle.getwidth();

		play_pos_get.clear();
		play_pos_get = _T("status song_") + std::to_string(origin_info.ID) + _T(" position");

		// 将歌曲总时长格式化为 HH:MM:SS 或 MM:SS 字符串
		int s, m, h;
		msToTime(origin_info.song_length, s, m, h);
		if (h == 0)
		{
			if (m < 10)
				_stprintf_s(song_length, "%d:%02d", m, s);
			else
				_stprintf_s(song_length, "%02d:%02d", m, s);
		}
		else
		{
			if (m < 10)
				_stprintf_s(song_length, "%d:%d:%02d", h, m, s);
			else
				_stprintf_s(song_length, "%d:%02d:%02d", h, m, s);
		}
	}

	void on_update(int delta, bool is_debug)
	{
		if (!game_pause)
		{
			// 游戏开始前：处理过渡效果并等待游戏开始
			if (game_start == false)
			{
				if (game_end == false)
				{
					if (transision_complete == false)
					{
						current_time = std::chrono::system_clock::now();
						if ((std::chrono::duration_cast<std::chrono::milliseconds>(current_time - enter_time).count()) >= 2000)
						{
							info_display = false;
							timer_blur.resume();
						}
					}
					if (is_debug) std::cout << delta << ' ' << game_start_wait_counter << std::endl;
					timer_blur.on_update(delta);
				}
			}
			// 游戏中：运行谱面和音符更新
			else
			{
				GetBeatmap();

				if (is_debug) printf_s("delta = %d", delta);

				// 更新每个轨道上的所有活动音符
				for (auto it = line_D_display.begin(); it != line_D_display.end(); ++it)
				{
					(*it)->on_update(delta, game_time, is_debug);
				}
				for (auto it = line_F_display.begin(); it != line_F_display.end(); ++it)
				{
					(*it)->on_update(delta, game_time, is_debug);
				}
				for (auto it = line_J_display.begin(); it != line_J_display.end(); ++it)
				{
					(*it)->on_update(delta, game_time, is_debug);
				}
				for (auto it = line_K_display.begin(); it != line_K_display.end(); ++it)
				{
					(*it)->on_update(delta, game_time, is_debug);
				}
				
				if (game_time <= 0)
				{
					game_time += delta;
				}
				else
				{
					// 开始播放歌曲音频（仅一次）
					if (music_play == false)
					{
						play_song_no_repeat(origin_info.ID);  
						music_play = true;
					}

					// 将游戏时间与 MCI 播放位置同步
					if (!game_end && transision_complete)
					{
						memset(play_pos, 0, sizeof(play_pos));
						mciSendString(play_pos_get.c_str(), play_pos, sizeof(play_pos), NULL);
						game_time = strtol(play_pos, NULL, 10);
					}
					AccuracyCalculate();
				}

				// 从显示队列中移除已判定的音符，统计 Miss 和长条音符得分
				if (!line_D_display.empty() && line_D_display.front()->get_judged())
					{
					if (line_D_display.front()->judge_level == Note::JudgeLevel::Miss)
					{
						judge_display = Note::JudgeLevel::Miss;
						if (line_D_display.front()->note_type == Note::NoteType::Hold)
							line_D_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
						line_D_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					}
					line_D_display.pop_front();
				}
				if (!line_F_display.empty() && line_F_display.front()->get_judged())
				{
					if (line_F_display.front()->judge_level == Note::JudgeLevel::Miss)
					{
						judge_display = Note::JudgeLevel::Miss;
						if (line_F_display.front()->note_type == Note::NoteType::Hold)
							line_F_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
						line_F_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					}
					line_F_display.pop_front();
				}
				if (!line_J_display.empty() && line_J_display.front()->get_judged())
				{
					if (line_J_display.front()->judge_level == Note::JudgeLevel::Miss)
					{
						judge_display = Note::JudgeLevel::Miss;
						if (line_J_display.front()->note_type == Note::NoteType::Hold)
							line_J_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
						line_J_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					}
					line_J_display.pop_front();
				}
				if (!line_K_display.empty() && line_K_display.front()->get_judged())
				{
					if (line_K_display.front()->judge_level == Note::JudgeLevel::Miss)
					{
						judge_display = Note::JudgeLevel::Miss;
						if (line_K_display.front()->note_type == Note::NoteType::Hold)
							line_K_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
						line_K_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					}
					line_K_display.pop_front();
				}
			}
			
			// 歌曲结束：切换到结算界面
			if (game_time >= origin_info.song_length && game_start == true && game_end == false)
			{
				game_start = false;
				game_end = true;
				point_display.clear();
				count_set_manage();

				pos_end_button.x = 850; pos_end_button.y = 560;

				region_end_button.top = pos_end_button.y;
				region_end_button.left = pos_end_button.x;
				region_end_button.bottom = region_end_button.top + ResourceMgr.img_end_button_idle.getheight();
				region_end_button.right = region_end_button.left + ResourceMgr.img_end_button_idle.getwidth();
			}

			// 结算界面：分数滚动动画和准确率评级
			if (game_end)
			{
				if (!point_change_finished)
				{
					if (!score_increase_SFX_play)
					{
						mciSendString(_T("play score_increase from 0"), NULL, 0, NULL);
						score_increase_SFX_play = true;
					}

					point_rolling = point_rolling + rand() % (((actual_point + 1) / 100) + 1);

					if (point_rolling >= actual_point)
					{
						point_rolling = actual_point;
						point_change_finished = true;
						srand(static_cast<unsigned int>(time(0)));

						// 根据准确率确定等级并播放对应的随机语音
						if (Accuracy > 1 - 1e-6)
						{
							TCHAR str[256];
							int num = rand() % 5 + 1;
							end_music = _T("Perfect_") + std::to_string(num);
							Level = _T("Perfect!!!");
							level_display = true;
							_stprintf_s(str, "play %s from 0", end_music.c_str());
							mciSendString(str, NULL, 0, NULL);
							mciSendString(_T("stop score_increase"), NULL, 0, NULL);
						}
						else if (Accuracy > 0.8 - 1e-6 && Accuracy <= 1 - 1e-6)
						{
							TCHAR str[256];
							int num = rand() % 6 + 1;
							end_music = _T("Great_") + std::to_string(num);
							Level = _T("Great!");
							level_display = true;
							_stprintf_s(str, "play %s from 0", end_music.c_str());
							mciSendString(str, NULL, 0, NULL);
							mciSendString(_T("stop score_increase"), NULL, 0, NULL);
						}
						else if (Accuracy > 0.6 - 1e-6 && Accuracy <= 0.8 - 1e-6)
						{
							TCHAR str[256];
							int num = rand() % 5 + 1;
							end_music = _T("Good_") + std::to_string(num);
							Level = _T("Good~");
							level_display = true;
							_stprintf_s(str, "play %s from 0", end_music.c_str());
							mciSendString(str, NULL, 0, NULL);
							mciSendString(_T("stop score_increase"), NULL, 0, NULL);
						}
						else if (Accuracy > 0.4 - 1e-6 && Accuracy <= 0.6 - 1e-6)
						{
							TCHAR str[256];
							int num = rand() % 9 + 1;
							end_music = _T("Bad_") + std::to_string(num);
							Level = _T("Bad(T_T)");
							level_display = true;
							_stprintf_s(str, "play %s from 0", end_music.c_str());
							mciSendString(str, NULL, 0, NULL);
							mciSendString(_T("stop score_increase"), NULL, 0, NULL);
						}
						else if (Accuracy <= 0.4 - 1e-6)
						{
							TCHAR str[256];
							int num = rand() % 8 + 1;
							end_music = _T("Worst_") + std::to_string(num);
							Level = _T("WorstT_T");
							level_display = true;
							_stprintf_s(str, "play %s from 0", end_music.c_str());
							mciSendString(str, NULL, 0, NULL);
							mciSendString(_T("stop score_increase"), NULL, 0, NULL);
						}
					}
					point_display = std::to_string(point_rolling);
				}
			}
		}
	}

	void on_draw(bool is_debug)
	{
		if (is_debug)
			std::cout << "gaming_scene" << std::endl;

		// 绘制带遮罩的背景
		putimage_alpha(0, 0, img_current_song_bg);
		putimage_alpha(0, 0, &ResourceMgr.img_masking, nullptr, trans_strength_bg);

		// 过渡阶段：显示封面、歌曲信息，并在后台加载谱面
		if (transision_complete == false)
		{
			putimage_alpha(0, 0, &ResourceMgr.img_masking, nullptr, trans_strength_transition);
			putimage_alpha(pos_current_song_cover.x, pos_current_song_cover.y, img_current_song_cover, img_current_song_shadow, trans_strength_transition);
			if (info_display)
			{
				outtextxy_shaded(pos_info_name_text.x, pos_info_name_text.y, origin_info.song_name);
				outtextxy_shaded(pos_info_singer_text.x, pos_info_singer_text.y, origin_info.singer_name);
			}
			// 在过渡期间加载谱面数据以避免阻塞游戏
			if (beatmap_loading == false)
			{
				FlushBatchDraw();
				load_beatmap();
				beatmap_loading = true;
			}
		}

		// 游戏中渲染：轨道、音符、分数、连击和判定
		if (game_start)
		{
			printf_s("game_time:%ld", game_time);

			putimage_alpha(pos_track_D.x, pos_track_D.y, &ResourceMgr.img_track);
			putimage_alpha(pos_track_F.x, pos_track_F.y, &ResourceMgr.img_track);
			putimage_alpha(pos_track_J.x, pos_track_J.y, &ResourceMgr.img_track);
			putimage_alpha(pos_track_K.x, pos_track_K.y, &ResourceMgr.img_track);

			// 绘制每个轨道上的所有活动音符
			for (auto it = line_D_display.begin(); it != line_D_display.end(); ++it)
			{
				(*it)->on_draw(is_debug);
			}
			for (auto it = line_F_display.begin(); it != line_F_display.end(); ++it)
			{
				(*it)->on_draw(is_debug);
			}
			for (auto it = line_J_display.begin(); it != line_J_display.end(); ++it)
			{
				(*it)->on_draw(is_debug);
			}
			for (auto it = line_K_display.begin(); it != line_K_display.end(); ++it)
			{
				(*it)->on_draw(is_debug);
			}

			// 当按下对应按键时高亮轨道
			if (key_status.status_D_now)
				putimage_alpha(pos_track_D.x, pos_track_D.y, &ResourceMgr.img_track_effect);
			if (key_status.status_F_now)
				putimage_alpha(pos_track_F.x, pos_track_F.y, &ResourceMgr.img_track_effect);
			if (key_status.status_J_now)
				putimage_alpha(pos_track_J.x, pos_track_J.y, &ResourceMgr.img_track_effect);
			if (key_status.status_K_now)
				putimage_alpha(pos_track_K.x, pos_track_K.y, &ResourceMgr.img_track_effect);

			// 绘制当前分数
			ResourceMgr.font_miku.lfHeight = 80;
			settextstyle(&ResourceMgr.font_miku);
			point_display = std::to_string(actual_point);
			outtextxy_shaded(getwidth() - textwidth(point_display.c_str()) - 20, 0, point_display.c_str());

			// 绘制已播放时间 / 总时间
			ResourceMgr.font_miku.lfHeight = 50;
			settextstyle(&ResourceMgr.font_miku);

			TCHAR play_pos[128] = { };
			int s, m, h;
			if(game_time <= 0)
				msToTime(0, s, m, h);
			else
				msToTime(game_time, s, m, h);

			if (h == 0)
			{
				if (m < 10)
					_stprintf_s(play_pos, "%d:%02d", m, s);
				else
					_stprintf_s(play_pos, "%02d:%02d", m, s);
			}
			else
			{
				if (m < 10)
					_stprintf_s(play_pos, "%d:%d:%02d", h, m, s);
				else
					_stprintf_s(play_pos, "%d:%02d:%02d", h, m, s);
			}
			_stprintf_s(play_pos, "%s / %s", play_pos, song_length);
			outtextxy_shaded(getwidth() - textwidth(play_pos) - 20, textheight(point_display.c_str()) + 30, play_pos);

			// 绘制判定文本（Perfect / Great / Good / Bad / Miss）
			ResourceMgr.font_miku.lfHeight = 40;
			settextstyle(&ResourceMgr.font_miku);

			switch (judge_display)
			{
			case Note::JudgeLevel::Perfect:
				outtextxy_shaded( (getwidth() - textwidth(_T("Perfect!!!"))) / 2, 400, _T("Perfect!!!"));
				break;
			case Note::JudgeLevel::Great:
				outtextxy_shaded((getwidth() - textwidth(_T("Great!!"))) / 2, 400, _T("Great!!"));
				break;
			case Note::JudgeLevel::Good:
				outtextxy_shaded((getwidth() - textwidth(_T("Good!"))) / 2, 400, _T("Good!"));
				break;
			case Note::JudgeLevel::Bad:
				outtextxy_shaded((getwidth() - textwidth(_T("Bad(T_T)"))) / 2, 400, _T("Bad(T_T)"));
				break;
			case Note::JudgeLevel::Miss:
				outtextxy_shaded((getwidth() - textwidth(_T("Miss......"))) / 2, 400, _T("Miss......"));
				break;
			default:
				break;
			};
			
			printf("%d", judge_display);
			
			// 在右侧绘制准确率和连击数
			TCHAR temp[256] = { };
			_stprintf_s(temp, _T("Accuracy: %.2lf%%"), Accuracy * 100);
			outtextxy_shaded(820, 300, temp);
			_stprintf_s(temp, _T("Combo: %dx"), combo);
			outtextxy_shaded(820, 350, temp);
		}
		
		// 暂停菜单：覆盖层和按钮
		if (game_pause)
		{
			putimage_alpha(0, 0, &ResourceMgr.img_pause);
			continue_button.on_draw(pos_continue_button.x, pos_continue_button.y, 255,
				&ResourceMgr.img_continue_button_idle, &ResourceMgr.img_continue_button_hovered, &ResourceMgr.img_continue_button_clicked);

			end_button.on_draw(pos_end_button.x, pos_end_button.y, 255,
				&ResourceMgr.img_end_button_idle, &ResourceMgr.img_end_button_hovered, &ResourceMgr.img_end_button_clicked);
		}

		// 结算界面：带有初音插画的結果显示
		if (game_end)
		{
			putimage_alpha(90, 51, &ResourceMgr.img_masking_miku);
			if (level_display)
			{
				ResourceMgr.font_miku.lfHeight = 150;
				settextstyle(&ResourceMgr.font_miku);
				if (Level == _T("Great!") || Level == _T("Good~"))
					outtextxy_shaded(660, 150, Level.c_str(), 0xC6EE76, 0xFFFFFF);
				else if(Level == _T("Perfect!!!") || Level == _T("WorstT_T") || Level == _T("Bad????"))
					outtextxy_shaded(560, 150, Level.c_str(), 0xC6EE76, 0xFFFFFF);
			}

			// 最终分数
			ResourceMgr.font_miku.lfHeight = 130;
			settextstyle(&ResourceMgr.font_miku);
			outtextxy_shaded(120, 100, point_display.c_str());

			// 准确率百分比
			ResourceMgr.font_miku.lfHeight = 40;
			settextstyle(&ResourceMgr.font_miku);
			outtextxy_shaded(120, 226, _T("Accuracy:"));

			ResourceMgr.font_miku.lfHeight = 100;
			settextstyle(&ResourceMgr.font_miku);
			outtextxy_shaded(140, 260, count_set[5].c_str());

			// 每种判定的统计数据（Perfect / Great / Good / Bad / Miss）
			ResourceMgr.font_miku.lfHeight = 50;
			settextstyle(&ResourceMgr.font_miku);
			outtextxy_shaded(130, 370, count_set[0].c_str());
			outtextxy_shaded(500, 370, count_set[1].c_str());
			outtextxy_shaded(130, 470, count_set[2].c_str());
			outtextxy_shaded(500, 470, count_set[3].c_str());
			outtextxy_shaded(130, 570, count_set[4].c_str());
			end_button.on_draw(pos_end_button.x, pos_end_button.y, 255, &ResourceMgr.img_end_button_idle, &ResourceMgr.img_end_button_hovered, &ResourceMgr.img_end_button_clicked);
		}
	}

	void on_input(const ExMessage& msg, bool is_debug)
	{
		is_continue_button_clicked = continue_button.on_input(msg, region_continue_button);
		is_end_button_clicked = end_button.on_input(msg, region_end_button);

		if (msg.message == WM_LBUTTONUP)
		{
			// 暂停时点击继续按钮 -> 恢复游戏
			if (is_continue_button_clicked && game_pause)
			{
				mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);
				game_pause = false;
				resume_song(origin_info.ID);
				is_continue_button_clicked = false;
			}

			// 暂停或结算界面点击结束按钮 -> 返回
			if (is_end_button_clicked && game_end || is_end_button_clicked && game_pause)
			{
				mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);
				scene_manager.switch_to_last_scene(is_debug);
				is_end_button_clicked = false;
			}
		}

		if (msg.message == WM_KEYDOWN)		
		{
			// ESC键：游戏中暂停 / 结算界面返回
			if (game_pause == false)
			{
				if (msg.vkcode == VK_ESCAPE)
				{
					if (game_end)
					{
						scene_manager.switch_to_last_scene(is_debug);
					}
					else 
					{
						game_pause = true;
						pause_song(origin_info.ID);
					}
				}
				if (msg.vkcode == VK_RETURN)
				{
					if (game_end)
						scene_manager.switch_to_last_scene(is_debug);
				}
			}
			else
			{
				if (is_continue_button_clicked)
				{
					game_pause = false;
					resume_song(origin_info.ID);
					is_continue_button_clicked = false;
				}
				if (is_end_button_clicked)
				{
					scene_manager.switch_to_last_scene(is_debug);
					is_end_button_clicked = false;
				}
			}

			// D键：判定 D 轨道的音符
			if (msg.vkcode == 0x44)
			{
				if (is_debug)
					std::cout << " D_pressed ";

				if(key_status.status_D_now == false)
					mciSendString(_T("play click_hit from 0"), NULL, 0, NULL);

				key_status.status_D_now = true;
				if (!line_D_display.empty())
				{
					line_D_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_D_display.front()->judge_level;
					line_D_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
				
			}
			// F键：判定 F 轨道的音符
			if (msg.vkcode == 0x46)
			{
				if (is_debug)
					std::cout << " F_pressed ";

				if (key_status.status_F_now == false)
					mciSendString(_T("play click_hit from 0"), NULL, 0, NULL);

				key_status.status_F_now = true;
				if (!line_F_display.empty())
				{
					line_F_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_F_display.front()->judge_level;
					line_F_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
				
			}
			// J键：判定 J 轨道的音符
			if (msg.vkcode == 0x4A)
			{
				if (is_debug)
					std::cout << " J_pressed ";

				if (key_status.status_J_now == false)
					mciSendString(_T("play click_hit from 0"), NULL, 0, NULL);

				key_status.status_J_now = true;
				if (!line_J_display.empty())
				{
					line_J_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_J_display.front()->judge_level;
					line_J_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
			}
			// K键：判定 K 轨道的音符
			if (msg.vkcode == 0x4B)
			{
				if (is_debug)
					std::cout << " K_pressed ";

				if (key_status.status_K_now == false)
					mciSendString(_T("play click_hit from 0"), NULL, 0, NULL);

				key_status.status_K_now = true;
				if (!line_K_display.empty())
				{
					line_K_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_K_display.front()->judge_level;
					line_K_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
			}

			// 调试快捷键：设置不同的分数场景用于测试
			if (is_debug)
			{
				switch (msg.vkcode) {
					case '6':
						actual_point = 700000;
						set_debug_vals();
						break;
					case '7':
						actual_point = 500000;
                        set_debug_vals();
						break;
					case '8':
						actual_point = 400000;
						set_debug_vals();
						break;
					case '9':
						actual_point = 300000;
						set_debug_vals();
						break;
					case '0':
						actual_point = 100000;
						set_debug_vals();
						break;
				}
			}
		}
		
		if (msg.message == WM_KEYUP)
		{
			// D键松开：如果当前音符是长条，则在松开时判定
			if (msg.vkcode == 0x44)
			{
				if (is_debug)
					std::cout << " D_idle ";

				key_status.status_D_now = false;
				if (!line_D_display.empty() && line_D_display.front()->get_type() == Note::NoteType::Hold)
				{
					line_D_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_D_display.front()->judge_level;
					line_D_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus); 
					judge_status_calculate(judge_display);
				}
			}
			// F键松开：长条判定
			if (msg.vkcode == 0x46)
			{
				if (is_debug)
					std::cout << " F_idle ";

				key_status.status_F_now = false;
				if (!line_F_display.empty() && line_F_display.front()->get_type() == Note::NoteType::Hold)
				{
					line_F_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_F_display.front()->judge_level;
					line_F_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
			}
			// J键松开：长条判定
			if (msg.vkcode == 0x4A)
			{
				if (is_debug)
					std::cout << " J_idle ";

				key_status.status_J_now = false;
				if (!line_J_display.empty() && line_J_display.front()->get_type() == Note::NoteType::Hold)
				{
					line_J_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_J_display.front()->judge_level;
					line_J_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
			}
			// K键松开：长条判定
			if (msg.vkcode == 0x4B)
			{
				if (is_debug)
					std::cout << " K_idle ";

				key_status.status_K_now = false;
				if (!line_K_display.empty() && line_K_display.front()->get_type() == Note::NoteType::Hold)
				{
					line_K_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_K_display.front()->judge_level;
					line_K_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
			}
		}
	}

	// 设置虚假的游戏结果用于调试测试
	void set_debug_vals() {
		ideal_point = 600000;
		PerfectCount = 100;
		GreatCount = 100;
		GoodCount = 100;
		BadCount = 100;
		MissCount = 100;
		AccuracyCalculate();
		count_set_manage();
		game_time = 9000000;
		game_start = false;
		game_end = true;
		stop_song(origin_info.ID);
		pos_end_button.x = 850; pos_end_button.y = 560;

		region_end_button.top = pos_end_button.y;
		region_end_button.left = pos_end_button.x;
		region_end_button.bottom = region_end_button.top + ResourceMgr.img_end_button_idle.getheight();
		region_end_button.right = region_end_button.left + ResourceMgr.img_end_button_idle.getwidth();
	}

	// 退出时重置所有场景状态
	void on_exit(bool is_debug)
	{
		stop_song(origin_info.ID);
		TCHAR temp[256];
		_stprintf_s(temp, _T("stop %s"), end_music.c_str());
		mciSendString(temp, NULL, 0, NULL);
		end_music.clear();
		line_D_display.clear();
		line_F_display.clear();
		line_J_display.clear();
		line_K_display.clear();
		note_storage.clear();
		note_count = 0;
		judge_display = Note::JudgeLevel::Invalid;
		game_time = -1000;
		game_start = false;
		game_end = false;
		game_pause = false;
		beatmap_loading = false;
		music_play = false;
		score_increase_SFX_play = false;
		is_continue_button_clicked = false;
		is_end_button_clicked = false;
		game_start_wait_counter = 100;
		info_display = true;
		transision_complete = false;
		point_change_finished = false;
		pos_continue_button = { 280,480 };
		pos_end_button = { 780,480 };
		img_current_song_cover = nullptr;
		img_current_song_bg = nullptr;
		trans_strength_transition = 255;
		trans_strength_bg = 255;
		timer_blur.restart();
		timer_blur.pause();
		actual_point = 0;
		ideal_point = 0;
		point_rolling = 0;
		combo = 0;
		ideal_combo = 0;
		PerfectCount = 0;
		GreatCount = 0;
		GoodCount = 0;
		BadCount = 0;
		MissCount = 0;
		Accuracy = 0.0;
		for (int i = 0; i < 6; i++)
		{
			count_set[i].clear();
		}
		level_display = false;
		Level.clear();
		ResourceMgr.font_miku.lfHeight = 80;
		ResourceMgr.font_yahei.lfHeight = 30;
		settextstyle(&ResourceMgr.font_yahei);
		memset(play_pos, '\0', sizeof(play_pos));
	}

	// 解析谱面文件并用 Click / Hold 音符填充 note_storage
	void load_beatmap()
	{
		int ID = 0;
		int last_start_time = 0;

		TCHAR path_template[256] = { };
		_stprintf_s(path_template, _T("./resources/beatmaps/song_%d/%s.txt"), origin_info.ID, origin_info.Level[current_level].c_str());

		std::ifstream file(path_template, std::ifstream::in);
		if (file.is_open() == false)
		{
			TCHAR path_error_msg[1024] = { };
			_stprintf_s(path_error_msg, sizeof(path_error_msg),_T("??????????????????\n????????.\\resources\\beatmaps\\song_%d\\%s.txt"), origin_info.ID, origin_info.Level[current_level].c_str());
			perror("fopen error!");
			MessageBox(GetHWnd(), path_error_msg, _T("??????"), MB_OK | MB_ICONERROR);
			SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
		}
		memset(path_template, '\0', sizeof(path_template));

		std::string line;
		while (getline(file, line))
		{
			int pos = 0, start_time = 0, end_time = 0, type = 0;
			char delimiter;

			std::istringstream data(line);

			if (is_debug)
				std::cout << line << std::endl;

			// 每行格式：type,pos,start_time,end_time
			if (data >> type >> delimiter && delimiter == ',' &&
				data >> pos >> delimiter && delimiter == ',' &&
				data >> start_time >> delimiter && delimiter == ',' &&
				data >> end_time)
			{
				if (is_debug)
					std::cout << type << ' ' << pos << ' ' << start_time << ' ' << end_time << ' ' << std::endl;
			}
			else
			{
				MessageBox(GetHWnd(), _T("????????????????????TxT?????"), _T("??????"), MB_OK | MB_ICONERROR);
				SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			}

			// 按开始时间分组；相同开始时间的音符放入同一个向量
			if (last_start_time != start_time)
			{
				ID += 1;
				note_storage.push_back(std::vector<Note*>());
			}

			last_start_time = start_time;

			if(type == 1)
				note_storage[ID - 1].push_back(new Click(numToPosition(pos), start_time, ID, type));
			else if(type == 2)
				note_storage[ID - 1].push_back(new Hold(numToPosition(pos), start_time, end_time, ID, type));
		}
	}

	// 将数字轨道标识符转换为 Note::Position 枚举
	Note::Position numToPosition(int num)
	{
		if (num == 1)
			return Note::Position::Key_D;
		else if (num == 2)
			return Note::Position::Key_F;
		else if (num == 3)
			return Note::Position::Key_J;
		else if (num == 4)
			return Note::Position::Key_K;
		else
		{
			MessageBox(GetHWnd(), _T("??????????????????????????TxT?????"), _T("??????"), MB_OK | MB_ICONERROR);
			SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			return Note::Position::Invalid;
		}
	}
	
	// 根据当前游戏时间将即将到来的音符从存储区推入各轨道的显示队列
	void GetBeatmap()
	{
		if (game_time <= 0)
		{
			while ((note_storage.size() > note_count) && (note_storage[note_count][0]->GetNoteTime() - 0 <= 3000))
			{
				for (auto it = note_storage[note_count].begin(); it != note_storage[note_count].end(); ++it)
				{
					switch ((*it)->getPosition())
					{
					case Note::Position::Key_D:
						line_D_display.push_back(*it);
						break;
					case Note::Position::Key_F:
						line_F_display.push_back(*it);
						break;
					case Note::Position::Key_J:
						line_J_display.push_back(*it);
						break;
					case Note::Position::Key_K:
						line_K_display.push_back(*it);
						break;
					}
				}
				note_count += 1;
			}
		}
		else
		{
			while ((note_storage.size() > note_count) && (note_storage[note_count][0]->GetNoteTime() - game_time <= 3000))
			{
				for (auto it = note_storage[note_count].begin(); it != note_storage[note_count].end(); ++it)
				{
					switch ((*it)->getPosition())
					{
					case Note::Position::Key_D:
						line_D_display.push_back(*it);
						break;
					case Note::Position::Key_F:
						line_F_display.push_back(*it);
						break;
					case Note::Position::Key_J:
						line_J_display.push_back(*it);
						break;
					case Note::Position::Key_K:
						line_K_display.push_back(*it);
						break;
					}
				}
				note_count += 1;
			}
		}
	}

	// 增加给定判定等级的计数器
	void judge_status_calculate(Note::JudgeLevel level)
	{
		switch (level)
		{
		case Note::JudgeLevel::Perfect:
			PerfectCount += 1;
			break;
		case Note::JudgeLevel::Great:
			GreatCount += 1;
			break;
		case Note::JudgeLevel::Good:
			GoodCount += 1;
			break;
		case Note::JudgeLevel::Bad:
			BadCount += 1;
			break;
		case Note::JudgeLevel::Miss:
			MissCount += 1;
			break;
		default:
			break;
		}
	}

	// 构建结算界面的统计字符串（Perfect/Great/Good/Bad/Miss 数量 + 准确率）
	void count_set_manage()
	{
		count_set[0] = _T("Perfect: ") + std::to_string(PerfectCount) + _T('x');
		count_set[1] = _T("Great: ") + std::to_string(GreatCount) + _T('x');
		count_set[2] = _T("Good: ") + std::to_string(GoodCount) + _T('x');
		count_set[3] = _T("Bad: ") + std::to_string(BadCount) + _T('x');
		count_set[4] = _T("Miss: ") + std::to_string(MissCount) + _T('x');
		TCHAR temp[256] = { };
		_stprintf_s(temp, _T("%.2f%%"), Accuracy * 100);
		count_set[5] = temp;
	}
	
	// 计算准确率：实际得分 / 理想得分
	void AccuracyCalculate()
	{
		if (ideal_point == 0)
			Accuracy = 0.0;
		else
			Accuracy = (float)actual_point / ideal_point;
	}

private:
	// 二维向量，按节拍时间对所有音符进行分组；每组可能包含多个同时出现的音符
	std::vector<std::vector<Note*>> note_storage;
	
	// 下一个要送入显示队列的音符组索引
	int note_count = 0;

	// 每个轨道（D、F、J、K）的实时显示队列
	std::list<Note*> line_D_display;
	std::list<Note*> line_F_display;
	std::list<Note*> line_J_display;
	std::list<Note*> line_K_display;

	// 当前显示的判定结果
	Note::JudgeLevel judge_display = Note::JudgeLevel::Invalid;

	// MCI 命令字符串，用于查询当前播放位置
	std::string play_pos_get;
	// 格式化后的当前播放时间缓冲区（MM:SS）
	TCHAR play_pos[128];
	// 格式化后的歌曲总时长（MM:SS）
	TCHAR song_length[64];

	// 目前已获得的实际分数
	int actual_point = 0;
	// 最大可能分数
	int ideal_point = 0;
	// 结算界面上显示的动画滚动分数
	int point_rolling = 0;

	// 当前连击数
	int combo = 0;
	// 最大可能连击数
	int ideal_combo = 0;
	// 连击分数倍率
	double combo_bonus = 1.0;

	// 每种判定等级的计数器
	unsigned int PerfectCount = 0;
	unsigned int GreatCount = 0;
	unsigned int GoodCount = 0;
	unsigned int BadCount = 0;
	unsigned int MissCount = 0;

	// 击中准确率（0.0 到 1.0）
	float Accuracy = 0.0;

	// 谱面数据是否已加载
	bool beatmap_loading = false;

	// 随机选择的结算界面语音片段名称（例如 "Perfect_3"）
	std::string end_music;

private:
	// 进入场景的时间戳
	std::chrono::system_clock::time_point enter_time;
	// 用于测量过渡期间经过时间的时间戳
	std::chrono::system_clock::time_point current_time;
	// 控制淡入过渡效果的计时器
	Timer timer_blur;

	// 游戏中经过的时间（毫秒）；负值表示在游戏开始前的准备阶段
	int game_time = -1000;
	// 过渡完成后游戏正式开始前的延迟帧计数器
	int game_start_wait_counter = 100;

	// 场景过渡淡入效果的透明度级别（255 = 完全不透明）
	BYTE trans_strength_transition = 255;
	// 背景遮罩的透明度级别
	BYTE trans_strength_bg = 255;

	// 游戏是否已开始（音符正在下落）
	bool game_start = false;
	// 歌曲是否已结束并显示结算界面
	bool game_end = false;
	// 歌曲音频是否已开始播放
	bool music_play = false;
	// 过渡期间歌曲信息（名称、歌手）是否当前可见
	bool info_display = true;
	// 场景过渡动画是否已完成
	bool transision_complete = false;
	// 游戏是否暂停
	bool game_pause = false;
	// 结算界面上的滚动分数动画是否已完成
	bool point_change_finished = false;
	
	// 暂停 / 结算界面按钮
	Button continue_button;
	Button end_button;
	POINT pos_continue_button = { 260,460 };
	POINT pos_end_button = { 760,460 };
	RECT region_continue_button = { 0,0 };
	RECT region_end_button = { 0,0 };
	bool is_continue_button_clicked = false;
	bool is_end_button_clicked = false;

	// 结算界面统计数据的字符串数组（Perfect/Great/Good/Bad/Miss/Accuracy）
	std::vector<std::string> count_set;
	// 当前分数的字符串表示，用于显示
	std::string point_display;
	// 是否应在结算界面上显示等级标签
	bool level_display;
	// 等级标签文本（例如 "Perfect!!!"、"Great!"）
	std::string Level;

	// 分数滚动音效是否正在播放
	bool score_increase_SFX_play;

private:
	// 当前歌曲的封面图片
	IMAGE* img_current_song_cover = nullptr;
	// 与封面宽高比匹配的阴影/装饰框
	IMAGE* img_current_song_shadow = nullptr;
	// 当前歌曲的完整背景图片
	IMAGE* img_current_song_bg = nullptr;

	// 封面的屏幕位置
	POINT pos_current_song_cover = { 0,0 };
	// 歌曲名称文本的屏幕位置
	POINT pos_info_name_text = { 0,0 };
	// 歌手名称文本的屏幕位置
	POINT pos_info_singer_text = { 0,0 };

	// 四个音符轨道的固定屏幕位置
	POINT pos_track_D = { 481,0 };
	POINT pos_track_F = { 562,0 };
	POINT pos_track_J = { 643,0 };
	POINT pos_track_K = { 724,0 };
};


#endif // !_GAMING_SCENE_H_