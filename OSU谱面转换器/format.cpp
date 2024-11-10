#include <iostream>
#include <fstream>
#include <string>

int main()
{
	std::cout << "                              OSU��Mania����ת����\n" << std::endl;
	std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
	std::cout << "ʹ�÷�����ֱ��������Ҫת���������ļ��ĵ�ַ���ɣ������ļ����ڵ�Ŀ¼������ת���õġ��ļ���.txt���ĵ�\n" << std::endl;
	std::cout << "СҪ�󣺼���.txt��.osu�ļ�������ԭ�����ļ��������пո�����δʹ��OSU����༭���޸�ԭ���棬�����ԭ�����ʽ�Ķ������ܻ�Ӱ��ת��Ч��" << std::endl;

	while (1)
	{
		std::string location;
		std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\" << std::endl;
		std::cout << "\n���뵱ǰ�ļ���ַ��" << std::endl;
		std::cin >> location;
		
		// ������
		std::ifstream file_origin(location);
		if (!file_origin.is_open()) {
			std::cerr << "Դtxt�ļ��򲻿���~����ļ�·�����ļ����Ƿ���ڿո���������ţ����ļ���ʽ�Ƿ����" << std::endl;
			std::cerr << "����������˳�����" << std::endl;
			system("pause");
			break;
		}

		// �����ļ���(������б�ܵ��ж�)
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
		location = location.substr(last + 1, location.size()); // ���ﲻ+1�Ļ��ض��Ǵ�б�ܿ�ʼ�ģ����б�ܰ���ȥ...
		
		// �������ļ���ŵ�ַ���ַ���
		size_t temp_pos = location.find(".txt");
		if(temp_pos == std::string::npos)
			temp_pos = location.find(".osu");
		std::string temp = location.substr(0, temp_pos);
		temp = ".\\output\\" + temp + ".txt";

		// ��������ȷ�ԣ��½����ļ�
		std::cout << location << std::endl;
		std::cout << temp << std::endl;
		std::ofstream file(temp);

		std::cout << "�Ѷ�ȡ����ʼ���\n" << std::endl;

		// ��ʼ���������ļ�
		std::string line;
		std::string target = "[HitObjects]";
		while (getline(file_origin, line))	// һ��һ����ȡԭ�ļ��е��ַ�����ֱ����Ŀ���ַ���ƥ��
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
			
			temp = line.substr(start, length);	// ��ȡ��Ч��ϸ������֮ǰ��note����Ĳ���

			for(int i = 0; i < 6; i++)			// �ָ��ַ����������ַ���ת��Ϊ��������
			{
				end = temp.find(L',', start);
				length = end - start;
				token = temp.substr(start, length);
				start = end + 1;
				temp_num[i] = atoi(token.c_str());
			}

			// ��ʼ�����Լ������еĶ�Ӧ����
			// if ((temp_num[0] == 64 || temp_num[0] == 0)&& (temp_num[1] == 192 || temp_num[1] == 384))	 ��������
			if ((temp_num[0] == 64 || temp_num[0] == 0))				// ����λ��(DFJK�ֱ��Ӧ1234)
				pos = 1;
			else if ((temp_num[0] == 192 || temp_num[0] == 128))
				pos = 2;
			else if ((temp_num[0] >= 256 && temp_num[0] <= 330))
				pos = 3;
			else if ((temp_num[0] >= 384 && temp_num[0] <= 450))
				pos = 4;

			start_time = temp_num[2];	// ������ʼʱ��

			if (temp_num[3] == 128)		// �������ࣨclick == 1, hold == 2��
				type = 2;
			else if(temp_num[3] == 5 || temp_num[3] == 1)
				type = 1;
			
			if (temp_num[5] > 200)		// hold������ֹʱ��
				end_time = temp_num[5];

			// ��ʽ���������
			file << type << ',' << pos << ',' << start_time << ',' << end_time << std::endl;
		}

		file_origin.close();
		file.close();

		std::cout << "\n�����ϣ��ļ����Ŀ¼Ϊ��" << temp << std::endl;
		std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n\n\n" << std::endl;
	}
	return 0;
}
