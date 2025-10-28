// udp_server.h
// Header for UDP command server (ENSC 351 Light Dip Detector)
// Compatible with udp_server.c implementation.
//
// Provides a background thread that listens on UDP port 12345 and responds
// to assignment commands: help, ?, count, length, dips, history, stop.
//
// Author: ChatGPT (based on ENSC 351 assignment spec)
// Date: 2025-10-28

#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------
// Callback structure: define functions that return data to be sent over UDP.
// The main application must fill in these pointers before calling udp_start().
// ---------------------------------------------------------------------------
typedef struct {
    long long (*get_count)(void);           // total samples taken since start
    int       (*get_history_size)(void);    // samples in previous second
    int       (*get_dips)(void);            // dips detected last second
    double*   (*get_history)(int* size);    // malloc'd array of voltages
                                            // caller frees the returned array
} UdpCallbacks;

// ---------------------------------------------------------------------------
// Starts the UDP listener thread.
//   port — UDP port number (e.g., 12345).
//   cb   — callbacks providing data access.
// Returns 0 on success, -1 on failure.
// ---------------------------------------------------------------------------
int udp_start(uint16_t port, UdpCallbacks cb);

// ---------------------------------------------------------------------------
// Stops the UDP server thread gracefully, closes socket, frees resources.
// Safe to call even if server isn’t running.
// ---------------------------------------------------------------------------
void udp_stop(void);

// ---------------------------------------------------------------------------
// Global flag the main loop can check to see if the UDP thread requested stop
// (for example, when receiving the "stop" command).
// ---------------------------------------------------------------------------
extern volatile bool g_running;

#ifdef __cplusplus
}
#endif

#endif // _UDP_SERVER_H_