#include <stdio.h>

int main(){
    int year;
    scanf("%d", &year);
    if (year % 4 == 0){
        if (year % 100 == 0) {
            if (year % 400 == 0){
                printf("%d was a leap year", year);
            }
            else{
               printf("%d was not a leap year", year);
            }
        }
        else{
            printf("%d was a leap year", year);
        }
    }
    else{
        printf("%d was not a leap year", year);
    }
}
