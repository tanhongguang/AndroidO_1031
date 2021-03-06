#include <stdio.h>
#include <string.h>
#include <time.h>

#define LOG_NDEBUG 0
#define LOG_TAG "bootinfo"
#include <cutils/log.h>

float get_uptime(void)
{
float up_time = 0;

FILE* file = fopen("/proc/uptime", "r");

if (!file) {
ALOGD("Could not open proc/uptime\n ");
return up_time;
}

if (fscanf(file, "%f %*f", &up_time) != 1) {
ALOGD("Could not parse /proc/uptime\n");
}

fclose(file);

return up_time;
}

int get_battery_level(void)
{
int level = 0;

FILE* file = fopen("/sys/class/power_supply/battery/capacity", "r");

if (!file) {
ALOGD("Could not open proc/uptime \n ");
return level;
}

if (fscanf(file, "%d", &level) != 1) {
ALOGD("Could not parse /sys/class/power_supply/battery/capacity\n");
}

fclose(file);
return level;
}

int get_up_date(char * date, long up_time)
{
struct tm *tt;
long up_date;

time_t t = time(NULL);
up_date = t - up_time;
tt = localtime(&up_date);

sprintf(date,"%4d-%02d-%02d %02d:%02d:%02d",
tt->tm_year+1900, tt->tm_mon+1, tt->tm_mday,
tt->tm_hour, tt->tm_min, tt->tm_sec);

return 0;
}

int main(int argc ,char* argv[])
{
int times = 1;
int tmp_times = 0;
float up_time = 0.0;
int battery_level = 0;

char line[128] = {0, };
char up_date[50] = {0, };

FILE* file;

file = fopen("/data/misc/gree_init/uptime.csv", "a+");
fseek(file, 0, 0);
while (!feof(file)) {
fgets(line, sizeof(line), file);
ALOGD("line = %s", line);
}

if (strlen(line) > 0) {
sscanf(line,"%d, %*f, %*d, %*s", &tmp_times);
times += tmp_times;
} else {
fprintf(file, "times, uptime, battery_capacity, data\n");
}

up_time = get_uptime();

get_up_date(up_date, (long)up_time);

battery_level = get_battery_level();

fprintf(file,"%d, %f, %d, %s,\n", times, up_time, battery_level, up_date);
fclose(file);

}