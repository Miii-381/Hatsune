#ifndef _RESOURCES_MANAGER_H_
#define _RESOURCES_MANAGER_H_

#include "Atlas.h"
#include <graphics.h>
#include <fstream>
#include <string>

// 资源管理器类：负责加载和管理所有游戏资源（图片、音频、字体等）
class ResourcesManager
{
public:
	ResourcesManager() = default;
	~ResourcesManager() = default;

	// 游戏基本参数
	int SONGS_NUM = 0;      // 歌曲总数
	int drop_speed = 50;    // 音符下落速度
	int offset = 0;         // 判定偏移量（毫秒）

	// 图片资源 - 加载界面
	IMAGE img_loading;

	// 图片资源 - 背景图
	IMAGE img_background_opening_scene;       // 开场场景背景
	IMAGE img_background_menu_scene;          // 主菜单背景
	IMAGE img_background_setting_scene;       // 设置界面背景
	IMAGE img_background_select_songs_scene;  // 选歌界面背景

	// 图片资源 - 标题
	IMAGE img_title;  // 游戏标题图片

	// 图片资源 - 主菜单按钮
	IMAGE img_start_button_idle;     // 开始按钮-空闲状态
	IMAGE img_start_button_hovered;  // 开始按钮-悬停状态
	IMAGE img_start_button_clicked;  // 开始按钮-点击状态
	IMAGE img_setting_button_idle;   // 设置按钮-空闲状态
	IMAGE img_setting_button_hovered;// 设置按钮-悬停状态
	IMAGE img_setting_button_clicked;// 设置按钮-点击状态
	IMAGE img_about_button_idle;     // 关于按钮-空闲状态
	IMAGE img_about_button_hovered;  // 关于按钮-悬停状态
	IMAGE img_about_button_clicked;  // 关于按钮-点击状态

	// 图片资源 - 选歌界面按钮
	IMAGE img_current_song_button_idle;      // 当前歌曲封面按钮-空闲
	IMAGE img_icon_setting_button_idle;      // 设置图标-空闲
	IMAGE img_icon_setting_button_hovered;   // 设置图标-悬停
	IMAGE img_icon_setting_button_clicked;   // 设置图标-点击
	IMAGE img_left_arrow_button_idle;        // 左箭头-空闲
	IMAGE img_left_arrow_button_hovered;     // 左箭头-悬停
	IMAGE img_left_arrow_button_clicked;     // 左箭头-点击
	IMAGE img_right_arrow_button_idle;       // 右箭头-空闲
	IMAGE img_right_arrow_button_hovered;    // 右箭头-悬停
	IMAGE img_right_arrow_button_clicked;    // 右箭头-点击
	IMAGE img_icon_pause_button_idle;        // 暂停图标-空闲
	IMAGE img_icon_pause_button_hovered;     // 暂停图标-悬停
	IMAGE img_icon_pause_button_clicked;     // 暂停图标-点击

	// 图片资源 - 游戏界面
	IMAGE img_masking;           // 遮罩层
	IMAGE img_masking_miku;      // Miku遮罩层
	IMAGE img_track;             // 轨道背景
	IMAGE img_track_effect;      // 轨道特效
	IMAGE img_click;             // 单击音符图片
	IMAGE img_hold;              // 长按音符图片

	// 歌曲封面和背景图集
	Atlas songs_background;      // 歌曲背景图集
	Atlas songs_cover;           // 歌曲封面图集
	
	// 封面阴影图片（不同比例）
	IMAGE img_cover_shadow_16_9; // 16:9比例阴影
	IMAGE img_cover_shadow_4_3;  // 4:3比例阴影
	IMAGE img_cover_shadow_1_1;  // 1:1比例阴影

	// 暂停界面相关图片
	IMAGE img_pause;                     // 暂停图标
	IMAGE img_continue_button_idle;      // 继续按钮-空闲
	IMAGE img_continue_button_hovered;   // 继续按钮-悬停
	IMAGE img_continue_button_clicked;   // 继续按钮-点击
	IMAGE img_end_button_idle;           // 结束按钮-空闲
	IMAGE img_end_button_hovered;        // 结束按钮-悬停
	IMAGE img_end_button_clicked;        // 结束按钮-点击

