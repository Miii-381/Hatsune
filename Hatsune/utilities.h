#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <graphics.h>
#include <iostream>
#include <string.h>
#include <string>

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "Winmm.lib")

// ����Ӱ��ͼƬ����,�����͸����pngͼƬ�ĺ���,ʹ�����������ڱ���ʱ�����ô��ú������ں���ֱ���滻����ʡ����ʱ�䣬ͨ������strength���Կ������ͼƬ��͸����
inline void putimage_alpha(int x, int y, IMAGE* img, IMAGE* img_shadow = nullptr, BYTE strength = 255)
{
    if (img_shadow != nullptr)
	{
		int shadow_w = img_shadow->getwidth();
		int shadow_h = img_shadow->getheight();
		AlphaBlend(GetImageHDC(NULL), x + 3, y + 3, shadow_w, shadow_h,
			GetImageHDC(img_shadow), 0, 0, shadow_w, shadow_h, { AC_SRC_OVER, 0, strength, AC_SRC_ALPHA });
	}
	int img_w = img->getwidth();
	int img_h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, img_w, img_h,
		GetImageHDC(img), 0, 0, img_w, img_h, { AC_SRC_OVER, 0, strength, AC_SRC_ALPHA });
}

// �ı��������Ӱ��ɫ
inline void outtextxy_shaded(int x, int y, LPCTSTR str, COLORREF font_color = RGB(255, 255, 255), COLORREF shadow_color = RGB(45, 45, 45))
{
	settextcolor(shadow_color);
	outtextxy(x + 2, y + 2, str);
	settextcolor(font_color);
	outtextxy(x, y, str);
}

// �������Ƿ��ڰ�����Χ�ڣ����޾��Σ������̫�鷳�ˣ�����...��
inline bool CheckMousePosition(RECT region, int x, int y)
{
	return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
}

// ��ʼ����
inline void play_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("setautio song_%d volume to 800"), current_song);	// Ҳû������...
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
	memset(path_command, '\0', sizeof(path_command));
	_stprintf_s(path_command, _T("play song_%d repeat from 0"), current_song);
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
}

inline void play_song_no_repeat(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("play song_%d"), current_song);
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
	memset(path_command, '\0', sizeof(path_command));
	_stprintf_s(path_command, _T("setautio song_%d volume to 800"), current_song);	// Ҳû������...
	mciSendString(path_command, NULL, 0, NULL);
}

// ֹͣ���ţ���֪��Ϊʲô�����ҵĳ����У�mciֹͣ���Ų����Ὣ����λ���ƻ���ʼ�㣬��Ҫ�ֶ�����...��
inline void stop_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("stop song_%d"), current_song);
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
	_stprintf_s(path_command, _T("seek song_%d to 0"), current_song);
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
}

// ��ͣ����
inline void pause_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("pause song_%d"), current_song);
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
}

// �ָ�����
inline void resume_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("resume song_%d"), current_song);
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
}

// ����ʱ����ʾת��
void msToTime(long long milliseconds, int& s, int& m, int& h)
{
	int seconds = milliseconds / 1000;
	h = seconds / 3600;
	m = (seconds % 3600) / 60;
	s = seconds % 60;
}

#endif // !_UTILITIES_H_

