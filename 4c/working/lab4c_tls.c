// NAME: William Randall
// EMAIL: wrandall1000@gmail.com
// ID: 805167986

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <poll.h>
#include <math.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ctype.h>
#include <openssl/ssl.h>
#include <mraa.h>

char USAGE_STR[] = "Usage: \"./lab4b --period=# --scale=[C|F] --log=[file] --id=# --host=[name|addr] [port#]\"\n";
SSL * ssl = NULL;
SSL_CTX * ssl_context = NULL;

int SENSOR_PIN = 1;
int BUTTON_PIN = 60;

uint32_t parse_host(char * host);
double convert(int reading, char scale);
void send_flush(char * sbuf, int log);

int main(int argc, char ** const argv)
{
    int i;
    
    // temp args
    int period = 1;
    char scale = 'F';
    
    // log args
    int log = 0;
    char * logfile = NULL;
    
    // server args
    int id = 0;
    char * host = NULL;
    int port = 19000;
    
    // handle opts
    int opt;
    static struct option long_options[] =
    {
        {"period", required_argument, 0, 'p'},
        {"scale", required_argument, 0, 's'},
        {"log", required_argument, 0, 'l'},
        {"id", required_argument, 0, 'i'},
        {"host", required_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    while (1)
    {
        opt = getopt_long(argc, argv, "", long_options, NULL);
        if (opt == -1)
        {
            break;
        }
        switch (opt)
        {
            case 'p':
                period = atoi(optarg);
                break;
            case 's':
                scale = *optarg;
                break;
            case 'l':
                log = 1;
                logfile = optarg;
                break;
            case 'i':
                id = atoi(optarg);
                break;
            case 'h':
                host = optarg;
                break;
            default:
                fprintf(stderr, "%s", USAGE_STR);
                exit(1);
        }
    }
    
    // get port
    if (optind < argc)
    {
        port = atoi(argv[optind]);
    }
    else
    {
        fprintf(stderr, "%s", USAGE_STR);
        exit(1);
    }
    
    // set up log
    if (log)
    {
        log = open(logfile, O_WRONLY | O_APPEND | O_CREAT, 0666);
        if (log < 0)
        {
            exit(2);
        }
    }
    
    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        fprintf(stderr, "Socket initialization failed\n");
        exit(2);
    }
    
    // create socket address
    struct sockaddr_in saddr;
    bzero((char * ) &saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    
    // set ip address
    struct hostent * server_ip = gethostbyname(host);
    memcpy((char *)(&saddr.sin_addr.s_addr), (char *)(server_ip->h_addr), server_ip->h_length);
    
    // socket connect
    if (connect(sock, (const struct sockaddr *) &saddr, sizeof(saddr)) < 0)
    {
        fprintf(stderr, "Socket connection failed\n");
        exit(2);
    }
    
    // ssl setup
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
    
    // ssl init
    ssl_context = SSL_CTX_new(TLSv1_client_method());
    if (ssl_context == NULL)
    {
        fprintf(stderr, "Failed SSL context init\n");
        exit(2);
    }
    ssl = SSL_new(ssl_context);
    if (ssl == NULL)
    {
        fprintf(stderr, "Failed SSL init\n");
        exit(2);
    }
    
    // ssl connect
    if (!SSL_set_fd(ssl, sock))
    {
        fprintf(stderr, "SSL association failed");
        exit(2);
    }
    if (SSL_connect(ssl) != 1)
    {
        fprintf(stderr, "SSL connection failed\n");
        exit(2);
    }
    
    // TEST
    // send buffer
    char sbuf[25] = {0};
    
    // send ID num
    sprintf(sbuf, "ID=%i\n", id);
    send_flush(sbuf, log);
    
    // set up sensor
    mraa_aio_context tsensor = mraa_aio_init(SENSOR_PIN);
    if (tsensor == NULL)
    {
        fprintf(stderr, "Failed to initialize sensor\n");
        mraa_deinit();
        exit(2);
    }
    
    // set up poll
    struct pollfd plist[1];
    plist[0].fd = sock;
    plist[0].events = POLLIN;
    
    // data
    int run = 1;
    int shutdown = 0;
    double temp = 0.00;
    
    // command line
    int rret = 0;
    char rbuf[256] = {0};
    char ibuf[256] = {0};
    int istart = 0;
    int inorm = 0;
    
    // set up time
    time_t raw;
    struct tm prev;
    struct tm cur;
    int elapsed = 0;
    int printed = 0;
    
    // first iter
    
    // get temp
    temp = convert(mraa_aio_read(tsensor), scale);
    
    // get time
    time(&raw);
    cur = *localtime(&raw);
    
    // print
    sprintf(sbuf, "%02d:%02d:%02d %.1f\n", cur.tm_hour, cur.tm_min, cur.tm_sec, temp);
    send_flush(sbuf, log);
    
    printed = cur.tm_sec;
    prev = cur;
    
    while (1)
    {
        poll(plist, 1, 0);
        
        // get temp
        temp = convert(mraa_aio_read(tsensor), scale);
        
        // get time
        time(&raw);
        cur = *localtime(&raw);
        elapsed = (cur.tm_min * 60 + cur.tm_sec) - (prev.tm_min * 60 + prev.tm_sec);
        
        // print
        if (run && elapsed > 0 && elapsed % period == 0)
        {
            if (cur.tm_sec != printed)
            {
                sprintf(sbuf, "%02d:%02d:%02d %.1f\n", cur.tm_hour, cur.tm_min, cur.tm_sec, temp);
                send_flush(sbuf, log);
                
                printed = cur.tm_sec;
                prev = cur;
            }
        }
        
        // get input
        if (plist[0].revents & POLLIN)
        {
            rret = SSL_read(ssl, rbuf, 256);
            for (i = 0; i < rret; i++)
            {
                ibuf[istart + i - inorm] = rbuf[i];
                
                // interpret input
                if (rbuf[i] == '\n')
                {
                    if (log)
                    {
                        write(log, ibuf, istart + i - inorm + 1);
                    }
                    
                    if (!strncmp(ibuf, "SCALE", 5))
                    {
                        scale = ibuf[6];
                    }
                    
                    else if (!strncmp(ibuf, "PERIOD", 6))
                    {
                        ibuf[istart + i - inorm] = '\0';
                        char num[4] = {0};
                        strcpy(num, ibuf + 7);
                        period = atoi(num);
                    }
                    
                    else if (!strncmp(ibuf, "STOP", 4))
                    {
                        run = 0;
                    }
                    
                    else if (!strncmp(ibuf, "START", 5))
                    {
                        run = 1;
                    }
                    
                    else if (!strncmp(ibuf, "LOG", 3))
                    {
                    }
                    
                    else if (!strncmp(ibuf, "OFF", 3))
                    {
                        shutdown = 1;
                    }
                    
                    istart = 0;
                    inorm = i + 1;
                }
            }
            
            // move istart to ibuf continuation point
            if (rbuf[rret - 1] != '\n')
            {
                istart = rret - inorm;
            }
            inorm = 0;
        }
        
        // check shutdown
        if (shutdown)
        {
            break;
        }
    }
    
    // shutdown
    
    // get time
    time(&raw);
    cur = *localtime(&raw);
    
    // print shutdown
    sprintf(sbuf, "%02d:%02d:%02d SHUTDOWN\n", cur.tm_hour, cur.tm_min, cur.tm_sec);
    send_flush(sbuf, log);
    
    // do shutdown
    SSL_shutdown(ssl);
    SSL_free(ssl);
    
    // close sensor
    if (mraa_aio_close(tsensor) != MRAA_SUCCESS)
    {
        fprintf(stderr, "Failed to close AIO\n");
        exit(2);
    }
    
    return 0;
}

double convert(int reading, char scale)
{
    const int B = 4275;
    double R = ( 1023.0 / ((double)reading) ) - 1.0;
    R *= 100000.0;
    double temp = 1.0 / (log(R/100000.0)/B + 1/298.15) - 273.15;
    if (scale == 'F')
    {
        return temp * 9/5 + 32;
    }
    return temp;
}

void send_flush(char * sbuf, int log)
{
    int len = strlen(sbuf);
    SSL_write(ssl, sbuf, len);
    write(log, sbuf, len);
    bzero(sbuf, 25);
}
