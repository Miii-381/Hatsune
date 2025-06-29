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
//  嗄老崇尺駅勣歌方   //
///////////////////////

const int WINDOW_WIDTH = 1280;						// 完笥寄弌
const int WINDOW_HEIGHT = 720;
const int FPS = 144;								// FPS�渣�
int SONGS_NUM = 0;									// 梧爆方
bool is_debug = false;								// Debug庁塀
bool load_all_songs = true;							// 頁倦紗墮畠何梧爆
int drop_speed = 50;								// 渠鯛堀業��埆寄埆酔,100撃競��
int offset = 0;										// 咄憲陶卞楚

SceneManager scene_manager;							// 魁尚砿尖匂
Scene* opening_scene = nullptr;						// 嗄老魁尚
Scene* menu_scene = nullptr;
Scene* select_songs_scene = nullptr;
Scene* gaming_scene = nullptr;
Scene* setting_scene = nullptr;

// ResourceManager ResourceMgr;

///////////////
// 夕頭贋慧   //
///////////////

// 嗄老炎籾
IMAGE img_title;

// 魁尚順中嘘尚
IMAGE img_loading;
IMAGE img_background_opening_scene;					
IMAGE img_background_menu_scene;
IMAGE img_background_setting_scene;
IMAGE img_background_select_songs_scene;
								
// 麼暇汽順中
IMAGE img_start_button_idle;						
IMAGE img_start_button_hovered;
IMAGE img_start_button_clicked;
IMAGE img_setting_button_idle;
IMAGE img_setting_button_hovered;
IMAGE img_setting_button_clicked;
IMAGE img_about_button_idle;
IMAGE img_about_button_hovered;
IMAGE img_about_button_clicked;

// 梧爆僉夲順中
IMAGE img_current_song_button_idle;				// 輝念梧爆撃中�┻益�梓泥侃尖��
IMAGE img_icon_setting_button_idle;				// 譜崔梓囚
IMAGE img_icon_setting_button_hovered;
IMAGE img_icon_setting_button_clicked;
IMAGE img_left_arrow_button_idle;				// �鰈鶲桓�
IMAGE img_left_arrow_button_hovered;
IMAGE img_left_arrow_button_clicked;
IMAGE img_right_arrow_button_idle;				// �鰉勸桓�
IMAGE img_right_arrow_button_hovered;
IMAGE img_right_arrow_button_clicked;
IMAGE img_icon_pause_button_idle;				// 壙唯梓囚
IMAGE img_icon_pause_button_hovered;
IMAGE img_icon_pause_button_clicked;

// 嗄老順中
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
IMAGE img_continue_button_idle;			// 写偬嗄老梓囚
IMAGE img_continue_button_hovered;
IMAGE img_continue_button_clicked;
IMAGE img_end_button_idle;				// 潤崩嗄老梓囚
IMAGE img_end_button_hovered;
IMAGE img_end_button_clicked;

// 譜崔順中
IMAGE img_setting_masking;
IMAGE img_left_arrow;
IMAGE img_right_arrow;
IMAGE img_exit_button_idle;
IMAGE img_exit_button_hovered;
IMAGE img_exit_button_clicked;
IMAGE img_box_masking;

// 忖悶鯉塀
LOGFONT font_yahei;
LOGFONT font_miku;

// 嗄老夕頭彿坿紗墮
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
	songs_cover.load_resources(_T("./resources/beatmaps/song_%d/cover.png"),_T("cover"), SONGS_NUM, 512, 288);		// 廣吭�左菁�撃中才梧爆嘘尚由匯葎PNG鯉塀
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

// 梧爆咄撞猟周紗墮�┫嗄勅�嶄響函梧爆兆各��俐個梧爆扮音喘壓坿旗鷹嶄俐個��
void load_songs(bool load_all_songs)
{
	std::ifstream file("./resources/beatmaps/songs_name.txt",std::ifstream::in);
	if (file.is_open() == false)
	{
		perror("fopen error!");
		MessageBox(GetHWnd(), _T("梧爆方資函危列��萩殊臥朔嶷仟尼強殻會\n梧爆紗墮猟周了噐��.\\resources\\beatmaps\\songs_name.txt"), _T("ERROR!"), MB_OK | MB_ICONERROR);
		SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
	}

	TCHAR path_command[512];
	std::string temp_song_name;
	
	// 謎講議諒籾�歳涕調�鷹朔mciSendString俶勣紗匯倖猟周窃侏�浙┣田棆シ賭�赤...峇佩匯肝旗鷹岻朔嗽音俶勣阻...、
	// 聞喘stprintf匯頁葎忖憲鹿惹否��參朔嗤扮寂序佩寄厚仟壅傍����屈頁咀葎string議塰麻憲嶷墮峪屶隔忖憲才忖憲堪憧俊��stprintf辛參繍詞栽鯉塀議圷殆屁栽欺匯倖忖憲堪嶄補竃
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

	// 嗄老潤麻咄丼��弌科軌~��
	mciSendString(_T("open resources/sound/game_end/Perfect/Awake.mp3 alias Perfect_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Brilliant_Work!.mp3 alias Perfect_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Precious_Memories.mp3 alias Perfect_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/邦圈び寄挫きあひるちゃん.mp3 alias Perfect_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Theme_From_San_Andreas.mp3 alias Perfect_5"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Great/astral_ability.mp3 alias Great_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/云晩は這爺なり.mp3 alias Great_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/今に佩こうよ!.mp3 alias Great_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/�xきの鵬�g.mp3 alias Great_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/伉仇いい晩械.mp3 alias Great_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/��だまり丶��で.mp3 alias Great_6"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Good/Casual_Everyday.mp3 alias Good_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/Comfortable_Tiredness.mp3 alias Good_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/Fun_Fun_Friends.mp3 alias Good_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/寄����.mp3 alias Good_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/俟�､泙擦�.mp3 alias Good_5"), NULL, 0, NULL);
	
	mciSendString(_T("open resources/sound/game_end/Bad/Okamochi&Jersey.mp3 alias Bad_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/ずっこけ�羚�.mp3 alias Bad_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/ダブル�\�s雰ぼっち�､��Zりversion.mp3 alias Bad_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/は´��.mp3 alias Bad_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/ほえ.mp3 alias Bad_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/宛翌いけるかも!.mp3 alias Bad_6"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/���B皮釘.mp3 alias Bad_7"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/採かがおかしい.mp3 alias Bad_8"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/なぜに採絞��.mp3 alias Bad_9"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Worst/Can_You_Feel_It.mp3 alias Worst_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/Y.M.C.A.mp3 alias Worst_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/その晩秘った仟繁より聞えないダメバイトのエレジ�`.mp3 alias Worst_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/はて��.mp3 alias Worst_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/やってもた.mp3 alias Worst_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/送惜宀岻梧.mp3 alias Worst_6"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/壼芦寄畢爽.mp3 alias Worst_7"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/弌鞄.mp3 alias Worst_8"), NULL, 0, NULL);
}

