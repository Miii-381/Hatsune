#pragma once
// �˴���ŵ��Ǳ�д��������в����ķϰ�������֤���ã������������ǶԵġ�

// �����б��ļ���ȡ��������(select_songs_scene.h)
		/*_wfopen_s(&song_list_ptr, _T("./resources/beatmaps/songs_list.txt"), _T("r"));
		if (song_list_ptr == NULL)
		{
			perror("fopen error!");
			MessageBox(GetHWnd(), _T("�����ļ���ȡ���������������������"), _T("ERROR!"), MB_OK);
			SendMessage(GetHWnd(), WM_CLOSE, NULL, NULL);
		}*/

		/*for (int i = 0; i < SONGS_NUM; i++)
		{
			fgetws(songs_list[i], _countof(songs_list[i]), song_list_ptr);
			for (int j = 0; j < sizeof(songs_list[i]); j++)
			{
				if (songs_list[i][j] == _T('\n'))
				{
					songs_list[i][j] = _T('\0');
					break;
				}
			}
			wprintf(_T("%s\n"), songs_list[i]);
		}
		fclose(song_list_ptr);*/

// ƫc���ԵĽ������(select_songs_scene.h)
	//void get_info()
	//{
	//	TCHAR str[1024] = { };
	//	int count = 0;	//	TCHAR* ptr = nullptr;
	//	wcscpy_s(str, _countof(songs_list[current_song - 1]), songs_list[current_song - 1]);
	//	LPCTSTR token = wcstok_s(str, _T("-"), &ptr);		// �ֽ��ַ���,�����һ����Ҫ�ָ�Ĳ���
	//	while (token != NULL && count < 5)
	//	{

	//		switch (count)
	//		{
	//		case 0:											// ������
	//			info.ID = _wtoi(token);
	//			break;
	//		case 1:											// ������
	//			wcscpy_s(info.song_name, _countof(info.song_name), token);
	//			info.song_name[_countof(info.song_name) - 1] = '\0';	// ȷ���ַ�����NULL��β������strtok����
	//			break;
	//		case 2:											// ������
	//			wcscpy_s(info.singer_name, _countof(info.singer_name), token);
	//			info.singer_name[_countof(info.singer_name) - 1] = '\0';
	//			break;
	//		case 3:											// ��������
	//			wcscpy_s(info.song_length, _countof(info.song_length), token);
	//			info.song_length[_countof(info.song_length) - 1] = '\0';
	//			break;
	//		case 4:											// �������
	//			info.note_num = _wtoi(token);
	//			break;
	//		};
	//		token = wcstok_s(NULL, _T("-"), &ptr);
	//		count++;
	//	}
	//}


//����opening_scene.h��û���ö�ʱ�����Ƶĵ��뵭������
/*if (!is_fade_in_finished)
{
	trans_strength = min(trans_strength + 3, 255);
	is_fade_in_finished = (trans_strength == 255);
	if (is_fade_in_finished)
	{
		printf("sleep\n");
		Sleep(1000);
	}
}
else
{
	trans_strength = max(trans_strength - 3, 0);
}*/

//	// �����Ļ�ϵ�note���߼�
		//	if (!current_display_note.empty())		
		//	{
		//		if (current_note_line <= note_line && note_invocation[current_note - 1][] - game_time <= 3000)
		//		{
		//			current_display_note.push_back(note_list[current_note - 1]);
		//			current_note += 1;
		//		}
		//	}
		//	else
		//	{
		//		current_display_note.push_back(note_list[current_note - 1]);
		//		current_note += 1;
		//	}
		//	// ������ж����ǵ�����Ļ��note���߼�
		//	if (!current_display_note.front().empty() && current_display_note.front().front()->is_judged())
		//	{
		//		current_display_note.pop_front();
		//	}

		//	// ������Ļ�ϵ�note״̬
		//	for (auto it = current_display_note.begin(); it != current_display_note.end(); ++it)			// �������������list
		//	{
		//		for (int i = 0; i < (*it).size(); i++)
		//		{
		//			if (!(*it).empty())
		//			{
		//				if ((*it)[i]->getPosition() == Note::Position::Key_D)
		//					(*it)[i]->on_update(delta, game_time, is_debug);								// �±�����ڲ�vector
		//				else if ((*it)[i]->getPosition() == Note::Position::Key_F)
		//					(*it)[i]->on_update(delta, game_time, is_debug);
		//				else if ((*it)[i]->getPosition() == Note::Position::Key_J)
		//					(*it)[i]->on_update(delta, game_time, is_debug);
		//				else if ((*it)[i]->getPosition() == Note::Position::Key_K)
		//					(*it)[i]->on_update(delta, game_time, is_debug);
		//			}
		//		}
		//	}

