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
//  ��Ϸ��ܱ�Ҫ����   //
///////////////////////

const int WINDOW_WIDTH = 1280;						// ���ڴ�С
const int WINDOW_HEIGHT = 720;
const int FPS = 144;								// FPS����
int SONGS_NUM = 0;									// ������
bool is_debug = false;								// Debugģʽ
bool load_all_songs = true;							// �Ƿ����ȫ������
int drop_speed = 50;								// �����ٶȣ�Խ��Խ��,100�ⶥ��
int offset = 0;										// ����ƫ����

SceneManager scene_manager;							// ����������
Scene* opening_scene = nullptr;						// ��Ϸ����
Scene* menu_scene = nullptr;
Scene* select_songs_scene = nullptr;
Scene* gaming_scene = nullptr;
Scene* setting_scene = nullptr;

// ResourceManager ResourceMgr;

///////////////
// ͼƬ���   //
///////////////

// ��Ϸ����
IMAGE img_title;

// �������汳��
IMAGE img_loading;
IMAGE img_background_opening_scene;					
IMAGE img_background_menu_scene;
IMAGE img_background_setting_scene;
IMAGE img_background_select_songs_scene;
								
// ���˵�����
IMAGE img_start_button_idle;						
IMAGE img_start_button_hovered;
IMAGE img_start_button_clicked;
IMAGE img_setting_button_idle;
IMAGE img_setting_button_hovered;
IMAGE img_setting_button_clicked;
IMAGE img_about_button_idle;
IMAGE img_about_button_hovered;
IMAGE img_about_button_clicked;

// ����ѡ�����
IMAGE img_current_song_button_idle;				// ��ǰ�������棨������ť����
IMAGE img_icon_setting_button_idle;				// ���ð���
IMAGE img_icon_setting_button_hovered;
IMAGE img_icon_setting_button_clicked;
IMAGE img_left_arrow_button_idle;				// ���󰴼�
IMAGE img_left_arrow_button_hovered;
IMAGE img_left_arrow_button_clicked;
IMAGE img_right_arrow_button_idle;				// ���Ұ���
IMAGE img_right_arrow_button_hovered;
IMAGE img_right_arrow_button_clicked;
IMAGE img_icon_pause_button_idle;				// ��ͣ����
IMAGE img_icon_pause_button_hovered;
IMAGE img_icon_pause_button_clicked;

// ��Ϸ����
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
IMAGE img_continue_button_idle;			// ������Ϸ����
IMAGE img_continue_button_hovered;
IMAGE img_continue_button_clicked;
IMAGE img_end_button_idle;				// ������Ϸ����
IMAGE img_end_button_hovered;
IMAGE img_end_button_clicked;

// ���ý���
IMAGE img_setting_masking;
IMAGE img_left_arrow;
IMAGE img_right_arrow;
IMAGE img_exit_button_idle;
IMAGE img_exit_button_hovered;
IMAGE img_exit_button_clicked;
IMAGE img_box_masking;

// �����ʽ
LOGFONT font_yahei;
LOGFONT font_miku;

// ��ϷͼƬ��Դ����
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
	songs_cover.load_resources(_T("./resources/beatmaps/song_%d/cover.png"),_T("cover"), SONGS_NUM, 512, 288);		// ע�⣺��������͸�������ͳһΪPNG��ʽ
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

