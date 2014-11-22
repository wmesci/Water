#include <Windows.h>

#pragma once
class Water
{
private:
	static const int r = 5;
    static const int h = -1;
	int size;
	float* buf1;
	float* buf2;
	
public:
	const int Width;
	const int Height;

private:
	float GetValue(float* buf, int x, int y)
    {
        if (x < 0)
            x = 0;
        else if (x > Width - 1)
            x = Width - 1;

        if (y < 0)
            y = 0;
        else if (y > Height - 1)
            y = Height - 1;

        return buf[y * Width + x];
    }

    void SetValue(float* buf, int x, int y, float value)
    {
        if (x < 0)
            x = 0;
        else if (x > Width - 1)
            x = Width - 1;

        if (y < 0)
            y = 0;
        else if (y > Height - 1)
            y = Height - 1;

        buf[y * Width + x] = value;
    }

public:
	Water(int w, int h);
	void Updata();
	void Drop(float xi, float yi);
	void CopyTo(void* buffer) { memcpy(buffer, buf2, size); }
	~Water(void);
};

