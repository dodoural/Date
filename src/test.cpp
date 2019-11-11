#include "Date.h"
#include <iostream>

int main()
{
Date::random_date();
Date mydate{"2019/11/10"};
Date randate{Date::random_date()};
std::time_t result = std::time(nullptr);
Date timedate{result};
mydate += 3 ;
mydate -= 10;
std::cout<<mydate<<std::endl;
std::cout<<timedate<<std::endl;
std::cout<<randate<<std::endl;


timedate++;
std::cout<<timedate<<std::endl;
timedate--;
std::cout<<timedate<<std::endl;

std::cout<<std::boolalpha<<"\n\tmydate > timedate is "<<(mydate > timedate)<<std::endl;
std::cout<<std::boolalpha<<"\n\tmydate < timedate is "<<(mydate < timedate)<<std::endl;
std::cout<<std::boolalpha<<"\n\tmydate == timedate is "<<(mydate == timedate)<<std::endl;
std::cout<<std::boolalpha<<"\n\tmydate != timedate is "<<(mydate != timedate)<<std::endl;

Date::WeekDay today = Date::WeekDay::Sunday;
for(int i=0 ; i < 5 ; i ++)
std::cout<<++today<<std::endl;
std::cout<<"\n\n";
for(int i=0 ; i < 5 ; i ++)
std::cout<<today++<<std::endl;
std::cout<<"\n\n";

for(int i=0 ; i < 5 ; i ++)
std::cout<<--today<<std::endl;
std::cout<<"\n\n";
for(int i=0 ; i < 5 ; i ++)
std::cout<<today--<<std::endl;

    return 0;
}