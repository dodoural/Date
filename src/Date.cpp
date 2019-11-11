#include <Date.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <random>


Date::s_date::s_date(uint16_t _year, uint16_t _month, uint16_t _day):year{_year},month{_month},day{_day}{}

Date::Date(unsigned int d,unsigned int m,unsigned int y):m_date(y,m,d){}

Date::Date(const char *p)
{
    std::string str{p};
    uint16_t date_members[3] = {0};
    std::string delimiter = "/";

    size_t i = 0;
    size_t pos = 0;
    std::string token{};
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        str.erase(0, pos + delimiter.length());
        date_members[i++] = string_to_int(token);
    }

    date_members[i++] = string_to_int(str);
    s_date tmp{static_cast<uint16_t>(date_members[0]),static_cast<uint8_t>(date_members[1]),static_cast<uint8_t>(date_members[2])};
    m_date = tmp ;

}

Date::Date(std::time_t &timer )
{
    tm* timePtr = localtime(&timer);
    s_date tmp{static_cast<uint16_t>(timePtr->tm_year + 1900),static_cast<uint8_t>(timePtr->tm_mon+1),static_cast<uint8_t>(timePtr->tm_mday)};
    m_date = tmp ;
}

Date::Date(const Date &r)
{
    this->m_date = r.m_date;
}

int Date::string_to_int(const std::string &r)
{
    if (r.empty())
        return -1;
    unsigned int ret = 0 ;
    size_t p = 0 ;

    for (auto i = r.end() - 1; i >= r.begin(); i--)
    {
        ret = ret + (static_cast<int>(*i)-48)*pow(10,p++);
    }

    return ret;
}

Date::WeekDay Date::get_week_day()const{

    unsigned int year= get_year();
    unsigned int month= get_month();
    unsigned int day= get_month_day();
    static const unsigned short month_code[] {0,3,3,6,1,4,6,2,5,0,3,5};
    unsigned short year_code = ((year%100) + (year%100)/4)%7;
    unsigned short century_code = 0;
    bool leap_year = false;

    if( year > 1752 || (year == 1752 && month > 9 ) || (year == 1752 && month == 9 && day >= 14) )
    {
        static const unsigned short century_codes[] {4,2,0,6,4,2,0};
        if(year >= 1700 && year < 1800)
            century_code = century_codes[0];
        else if(year >= 1800 && year < 1900)
            century_code = century_codes[1];
        else if(year >= 1900 && year < 2000)
            century_code = century_codes[2];
        else if(year >= 2000 && year < 2100)
            century_code = century_codes[3];
        else if(year >= 2100 && year < 2200)
            century_code = century_codes[4];
        else if(year >= 2200 && year < 2300)
            century_code = century_codes[5];
        else if(year >= 2300 && year < 2400)
            century_code = century_codes[6];

        leap_year = isleap(year);

        if(leap_year)
            return static_cast<Date::WeekDay>((year_code + month_code[month-1] + century_code -1 + day)%7);
        else
            return static_cast<Date::WeekDay>((year_code + month_code[month-1] + century_code + day)%7);
    }

    else // Julian Calendar is not supported
    {
        return Date::WeekDay::Not_Implemented;
    }
}
uint16_t Date::get_month_day()const
{
    return m_date.day;
}
uint16_t Date::get_month()const
{
    return m_date.month;
}
uint16_t Date::get_year()const
{
    return m_date.year;
}
int Date::get_year_day()const
{
    static const uint8_t month_days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    uint16_t year = get_year();
    uint16_t month = get_month();
    uint16_t day = get_month_day();
    if(month == 1)
        return get_month_day();

    uint16_t days_in_year = 0 ;
    bool leap_year = false;

    leap_year = isleap(year);

    for (uint8_t i = 0; i < month; i++)
    {
        if(i == month -1)
        {
            days_in_year = days_in_year + day;
            continue;
        }
        days_in_year = days_in_year + month_days[i];
    }
    if (leap_year && month > 2)
    {
        return days_in_year + 1 ;
    }
    else
    {
         return days_in_year;
    }

}

