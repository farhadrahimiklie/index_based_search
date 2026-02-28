#include <stdio.h>
#include <string.h>

typedef struct {
    int id;
    char name[32];
    float score;
}Student;

typedef struct {
    int id;
    long offset;
}IndexEntry;

void insert_student(int id, char name[], float score){
    FILE* data = fopen("data.bin", "ab");
    FILE* index = fopen("index.bin", "ab");

    if(data == NULL && index == NULL){
        perror("Cannot open file for append data");
        return;
    }

    Student s;
    IndexEntry ie;

    s.id = id;
    strcpy(s.name, name);
    s.score = score;

    ie.id = id;
    ie.offset = ftell(data);

    fwrite(&s, sizeof(s), 1, data);
    fwrite(&ie, sizeof(ie), 1, index);

    fclose(data);
    fclose(index);
}

int update_score(int id, float new_score){
    FILE *data = fopen("data.bin", "rb+");
    FILE *index = fopen("index.bin", "rb");

    IndexEntry ie;
    Student s;

    while(fread(&ie, sizeof(ie), 1, index) == 1){
        if(ie.id == id){
            fseek(data, ie.offset, SEEK_SET);
            fread(&s, sizeof(s), 1, data);

            s.score = new_score;

            fseek(data, ie.offset, SEEK_SET);
            fwrite(&s, sizeof(s), 1, data);

            fclose(data);
            fclose(index);
            return 1;
        }
    }
    fclose(index);
    fclose(data);
    return 0;
}

int find_student(int id, Student* student_struct){
    FILE* data = fopen("data.bin", "rb");
    FILE* index = fopen("index.bin", "rb");

    if(data == NULL && index == NULL){
        perror("Cannot open file for Reading data");
        return 1;
    }

    IndexEntry ie;

    while(fread(&ie, sizeof(ie), 1, index) == 1){
        if(ie.id == id){
            fseek(data, ie.offset, SEEK_SET);
            fread(student_struct, sizeof(*student_struct), 1, data);
            fclose(data);
            fclose(index);
            return 1;
        }
    }
    fclose(data);
    fclose(index);
    return 0;
}


int main(){

    insert_student(1, "farhad klie", 4.78);
    insert_student(2, "shaher ishan", 123.5);
    insert_student(3, "janan", 12.7);
    insert_student(4, "khalid", 98.5);
    insert_student(5, "almas", 5.9);

    Student student; // struct student is here.
    if(find_student(3, &student)){
        printf("ID: %d, Name: %s, Score: %.2f \n", student.id, student.name, student.score);
    }

    // update_score(3, 100.5);

    // if(find_student(3, &student)){
       // printf("Score Updated Successfully . %.2f \n", student.score);
    //}

    return 0;
}
