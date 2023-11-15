#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STRING_SIZE 100

typedef struct MusicPlayer {
    char song[MAX_STRING_SIZE];
    char artist[MAX_STRING_SIZE];
    char album[MAX_STRING_SIZE];
    char duration[MAX_STRING_SIZE];
    struct MusicPlayer* prev_song;
    struct MusicPlayer* next_song;
} MusicPlayer;

// Function prototypes
void display_menu();
void list_of_songs(MusicPlayer *ptr);
void free_data(MusicPlayer *ptr);
MusicPlayer* read_line(FILE *fp);
MusicPlayer* readcsv();
void add_to_playlist(MusicPlayer *head);
void display_playlist(MusicPlayer *head);
void play_playlist(MusicPlayer *head);
MusicPlayer* shuffle_playlist(MusicPlayer *head);
void show_details(MusicPlayer* ptr);
MusicPlayer* play(MusicPlayer* ptr);
void pause_song();
MusicPlayer* play_prev(MusicPlayer *head, MusicPlayer *ptr);
MusicPlayer* play_next(MusicPlayer *head, MusicPlayer *ptr);
int does_playlist_exist(MusicPlayer *head);
int search(MusicPlayer *head,char *search_song);


int main() {
    MusicPlayer *head = NULL;
    printf("-----------------------------WELCOME TO DECIBELS---------------------------------------\n");
    head = readcsv();
    display_menu();
    char *search_song;
    MusicPlayer *ptr = head;
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        printf("\n");
        switch (choice) {
            case 1:
                list_of_songs(head);
                break;
            case 2:
                add_to_playlist(head);
                break;
            case 3:
                display_playlist(head);
                break;
            case 4:
                play_playlist(head);
                break;
            case 5:
                ptr = play_prev(head, ptr);
                break;
            case 6:
                ptr = play_next(head, ptr);
                break;
            case 7:
                ptr=shuffle_playlist(head);
                break;
            case 8:
                ptr = play(ptr);
                break;
            case 9:
                pause_song();
                break;
            case 10:
                show_details(ptr);
                break;
            case 11:
                display_menu();
                break;
            case 12:
                   char search_song[MAX_STRING_SIZE];
                   printf("Enter the song to be searched:\n");
                   scanf("%s",search_song);
                   int x=search(head,search_song);
                   if(x==-1)
                   {
                    printf("The song is not found.");
                   }
                   else{
                    MusicPlayer *cur=head;
                    printf("The song is found at position %d\n",x+1);
                    for(int i=0;i<x;i++)
                    {
                        cur=cur->next_song;
                    }
                    show_details(cur);
                   }
                    break;
            case -1:
                break;
            default:
                printf("Invalid option! Please enter a valid choice.\n");
                break;
        }
    } while (choice != -1);

    // Free allocated memory for the playlist
    MusicPlayer *current = head;
    while (current != NULL) {
        MusicPlayer *next = current->next_song;
        free(current);
        current = next;
    }

    return 0;
}

MusicPlayer* read_line(FILE *fp) {
    MusicPlayer *data = malloc(sizeof(MusicPlayer));
    if (data == NULL) {
        printf("Memory allocation error\n");
        return NULL;
    }

    char buffer[MAX_STRING_SIZE * 4];
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        free(data);
        return NULL;
    }

    char *token = strtok(buffer, ",");
    if (token == NULL) {
        free(data);
        return NULL;
    }

    strcpy(data->song, token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        free(data);
        return NULL;
    }
    strcpy(data->artist, token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        free(data);
        return NULL;
    }
    strcpy(data->album, token);
    token = strtok(NULL, ",");
    if (token == NULL) {
        free(data);
        return NULL;
    }
    strcpy(data->duration, token);

    return data;
}


void list_of_songs(MusicPlayer *ptr) {
    while (ptr != NULL) {
        printf("%s - %s\n", ptr->song, ptr->artist);
        ptr = ptr->next_song;
    }
}

void free_data(MusicPlayer *ptr) {
    free(ptr);
}

MusicPlayer* readcsv() {
    MusicPlayer *songlist = NULL;
    FILE *fp = fopen("Songs.csv", "r");
    if (fp == NULL) {
        printf("File open error\n");
        return NULL;
    }

    MusicPlayer *head = NULL;
    MusicPlayer *current = NULL;
    MusicPlayer *temp = NULL;
    while ((temp = read_line(fp)) != NULL) {
        if (head == NULL) {
            head = temp;
            current = temp;
        } else {
            current->next_song = temp;
            temp->prev_song = current;
            current = temp;
        }
        if (songlist == NULL) {
            songlist = temp;
        }
    }
    fclose(fp);
    return songlist;
}

