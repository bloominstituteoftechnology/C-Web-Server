#include <stdio.h>
#include <time.h>

// TIME STRUCT REFRENCE //
// struct tm {
//    int tm_sec;         /* seconds,  range 0 to 59          */
//    int tm_min;         /* minutes, range 0 to 59           */
//    int tm_hour;        /* hours, range 0 to 23             */
//    int tm_mday;        /* day of the month, range 1 to 31  */
//    int tm_mon;         /* month, range 0 to 11             */
//    int tm_year;        /* The number of years since 1900   */
//    int tm_wday;        /* day of the week, range 0 to 6    */
//    int tm_yday;        /* day in the year, range 0 to 365  */
//    int tm_isdst;       /* daylight saving time             */
// };

// EXAMPLE EXPECTED OUTPUT //
// Date: Mon, 27 Jul 2009 12:28:53 GMT

char *time_day(int day)
{
    switch (day)
    {
    case 0:
        return "Sun";
    case 1:
        return "Mon";
    case 2:
        return "Tue";
    case 3:
        return "Wed";
    case 4:
        return "Thu";
    case 5:
        return "Fri";
    case 6:
        return "Sat";
    }
}

char *time_month(int month)
{
    switch (month)
    {
    case 0:
        return "Jan";
    case 1:
        return "Feb";
    case 2:
        return "Mar";
    case 3:
        return "Apr";
    case 4:
        return "May";
    case 5:
        return "Jun";
    case 6:
        return "Jul";
    case 7:
        return "Aug";
    case 8:
        return "Sep";
    case 9:
        return "Oct";
    case 10:
        return "Nov";
    case 11:
        return "Dec";
    }
}

char *time_stamp()
{
    time_t rawtime;
    struct tm *c_time;
    char rv[512];

    time(&rawtime);

    c_time = localtime(&rawtime);
    sprintf(rv, "%s, %d %s %d %d:%d:%d CST",
            time_day(c_time->tm_wday),
            c_time->tm_mday,
            time_month(c_time->tm_mon),
            c_time->tm_year + 1900,
            c_time->tm_hour,
            c_time->tm_min,
            c_time->tm_sec);
    return rv;
}
