#include "subway.h"

char* const startStation = "Gongneung";
// char destination[100][50];
struct tm* currentTime = NULL;

int weekdayTime[WEEKDAY_SIZE][2];
int saturdayTime[SATURDAY_SIZE_AND_HOLIDAY_SIZE][2];
int holidayTime[SATURDAY_SIZE_AND_HOLIDAY_SIZE][2];

Timetable weekday[WEEKDAY_SIZE];
Timetable saturday[SATURDAY_SIZE_AND_HOLIDAY_SIZE];
Timetable holiday[SATURDAY_SIZE_AND_HOLIDAY_SIZE];

Station stations[STATION_INTERVAL_SIZE];



int main(void)
{
    char temp[100];
    /* while(1)
    {
        if(CanStartSimulation() == 1)
        {
            printf("시작\n");
            break;
        }

        else if(CanStartSimulation() == 0)
        {
            printf("종료\n");
            exit(1);
        }

        else if(CanStartSimulation() == -1)
        {
            printf("오류\n");
            continue;
        }
    } */

    LoadTimeTable();
    LoadStationInterval();

    scanf("%s", temp);    
    currentTime = FindCurrentTimeByTm();
    ChangeTimeFromCharToInt();
    CompareBetweenTimetableAndCurrentTime(temp);
    return 0;
}

int CanStartSimulation()
{
    char yesOrNo;
    printf("시뮬레이션을 시작하시겠습니까?");
    scanf("%c", &yesOrNo);
    while(getchar() != '\n');

    switch(yesOrNo)
    {
        case 'Y':
        case 'y':
            return 1;
            break;
        case 'N':
        case 'n':
            return 0;
            break;
        default:
            return -1;
            break;
    }
}

void LoadTimeTable()
{
    FILE *fpWeekday = NULL;
    FILE *fpSaturday = NULL;
    FILE *fpHoliday = NULL;

    fpWeekday = fopen("train_table_Weekday.txt", "r");
    fpSaturday = fopen("train_table_Saturday.txt", "r");
    fpHoliday = fopen("train_table_Holiday.txt", "r");
    
    for (int i = 0; i < 397; i++)
    {
        fscanf(fpWeekday, "%s %s", weekday[i]._time, weekday[i].destination);
    }

    for (int i = 0; i < 343; i++)
    {
        fscanf(fpSaturday, "%s %s", saturday[i]._time, saturday[i].destination);
        fscanf(fpHoliday, "%s %s", holiday[i]._time, holiday[i].destination);
    }

    fclose(fpHoliday);
    fclose(fpSaturday);
    fclose(fpWeekday);
}

void LoadStationInterval()
{
    FILE *fpInterval = NULL;
    fpInterval = fopen("station_interval.txt", "r");

    for(int i = 0; i < 51; i++)
    {
        fscanf(fpInterval, "%d %s %d", &stations[i].stationNum, stations[i].station, &stations[i].intervalFromGongneung);
    }

    fclose(fpInterval);
}

int ReturnStationNumber(char* station)
{
    for(int i = 0; i < STATION_INTERVAL_SIZE; i++)
    {
        if(strcmp(station, stations[i].station) == 0)
            return stations[i].stationNum;
    }
}

char* FindCurrentTime()
{
    char buf[80];
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(buf, 80, "%k:%M", timeinfo);
    return buf;
}

struct tm* FindCurrentTimeByTm()
{
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    return timeinfo;
}

void ChangeTimeFromCharToInt()
{
    int hour, min;
    int tempnum;
    char temp[100];
    for(int i = 0; i < WEEKDAY_SIZE; i++)
    {
        temp[0] = weekday[i]._time[0];
        temp[1] = weekday[i]._time[1];
        temp[2] = weekday[i]._time[3];
        temp[3] = weekday[i]._time[4];
        tempnum = (int) atoi(temp);
        hour = tempnum / 100; // 앞 두자리 수 시간
        min = tempnum % 100; // 뒤 두자리수 분
        weekdayTime[i][0] = hour;
        weekdayTime[i][1] = min;
    }

    for(int i = 0; i < SATURDAY_SIZE_AND_HOLIDAY_SIZE; i++)
    {
        temp[0] = saturday[i]._time[0];
        temp[1] = saturday[i]._time[1];
        temp[2] = saturday[i]._time[3];
        temp[3] = saturday[i]._time[4];
        tempnum = (int) atoi(temp);
        hour = tempnum / 100;
        min = tempnum % 100;
        saturdayTime[i][0] = hour;
        saturdayTime[i][1] = min;
    }

    for(int i = 0; i < SATURDAY_SIZE_AND_HOLIDAY_SIZE; i++)
    {
        temp[0] = holiday[i]._time[0];
        temp[1] = holiday[i]._time[1];
        temp[2] = holiday[i]._time[3];
        temp[3] = holiday[i]._time[4];
        tempnum = (int) atoi(temp);
        hour = tempnum / 100;
        min = tempnum % 100;
        holidayTime[i][0] = hour;
        holidayTime[i][1] = min;
    }
}

