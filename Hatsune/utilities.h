#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <graphics.h>
#include <iostream>
#include <string.h>
#include <string>

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "Winmm.lib")

//�����͸����pngͼƬ�ĺ���,ʹ�����������ڱ���ʱ�����ô��ú������ں���ֱ���滻����ʡ����ʱ��
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

// ͨ������strength���Կ������ͼƬ��͸����
inline void putimage_alpha(int x, int y, IMAGE* img, BYTE strength)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,strength,AC_SRC_ALPHA });
}

inline void putimage_alpha_shadow(int x, int y, IMAGE* img,IMAGE* img_shadow, BYTE strength)
{
	int w = img->getwidth();
	int h = img->getheight();
	if(img_shadow != nullptr)
		putimage_alpha(x + 3, y + 3, img_shadow, strength);
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,strength,AC_SRC_ALPHA });
}

// �ط�װ�����ɴ���Ӱ���ֵĺ���
inline void outtextxy_shaded(int x, int y, LPCTSTR str)
{
	settextcolor(RGB(45, 45, 45));
	outtextxy(x + 2, y + 2, str);
	settextcolor(RGB(255, 255, 255));
	outtextxy(x, y, str);
}
// �ı�������ɫ
inline void outtextxy_shaded(int x, int y, LPCTSTR str, COLORREF font_color)
{
	settextcolor(RGB(45, 45, 45));
	outtextxy(x + 2, y + 2, str);
	settextcolor(font_color);
	outtextxy(x, y, str);
}

// �ı��������Ӱ��ɫ
inline void outtextxy_shaded(int x, int y, LPCTSTR str, COLORREF font_color, COLORREF shadow_color)
{
	settextcolor(shadow_color);
	outtextxy(x + 2, y + 2, str);
	settextcolor(font_color);
	outtextxy(x, y, str);
}

// �������Ƿ��ڰ�����Χ�ڣ����޾��Σ�����β���...��
inline bool CheckMousePosition(RECT region, int x, int y)
{
	return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
}

// ��ʼ����
inline void play_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("setautio song_%d volume to 80"), current_song);
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
	_stprintf_s(path_command, _T("setautio song_%d volume to 80"), current_song);
	mciSendString(path_command, NULL, 0, NULL);
}

// ֹͣ����
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

#endif // !_UTILITIES_H_

