#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
#include <ctime>
#include <functional>


class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void restart()										//��ʱ����״̬����
	{
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(int val)							// ��ʱ���ȴ�ʱ��
	{
		wait_time = val;
	}

	void set_one_shot(bool flag)						// ��ʱ���Ƿ񵥴δ���
	{
		one_shot = flag;
	}

	void set_callback(std::function<void()> callback)	// �����Զ���Ļص�����������ʹ��std::function��������һ���޲Σ�����ֵΪvoid���͵��κοɵ��ö��󣬲���������callback��Ա�����еȴ����ã�
	{
		this->callback = callback;	// �˴�ʹ��this��Ϊ����ȷָ���������ǵ�ǰ�����callback��Ա�������������β�
	}

	void set_callback(std::function<void(int)> callback)	// �����Զ���Ļص�����������ʹ��std::function����������һ������������ֵΪvoid���͵��κοɵ��ö��󣬲���������callback��Ա�����еȴ����ã�
	{
		this->callback_int = callback;	// �˴�ʹ��this��Ϊ����ȷָ���������ǵ�ǰ�����callback��Ա�������������β�
	}

	void pause()										// ��ʱ������ͣ
	{
		paused = true;
	}

	void resume()										// ��ʱ���Ļָ�����
	{
		paused = false;
	}

	bool get_status() const
	{
		return paused;
	}

	void on_update(int delta)							// ��ʱ������
	{
		if (paused)					// ����Ƿ���ͣ����ͣ�Ͳ�ִ�м�ʱ��ʱ�����
			return;

		int time = 0;
		pass_time += delta;			// �ۼ�֡����ʵ�ʹ�ȥ��ʱ��
		time += delta / wait_time; 	// �����ڹ�ȥ��ʱ������Ҫ�ظ����ζ�ʱ��

		if (pass_time >= wait_time) // ��ʱ��ʱ����������ʱ���бȽ�
		{
			if ((!one_shot || (one_shot && !shotted)) && callback)
				for (int i = 0; i < time; i++)
				{
					callback();
				}  // �������һ���Եģ���һ���Ե�δ��������֮���Ƕ�ʱ���Ƿ��ܱ�������������callback��Ϊ�գ�std::function����ɱ���ʽת��Ϊboolֵ�������԰����ظ�����ִ�и���Ļص�����
			else if ((!one_shot || (one_shot && !shotted)) && callback_int)
				callback_int(time);// �������һ���Եģ���һ���Ե�δ��������֮���Ƕ�ʱ���Ƿ��ܱ�������������callback_int��Ϊ�գ�std::function����ɱ���ʽת��Ϊboolֵ�������԰����ظ�����ִ�и���Ļص�����
			shotted = true;	 // �������
			pass_time = 0;   // ���ö�ʱ��
		}
	}

private:
	int pass_time = 0;					// �ѹ�ʱ��
	int wait_time = 0;					// �ȴ�ʱ��
	bool paused = false;				// �Ƿ���ͣ
	bool shotted = false;				// �Ƿ񴥷�
	bool one_shot = false;				// ���δ���
	std::function<void()> callback;		// �洢����Ա�ֶ�����Ļص�������std::function��ʵ���Կ�����һ������ָ��ĸ߼����������Ʒ����Ȼ����ֻ���Դ�ź�����������һ��������������������functionģ�壬
										// ����������ڿɱ���Ϊ��һ���ܹ��洢�κη�����ģ�����ָ��ǩ�������������ͺͷ������ͣ��Ŀɵ��ö�����������ȴ�����һ���ɵ��ö��󣬲�����Ҫʱʹ��������������ô�ŵĶ���
	std::function<void(int)> callback_int;		
};

#endif // !_TIMER_H_
