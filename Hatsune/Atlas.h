#ifndef _ATLAS_H_
#define _ATLAS_H_

#include <vector>
#include <graphics.h>

extern bool is_debug;

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	enum img_scale
	{
		scale_1_1, scale_4_3, scale_16_9
	};

	// 从给定路径模板加载图片资源，自动判断图片是1:1、4:3或16:9并加载为对应尺寸
	void load_resources(LPCTSTR path_template, LPCTSTR imgType, int num, int x, int y)
	{
		img_list.clear();
		img_list.resize(num);

		for (int i = 0; i < num; i++)
		{
			IMAGE temp_img;
			TCHAR path_file[256];
			_stprintf_s(path_file, path_template, i + 1, i + 1);
			loadimage(&temp_img, path_file);
			if (temp_img.getheight() == temp_img.getwidth()) // 判断是否为1:1图片
			{
				scale.push_back(img_scale::scale_1_1);
				loadimage(&img_list[i], path_file, 384, 384);
				if (is_debug)
					std::cout << "load song_" << i + 1 << " " << imgType << ".png complete" << std::endl;
			}
			else if ((double)temp_img.getwidth() / temp_img.getheight() >= 1.32 && (double)temp_img.getwidth() / temp_img.getheight() <= 1.34) // 判断是否为4:3图片
			{
				scale.push_back(img_scale::scale_4_3);
				loadimage(&img_list[i], path_file, 464, 348);
				if (is_debug)
					std::cout << "load song_" << i + 1 << " " << imgType << ".png complete" << std::endl;
			}
			else if ((double)temp_img.getwidth() / temp_img.getheight() >= 1.76 && (double)temp_img.getwidth() / temp_img.getheight() <= 1.78) // 判断是否为16:9图片
			{
				scale.push_back(img_scale::scale_16_9);
				loadimage(&img_list[i], path_file, x, y);
				if (is_debug)
					std::cout << "load song_" << i + 1 << " " << imgType << ".png complete" << std::endl;
			}
			else
			{
				memset(path_file, '\0', sizeof(path_file));
				if (imgType == _T("bg"))
					_stprintf_s(path_file, _T("错误！检查song_%d的对应资源图片（bg.png）是否符合beatmap文件夹中的readme文件内规定的特定尺寸！"), i + 1);
				else if (imgType == _T("cover"))
					_stprintf_s(path_file, _T("错误！检查song_%d的对应资源图片（cover.png）是否符合beatmap文件夹中的readme文件内规定的特定尺寸！"), i + 1);
				MessageBox(GetHWnd(), path_file, _T("初始之音"), MB_OK);
				SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			}
		}
	}

	// 获取图片组元素个数
	int get_size()
	{
		return (int)img_list.size();
	}

	// 获取图片组中的第idx张图片（下标从1开始）
	IMAGE* get_image(int idx)
	{
		if (idx <= 0 || idx > img_list.size())
			return nullptr;

		return &img_list[idx - 1];
	}

	// 获取图片的缩放比例
	img_scale get_scale(int ID)
	{
		return scale[ID - 1];
	}

private:
	std::vector<IMAGE> img_list; // 使用vector存储图片列表，动态扩展大小
	std::vector<img_scale> scale;
};

#endif //!_ATLAS_H_
