#pragma once
// 此处存放的是编写程序过程中产生的废案，不保证能用，但大方向大概率是对的。

// 歌曲列表文件获取及错误检测(select_songs_scene.h)
		/*_wfopen_s(&song_list_ptr, _T("./resources/beatmaps/songs_list.txt"), _T("r"));
		if (song_list_ptr == NULL)
		{
			perror("fopen error!");
			MessageBox(GetHWnd(), _T("歌曲文件获取错误，请检查后重新启动程序"), _T("ERROR!"), MB_OK);
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

// 偏c语言的解决方法(select_songs_scene.h)
	//void get_info()
	//{
	//	TCHAR str[1024] = { };
	//	int count = 0;	//	TCHAR* ptr = nullptr;
	//	wcscpy_s(str, _countof(songs_list[current_song - 1]), songs_list[current_song - 1]);
	//	LPCTSTR token = wcstok_s(str, _T("-"), &ptr);		// 分解字符串,保存第一个需要分割的部分
	//	while (token != NULL && count < 5)
	//	{

	//		switch (count)
	//		{
	//		case 0:											// 歌手名
	//			info.ID = _wtoi(token);
	//			break;
	//		case 1:											// 歌曲名
	//			wcscpy_s(info.song_name, _countof(info.song_name), token);
	//			info.song_name[_countof(info.song_name) - 1] = '\0';	// 确保字符串以NULL结尾，避免strtok出错
	//			break;
	//		case 2:											// 歌手名
	//			wcscpy_s(info.singer_name, _countof(info.singer_name), token);
	//			info.singer_name[_countof(info.singer_name) - 1] = '\0';
	//			break;
	//		case 3:											// 歌曲长度
	//			wcscpy_s(info.song_length, _countof(info.song_length), token);
	//			info.song_length[_countof(info.song_length) - 1] = '\0';
	//			break;
	//		case 4:											// 总物件数
	//			info.note_num = _wtoi(token);
	//			break;
	//		};
	//		token = wcstok_s(NULL, _T("-"), &ptr);
	//		count++;
	//	}
	//}


//这是opening_scene.h中没有用定时器控制的淡入淡出代码
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

//	// 添加屏幕上的note的逻辑
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
		//	// 清除已判定或是掉出屏幕的note的逻辑
		//	if (!current_display_note.front().empty() && current_display_note.front().front()->is_judged())
		//	{
		//		current_display_note.pop_front();
		//	}

		//	// 更新屏幕上的note状态
		//	for (auto it = current_display_note.begin(); it != current_display_note.end(); ++it)			// 迭代器访问外层list
		//	{
		//		for (int i = 0; i < (*it).size(); i++)
		//		{
		//			if (!(*it).empty())
		//			{
		//				if ((*it)[i]->getPosition() == Note::Position::Key_D)
		//					(*it)[i]->on_update(delta, game_time, is_debug);								// 下标访问内层vector
		//				else if ((*it)[i]->getPosition() == Note::Position::Key_F)
		//					(*it)[i]->on_update(delta, game_time, is_debug);
		//				else if ((*it)[i]->getPosition() == Note::Position::Key_J)
		//					(*it)[i]->on_update(delta, game_time, is_debug);
		//				else if ((*it)[i]->getPosition() == Note::Position::Key_K)
		//					(*it)[i]->on_update(delta, game_time, is_debug);
		//			}
		//		}
		//	}

//void on_input(const ExMessage& msg, int game_time)		// note识别逻辑
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
				//	// 如果note_list为空，或者当前行最后一个音符的time等于要添加的time，  
				//	// 则添加新行到note_list和note_invocation  
				//	std::vector<Note*> newLine;
				//	newLine.push_back(new Click(numToPosition(pos), time)); // 创建新音符并添加到新行  
				//	note_list.push_back(newLine);
				//	note_invocation.push_back(time);						// 添加该行音符对应的时间
				//	note_line += 1;
				//	note_list[note_line - 1].push_back(new Click(numToPosition(pos), time));
				//}
				//else
				//{
				//	// 否则，只向当前行添加音符  
				//	note_list[note_line - 1].push_back(new Click(numToPosition(pos), time));
				//}


// 使用delta_tick，也就是这次更新距上次更新的时间的长度，令元素的运动等与时间有关的动作跟现实时间挂钩，而不是与主循环执行的时间挂钩（帧率无关性）
		// 将这个时间量用作更新逻辑中的一个因子，以确保无论主循环的执行速度如何变化，游戏中的时间流逝和基于时间的动作都会保持一致。
		// 例如，如果您想让一个对象以恒定的速度移动，您可以将对象的速度（以单位/秒为单位）乘以delta_tick（以秒为单位），
		// 从而计算出在自上次更新以来的这段时间内对象应该移动的距离。这样，无论帧率如何变化，对象都会以相同的速度移动。
		// 注意：GetTickCount()函数的精度在16~18ms之间，这意味着任何更新频率小于16ms的操作都会与循环执行时间挂钩，直到循环执行时间大于16ms。。。
		// 旧实现：使用GetTickCount()，无法满足音游的延迟需要，改用高精度时间函数
		//static DWORD last_tick_time = GetTickCount();// 该静态变量的初始化仅会执行一次，之后循环中使用的该变量值源自上个循环
		//DWORD current_tick_time = GetTickCount();
		//DWORD delta_tick = current_tick_time - last_tick_time;
		//if (delta_tick < 15)
		//	delta_tick = 5;		// 我设置了一下循环时间小于16ms后，delta_tick的默认值，在这个游戏里也就淡入淡出需要用这么抽象的更新频率...(但好像我这个游戏循环时间貌似也超不过30ms...)
		//scene_manager.on_update(delta_tick, is_debug);
		//last_tick_time = current_tick_time;

// 暂时想法：使用双层嵌套vector存放实际游玩的音符数据，外层vector存放的是行数，内层vector存放的是每行实际存在的音符
//			然后加载时将音符时间转化为能够调用vector的下标，存放在一个数组中，就可以使用下标调用vector了。
//			使用多态的写法，写一个note的基类，再在子类中实现click和hold各自的方法。
// 
//			当游戏开始后，音频播放命令下达后立刻开始记录game_time，用于音符掉落和判定
//			加载音符时提前一段时间加载所需要的音符，具体在什么时候进行加载，以及怎么加载音符，仍待商榷。
//			// 在场景的on_update中访问note_list中note的时间戳，当时间戳的
//			音符按照设定的下落速度匀速下落，click好办，hold中间的条怎么实现还得思考......
// 
//			音符按照下落速度匀速下落，在移动到判定区间外一定距离时才开始进入可判定状态。在按键时先确定是否可被判定，若不可判定直接跳过，若可判定则将其与谱面文件内的时间戳进行比较，按照设定好的判定范围进行判定
//
//			判定可以根据传入的音符类型来实现不同的判定方式（click和hold应该得单独写判定方法...）
//			若音符在判定区间内没被判定，则调用miss方法，并且在出屏幕后结束绘制
//			若音符在判定区间内判定完毕，则调用判定方法，并且结束绘制
//			click判定好写，比较判定范围就行，hold判定得设置个按键时长计时器，把按下的时长传给计分器进行计分，头判应该可以调用click的判定和计分方法，中间松键判定给个bad，计分按照头判+hold时间给分，尾判也可以用click的方法
//
//			判定完后肯定需要计分，需要写个计分器成员函数。click按照判定结果给分，hold可以按照按下的时间给分，给分数值可以直接固定下来，方便计算。
//			还要实现血条，bad和miss需要扣血，good不变，great和perfect需要以不同比例加血，给个整数变量就行，限制最高为100，直接使用easyx的绘制图像功能就行，避免生成图像出现锯齿可以套个外壳。
// 
//			还有combo机制，combo需要累加计算，每个音符算一个combo，不像osu一样每行算combo，miss和bad的话combo归零。（有条件的话可以写个combo的加分机制）(搞定)
// 
//			（计分和判定的变量需要存放到场景中，到时候调用note的成员函数获取返回值就行）(搞定)
// 
//			最后还有结算页面，结算页面为了简单起见，直接写到gaming_scene里面算了。
//			在打歌计分时可以同时计算一下理论分数，理论分数和实际分数直接使用引用将场景里定义的分数变量传入就行
//			结算页面需要显示歌曲封面，歌曲信息，combo，判定结果，分数
//			封面和信息进入的时候就传进来了，直接用，combo，判定结果和分数需要存放在场景里，到时候调用。(搞定)
//			
//			最后离开的时候需要在on_exit方法里面把一切所用到的东西全部清空......