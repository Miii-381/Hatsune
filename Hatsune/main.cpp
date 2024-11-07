#include "utilities.h"
#include "atlas.h"
#include "button.h"
#include "click.h"
#include "scene_manager.h"
#include "opening_scene.h"
#include "menu_scene.h"
#include "setting_scene.h"
#include "select_songs_scene.h"
#include "gaming_scene.h"

#include <iostream>
#include <thread>

///////////////////////
//  游戏框架必要参数   //
///////////////////////

const int WINDOW_WIDTH = 1280;						// 窗口大小
const int WINDOW_HEIGHT = 720;
const int FPS = 144;								// FPS限制
int SONGS_NUM = 0;									// 歌曲数
bool is_debug = false;								// Debug模式
bool load_all_songs = true;							// 是否加载全部歌曲
int drop_speed = 50;								// 掉落速度（越大越快,100封顶）
int offset = 0;										// 音符偏移量

SceneManager scene_manager;							// 场景管理器
Scene* opening_scene = nullptr;						// 游戏场景
Scene* menu_scene = nullptr;
Scene* select_songs_scene = nullptr;
Scene* gaming_scene = nullptr;
Scene* setting_scene = nullptr;

// ResourceManager ResourceMgr;

///////////////
// 图片存放   //
///////////////

// 游戏标题
IMAGE img_title;

// 场景界面背景
IMAGE img_loading;
IMAGE img_background_opening_scene;					
IMAGE img_background_menu_scene;
IMAGE img_background_setting_scene;
IMAGE img_background_select_songs_scene;
								
// 主菜单界面
IMAGE img_start_button_idle;						
IMAGE img_start_button_hovered;
IMAGE img_start_button_clicked;
IMAGE img_setting_button_idle;
IMAGE img_setting_button_hovered;
IMAGE img_setting_button_clicked;
IMAGE img_about_button_idle;
IMAGE img_about_button_hovered;
IMAGE img_about_button_clicked;

// 歌曲选择界面
IMAGE img_current_song_button_idle;				// 当前歌曲封面（当作按钮处理）
IMAGE img_icon_setting_button_idle;				// 设置按键
IMAGE img_icon_setting_button_hovered;
IMAGE img_icon_setting_button_clicked;
IMAGE img_left_arrow_button_idle;				// 向左按键
IMAGE img_left_arrow_button_hovered;
IMAGE img_left_arrow_button_clicked;
IMAGE img_right_arrow_button_idle;				// 向右按键
IMAGE img_right_arrow_button_hovered;
IMAGE img_right_arrow_button_clicked;
IMAGE img_icon_pause_button_idle;				// 暂停按键
IMAGE img_icon_pause_button_hovered;
IMAGE img_icon_pause_button_clicked;

// 游戏界面
IMAGE img_masking;
IMAGE img_masking_miku;
IMAGE img_track;
IMAGE img_track_effect;
IMAGE img_click;
IMAGE img_hold;

Atlas songs_background;
Atlas songs_cover;
IMAGE img_cover_shadow_16_9;
IMAGE img_cover_shadow_4_3;
IMAGE img_cover_shadow_1_1;

IMAGE img_pause;
IMAGE img_continue_button_idle;			// 继续游戏按键
IMAGE img_continue_button_hovered;
IMAGE img_continue_button_clicked;
IMAGE img_end_button_idle;				// 结束游戏按键
IMAGE img_end_button_hovered;
IMAGE img_end_button_clicked;

// 设置界面
IMAGE img_setting_masking;
IMAGE img_left_arrow;
IMAGE img_right_arrow;
IMAGE img_exit_button_idle;
IMAGE img_exit_button_hovered;
IMAGE img_exit_button_clicked;
IMAGE img_box_masking;

// 字体格式
LOGFONT font_yahei;
LOGFONT font_miku;

