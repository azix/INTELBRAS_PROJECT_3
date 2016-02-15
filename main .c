#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

#define ESC 27
#define MAX 100


//Declaration et Definition de la structure students
typedef struct _Students{
    int id_student;
    char First_name[MAX];
    char Last_name[MAX];
    int Score_math;
    int Score_science;
    struct _Students *Next;
}Students;

void show_Students(Students* ListStudents){
    Students* stud = NULL;
    if(!ListStudents)
        printf("there is no student. the list is empty. !!!");
    else
        for(stud = ListStudents ; stud != NULL ; stud = stud->Next){ //parcours la liste des etudiants
            printf("Id student : %d \n",stud->id_student);
            printf("Fisrt name : %s     ||",stud->First_name);
            printf("Last name : %s      ||",stud->Last_name);
            printf("score math : %d     ||",stud->Score_math);
            printf("score science : %d     ||",stud->Score_science);
            printf("\n\n");
        }
    return;
}

void show_student_ById(Students* ListStudents, int Reference){
    Students* stud = NULL;
    if(!ListStudents)
        printf("the list of students is empty ,  !!!");
    else
        for(stud = ListStudents ; stud != NULL ; stud = stud->Next)
            if(stud->id_student == Reference){
                printf("Id student : %d \n",stud->id_student);
                printf("Fisrt name : %s     ||",stud->First_name);
                printf("Last name : %s      ||",stud->Last_name);
                printf("score math : %d     ||",stud->Score_math);
                printf("score science : %d     ||",stud->Score_science);
                printf("\n\n");
                return;
            }
    printf("there is no student with this id");
    return;
}

Students* add_Student_ListStudents(Students* ListStudents, int id_Student, const char* first_name, const char* last_name, int score_math , int score_science){
    Students* stud = NULL;
    Students* Elem = NULL;

    Elem = malloc(sizeof(Students));
    if(!Elem){
        printf("Allocation Error.");
        return NULL;
    }
    stud = malloc(sizeof(Students));
    if(!stud){
        printf("Allocation Error.");
        return NULL;
    }
    stud = ListStudents;

    Elem->id_student = id_Student;
    strcpy(Elem->First_name , first_name);
    strcpy(Elem->Last_name , last_name);
    Elem->Score_math = score_math;
    Elem->Score_science = score_science;
    Elem->Next = NULL;

    if(!stud) stud = Elem;
    else{
        Elem->Next = stud;
        stud = Elem;
    }
    return stud;
}

Students* remove_Student_ListStudents(Students* ListStudents, int Reference){
    Students* stud = NULL;
    Students* Elem = NULL;

    if(!ListStudents){
        printf("No students to delete!\n");
        return NULL;
    }
    // cas ou il est le premier element
    if(ListStudents->id_student == Reference){
        Elem = ListStudents;
        ListStudents = Elem->Next;
        free(Elem);
        printf("the student has been deleted.\n");
        return ListStudents;
    }
    // on cherche l'etudiant
    for(stud = ListStudents ; stud->Next != NULL ; stud = stud->Next)
        if(stud->Next->id_student == Reference){
            Elem = stud->Next;
            stud->Next = Elem->Next;
            free(Elem);
            printf("the student has been deleted\n");
            return ListStudents;
        }
    printf("there is no student with this id.\n");
    return ListStudents;
}

Students* modify_Student_MathScore(Students* ListStudents, int Reference, int scoreMath){
    Students* stud = NULL;

    if(!ListStudents){
        printf("the list is empty nothing to modify !\n");
        return NULL;
    }

    //Cas ou il est le premier element
    if(ListStudents->id_student == Reference){
        ListStudents->Score_math = scoreMath;
        printf("the math grade has deen modified .\n");
        return ListStudents;
    }
    for(stud = ListStudents ; stud->Next != NULL ; stud = stud->Next)
        if(stud->Next->id_student == Reference){
            stud->Next->Score_math = scoreMath;
            printf("the math grade has deen modified\n");
            return ListStudents;
        }
    printf("there is no student with this id\n");
    return ListStudents;
}

Students* modify_Student_ScienceScore(Students* ListStudents, int Reference, int scoreScience){
    Students* stud = NULL;

    if(!ListStudents){
        printf("the list of students is empty ,  !!! nothign to modify\n");
        return NULL;
    }

    //Cas ou il est le premier element
    if(ListStudents->id_student == Reference){
        ListStudents->Score_science = scoreScience;
        printf("the science grade has deen modified.\n");
        return ListStudents;
    }
    for(stud = ListStudents ; stud->Next != NULL ; stud = stud->Next)
        if(stud->Next->id_student == Reference){
            stud->Next->Score_science = scoreScience;
            printf("the science grade has deen modified.\n");
            return ListStudents;
        }
    printf("there is no student with this id\n");
    return ListStudents;
}

//Fonctions de Sauvegarde / Chargement
void SaveList_Students(Students* ListStudents){
    FILE* F = NULL;
    Students* stud = NULL;
    F = fopen("./Students.txt", "w");
    if(!F){
        printf("Error can't open the file !\n");
        return;
    }
    for(stud = ListStudents ; stud != NULL ; stud = stud->Next)
        fprintf(F,"%d\t\t%s\t\t%s\t\t%d\t\t%d\t\t", stud->id_student, stud->First_name, stud->Last_name, stud->Score_math, stud->Score_science);
    fclose(F);
    return;
}

