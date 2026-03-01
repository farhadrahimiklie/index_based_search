#include <stdio.h>
#include <string.h>

// Students Structure
typedef struct {
    int id;
    char name[32];
    float score;
    char deleted; // 0 active 1 delete
}Student;

// Index for Search the student structrue
typedef struct {
    int id;
    long offset;
}Index;

void Insert_data(int id, const char* name, float score){
    FILE *data = fopen("data.bin", "ab");
    FILE *index = fopen("index.bin", "ab");
    if(data == NULL && index == NULL){
        perror("Cannot open for append data inside files. ");
        return;
    }
    Student student;
    Index indexEntry;

    student.id = id;
    strcpy(student.name, name);
    student.score = score;

    indexEntry.id = id;
    indexEntry.offset = ftell(data);

    fwrite(&student, sizeof(student), 1, data);
    fwrite(&indexEntry, sizeof(indexEntry), 1, index);
    fclose(data);
    fclose(index);
}

int update_student(int id, const char* new_name){
    FILE *data = fopen("data.bin", "rb+");
    FILE *index = fopen("index.bin", "rb+");
    
    if(data == NULL && index == NULL){
        perror("Cannot open for reading data inside files. ");
        return 1;
    }
    Student student;
    Index Entry;

    while(fread(&Entry, sizeof(Entry), 1, index) == 1){
        if(Entry.id == id){
            fseek(data, Entry.offset, SEEK_SET);
            fread(&student, sizeof(student), 1, data);
            strcpy(student.name, new_name);
            fseek(data, Entry.offset, SEEK_SET);
            fwrite(&student, sizeof(student), 1, data);

            fclose(data);
            fclose(index);
            return 1;
        }
    }
    fclose(data);
    fclose(index);
    return 0;
}

int delete_student(int id) {
    FILE *data = fopen("data.bin", "rb+");
    FILE *index = fopen("index.bin", "rb");

    Index ie;
    Student s;

    while (fread(&ie, sizeof(ie), 1, index) == 1) {
       if (ie.id == id) {
          fseek(data, ie.offset, SEEK_SET);
          fread(&s, sizeof(s), 1, data);

          s.deleted = 1;

          fseek(data, ie.offset, SEEK_SET);
          fwrite(&s, sizeof(s), 1, data);

          fclose(data);
          fclose(index);
          return 1;                              
      }
     }

         fclose(data);
         fclose(index);
         return 0;
}

int find_student(int id, Student *student){
    FILE *data = fopen("data.bin", "rb");
    FILE *index = fopen("index.bin", "rb");
    
    if(data == NULL && index == NULL){
        perror("Cannot open for reading data inside files. ");
        return 1;
    }

    Index Entry;
    while(fread(&Entry, sizeof(Entry), 1, index) == 1){
        if(Entry.id == id){
            fseek(data, Entry.offset,SEEK_SET);
            fread(student, sizeof(*student), 1, data);
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
    Student student;
    //Insert_data(1, "Farhad Rahimi Klie", 12.6);
    //Insert_data(2, "Hashmatullah Ishan", 18.7);
    //Insert_data(3, "Shaher Ishan", 14.6);
    //Insert_data(4, "Nouman Aziz", 10.1);
    //Insert_data(5, "Rustam Hayat", 2.65);

    delete_student(1);

    if(find_student(2, &student) && !student.deleted){
        printf("ID: %d, Name: %s, Score: %.2f \n", student.id, student.name, student.score);
    }

    //update_student(1, "Farhad Rahimi Klie Khan");
    return 0;
}
