#ifndef _CLICK_H_
#define _CLICK_H_

#define JUDGE_LINE 630;

#include "timer.h"
#include "utilities.h"
#include "note.h"

extern const int WINDOW_HEIGHT;
extern const int WINDOW_WIDTH;

extern int drop_speed;
extern int offset;
extern IMAGE img_click;

class Click : public Note
{
public:
	Click(Position pos, int note_time, int ID, int type) : posi(pos), note_time(note_time), ID(ID)
	{
		note_type = NoteType::Click;
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

	void on_update(int delta, int game_time, bool is_debug)
	{
		pos_click.y = 630 + (double)(game_time - note_time + offset) * (double)(drop_speed / 50) - img_click.getheight() / 2;

		if (pos_click.y > WINDOW_HEIGHT)
		{
			if(is_debug) std::cout << " Miss note ";
			judge_level = Note::JudgeLevel::Miss;
			is_judged = true;
		}
	}

	void on_draw(bool is_debug)
	{
		if(pos_click.y >= -100 && is_judged == false)
			putimage_alpha(pos_click.x, pos_click.y, &img_click);

		line(pos_click.x, (pos_click.y + img_click.getheight() / 2), (pos_click.x + 75), (pos_click.y + img_click.getheight() / 2));
	}

	void judge(int game_time, bool is_debug, USHORT message)
	{
		if (!is_judged)
		{
			if (pos_click.y > 400)
			{
				int delta_time = game_time - note_time;
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
					if (pos_click.y <= WINDOW_HEIGHT)
					{
						if (is_debug) printf("delta_time = %d ", delta_time);
						judge_level = Note::JudgeLevel::Bad;
					}
				}
				is_judged = true;
			}
		}
	}

	void get_point(int& actual_point, int& ideal_point, int& combo, int& ideal_combo, double& combo_bonus)
	{
		if (first_judge)
		{
			switch (judge_level)
			{
			case JudgeLevel::Perfect:							// 分数 = 基础分数 * combo数加成（初始值为1，每100combo加0.1）
				combo += 1;									// (实际分数有连击加成，所以准确度可能会出现超过100%的情况，属于游戏设定)
				combo_bonus = 1.0 + (combo / 100) * 0.05;
				actual_point += 300 * combo_bonus;
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
			first_judge = false;
		}
	}

	bool get_judged() const
	{
		return is_judged;
	}

	int GetNoteTime() override
	{
		return note_time;
	}

	Position getPosition() const
	{
		return posi;
	}

	Note::NoteType get_type() const override
	{
		return note_type;
	}

private:
	POINT2 pos_click = { 0,0 };
	Position posi;
	int ID;
	int note_time;
	bool is_judged = false;
	bool first_judge = true;
};


#endif // !_CLICK_H_