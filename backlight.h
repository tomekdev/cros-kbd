#ifndef _BACKLIGHT_H_
#define _BACKLIGHT_H_

#include "types.h"

void brightnessup(struct backlight_properties properties, int num_steps);
void brightnessdown(struct backlight_properties properties, int num_steps);
int get_backlight_properties(struct backlight_properties &properties);

#endif /* _BACKLIGHT_H_ */