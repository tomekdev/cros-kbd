#include <string>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <errno.h>

using namespace std;

void volumedown(int relative_percent)
{
	if(relative_percent <= 0 || relative_percent >= 100)
	{
		cout << "Error: invalid relative_percent is set (not within 1-100 range)" << endl;
		return;
	}
	string command = "pactl set-sink-volume 0 -";
	char buf[4];
	sprintf(buf, "%d", relative_percent);
	command += buf;
	command += "%";
	if(system(command.c_str()) != 0)
	{
		cout << "Failed to run pactl: " << strerror(errno) << endl;
		return;
	}
}

void volumeup(int relative_percent)
{
	if(relative_percent <= 0 || relative_percent >= 100)
	{
		cout << "Error: invalid relative_percent is set (not within 1-100 range)" << endl;
		return;
	}
	string command = "pactl set-sink-volume 0 +";
	char buf[4];
	sprintf(buf, "%d", relative_percent);
	command += buf;
	command += "%";
	if(system(command.c_str()) != 0)
	{
		cout << "Failed to run amixer: " << strerror(errno) << endl;
		return;
	}
}
void volumemute()
{
	string command = "pactl set-sink-mute 0 toggle";
	if(system(command.c_str()) != 0)
	{
		cout << "Failed to run amixer: " << strerror(errno) << endl;
		return;
	}
}
