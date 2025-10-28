// udp_server.c
// UDP command server for ENSC 351 Assignment 2.
// Build: gcc -Wall -Wextra -O2 -pthread udp_server.c -o udp_demo
// Run demo: ./udp_demo
// Test from host: netcat -u <target-ip> 12345

#define _GNU_SOURCE
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

typedef struct {
    long long (*get_count)(void);           // total samples so far
    int       (*get_history_size)(void);    // samples in previous second
    int       (*get_dips)(void);            // dips in previous second
    double*   (*get_history)(int* size);    // malloc'd array of voltages; caller will free
} UdpCallbacks;

static int                g_sock = -1;
static pthread_t          g_thread;
static volatile bool      g_running = false;
static UdpCallbacks       g_cb = {0};
static char               g_last_cmd[64] = {0};

static void send_text(int sock, const struct sockaddr_in* cli, const char* fmt, ...)
{
    char buf[1400];
    va_list ap; va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n < 0) return;
    if (n > (int)sizeof(buf)) n = (int)sizeof(buf);
    sendto(sock, buf, n, 0, (const struct sockaddr*)cli, sizeof(*cli));
}

static void send_help(int sock, const struct sockaddr_in* cli)
{
    const char* h =
        "Accepted command examples:\n"
        "count       -- get the total number of samples taken.\n"
        "length      -- get the number of samples taken in the previously completed second.\n"
        "dips        -- get the number of dips in the previously completed second.\n"
        "history     -- get all the samples in the previously completed second.\n"
        "stop        -- cause the server program to end.\n"
        "<enter>     -- repeat last command.\n";
    sendto(sock, h, (int)strlen(h), 0, (const struct sockaddr*)cli, sizeof(*cli));
}

// Pack history as "1.234, 0.056, ..." 10 per line; keep packets <1400B.
static void send_history(int sock, const struct sockaddr_in* cli, double* hist, int N)
{
    const int MAX = 1400;
    char pkt[MAX];
    int pos = 0, on_line = 0;

    for (int i = 0; i < N; i++) {
        char one[32];
        int len = snprintf(one, sizeof(one), "%.3f%s", hist[i], (on_line == 9 || i == N-1) ? "\n" : ", ");
        if (len < 0) len = 0;

        if (pos + len >= MAX) { // flush
            sendto(sock, pkt, pos, 0, (const struct sockaddr*)cli, sizeof(*cli));
            pos = 0;
        }
        memcpy(pkt + pos, one, len);
        pos += len;

        on_line = (on_line + 1) % 10;
    }
    if (pos > 0) {
        sendto(sock, pkt, pos, 0, (const struct sockaddr*)cli, sizeof(*cli));
    }
}

static void* udp_thread(void* arg)
{
    (void)arg;
    struct sockaddr_in cli;
    socklen_t slen = sizeof(cli);
    char buf[2048];

    while (g_running) {
        ssize_t n = recvfrom(g_sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&cli, &slen);
        if (n < 0) {
            if (errno == EINTR) continue;
            if (g_running) perror("recvfrom");
            break;
        }
        buf[n] = '\0';

        // Trim CR/LF and leading/trailing spaces:
        char* s = buf;
        while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n') s++;
        for (ssize_t i = (ssize_t)strlen(s)-1; i >= 0 && (s[i]=='\r'||s[i]=='\n'||s[i]==' '||s[i]=='\t'); --i) s[i] = '\0';

        // Blank line → repeat last command (if any)
        if (s[0] == '\0') {
            if (g_last_cmd[0] == '\0') {
                send_text(g_sock, &cli, "Unknown command (no previous).\n");
                continue;
            }
            s = g_last_cmd;
        } else {
            // save as last command (lower-cased)
            size_t L = strlen(s); if (L >= sizeof(g_last_cmd)) L = sizeof(g_last_cmd)-1;
            for (size_t i = 0; i < L; i++) g_last_cmd[i] = (char)tolower((unsigned char)s[i]);
            g_last_cmd[L] = '\0';
            s = g_last_cmd;
        }

        // Dispatch
        if (!strcmp(s, "help") || !strcmp(s, "?")) {
            send_help(g_sock, &cli);
        } else if (!strcmp(s, "count")) {
            long long c = g_cb.get_count ? g_cb.get_count() : 0;
            send_text(g_sock, &cli, "# samples taken total: %lld\n", c);
        } else if (!strcmp(s, "length")) {
            int L = g_cb.get_history_size ? g_cb.get_history_size() : 0;
            send_text(g_sock, &cli, "# samples taken last second: %d\n", L);
        } else if (!strcmp(s, "dips")) {
            int d = g_cb.get_dips ? g_cb.get_dips() : 0;
            send_text(g_sock, &cli, "# Dips: %d\n", d);
        } else if (!strcmp(s, "history")) {
            int N = 0;
            double* H = g_cb.get_history ? g_cb.get_history(&N) : NULL;
            if (!H || N <= 0) {
                send_text(g_sock, &cli, "(no history)\n");
            } else {
                send_history(g_sock, &cli, H, N);
                free(H);
            }
        } else if (!strcmp(s, "stop")) {
            send_text(g_sock, &cli, "Program terminating.\n");
            g_running = false; // tell main to shut down
            break;
        } else {
            send_text(g_sock, &cli, "Unknown command: %s\n", s);
        }
    }
    return NULL;
}

// Public API:
int udp_start(uint16_t port, UdpCallbacks cb)
{
    if (g_running) return 0;
    g_cb = cb;

    g_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sock < 0) { perror("socket"); return -1; }

    int yes = 1; setsockopt(g_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    struct sockaddr_in srv = {0};
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    srv.sin_port = htons(port);
    if (bind(g_sock, (struct sockaddr*)&srv, sizeof(srv)) < 0) {
        perror("bind"); close(g_sock); g_sock = -1; return -1;
    }

    g_running = true;
    if (pthread_create(&g_thread, NULL, udp_thread, NULL) != 0) {
        perror("pthread_create");
        close(g_sock); g_sock = -1; g_running = false; return -1;
    }
    return 0;
}

void udp_stop(void)
{
    if (!g_running) return;
    g_running = false;
    shutdown(g_sock, SHUT_RDWR);
    pthread_join(g_thread, NULL);
    close(g_sock);
    g_sock = -1;
}

/* ------------------------- DEMO MAIN (remove in your app) ------------------ */
#ifdef BUILD_DEMO
#include <time.h>
static long long demo_count(void){ static long long c=0; return (c += 487); }
static int demo_len(void){ return 487; }
static int demo_dips(void){ static int d=9; d = (d+3) % 50; return d; }
static double* demo_hist(int* size){
    int N = demo_len();
    double* a = malloc(sizeof(double)*N);
    if (!a){ *size=0; return NULL; }
    for (int i=0;i<N;i++) a[i] = (i%50==0)?1.340: ((i%7)?0.012:0.008);
    *size = N; return a;
}
int main(void)
{
    UdpCallbacks cb = { demo_count, demo_len, demo_dips, demo_hist };
    if (udp_start(12345, cb) != 0) { fprintf(stderr, "udp_start failed\n"); return 1; }
    printf("UDP server listening on 12345. Ctrl+C to quit.\n");
    // Normally your program runs other threads; here we idle until stop is received.
    while (g_running) { usleep(200*1000); }
    udp_stop();
    return 0;
}
#endif