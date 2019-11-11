#include <iosfwd>
#include <ctime>
#include <cstdint>
#include <string>
#include <iostream>

class Date {

    struct s_date
    {
    public:
        uint16_t year{};
        uint16_t month{};
        uint16_t day{};
        explicit s_date(uint16_t _year, uint16_t _month, uint16_t _day);
        s_date() = default;
        s_date& operator=(const s_date& r)
        {
            this->day = r.day;
            this->month = r.month;
            this->year = r.year;
            return *this;
        }
    };

    int string_to_int(const std::string &);
    s_date m_date{};


public:
	static constexpr int year_base = 1900;  //1
	static constexpr int random_min_year = 1940;  //2
	static constexpr int random_max_year = 2020;  //3
	enum class WeekDay {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Not_Implemented}; //4
	Date()= default; //5
	Date(unsigned int d, unsigned int m, unsigned int y);  //6
	Date(const char *p);
	Date(std::time_t &);
    Date(const Date &);
	uint16_t get_month_day()const; //9
	uint16_t get_month()const; //10
	uint16_t get_year()const; //11
	int get_year_day()const; //12
	WeekDay get_week_day()const; //13

	Date& set_month_day(int day); //14
	Date& set_month(int month); //15
	Date& set_year(int year); //16

    Date& operator=(const Date & );
    Date& operator=(Date &&r );
	Date operator-(int day)const; //17
	Date& operator+=(int day); //18
	Date& operator-=(int day); //19
	Date& operator++(); //20
	Date operator++(int); //21
	Date& operator--(); //22
	Date operator--(int); //23

	static Date random_date(); //24

	static constexpr bool isleap(int year)
    {
        bool leap_year = false;

        if((year % 4) == 0)
    {
        leap_year = true;

        if((year % 100) == 0)
        {
            leap_year = false;

            if((year % 400) == 0)
            {
                leap_year = true;
            }
        }
    }
        return leap_year;
    }
    static Date integer_to_date(unsigned long  n);
    static unsigned long  date_to_integer(unsigned int ,unsigned int, unsigned int);

    friend std::ostream &operator<<(std::ostream &os, const Date &date);

};

bool operator<(const Date &, const Date &); //26
bool operator<=(const Date &, const Date &); //27
bool operator>(const Date &, const Date &); //28
bool operator>=(const Date &, const Date &); //29
bool operator==(const Date &, const Date &); //30
bool operator!=(const Date &, const Date &); //31

unsigned int operator-(const Date &d1, const Date &d2);
Date operator+(const Date &date, int n); //33
Date operator+(int n, const Date &); //34
Date::WeekDay& operator++(Date::WeekDay &r); //35
Date::WeekDay operator++(Date::WeekDay &r, int); //36
Date::WeekDay& operator--(Date::WeekDay &r); //37
Date::WeekDay operator--(Date::WeekDay &r, int); //38
std::ostream &operator<<(std::ostream &os, const Date::WeekDay& day);


std::ostream &operator<<(std::ostream &os, const Date &date); //39
std::istream &operator>>(std::istream &is, Date &date); //40
