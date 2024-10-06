#include "Ccount.h"
#include <cstdio>
using namespace std;
void Ccount::convert() {
	if (second > 59) {
		minute++;
		second -= 60;
	}
	else if (second < 0) {
		minute--;;
		second += 60;
	}
	if (minute > 59) {
		hour++;
		minute -= 60;
	}
	else if (minute < 0) {
		hour--;
		minute += 60;
	}
}

Ccount::Ccount() {
	hour = 0;
	minute = 0;
	second = 0;
}

Ccount::Ccount(const int& hour, const int& minute, const int& second) {
	this->hour = hour;
	this->minute = minute;
	this->second = second;
	convert();
}

wchar_t* Ccount::show() {
	wchar_t time[256];
	swprintf_s(time, 256, L"%d : %02d : %02d", hour, minute, second);
	return time;
}

void Ccount::add_second() {
	second++;
	convert();
}

void Ccount::minus_second() {
	second--;
	convert();
}

void Ccount::clean() {
	hour = 0;
	minute = 0;
	second = 0;
}

void Ccount::SetSecond(const int& second) {
	this->second = second;
}

void Ccount::SetMinute(const int& minute) {
	this->minute = minute;
}

void Ccount::SetHour(const int& hour) {
	this->hour = hour;
}

bool Ccount::check() {
	if (second > 59 || second < 0 || minute > 59 || minute < 0 || hour > 999 || hour < 0) {
		return false;
	}
	if (hour == 0 && minute == 0 && second == 0) {
		return false;
	}
	return true;
}

bool Ccount::is_end() {
	int time = hour * 3600 + minute * 60 + second;
	if (time < 0) {
		return true;
	}
	return false;
}