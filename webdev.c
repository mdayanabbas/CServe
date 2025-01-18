#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
// #include <json-c/json.h>
// #include <sqlite3.h>
#include <time.h>

// Constants
#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 4096
#define MAX_GROUPS 1000
#define MAX_SKILLS 50
#define MAX_PROJECTS 100

// Data Structures

// User Profile
typedef struct {
    int user_id;
    char username[50];
    char email[100];
    char skills[MAX_SKILLS][50];
    int skill_count;
    int privacy_level;
    time_t last_active;
} UserProfile;

// Project Structure
typedef struct {
    int project_id;
    char title[100];
    char description[500];
    int owner_id;
    char tech_stack[10][50];
    int collaborators[MAX_CLIENTS];
    int collab_count;
    time_t created_at;
} Project;

// Group Structure
typedef struct {
    int group_id;
    char name[100];
    char description[500];
    int members[MAX_CLIENTS];
    int member_count;
    int privacy_level;
} Group;

// Event Structure
typedef struct {
    int event_id;
    char title[100];
    char description[500];
    time_t start_date;
    time_t end_date;
    int participants[MAX_CLIENTS];
    int participant_count;
    char event_type[50];
} Event;

// Mentorship Program Structure
typedef struct {
    int program_id;
    int mentor_id;
    int mentee_id;
    char focus_area[100];
    time_t start_date;
    time_t end_date;
    int status;
} MentorshipProgram;

// Notification Structure
typedef struct {
    int notification_id;
    int user_id;
    char message[256];
    int type;
    time_t created_at;
    int read_status;
} Notification;

// Global Variables
UserProfile users[MAX_CLIENTS];
Project projects[MAX_PROJECTS];
Group groups[MAX_GROUPS];
Event events[100];
MentorshipProgram mentorship_programs[100];
Notification notifications[1000];

// Database connection
// sqlite3 *db;

// Mutex for thread safety
CRITICAL_SECTION users_mutex;
CRITICAL_SECTION projects_mutex;
CRITICAL_SECTION groups_mutex;

// 1. Recommendation System Implementation
typedef struct {
    int user_id;
    double similarity_score;
} UserSimilarity;

double calculate_skill_similarity(UserProfile *user1, UserProfile *user2) {
    int common_skills = 0;
    
    for (int i = 0; i < user1->skill_count; i++) {
        for (int j = 0; j < user2->skill_count; j++) {
            if (strcmp(user1->skills[i], user2->skills[j]) == 0) {
                common_skills++;
            }
        }
    }
    
    return (double)common_skills / 
           ((double)(user1->skill_count + user2->skill_count - common_skills));
}

void generate_recommendations(int user_id, UserSimilarity *recommendations) {
    UserProfile *current_user = &users[user_id];
    int rec_count = 0;
    
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (i != user_id && users[i].user_id != 0) {
            recommendations[rec_count].user_id = i;
            recommendations[rec_count].similarity_score = 
                calculate_skill_similarity(current_user, &users[i]);
            rec_count++;
        }
    }
    
    // Sort recommendations by similarity score
    for (int i = 0; i < rec_count - 1; i++) {
        for (int j = 0; j < rec_count - i - 1; j++) {
            if (recommendations[j].similarity_score < 
                recommendations[j + 1].similarity_score) {
                UserSimilarity temp = recommendations[j];
                recommendations[j] = recommendations[j + 1];
                recommendations[j + 1] = temp;
            }
        }
    }
}

// 2. Project Showcase Section Implementation
void create_project(Project *project, const char *title, const char *description, 
                   int owner_id) {
    EnterCriticalSection(&projects_mutex);
    
    static int next_project_id = 1;
    project->project_id = next_project_id++;
    strncpy(project->title, title, sizeof(project->title) - 1);
    strncpy(project->description, description, sizeof(project->description) - 1);
    project->owner_id = owner_id;
    project->collab_count = 0;
    project->created_at = time(NULL);
    
    LeaveCriticalSection(&projects_mutex);
}

// json_object* project_to_json(Project *project) {
//     json_object *project_obj = json_object_new_object();
    
//     json_object_object_add(project_obj, "project_id", 
//                           json_object_new_int(project->project_id));
//     json_object_object_add(project_obj, "title", 
//                           json_object_new_string(project->title));
//     json_object_object_add(project_obj, "description", 
//                           json_object_new_string(project->description));
    
//     return project_obj;
// }

// 3. Group Collaboration Spaces Implementation
int create_group(const char *name, const char *description, int creator_id, 
                int privacy_level) {
    EnterCriticalSection(&groups_mutex);
    
    static int next_group_id = 1;
    int group_index = -1;
    
    // Find empty slot
    for (int i = 0; i < MAX_GROUPS; i++) {
        if (groups[i].group_id == 0) {
            group_index = i;
            break;
        }
    }
    
    if (group_index != -1) {
        Group *new_group = &groups[group_index];
        new_group->group_id = next_group_id++;
        strncpy(new_group->name, name, sizeof(new_group->name) - 1);
        strncpy(new_group->description, description, 
                sizeof(new_group->description) - 1);
        new_group->members[0] = creator_id;
        new_group->member_count = 1;
        new_group->privacy_level = privacy_level;
    }
    
    LeaveCriticalSection(&groups_mutex);
    return group_index;
}

