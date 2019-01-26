#include <string>
#include <fstream>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <cstdlib>
#include <linux/input.h>
#include "backlight.h"
#include "types.h"

using namespace std;

std::string devpth;

int find_cros_ec() /* Finds cros_ec input node for opening eg. /dev/input/event0 */
{
	int eventnum = 0;
	bool found = false;
	while(!found) {
		char eventnum_str[16];
		sprintf(eventnum_str, "%d", eventnum);
		std::string path;
		path = "/sys/class/input/event";
		path = path + eventnum_str;
		path += "/device/name";
		std::fstream eventfile;
		eventfile.open(path, std::ios::in);
		if(eventfile.good())
		{
			std::string buf;
			getline(eventfile, buf);
			if(buf == "cros_ec")
			{
				found = true;
				devpth = "/dev/input/event";
				devpth += eventnum_str;
				std::cout << "Found cros_ec keyboard as: " << devpth << endl;
			}
			eventfile.close();
		}
		else
		{
			if(errno == ENOENT && eventnum == 0)
			{
				cout << "Failed to find cros_ec keyboard (is sysfs mounted ?)" << endl;
				break;
			} else if(errno == ENOENT && eventnum != 0 && !found) {
				cout << "Device cros_ec keyboard was not found" << endl;
				break;
			}
			else
			{
				cout << "Failed to open " << path << ": " << strerror(errno) << endl;
				break;
			}
		}
		eventnum++;
	}
}

bool die = false;

/* Keycodes */
int bklup_code = 65;
int bkldn_code = 64;
int volup_code = 68;
int voldn_code = 67;
int mute_code = 66;
int searchbtn_code = 125;

/* Our "function" key state */
bool searchbtn = false;

/* Percent that is volume increased or decreased by */
int relative_percent = 5;

void interpret_event(struct input_event event)
{
	if(event.code == searchbtn_code && event.type == EV_KEY && event.value == 1)
	{
		searchbtn = true;
	}
	if(event.code == searchbtn_code && event.type == EV_KEY && event.value == 0)
	{
		searchbtn = false;
	}
	if(event.code == bklup_code && event.type == EV_KEY && event.value == 1 && searchbtn)
	{
		struct backlight_properties prop;
		if(get_backlight_properties(prop) != 0) return;

		brightnessup(prop, 10);
	}
	if(event.code == bkldn_code && event.type == EV_KEY && event.value == 1 && searchbtn)
	{
		struct backlight_properties prop;
		if(get_backlight_properties(prop) != 0) return;

		brightnessdown(prop, 10);
	}
//	if(event.code == mute_code && event.type == EV_KEY && event.value == 1 && searchbtn)
//	{
//		volumemute();
//	}
//	if(event.code == voldn_code && event.type == EV_KEY && event.value == 1 && searchbtn)
//	{
//		volumedown(relative_percent);
//	}
//	if(event.code == volup_code && event.type == EV_KEY && event.value == 1 && searchbtn)
//	{
//		volumeup(relative_percent);
//	}
}

int main()
{
	find_cros_ec();
	if(devpth == "")
	{
		std::cout << "Fatal error occurred. Exiting" << endl;
		return -1;
	}
	FILE *kbfd = fopen(devpth.c_str(), "rb");
	if(kbfd)
	{
		struct input_event buf;
		while(!die)
		{
			fread(&buf, 1, 16, kbfd);
			interpret_event(buf);
		}
		fclose(kbfd);
	}
	else
	{
		cout << "Failed to open " << devpth << " for reading: " << strerror(errno) << endl;
	}
	return 0;
}
