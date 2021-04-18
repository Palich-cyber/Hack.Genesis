#include "convert.h"
#include "matrix.h"
#include "pz90.h"

double C_frac(double x )
{
    // Возвращает дробную часть числа

    return (x - (int)x);
}

double C_to0_2pi(double x)
{
    // Приводит угол к диапазону 0 - 2Pi

    return(x - M_PI * 2 * floor(x / (M_PI * 2)));
}

double C_to_pi_pi(double x)
{
    // Приводит угол к диапазону -Pi - Pi

    double tmp;

    tmp = x - M_PI * 2 * floor(x / (M_PI * 2));

    return((tmp > M_PI) ? tmp - M_PI * 2: tmp);
}

double C_deg_to_rad (double In)
{
    // Преобразование угла из градусов в радианы

    return (In * M_PI / 180.0);
}

double C_rad_to_deg (double In)
{
    // Преобразование угла из радианов в градусы

    return (In * 180 / M_PI);
}

double C_sign(double x)
{
    // Возвращает знач числа

    return((x < 0) ? -1.0 : 1.0);
}

/* -----------------------------------------------------------------------------
*
*                           procedure jday
*
*  this procedure finds the julian date given the year, month, day, and time.
*    the julian date is defined by each elapsed day since noon, jan 1, 4713 bc.
*
*  algorithm     : calculate the answer in one step for efficiency
*
*  author        : david vallado                  719-573-2600    1 mar 2001
*
*  inputs          description                    range / units
*    year        - year                           1900 .. 2100
*    mon         - month                          1 .. 12
*    day         - day                            1 .. 28,29,30,31
*    hr          - universal time hour            0 .. 23
*    min         - universal time min             0 .. 59
*    sec         - universal time sec             0.0 .. 59.999
*
*  outputs       :
*    jd          - julian date                    days from 4713 bc
*
*  locals        :
*    none.
*
*  coupling      :
*    none.
*
*  references    :
*    vallado       2007, 189, alg 14, ex 3-14
*
* --------------------------------------------------------------------------- */

void    C_jday
(
    int year, int mon, int day, int hr, int minute, double sec,
    double& jd
)
{
    jd = 367.0 * year -
         floor((7 * (year + floor((mon + 9) / 12.0))) * 0.25) +
         floor( 275 * mon / 9.0 ) +
         day + 1721013.5 +
         ((sec / 60.0 + minute) / 60.0 + hr) / 24.0;  // ut in days
    // - 0.5*sgn(100.0*year + mon - 190002.5) + 0.5;
}  // end jday

void C_mjday
(
    int year, int mon, int day, int hr, int minute, double sec,
    double& mjd
)
{
    double j1, m1, q, iq, ij;

    j1 = year - 1900;
    m1 = mon - 3;
    if  (m1 < 0)
    {
        m1 = m1 + 12;
        j1 = j1 - 1;
    }
    q = 0.5 + 30.6 * m1;
    iq = (int)q;
    ij = (int)(j1 / 4);
    q = iq + ij + day;
    mjd = 15078. + 365. * j1 + q;
    mjd = mjd + (hr + (minute + sec / 60.0) / 60.0) / 24.0 - 0.125;
}

void C_days2mdhms
(
        int year, double days,
        int& mon, int& day, int& hr, int& minute, double& sec
        )
{
    int i, inttemp, dayofyr;
    double    temp;
    int lmonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    dayofyr = (int)floor(days);

    if ( (year % 4) == 0 )
        lmonth[1] = 29;

    i = 1;
    inttemp = 0;
    while ((dayofyr > inttemp + lmonth[i-1]) && (i < 12))
    {
        inttemp = inttemp + lmonth[i-1];
        i++;
    }
    mon = i;
    day = dayofyr - inttemp;

    temp = (days - dayofyr) * 24.0;
    hr   = (int)floor(temp);
    temp = (temp - hr) * 60.0;
    minute  = (int)floor(temp);
    sec  = (temp - minute) * 60.0;
}  // end days2mdhms

