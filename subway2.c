#include "subway.h"

int simulationCount;
int stationCount = 0;
char destinations[100][50];
char dest[50] = { 0 }; 
char* const startStation = "Gongneung";
struct tm* currentTime = NULL;

int weekdayTime[WEEKDAY_SIZE][2];
int saturdayTime[SATURDAY_SIZE_AND_HOLIDAY_SIZE][2];
int holidayTime[SATURDAY_SIZE_AND_HOLIDAY_SIZE][2];

Timetable weekday[WEEKDAY_SIZE];
Timetable saturday[SATURDAY_SIZE_AND_HOLIDAY_SIZE];
Timetable holiday[SATURDAY_SIZE_AND_HOLIDAY_SIZE];

Station stations[STATION_INTERVAL_SIZE];

const int numberOfJangam = 709;
int startHour;
int startMinute;

int main(void)
{
    char yesOrNo;
    while(1)
    {
        printf("시뮬레이션을 시작하시겠습니까?");
        scanf("%c", &yesOrNo);
        while(getchar() != '\n');

        switch(yesOrNo)
        {
            case 'Y':
            case 'y':
                break;
            case 'N':
            case 'n':
                return 0;
                break;
            default:
                printf("Y(y) 또는 N(n)으로 입력해주세요\n");
                continue;
                break;
        }
    }

    ChooseCountOfStation();
    LoadTimeTable();
    LoadStationInterval();   
    currentTime = FindCurrentTimeByTm();
    ChangeTimeFromCharToInt();
    for(int i = 0; i < simulationCount; i++)
    {
        PrintDurationOfTime(destinations[i]);
    }
    return 0;
}

void ChooseCountOfStation()
{     
     printf("몇 개의 역을 시뮬레이션하시겠습니까?");     
     scanf("%d", simulationCount);     
     ChooseDestinationStation(); 
}

void ChooseDestinationStation() 
{ 	
    int check; 	 	
    
    while(stationCount != simulationCount)
    {
        printf("도착할 역 이름을 영어로 입력해주세요. : "); 	
        scanf("%s", dest);
        while (getchar() != '\n');   	
        for (int i = 0; i < 50; i++) { 		
            if (dest[i] >= 'A' || dest[i] <= 'Z' || dest[i] >= 'a' || dest[i] <= 'z') 			
                check = 0; 		
            else 			
                check = 1; 	
        }

        if (check == 0) 	
        { 		
            strcpy(destinations[stationCount++], dest); 	
        } 
    }
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
    int i;
    FILE *fpWeekday = NULL;
    FILE *fpSaturday = NULL;
    FILE *fpHoliday = NULL;

    fpWeekday = fopen("train_table_Weekday.txt", "r");
    fpSaturday = fopen("train_table_Saturday.txt", "r");
    fpHoliday = fopen("train_table_Holiday.txt", "r");
    
    for (i = 0; i < 397; i++)
    {
        fscanf(fpWeekday, "%s %s", weekday[i]._time, weekday[i].destination);
    }

    for (i = 0; i < 343; i++)
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
    int i;
    FILE *fpInterval = NULL;
    fpInterval = fopen("station_interval.txt", "r");

    for(i = 0; i < 51; i++)
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

int FindStartTimeWeekdayIndex(int startStationNum, int destStationNum)
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
                if(destStationNum == 709)
                {
                    if(strcmp(weekday[i].destination, "Jangam") == 0)
                    {
                        return i;
                    }
                }

                else if(destStationNum >= 710 && destStationNum <= 715)
                {
                    if(strcmp(weekday[i].destination, "Jangam") == 0 || strcmp(weekday[i].destination, "Dobongsan") == 0)
                    {
                        return i;
                    }
                }
                        

                else if(destStationNum >= 717 && destStationNum <= 750)
                {
                    if(strcmp(weekday[i].destination, "Onsu") == 0 || strcmp(weekday[i].destination, "Bupyeong-gu_Office") == 0)
                    {   
                        return i;
                    }
                }

                else
                {
                    if(strcmp(weekday[i].destination, "Bupyeong-gu_Office") == 0)
                    {
                        return i;
                    }
                }
                        
            }
                    
        }
        else
        {
            if(destStationNum == 709)
            {
                if(strcmp(weekday[i].destination, "Jangam") == 0)
                {
                    return i;
                }
            }

            else if(destStationNum >= 710 && destStationNum <= 715)
            {
                if(strcmp(weekday[i].destination, "Jangam") == 0 || strcmp(weekday[i].destination, "Dobongsan") == 0)
                {
                    return i;
                }
            }
                        

            else if(destStationNum >= 717 && destStationNum <= 750)
            {
                if(strcmp(weekday[i].destination, "Onsu") == 0 || strcmp(weekday[i].destination, "Bupyeong-gu_Office") == 0)
                {   
                    return i;
                }
            }

            else
            {
                if(strcmp(weekday[i].destination, "Bupyeong-gu_Office") == 0)
                {
                    return i;
                }
            }
                    
        }
    }
}