// ������Ƶ�ļ����أ����ļ��ж�ȡ�������ƣ��޸ĸ���ʱ������Դ�������޸ģ�
void load_songs(bool load_all_songs)
{
	std::ifstream file("./resources/beatmaps/songs_name.txt",std::ifstream::in);
	if (file.is_open() == false)
	{
		perror("fopen error!");
		MessageBox(GetHWnd(), _T("��������ȡ���������������������\n���������ļ�λ�ڣ�.\\resources\\beatmaps\\songs_name.txt"), _T("ERROR!"), MB_OK | MB_ICONERROR);
		SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
	}

	TCHAR path_command[512];
	std::string temp_song_name;
	
	// ��ֵ����⣺�޸Ĵ����mciSendString��Ҫ��һ���ļ������޶����ܲ�������...ִ��һ�δ���֮���ֲ���Ҫ��...��
	// ʹ��stprintfһ��Ϊ�ַ������ݣ��Ժ���ʱ����д������˵����������Ϊstring�����������ֻ֧���ַ����ַ���ƴ�ӣ�stprintf���Խ���ϸ�ʽ��Ԫ�����ϵ�һ���ַ��������
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

	// ��Ϸ������Ч��С�ʵ�~��
	mciSendString(_T("open resources/sound/game_end/Perfect/Awake.mp3 alias Perfect_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Brilliant_Work!.mp3 alias Perfect_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Precious_Memories.mp3 alias Perfect_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/ˮԡ�Ӵ�ä����Ҥ�����.mp3 alias Perfect_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Perfect/Theme_From_San_Andreas.mp3 alias Perfect_5"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Great/astral_ability.mp3 alias Great_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/���դ�����ʤ�.mp3 alias Great_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/�����Ф�����!.mp3 alias Great_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/�x����˲�g.mp3 alias Great_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/�ĵؤ����ճ�.mp3 alias Great_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Great/ꖤ��ޤ�Ц�.mp3 alias Great_6"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Good/Casual_Everyday.mp3 alias Good_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/Comfortable_Tiredness.mp3 alias Good_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/Fun_Fun_Friends.mp3 alias Good_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/����.mp3 alias Good_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/�؏��ޤ���.mp3 alias Good_5"), NULL, 0, NULL);
	
	mciSendString(_T("open resources/sound/game_end/Bad/Okamochi&Jersey.mp3 alias Bad_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/���ä����湝.mp3 alias Bad_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/���֥��\�sʷ�ܤä������Z��version.mp3 alias Bad_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/�ϡ���.mp3 alias Bad_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/�ۤ�.mp3 alias Bad_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/���⤤���뤫��!.mp3 alias Bad_6"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/��B��ţ.mp3 alias Bad_7"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Bad/�Τ�����������.mp3 alias Bad_8"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Good/�ʤ��˺ιʣ�.mp3 alias Bad_9"), NULL, 0, NULL);

	mciSendString(_T("open resources/sound/game_end/Worst/Can_You_Feel_It.mp3 alias Worst_1"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/Y.M.C.A.mp3 alias Worst_2"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/��������ä����ˤ��ʹ���ʤ�����Х��ȤΥ��쥸�`.mp3 alias Worst_3"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/�Ϥƣ�.mp3 alias Worst_4"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/��äƤ⤿.mp3 alias Worst_5"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/������֮��.mp3 alias Worst_6"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/�簲��ɭ��.mp3 alias Worst_7"), NULL, 0, NULL);
	mciSendString(_T("open resources/sound/game_end/Worst/С��.mp3 alias Worst_8"), NULL, 0, NULL);
}

// ��Ϸ����Ԥ����
void set_font()
{
	lstrcpy(font_yahei.lfFaceName, _T("΢���ź�"));		
	font_yahei.lfHeight = 30;
	font_yahei.lfWidth = 0;
	font_yahei.lfQuality = PROOF_QUALITY;

	AddFontResourceEx(_T("resources/others/Aaδ����65.ttf"), FR_PRIVATE, NULL); // Windows GDI ��������ָ���ļ��е�������Դ��ӵ�ϵͳ���ڶ��������޶���������ó����ʹ�ã����ڳ���������Զ�ɾ��������������Ϊ��������������Ϊ0
	lstrcpy(font_miku.lfFaceName, _T("Aaδ���� 65"));
	font_miku.lfHeight = 80;
	font_miku.lfWidth = 0;
	font_miku.lfQuality = PROOF_QUALITY;
}

int main()
{
	// ��Ϸ��ʼ��
	ExMessage msg;	// ��������
	
	switch (MessageBox(GetHWnd(), _T("�Ƿ���debugģʽ��"), _T("��ʼ֮��"), MB_YESNO))
	{
		case IDYES:
			initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EX_SHOWCONSOLE);	// ������ͼ���ڣ���׽��ͼ���ھ��
			SetWindowText(GetHWnd(), _T("��ʼ֮��"));
			is_debug = true;
			switch (MessageBox(GetHWnd(), _T("�Ƿ����ȫ��������"), _T("��ʼ֮��"), MB_YESNO))
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
			initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);					// ������ͼ���ڣ���׽��ͼ���ھ��
			SetWindowText(GetHWnd(), _T("��ʼ֮��"));
			break;
	};

	loadimage(&img_loading, _T("./resources/others/img_loading.png")); 
	putimage_alpha(0, 0, &img_loading);					// ����ȫ���ļ���������ѭ��ǰ���أ����¼���ʱ����΢�������ԼӸ�loading����

	load_songs(load_all_songs);							// ��������������Ƶ�ļ�����
	load_game_resources(load_all_songs);				// ��ϷͼƬ����
	set_font();											// �������
	
	opening_scene = new OpeningScene();					// ʵ������Ϸ����
	menu_scene = new MenuScene();
	setting_scene = new SettingScene();
	select_songs_scene = new SelectSongsScene();
	gaming_scene = new GamingScene();
	scene_manager.set_current_scene(opening_scene);

	setbkmode(TRANSPARENT);								// ��������������(�����ڴ��ڽ����󷽿�����)
	settextcolor(WHITE);
	settextstyle(&font_yahei);

	BeginBatchDraw();								// ˫�����ͼ��������˸

	// ��Ϸ��ѭ��
	while (true)
	{
		auto frame_start_time = std::chrono::system_clock::now();

		while (peekmessage(&msg))					// ��ⰴ������
			scene_manager.on_input(msg,is_debug);

		// ��ʵ�֣�ʹ��c++11��������ÿ�<chrono>,���Խ�ʱ�侫ȷ��΢���������뼶��
		static auto last_tick_time = std::chrono::system_clock::now();
		auto current_tick_time = std::chrono::system_clock::now();
		auto delta_tick = std::chrono::duration_cast<std::chrono::milliseconds>(current_tick_time - last_tick_time).count();
		scene_manager.on_update(delta_tick, is_debug);
		last_tick_time = current_tick_time;

		cleardevice();
		scene_manager.on_draw(is_debug);					// ���ó����������еĻ�ͼ����
		FlushBatchDraw();				

		// ֡��������
		auto frame_end_time = std::chrono::system_clock::now();
		auto frame_delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end_time - frame_start_time).count();
		if (frame_delta_time < 1000 / FPS)
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS - frame_delta_time));
	}

	EndBatchDraw();

	return 0;
}