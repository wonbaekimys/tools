#pragma once

#include <string>
#include <ctime>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

enum LogTypes {
  kHadoop
};

class WbTime {
 public:
  WbTime();
  WbTime(const WbTime &t);
  WbTime(int year, int mon, int day, int hour, int min, double sec);
  ~WbTime();
  bool operator==(const WbTime &rhs);
  WbTime& operator=(const WbTime &t);
  bool operator>(const WbTime &t);
  bool operator<(const WbTime &t);
  static double get_duration(WbTime a, WbTime b);
  static WbTime get_wbtime(const string &record, const LogTypes &log_type);

  int year_;
  int mon_;
  int day_;
  int hour_;
  int min_;
  double sec_;

 private:
  static void swap(WbTime &a, WbTime &b);
  static struct tm to_tm_zerosec(const WbTime &t);
};

WbTime::WbTime() {
}

WbTime::WbTime(const WbTime &t) {
  year_ = t.year_;
  mon_ = t.mon_;
  day_ = t.day_;
  hour_ = t.hour_;
  min_ = t.min_;
  sec_ = t.sec_;
}

WbTime::WbTime(int year, int mon, int day, int hour, int min, double sec) {
  year_ = year;
  mon_ = mon;
  day_ = day;
  hour_ = hour;
  min_ = min;
  sec_ = sec;
}

WbTime::~WbTime() {
}


WbTime& WbTime::operator=(const WbTime &t) {
  year_ = t.year_;
  mon_ = t.mon_;
  day_ = t.day_;
  hour_ = t.hour_;
  min_ = t.min_;
  sec_ = t.sec_;
}


inline bool WbTime::operator==(const WbTime &rhs) {
  if (this->year_ == rhs.year_ && this->mon_ == rhs.mon_
        && this->day_ == rhs.day_ && this->hour_ == rhs.hour_
        && this->min_ == rhs.min_ && this->sec_ == rhs.sec_) {
    return true;
  } else {
    return false;
  }
}

bool WbTime::operator>(const WbTime &t) {
  if (*this == t) {
    return false;
  }
  if (year_ > t.year_) {
    return true;
  } else if (year_ < t.year_) {
    return false;
  } else {
    if (mon_ > t.mon_) {
      return true;
    } else if (mon_ < t.mon_) {
      return false;
    } else {
      if (day_ > t.day_) {
        return true;
      } else if (day_ < t.day_) {
        return false;
      } else {
        if (hour_ > t.hour_) {
            return true;
        } else if (hour_ < t.hour_) {
          return false;
        } else {
          if (min_ > t.min_) {
              return true;
          } else if (min_ < t.min_) {
            return false;
          } else {
            if (sec_ > t.sec_) {
                return true;
            } else {
              return false;
            }
          }
        }
      }
    }
  }
}

bool WbTime::operator<(const WbTime &t) {
  if (*this == t) {
    return false;
  }
  return !(*this > t);
}

void WbTime::swap(WbTime &a, WbTime &b) {
  WbTime tmp;
  tmp = a;
  a = b;
  b = tmp;
}

struct tm WbTime::to_tm_zerosec(const WbTime &t) {
  struct tm tmp;
  tmp.tm_sec = 0.0;
  tmp.tm_min = t.min_;
  tmp.tm_hour = t.hour_;
  tmp.tm_mday = t.day_;
  tmp.tm_mon = t.mon_;
  tmp.tm_year = t.year_;
  return tmp;
}

double WbTime::get_duration(WbTime a, WbTime b) {
// a is earlier time.
  if (a > b) {
    swap(a, b);
  }
  struct tm tm_a;
  struct tm tm_b;

  tm_a = to_tm_zerosec(a);
  tm_b = to_tm_zerosec(b);
  double diff = difftime(mktime(&tm_b), mktime(&tm_a));
  diff += b.sec_ - a.sec_;



  return diff;
}


WbTime WbTime::get_wbtime(const string &record, const LogTypes &log_type) {
  if (log_type == kHadoop) {
    int year = stoi(record.substr(0, 4));
    int mon = stoi(record.substr(5, 2));
    int day = stoi(record.substr(8, 2));
    int hour = stoi(record.substr(11, 2));
    int min = stoi(record.substr(14, 2));
    double sec = stoi(record.substr(17, 2)) + stod(record.substr(20, 3)) / 1000;
    WbTime tmp(year, mon, day, hour, min, sec);
    return tmp;
  }
  return WbTime(0, 0, 0, 0, 0, 0);
}