int FindStartTimeSaturdayIndex(int startStationNum, int destStationNum)
{
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
                if(destStationNum == 709)
                {
                    if(strcmp(saturday[i].destination, "Jangam") == 0)
                    {
                        return i;
                    }
                }

                else if(destStationNum >= 710 && destStationNum <= 715)
                {
                    if(strcmp(saturday[i].destination, "Jangam") == 0 || strcmp(saturday[i].destination, "Dobongsan") == 0)
                    {
                        return i;
                    }
                }
                        

                else if(destStationNum >= 717 && destStationNum <= 750)
                {
                    if(strcmp(saturday[i].destination, "Onsu") == 0 || strcmp(saturday[i].destination, "Bupyeong-gu_Office") == 0)
                    {   
                        return i;
                    }
                }

                else
                {
                    if(strcmp(saturday[i].destination, "Bupyeong-gu_Office") == 0)
                    {
                        return i;
                    }
                }
                        
            }
                    
        }
        else
        {
            if(destStationNum == 709)
            {
                if(strcmp(saturday[i].destination, "Jangam") == 0)
                {
                    return i;
                }
            }

            else if(destStationNum >= 710 && destStationNum <= 715)
            {
                if(strcmp(saturday[i].destination, "Jangam") == 0 || strcmp(saturday[i].destination, "Dobongsan") == 0)
                {
                    return i;
                }
            }
                        

            else if(destStationNum >= 717 && destStationNum <= 750)
            {
                if(strcmp(saturday[i].destination, "Onsu") == 0 || strcmp(saturday[i].destination, "Bupyeong-gu_Office") == 0)
                {   
                    return i;
                }
            }

            else
            {
                if(strcmp(saturday[i].destination, "Bupyeong-gu_Office") == 0)
                {
                    return i;
                }
            }
                    
        }
    }
}

int FindStartTimeHolidayIndex(int startStationNum, int destStationNum)
{
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
                if(destStationNum == 709)
                {
                    if(strcmp(holiday[i].destination, "Jangam") == 0)
                    {
                        return i;
                    }
                }

                else if(destStationNum >= 710 && destStationNum <= 715)
                {
                    if(strcmp(holiday[i].destination, "Jangam") == 0 || strcmp(holiday[i].destination, "Dobongsan") == 0)
                    {
                        return i;
                    }
                }
                        

                else if(destStationNum >= 717 && destStationNum <= 750)
                {
                    if(strcmp(holiday[i].destination, "Onsu") == 0 || strcmp(holiday[i].destination, "Bupyeong-gu_Office") == 0)
                    {   
                        return i;
                    }
                }

                else
                {
                    if(strcmp(holiday[i].destination, "Bupyeong-gu_Office") == 0)
                    {
                        return i;
                    }
                }
                        
            }
                    
        }
        else
        {
            if(destStationNum == 709)
            {
                if(strcmp(holiday[i].destination, "Jangam") == 0)
                {
                    return i;
                }
            }

            else if(destStationNum >= 710 && destStationNum <= 715)
            {
                if(strcmp(holiday[i].destination, "Jangam") == 0 || strcmp(holiday[i].destination, "Dobongsan") == 0)
                {
                    return i;
                }
            }
                        

            else if(destStationNum >= 717 && destStationNum <= 750)
            {
                if(strcmp(holiday[i].destination, "Onsu") == 0 || strcmp(holiday[i].destination, "Bupyeong-gu_Office") == 0)
                {   
                    return i;
                }
            }

            else
            {
                if(strcmp(holiday[i].destination, "Bupyeong-gu_Office") == 0)
                {
                    return i;
                }
            }
                    
        }
    }
}


void PrintDurationOfTime(char* _destination)
{
    int startStationNum;
    int destStationNum;
    int timeInterval;

    int destHour;
    int destMin;
    
    int startTimeIndex;

    int tempI;
    startStationNum = ReturnStationNumber(startStation);
    destStationNum = ReturnStationNumber(_destination);

    timeInterval = stations[destStationNum - numberOfJangam].intervalFromGongneung;

    if(currentTime -> tm_wday >= 1 && currentTime -> tm_wday <= 5)
    {
        startTimeIndex = FindStartTimeWeekdayIndex(startStationNum, destStationNum);
        printf("DepartureTime: %s\n", weekday[startTimeIndex]._time);
        destHour = weekdayTime[startTimeIndex][0];
        destMin = weekdayTime[startTimeIndex][1] + timeInterval;
    }

    else if(currentTime -> tm_wday == 6)
    {
        startTimeIndex = FindStartTimeSaturdayIndex(startStationNum, destStationNum);
        printf("DepartureTime: %s\n", saturday[startTimeIndex]._time);
        destHour = saturdayTime[startTimeIndex][0];
        destMin = saturdayTime[startTimeIndex][1] + timeInterval;
    }

    else
    {
        startTimeIndex = FindStartTimeHolidayIndex(startStationNum, destStationNum);
        printf("DepartureTime: %s\n", holiday[startTimeIndex]._time);
        destHour = holidayTime[startTimeIndex][0];
        destMin = holidayTime[startTimeIndex][1] + timeInterval;
    }

    if(destMin >= 60 && destMin <= 120)
    {
        destMin -= 60;
        destHour += 1;
    }

    if(destMin >= 120)
    {
        destMin -= 120;
        destHour += 2;
    }

    printf("TimeInterval: %d\n", timeInterval);
    printf("ArrivalTime: %d:%d\n", destHour, destMin);
}  