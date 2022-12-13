#include <stdarg.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void debug(int rank, char *format, ...){

    va_list args; /*Variable argument list*/

    /*  Initialize variable argument list;
    `format` is last argument before ...
    */
    time_t t = time(NULL);  
    struct tm * timeinfo;
    timeinfo =  localtime(&t);

    va_start(args, format);

    printf("%s |%2d|", asctime(timeinfo) , rank);
    vprintf(format, args);

    /*   Clean up variable argument list*/
    va_end(args);
}