	// 图片资源 - 设置界面
	IMAGE img_setting_masking;   // 设置界面遮罩
	IMAGE img_left_arrow;        // 左箭头小图标
	IMAGE img_right_arrow;       // 右箭头小图标
	IMAGE img_exit_button_idle;  // 退出按钮-空闲
	IMAGE img_exit_button_hovered;// 退出按钮-悬停
	IMAGE img_exit_button_clicked;// 退出按钮-点击
	IMAGE img_box_masking;       // 数值调节框遮罩

	// 字体资源
	LOGFONT font_yahei;  // 微软雅黑字体
	LOGFONT font_miku;   // 初音未来风格字体

	// 加载所有游戏图片资源
	void load_game_resources()
	{
		// 加载标题图片
		loadimage(&img_title, _T("./resources/others/img_title.png"), 625, 190);

		// 加载背景图片
		loadimage(&img_background_opening_scene, _T("./resources/background/img_background_opening_scene.png"), 1280, 720);
		loadimage(&img_background_menu_scene, _T("./resources/background/img_background_menu_scene.png"), 1280, 720);
		loadimage(&img_background_setting_scene, _T("./resources/background/img_background_setting_scene.png"), 1280, 720);
		loadimage(&img_background_select_songs_scene, _T("./resources/background/img_background_select_songs_scene.png"), 1280, 720);

		// 加载主菜单按钮图片
		loadimage(&img_start_button_idle, _T("./resources/others/img_start_button_idle.png"));
		loadimage(&img_start_button_hovered, _T("./resources/others/img_start_button_hovered.png"));
		loadimage(&img_start_button_clicked, _T("./resources/others/img_start_button_clicked.png"));
		loadimage(&img_setting_button_idle, _T("./resources/others/img_setting_button_idle.png"));
		loadimage(&img_setting_button_hovered, _T("./resources/others/img_setting_button_hovered.png"));
		loadimage(&img_setting_button_clicked, _T("./resources/others/img_setting_button_clicked.png"));
		loadimage(&img_about_button_idle, _T("./resources/others/img_about_button_idle.png"));
		loadimage(&img_about_button_hovered, _T("./resources/others/img_about_button_hovered.png"));
		loadimage(&img_about_button_clicked, _T("./resources/others/img_about_button_clicked.png"));

		// 加载选歌界面按钮图片
		loadimage(&img_left_arrow_button_idle, _T("./resources/others/img_left_arrow_button_idle.png"), 80, 80);
		loadimage(&img_left_arrow_button_hovered, _T("./resources/others/img_left_arrow_button_hovered.png"), 80, 80);
		loadimage(&img_left_arrow_button_clicked, _T("./resources/others/img_left_arrow_button_clicked.png"), 80, 80);
		loadimage(&img_right_arrow_button_idle, _T("./resources/others/img_right_arrow_button_idle.png"), 80, 80);
		loadimage(&img_right_arrow_button_hovered, _T("./resources/others/img_right_arrow_button_hovered.png"), 80, 80);
		loadimage(&img_right_arrow_button_clicked, _T("./resources/others/img_right_arrow_button_clicked.png"), 80, 80);
		loadimage(&img_icon_setting_button_idle, _T("./resources/others/img_icon_setting_button_idle.png"), 50, 50);
		loadimage(&img_icon_setting_button_hovered, _T("./resources/others/img_icon_setting_button_hovered.png"), 50, 50);
		loadimage(&img_icon_setting_button_clicked, _T("./resources/others/img_icon_setting_button_clicked.png"), 50, 50);
		loadimage(&img_icon_pause_button_idle, _T("./resources/others/img_icon_pause_button_idle.png"), 50, 50);
		loadimage(&img_icon_pause_button_hovered, _T("./resources/others/img_icon_pause_button_hovered.png"), 50, 50);
		loadimage(&img_icon_pause_button_clicked, _T("./resources/others/img_icon_pause_button_clicked.png"), 50, 50);

		// 加载封面阴影图片
		loadimage(&img_cover_shadow_16_9, _T("./resources/others/img_cover_shadow_16_9.png"));
		loadimage(&img_cover_shadow_4_3, _T("./resources/others/img_cover_shadow_4_3.png"));
		loadimage(&img_cover_shadow_1_1, _T("./resources/others/img_cover_shadow_1_1.png"));
		
		// 加载歌曲封面和背景图集
		songs_cover.load_resources(_T("./resources/beatmaps/song_%d/cover.png"), _T("cover"), SONGS_NUM, 512, 288);
		songs_background.load_resources(_T("./resources/beatmaps/song_%d/bg.png"), _T("bg"), SONGS_NUM, 1280, 720);

		// 加载游戏界面图片
		loadimage(&img_masking, _T("./resources/others/img_masking.png"));
		loadimage(&img_masking_miku, _T("./resources/others/img_masking_miku.png"));
		loadimage(&img_track, _T("./resources/others/img_track.png"));
		loadimage(&img_track_effect, _T("./resources/others/img_track_effect.png"));
		loadimage(&img_click, _T("./resources/note/img_click.png"));
		loadimage(&img_hold, _T("./resources/note/img_hold.png"));
		
		// 加载暂停界面图片
		loadimage(&img_pause, _T("./resources/others/img_pause.png"));
		loadimage(&img_continue_button_idle, _T("./resources/others/img_continue_button_idle.png"));
		loadimage(&img_continue_button_hovered, _T("./resources/others/img_continue_button_hovered.png"));
		loadimage(&img_continue_button_clicked, _T("./resources/others/img_continue_button_clicked.png"));
		loadimage(&img_end_button_idle, _T("./resources/others/img_end_button_idle.png"));
		loadimage(&img_end_button_hovered, _T("./resources/others/img_end_button_hovered.png"));
		loadimage(&img_end_button_clicked, _T("./resources/others/img_end_button_clicked.png"));

		// 加载设置界面图片
		loadimage(&img_setting_masking, _T("./resources/others/img_setting_masking.png"));
		loadimage(&img_left_arrow, _T("./resources/others/img_left_arrow.png"));
		loadimage(&img_right_arrow, _T("./resources/others/img_right_arrow.png"));
		loadimage(&img_exit_button_idle, _T("./resources/others/img_exit_button_idle.png"));
		loadimage(&img_exit_button_hovered, _T("./resources/others/img_exit_button_hovered.png"));
		loadimage(&img_exit_button_clicked, _T("./resources/others/img_exit_button_clicked.png"));
		loadimage(&img_box_masking, _T("./resources/others/img_box_masking.png"));
	}