void C_invjday
(
        double jd,
        int& year, int& mon, int& day,
        int& hr, int& minute, double& sec
        )
{
    int leapyrs;
    double    days, tu, temp;

    temp    = jd - 2415019.5;
    tu      = temp / 365.25;
    year    = 1900 + (int)floor(tu);
    leapyrs = (int)floor((year - 1901) * 0.25);

    days    = temp - ((year - 1900) * 365.0 + leapyrs) + 0.00000000001;

    if (days < 1.0)
    {
        year    = year - 1;
        leapyrs = (int)floor((year - 1901) * 0.25);
        days    = temp - ((year - 1900) * 365.0 + leapyrs);
    }

    C_days2mdhms(year, days, mon, day, hr, minute, sec);
    sec = sec - 0.00000086400;
}  // end invjday

double DateTime_to_JD(const QDateTime& dateTime)
{
    double _jd;

    C_jday( dateTime.date().year(), dateTime.date().month(), dateTime.date().day(), dateTime.time().hour(), dateTime.time().minute(), dateTime.time().second() + dateTime.time().msec() / 1000.0, _jd);
    return (_jd);
}

QDateTime JD_to_DateTime (double& JD)
{
    QDateTime _dateTime;
    int year, mon, day, hr, minute;
    double sec;

    C_invjday (JD, year, mon, day, hr, minute, sec);
    _dateTime.setDate(QDate(year, mon, day));
    _dateTime.setTime(QTime(hr, minute, (int)sec, C_frac(sec) * 1000.0));
    return _dateTime;
}

double m_getDeltaTime(const QDateTime& startTime, const QDateTime& finishTime){

    double tmp = 0.0F;
    double tmpStart = 0.0F;
    double tmpFinish = 0.0F;
    if (startTime.date().year()>finishTime.date().year()) return -1.0F;
    else{
        tmp+=(finishTime.date().year()-startTime.date().year())*364.25F*86400.0F;
        if (startTime.date().year()==finishTime.date().year() && startTime.date().month()>finishTime.date().month()) return -1.0F;
        else{
            int lmonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
            if ((startTime.date().year()%4)==0) lmonth[1] = 29;
            for (int i=startTime.date().month(); i<13; ++i){
                tmpStart+=lmonth[i]*86400.0F;
            }
            lmonth[1] = 28;
            if ((finishTime.date().year()%4)==0) lmonth[1] = 29;
            for (int i=finishTime.date().month(); i<13; ++i){
                tmpFinish+=lmonth[i]*86400.0F;
            }
            lmonth[1] = 28;
            if (startTime.date().year()==finishTime.date().year() && startTime.date().month()==finishTime.date().month() &&
                startTime.date().day()>finishTime.date().day()) return -1.0F;
            else{
                tmpStart-=startTime.date().day()*86400;
                tmpFinish-=finishTime.date().day()*86400;
                if (startTime.date().month()>finishTime.date().month()) tmp-=(tmpStart-tmpFinish);
                else tmp+=(tmpStart-tmpFinish);
                if ((startTime.date().year()==finishTime.date().year() && startTime.date().month()==finishTime.date().month() &&
                     startTime.date().day()==finishTime.date().day() && startTime.time().hour()>finishTime.time().hour()) ||
                    (startTime.date().year()==finishTime.date().year() && startTime.date().month()==finishTime.date().month() &&
                     startTime.date().day()==finishTime.date().day() && startTime.time().hour()==finishTime.time().hour() &&
                     startTime.time().minute()>finishTime.time().minute()) ||
                    (startTime.date().year()==finishTime.date().year() && startTime.date().month()==finishTime.date().month() &&
                     startTime.date().day()==finishTime.date().day() && startTime.time().hour()==finishTime.time().hour() &&
                     startTime.time().minute()==finishTime.time().minute() && startTime.time().second()>finishTime.time().second())) return -1.0F;
                else{
                    tmp+=((finishTime.time().hour()*3600+finishTime.time().minute()*60+finishTime.time().second())-
                         ((startTime.time().hour()*3600+startTime.time().minute()*60+startTime.time().second())));
                    return tmp;
                }
            }
        }
    }
}

double dateHistoryToJD( const QString &dateHistoryValue ) noexcept{

    QStringList list = dateHistoryValue.split("-");
    if ( list.size() <= 1 ) return 0.0F;

    QStringList underList = list[2].split(" ");
    double _jd;

    C_jday( list[0].toInt(), list[1].toInt(), underList[0].toInt(), 0, 0, 0.0F, _jd);

    return (_jd);
}
