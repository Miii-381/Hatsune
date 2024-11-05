#ifndef _ATLAS_H_
#define _ATLAS_H_

#include <vector>

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

	// 从赋予的路径中加载图片资源(注意，加载路径是两个参数！例如："song_%d/%d_bg.jpg")
	// 兼容1：1图片，16：9图片及4：3图片加载
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
			if (temp_img.getheight() == temp_img.getwidth()) // 判断是否近似为1：1图片
			{
				scale.push_back(img_scale::scale_1_1);
				loadimage(&img_list[i], path_file, 384, 384);
				if (is_debug)
					std::cout << "load song_" << i + 1 << " " << imgType << ".png complete" << std::endl;
			}
			else if((double)temp_img.getwidth() / temp_img.getheight() >= 1.32 && (double)temp_img.getwidth() / temp_img.getheight() <= 1.34)		// 判断是否近似为4：3图片
			{
				scale.push_back(img_scale::scale_4_3);
				loadimage(&img_list[i], path_file, 464, 348);
				if (is_debug)
					std::cout << "load song_" << i + 1 << " " << imgType << ".png complete" << std::endl;
			}
			else if((double)temp_img.getwidth() / temp_img.getheight() >= 1.76 && (double)temp_img.getwidth() / temp_img.getheight() <= 1.78)		// 判断是否近似为16：9图片
			{
				scale.push_back(img_scale::scale_16_9);
				loadimage(&img_list[i], path_file, x, y);
				if (is_debug)
					std::cout << "load song_" << i + 1 << " " << imgType << ".png complete" << std::endl;
			}
			else
			{
				memset(path_file, '\0', sizeof(path_file));
				if(imgType == "bg")
					_stprintf_s(path_file, _T("检查song_%d的歌曲背景图片（bg.png）是否为beatmap文件夹中的readme文件列出的特定尺寸！"), i + 1);
				else if(imgType == "cover")
					_stprintf_s(path_file, _T("检查song_%d的歌曲封面图片（cover.png）是否为beatmap文件夹中的readme文件列出的特定尺寸！"), i + 1);
				MessageBox(GetHWnd(), path_file, _T("初始之音"), MB_OK);
				SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			}
		}
	}

	//获取图集中元素的个数
	int get_size()
	{
		return (int)img_list.size();
	}

	// 获取存放在图集中的图片
	IMAGE* get_image(int idx)
	{
		if (idx <= 0 || idx > img_list.size())
			return nullptr;

		return &img_list[idx - 1];
	}

	img_scale get_scale(int ID)
	{
		return scale[ID - 1];
	}

private:
	std::vector<IMAGE> img_list;	//使用vector容器存放图集，动态赋予数组大小
	std::vector<img_scale> scale;
};


#endif //!_ATLAS_H_

