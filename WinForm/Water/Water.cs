using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Water
{
    unsafe class Water : IDisposable
    {
        [DllImport("kernel32.dll", EntryPoint = "RtlMoveMemory")]
        static extern void CopyMemory(void* Destination, void* Source, int Length);

        [DllImport("kernel32.dll", EntryPoint = "RtlZeroMemory")]
        static extern void ZeroMemory(void* Destination, int Length);

        readonly int size;
        public Water(int w, int h)
        {
            this.Width = w;
            this.Height = h;
            data = new float[w * h];
            size = w * h * 4;
            this.buf1 = (float*)Marshal.AllocHGlobal(size);
            this.buf2 = (float*)Marshal.AllocHGlobal(size);
            ZeroMemory(buf1, size);
            ZeroMemory(buf2, size);
        }

        public readonly int Width;

        public readonly int Height;

        float[] data;
        public float[] Data
        {
            get
            {
                fixed (float* p = &data[0])
                    CopyMemory(p, buf1, size);
                return data;
            }
        }

        float* buf1, buf2;
        public void Updata()
        {
            Action<int> act = y =>
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
            };
            Parallel.For(0, Height, act);
            var temp = buf1;
            buf1 = buf2;
            buf2 = temp;
        }

        private float GetValue(float* buf, int x, int y)
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

        private void SetValue(float* buf, int x, int y, float value)
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

        int r = 5;
        float h = -1f;
        public void Drop(float xi, float yi)
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

        public void Dispose()
        {
            if ((IntPtr)buf1 != IntPtr.Zero) Marshal.FreeHGlobal((IntPtr)buf1);
            if ((IntPtr)buf2 != IntPtr.Zero) Marshal.FreeHGlobal((IntPtr)buf2);
            GC.SuppressFinalize(this);
        }

        ~Water()
        {
            Dispose();
        }
    }
}
