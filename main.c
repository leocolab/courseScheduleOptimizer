#include "calculateSchedule.c"
#include <stdio.h>
#include <stdlib.h>

int main() {
    
    int numCourses;
    printf("How many courses are you enrolling in for this semester?\n");
    scanf("%d", &numCourses);
    
    course *courses = malloc(sizeof(course)*numCourses);

    printf("**INSTRUCTIONS**\n");
    printf("For each availible section, enter on seperate lines:\nThe name of the course\nThe day of the week (m, t, w, T, f)\nThe start time in 24 hour format (ex. for 1:30pm enter 1330\nThe end time in 24 hour format\n");
    printf("Enter 'NEXT' once all sections of a course have been entered to move onto the next course\n");
    for (int i=0; i<numCourses; i++){
        course *theCourse = malloc(sizeof(course));
        theCourse = course_ini(theCourse);
        char name[50] = "null";
        char day;
        int start;
        int end;
        int index = 0;
        while (1){
            scanf("%s", name);
            if (strcmp(name, "NEXT") == 0) break;
            scanf(" %c", &day);
            scanf("%d", &start);
            scanf("%d", &end);
            section aSection = section_ini(name, day, start, end);
            printf("try adding section to course\n");
            *theCourse[index] = aSection;
            printf("success in adding section to course\n");
            index++;
        }
        printf("try adding course to courses array\n");
        memcpy(courses[i], *theCourse, sizeof(course));
        printf("success in adding course to courses arr\n");

    }

    schedule *bestSchedule = malloc(sizeof(schedule));
    bestSchedule = schedule_ini(bestSchedule, courses, numCourses);
    printf("main.1\n");

    schedule *currSchedule = malloc(sizeof(schedule));
    currSchedule = schedule_ini(bestSchedule, courses, numCourses);
    printf("main.2\n");

    *bestSchedule = globallyOptimalSchedule(numCourses, *currSchedule, bestSchedule);
    printf("main.3\n");

    print_schedule(*bestSchedule);
    printf("main.4\n");

    return 0;
}