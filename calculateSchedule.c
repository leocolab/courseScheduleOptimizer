#include "calculateSchedule.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

section section_ini(char* name, char day, int start, int end){
    section aSection;
    aSection.name = malloc(sizeof(char)*strlen(name)+1);
    strcpy(aSection.name, name);
    aSection.day = day;
    aSection.start = start;
    aSection.end = end;
    return aSection;
}

course *course_ini(course *aCourse){
    for (int i=0; i<20; i++) {
        section emptySection = {NULL, '\0', 0, 0};
        (*aCourse)[i] = emptySection;
    }
    return aCourse;
}

schedule *schedule_ini(schedule *aSch, course *courses, int numCourses){
    for (int i=0; i<50; i++) {
        section emptySection = {NULL, '\0', 0, 0};
        aSch->timeSlots[i] = emptySection;
    }
    aSch->idleTime = INT_MAX; // might need to init this different perhaps infitie?
    for (int i=0; i<numCourses; i++){
        memcpy(&(aSch->toBeSch[i]), courses + i, sizeof(course));
    }
    return aSch;
}

int isEmpty(section aSection){
    if (aSection.name == NULL) return 1;
    return 0;
}

int isEmpty_course(course aCourse){
    for (int i=0; i<20; i++){
        if (aCourse[i].name != NULL) return 0;
    }
    return 1;
}

int num_coursesScheduled(schedule aSchedule, int numCourses){
    int num_courses_in_curr = 0;
    for (int i=0; i<numCourses; i++){
        if (isEmpty_course(aSchedule.toBeSch[i])) num_courses_in_curr++;
    }
    return num_courses_in_curr;
}

int conflicts(schedule currSchedule, section currSection){
    for (int i=0; i <50; i++){
        section schSection = currSchedule.timeSlots[i];
        if (isEmpty(schSection)) break;
        if (schSection.day == currSection.day){ // chat gtp to make sure the conditions cover everything
            if (currSection.start < schSection.start && schSection.start < currSection.end || currSection.start < schSection.end && schSection.end < currSection.end ){
                printf("conflict 1\n");
                return 1;
            }
            if (schSection.start < currSection.start && currSection.start < schSection.end || schSection.start < currSection.end && currSection.end < schSection.end ){
                printf("conflict 2\n");
                return 1;
            }
            if (schSection.start == currSection.start && schSection.end == currSection.end){
                return 1;
            }
        }
    }
    return 0;
}

int dayIdleTime(schedule sch, char day){
    int total = 0;
    section dayArr[10];
    for (int i=0; i<10; i++){
        section emptySection = {NULL, '\0', 0, 0};
        dayArr[i] = emptySection;
    }
    for (int i=0; i<50; i++){
        section class = sch.timeSlots[i];
        if (isEmpty(class)) break;
        if (class.day == day){ // add all to classes in day to dayArr
            for (int i=0; i<10; i++){
                if (isEmpty(dayArr[i])){
                    dayArr[i] = class;
                    break;
                }
            }
        }
    }
    for (int i=0; i<10; i++){
        section curr = dayArr[i];
        int smallestGap = 100000;
        for (int j=0; j<10; j++){
            section other = dayArr[j];
            if (isEmpty(other)) break;
            if (curr.start == other.start && curr.end == other.end) continue;
            int gap = curr.start - other.end;
            if (gap < smallestGap && gap > 0) smallestGap = gap;
        }
        if (smallestGap != 100000) total += smallestGap;
    }
    return total;
}

int totalIdleTime(schedule sch){
    return dayIdleTime(sch, 'm') + dayIdleTime(sch, 't') + dayIdleTime(sch, 'w') + dayIdleTime(sch, 'T') + dayIdleTime(sch, 'f');
}

void free_section(section aSection){
    if(aSection.name != NULL){
        free(aSection.name);
    }
}

schedule globallyOptimalSchedule(int numCourses, schedule currSchedule, schedule *bestSchedule){
    if (numCourses == num_coursesScheduled(currSchedule, numCourses)){
        printf("reached base case\n");
        if (currSchedule.idleTime < bestSchedule->idleTime) *bestSchedule = currSchedule;
    }
    else {
        for (int i=0; i<numCourses; i++){
            printf("1\n");
            if (isEmpty_course(currSchedule.toBeSch[i])) continue;
            course currCourse;
            memcpy(currCourse, currSchedule.toBeSch[i], sizeof(course));
            printf("2\n");
            for (int j=0; j<20; j++){
                if (isEmpty(currCourse[j])) break; // you have reached an empty course/ you have dealt with all courses in array
                section currSection = currCourse[j];
                if (!conflicts(currSchedule, currSection)){ 
                    printf("3\n");
                    int index = 0;
                    for (int k=0; k<50; k++){ // add section time to curr schedule and remove course from course arr
                        if (isEmpty(currSchedule.timeSlots[k])){
                            currSchedule.timeSlots[k] = currSection;
                            memcpy(currSchedule.toBeSch[i], *course_ini(currSchedule.toBeSch[i]), sizeof(course)); // sets the course to be scheduled as empty in the tobSch array is this right?
                            currSchedule.idleTime = totalIdleTime(currSchedule);
                            index = k;
                            break;
                        }
                    }
                    printf("4\n");

                    *bestSchedule = globallyOptimalSchedule(numCourses, currSchedule, bestSchedule);
                    print_schedule(*bestSchedule);
                    
                    // remove section time from curr schedule and add back course to courses arr
                    section emptySection = {NULL, '\0', 0, 0};
                    currSchedule.timeSlots[index] = emptySection; // the memcpy line (below) good be adding back section.name as deleted/NULL
                    memcpy(currSchedule.toBeSch[i], currCourse, sizeof(course)); // adds back the current course to the to be scheduled list
                    currSchedule.idleTime = totalIdleTime(currSchedule);

                }
            }
        }
    }
    return *bestSchedule;
}

int print_schedule(schedule sch){
    printf("Total time between classes: %d\n", sch.idleTime);

    for (int i=0; i<50; i++){
        section aSection = sch.timeSlots[i];
        printf("reached empty section\n");
        if (isEmpty(aSection)) break;
        printf("%s %c %d %d\n", aSection.name, aSection.day, aSection.start, aSection.end);
    }
    return 0;
}
