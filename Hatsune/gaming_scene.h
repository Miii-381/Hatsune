#ifndef _GAMING_SCENE_H_
#define _GAMING_SCENE_H_

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
extern Atlas songs_cover;
extern Atlas songs_background;
extern IMAGE img_cover_shadow_1_1;
extern IMAGE img_cover_shadow_4_3;
extern IMAGE img_cover_shadow_16_9;
extern IMAGE img_masking;
extern IMAGE img_masking_miku;
extern IMAGE img_track;
extern IMAGE img_track_effect;
extern IMAGE img_click;
extern IMAGE img_pause;
extern IMAGE img_continue_button_idle;
extern IMAGE img_continue_button_hovered;
extern IMAGE img_continue_button_clicked;
extern IMAGE img_end_button_idle;
extern IMAGE img_end_button_hovered;
extern IMAGE img_end_button_clicked;
extern IMAGE img_exit_button_idle;
extern IMAGE img_exit_button_hovered;
extern IMAGE img_exit_button_clicked;

extern LOGFONT font_yahei;
extern LOGFONT font_miku;

class GamingScene : public Scene
{
public:
	struct KeyStatus										// ����״̬������ʵ�ְ�����Ч��ʾ
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
				trans_strength_transition = max(trans_strength_transition - 1, 0);
				if (trans_strength_transition == 0)
				{
					transision_complete = true;
					if (transision_complete)
					{
						game_start_wait_counter--;
						if (!game_start_wait_counter)
						{
							game_start = true;
							settextstyle(&font_miku);
						}
					}
				}
			});
		timer_blur.pause();
	};

	~GamingScene() = default;

	void on_enter(bool is_debug)
	{
		game_time = 0;
		setfillcolor(0x7bbff8);
		srand(static_cast<unsigned int>(time(0)));		// static_cast��c++�е�ǿ������ת��
		enter_time = std::chrono::system_clock::now();
		// ���ɽ���ĸ������桢������Ӱ����Ϣ����
		font_yahei.lfHeight = 35;
		settextstyle(&font_yahei);

		img_current_song_bg = songs_background.get_image(origin_info.ID);
		img_current_song_cover = songs_cover.get_image(origin_info.ID);
		if (songs_cover.get_scale(origin_info.ID) == songs_cover.scale_1_1)
			img_current_song_shadow = &img_cover_shadow_1_1;
		else if (songs_cover.get_scale(origin_info.ID) == songs_cover.scale_4_3)
			img_current_song_shadow = &img_cover_shadow_4_3;
		else if (songs_cover.get_scale(origin_info.ID) == songs_cover.scale_16_9)
			img_current_song_shadow = &img_cover_shadow_16_9;

		pos_current_song_cover.x = (getwidth() - img_current_song_cover->getwidth()) / 2;
		if (songs_cover.get_scale(origin_info.ID) == songs_cover.scale_1_1)
			pos_current_song_cover.y = 100;
		else if (songs_cover.get_scale(origin_info.ID) == songs_cover.scale_4_3)
			pos_current_song_cover.y = 150;
		else if (songs_cover.get_scale(origin_info.ID) == songs_cover.scale_16_9)
			pos_current_song_cover.y = 150;
		
		pos_info_name_text.x = (getwidth() - textwidth(origin_info.song_name)) / 2;
		pos_info_name_text.y = pos_current_song_cover.y + img_current_song_cover->getheight() + 10;
		pos_info_singer_text.x = (getwidth() - textwidth(origin_info.singer_name)) / 2;
		pos_info_singer_text.y = pos_info_name_text.y + font_yahei.lfHeight + 1;

		// ��ť�ж���Χ
		region_continue_button.top = pos_continue_button.y;
		region_continue_button.left = pos_continue_button.x;
		region_continue_button.bottom = region_continue_button.top + img_continue_button_idle.getheight();
		region_continue_button.right = region_continue_button.left + img_continue_button_idle.getwidth();

		region_end_button.top = pos_end_button.y;
		region_end_button.left = pos_end_button.x;
		region_end_button.bottom = region_end_button.top + img_end_button_idle.getheight();
		region_end_button.right = region_end_button.left + img_end_button_idle.getwidth();

		play_pos_get.clear();
		play_pos_get = _T("status song_") + std::to_string(origin_info.ID) + _T(" position");
	}

	void on_update(int delta, bool is_debug)
	{
		if (!game_pause)
		{
			// ��ʼǰ�ĵ��뵭��
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
			// ��ʼ�����Ϸ�߼�
			else
			{
				// ���ֲ���
				if (music_play == false)
				{
					play_song_no_repeat(origin_info.ID);
					music_play = true;
				}

				if (!game_end && transision_complete)
				{
					mciSendString(play_pos_get.c_str(), play_pos, sizeof(play_pos), NULL);
					game_time = strtol(play_pos, NULL, 10) + offset; // �ַ���ת��Ϊ������
					memset(play_pos, 0, sizeof(play_pos));
				}
					//game_time = delta + game_time;		// ��Ϸʱ���ۼӣ������ж�note����Ϸ�Ƿ����
				// ������Ϣ��ʹ��֡�䶨ʱ���ᷢ������ƫ�������ۼӣ�������Ϸ�ٶȱ���
				// ����֡�䶨ʱ��������Ϸʱ����ת��ʹ�����ֲ���ʱ���������Ϸʱ����ȷ����������~
				if (is_debug) printf_s("delta = %d", delta);

				GetBeatmap();

				if (!line_D_display.empty() && line_D_display.front()->get_judged())
				{
					if (line_D_display.front()->judge_level == Note::JudgeLevel::Miss)
					{
						judge_display = Note::JudgeLevel::Miss;
						if(line_D_display.front()->note_type == Note::NoteType::Hold)
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

				AccuracyCalculate();

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
			}

			// ���������ж�
			if (game_time >= origin_info.song_length && game_start == true && game_end == false)
			{
				game_start = false;
				game_end = true;
				point_display.clear();
				count_set_manage();

				pos_end_button.x = 850; pos_end_button.y = 560;

				region_end_button.top = pos_end_button.y;
				region_end_button.left = pos_end_button.x;
				region_end_button.bottom = region_end_button.top + img_end_button_idle.getheight();
				region_end_button.right = region_end_button.left + img_end_button_idle.getwidth();
			}
			// ����ҳ�����
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
						if (Accuracy > 1 - 1e-6)
						{
							TCHAR str[256];
							int num = rand() % 5 + 1;	// ��5�׸�
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
							int num = rand() % 6 + 1;	// ��6�׸�
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
							int num = rand() % 5 + 1;	// ��5�׸�
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
							int num = rand() % 9 + 1;	// ��9�׸�
							end_music = _T("Bad_") + std::to_string(num);
							Level = _T("Bad���䣼");
							level_display = true;
							_stprintf_s(str, "play %s from 0", end_music.c_str());
							mciSendString(str, NULL, 0, NULL);
							mciSendString(_T("stop score_increase"), NULL, 0, NULL);
						}
						else if (Accuracy <= 0.4 - 1e-6)
						{
							TCHAR str[256];
							int num = rand() % 8 + 1;	// ��8�׸�
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

		// ����
		putimage_alpha(0, 0, img_current_song_bg);
		putimage_alpha(0, 0, &img_masking, trans_strength_bg);

		// �볡��������
		if (transision_complete == false)
		{
			putimage_alpha(0, 0, &img_masking, trans_strength_transition);
			putimage_alpha_shadow(pos_current_song_cover.x, pos_current_song_cover.y, img_current_song_cover, img_current_song_shadow, trans_strength_transition);
			if (info_display)
			{
				outtextxy_shaded(pos_info_name_text.x, pos_info_name_text.y, origin_info.song_name);
				outtextxy_shaded(pos_info_singer_text.x, pos_info_singer_text.y, origin_info.singer_name);
			}
			// ������أ�û�취��Ҫ������ʾ����ʱ�������棬�������ظ��ܣ��������ڵ���Ϸ�߼���ֻ��д��on_draw�������棬����Ҫ��ǰ����˫�����ڵ�ͼ��...��
			if (beatmap_loading == false)
			{
				FlushBatchDraw();
				load_beatmap();
				beatmap_loading = true;
			}
		}

		// ��Ϸ��ʼ��Ļ�ͼ�߼�
		if (game_start)
		{
			printf_s("game_time:%ld", game_time);

			// ���
			putimage_alpha(pos_track_D.x, pos_track_D.y, &img_track);
			putimage_alpha(pos_track_F.x, pos_track_F.y, &img_track);
			putimage_alpha(pos_track_J.x, pos_track_J.y, &img_track);
			putimage_alpha(pos_track_K.x, pos_track_K.y, &img_track);

			for (auto it = line_D_display.begin(); it != line_D_display.end(); ++it)	// ����������list
			{
				(*it)->on_draw(is_debug);
			}
			for (auto it = line_F_display.begin(); it != line_F_display.end(); ++it)	// ����������list
			{
				(*it)->on_draw(is_debug);
			}
			for (auto it = line_J_display.begin(); it != line_J_display.end(); ++it)	// ����������list
			{
				(*it)->on_draw(is_debug);
			}
			for (auto it = line_K_display.begin(); it != line_K_display.end(); ++it)	// ����������list
			{
				(*it)->on_draw(is_debug);
			}

			if (key_status.status_D_now)
				putimage_alpha(pos_track_D.x, pos_track_D.y, &img_track_effect);
			if (key_status.status_F_now)
				putimage_alpha(pos_track_F.x, pos_track_F.y, &img_track_effect);
			if (key_status.status_J_now)
				putimage_alpha(pos_track_J.x, pos_track_J.y, &img_track_effect);
			if (key_status.status_K_now)
				putimage_alpha(pos_track_K.x, pos_track_K.y, &img_track_effect);

			font_miku.lfHeight = 80;
			settextstyle(&font_miku);
			point_display = std::to_string(actual_point);
			outtextxy_shaded(getwidth() - textwidth(point_display.c_str()) - 20, 0, point_display.c_str());

			font_miku.lfHeight = 40;
			settextstyle(&font_miku);
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
			TCHAR temp[256] = { };
			_stprintf_s(temp, _T("Accuracy: %.2lf%%"), Accuracy * 100);
			outtextxy_shaded(820, 300, temp);
			_stprintf_s(temp, _T("Combo: %dx"), combo);
			outtextxy_shaded(820, 350, temp);
		}
		
		// ��ͣ�����ͼ�߼�
		if (game_pause)
		{
			putimage_alpha(0, 0, &img_pause);
			continue_button.on_draw(pos_continue_button.x, pos_continue_button.y, 255,
				&img_continue_button_idle, &img_continue_button_hovered, &img_continue_button_clicked);

			end_button.on_draw(pos_end_button.x, pos_end_button.y, 255,
				&img_end_button_idle, &img_end_button_hovered, &img_end_button_clicked);
		}

		// ��������ͼ�߼�
		if (game_end)
		{
			putimage_alpha(90, 51, &img_masking_miku);
			if (level_display)
			{
				font_miku.lfHeight = 150;
				settextstyle(&font_miku);
				if (Level == _T("Great!") || Level == _T("Good~"))
					outtextxy_shaded(660, 150, Level.c_str(), 0xC6EE76, 0xFFFFFF);
				else if(Level == _T("Perfect!!!") || Level == _T("WorstT_T") || Level == _T("Bad���䣼"))
					outtextxy_shaded(560, 150, Level.c_str(), 0xC6EE76, 0xFFFFFF);
			}

			font_miku.lfHeight = 130;
			settextstyle(&font_miku);
			outtextxy_shaded(120, 100, point_display.c_str());

			font_miku.lfHeight = 40;
			settextstyle(&font_miku);
			outtextxy_shaded(120, 226, _T("Accuracy:"));

			font_miku.lfHeight = 100;
			settextstyle(&font_miku);
			outtextxy_shaded(140, 260, count_set[5].c_str());

			font_miku.lfHeight = 50;
			settextstyle(&font_miku);
			outtextxy_shaded(130, 370, count_set[0].c_str());
			outtextxy_shaded(500, 370, count_set[1].c_str());
			outtextxy_shaded(130, 470, count_set[2].c_str());
			outtextxy_shaded(500, 470, count_set[3].c_str());
			outtextxy_shaded(130, 570, count_set[4].c_str());
			end_button.on_draw(pos_end_button.x, pos_end_button.y, 255, &img_end_button_idle, &img_end_button_hovered, &img_end_button_clicked);
		}
	}

	void on_input(const ExMessage& msg, bool is_debug)
	{
		is_continue_button_clicked = continue_button.on_input(msg, region_continue_button);
		is_end_button_clicked = end_button.on_input(msg, region_end_button);

		if (msg.message == WM_LBUTTONUP)
		{
			if (is_continue_button_clicked && game_pause)
			{
				mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);
				game_pause = false;
				resume_song(origin_info.ID);
				is_continue_button_clicked = false;
			}

			if (is_end_button_clicked && game_end || is_end_button_clicked && game_pause)
			{
				mciSendString(_T("play button_selected from 0"), NULL, 0, NULL);
				scene_manager.switch_to_last_scene(is_debug);
				is_end_button_clicked = false;
			}
		}

		if (msg.message == WM_KEYDOWN)		
		{
			// ��ͣ���˳��߼�
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

			// noteʶ���߼�
			if (msg.vkcode == 0x44)	// Key D
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
			if (msg.vkcode == 0x46)	// Key F
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
			if (msg.vkcode == 0x4A)	// Key J
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
			if (msg.vkcode == 0x4B)	// Key K
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

			// ���Դ���
			if (is_debug)
			{
				if (msg.vkcode == VK_NUMPAD1)
				{
					actual_point = 700000;
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
					region_end_button.bottom = region_end_button.top + img_end_button_idle.getheight();
					region_end_button.right = region_end_button.left + img_end_button_idle.getwidth();
				}

				if (msg.vkcode == VK_NUMPAD2)
				{
					actual_point = 500000;
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
					region_end_button.bottom = region_end_button.top + img_end_button_idle.getheight();
					region_end_button.right = region_end_button.left + img_end_button_idle.getwidth();
				}

				if (msg.vkcode == VK_NUMPAD3)
				{
					actual_point = 400000;
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
					region_end_button.bottom = region_end_button.top + img_end_button_idle.getheight();
					region_end_button.right = region_end_button.left + img_end_button_idle.getwidth();
				}

				if (msg.vkcode == VK_NUMPAD4)
				{
					actual_point = 300000;
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
					region_end_button.bottom = region_end_button.top + img_end_button_idle.getheight();
					region_end_button.right = region_end_button.left + img_end_button_idle.getwidth();
				}

				if (msg.vkcode == VK_NUMPAD5)
				{
					actual_point = 100000;
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
					region_end_button.bottom = region_end_button.top + img_end_button_idle.getheight();
					region_end_button.right = region_end_button.left + img_end_button_idle.getwidth();
				}
			}
		}
		
		if (msg.message == WM_KEYUP)
		{
			if (msg.vkcode == 0x44)
			{
				if (is_debug)
					std::cout << " D_idle ";

				key_status.status_D_now = false;
				if (!line_D_display.empty() && line_D_display.front()->get_type() == Note::NoteType::Hold)
				{
					// mciSendString(_T("play click_hit from 0"), NULL, 0, NULL);     ���ڳ����ж��߼���˳���ԣ�����hold�ɼ�������������������һ���ʱ����Ч�����΢С�ݳ�����ʱû������������ع��ж��߼����ߴ洢֮ǰ����״̬���������ж��߼�ִ����֮��ͳһ���Ű�����Ч
					line_D_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_D_display.front()->judge_level;
					line_D_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus); 
					judge_status_calculate(judge_display);
				}
			}
			if (msg.vkcode == 0x46)
			{
				if (is_debug)
					std::cout << " F_idle ";

				key_status.status_F_now = false;
				if (!line_F_display.empty() && line_F_display.front()->get_type() == Note::NoteType::Hold)
				{
					// mciSendString(_T("play click_hit from 0"), NULL, 0, NULL);
					line_F_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_F_display.front()->judge_level;
					line_F_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
			}
			if (msg.vkcode == 0x4A)
			{
				if (is_debug)
					std::cout << " J_idle ";

				key_status.status_J_now = false;
				if (!line_J_display.empty() && line_J_display.front()->get_type() == Note::NoteType::Hold)
				{
					// mciSendString(_T("play click_hit from 0"), NULL, 0, NULL);
					line_J_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_J_display.front()->judge_level;
					line_J_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
			}
			if (msg.vkcode == 0x4B)
			{
				if (is_debug)
					std::cout << " K_idle ";

				key_status.status_K_now = false;
				if (!line_K_display.empty() && line_K_display.front()->get_type() == Note::NoteType::Hold)
				{
					// mciSendString(_T("play click_hit from 0"), NULL, 0, NULL);
					line_K_display.front()->judge(game_time, is_debug, msg.message);
					judge_display = line_K_display.front()->judge_level;
					line_K_display.front()->get_point(actual_point, ideal_point, combo, ideal_combo, combo_bonus);
					judge_status_calculate(judge_display);
				}
			}
		}
	}

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
		game_time = 0;
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
		point_rolling = 0;
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
		font_miku.lfHeight = 80;
		font_yahei.lfHeight = 30;
		settextstyle(&font_yahei);
		memset(play_pos, '\0', sizeof(play_pos));
	}

	// �������
	void load_beatmap()
	{
		int ID = 0;
		int last_start_time = 0;		// �������ʱ���ݵ�ǰnote_time���ϸ�note_time����ID
		TCHAR path_template[256] = { };
		_stprintf_s(path_template, _T("./resources/beatmaps/song_%d/%s.txt"), origin_info.ID, origin_info.Level[current_level].c_str());

		std::ifstream file(path_template, std::ifstream::in);
		if (file.is_open() == false)
		{
			TCHAR path_error_msg[1024] = { };
			_stprintf_s(path_error_msg, sizeof(path_error_msg),_T("�����ļ���ȡ���������������������\n�����ļ�λ�ڣ�.\\resources\\beatmaps\\song_%d\\%s.txt"), origin_info.ID, origin_info.Level[current_level].c_str());
			perror("fopen error!");
			MessageBox(GetHWnd(), path_error_msg, _T("ERROR!"), MB_OK);
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

			if (data >> type >> delimiter && delimiter == ',' &&
				data >> pos >> delimiter && delimiter == ',' &&
				data >> start_time >> delimiter && delimiter == ',' &&
				data >> end_time)
			{
				// ����ɹ���ȡ������������������֮���ɶ��ŷָ�  
				if (is_debug)
					std::cout << type << ' ' << pos << ' ' << start_time << ' ' << end_time << ' ' << std::endl;
			}
			else
			{
				MessageBox(GetHWnd(), _T("�������ش������������������Ϸ��TxT��"), _T("ERROR!"), MB_OK);
				SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			}

			if (last_start_time != start_time)
			{
				ID += 1;
				note_storage.push_back(std::vector<Note*>()); // Ϊ��ά��������µĺ��У����캯����
			}

			last_start_time = start_time;

			// note�������������Ǻ���������ж�Ϊ���������ٶȽϳ��������ٶȸ���
			// ��������һ���߳����������ţ��������ǵ��߳����У����ܴ���һ����������
			// ����Ϊclick���������ʼƫ�������½�������⣬����hold������Ȼ���������������ٶ�

			// 2024/11/2 ��������ֱ�ӻ�ȡ��Ƶ����ʱ�����Ϊ��Ϸʱ�䣬�ܶ�����~
			if(type == 1)
				note_storage[ID - 1].push_back(new Click(numToPosition(pos), start_time, ID, type));
			else if(type == 2)
				note_storage[ID - 1].push_back(new Hold(numToPosition(pos), start_time, end_time, ID, type));
		}
	}

	// �������ʱ��λ������ת��
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
			MessageBox(GetHWnd(), _T("����λ����д�������������������Ϸ��TxT��"), _T("ERROR!"), MB_OK);
			SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			return Note::Position::Invalid;
		}
	}
	
	// ʵʱ������������
	void GetBeatmap()
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

	// ��Ϸ�����е����ݼ�¼
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

	// ����������Ϸ������ʾ
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
	
	// ׼�ȼ���
	void AccuracyCalculate()
	{
		if (ideal_point == 0)
			Accuracy = 0.0;
		else
			Accuracy = (float)actual_point / ideal_point;
	}

private: // note
	std::vector<std::vector<Note*>> note_storage;		// �洢�������Ա���á�

	int note_count = 0;									// ��¼���õ��ڼ�����

	std::list<Note*> line_D_display;					// ������ʵʱ���ص���
	std::list<Note*> line_F_display;
	std::list<Note*> line_J_display;
	std::list<Note*> line_K_display;

	Note::JudgeLevel judge_display = Note::JudgeLevel::Invalid;	// note�ж������ʾ

	std::string play_pos_get;							// ���ڻ�ȡ��ǰ����λ��
	char play_pos[128];									// ��ŵ�ǰ����λ�ã�mci���ص����ַ���������ֻ�ܴ�����ַ����У�

	// ��Ϸ���ݼ�¼
	int actual_point = 0;			// ʵ���������
	int ideal_point = 0;			// ����ֵ
	int point_rolling = 0;			// ������������������

	int	combo = 0;					// ������
	int ideal_combo = 0;			// ����������	
	double combo_bonus = 1.0;		// ��������

	unsigned int PerfectCount = 0;	// ���ݼ�¼
	unsigned int GreatCount = 0;
	unsigned int GoodCount = 0;
	unsigned int BadCount = 0;
	unsigned int MissCount = 0;
	float Accuracy = 0.0;

	bool beatmap_loading = false;	// �����Ƿ񱻼��أ���ֹ�ظ�����

	std::string end_music;			// �洢�������������������ֲ���

private: // ����
	std::chrono::system_clock::time_point enter_time;		// ���볡����ʱ��
	std::chrono::system_clock::time_point current_time;		// ��ǰʱ��
	Timer timer_blur;										// ���뵭����ʱ��
	DWORD game_time = 0;									// ��Ϸ��ʱ����ʹ�����������ڵ�����ʱ������������ж�
	int game_start_wait_counter = 100;						// ���������������ͣ��ʱ�䣨Խ��Խ�ã���debug��ֵȷ��ѭ��ʱ����������ȴ�ʱ�䣩

	BYTE trans_strength_transition = 255;					// ���ɻ�������͸����
	BYTE trans_strength_bg = 255;							// ����͸����
	bool game_start = false;								// �Ƿ���Կ�ʼ��Ϸ
	bool game_end = false;									// ��Ϸ�Ƿ����
	bool music_play = false;								// �����Ƿ����ڲ���
	bool info_display = true;								// �Ƿ���ʾ������Ϣ������������
	bool transision_complete = false;						// ��ʼ�Ĺ��������Ƿ����
	bool game_pause = false;								// ��Ϸ�Ƿ���ͣ
	bool point_change_finished = false;						// ������ʾ�Ƿ����
	
	// ��ͣ���水ť��װ
	Button continue_button;
	Button end_button;
	POINT pos_continue_button = { 260,460 };
	POINT pos_end_button = { 760,460 };
	RECT region_continue_button = { 0,0 };
	RECT region_end_button = { 0,0 };
	bool is_continue_button_clicked = false;
	bool is_end_button_clicked = false;

	std::vector<std::string> count_set;						// �������ݼ�¼�����������ʾ���ڳ����Ĺ��캯����̶��䳤�ȣ�
	std::string point_display;								// ����������ʵ����ʾ
	bool level_display;										// �������ȼ��Ƿ���ʾ
	std::string Level;										// �������ȼ���ʾ

	bool score_increase_SFX_play;							// ����������Ч����

private: // ͼƬ
	IMAGE* img_current_song_cover = nullptr;				// ��ǰѡ���������ͼƬ
	IMAGE* img_current_song_shadow = nullptr;				// ������Ӱ
	IMAGE* img_current_song_bg = nullptr;					// ��ǰѡ���������ͼƬ
	POINT pos_current_song_cover = { 0,0 };					// ��ǰѡ���������λ��
	POINT pos_info_name_text = { 0,0 };						// ��������
	POINT pos_info_singer_text = { 0,0 };					// ��������
	POINT pos_track_D = { 481,0 };							// ���ͼƬλ��
	POINT pos_track_F = { 562,0 };							
	POINT pos_track_J = { 643,0 };							
	POINT pos_track_K = { 724,0 };							
};


#endif // !_GAMING_SCENE_H_