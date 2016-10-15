#include "Date.hpp"
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;

  Date::Date() {
    m_year = 0;
    m_month = 0;
    m_day = 0;
    m_hour = 0;
    m_minute = 0;
  }

  /**
  * @brief constructor with arguments
  */
  Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    m_year = t_year;
    m_month = t_month;
    m_day = t_day;
    m_hour = t_hour;
    m_minute = t_minute;
  }

  /**
  * @brief constructor with a string
  */
  Date::Date(string dateString) {
    *this = Date::stringToDate(dateString); 
  }
  /**
  * @brief return the year of a Date
  * @return   a integer indicate the year of a date
  */
  int Date::getYear(void) const {
    return m_year;
  }

  /**
  * @brief set the year of a date
  * @param a integer indicate the new year of a date
  */
  void Date::setYear(const int t_year) {
    m_year = t_year;
  }

  /**
  * @brief return the month of a Date
  * @return   a integer indicate the month of a date
  */
  int Date::getMonth(void) const {
    return m_month;
  }

  /**
  * @brief set the month of a date
  * @param a integer indicate the new month of a date
  */
  void Date::setMonth(const int t_month) {
    m_month = t_month;
  }

  /**
  * @brief return the day of a Date
  * @return   a integer indicate the day of a date
  */
  int Date::getDay(void) const {
    return m_day;
  }

  /**
  * @brief set the day of a date
  * @param a integer indicate the new day of a date
  */
  void Date::setDay(const int t_day) {
    m_day = t_day;
  }

  /**
  * @brief return the hour of a Date
  * @return   a integer indicate the hour of a date
  */
  int Date::getHour(void) const {
    return m_hour;
  }

  /**
  * @brief set the hour of a date
  * @param a integer indicate the new hour of a date
  */
  void Date::setHour(const int t_hour) {
    m_hour = t_hour;
  }

  /**
  * @brief return the minute of a Date
  * @return   a integer indicate the minute of a date
  */
  int Date::getMinute(void) const {
    return m_minute;
  }

  /**
  * @brief set the minute of a date
  * @param a integer indicate the new minute of a date
  */
  void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
  }

  /**
  *   @brief check whether the date is valid or not
  *   @return the bool indicate valid or not
  */
  bool Date::isValid(const Date t_date) {
    if (t_date.getYear() < 1000 || t_date.getYear() > 9999) return false;
    if (t_date.getMonth() > 12 || t_date.getMonth() < 1) return false;
    if (t_date.getDay() > 31 || t_date.getDay() < 1) return false;
    if (t_date.getHour() > 23 || t_date.getHour() < 0) return false;
    if (t_date.getMinute() > 59 || t_date.getMinute() < 0) return false;
    if (t_date.getMonth() == 4 || t_date.getMonth() == 6 || t_date.getMonth() == 9 || t_date.getMonth() == 11) {
      if (t_date.getDay() > 30) return false;
    }
    if (t_date.getMonth() == 2) {
      if ((t_date.getYear() % 4 == 0 && t_date.getYear() % 100 != 0) || t_date.getYear() % 400 == 0) {
        if (t_date.getDay() > 29) return false;
      } else {
        if (t_date.getDay() > 28) return false;
      }
    }
    return true;
  }

  /**
  * @brief convert a string to date, if the format is not correct return
  * 0000-00-00/00:00
  * @return a date
  */
  Date Date::stringToDate(const string t_dateString) {
    if (t_dateString.length() != 16) {
      return Date(0, 0, 0, 0, 0);
    } else {
      int m_year = 0, m_month = 0, m_day = 0, m_hour = 0, m_minute = 0;
      int count = sscanf(t_dateString.c_str(), "%04d-%02d-%02d/%02d:%02d", &m_year, &m_month, &m_day, &m_hour, &m_minute);
      Date ret(m_year, m_month, m_day, m_hour, m_minute);
      if (count == 5) {
        return ret;
      } else {
        return Date(0, 0, 0, 0, 0);
      }
    }
  }

  /**
  * @brief convert a date to string, if the format is not correct return
  * 0000-00-00/00:00
  */
  string Date::dateToString(Date t_date) {
    if (Date::isValid(t_date) == false) {
      return "0000-00-00/00:00";
    }
    string result = "";
    result += t_date.getYear() / 1000 + '0';
    result += ((t_date.getYear() / 100) % 10) + '0';
    result += (((t_date.getYear() / 10) % 100) % 10) + '0';
    result += (t_date.getYear() % 10) + '0';
    result += "-";
    result += t_date.getMonth() / 10 + '0';
    result += t_date.getMonth() % 10 + '0';
    result += "-";
    result += t_date.getDay() / 10 + '0';
    result += t_date.getDay() % 10 + '0';
    result += "/";
    result += t_date.getHour() / 10 + '0';
    result += t_date.getHour() % 10 + '0';
    result += ":";
    result += t_date.getMinute() / 10 + '0';
    result += t_date.getMinute() % 10 + '0';
    return result;
  }

  /**
  *  @brief overload the assign operator
  */
  Date& Date::operator=(const Date &t_date) {
    setYear(t_date.getYear());
    setMonth(t_date.getMonth());
    setDay(t_date.getDay());
    setHour(t_date.getHour());
    setMinute(t_date.getMinute());
    return *this;
  }

  /**
  * @brief check whether the CurrentDate is equal to the t_date
  */
  bool Date::operator==(const Date &t_date) const {
    if (getYear() == t_date.getYear() && getMonth() == t_date.getMonth() && getDay() == t_date.getDay() && getHour() == t_date.getHour() && getMinute() == t_date.getMinute()) {
      return true;
    } else {
      return false;
    }
  }

  /**
  * @brief check whether the CurrentDate is  greater than the t_date
  */
  bool Date::operator>(const Date &t_date) const {
    if (m_year > t_date.m_year) return true;
    if (m_year == t_date.m_year && m_month > t_date.m_month) return true;
    if (m_year == t_date.m_year && m_month == t_date.m_month && m_day > t_date.m_day) return true;
    if (m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day && m_hour > t_date.m_hour) return true;
    if (m_year == t_date.m_year && m_month == t_date.m_month && m_day == t_date.m_day && m_hour == t_date.m_hour && m_minute > t_date.m_minute) return true;
    return false;
  }

  /**
  * @brief check whether the CurrentDate is  less than the t_date
  */
  bool Date::operator<(const Date &t_date) const {
    if ((*this) > t_date || (*this) == t_date) {
      return false;
    } else {
      return true;
    }
  }

  /**
  * @brief check whether the CurrentDate is  greater or equal than the t_date
  */
  bool Date::operator>=(const Date &t_date) const {
    if ((*this) < t_date) {
      return false;
    } else {
      return true;
    }
  }

  /**
  * @brief check whether the CurrentDate is  less than or equal to the t_date
  */
  bool Date::operator<=(const Date &t_date) const {
    if ((*this) > t_date) {
      return false;
    } else {
      return true;
    }
  }