char* FindStartTimeWeekday(int startStationNum, int destStationNum)
{
    static char startTime[6];
    int tempI;

    if(startStationNum == destStationNum)
        return NULL;

    for(int i = 0; i < WEEKDAY_SIZE; i++)
    {
        if(currentTime -> tm_hour > weekdayTime[i][0])
            continue;
        else if(currentTime -> tm_hour == weekdayTime[i][0])
        {
            if(currentTime -> tm_min > weekdayTime[i][1])
                continue;
            else
            {
                tempI = i;
                if(destStationNum == 709)
                {
                    if(strcmp(weekday[tempI].destination, "Jangam") == 0)
                    {
                        strcpy(startTime, weekday[tempI]._time);
                        break;
                    }
                }

                else if(destStationNum >= 710 && destStationNum <= 715)
                {
                    if(strcmp(weekday[tempI].destination, "Jangam") == 0 || strcmp(weekday[tempI].destination, "Dobongsan") == 0)
                    {
                        strcpy(startTime, weekday[tempI]._time);
                        break;
                    }
                }
                        

                else if(destStationNum >= 717 && destStationNum <= 750)
                {
                    if(strcmp(weekday[tempI].destination, "Onsu") == 0 || strcmp(weekday[tempI].destination, "Bupyeong-gu_Office") == 0)
                    {   
                        strcpy(startTime, weekday[tempI]._time);
                        break;
                    }
                }

                else
                {
                    if(strcmp(weekday[tempI].destination, "Bupyeong-gu_Office") == 0)
                    {
                        strcpy(startTime, weekday[tempI]._time);
                        break;
                    }
                }
                        
            }
                    
        }
        else
        {
            tempI = i;
            if(destStationNum == 709)
            {
                if(strcmp(weekday[tempI].destination, "Jangam") == 0)
                {
                    strcpy(startTime, weekday[tempI]._time);
                    break;
                }
            }

            else if(destStationNum >= 710 && destStationNum <= 715)
            {
                if(strcmp(weekday[tempI].destination, "Jangam") == 0 || strcmp(weekday[tempI].destination, "Dobongsan") == 0)
                {
                    strcpy(startTime, weekday[tempI]._time);
                    break;
                }
            }
                        

            else if(destStationNum >= 717 && destStationNum <= 750)
            {
                if(strcmp(weekday[tempI].destination, "Onsu") == 0 || strcmp(weekday[tempI].destination, "Bupyeong-gu_Office") == 0)
                {   
                    strcpy(startTime, weekday[tempI]._time);
                    break;
                }
            }

            else
            {
                if(strcmp(weekday[tempI].destination, "Bupyeong-gu_Office") == 0)
                {
                        strcpy(startTime, weekday[tempI]._time);
                        break;
                }
            }
                    
        }
    }

    return startTime;
}

char* FindStartTimeSaturday(int startStationNum, int destStationNum)
{
    static char startTime[6];
    int tempI;

    if(startStationNum == destStationNum)
        return NULL;

    for(int i = 0; i < SATURDAY_SIZE_AND_HOLIDAY_SIZE; i++)
    {
        if(currentTime -> tm_hour > saturdayTime[i][0])
            continue;
        else if(currentTime -> tm_hour == saturdayTime[i][0])
        {
            if(currentTime -> tm_min > saturdayTime[i][1])
                continue;
            else
            {
                tempI = i;
                if(destStationNum == 709)
                {
                    if(strcmp(saturday[tempI].destination, "Jangam") == 0)
                    {
                        strcpy(startTime, saturday[tempI]._time);
                        break;
                    }
                }

                else if(destStationNum >= 710 && destStationNum <= 715)
                {
                    if(strcmp(saturday[tempI].destination, "Jangam") == 0 || strcmp(saturday[tempI].destination, "Dobongsan") == 0)
                    {
                        strcpy(startTime, saturday[tempI]._time);
                        break;
                    }
                }
                        

                else if(destStationNum >= 717 && destStationNum <= 750)
                {
                    if(strcmp(saturday[tempI].destination, "Onsu") == 0 || strcmp(saturday[tempI].destination, "Bupyeong-gu_Office") == 0)
                    {   
                        strcpy(startTime, saturday[tempI]._time);
                        break;
                    }
                }

                else
                {
                    if(strcmp(saturday[tempI].destination, "Bupyeong-gu_Office") == 0)
                    {
                        strcpy(startTime, saturday[tempI]._time);
                        break;
                    }
                }
                        
            }
                    
        }
        else
        {
            tempI = i;
            if(destStationNum == 709)
            {
                if(strcmp(saturday[tempI].destination, "Jangam") == 0)
                {
                    strcpy(startTime, saturday[tempI]._time);
                    break;
                }
            }

            else if(destStationNum >= 710 && destStationNum <= 715)
            {
                if(strcmp(saturday[tempI].destination, "Jangam") == 0 || strcmp(saturday[tempI].destination, "Dobongsan") == 0)
                {
                    strcpy(startTime, saturday[tempI]._time);
                    break;
                }
            }
                        

            else if(destStationNum >= 717 && destStationNum <= 750)
            {
                if(strcmp(saturday[tempI].destination, "Onsu") == 0 || strcmp(saturday[tempI].destination, "Bupyeong-gu_Office") == 0)
                {   
                    strcpy(startTime, saturday[tempI]._time);
                    break;
                }
            }

            else
            {
                if(strcmp(saturday[tempI].destination, "Bupyeong-gu_Office") == 0)
                {
                        strcpy(startTime, saturday[tempI]._time);
                        break;
                }
            }
                    
        }
    }

    return startTime;
}

