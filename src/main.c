#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define TIMER_HOUR_SIG_CODE 2150
#define TIMER_HOUR_SIG_CODE_SHORT 253

#define TIMER_MINUTE_SIG_CODE 3754
#define TIMER_MINUTE_SIG_CODE_SHORT 263

#define TIMER_SECOND_SIG_CODE 4650
#define TIMER_SECOND_SIG_CODE_SHORT 275

#define TIMER_MAX_HOURS 120
#define TIMER_MAX_MINUTES 59
#define TIMER_MAX_SECONDS 59

#define SHELL_ERROR -1
#define FATAL_ERROR 1
#define SUCCESS 0

void cprintf(const char *color_code, const char *format, ...);

int StrToSig(const char *str, int verbose) {
  int signature = 0;
  if (!str) {
    return -1;
  }

  if (strlen(str) > 512) {
    return -2;
  }

  for (int i = 0; str[i] != '\0'; ++i) {
    signature += (int)str[i] * ((i + 1) * 1);
  }

  if (verbose) {
    cprintf("94", "Code: %ld\n", signature);
  }
  return signature;
}

void cprintf(const char *color_code, const char *format, ...) {
  time_t raw_t;
  struct tm *time_info;
  time(&raw_t);
  time_info = localtime(&raw_t);

  char time_str[32]; // Sufficient size for HH:MM:SS format
  snprintf(time_str, sizeof(time_str), "[%02d:%02d:%02d] ", time_info->tm_hour,
           time_info->tm_min, time_info->tm_sec);

  // Merge time_str and format
  size_t merged_len = strlen(time_str) + strlen(format) + 1;
  char *merged_format = malloc(merged_len);
  if (!merged_format) {
    fprintf(stderr, "Error allocating memory for merged format\n");
    return;
  }
  snprintf(merged_format, merged_len, "%s%s", time_str, format);

  // Handle variable arguments and print
  va_list args;
  va_start(args, format);

  if (color_code) {
    printf("\033[%sm", color_code);
  }

  vprintf(merged_format, args);

  if (color_code) {
    printf("\033[0m");
  }

  va_end(args);

  free(merged_format);
}

int is_numeric(const char *str) {
  for (size_t i = 0; str[i] != '\0'; i++) {
    if (!isdigit(str[i])) {
      return 0;
    }
  }
  return 1;
}

int main(int argc, char **argv) {
  if (system("date") != 0) {
    fprintf(stderr, "Shell error: date\n");
    return SHELL_ERROR;
  }
  printf("\n");

  if (argc == 1) {
    if (system("clear") != 0) {
      fprintf(stderr, "Shell error: clear\n");
      return SHELL_ERROR;
    }

    fprintf(stderr, "Missing arguments\n");
    cprintf("34", "Usage: Timer <opt1> <value1> <opt2> <value2> <...>\n");
    cprintf(
        "34",
        "       Timer --hour 1 #This will count for an hour 0 min 0 sec.\n");
    cprintf("34", "       Timer --second 5 #This will count for 5 seconds.\n");
    cprintf("34", "Options:\n");
    printf("\t\t-h | --hour\n\t\t-m | --minute\n\t\t-s | --second\n\n");
    cprintf("34", "If an option is missing it defaults to 0.\n");
    return SUCCESS;
  }

  int h = 0, m = 0, s = 0;

  while (argc > 1) {
    argc--;
    argv++;

    int sig_code = StrToSig(*argv, 0);
    switch (sig_code) {
    case TIMER_HOUR_SIG_CODE_SHORT:
    case TIMER_HOUR_SIG_CODE: {
      if (argc > 1) {
        argv++;
        argc--;
        if (is_numeric(*argv)) {
          h = atoi(*argv);
          if (h < 0 || h > TIMER_MAX_HOURS) {
            fprintf(stderr, "Invalid hour value\n");
            h = 0;
          }
        }
      }
    } break;

    case TIMER_MINUTE_SIG_CODE_SHORT:
    case TIMER_MINUTE_SIG_CODE: {
      if (argc > 1) {
        argv++;
        argc--;
        if (is_numeric(*argv)) {
          m = atoi(*argv);
          if (m < 0 || m > TIMER_MAX_MINUTES) {
            fprintf(stderr, "Invalid minute value\n");
            m = 0;
          }
        }
      }
    } break;

    case TIMER_SECOND_SIG_CODE_SHORT:
    case TIMER_SECOND_SIG_CODE: {
      if (argc > 1) {
        argv++;
        argc--;
        if (is_numeric(*argv)) {
          s = atoi(*argv);
          if (s < 0 || s > TIMER_MAX_SECONDS) {
            fprintf(stderr, "Invalid second value\n");
            s = 0;
          }
        }
      }
    } break;

    default:
      fprintf(stderr, "Unknown option: %s\n", *argv);
    }
  }

  int total_seconds = h * 3600 + m * 60 + s;

  while (total_seconds > 0) {
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;

    printf("\rTime remaining: \033[34m%02d:%02d:%02d\033[0m    ", hours,
           minutes, seconds);
    fflush(stdout);

    sleep(1);

    total_seconds--;
  }

  printf("\rTime remaining: \033[34m00:00:00\033[0m\n");
  printf("\nTimer finished!!\n");
  return SUCCESS;
}