// 游戏图片资源加载
void load_game_resources(bool load_all_songs)
{
	if (is_debug)
		std::cout << "load_game_resources start" << std::endl;
	loadimage(&img_title, _T("./resources/others/img_title.png"), 625, 190);

	loadimage(&img_background_opening_scene, _T("./resources/background/img_background_opening_scene.png"), 1280, 720);
	loadimage(&img_background_menu_scene, _T("./resources/background/img_background_menu_scene.png"),1280 ,720);
	loadimage(&img_background_setting_scene, _T("./resources/background/img_background_setting_scene.png"),1280, 720);
	loadimage(&img_background_select_songs_scene, _T("./resources/background/img_background_select_songs_scene.png"), 1280, 720);

	loadimage(&img_start_button_idle,_T("./resources/others/img_start_button_idle.png"));
	loadimage(&img_start_button_hovered,_T("./resources/others/img_start_button_hovered.png"));
	loadimage(&img_start_button_clicked,_T("./resources/others/img_start_button_clicked.png"));
	loadimage(&img_setting_button_idle, _T("./resources/others/img_setting_button_idle.png"));
	loadimage(&img_setting_button_hovered, _T("./resources/others/img_setting_button_hovered.png"));
	loadimage(&img_setting_button_clicked, _T("./resources/others/img_setting_button_clicked.png"));
	loadimage(&img_about_button_idle, _T("./resources/others/img_about_button_idle.png"));
	loadimage(&img_about_button_hovered, _T("./resources/others/img_about_button_hovered.png"));
	loadimage(&img_about_button_clicked, _T("./resources/others/img_about_button_clicked.png"));

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

	loadimage(&img_cover_shadow_16_9, _T("./resources/others/img_cover_shadow_16_9.png"));
	loadimage(&img_cover_shadow_4_3, _T("./resources/others/img_cover_shadow_4_3.png"));
	loadimage(&img_cover_shadow_1_1, _T("./resources/others/img_cover_shadow_1_1.png"));
	songs_cover.load_resources(_T("./resources/beatmaps/song_%d/cover.png"),_T("cover"), SONGS_NUM, 512, 288);		// 注意：歌曲封面和歌曲背景统一为PNG格式
	songs_background.load_resources(_T("./resources/beatmaps/song_%d/bg.png"), _T("bg"), SONGS_NUM, 1280, 720);

	loadimage(&img_masking, _T("./resources/others/img_masking.png"));
	loadimage(&img_masking_miku, _T("./resources/others/img_masking_miku.png"));
	loadimage(&img_track, _T("./resources/others/img_track.png"));
	loadimage(&img_track_effect, _T("./resources/others/img_track_effect.png"));
	loadimage(&img_click, _T("./resources/note/img_click.png"));
	loadimage(&img_hold, _T("./resources/note/img_hold.png"));
	loadimage(&img_pause, _T("./resources/others/img_pause.png"));
	loadimage(&img_continue_button_idle, _T("./resources/others/img_continue_button_idle.png"));
	loadimage(&img_continue_button_hovered, _T("./resources/others/img_continue_button_hovered.png"));
	loadimage(&img_continue_button_clicked, _T("./resources/others/img_continue_button_clicked.png"));
	loadimage(&img_end_button_idle, _T("./resources/others/img_end_button_idle.png"));
	loadimage(&img_end_button_hovered, _T("./resources/others/img_end_button_hovered.png"));
	loadimage(&img_end_button_clicked, _T("./resources/others/img_end_button_clicked.png"));

	loadimage(&img_setting_masking, _T("./resources/others/img_setting_masking.png"));
	loadimage(&img_left_arrow, _T("./resources/others/img_left_arrow.png"));
	loadimage(&img_right_arrow, _T("./resources/others/img_right_arrow.png"));
	loadimage(&img_exit_button_idle, _T("./resources/others/img_exit_button_idle.png"));
	loadimage(&img_exit_button_hovered, _T("./resources/others/img_exit_button_hovered.png"));
	loadimage(&img_exit_button_clicked, _T("./resources/others/img_exit_button_clicked.png"));
	loadimage(&img_box_masking, _T("./resources/others/img_box_masking.png"));

	if (is_debug)
		std::cout << "load_game_resources complete" << std::endl;
}