	// 加载歌曲音频资源（包括音效）
	// load_all_songs: 是否加载所有歌曲（false则只加载第一首用于测试）
	void load_songs(bool load_all_songs)
	{
		// 打开歌曲名称列表文件
		std::ifstream file("./resources/beatmaps/songs_name.txt", std::ifstream::in);
		if (file.is_open() == false)
		{
			perror("fopen error!");
			MessageBox(GetHWnd(), _T("歌曲列表读取失败，请检查该文件是否存在！\n文件位于：.\\resources\\beatmaps\\songs_name.txt"), _T("ERROR!"), MB_OK | MB_ICONERROR);
			SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
		}

		TCHAR path_command[512];
		std::string temp_song_name;

		// 使用MCI命令打开歌曲音频文件，格式为song_编号
		if (load_all_songs == false)
		{
			// 只加载第一首歌曲（调试模式）
			SONGS_NUM = 1;
			getline(file, temp_song_name);
			_stprintf_s(path_command, _T("open resources/beatmaps/song_%d/%s alias song_%d"), SONGS_NUM, temp_song_name.c_str(), SONGS_NUM);
			mciSendString(path_command, NULL, 0, NULL);
		}
		else
		{
			// 加载所有歌曲
			while (getline(file, temp_song_name))
			{
				_stprintf_s(path_command, _T("open resources/beatmaps/song_%d/%s alias song_%d"), SONGS_NUM + 1, temp_song_name.c_str(), SONGS_NUM + 1);
				mciSendString(path_command, NULL, 0, NULL);
				SONGS_NUM += 1;
			}
		}

		file.close();

		// 加载音效资源
		mciSendString(_T("open resources/sound/Tell_Your_World.mp3 alias menu_scene_BGM"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/SFX/button_selected.wav alias button_selected"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/SFX/click_hit.wav type MPEGVideo alias click_hit"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/SFX/song_selected.wav alias song_selected"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/SFX/score_increase.mp3 type MPEGVideo alias score_increase"), NULL, 0, NULL);

		// 加载结算界面音效（Perfect评价）
		mciSendString(_T("open resources/sound/game_end/Perfect/Awake.mp3 alias Perfect_1"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Perfect/Brilliant_Work!.mp3 alias Perfect_2"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Perfect/Precious_Memories.mp3 alias Perfect_3"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Perfect/水浴び大好きあひるちゃん.mp3 alias Perfect_4"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Perfect/Theme_From_San_Andreas.mp3 alias Perfect_5"), NULL, 0, NULL);

		// 加载结算界面音效（Great评价）
		mciSendString(_T("open resources/sound/game_end/Great/astral_ability.mp3 alias Great_1"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Great/本日は晴天なり.mp3 alias Great_2"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Great/海に行こうよ!.mp3 alias Great_3"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Great/輝きの瞬間.mp3 alias Great_4"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Great/心地いい日常.mp3 alias Great_5"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Great/陽だまり笑顔で.mp3 alias Great_6"), NULL, 0, NULL);

		// 加载结算界面音效（Good评价）
		mciSendString(_T("open resources/sound/game_end/Good/Casual_Everyday.mp3 alias Good_1"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Good/Comfortable_Tiredness.mp3 alias Good_2"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Good/Fun_Fun_Friends.mp3 alias Good_3"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Good/大団円.mp3 alias Good_4"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Good/胸弾ませて.mp3 alias Good_5"), NULL, 0, NULL);

		// 加载结算界面音效（Bad评价）
		mciSendString(_T("open resources/sound/game_end/Bad/Okamochi&Jersey.mp3 alias Bad_1"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Bad/ずっこけ涙節.mp3 alias Bad_2"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Bad/ダブル黒歴史ぼっち弾き語りversion.mp3 alias Bad_3"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Bad/なぜに何故？.mp3 alias Bad_4"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Bad/は…？.mp3 alias Bad_5"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Bad/ほえ.mp3 alias Bad_6"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Bad/案外いけるかも!.mp3 alias Bad_7"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Bad/変態乳牛.mp3 alias Bad_8"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Bad/何かがおかしい.mp3 alias Bad_9"), NULL, 0, NULL);

		// 加载结算界面音效（Worst评价）
		mciSendString(_T("open resources/sound/game_end/Worst/Can_You_Feel_It.mp3 alias Worst_1"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Worst/Y.M.C.A.mp3 alias Worst_2"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Worst/その日入った新人より使えないダメバイトのエレジー.mp3 alias Worst_3"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Worst/はて？.mp3 alias Worst_4"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Worst/やってもた.mp3 alias Worst_5"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Worst/流浪者之歌.mp3 alias Worst_6"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Worst/小丑.mp3 alias Worst_7"), NULL, 0, NULL);
		mciSendString(_T("open resources/sound/game_end/Worst/早安大森林.mp3 alias Worst_8"), NULL, 0, NULL);
	}

	// 设置字体样式
	void set_font()
	{
		// 配置微软雅黑字体
		lstrcpy(font_yahei.lfFaceName, _T("微软雅黑"));
		font_yahei.lfHeight = 30;
		font_yahei.lfWidth = 0;
		font_yahei.lfQuality = PROOF_QUALITY;

		// 加载并配置初音未来风格字体
		AddFontResourceEx(_T("resources/others/Aa未来黑65.ttf"), FR_PRIVATE, NULL);
		lstrcpy(font_miku.lfFaceName, _T("Aa未来黑 65"));
		font_miku.lfHeight = 80;
		font_miku.lfWidth = 0;
		font_miku.lfQuality = PROOF_QUALITY;
	}
};

extern ResourcesManager ResourceMgr;  // 全局资源管理器实例声明

#endif // !_RESOURCES_MANAGER_H_
