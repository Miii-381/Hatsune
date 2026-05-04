#ifndef _CLICK_H_
#define _CLICK_H_

#include "ResourcesManager.h"
#include "timer.h"
#include "utilities.h"
#include "note.h"

// 单击音符类，继承自Note基类
// 表示需要在特定时间点按下的普通音符
class Click : public Note
{
public:
	// 构造函数
	// pos: 音符所在的轨道位置（D/F/J/K）
	// note_time: 音符应该被击中的时间点（毫秒）
	// ID: 音符的唯一标识符
	// type: 音符类型
	Click(Position pos, int note_time, int ID, int type) : posi(pos), note_time(note_time), ID(ID)
	{
		note_type = NoteType::Click;
		// 根据轨道位置设置音符的水平坐标
		switch (pos)
		{
		case Position::Key_D:
			pos_click.x = 481;
			break;
		case Position::Key_F:
			pos_click.x = 562;
			break;
		case Position::Key_J:
			pos_click.x = 643;
			break;
		case Position::Key_K:
			pos_click.x = 724;
			break;
		}
	};

	~Click() = default;

	// 更新音符状态和位置
	// delta: 上一帧到当前帧的时间间隔（毫秒）
	// game_time: 当前游戏时间（毫秒）
	// is_debug: 是否处于调试模式
	void on_update(int delta, int game_time, bool is_debug)
	{
		static const int WINDOW_HEIGHT = 720;
		
		// 计算音符的垂直位置
		// 公式：基准位置 + (当前时间 - 音符时间 + 偏移量) * (下落速度 / 50) - 图片高度的一半
		// 这样可以让音符在正确的时间点到达判定线（y=630）
		pos_click.y = 630 + (double)(game_time - note_time + ResourceMgr.offset) * ((double)ResourceMgr.drop_speed / 50) - ResourceMgr.img_click.getheight() / 2;

		// 如果音符超出屏幕底部且未被判定，则判定为Miss
		if (pos_click.y > WINDOW_HEIGHT)
		{
			if (is_debug) std::cout << " Miss note ";
			judge_level = Note::JudgeLevel::Miss;
			is_judged = true;
		}
	}

	// 绘制音符
	// is_debug: 是否处于调试模式（显示判定线）
	void on_draw(bool is_debug)
	{
		// 只有当音符在可视范围内且未被判定时才绘制
		if (pos_click.y >= -100 && is_judged == false)
			putimage_alpha(pos_click.x, pos_click.y, &ResourceMgr.img_click);

		// 绘制判定线（用于调试）
		if(is_debug)
			line(pos_click.x, (pos_click.y + ResourceMgr.img_click.getheight() / 2), (pos_click.x + 75), (pos_click.y + ResourceMgr.img_click.getheight() / 2));
	}

	// 判定玩家按键时机
	// game_time: 当前游戏时间
	// is_debug: 是否处于调试模式
	// message: 消息类型（WM_KEYDOWN或WM_KEYUP）
	void judge(int game_time, bool is_debug, USHORT message)
	{
		static const int WINDOW_HEIGHT = 720;
		if (!is_judged)
		{
			// 只有当音符进入判定区域（y > 400）时才进行判定
			if (pos_click.y > 400)
			{
				// 计算按键时间与音符时间的差值
				int delta_time = game_time - note_time;
				
				// 根据时间差判定等级
				// Perfect: ±50ms
				if (delta_time >= -50 && delta_time <= 50)
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Perfect;
				}
				// Great: ±70ms
				else if (delta_time >= -70 && delta_time <= 70)
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Great;
				}
				// Good: ±100ms
				else if (delta_time >= -100 && delta_time <= 100)
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Good;
				}
				// Bad: 超过±100ms但仍在屏幕内
				else if (delta_time < -100 || delta_time > 100)
				{
					if (pos_click.y <= WINDOW_HEIGHT)
					{
						if (is_debug) printf("delta_time = %d ", delta_time);
						judge_level = Note::JudgeLevel::Bad;
					}
				}
				is_judged = true;  // 标记为已判定
			}
		}
	}

	// 计算得分和连击
	// actual_point: 实际得分（引用传递，会被修改）
	// ideal_point: 理想得分（满分情况下的得分）
	// combo: 当前连击数
	// ideal_combo: 理想连击数
	// combo_bonus: 连击加成系数
	void get_point(int& actual_point, int& ideal_point, int& combo, int& ideal_combo, double& combo_bonus)
	{
		// 只在首次判定时计分
		if (first_judge)
		{
			switch (judge_level)
			{
			case JudgeLevel::Perfect:
				// Perfect: 基础分300，享受连击加成
				combo += 1;
				combo_bonus = 1.0 + (combo / 100) * 0.05;
				actual_point += 300 * combo_bonus;
				break;
			case JudgeLevel::Great:
				// Great: 基础分200，享受连击加成
				combo += 1;
				combo_bonus = 1.0 + (combo / 100) * 0.05;
				actual_point += 200 * combo_bonus;
				break;
			case JudgeLevel::Good:
				// Good: 基础分100，享受连击加成
				combo += 1;
				combo_bonus = 1.0 + (combo / 100) * 0.05;
				actual_point += 100 * combo_bonus;
				break;
			case JudgeLevel::Bad:
				// Bad: 固定50分，打断连击
				combo = 0;
				combo_bonus = 1.0;
				actual_point += 50;
				break;
			case JudgeLevel::Miss:
				// Miss: 不得分，打断连击
				combo = 0;
				combo_bonus = 1.0;
				break;
			default:
				break;
			}
			ideal_combo += 1;   // 理想连击数增加
			ideal_point += 300; // 理想得分增加（按Perfect计算）
			first_judge = false; // 标记为已计分
		}
	}

	// 获取音符是否已被判定
	bool get_judged() const
	{
		return is_judged;
	}

	// 获取音符的目标时间点
	int GetNoteTime() override
	{
		return note_time;
	}

	// 获取音符所在的轨道位置
	Position getPosition() const
	{
		return posi;
	}

	// 获取音符类型
	Note::NoteType get_type() const override
	{
		return note_type;
	}

private:
	POINT2 pos_click = { 0, 0 };  // 音符的位置坐标
	Position posi;                 // 音符所在的轨道位置
	int ID;                        // 音符ID
	int note_time;                 // 音符的目标时间点（毫秒）
	bool is_judged = false;        // 是否已被判定
	bool first_judge = true;       // 是否为首次判定（用于计分控制）
};

#endif // !_CLICK_H_
