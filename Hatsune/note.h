#ifndef _NOTE_H_
#define _NOTE_H_

class Note
{
public:
	enum class Position
	{
		Key_D, Key_F, Key_J, Key_K, Invalid
	};

	enum class JudgeLevel
	{
		Perfect, Great, Good, Bad, Miss, Invalid
	};

	enum class NoteType
	{
		Click, Hold
	};

	struct POINT2
	{
		float x;
		float y;
	};

	Note() = default;
	~Note() = default;

	virtual void on_enter(bool is_debug) { }

	virtual void on_update(int delta, int game_time, bool is_debug) { }

	virtual void on_draw(bool is_debug) { }

	virtual void on_input(const ExMessage& msg) { }
	
	virtual void judge(int game_time, bool is_debug, USHORT message) { }

	virtual int GetNoteTime()
	{
		return -1;
	}

	virtual bool get_judged() const
	{
		return false;
	}

	virtual Position getPosition() const
	{
		return Position::Invalid;
	}

	virtual void get_point(int& actual_point, int& ideal_point, int& combo, int& ideal_combo, double& combo_bonus) { }

	virtual Note::NoteType get_type() const
	{
		return note_type;
	}

public:
	JudgeLevel judge_level = JudgeLevel::Invalid;
	NoteType note_type;
};

#endif// !_NOTE_H_