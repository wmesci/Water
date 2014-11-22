#include "Water.h"

void Water::Updata()
{
	for(int y = 0; y < Height; y++)
	{
        int n = y * Width;
        for (int x = 0; x < Width; x++, n++)
        {
            float s = GetValue(buf1, x, y - 1) + GetValue(buf1, x, y + 1) + GetValue(buf1, x - 1, y) + GetValue(buf1, x + 1, y);
            s = (s / 2 - buf2[n]);
            s -= s / 128;
            if (s > 2) s = 2;
            if (s < -2) s = -2;
            buf2[n] = s;
        }
	}
	auto temp = buf1;
    buf1 = buf2;
    buf2 = temp;
}

void Water::Drop(float xi, float yi)
{
	int px = (int)(xi * (Width - 1));
    int py = (int)(yi * (Height - 1));
    for (int j = py - r; j <= py + r; j++)
    {
        for (int i = px - r; i <= px + r; i++)
        {
            float dx = i - px;
            float dy = j - py;
            float a = (float)(1 - (dx * dx + dy * dy) / (r * r));
            if (a > 0 && a <= 1)
            {
                SetValue(buf1, i, j, a * h);
            }
        }
    }
}

Water::Water(int w, int h)
	: Width(w), Height(h)
{
	size = w * h * 4;
	buf1 = (float*)malloc(size);
	buf2 = (float*)malloc(size);

	memset(buf1, 0, size);
	memset(buf2, 0, size);
}

Water::~Water(void)
{
	if(buf1 != NULL)
	{
		free(buf1);
		buf1 = NULL;
	}
	if(buf2 != NULL)
	{
		free(buf2);
		buf2 = NULL;
	}
}
