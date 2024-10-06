#include "Ctm.h"
#include <ctime>
#include <string>
#include <string>
#include <cmath>
#include <windows.h>
#include <cstdio>
using namespace std;

int N_Mon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
S_Mon[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

Ctm::Ctm() {
	second = 0;
	minute = 0;
	hour = 0;
	day = 1;
	month = 1;
	year = 1;
}

Ctm::Ctm(const int& year, const int& month, const int& day, const int& hour, const int& minute, const int& second) {
	this->second = second;
	this->minute = minute;
	this->hour = hour;
	this->day = day;
	this->month = month;
	this->year = year;
}

Ctm::Ctm(const tm* stm) {
	year = stm->tm_year + 1900;
	month = stm->tm_mon + 1;
	day = stm->tm_mday;
	hour = stm->tm_hour;
	minute = stm->tm_min;
	second = stm->tm_sec;
}

void Ctm::convert() {
	if (second > 59) {
		minute += second / 60;
		second = second % 60;
	}
	else if (second < 0) {
		minute += floor(double(second) / 60.0);
		second = 60 + second % 60;
	}
	if (minute > 59) {
		hour += minute / 60;
		minute = minute % 60;
	}
	else if (minute < 0) {
		hour += floor(double(minute) / 60.0);
		minute = 60 + minute % 60;
	}
	if (hour > 23) {
		day++;
		hour = hour % 24;
	}
	else if (hour < 0) { 
		day--;
		hour = 24 + hour % 24;
	}
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		if (day > S_Mon[month - 1]) {
			day %= S_Mon[month - 1];
			month++;
			day++;
		}
		else if (day < 1) {
			day %= S_Mon[month - 2];
			month--;
			day++;
		}
	}
	else {
		if (day > N_Mon[month - 1]) {
			day %= N_Mon[month - 1];
			month++;
			day++;
		}
		else if (day < 1) {
			day %= N_Mon[month - 2];
			month -= day / N_Mon[month - 1];
			day++;
		}
	}
	if (month > 12) {
		year++;
		month = 1;
	}
	else if (month < 1) {
		year++;
		month = 12;
	}
}

wchar_t* Ctm::month_fullname_eng() {
	wchar_t name[256];
	switch (month) {
	case 1:
		wcscpy_s(name, 256, L"January");
		break;
	case 2:
		wcscpy_s(name, 256, L"February");
		break;
	case 3:
		wcscpy_s(name, 256, L"March");
		break;
	case 4:
		wcscpy_s(name, 256, L"April");
		break;
	case 5:
		wcscpy_s(name, 256, L"May");
		break;
	case 6:
		wcscpy_s(name, 256, L"June");
		break;
	case 7:
		wcscpy_s(name, 256, L"July");
		break;
	case 8:
		wcscpy_s(name, 256, L"August");
		break;
	case 9:
		wcscpy_s(name, 256, L"September");
		break;
	case 10:
		wcscpy_s(name, 256, L"October");
		break;
	case 11:
		wcscpy_s(name, 256, L"November");
		break;
	case 12:
		wcscpy_s(name, 256, L"December");
		break;
	}
	return name;
}

wchar_t* Ctm::month_shortname_eng() {
	wchar_t name[256];
	switch (month) {
	case 1:
		wcscpy_s(name, 256, L"Jan.");
		break;
	case 2:
		wcscpy_s(name, 256, L"Feb.");
		break;
	case 3:
		wcscpy_s(name, 256, L"Mar.");
		break;
	case 4:
		wcscpy_s(name, 256, L"Apr.");
		break;
	case 5:
		wcscpy_s(name, 256, L"May.");
		break;
	case 6:
		wcscpy_s(name, 256, L"Jun.");
		break;
	case 7:
		wcscpy_s(name, 256, L"Jul.");
		break;
	case 8:
		wcscpy_s(name, 256, L"Aug.");
		break;
	case 9:
		wcscpy_s(name, 256, L"Sep.");
		break;
	case 10:
		wcscpy_s(name, 256, L"Oct.");
		break;
	case 11:
		wcscpy_s(name, 256, L"Nov.");
		break;
	case 12:
		wcscpy_s(name, 256, L"Dec.");
		break;
	}
	return name;
}

void Ctm::to_tm(tm* ti) {
	ti->tm_year = this->year - 1900;
	ti->tm_mon = this->month - 1;
	ti->tm_mday = this->day;
	ti->tm_hour = this->hour;
	ti->tm_min = this->minute;
	ti->tm_sec = this->second;
}

void Ctm::nowgm(const int& zone) {
	int h = zone / 100, min = zone % 100;
	time_t now = time(&now);
	tm* gmt = new tm;
	gmtime_s(gmt, &now);
	this->year = gmt->tm_year + 1900;
	this->month = gmt->tm_mon + 1;
	this->day = gmt->tm_mday;
	this->hour = gmt->tm_hour + h;
	this->minute = gmt->tm_min + min;
	this->second = gmt->tm_sec;
	delete gmt;
	convert();
}