// 歌曲音频文件加载（从文件中读取歌曲名称，修改歌曲时不用在源代码中修改）
void load_songs(bool load_all_songs)
{
	std::ifstream file("./resources/beatmaps/songs_name.txt",std::ifstream::in);
	if (file.is_open() == false)
	{
		perror("fopen error!");
		MessageBox(GetHWnd(), _T("歌曲数获取错误，请检查后重新启动程序\n歌曲加载文件位于：.\\resources\\beatmaps\\songs_name.txt"), _T("ERROR!"), MB_OK | MB_ICONERROR);
		SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
	}

	TCHAR path_command[512];
	std::string temp_song_name;
	
	// 奇怪的问题：修改代码后mciSendString需要加一个文件类型限定才能播放音乐...执行一次代码之后又不需要了...、
	// 使用stprintf一是为字符集兼容（以后有时间进行大更新再说），二是因为string的运算符重载只支持字符和字符串拼接，stprintf可以将混合格式的元素整合到一个字符串中输出
	if (load_all_songs == false)
	{
		SONGS_NUM = 1;
		getline(file, temp_song_name);
		_stprintf_s(path_command, _T("open resources/beatmaps/song_%d/%s alias song_%d"), SONGS_NUM, temp_song_name.c_str(), SONGS_NUM); 
		printf("%s\n", path_command);
		mciSendString(path_command, NULL, 0, NULL);
	}	
	else
	{
		while (getline(file, temp_song_name))
		{
			_stprintf_s(path_command, _T("open resources/beatmaps/song_%d/%s alias song_%d"), SONGS_NUM + 1, temp_song_name.c_str(), SONGS_NUM + 1);
			printf("%s\n", path_command);
			mciSendString(path_command, NULL, 0, NULL);
			SONGS_NUM += 1;
		}
	}
	
	file.close();

	// SFX
	mciSendString(_T("open resources/sound/Tell_Your_World.mp3 alias menu_scene_BGM"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/SFX/button_selected.wav alias button_selected"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/SFX/click_hit.wav type MPEGVideo alias click_hit"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/SFX/song_selected.wav alias song_selected"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/SFX/score_increase.mp3 type MPEGVideo alias score_increase"), NULL, 0, NULL);

	// 游戏结算音效（小彩蛋~）
	mciSendString(_T("open resources/sound/game_end/Perfect/Awake.mp3 alias Perfect_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Brilliant_Work!.mp3 alias Perfect_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Precious_Memories.mp3 alias Perfect_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/水浴び大好きあひるちゃん.mp3 alias Perfect_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Theme_From_San_Andreas.mp3 alias Perfect_5"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Great/astral_ability.mp3 alias Great_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/本日は晴天なり.mp3 alias Great_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/海に行こうよ!.mp3 alias Great_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/輝きの瞬間.mp3 alias Great_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/心地いい日常.mp3 alias Great_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/陽だまり笑顔で.mp3 alias Great_6"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Good/Casual_Everyday.mp3 alias Good_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/Comfortable_Tiredness.mp3 alias Good_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/Fun_Fun_Friends.mp3 alias Good_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/大団円.mp3 alias Good_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/胸弾ませて.mp3 alias Good_5"), NULL, 0, NULL);
	
	mciSendString(_T("open resources/sound/game_end/Bad/Okamochi&Jersey.mp3 alias Bad_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/ずっこけ涙節.mp3 alias Bad_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/ダブル黒歴史ぼっち弾き語りversion.mp3 alias Bad_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/は…？.mp3 alias Bad_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/ほえ.mp3 alias Bad_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/案外いけるかも!.mp3 alias Bad_6"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/変態乳牛.mp3 alias Bad_7"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/何かがおかしい.mp3 alias Bad_8"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/なぜに何故？.mp3 alias Bad_9"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Worst/Can_You_Feel_It.mp3 alias Worst_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/Y.M.C.A.mp3 alias Worst_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/その日入った新人より使えないダメバイトのエレジー.mp3 alias Worst_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/はて？.mp3 alias Worst_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/やってもた.mp3 alias Worst_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/流浪者之歌.mp3 alias Worst_6"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/早安大森林.mp3 alias Worst_7"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/小丑.mp3 alias Worst_8"), NULL, 0, NULL);
}

