typedef struct {
    char *name; // does this pointer cause problem with backtracking and modifing?
    char day;
    int start;
    int end; 
} section;

typedef section course[20];

typedef struct{
    section timeSlots[50];
    course toBeSch[7]; // this being a pointer could mess up recursive call in last algorithm
    int idleTime;
}schedule;
