#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

#define MAX 100

// STRUCT FOR LEADERBOARD
struct Player {
    char name[50];
    int wins;
};

// FUNCTION DECLARATIONS
void board(char *a, char *p1, char *p2);
int checkWin(char *a);
int getMove(char *a);
int aiMove(char *a);
void saveScore(char *winner);
void showLeaderboard();

// MAIN
int main()
{
    srand(time(NULL));
    int option;

    do {
        system(CLEAR);
        printf("==== TIC TAC TOE ====\n");
        printf("1. Play Game\n");
        printf("2. View Leaderboard\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d",&option);

        if(option == 1)
        {
            char a[9] = {'1','2','3','4','5','6','7','8','9'};
            char p1[50], p2[50];
            int player=1, move, status=-1, mode;

            printf("1. Player vs Player\n");
            printf("2. Player vs AI\n");
            printf("Select mode: ");
            scanf("%d",&mode);

            printf("Enter Player1 name: ");
            scanf("%s",p1);

            if(mode==1){
                printf("Enter Player2 name: ");
                scanf("%s",p2);
            } else {
                strcpy(p2,"AI-BOT");
            }

            do {
                system(CLEAR);
                board(a, p1, p2);

                if(player==1){
                    printf("%s (X), enter position: ",p1);
                    move = getMove(a);
                    a[move] = 'X';
                } else {
                    if(mode==2){
                        move = aiMove(a);
                        printf("AI chose %d\n", move+1);
                    } else {
                        printf("%s (O), enter position: ",p2);
                        move = getMove(a);
                    }
                    a[move] = 'O';
                }

                status = checkWin(a);
                player = (player==1)?2:1;

            } while(status==-1);

            system(CLEAR);
            board(a, p1, p2);

            if(status==1){
                if(player==2){
                    printf("\n%s wins!\n",p1);
                    saveScore(p1);
                } else {
                    printf("\n%s wins!\n",p2);
                    saveScore(p2);
                }
            } else {
                printf("\nGame Draw!\n");
            }

            printf("\nPress Enter...");
            getchar(); getchar();
        }

        else if(option==2){
            showLeaderboard();
            printf("\nPress Enter...");
            getchar(); getchar();
        }

    } while(option!=3);

    return 0;
}

// BOARD DISPLAY
void board(char *a, char *p1, char *p2)
{
    printf("\n%s vs %s\n\n", p1, p2);
    printf(" %c | %c | %c\n", a[0],a[1],a[2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c\n", a[3],a[4],a[5]);
    printf("---|---|---\n");
    printf(" %c | %c | %c\n\n", a[6],a[7],a[8]);
}

// CHECK WIN
int checkWin(char *a)
{
    int win[8][3] = {
        {0,1,2},{3,4,5},{6,7,8},
        {0,3,6},{1,4,7},{2,5,8},
        {0,4,8},{2,4,6}
    };

    for(int i=0;i<8;i++){
        if(a[win[i][0]]==a[win[i][1]] && a[win[i][1]]==a[win[i][2]])
            return 1;
    }

    for(int i=0;i<9;i++){
        if(a[i]!='X' && a[i]!='O')
            return -1;
    }

    return 0;
}

// GET MOVE
int getMove(char *a)
{
    int choice;
    while(1){
        scanf("%d",&choice);
        if(choice>=1 && choice<=9 && a[choice-1]!='X' && a[choice-1]!='O')
            return choice-1;
        printf("Invalid move! Try again: ");
    }
}

// AI MOVE
int aiMove(char *a)
{
    int move;
    while(1){
        move = rand()%9;
        if(a[move]!='X' && a[move]!='O')
            return move;
    }
}

// SAVE SCORE (ONLY WINNER NAME)
void saveScore(char *winner)
{
    FILE *fp = fopen("score.txt","a");
    if(fp==NULL) return;

    fprintf(fp,"%s\n",winner);
    fclose(fp);
}

// SHOW LEADERBOARD WITH COUNT + SORT
void showLeaderboard()
{
    system(CLEAR);

    FILE *fp = fopen("score.txt","r");
    if(fp==NULL){
        printf("No records found!\n");
        return;
    }

    struct Player players[MAX];
    int count = 0;

    char name[50];

    // COUNT WINS
    while(fscanf(fp,"%s",name)!=EOF){
        int found = 0;

        for(int i=0;i<count;i++){
            if(strcmp(players[i].name,name)==0){
                players[i].wins++;
                found = 1;
                break;
            }
        }

        if(!found){
            strcpy(players[count].name,name);
            players[count].wins = 1;
            count++;
        }
    }

    fclose(fp);

    // SORT (DESCENDING)
    for(int i=0;i<count-1;i++){
        for(int j=i+1;j<count;j++){
            if(players[j].wins > players[i].wins){
                struct Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    // DISPLAY
    printf("===== LEADERBOARD =====\n\n");
    printf("%-15s %s\n","PLAYER","WINS");
    printf("----------------------------\n");

    for(int i=0;i<count;i++){
        printf("%-15s %d\n",players[i].name,players[i].wins);
    }
}
