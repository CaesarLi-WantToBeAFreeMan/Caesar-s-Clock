#ifndef CTM_H_
#define CTM_H_
#include <ctime>

class Ctm {
	int second, minute, hour, day, month, year;
	void convert();
	wchar_t* month_fullname_eng();
	wchar_t* month_shortname_eng();
	void to_tm(tm* ti);

public:
	Ctm();
	Ctm(const int& year, const int& month, const int& day, const int& hour, const int& minute, const int& second);
	Ctm(const tm* stm);
	void nowgm(const int& zone);
	void now();
	wchar_t* show_time(const int& model);
	wchar_t* show_date(const int& model);
	wchar_t* different(Ctm* now);
	bool is_alarm_end(Ctm* now);
	void time_zone(const int& zone);
	void SetSecond(const int& second);
	void SetMinute(const int& minute);
	void SetHour(const int& hour);
	void SetDay(const int& day);
	void SetMonth(const int& month);
	void SetYear(const int& year);
	void clean();
	bool check();
	void minus_second();
	bool is_end();
	bool is_set_right(Ctm* Cnow);
};

#endif