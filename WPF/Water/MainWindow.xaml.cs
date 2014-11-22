using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Runtime.InteropServices;

namespace Water
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        WaterEffect water;
        public MainWindow()
        {
            InitializeComponent();

            water = new WaterEffect(160, 120);
            back.Effect = water;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            DropWater();
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            if (Mouse.LeftButton == MouseButtonState.Pressed) DropWater();
        }

        private void DropWater() 
        {
            var p = Mouse.GetPosition(back);
            p.X /= back.RenderSize.Width;
            p.Y /= back.RenderSize.Height;
            if (p.X >= 0 && p.X <= 1 && p.Y >= 0 && p.Y <= 1)
                water.Drop((float)p.X, (float)p.Y);
        }
    }
}
