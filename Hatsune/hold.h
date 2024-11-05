#ifndef _HOLD_H_
#define _HOLE_H_

#include "utilities.h"
#include "timer.h"
#include "note.h"

extern const int WINDOW_HEIGHT;
extern const int WINDOW_WIDTH;

extern int drop_speed;
extern int offset;
extern IMAGE img_hold;

class Hold : public Note
{
public:
	Hold(Position pos, int start_time, int end_time, int ID, int type) : posi(pos), start_time(start_time), end_time(end_time), ID(ID)
	{
		note_type = NoteType::Hold;

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
		pos_hold_end.x = pos_hold_start.x;
	};

	~Hold() = default;

	void on_update(int delta, int game_time, bool is_debug)
	{
		if (!start_judged || !end_judged)
		{
			pos_hold_start.y = 630 + (double)(game_time - start_time + offset) * (double)(drop_speed / 50) - img_hold.getheight() / 2;
			pos_hold_end.y = 630 + (double)(game_time - end_time + offset) * (double)(drop_speed / 50) - img_hold.getheight() / 2;
		}
		
		if (key_pressed)
		{
			press_time += delta;
		}

		if (pos_hold_start.y > WINDOW_HEIGHT && !start_judged)
		{
			if (is_debug) std::cout << " Miss note ";
			judge_level = Note::JudgeLevel::Miss;
			start_judged = true;
		}
		if (pos_hold_end.y > WINDOW_HEIGHT)
		{
			if (is_debug) std::cout << " Miss note ";
			judge_level = Note::JudgeLevel::Miss;
			end_judged = true;
		}
		
	}

	void on_draw(bool is_debug)
	{
		if (pos_hold_end.y <= WINDOW_HEIGHT&& (!start_judged || !end_judged) && (judge_level != Note::JudgeLevel::Miss))
		{
			solidrectangle(pos_hold_end.x + 2, pos_hold_end.y, pos_hold_start.x + img_hold.getwidth() - 4, pos_hold_start.y);

			putimage_alpha(pos_hold_start.x, pos_hold_start.y, &img_hold);
			putimage_alpha(pos_hold_end.x, pos_hold_end.y, &img_hold);
		}

		line(pos_hold_start.x, (pos_hold_start.y + img_hold.getheight() / 2), (pos_hold_start.x + 75), (pos_hold_start.y + img_hold.getheight() / 2));
		line(pos_hold_end.x, (pos_hold_end.y + img_hold.getheight() / 2), (pos_hold_end.x + 75), (pos_hold_end.y + img_hold.getheight() / 2));
	}

	void judge(int game_time, bool is_debug, USHORT message) override
	{
		if (!start_judged || !end_judged)
		{
			if (pos_hold_start.y > 400 && !start_judged && message == WM_KEYDOWN)
			{
				int delta_time = game_time - start_time + offset;

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

				allow_get_point = true;
				start_judged = true;
			}

			if (start_judged && !end_judged && message == WM_KEYUP)
			{
				int delta_time = game_time - end_time + offset;

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
					if (pos_hold_end.y <= WINDOW_HEIGHT && pos_hold_end.y >= 500)
					{
						if (is_debug) printf("delta_time = %d ", delta_time);
						judge_level = Note::JudgeLevel::Bad;
					}
					else
					{
						if (is_debug) printf("delta_time = %d ", delta_time);
						judge_level = Note::JudgeLevel::Miss;
					}
				}

				allow_get_point = true;
				end_judged = true;
				first_judge = false;
			}
		}
	}

	void get_point(int& actual_point, int& ideal_point, int& combo, int& ideal_combo, double& combo_bonus)
	{
		if (first_judge && allow_get_point)
		{
			switch (judge_level)
			{
			case JudgeLevel::Perfect:						// 分数 = 基础分数 * combo数加成（初始值为1，每100combo加0.05）
				combo += 1;									// (实际分数有连击加成，所以准确度可能会出现超过100%的情况，属于游戏设定)
				combo_bonus = 1.0 + (combo / 100) * 0.05;	// Hold根据按下时间也有额外加分
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
			allow_get_point = false;
		}

		press_count = press_time / 100;

		if (press_count - last_press_count)
		{
			actual_point += 10;
		}

		last_press_count = press_count;
	}

	bool get_judged() const override
	{
		return end_judged;
	}

	int GetNoteTime() override
	{
		return start_time;
	}

	Position getPosition() const
	{
		return posi;
	}

	Note::NoteType get_type() const
	{
		return note_type;
	}
private:
	POINT2 pos_hold_start = { 0,0 };
	POINT2 pos_hold_end = { 0,0 };
	Position posi;
	int ID = 0;
	int start_time = 0;
	int end_time = 0;
	int press_time = 0;
	int press_count = 0;
	int last_press_count = 0;
	bool start_judged = false;
	bool end_judged = false;
	bool first_judge = true;
	bool key_pressed = false;
	bool allow_get_point = false;
};




#endif // !_HOLD_H_
