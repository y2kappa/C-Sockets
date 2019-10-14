#include "utils.h"
#include <stdio.h>
#include <time.h>
#include <strings.h>
#include <sys/time.h>

void timenow(char buffer[],  unsigned int size_buffer)
{
    bzero(buffer, size_buffer);
    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(buffer, size_buffer, "%X", timeinfo);

    buffer[1023] = '\0';

}

ssize_t format_timeval(struct timeval *tv, char *buf, size_t sz)
{
  ssize_t written = -1;
  struct tm *gm = gmtime(&tv->tv_sec);

  if (gm)
  {
    written = (ssize_t)strftime(buf, sz, "%Y-%m-%dT%H:%M:%S", gm);
    if ((written > 0) && ((size_t)written < sz))
    {
      int w = snprintf(buf+written, sz-(size_t)written, ".%06dZ", tv->tv_usec);
      written = (w > 0) ? written + w : -1;
    }
  }
  return written;
}

void current_timestamp(char buf[], unsigned int size_buffer) {
    struct timeval te;

    // char buf[28];
    gettimeofday(&te, NULL); // get current time


    long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds


    format_timeval(&te, buf, size_buffer);

    // printf("%s milliseconds: %lld\n", buf, milliseconds);

    // return buf;
}

