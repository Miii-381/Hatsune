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

	// �Ӹ����·���м���ͼƬ��Դ(ע�⣬����·�����������������磺"song_%d/%d_bg.jpg")
	// ����1��1ͼƬ��16��9ͼƬ��4��3ͼƬ����
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
			if (temp_img.getheight() == temp_img.getwidth()) // �ж��Ƿ����Ϊ1��1ͼƬ
			{
				scale.push_back(img_scale::scale_1_1);
				loadimage(&img_list[i], path_file, 384, 384);
				if (is_debug)
					std::cout << "load song_" << i + 1 << " " << imgType << ".png complete" << std::endl;
			}
			else if((double)temp_img.getwidth() / temp_img.getheight() >= 1.32 && (double)temp_img.getwidth() / temp_img.getheight() <= 1.34)		// �ж��Ƿ����Ϊ4��3ͼƬ
			{
				scale.push_back(img_scale::scale_4_3);
				loadimage(&img_list[i], path_file, 464, 348);
				if (is_debug)
					std::cout << "load song_" << i + 1 << " " << imgType << ".png complete" << std::endl;
			}
			else if((double)temp_img.getwidth() / temp_img.getheight() >= 1.76 && (double)temp_img.getwidth() / temp_img.getheight() <= 1.78)		// �ж��Ƿ����Ϊ16��9ͼƬ
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
					_stprintf_s(path_file, _T("���song_%d�ĸ�������ͼƬ��bg.png���Ƿ�Ϊbeatmap�ļ����е�readme�ļ��г����ض��ߴ磡"), i + 1);
				else if(imgType == "cover")
					_stprintf_s(path_file, _T("���song_%d�ĸ�������ͼƬ��cover.png���Ƿ�Ϊbeatmap�ļ����е�readme�ļ��г����ض��ߴ磡"), i + 1);
				MessageBox(GetHWnd(), path_file, _T("��ʼ֮��"), MB_OK);
				SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
			}
		}
	}

	//��ȡͼ����Ԫ�صĸ���
	int get_size()
	{
		return (int)img_list.size();
	}

	// ��ȡ�����ͼ���е�ͼƬ
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
	std::vector<IMAGE> img_list;	//ʹ��vector�������ͼ������̬���������С
	std::vector<img_scale> scale;
};


#endif //!_ATLAS_H_

