#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <graphics.h>
#include <iostream>
#include <string.h>
#include <string>

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "Winmm.lib")

//输入带透明度png图片的函数,使用内联函数在编译时将调用处用函数体内函数直接替换，节省运行时间
inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

// 通过调整strength可以控制输出图片的透明度
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

// 重封装的生成带阴影文字的函数
inline void outtextxy_shaded(int x, int y, LPCTSTR str)
{
	settextcolor(RGB(45, 45, 45));
	outtextxy(x + 2, y + 2, str);
	settextcolor(RGB(255, 255, 255));
	outtextxy(x, y, str);
}
// 改变字体颜色
inline void outtextxy_shaded(int x, int y, LPCTSTR str, COLORREF font_color)
{
	settextcolor(RGB(45, 45, 45));
	outtextxy(x + 2, y + 2, str);
	settextcolor(font_color);
	outtextxy(x, y, str);
}

// 改变字体和阴影颜色
inline void outtextxy_shaded(int x, int y, LPCTSTR str, COLORREF font_color, COLORREF shadow_color)
{
	settextcolor(shadow_color);
	outtextxy(x + 2, y + 2, str);
	settextcolor(font_color);
	outtextxy(x, y, str);
}

// 检查鼠标是否在按键范围内（仅限矩形，多边形不会...）
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

// 重载播放函数，添加进度选择（好像没用...）
inline void play_song(int current_song, int pos)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("setautio song_%d volume to 800"), current_song);  // 也没起作用...
	printf("%s\n", path_command);
	mciSendString(path_command, NULL, 0, NULL);
	memset(path_command, '\0', sizeof(path_command));
	_stprintf_s(path_command, _T("play song_%d from %d repeat"), current_song, pos);
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

