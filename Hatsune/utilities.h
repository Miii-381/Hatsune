#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <graphics.h>
#include <iostream>
#include <string.h>
#include <string>

#pragma comment(lib, "MSIMG32.LIB")   // AlphaBlend函数所需库
#pragma comment(lib, "Winmm.lib")      // MCI音频播放所需库

// 绘制带透明度的图片（支持阴影效果）
// x, y: 绘制位置坐标
// img: 要绘制的图片
// img_shadow: 阴影图片（可选，默认为nullptr）
// strength: 透明度（0-255，255为完全不透明）
inline void putimage_alpha(int x, int y, IMAGE* img, IMAGE* img_shadow = nullptr, BYTE strength = 255)
{
    // 如果提供了阴影图片，先绘制阴影
    if (img_shadow != nullptr)
	{
		int shadow_w = img_shadow->getwidth();
		int shadow_h = img_shadow->getheight();
		AlphaBlend(GetImageHDC(NULL), x + 3, y + 3, shadow_w, shadow_h,
			GetImageHDC(img_shadow), 0, 0, shadow_w, shadow_h, { AC_SRC_OVER, 0, strength, AC_SRC_ALPHA });
	}
	
	// 绘制主图片
	int img_w = img->getwidth();
	int img_h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, img_w, img_h,
		GetImageHDC(img), 0, 0, img_w, img_h, { AC_SRC_OVER, 0, strength, AC_SRC_ALPHA });
}

// 绘制带阴影效果的文本
// x, y: 文本位置坐标
// str: 要显示的文本字符串
// font_color: 文字颜色（默认白色）
// shadow_color: 阴影颜色（默认深灰色）
inline void outtextxy_shaded(int x, int y, LPCTSTR str, COLORREF font_color = RGB(255, 255, 255), COLORREF shadow_color = RGB(45, 45, 45))
{
	settextcolor(shadow_color);     // 设置阴影颜色
	outtextxy(x + 2, y + 2, str);   // 绘制偏移的阴影
	settextcolor(font_color);       // 设置文字颜色
	outtextxy(x, y, str);           // 绘制原文字
}

// 检测鼠标是否在指定矩形区域内
// region: 矩形区域
// x, y: 鼠标坐标
// 返回值：如果在区域内返回true，否则返回false
inline bool CheckMousePosition(RECT region, int x, int y)
{
	return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
}

// 循环播放歌曲
// current_song: 当前歌曲编号
inline void play_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("setaudio song_%d volume to 800"), current_song);  // 设置音量为800
	mciSendString(path_command, NULL, 0, NULL);
	memset(path_command, '\0', sizeof(path_command));
	
	_stprintf_s(path_command, _T("play song_%d repeat from 0"), current_song);  // 循环播放
	mciSendString(path_command, NULL, 0, NULL);
}

// 单次播放歌曲（不循环）
// current_song: 当前歌曲编号
inline void play_song_no_repeat(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("play song_%d"), current_song);  // 播放一次
	mciSendString(path_command, NULL, 0, NULL);
	memset(path_command, '\0', sizeof(path_command));
	
	_stprintf_s(path_command, _T("setaudio song_%d volume to 800"), current_song);  // 设置音量
	mciSendString(path_command, NULL, 0, NULL);
}

// 停止播放歌曲并重置到开头
// current_song: 当前歌曲编号
inline void stop_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("stop song_%d"), current_song);  // 停止播放
	mciSendString(path_command, NULL, 0, NULL);
	
	_stprintf_s(path_command, _T("seek song_%d to 0"), current_song);  // 定位到开头
	mciSendString(path_command, NULL, 0, NULL);
}

// 暂停播放歌曲
// current_song: 当前歌曲编号
inline void pause_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("pause song_%d"), current_song);  // 暂停
	mciSendString(path_command, NULL, 0, NULL);
}

// 恢复播放歌曲
// current_song: 当前歌曲编号
inline void resume_song(int current_song)
{
	TCHAR path_command[256];
	_stprintf_s(path_command, _T("resume song_%d"), current_song);  // 恢复播放
	mciSendString(path_command, NULL, 0, NULL);
}

// 将毫秒时间转换为时分秒格式
// milliseconds: 毫秒数
// s: 输出参数-秒数
// m: 输出参数-分钟数
// h: 输出参数-小时数
void msToTime(long long milliseconds, int& s, int& m, int& h)
{
	int seconds = milliseconds / 1000;
	h = seconds / 3600;              // 计算小时
	m = (seconds % 3600) / 60;       // 计算分钟
	s = seconds % 60;                // 计算秒数
}

#endif // !_UTILITIES_H_