void display_menu() {
    printf("---------------------------------------------------------------------------------------\n");
    printf("--                              FEATURES                                             --\n");
    printf("---------------------------------------------------------------------------------------\n");
    printf("--          1)  View Song List                                                       --\n");
    printf("--          2)  Add to playlist                                                      --\n");
    printf("--          3)  Display playlist                                                     --\n");
    printf("--          4)  Play playlist                                                        --\n");
    printf("--          5)  Play previous song                                                   --\n");
    printf("--          6)  Play next song                                                       --\n");
    printf("--          7)  Shuffle playlist                                                     --\n");
    printf("--          8)  Play                                                                 --\n");
    printf("--          9)  Pause                                                                --\n");
    printf("--          10) Show Song details                                                    --\n");
    printf("--          11) View Menu                                                            --\n");
    printf("--          12) Search Song                                                            --\n");
    printf("--          -1) Exit                                                                 --\n");
    printf("---------------------------------------------------------------------------------------\n");
    printf("--                          ENTER YOUR OPTION                                        --\n");
    printf("---------------------------------------------------------------------------------------\n");                                              

}

void add_to_playlist(MusicPlayer *head) {
    // Implementation for adding songs to the playlist
    // Use a similar logic as creating a playlist while ensuring proper connections between songs
    char ch = 'y'; 
    MusicPlayer *playlist = head;
    while(ch == 'y') 
    {
        char s[50];
        printf("Enter the song to be added: ");
        scanf("%s", s);
        MusicPlayer *temp = (MusicPlayer*)malloc(sizeof(MusicPlayer));
        strcpy(temp->song, s);
        temp->prev_song = NULL;
        temp->next_song = NULL;
        playlist->next_song = temp;
        temp->prev_song = playlist;
        playlist = temp;
        printf("Do you want to add another song to the playlist (y/n): ");
        scanf(" %c", &ch);
        if (ch != 'y' && ch != 'n') 
        {
            printf("Invalid input. Exiting loop.\n");
            break;
        }
    }
}

void display_playlist(MusicPlayer *head) {
    // Display the playlist items
    //list_of_songs(head);
    printf("Your playlist:\n");
    while (head != NULL) 
    {
        printf("%s\n", head->song);
        head = head->next_song;
    }
}

void play_playlist(MusicPlayer *head) {
    // Play the entire playlist
    
    while (head != NULL) {
        printf("Now Playing: %s \n", head->song);
        head = head->next_song;
    }
}

MusicPlayer* shuffle_playlist(MusicPlayer *head) {
    // Shuffle the playlist and play a random song
    // Implementation similar to the previous shuffle_playlist function
    int count=0;
    MusicPlayer *cur=head;
    MusicPlayer *cur2=head;
    while(cur!=NULL)
    {
        count++;
        cur=cur->next_song;
    }
    //printf("\n %d",count);
    if(does_playlist_exist(head))
    {
        int lower=1,upper=count,number;
        
        srand(time(0));
        number = (rand() % (upper - lower + 1)) + lower;
        for(int i=0;i<number;i++)
        {
            cur2=cur2->next_song;
        }
        printf("Now playing: %s",cur2->song);
    }
    else
    {
        printf("The playlist does not exits.");
    }
    return cur2;
}

void show_details(MusicPlayer* ptr) {
    // Display song details
    printf("Current song details:\n");
    printf("Song title: %s\n", ptr->song);
    printf("Artist: %s\n", ptr->artist);
    printf("Album: %s\n", ptr->album);
    printf("Duration: %s\n", ptr->duration);
}

MusicPlayer* play(MusicPlayer* ptr) {
    // Play the current song
    printf("Now Playing: %s - %s\n", ptr->song, ptr->artist);
    return ptr;
}

void pause_song() {
    // Pause the current song
    printf("Song Paused\n");
}

MusicPlayer* play_prev(MusicPlayer *head, MusicPlayer *ptr) {
    // Play the previous song
    // Similar logic as previously implemented
    if (does_playlist_exist(head) && ptr != NULL)
    {
        if (ptr->prev_song != NULL)
        {
            ptr = ptr->prev_song;
        }
        else
        {
            MusicPlayer *cur=head;
            while(cur->next_song!=NULL)
            {
                cur=cur->next_song;
            }
            ptr=cur;
            //printf("REACHED START OF PLAYLIST");
        }
        printf("Now Playing :%s",ptr->song);
    }
    else
    {
        printf("No song added to playlist");
    }
    return ptr;
}

MusicPlayer* play_next(MusicPlayer *head, MusicPlayer *ptr) {
    // Play the next song
    // Similar logic as previously implemented
    if (does_playlist_exist(head) && ptr != NULL)
    {
        if (ptr->next_song != NULL)
        {
            ptr = ptr->next_song;
        }
        else
        {
            ptr=head;
            //printf("REACHED START OF PLAYLIST");
        }
        printf("Now Playing :%s",ptr->song);
    }
    else
    {
        printf("No song added to playlist");
    }
    return ptr;
}

int does_playlist_exist(MusicPlayer *head) {
    // Check if the playlist exists
    return (head != NULL);
}

int search(MusicPlayer *head, char *search_song)
{
    MusicPlayer* cur = head; 
    int count = 0;
    while(cur!=NULL){
        if (strcmp(cur->song, search_song) == 0) {
            // String found, return the position (0-based)
            return count;
        }

        // Move to the next node
        cur = cur->next_song;
        count++;

    }
    return -1;
}
