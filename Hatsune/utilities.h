#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <graphics.h>
#include <iostream>
#include <string.h>
#include <string>

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "Winmm.lib")

// 带阴影的图片绘制,输入带透明度png图片的函数,使用内联函数在编译时将调用处用函数体内函数直接替换，节省运行时间，通过调整strength可以控制输出图片的透明度
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

// 改变字体和阴影颜色
inline void outtextxy_shaded(int x, int y, LPCTSTR str, COLORREF font_color = RGB(255, 255, 255), COLORREF shadow_color = RGB(45, 45, 45))
{
	settextcolor(shadow_color);
	outtextxy(x + 2, y + 2, str);
	settextcolor(font_color);
	outtextxy(x, y, str);
}

// 检查鼠标是否在按键范围内（仅限矩形，多边形太麻烦了，不会...）
inline bool CheckMousePosition(RECT region, int x, int y)
{
	return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
}

// 开始播放
inline void play_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("setautio song_%d volume to 800"), current_song);	// 也没起作用...
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
	_stprintf_s(path_command, _T("setautio song_%d volume to 800"), current_song);	// 也没起作用...
	mciSendString(path_command, NULL, 0, NULL);
}

// 停止播放（不知道为什么，在我的程序中，mci停止播放并不会将播放位置移回起始点，需要手动设置...）
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

// 暂停播放
inline void pause_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("pause song_%d"), current_song);
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
}

// 恢复播放
inline void resume_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("resume song_%d"), current_song);
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
}

// 音乐时间显示转换
void msToTime(long long milliseconds, int& s, int& m, int& h)
{
	int seconds = milliseconds / 1000;
	h = seconds / 3600;
	m = (seconds % 3600) / 60;
	s = seconds % 60;
}

#endif // !_UTILITIES_H_

