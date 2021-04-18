#ifndef CONVERT_H
#define CONVERT_H

#include <math.h>
#include <QDateTime>

void C_jday (
    int year, int mon, int day, int hr, int minute, double sec,
    double& jd
);

void C_mjday(
    int year, int mon, int day, int hr, int minute, double sec,
    double& mjd
);

//Older version
void C_days2mdhms
(
        int year, double days,
        int& mon, int& day, int& hr, int& minute, double& sec
        );
void C_invjday
(
        double jd,
        int& year, int& mon, int& day,
        int& hr, int& minute, double& sec
        );

double DateTime_to_JD (const QDateTime &dateTime);
QDateTime JD_to_DateTime (double& JD);

double dateHistoryToJD (const QString &dateHistoryValue) noexcept;

double m_getDeltaTime(const QDateTime &startTime, const QDateTime &finishTime);
#endif