// 4. Skill Tags and Endorsements Implementation
typedef struct {
    int skill_id;
    char skill_name[50];
    int endorsements[MAX_CLIENTS];
    int endorsement_count;
} Skill;

void add_skill(UserProfile *user, const char *skill_name) {
    if (user->skill_count < MAX_SKILLS) {
        strncpy(user->skills[user->skill_count], skill_name, 49);
        user->skill_count++;
    }
}

void endorse_skill(int endorser_id, int user_id, const char *skill_name) {
    UserProfile *user = &users[user_id];
    
    for (int i = 0; i < user->skill_count; i++) {
        if (strcmp(user->skills[i], skill_name) == 0) {
            // Add endorsement logic here
            // Would typically involve a separate endorsements table in the database
            break;
        }
    }
}

// 5. Event/Hackathon Integration Implementation
Event* create_event(const char *title, const char *description, 
                   time_t start_date, time_t end_date, const char *event_type) {
    static int next_event_id = 1;
    Event *new_event = NULL;
    
    // Find empty slot
    for (int i = 0; i < 100; i++) {
        if (events[i].event_id == 0) {
            new_event = &events[i];
            break;
        }
    }
    
    if (new_event) {
        new_event->event_id = next_event_id++;
        strncpy(new_event->title, title, sizeof(new_event->title) - 1);
        strncpy(new_event->description, description, 
                sizeof(new_event->description) - 1);
        new_event->start_date = start_date;
        new_event->end_date = end_date;
        strncpy(new_event->event_type, event_type, 
                sizeof(new_event->event_type) - 1);
        new_event->participant_count = 0;
    }
    
    return new_event;
}

// 6. Interest Groups/Communities Implementation
typedef struct {
    int interest_id;
    char name[100];
    char description[500];
    int members[MAX_CLIENTS];
    int member_count;
} InterestGroup;

InterestGroup interest_groups[100];

void join_interest_group(int user_id, int group_id) {
    EnterCriticalSection(&groups_mutex);
    
    InterestGroup *group = &interest_groups[group_id];
    if (group->member_count < MAX_CLIENTS) {
        group->members[group->member_count++] = user_id;
    }
    
    LeaveCriticalSection(&groups_mutex);
}

// 7. Mentorship Program Implementation
MentorshipProgram* create_mentorship_program(int mentor_id, int mentee_id, 
                                             const char *focus_area) {
    static int next_program_id = 1;
    MentorshipProgram *new_program = NULL;
    
    // Find empty slot
    for (int i = 0; i < 100; i++) {
        if (mentorship_programs[i].program_id == 0) {
            new_program = &mentorship_programs[i];
            break;
        }
    }
    
    if (new_program) {
        new_program->program_id = next_program_id++;
        new_program->mentor_id = mentor_id;
        new_program->mentee_id = mentee_id;
        strncpy(new_program->focus_area, focus_area, 
                sizeof(new_program->focus_area) - 1);
        new_program->start_date = time(NULL);
        new_program->status = 1;  // Active
    }
    
    return new_program;
}

// 8. Custom Notifications Implementation
void send_notification(int user_id, const char *message) {
    static int next_notification_id = 1;
    Notification *new_notification = NULL;
    
    // Find empty slot
    for (int i = 0; i < 1000; i++) {
        if (notifications[i].notification_id == 0) {
            new_notification = &notifications[i];
            break;
        }
    }
    
    if (new_notification) {
        new_notification->notification_id = next_notification_id++;
        new_notification->user_id = user_id;
        strncpy(new_notification->message, message, 
                sizeof(new_notification->message) - 1);
        new_notification->created_at = time(NULL);
        new_notification->read_status = 0;  // Unread
    }
}

// Main Server Logic
void handle_client_connection(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;
    
    while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        // Process the request and send a response based on the received data
        send(client_socket, buffer, bytes_received, 0);  // Send data back to the client
    }
    
    closesocket(client_socket);
}

DWORD WINAPI client_handler(LPVOID lpParam) {
    SOCKET client_socket = (SOCKET)lpParam;
    handle_client_connection(client_socket);
    return 0;
}

int main() {
    WSADATA wsa_data;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int client_len;
    
    InitializeCriticalSection(&users_mutex);
    InitializeCriticalSection(&projects_mutex);
    InitializeCriticalSection(&groups_mutex);
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("Winsock initialization failed\n");
        return 1;
    }
    
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    // Listen for incoming connections
    if (listen(server_socket, 3) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }
    
    printf("Server listening on port %d\n", PORT);
    
    // Accept clients
    client_len = sizeof(client_addr);
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) != INVALID_SOCKET) {
        printf("Client connected\n");
        
        // Create a new thread for each client connection
        CreateThread(NULL, 0, client_handler, (LPVOID)client_socket, 0, NULL);
    }
    
    // Clean up
    closesocket(server_socket);
    WSACleanup();
    DeleteCriticalSection(&users_mutex);
    DeleteCriticalSection(&projects_mutex);
    DeleteCriticalSection(&groups_mutex);
    
    return 0;
}