char* FindStartTimeHoliday(int startStationNum, int destStationNum)
{
    static char startTime[6];
    int tempI;

    if(startStationNum == destStationNum)
        return NULL;

    for(int i = 0; i < SATURDAY_SIZE_AND_HOLIDAY_SIZE; i++)
    {
        if(currentTime -> tm_hour > holidayTime[i][0])
            continue;
        else if(currentTime -> tm_hour == holidayTime[i][0])
        {
            if(currentTime -> tm_min > holidayTime[i][1])
                continue;
            else
            {
                tempI = i;
                if(destStationNum == 709)
                {
                    if(strcmp(holiday[tempI].destination, "Jangam") == 0)
                    {
                        strcpy(startTime, holiday[tempI]._time);
                        break;
                    }
                }

                else if(destStationNum >= 710 && destStationNum <= 715)
                {
                    if(strcmp(holiday[tempI].destination, "Jangam") == 0 || strcmp(holiday[tempI].destination, "Dobongsan") == 0)
                    {
                        strcpy(startTime, holiday[tempI]._time);
                        break;
                    }
                }
                        

                else if(destStationNum >= 717 && destStationNum <= 750)
                {
                    if(strcmp(holiday[tempI].destination, "Onsu") == 0 || strcmp(holiday[tempI].destination, "Bupyeong-gu_Office") == 0)
                    {   
                        strcpy(startTime, holiday[tempI]._time);
                        break;
                    }
                }

                else
                {
                    if(strcmp(holiday[tempI].destination, "Bupyeong-gu_Office") == 0)
                    {
                        strcpy(startTime, holiday[tempI]._time);
                        break;
                    }
                }
                        
            }
                    
        }
        else
        {
            tempI = i;
            if(destStationNum == 709)
            {
                if(strcmp(holiday[tempI].destination, "Jangam") == 0)
                {
                    strcpy(startTime, holiday[tempI]._time);
                    break;
                }
            }

            else if(destStationNum >= 710 && destStationNum <= 715)
            {
                if(strcmp(holiday[tempI].destination, "Jangam") == 0 || strcmp(holiday[tempI].destination, "Dobongsan") == 0)
                {
                    strcpy(startTime, holiday[tempI]._time);
                    break;
                }
            }
                        

            else if(destStationNum >= 717 && destStationNum <= 750)
            {
                if(strcmp(holiday[tempI].destination, "Onsu") == 0 || strcmp(holiday[tempI].destination, "Bupyeong-gu_Office") == 0)
                {   
                    strcpy(startTime, holiday[tempI]._time);
                    break;
                }
            }

            else
            {
                if(strcmp(holiday[tempI].destination, "Bupyeong-gu_Office") == 0)
                {
                        strcpy(startTime, holiday[tempI]._time);
                        break;
                }
            }
                    
        }
    }

    return startTime;
}


int CompareBetweenTimetableAndCurrentTime(char* _destination)
{
    int startStationNum;
    int destStationNum;
    
    char startTime[6];
    char destination[50];
    int tempI;
    startStationNum = ReturnStationNumber(startStation);
    destStationNum = ReturnStationNumber(_destination);

    printf("%d\n", destStationNum);


    if(currentTime -> tm_wday >= 1 && currentTime -> tm_wday <= 5)
    {
        strcpy(startTime, FindStartTimeWeekday(startStationNum, destStationNum));
    }

    else if(currentTime -> tm_wday == 6)
    {
        strcpy(startTime, FirstStartTimeSaturday(startStationNum, destStationNum));
    }

    else
    {
        strcpy(startTime, FindStartTimeHoliday(startStationNum, destStationNum));
    }

    printf("%s\n", startTime);
    return 0;
}    