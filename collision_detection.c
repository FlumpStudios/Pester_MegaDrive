#include "collision_detection.h"

bool checkRectangleCollision(Rectangle_t obj, Rectangle_t obj2)
{
	// Check x intersects
	if (obj.x < obj2.x + obj2.width && obj.x + obj.width > obj2.x)
	{
		// Check y intersects
		if (obj.y < obj2.y + obj2.height && obj.y + obj.height >= obj2.y)
		{
			return true;
		}
	}
	return false;
}