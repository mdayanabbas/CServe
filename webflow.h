#ifndef WEBFLOW_H
#define WEBFLOW_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <winsock2.h>  

#define int8 char
#define int16 short
#define int32 int
#define int64 long long

// Basic HTTP Response Codes
#define HTTP_OK 200
#define HTTP_NOT_FOUND 404
#define HTTP_INTERNAL_ERROR 500

// A structure to define each flow stage
typedef struct FlowStage {
    char *name;
    void (*handler)(int32 client_socket, char *request);
    struct FlowStage *next;
} FlowStage;

// Web Framework Core
void start_server(int16 port);
void add_flow_stage(FlowStage *stage);
void run_flows(int32 client_socket, char *request);

#endif // WEBFLOW_H