Students* LoadList_Students(){
    Students*  ListStudents = NULL;
    Students* stud = NULL;
    FILE *F = NULL;

    stud = malloc(sizeof(Students));
    if(!stud) {
        printf("Allocation Error!");
        return NULL;
    }

    F = fopen("./Students.txt", "r");

    if(!F) return NULL;

    while(!feof(F)){
        fscanf(F,"%d\t\t%s\t\t%s\t\t%d\t\t%d\t\t", &stud->id_student, &stud->First_name, &stud->Last_name, &stud->Score_math, &stud->Score_science);
        ListStudents = add_Student_ListStudents(ListStudents, stud->id_student, stud->First_name, stud->Last_name, stud->Score_math , stud->Score_science);
    }
    fclose(F);
    return ListStudents;
}

int raise(int sig);

int getch(){
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
 }



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


int main(){

	/* Variables d'aide */
	char c, MotDePasse[MAX], first_name[MAX], last_name[MAX];
	int i, choice, choiceStud,choiceStudMod, reference, exist,score_math, score_science, back;
	i = 0;
	/* Date Et Heure */
	time_t temps;
	struct tm d;

	/* listes chainees */
	Students * ListStudents = NULL;

	char MdP[MAX] = "Store";	  /* MOT DE PASSE REQUIS */

	Students * Stud = malloc(sizeof(Students));
	if(!Stud){
		printf("Allocation Error!\n");
		return 0;
	}

	/* Chargement des donnees */
	ListStudents = LoadList_Students();

	/* page d'authentification */
	system("clear");
	printf("\t\t\t\tInformation of students\n\n\n");
	do{
		printf("\n\t\tGive the Password to get in  : ");
    	while (i < MAX){
    		fflush(stdin);
    		MotDePasse[i]=getch();
    		c = MotDePasse[i];
    		if(c == '\n') break;
    		else printf("*");
  		    i++;
		}
		fflush(stdin);
		MotDePasse[i]='\0';
		i = 0;

	} while (strcmp(MotDePasse, MdP) != 0);

	do{
			system("clear");
			printf("\t\t\t\tOrganisation des Students \n\n\n\n");
			printf("1- show the list of students \n");
			printf("2- search for a Student\n");
			printf("3- add a Student\n");
			printf("4- delete a Student\n");
			printf("5- Modify  Students grades \n");
			printf("6- Save Students data \n");

			do{
				printf("Choose a number  : ");
				fflush(stdin);
				scanf("%d",&choiceStud);
			} while (choiceStud < 1 || choiceStud > 6);

			switch(choiceStud){
				case 1 : system("clear");show_Students(ListStudents);break;
				case 2 : {
                            system("clear");
							printf("\t\t\t\t search for a Student\n\n\n\n");
							printf(" write a number   : \n");
							fflush(stdin);
							scanf("%d", &reference);
							show_student_ById(ListStudents, reference);
							break;
				case 3 : {
                        system("clear");
                        printf("\t\t\t\tadd a Studentt\n\n\n\n");
                        printf("write the student ID : \n");
                        fflush(stdin);
                        scanf("%d", &reference);
                        exist = 0;
                        for (Stud = ListStudents ; Stud != NULL ; Stud = Stud->Next)
                        {
                            if (Stud->id_student == reference)
                            {
                                exist = 1;
                                break;
                            }
                        }
                        if (exist)
                        {
                            printf("This ID already exist .\n");
                            break;
                        }
                        printf("Give the  first name of the  Student : \n");
                        fflush(stdin);
                        scanf("%s", &first_name);
                        printf("Give the last name of Student : \n");
                        fflush(stdin);
                        scanf("%s", &last_name);
                        printf("Give  a math score for the Student : \n");
                        fflush(stdin);
                        scanf("%d", &score_math);
                        printf("Give  a science score for the Student : \n");
                        fflush(stdin);
                        scanf("%d", &score_science);
                        ListStudents = add_Student_ListStudents(ListStudents, reference, first_name, last_name, score_math , score_science);
                        printf("Le Student had been added .\n");
                        break;

				}
				case 4 :{   system("clear");
							printf("\t\t\t\t Deleting a Student  \n\n\n\n");
							printf("Give the Students  ID : \n");
							fflush(stdin);
							scanf("%d", &reference);
							ListStudents = remove_Student_ListStudents(ListStudents, reference);
							break;
				}
				case 5 : {
					system("clear");
					printf("\t\t\t\t Modify a Student\n\n\n\n");
					printf("Give the Student  ID : \n");
					fflush(stdin);
					scanf("%d", &reference);
					printf("You want to modify ? : \n");
					printf("1- Le score math \n");
					printf("2- La score science \n");
					do
					{
						printf("Choose a number ? : ");
						fflush(stdin);
						scanf("%d",&choiceStudMod);
					} while (choiceStudMod < 1 || choiceStudMod > 4);
					switch(choiceStudMod){
						case 1 :{
							printf("Give the new  math score : \n");
							fflush(stdin);
							scanf("%d", &score_math);
							ListStudents = modify_Student_MathScore(ListStudents, reference , score_math);
							break;
						}
						case 2 :{
							printf("Give the new science score : \n");
							fflush(stdin);
							scanf("%d", &score_science);
							ListStudents = modify_Student_ScienceScore(ListStudents, reference , score_science);
							break;
						}
					}
					break;
				}
				case 6 :{
					SaveList_Students(ListStudents);
					system("clear");
					printf("students informations wer registred succesfully .\n");
					break;
				}
				default : break;
			}
			printf("\n\n Press 0 to come back \nPress Any Key To Quit The Program.\n");
			fflush(stdin);
			scanf("%d", &back);
			if (back)
			{
				return 0;
			}
		}

			printf("\n\nPress 0 to come backr \nPress Any Key To Quit The Program.\n");
			fflush(stdin);
			scanf("%d", &back);
			if (back)
			{
				return 0;
			}

	} while (back == 0);
	return 0;
}

