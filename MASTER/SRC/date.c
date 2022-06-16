#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../HEADERS/date.h"



//Periptwseis
//an exitdate einai -, return -1
//date2>date1 , return -1
//date1>date2, return 1
//date1=date2, return 0
int compare_dates (char* date1, char* date2){

    char* tmpdate1 = strdup(date1);
    char* tmpdate2 = strdup(date2);
    const char s[3] = "-";
    char    *token1,*token2;
    int day1,day2,month1,month2,year1,year2;
    
    if(strcmp(date2,s) == 0)      //periptwsh pou exitdate einai -, ara einai egkiri
    {  
        free(tmpdate1);
        free(tmpdate2);
        return -1; 
    }

//Kommatiazoume ta dates sumfwna me DD-MM-YYYY
    token1 = strtok(tmpdate1,s);
    day1= atoi(token1);
    
    token1 = strtok(NULL,s);
    month1 = atoi(token1);

    token1 = strtok(NULL,s);
    year1 = atoi(token1);


    token2= strtok(tmpdate2,s);
    day2=atoi(token2);
 
    token2 = strtok(NULL,s);
    month2=atoi(token2);

    token2 = strtok(NULL,s);
    year2 = atoi(token2);

    free(tmpdate1);
    free(tmpdate2);

    if (year1 < year2)
        return -1;     //date2 einai megalutero
    else if (year1 > year2)
        return 1;    //date1 einai megalutero
    if (year1 == year2)
    {
        if (month1<month2)
            return -1;
        else if (month1>month2)
            return 1;
        else if(month1 == month2)
        {     
            if (day1<day2)
                return -1;
            else if(day1>day2)
                return 1;
            else
                return 0;  //idies
        }
    }
        printf("exei bgei eksw");//periptwsh lathous
        return 0;
}
