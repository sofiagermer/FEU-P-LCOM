#ifndef _LCOM_RTC_H_
#define _LCOM_RTC_H_

#include <lcom/lcf.h>

typedef struct{
    uint32_t year;
    uint32_t month;
    uint32_t day;
} Date;

typedef struct{
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
} Time;

void bcd_to_bin(uint32_t* bcd);

bool updating_rtc();

bool bcd_format();

bool military_time();

int read_date(Date* date);

int read_time(Time* time);

Date get_date();

Time get_time();


#endif
