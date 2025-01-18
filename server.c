#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define SERVER_PORT 8080
#define MAX_REQUEST_SIZE 1024
#define MAX_FILE_SIZE 1024 * 1024 
#define STATIC_DIR "public" 


typedef struct {
    char name[100];
    int endorsements;
} SkillTag;

typedef struct {
    char event_name[100];
    char description[500];
    char date[50];
} Event;

typedef struct {
    char project_name[100];
    char description[500];
    char link[500];
} Project;


typedef struct {
    SOCKET client_socket;
    const char* static_dir;
} Server;

typedef enum {
    GET,
    POST,
    UNKNOWN
} HttpMethod;

typedef struct {
    HttpMethod method;
    char path[MAX_REQUEST_SIZE];
    char body[MAX_REQUEST_SIZE];
} HttpRequest;


void log_request(const char* method, const char* path) {
    printf("Request received: %s %s\n", method, path);
}

char* get_mime_type(const char* file_path) {
    if (strstr(file_path, ".html")) return "text/html";
    if (strstr(file_path, ".css")) return "text/css";
    if (strstr(file_path, ".js")) return "application/javascript";
    if (strstr(file_path, ".jpg") || strstr(file_path, ".jpeg")) return "image/jpeg";
    if (strstr(file_path, ".png")) return "image/png";
    return "application/octet-stream";
}

// Function to parse HTTP method
HttpMethod parse_method(const char* method_str) {
    if (strcmp(method_str, "GET") == 0) return GET;
    if (strcmp(method_str, "POST") == 0) return POST;
    return UNKNOWN;
}

// Function to parse HTTP request into a structured format
void parse_request(const char* request, HttpRequest* http_request) {
    char method[8];
    sscanf(request, "%s %s", method, http_request->path);
    http_request->method = parse_method(method);
    if (http_request->method == POST) {
        sscanf(request, "%s %s %s", method, http_request->path, http_request->body);
    }
}

// Function to serve static files
void serve_static_file(const char* file_path, char* response) {
    FILE* file = fopen(file_path, "rb");
    if (!file) {
        sprintf(response,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h1>404 - File Not Found</h1></body></html>"
        );
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* content = (char*)malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);

    sprintf(response,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        get_mime_type(file_path),
        content
    );
    free(content);
}

//working on ADV               you remember
// Handler function for advanced features like recommendation, showcase, etc.
void handle_advanced_features(const char* path, char* response) {
    if (strcmp(path, "/recommendations") == 0) {
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"
                          "<html><body><h1>Recommended Projects</h1><ul><li>Project 1</li><li>Project 2</li></ul></body></html>");
    }
    else if (strcmp(path, "/showcase") == 0) {
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"
                          "<html><body><h1>Project Showcase</h1><ul><li>Project A</li><li>Project B</li></ul></body></html>");
    }
    else if (strcmp(path, "/events") == 0) {
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"
                          "<html><body><h1>Upcoming Events</h1><ul><li>Hackathon 2023</li><li>Workshop on ML</li></ul></body></html>");
    }
    else {

        sprintf(response, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"
                          "<html><body><h1>404 - Not Found</h1></body></html>");
    }
}


DWORD WINAPI handle_connection_thread(LPVOID arg) {
    Server* server = (Server*)arg;
    SOCKET client_socket = server->client_socket;
    char request[MAX_REQUEST_SIZE];
    char response[MAX_FILE_SIZE];

    int bytes_received = recv(client_socket, request, sizeof(request) - 1, 0);
    if (bytes_received <= 0) {
        closesocket(client_socket);
        return 0;
    }

    request[bytes_received] = '\0';

    HttpRequest http_request;
    parse_request(request, &http_request);
    log_request(http_request.method == GET ? "GET" : "POST", http_request.path);

    if (http_request.method == GET) {
        handle_advanced_features(http_request.path, response);
    } else {
        sprintf(response,
            "HTTP/1.1 405 Method Not Allowed\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h1>405 - Method Not Allowed</h1></body></html>"
        );
    }

    send(client_socket, response, strlen(response), 0);
    closesocket(client_socket);
    return 0;
}

// Function to start the server
void start_server(int port, const char* static_dir) {
    WSADATA wsa_data;
    SOCKET server_socket, client_socket;
    SOCKADDR_IN server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        printf("WSAStartup failed\n");
        return;
    }

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind socket
    if (bind(server_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("Bind failed\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == SOCKET_ERROR) {
        printf("Listen failed\n");
        closesocket(server_socket);
        WSACleanup();
        return;
    }

    printf("Server listening on port %d...\n", port);

    while (1) {
        // Accept incoming client connections
        client_socket = accept(server_socket, (SOCKADDR*)&client_addr, &client_addr_size);
        if (client_socket == INVALID_SOCKET) {
            printf("Client accept failed\n");
            continue;
        }

        // Handle the connection in a new thread
        Server* server = (Server*)malloc(sizeof(Server));
        server->client_socket = client_socket;
        server->static_dir = static_dir;
        HANDLE thread = CreateThread(NULL, 0, handle_connection_thread, server, 0, NULL);
        if (thread == NULL) {
            printf("Thread creation failed\n");
            closesocket(client_socket);
            free(server);
        } else {
            CloseHandle(thread);
        }
    }

    closesocket(server_socket);
    WSACleanup();
}

int main() {
    start_server(SERVER_PORT, STATIC_DIR);
    return 0;
}
