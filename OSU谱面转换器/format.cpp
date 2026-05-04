#include <iostream>
#include <fstream>
#include <string>

int main()
{
	std::cout << "                              OSU！Mania谱面转换器\n" << std::endl;
	std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
	std::cout << "使用方法：直接输入需要转换的谱面文件的地址即可，会在文件所在的目录下生成转换好的“文件名.txt”文档\n" << std::endl;
	std::cout << "小要求：兼容.txt和.osu文件，但是原谱面文件名不能有空格，且若未使用OSU谱面编辑器修改原谱面，而造成原谱面格式改动，可能会影响转换效果" << std::endl;

	while (1)
	{
		std::string location;
		std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
		std::cout << "\n输入当前文件地址：" << std::endl;
		std::cin >> location;
		
		// 错误检测
		std::ifstream file_origin(location);
		if (!file_origin.is_open()) {
			std::cerr << "源txt文件打不开捏~检查文件路径和文件名是否存在空格等其他符号，或文件格式是否错误" << std::endl;
			std::cerr << "按任意键以退出程序" << std::endl;
			system("pause");
			break;
		}

		// 剥离文件名(有正反斜杠的判断)
		bool is_backslash = false;
		size_t now = location.find('/', 1);
		if (now == std::string::npos)
		{
			now = location.find('\\', 1);
			is_backslash = true;
		}
		size_t last = now;
		while (now != std::string::npos)
		{
			last = now;
			if(is_backslash)
				now = location.find('\\', now + 1);
			else
				now = location.find('/', now + 1);
		}
		location = location.substr(last + 1, location.size()); // 这里不+1的话截断是从斜杠开始的，会把斜杠包进去...
		
		// 设置新文件存放地址的字符串
		size_t temp_pos = location.find(".txt");
		if(temp_pos == std::string::npos)
			temp_pos = location.find(".osu");
		std::string temp = location.substr(0, temp_pos);
		temp = ".\\output\\" + temp + ".txt";

		// 检查操作正确性，新建新文件
		std::cout << location << std::endl;
		std::cout << temp << std::endl;
		std::ofstream file(temp);

		std::cout << "已读取，开始输出\n" << std::endl;

		// 开始处理谱面文件
		std::string line;
		std::string target = "[HitObjects]";
		while (getline(file_origin, line))	// 一行一行提取原文件中的字符串，直到与目标字符串匹配
		{			
			if (line == target)
				break;
		}

		while (1)
		{
			int temp_num[6] = { };
			int pos = 0, start_time = 0, end_time = 0, type = 0;
			if (!getline(file_origin, line))
				break;

			std::string temp;
			std::string token;
			size_t start = 0;
			size_t end = line.find(L':');
			size_t length = end - start;
			
			temp = line.substr(start, length);	// 提取音效精细化参数之前的note本身的参数

			for(int i = 0; i < 6; i++)			// 分割字符串，并将字符串转化为整型数字
			{
				end = temp.find(L',', start);
				length = end - start;
				token = temp.substr(start, length);
				start = end + 1;
				temp_num[i] = atoi(token.c_str());
			}

			// 开始生成自己程序中的对应参数
			// if ((temp_num[0] == 64 || temp_num[0] == 0)&& (temp_num[1] == 192 || temp_num[1] == 384))	 留个备份
			if ((temp_num[0] == 64 || temp_num[0] == 0))				// 音符位置(DFJK分别对应1234)
				pos = 1;
			else if ((temp_num[0] == 192 || temp_num[0] == 128))
				pos = 2;
			else if ((temp_num[0] >= 256 && temp_num[0] <= 330))
				pos = 3;
			else if ((temp_num[0] >= 384 && temp_num[0] <= 450))
				pos = 4;

			start_time = temp_num[2];	// 音符开始时间

			if (temp_num[3] == 128)		// 音符种类（click == 1, hold == 2）
				type = 2;
			else if(temp_num[3] == 5 || temp_num[3] == 1)
				type = 1;
			
			if (temp_num[5] > 200)		// hold音符截止时间
				end_time = temp_num[5];

			// 格式化输出数据
			file << type << ',' << pos << ',' << start_time << ',' << end_time << std::endl;
		}

		file_origin.close();
		file.close();

		std::cout << "\n输出完毕，文件输出目录为：" << temp << std::endl;
		std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n" << std::endl;
	}
	return 0;
}