// 游戏字体预设置
void set_font()
{
	lstrcpy(font_yahei.lfFaceName, _T("微软雅黑"));		
	font_yahei.lfHeight = 30;
	font_yahei.lfWidth = 0;
	font_yahei.lfQuality = PROOF_QUALITY;

	AddFontResourceEx(_T("resources/others/Aa未来黑65.ttf"), FR_PRIVATE, NULL); // Windows GDI 函数，将指定文件中的字体资源添加到系统，第二个参数限定该字体仅该程序可使用，且在程序结束后自动删除，第三个参数为保留参数，必须为0
	lstrcpy(font_miku.lfFaceName, _T("Aa未来黑 65"));
	font_miku.lfHeight = 80;
	font_miku.lfWidth = 0;
	font_miku.lfQuality = PROOF_QUALITY;
}

int main()
{
	// 游戏初始化
	ExMessage msg;	// 按键变量
	
	switch (MessageBox(GetHWnd(), _T("是否开启debug模式？"), _T("初始之音"), MB_YESNO))
	{
		case IDYES:
			initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EX_SHOWCONSOLE);	// 创建绘图窗口，捕捉绘图窗口句柄
			SetWindowText(GetHWnd(), _T("初始之音"));
			is_debug = true;
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
			initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);					// 创建绘图窗口，捕捉绘图窗口句柄
			SetWindowText(GetHWnd(), _T("初始之音"));
			break;
	};

	loadimage(&img_loading, _T("./resources/others/img_loading.png")); 
	putimage_alpha(0, 0, &img_loading);					// 由于全部文件都堆在主循环前加载，导致加载时间略微抽象，所以加个loading界面

	load_songs(load_all_songs);							// 歌曲数及歌曲音频文件加载
	load_game_resources(load_all_songs);				// 游戏图片加载
	set_font();											// 字体加载
	
	opening_scene = new OpeningScene();					// 实例化游戏场景
	menu_scene = new MenuScene();
	setting_scene = new SettingScene();
	select_songs_scene = new SelectSongsScene();
	gaming_scene = new GamingScene();
	scene_manager.set_current_scene(opening_scene);

	setbkmode(TRANSPARENT);								// 背景、字体设置(必须在窗口建立后方可设置)
	settextcolor(WHITE);
	settextstyle(&font_yahei);

	BeginBatchDraw();								// 双缓冲绘图，减少闪烁

	// 游戏主循环
	while (true)
	{
		auto frame_start_time = std::chrono::system_clock::now();

		while (peekmessage(&msg))					// 检测按键输入
			scene_manager.on_input(msg,is_debug);

		// 新实现：使用c++11给予的内置库<chrono>,可以将时间精确到微秒甚至纳秒级别。
		static auto last_tick_time = std::chrono::system_clock::now();
		auto current_tick_time = std::chrono::system_clock::now();
		auto delta_tick = std::chrono::duration_cast<std::chrono::milliseconds>(current_tick_time - last_tick_time).count();
		scene_manager.on_update(delta_tick, is_debug);
		last_tick_time = current_tick_time;

		cleardevice();
		scene_manager.on_draw(is_debug);					// 调用场景管理器中的绘图方法
		FlushBatchDraw();				

		// 帧率限制器
		auto frame_end_time = std::chrono::system_clock::now();
		auto frame_delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end_time - frame_start_time).count();
		if (frame_delta_time < 1000 / FPS)
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS - frame_delta_time));
	}

	EndBatchDraw();

	return 0;
}