inline unsigned long  Date::date_to_integer(unsigned int year,unsigned int month, unsigned int day)
{
    month = (month + 9) % 12;
    year = year - month/10;
    return 365*year + year/4 - year/100 + year/400 + (month*306 + 5)/10 + ( day - 1 );
}

inline Date Date::integer_to_date(unsigned long  n)
{
    unsigned int year = (10000*n + 14780)/3652425 ;
    int ddd = n - (365*year + year/4 - year/100 + year/400);
    if (ddd < 0)
    {
    year = year - year;
    ddd = n - (365*year + year/4 - year/100 + year/400);
    }

    int mi = (100*ddd + 52)/3060 ;
    unsigned int month = (mi + 2)%12 + 1;
    year = year + (mi + 2)/12;
    unsigned int day = ddd - (mi*306 + 5)/10 + 1;

    return Date{day,month,year};
}
Date Date::random_date()
{
    static const uint8_t month_days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rand_year(1700,2400); // distribution in range [1, 6]
    std::uniform_int_distribution<std::mt19937::result_type> rand_month(1,12); // distribution in range [1, 6]
    std::uniform_int_distribution<std::mt19937::result_type> rand_day(1,31); // distribution in range [1, 6]
    uint16_t year = rand_year(rng);
    uint16_t month = rand_month(rng);
    uint16_t day = rand_day(rng);
    if(isleap(year) && month == 2)
    {
        while(day > 29)
            day = rand_day(rng);
    }
    else if(month == 2)
    {
        while(day > 28)
            day = rand_day(rng);
    }
    else
    {
        while(day > month_days[month-1])
            day = rand_day(rng);
    }

    return Date{day,month,year};
}

std::ostream &operator<<(std::ostream &os, const Date &date)
{
    os<<"\n\t"<<date.get_month_day()<<" / "<<date.get_month()<<" / "<<date.get_year()<<std::endl;
    os<<"\t"<<date.get_week_day()<<std::endl;
    return os;
}

Date& Date::operator=(const Date & r )
{
    this->m_date = r.m_date;
    return *this;
}

Date& Date::operator=(Date &&r )
{
    this->m_date = r.m_date;
    return *this;
}
Date& Date::operator+=(int day)
{
    unsigned long date_int = Date::date_to_integer(this->m_date.year,this->m_date.month,this->m_date.day);
    date_int = date_int + day;
    *this = integer_to_date(date_int);
    return *this;

}

Date& Date::operator-=(int day)
{
    unsigned long date_int = Date::date_to_integer(this->m_date.year,this->m_date.month,this->m_date.day);
    date_int = date_int - day;
    *this = integer_to_date(date_int);
    return *this;
}
Date& Date::operator++()
{
    unsigned long date_int = Date::date_to_integer(this->m_date.year,this->m_date.month,this->m_date.day);
    date_int = date_int + 1;
    *this = integer_to_date(date_int);
    return *this;
}

Date Date::operator++(int)
{
    Date tmp{*this};
    unsigned long date_int = Date::date_to_integer(this->m_date.year,this->m_date.month,this->m_date.day);
    date_int = date_int + 1;
    *this = integer_to_date(date_int);
    return tmp;
}

Date& Date::operator--()
{
    unsigned long date_int = Date::date_to_integer(this->m_date.year,this->m_date.month,this->m_date.day);
    date_int = date_int - 1;
    *this = integer_to_date(date_int);
    return *this;
}
Date Date::operator--(int)
{
    Date tmp{*this};
    unsigned long date_int = Date::date_to_integer(this->m_date.year,this->m_date.month,this->m_date.day);
    date_int = date_int - 1;
    *this = integer_to_date(date_int);
    return tmp;
}
bool operator==(const Date &r1, const Date &r2)
{
    if(r1.get_month_day() == r2.get_month_day() &&  r1.get_month() == r2.get_month() && r1.get_year() == r2.get_year())
        return true;
    else
        return false;
}

