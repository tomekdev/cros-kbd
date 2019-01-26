#include <iostream>
#include "types.h"
#include <fstream>
#include <string>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define BACKLIGHT_SYS "/sys/class/backlight/backlight"

using namespace std;

int get_backlight_properties(struct backlight_properties &properties)
{
	fstream fbrightness;
	string brightness_path = BACKLIGHT_SYS;
	brightness_path += "/brightness";
	fbrightness.open(brightness_path, std::ios::in);
	if(fbrightness.good())
	{
		string buf;
		getline(fbrightness, buf);
		properties.cur_brightness = strtol(buf.c_str(), NULL, 10);
		fbrightness.close();
	}
	else if (errno == ENOENT)
	{
		cout << "Error: " << brightness_path << " was not found (is sysfs mounted or backlight driver loaded ?)" << endl;
	} else {
		cout << "Error: failed to open " << brightness_path << ": " << strerror(errno) << endl;
		return -1;
	}
	fstream fmax_brightness;
	string max_brightness_path = BACKLIGHT_SYS;
	max_brightness_path += "/max_brightness";
	fmax_brightness.open(max_brightness_path, std::ios::in);
	if(fmax_brightness.good())
	{
		string buf;
		getline(fmax_brightness, buf);
		properties.max_brightness = strtol(buf.c_str(), NULL, 10);
		fmax_brightness.close();
	}
	else
	{
		cout << "Error: failed to open " << max_brightness_path << ": " << strerror(errno) << endl;
		return -1;
	}

	return 0;
}
void brightnessup(struct backlight_properties properties, int num_steps)
{
	/* Calculate step */
	int step = properties.max_brightness / num_steps;
	if(step < 1 )
	{
		cout << "Invalid number of steps specified (steps > max_brightness)" << endl;
		return;
	}
	int new_brightness = properties.cur_brightness + step;
	if(new_brightness > properties.max_brightness) new_brightness = properties.max_brightness;

	/* Add reservation for levels 0-5 */
	if(properties.cur_brightness >= 0 && properties.cur_brightness <= 5) new_brightness = properties.cur_brightness + 1;

	fstream fbrightness;
	string brightness_path = BACKLIGHT_SYS;
	brightness_path += "/brightness";
	fbrightness.open(brightness_path, std::ios::out);
	if(fbrightness.good())
	{
		char buf[16];
		sprintf(buf, "%d", new_brightness);
		string w = buf;
		fbrightness << w;
		fbrightness.close();
	}
	else
	{
		cout << "Failed to open " << brightness_path << ": " << strerror(errno) << endl;
		return;
	}
}

void brightnessdown(struct backlight_properties properties, int num_steps)
{
	/* Calculate step */
	int step = properties.max_brightness / num_steps;
	if(step < 1 )
	{
		cout << "Invalid number of steps specified (steps > max_brightness)" << endl;
		return;
	}
	int new_brightness = properties.cur_brightness - step;
	if(new_brightness < 0) new_brightness = 0;

	/* Add reservation for levels 0-5 */
	if(properties.cur_brightness >= 0 && properties.cur_brightness <= 5) new_brightness = properties.cur_brightness - 1;

	fstream fbrightness;
	string brightness_path = BACKLIGHT_SYS;
	brightness_path += "/brightness";
	fbrightness.open(brightness_path, std::ios::out);
	if(fbrightness.good())
	{
		char buf[16];
		sprintf(buf, "%d", new_brightness);
		string w = buf;
		fbrightness << w;
		fbrightness.close();
	}
	else
	{
		cout << "Failed to open " << brightness_path << ": " << strerror(errno) << endl;
		return;
	}
}
