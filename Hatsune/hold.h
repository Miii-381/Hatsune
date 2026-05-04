#ifndef _HOLD_H_
#define _HOLD_H_

#include "ResourcesManager.h"
#include "utilities.h"
#include "timer.h"
#include "note.h"

// 长按音符类，继承自Note基类
// 表示需要玩家长时间按住的音符（有起始点和结束点）
class Hold : public Note
{
public:
	// 构造函数
	// pos: 音符所在的轨道位置
	// start_time: 长按开始的时间点（毫秒）
	// end_time: 长按结束的时间点（毫秒）
	// ID: 音符的唯一标识符
	// type: 音符类型
	Hold(Position pos, int start_time, int end_time, int ID, int type) : posi(pos), start_time(start_time), end_time(end_time), ID(ID)
	{
		note_type = NoteType::Hold;

		// 根据轨道位置设置长按音符的水平坐标
		switch (pos)
		{
		case Position::Key_D:
			pos_hold_start.x = 481;
			break;
		case Position::Key_F:
			pos_hold_start.x = 562;
			break;
		case Position::Key_J:
			pos_hold_start.x = 643;
			break;
		case Position::Key_K:
			pos_hold_start.x = 724;
			break;
		}
		// 结束点的水平坐标与起始点相同
		pos_hold_end.x = pos_hold_start.x;
	};

	~Hold() = default;

	// 更新长按音符的状态和位置
	// delta: 上一帧到当前帧的时间间隔（毫秒）
	// game_time: 当前游戏时间（毫秒）
	// is_debug: 是否处于调试模式
	void on_update(int delta, int game_time, bool is_debug)
	{
		static const int WINDOW_HEIGHT = 720;
		
		// 如果起始点或结束点还未被判定，则更新它们的位置
		if (!start_judged || !end_judged)
		{
			// 计算起始点的垂直位置
			pos_hold_start.y = 630.0 + (double)(game_time - start_time + ResourceMgr.offset) * ((double)ResourceMgr.drop_speed / 50.0) - ResourceMgr.img_hold.getheight() / 2;
			// 计算结束点的垂直位置
			pos_hold_end.y = 630.0 + (double)(game_time - end_time + ResourceMgr.offset) * ((double)ResourceMgr.drop_speed / 50.0) - ResourceMgr.img_hold.getheight() / 2;
		}

		// 累加按键按下的持续时间（用于计算长按得分）
		if (key_pressed)
		{
			press_time += delta;
		}

		// 如果起始点超出屏幕底部且未被判定，则判定为Miss
		if (pos_hold_start.y > WINDOW_HEIGHT && !start_judged)
		{
			if (is_debug) std::cout << " Miss note ";
			judge_level = Note::JudgeLevel::Miss;
			start_judged = true;
		}
		// 如果结束点超出屏幕底部，则判定为Miss
		if (pos_hold_end.y > WINDOW_HEIGHT)
		{
			if (is_debug) std::cout << " Miss note ";
			judge_level = Note::JudgeLevel::Miss;
			end_judged = true;
		}
	}

	// 绘制长按音符
	// is_debug: 是否处于调试模式（显示判定线）
	void on_draw(bool is_debug)
	{
		// 只有当音符在可视范围内、未完全判定且不是Miss时才绘制
		if (pos_hold_start.y >= -100 && (!start_judged || !end_judged) && (judge_level != Note::JudgeLevel::Miss))
		{
			// 绘制连接起始点和结束点的矩形条（长按的主体部分）
			solidrectangle(pos_hold_end.x + 2, pos_hold_end.y, pos_hold_start.x + ResourceMgr.img_hold.getwidth() - 4, pos_hold_start.y);

			// 绘制起始点和结束点的图片
			putimage_alpha(pos_hold_start.x, pos_hold_start.y, &ResourceMgr.img_hold);
			putimage_alpha(pos_hold_end.x, pos_hold_end.y, &ResourceMgr.img_hold);
		}

		// 调试模式下绘制判定线
		if (is_debug)
		{
			line(pos_hold_start.x, (pos_hold_start.y + ResourceMgr.img_hold.getheight() / 2.0), (pos_hold_start.x + 75.0), (pos_hold_start.y + ResourceMgr.img_hold.getheight() / 2.0));
			line(pos_hold_end.x, (pos_hold_end.y + ResourceMgr.img_hold.getheight() / 2.0), (pos_hold_end.x + 75.0), (pos_hold_end.y + ResourceMgr.img_hold.getheight() / 2.0));
		}
	}