//void on_input(const ExMessage& msg, int game_time)		// noteʶ���߼�
	//{
	//	if (msg.message == WM_KEYDOWN)
	//	{
	//		switch (posi)
	//		{
	//		case Position::Key_D:
	//			if (msg.vkcode == 0x44)
	//			{
	//				if (is_debug)
	//					std::cout << " note_D_pressed ";
	//				if (!key_status_for_note)
	//					key_status_for_note = true;
	//				judge(game_time, is_debug);
	//			}
	//			break;
	//		case Position::Key_F:
	//			if (msg.vkcode == 0x46)
	//			{
	//				if (is_debug)
	//					std::cout << " note_F_pressed ";
	//				if (!key_status_for_note)
	//					key_status_for_note = true;
	//				judge(game_time, is_debug);
	//			}
	//			break;
	//		case Position::Key_J:
	//			if (msg.vkcode == 0x4A)
	//			{
	//				if (is_debug)
	//					std::cout << " note_J_pressed ";
	//				if (!key_status_for_note)
	//					key_status_for_note = true;
	//				judge(game_time, is_debug);
	//			}
	//			break;
	//		case Position::Key_K:
	//			if (msg.vkcode == 0x4B)
	//			{
	//				if (is_debug)
	//					std::cout << " note_K_pressed ";
	//				if (!key_status_for_note)
	//					key_status_for_note = true;
	//				judge(game_time, is_debug);
	//			}
	//			break;
	//		}
		//	if (msg.vkcode == 0x44)	
		//	{
		//		if (is_debug)
		//			std::cout << " note_D_pressed ";
		//		if (!key_status_for_note.status_D_pressed)
		//			key_status_for_note.status_D_pressed = true;
		//	}
		//	if (msg.vkcode == 0x46)
		//	{
		//		if (is_debug)
		//			std::cout << " note_F_pressed ";
		//		if (!key_status_for_note.status_F_pressed)
		//			key_status_for_note.status_F_pressed = true;
		//	}
		//	if (msg.vkcode == 0x4A)
		//	{
		//		if (is_debug)
		//			std::cout << " note_J_pressed ";
		//		if (!key_status_for_note.status_J_pressed)
		//			key_status_for_note.status_J_pressed = true;
		//	}
		//	if (msg.vkcode == 0x4B)
		//	{
		//		if (is_debug)
		//			std::cout << " note_K_pressed ";
		//		if (!key_status_for_note.status_K_pressed)
		//			key_status_for_note.status_K_pressed = true;
		//	}
	/*	}
	}*/

	//if (note_list.empty() || note_list[note_line - 1].back()->get_note_time() == time) {
				//	// ���note_listΪ�գ����ߵ�ǰ�����һ��������time����Ҫ��ӵ�time��  
				//	// ��������е�note_list��note_invocation  
				//	std::vector<Note*> newLine;
				//	newLine.push_back(new Click(numToPosition(pos), time)); // ��������������ӵ�����  
				//	note_list.push_back(newLine);
				//	note_invocation.push_back(time);						// ��Ӹ���������Ӧ��ʱ��
				//	note_line += 1;
				//	note_list[note_line - 1].push_back(new Click(numToPosition(pos), time));
				//}
				//else
				//{
				//	// ����ֻ��ǰ���������  
				//	note_list[note_line - 1].push_back(new Click(numToPosition(pos), time));
				//}


// ʹ��delta_tick��Ҳ������θ��¾��ϴθ��µ�ʱ��ĳ��ȣ���Ԫ�ص��˶�����ʱ���йصĶ�������ʵʱ��ҹ�������������ѭ��ִ�е�ʱ��ҹ���֡���޹��ԣ�
		// �����ʱ�������������߼��е�һ�����ӣ���ȷ��������ѭ����ִ���ٶ���α仯����Ϸ�е�ʱ�����źͻ���ʱ��Ķ������ᱣ��һ�¡�
		// ���磬���������һ�������Ժ㶨���ٶ��ƶ��������Խ�������ٶȣ��Ե�λ/��Ϊ��λ������delta_tick������Ϊ��λ����
		// �Ӷ�����������ϴθ������������ʱ���ڶ���Ӧ���ƶ��ľ��롣����������֡����α仯�����󶼻�����ͬ���ٶ��ƶ���
		// ע�⣺GetTickCount()�����ľ�����16~18ms֮�䣬����ζ���κθ���Ƶ��С��16ms�Ĳ���������ѭ��ִ��ʱ��ҹ���ֱ��ѭ��ִ��ʱ�����16ms������
		// ��ʵ�֣�ʹ��GetTickCount()���޷��������ε��ӳ���Ҫ�����ø߾���ʱ�亯��
		//static DWORD last_tick_time = GetTickCount();// �þ�̬�����ĳ�ʼ������ִ��һ�Σ�֮��ѭ����ʹ�õĸñ���ֵԴ���ϸ�ѭ��
		//DWORD current_tick_time = GetTickCount();
		//DWORD delta_tick = current_tick_time - last_tick_time;
		//if (delta_tick < 15)
		//	delta_tick = 5;		// ��������һ��ѭ��ʱ��С��16ms��delta_tick��Ĭ��ֵ���������Ϸ��Ҳ�͵��뵭����Ҫ����ô����ĸ���Ƶ��...(�������������Ϸѭ��ʱ��ò��Ҳ������30ms...)
		//scene_manager.on_update(delta_tick, is_debug);
		//last_tick_time = current_tick_time;