bool operator!=(const Date &r1, const Date &r2)
{
    return !(r1 == r2);
}


bool operator>(const Date &r1, const Date &r2)
{

    if(r1.get_year() < r2.get_year())
        return false;
    else if((r1.get_year() == r2.get_year()) && (r1.get_month() < r2.get_month()) )
        return false;
    else if((r1.get_year() == r2.get_year()) && (r1.get_month() == r2.get_month()) && (r1.get_month_day() < r2.get_month_day()))
        return false;
    else
        return true && (r1!=r2);
}

bool operator<(const Date &r1, const Date &r2)
{
    return !(r1>r2) && (r1!=r2);
}

bool operator<=(const Date &r1, const Date &r2)
{
    return (r1 < r2 ) || (r1 == r2 );
}

bool operator>=(const Date &r1, const Date &r2)
{
    return (r1 > r2 ) || (r1 == r2 );
}
Date Date::operator-(int days)const
{
    unsigned short year= get_year();
    unsigned short month= get_month();
    unsigned short day= get_month_day();
    unsigned long date_int = date_to_integer(year,month,day);
    date_int = date_int - days;
    return integer_to_date(date_int);
}

unsigned int operator-(const Date &d1, const Date &d2)
{
    unsigned short d1_year= d1.get_year();
    unsigned short d1_month= d1.get_month();
    unsigned short d1_day= d1.get_month_day();

    unsigned short d2_year= d2.get_year();
    unsigned short d2_month= d2.get_month();
    unsigned short d2_day= d2.get_month_day();

    unsigned long d1_date = Date::date_to_integer(d1_year,d1_month,d1_day);

    unsigned long d2_date = Date::date_to_integer(d2_year,d2_month,d2_day);
    long diff = d1_date - d2_date ;
    return abs(diff);

}

Date operator+(const Date &date, int n)
{
    unsigned short year= date.get_year();
    unsigned short month= date.get_month();
    unsigned short day= date.get_month_day();
    unsigned long date_int = Date::date_to_integer(year,month,day);
    date_int = date_int + n;
    return Date::integer_to_date(date_int);
}
Date operator+(int n, const Date &date)
{
    unsigned short year= date.get_year();
    unsigned short month= date.get_month();
    unsigned short day= date.get_month_day();
    unsigned long date_int = Date::date_to_integer(year,month,day);
    date_int = date_int + n;
    return Date::integer_to_date(date_int);
}



std::ostream &operator<<(std::ostream &os, const Date::WeekDay& day)
{
   static const char * const days[] = {"Sunday",
               "Monday",
               "Tuesday",
               "Wednesday",
               "Thursday",
               "Friday",
               "Saturday",
               "Not_Implemented"};

    return os<<days[static_cast<int>(day)];
}

Date::WeekDay& operator++(Date::WeekDay &r)
{
    if(r == Date::WeekDay::Saturday)
        r = Date::WeekDay::Sunday;

    else
        r = static_cast<Date::WeekDay>(static_cast<int>(r)+1);

    return r;

}

Date::WeekDay operator++(Date::WeekDay &r, int n)
{
    Date::WeekDay tmp = r ;
    if(r == Date::WeekDay::Saturday)
        r = Date::WeekDay::Sunday;

    else
        r = static_cast<Date::WeekDay>(static_cast<int>(r)+1);

    return tmp;

}

Date::WeekDay& operator--(Date::WeekDay &r)
{
    if(r == Date::WeekDay::Sunday)
        r = Date::WeekDay::Saturday;

    else
        r = static_cast<Date::WeekDay>(static_cast<int>(r)-1);

    return r;

}

Date::WeekDay operator--(Date::WeekDay &r, int n)
{
    Date::WeekDay tmp = r ;
    if(r == Date::WeekDay::Sunday)
        r = Date::WeekDay::Saturday;

    else
        r = static_cast<Date::WeekDay>(static_cast<int>(r)-1);

    return tmp;

}