void Ctm::now() {
	time_t now = time(&now);
	tm* tmnow = new tm;
	localtime_s(tmnow, &now);
	this->year = tmnow->tm_year + 1900;
	this->month = tmnow->tm_mon + 1;
	this->day = tmnow->tm_mday;
	this->hour = tmnow->tm_hour;
	this->minute = tmnow->tm_min;
	this->second = tmnow->tm_sec;
}

wchar_t* Ctm::show_time(const int& model) {
	wchar_t text [256], temp [256];
	switch (model) {
	case 1:
		swprintf_s(text, 256, L"%02d : %02d : %02d", hour, minute, second);
		break;
	case 2:
		hour < 12 ? wcscpy_s(temp, 256, L" A.M.") : wcscpy_s(temp, 256, L" P.M.");
		swprintf_s(text, 256, L"%02d : %02d : %02d %s", hour < 12 ? true : hour == 12 ? hour = 12 : hour - 12, minute, second, temp);
		break;
	case 3:
		hour < 12 ? wcscpy_s(temp, 256, L" a.m.") : wcscpy_s(temp, 256, L" p.m.");
		swprintf_s(text, 256, L"%02d : %02d : %02d %s", hour < 12 ? true : hour == 12 ? hour = 12 : hour - 12, minute, second, temp);
		break;
	}
	return text;
}

wchar_t* Ctm::show_date(const int& model) {
	wchar_t text[256], temp[256];
	switch (model) {
	case 1:
		swprintf_s(text, 256, L"%04d / %02d / %02d", year, month, day);
		break;
	case 2:
		wcscpy_s(temp, 256, month_fullname_eng());
		swprintf_s(text, 256, L"%s %02d, %04d", temp, day, year);
		break;
	case 3:
		wcscpy_s(temp, 256, month_shortname_eng());
		swprintf_s(text, 256, L"%s %02d, %04d", temp, day, year);
		break;
	}
	return text;
}

wchar_t* Ctm::different(Ctm* now) {
	tm* tmNow = new tm;
	now->to_tm(tmNow);
	time_t tNow = mktime(tmNow);

	tm* tmGoal = new tm;
	this->to_tm(tmGoal);
	time_t tGoal = mktime(tmGoal);

	time_t delta = difftime(tGoal, tNow);
	wchar_t result[256];
	time_t h = delta / 3600;
	delta -= h * 3600;

	time_t min = delta / 60;
	delta -= min * 60;
	wsprintf(result, L"%d : %02d : %02d", int(h), int(min), int(delta));
	return result;
}

bool Ctm::is_alarm_end(Ctm* now) {
	tm* tmNow = new tm;
	now->to_tm(tmNow);
	time_t tNow = mktime(tmNow);

	tm* tmGoal = new tm;
	this->to_tm(tmGoal);
	time_t tGoal = mktime(tmGoal);

	time_t delta = difftime(tNow, tGoal);
	if (delta > 0) {
		return true;
	}
	return false;
}

void Ctm::time_zone(const int& zone) {
	int h = zone / 100, min = zone % 100;
	convert();
}

void time_zone(const int& zone) {

}

void Ctm::SetSecond(const int& second) {
	this->second = second;
}

void Ctm::SetMinute(const int& minute) {
	this->minute = minute;
}

void Ctm::SetHour(const int& hour) {
	this->hour = hour;
}

void Ctm::SetDay(const int& day) {
	this->day = day;
}

void Ctm::SetMonth(const int& month) {
	this->month = month;
}

void Ctm::SetYear(const int& year) {
	this->year = year;
}

void Ctm::clean() {
	this->year = 1;
	this->month = 1;
	this->day = 1;
	this->hour = 0;
	this->minute = 0;
	this->second = 0;
}

bool Ctm::check() {
	if (second > 59 || second < 0 || minute > 59 || minute < 0 || hour > 24 || hour < 0 || day < 1 || month < 1 || month > 12 || year < 1900) {
		return false;
	}
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		if (day > S_Mon[month - 1]) {
			return false;
		}
	}
	else {
		if (day > N_Mon[month - 1]) {
			return false;
		}
	}
	if (year == 0 && minute == 0 && day == 0 && hour == 0 && minute == 0 && second == 0) {
		return false;
	}
	return true;
}

void Ctm::minus_second() {
	second--;
	convert();
}

bool Ctm::is_end() {
	int time = hour * 3600 + minute * 60 + second;
	if (time < 0) {
		return true;
	}
	return false;
}

bool Ctm::is_set_right(Ctm* Cnow) {
	tm* tmNow = new tm;
	Cnow->to_tm(tmNow);
	time_t tNow = mktime(tmNow);
	delete tmNow;

	tm* tmSetted = new tm;
	this->to_tm(tmSetted);
	time_t tSetted = mktime(tmSetted);

	if (tNow >= tSetted) {
		return false;
	}
	return true;
}