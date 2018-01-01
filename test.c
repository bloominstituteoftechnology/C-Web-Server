#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *find_end_of_header(char *header)
{
    char *p;

    p = strstr(header, "\n\n");

    if (p != NULL) return p;

    p = strstr(header, "\r\n\r\n");

    if (p != NULL) return p;

    p = strstr(header, "\r\r");

    return p;
}

int main () {
//    int day, year;
//    char weekday[20], month[20], dtm[100];

//    strcpy( dtm, "Saturday March 25 1989" );
//    sscanf( dtm, "%s %s %d  %d", weekday, month, &day, &year );

//    printf("%s %d, %d = %s\n", month, day, year, weekday );

    // char test[100] = "hello I am neil";
    // char str1[40], str2[40], str3[40], str4[40];

    // sscanf(test, "%s %s %s %s", str1, str2, str3, str4);

    // printf("%s\n", str1);
    // printf("%s\n", str2);
    // printf("%s\n", str3);
    // printf("%s\n", str4);

    // char str[90] = "hello \r\n";
    // char find[20];

    // sscanf(str, "%s %s", find);
    // printf("%s\n", find);
    // srand(time(NULL));
    // int r = rand() % 21; 
    // char str[10];
    // sprintf(str, "%d", r);

    // printf("%s\n", str);

    
    
   return(0);
}