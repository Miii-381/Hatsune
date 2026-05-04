#ifndef _NOTE_H_
#define _NOTE_H_

// 音符基类：定义了所有音符类型的通用接口和属性
class Note
{
public:
	// 音符位置枚举（对应四个按键轨道）
	enum class Position {
		Key_D,     // D键轨道
		Key_F,     // F键轨道
		Key_J,     // J键轨道
		Key_K,     // K键轨道
		Invalid    // 无效位置
	};

	// 判定等级枚举
	enum class JudgeLevel {
		Perfect,   // 完美判定
		Great,     // 优秀判定
		Good,      // 良好判定
		Bad,       // 较差判定
		Miss,      // 未击中
		Invalid    // 无效判定
	};

	// 音符类型枚举
	enum class NoteType {
		Click,     // 单击音符
		Hold,      // 长按音符
		Invalid    // 无效类型
	};

	// 二维坐标点结构体
	struct POINT2
	{
		float x;   // X坐标
		float y;   // Y坐标
	};

	Note() = default;
	~Note() = default;

	// 音符进入时调用（初始化）
	virtual void on_enter(bool is_debug) { }

	// 每帧更新音符状态和位置
	// delta: 帧时间间隔（毫秒）
	// game_time: 当前游戏时间（毫秒）
	virtual void on_update(int delta, int game_time, bool is_debug) { }

	// 绘制音符
	virtual void on_draw(bool is_debug) { }

	// 处理用户输入
	virtual void on_input(const ExMessage& msg) { }
	
	// 判定玩家按键时机
	// game_time: 当前游戏时间
	// is_debug: 是否处于调试模式
	// message: 消息类型（WM_KEYDOWN或WM_KEYUP）
	virtual void judge(int game_time, bool is_debug, USHORT message) { }

	// 获取音符的目标时间点
	// 返回值：音符应该在的时间点（毫秒）
	virtual int GetNoteTime()
	{
		return -1;
	}

	// 获取音符是否已被判定
	// 返回值：true表示已判定，false表示未判定
	virtual bool get_judged() const
	{
		return false;
	}

	// 获取音符所在的轨道位置
	// 返回值：音符的位置枚举值
	virtual Position getPosition() const
	{
		return Position::Invalid;
	}

	// 计算音符得分
	// actual_point: 实际得分（引用传递，会被修改）
	// ideal_point: 理想得分
	// combo: 当前连击数
	// ideal_combo: 理想连击数
	// combo_bonus: 连击加成系数
	virtual void get_point(int& actual_point, int& ideal_point, int& combo, int& ideal_combo, double& combo_bonus) { }

	// 获取音符类型
	// 返回值：音符类型枚举值
	virtual Note::NoteType get_type() const
	{
		return note_type;
	}

public:
	JudgeLevel judge_level = JudgeLevel::Invalid;  // 当前判定等级
	NoteType note_type = NoteType::Invalid;        // 音符类型
};

#endif// !_NOTE_H_
