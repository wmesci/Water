using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using SlimDX.Direct3D9;
using SlimDX;
using System.Runtime.InteropServices;
using System.IO;

namespace Water
{
    public unsafe partial class MainForm : Form
    {
        Direct3D Direct3D;
        PresentParameters PresentParameters;
        Device Device;
        VertexBuffer vertices;
        Effect effect;
        Water water;
        Texture map;
        public MainForm()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.Opaque, true);
            InitializeComponent();
            Initialize();
        }

        private void Initialize() 
        {
            var width = this.ClientSize.Width;
            var height = this.ClientSize.Height;
            water = new Water(160, 120);

            Direct3D = new Direct3D();

            PresentParameters = new PresentParameters();
            PresentParameters.BackBufferFormat = Format.X8R8G8B8;
            PresentParameters.BackBufferCount = 1;
            PresentParameters.BackBufferWidth = width;
            PresentParameters.BackBufferHeight = height;
            PresentParameters.SwapEffect = SwapEffect.Discard;
            PresentParameters.Windowed = true;
            PresentParameters.DeviceWindowHandle = this.Handle;

            Device = new Device(Direct3D, 0, DeviceType.Hardware, this.Handle, CreateFlags.HardwareVertexProcessing, PresentParameters);

            Device.SetTexture(0, Texture.FromFile(Device, "back.jpg"));

            map = new Texture(Device, water.Width, water.Height, 0, Usage.None, Format.R32F, Pool.Managed);

            effect = Effect.FromString(Device, Properties.Resources.Shader, ShaderFlags.None);

            //声明顶点
            vertices = new VertexBuffer(Device, 4 * 24, Usage.WriteOnly, VertexFormat.None, SlimDX.Direct3D9.Pool.Managed);
            var dat = vertices.Lock(0, 0, LockFlags.None);
            dat.WriteRange(new float[] { 0,     0,      0f, 1f, 0, 0 });
            dat.WriteRange(new float[] { width, 0,      0f, 1f, 1, 0 });
            dat.WriteRange(new float[] { 0,     height, 0f, 1f, 0, 1 });
            dat.WriteRange(new float[] { width, height, 0f, 1f, 1, 1 });
            vertices.Unlock();

            //声明顶点格式
            var vertexElems = new[] 
            {
        		new VertexElement(0, 0, DeclarationType.Float4, DeclarationMethod.Default, DeclarationUsage.PositionTransformed, 0),
        		new VertexElement(0, 16, DeclarationType.Float2, DeclarationMethod.Default, DeclarationUsage.TextureCoordinate, 0),
				VertexElement.VertexDeclarationEnd
        	};

            Device.VertexDeclaration = new VertexDeclaration(Device, vertexElems);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            Device.Clear(ClearFlags.Target | ClearFlags.ZBuffer, new Color4(0f, 0f, 0f), 1.0f, 0);
 
            effect.Technique = "simple";
            Device.BeginScene();
            {
                effect.Begin();
                {
                    effect.BeginPass(0);
                    {
                        effect.SetValue<float>("dx", 1f / (water.Width - 1));
                        effect.SetValue<float>("dy", 1f / (water.Height - 1));
                        effect.SetTexture("Height", map);
                        Device.SetStreamSource(0, vertices, 0, 24);
                        Device.DrawPrimitives(PrimitiveType.TriangleStrip, 0, 2);
                    }
                    effect.EndPass();
                }
                effect.End();
            }

            Device.EndScene();
            Device.Present();
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            base.OnClosing(e);
            if (e.Cancel) return;
            if (Device != null) Device.Dispose();
            if (Direct3D != null) Direct3D.Dispose();
            if (water != null) water.Dispose();
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            if (MouseButtons.HasFlag(MouseButtons.Left))
                water.Drop(e.X / (float)this.ClientSize.Width, e.Y / (float)this.ClientSize.Height);
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            OnMouseDown(e);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            water.Updata();
            map.LockRectangle(0, LockFlags.DoNotCopyData).Data.WriteRange(water.Data);
            map.UnlockRectangle(0);
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            Refresh();
        }

        private void Drop(float xi, float yi) 
        {
            water.Drop(xi, yi);
        }
    }
}
