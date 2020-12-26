#include <lcom/lcf.h>
#include "rtc.h"
#include "rtc_macros.h"
#include <stdint.h>

void bcd_to_bin(uint32_t* bcd){
    *bcd = *bcd -6*(*bcd >> 4);
}

bool updating_rtc(){
    uint32_t reg_a_data;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_A)) return true;
    if(sys_inb(RTC_DATA_REG, &reg_a_data)) return true;

    if(reg_a_data & UIP) return true;
    return false;
}

bool bcd_format(){
    uint32_t reg_b_data=0;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_B)) return true;
    if(sys_inb(RTC_DATA_REG, &reg_b_data)) return true;

    if(!(reg_b_data & DM)) return true;
    return false;
}

bool military_time(){
    uint32_t reg_b_data=0;

    if(sys_outb(RTC_ADDR_REG, RTC_REGISTER_B)) return true;
    if(sys_inb(RTC_DATA_REG, &reg_b_data)) return true;

    if(reg_b_data & MILITARY_TIME) return true;
    return false;
}

int read_date(Date* date){
    if(sys_outb(RTC_ADDR_REG,YEAR_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(date->year))) return 1;

    if(sys_outb(RTC_ADDR_REG,MONTH_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(date->month))) return 1;

    if(sys_outb(RTC_ADDR_REG,DAY_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(date->day))) return 1;

    if(bcd_format()){
        bcd_to_bin(&(date->year));
        bcd_to_bin(&(date->month));
        bcd_to_bin(&(date->day));
    }

    return 0;
}

int read_time(Time* time){

    if(sys_outb(RTC_ADDR_REG,HOUR_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(time->hour))) return 1;

    if(sys_outb(RTC_ADDR_REG,MIN_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(time->minute))) return 1;

    if(sys_outb(RTC_ADDR_REG,SEC_REG)) return 1;
    if(sys_inb(RTC_DATA_REG, &(time->second))) return 1;


    if(bcd_format()){
        bcd_to_bin(&(time->hour));
        bcd_to_bin(&(time->minute));
        bcd_to_bin(&(time->second));
    }

    //if(military_time()) ;

    return 0;
}


Date get_date(){
    Date res;

    while(true){
        if(!updating_rtc()){
            read_date(&res);
            return res;
        }
    }
}

Time get_time(){
    Time res;

    while(true){
        if(!updating_rtc()){
            read_time(&res);
            return res;
        }
    }
}