// ��ʱ�뷨��ʹ��˫��Ƕ��vector���ʵ��������������ݣ����vector��ŵ����������ڲ�vector��ŵ���ÿ��ʵ�ʴ��ڵ�����
//			Ȼ�����ʱ������ʱ��ת��Ϊ�ܹ�����vector���±꣬�����һ�������У��Ϳ���ʹ���±����vector�ˡ�
//			ʹ�ö�̬��д����дһ��note�Ļ��࣬����������ʵ��click��hold���Եķ�����
// 
//			����Ϸ��ʼ����Ƶ���������´�����̿�ʼ��¼game_time����������������ж�
//			��������ʱ��ǰһ��ʱ���������Ҫ��������������ʲôʱ����м��أ��Լ���ô�����������Դ���ȶ��
//			// �ڳ�����on_update�з���note_list��note��ʱ�������ʱ�����
//			���������趨�������ٶ��������䣬click�ð죬hold�м������ôʵ�ֻ���˼��......
// 
//			�������������ٶ��������䣬���ƶ����ж�������һ������ʱ�ſ�ʼ������ж�״̬���ڰ���ʱ��ȷ���Ƿ�ɱ��ж����������ж�ֱ�������������ж������������ļ��ڵ�ʱ������бȽϣ������趨�õ��ж���Χ�����ж�
//
//			�ж����Ը��ݴ��������������ʵ�ֲ�ͬ���ж���ʽ��click��holdӦ�õõ���д�ж�����...��
//			���������ж�������û���ж��������miss�����������ڳ���Ļ���������
//			���������ж��������ж���ϣ�������ж����������ҽ�������
//			click�ж���д���Ƚ��ж���Χ���У�hold�ж������ø�����ʱ����ʱ�����Ѱ��µ�ʱ�������Ʒ������мƷ֣�ͷ��Ӧ�ÿ��Ե���click���ж��ͼƷַ������м��ɼ��ж�����bad���Ʒְ���ͷ��+holdʱ����֣�β��Ҳ������click�ķ���
//
//			�ж����϶���Ҫ�Ʒ֣���Ҫд���Ʒ�����Ա������click�����ж�������֣�hold���԰��հ��µ�ʱ����֣�������ֵ����ֱ�ӹ̶�������������㡣
//			��Ҫʵ��Ѫ����bad��miss��Ҫ��Ѫ��good���䣬great��perfect��Ҫ�Բ�ͬ������Ѫ�����������������У��������Ϊ100��ֱ��ʹ��easyx�Ļ���ͼ���ܾ��У���������ͼ����־�ݿ����׸���ǡ�
// 
//			����combo���ƣ�combo��Ҫ�ۼӼ��㣬ÿ��������һ��combo������osuһ��ÿ����combo��miss��bad�Ļ�combo���㡣���������Ļ�����д��combo�ļӷֻ��ƣ�(�㶨)
// 
//			���Ʒֺ��ж��ı�����Ҫ��ŵ������У���ʱ�����note�ĳ�Ա������ȡ����ֵ���У�(�㶨)
// 
//			����н���ҳ�棬����ҳ��Ϊ�˼������ֱ��д��gaming_scene�������ˡ�
//			�ڴ��Ʒ�ʱ����ͬʱ����һ�����۷��������۷�����ʵ�ʷ���ֱ��ʹ�����ý������ﶨ��ķ��������������
//			����ҳ����Ҫ��ʾ�������棬������Ϣ��combo���ж����������
//			�������Ϣ�����ʱ��ʹ������ˣ�ֱ���ã�combo���ж�����ͷ�����Ҫ����ڳ������ʱ����á�(�㶨)
//			
//			����뿪��ʱ����Ҫ��on_exit���������һ�����õ��Ķ���ȫ�����......