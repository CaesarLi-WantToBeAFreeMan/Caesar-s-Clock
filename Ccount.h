#ifndef CCOUNT_H_
#define CCOUNT_H_

class Ccount{
	int second, minute, hour;
	void convert();
public:
	Ccount();
	Ccount(const int& hour, const int& minute, const int& second);
	wchar_t* show();
	void add_second();
	void minus_second();
	void clean();
	void SetSecond(const int& second);
	void SetMinute(const int& minute);
	void SetHour(const int& hour);
	bool check();
	bool is_end();

};

#endif