	// 判定玩家按键时机（处理长按的按下和释放）
	// game_time: 当前游戏时间
	// is_debug: 是否处于调试模式
	// message: 消息类型（WM_KEYDOWN或WM_KEYUP）
	void judge(int game_time, bool is_debug, USHORT message) override
	{
		static const int WINDOW_HEIGHT = 720;
		if (!start_judged || !end_judged)
		{
			// 处理起始点判定（按键按下）
			if (pos_hold_start.y > 400 && !start_judged && message == WM_KEYDOWN)
			{
				int delta_time = game_time - start_time;

				// 根据时间差判定等级
				if (delta_time >= -50 && delta_time <= 50)
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Perfect;
				}
				else if (delta_time >= -70 && delta_time <= 70)
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Great;
				}
				else if (delta_time >= -100 && delta_time <= 100)
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Good;
				}
				else if (delta_time < -100 || delta_time > 100)
				{
					if (pos_hold_start.y <= WINDOW_HEIGHT)
					{
						if (is_debug) printf("delta_time = %d ", delta_time);
						judge_level = Note::JudgeLevel::Bad;
					}
				}

				allow_get_point = true;  // 允许计分
				start_judged = true;     // 标记起始点已判定
			}

			// 处理结束点判定（按键释放）
			if (start_judged && !end_judged && message == WM_KEYUP)
			{
				int delta_time = game_time - end_time;

				// 根据时间差判定等级
				if (delta_time >= -50 && delta_time <= 50)
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Perfect;
				}
				else if (delta_time >= -70 && delta_time <= 70)
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Great;
				}
				else if ((delta_time >= -100 && delta_time <= 100))
				{
					if (is_debug) printf("delta_time = %d ", delta_time);
					judge_level = Note::JudgeLevel::Good;
				}
				else if (delta_time < -100 || delta_time > 100)
				{
					// 如果结束点在屏幕内且在合理范围内，判定为Bad
					if (pos_hold_end.y <= WINDOW_HEIGHT && pos_hold_end.y >= 500)
					{
						if (is_debug) printf("delta_time = %d ", delta_time);
						judge_level = Note::JudgeLevel::Bad;
					}
					else
					{
						// 否则判定为Miss
						if (is_debug) printf("delta_time = %d ", delta_time);
						judge_level = Note::JudgeLevel::Miss;
					}
				}

				allow_get_point = true;  // 允许计分
				end_judged = true;       // 标记结束点已判定
				first_judge = false;     // 首次判定标志设为false
			}
		}
	}

	// 计算长按音符的得分
	// actual_point: 实际得分（引用传递，会被修改）
	// ideal_point: 理想得分
	// combo: 当前连击数
	// ideal_combo: 理想连击数
	// combo_bonus: 连击加成系数
	void get_point(int& actual_point, int& ideal_point, int& combo, int& ideal_combo, double& combo_bonus)
	{
		// 处理起始点或结束点的判定得分（只计一次）
		if (first_judge && allow_get_point)
		{
			switch (judge_level)
			{
			case JudgeLevel::Perfect:
				combo += 1;
				combo_bonus = 1.0 + (combo / 100) * 0.05;
				actual_point += 300;
				break;
			case JudgeLevel::Great:
				combo += 1;
				combo_bonus = 1.0 + (combo / 100) * 0.05;
				actual_point += 200 * combo_bonus;
				break;
			case JudgeLevel::Good:
				combo += 1;
				combo_bonus = 1.0 + (combo / 100) * 0.05;
				actual_point += 100 * combo_bonus;
				break;
			case JudgeLevel::Bad:
				combo = 0;
				combo_bonus = 1.0;
				actual_point += 50;
				break;
			case JudgeLevel::Miss:
				combo = 0;
				combo_bonus = 1.0;
				break;
			default:
				break;
			}
			ideal_combo += 1;
			ideal_point += 300;
			allow_get_point = false;  // 计分后重置标志
		}

		// 计算长按期间的持续得分（每100毫秒得10分）
		press_count = press_time / 100;

		// 如果按压计数增加，则加分
		if (press_count - last_press_count)
		{
			actual_point += 10;
		}

		last_press_count = press_count;
	}

	// 获取长按音符是否已完全判定（起始点和结束点都判定完成）
	bool get_judged() const override
	{
		return end_judged;
	}

	// 获取长按音符的起始时间点
	int GetNoteTime() override
	{
		return start_time;
	}

	// 获取音符所在的轨道位置
	Position getPosition() const
	{
		return posi;
	}

	// 获取音符类型
	Note::NoteType get_type() const
	{
		return note_type;
	}

private:
	POINT2 pos_hold_start = { 0, 0 };  // 长按起始点的位置
	POINT2 pos_hold_end = { 0, 0 };    // 长按结束点的位置
	Position posi;                      // 音符所在的轨道位置
	int ID = 0;                         // 音符ID
	int start_time = 0;                 // 长按开始时间（毫秒）
	int end_time = 0;                   // 长按结束时间（毫秒）
	int press_time = 0;                 // 按键按下的总时长（毫秒）
	int press_count = 0;                // 按压计数（每100毫秒算一次）
	int last_press_count = 0;           // 上一次的按压计数
	bool start_judged = false;          // 起始点是否已判定
	bool end_judged = false;            // 结束点是否已判定
	bool first_judge = true;            // 是否为首次判定
	bool key_pressed = false;           // 按键是否被按下
	bool allow_get_point = false;       // 是否允许计分
};

#endif // !_HOLD_H_