// 嗄老忖悶圓譜崔
void set_font()
{
	lstrcpy(font_yahei.lfFaceName, _T("裏罷囘菜"));		
	font_yahei.lfHeight = 30;
	font_yahei.lfWidth = 0;
	font_yahei.lfQuality = PROOF_QUALITY;

	AddFontResourceEx(_T("resources/others/Aa隆栖菜65.ttf"), FR_PRIVATE, NULL); // Windows GDI 痕方��繍峺協猟周嶄議忖悶彿坿耶紗欺狼由��及屈倖歌方�浙┯致嵬綵�乎殻會辛聞喘��拝壓殻會潤崩朔徭強評茅��及眉倖歌方葎隠藻歌方��駅倬葎0
	lstrcpy(font_miku.lfFaceName, _T("Aa隆栖菜 65"));
	font_miku.lfHeight = 80;
	font_miku.lfWidth = 0;
	font_miku.lfQuality = PROOF_QUALITY;
}

int main()
{
	// 嗄老兜兵晒
	ExMessage msg;	// 梓囚延楚
	
	switch (MessageBox(GetHWnd(), _T("頁倦蝕尼debug庁塀��"), _T("兜兵岻咄"), MB_YESNO))
	{
		case IDYES:
			initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EX_SHOWCONSOLE);	// 幹秀揮陣崙岬議紙夕完笥��俺彌紙夕完笥鞘凹
			SetWindowText(GetHWnd(), _T("兜兵岻咄"));
			is_debug = true;
			switch (MessageBox(GetHWnd(), _T("頁倦紗墮畠何梧爆��"), _T("兜兵岻咄"), MB_YESNO))  // 卦指峙葎梓泥議峙��1葎YES��2葎NO
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
			initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);					// 幹秀音揮陣崙岬議紙夕完笥��俺彌紙夕完笥鞘凹
			SetWindowText(GetHWnd(), _T("兜兵岻咄"));
			break;
	};

	loadimage(&img_loading, _T("./resources/others/img_loading.png")); 
	putimage_alpha(0, 0, &img_loading);		// 喇噐畠何猟周脅均壓麼儉桟念紗墮��擬崑紗墮扮寂待裏渇�鵤�侭參紗倖loading順中

	load_songs(load_all_songs);							// 梧爆方式梧爆咄撞猟周紗墮
	load_game_resources(load_all_songs);				// 嗄老夕頭紗墮
	set_font();											// 忖悶紗墮
	
	opening_scene = new OpeningScene();					// 糞箭晒嗄老魁尚
	menu_scene = new MenuScene();
	setting_scene = new SettingScene();
	select_songs_scene = new SelectSongsScene();
	gaming_scene = new GamingScene();
	scene_manager.set_current_scene(opening_scene);

	setbkmode(TRANSPARENT);								// 嘘尚、忖悶譜崔(駅倬壓完笥秀羨朔圭辛譜崔)
	settextcolor(WHITE);
	settextstyle(&font_yahei);

	BeginBatchDraw();								// 褒産喝紙夕��受富描妨

	// 嗄老麼儉桟
	while (true)
	{
		auto frame_start_time = std::chrono::system_clock::now();

		while (peekmessage(&msg))					// 殊霞梓囚補秘
			scene_manager.on_input(msg,is_debug);

		// 仟糞�孱妻荒�c++11公嚠議坪崔垂<chrono>,辛參繍扮寂娼鳩欺裏昼封崛追昼雫艶。
		static auto last_tick_time = std::chrono::system_clock::now();
		auto current_tick_time = std::chrono::system_clock::now();
		auto delta_tick = std::chrono::duration_cast<std::chrono::milliseconds>(current_tick_time - last_tick_time).count();
		scene_manager.on_update(delta_tick, is_debug);
		last_tick_time = current_tick_time;

		cleardevice();
		scene_manager.on_draw(is_debug);					// 距喘魁尚砿尖匂嶄議紙夕圭隈
		FlushBatchDraw();				

		// 屐楕�渣篤�
		auto frame_end_time = std::chrono::system_clock::now();
		auto frame_delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end_time - frame_start_time).count();
		if (frame_delta_time < 1000 / FPS)
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS - frame_delta_time));
	}

	EndBatchDraw();

	return 0;
}