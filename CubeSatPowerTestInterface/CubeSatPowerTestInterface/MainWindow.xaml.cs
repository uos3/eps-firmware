using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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

namespace CubeSatPowerTestInterface
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        RegisterInterface regs;
        SerialLayer serial;
        RailModel[] models;
        DispatcherTimer requestTimer;
        PacketMonitor monitor;
        int _dutReceivePacketCount = 0;
        int _dutTransmitPacketCount = 0;

        public MainWindow()
        {
            InitializeComponent();

            serial = new SerialLayer();
            serial.PacketReceived += Serial_PacketReceived;
            serial.PacketSent += Serial_PacketSent;
            if (!serial.Open("COM4", 57600))
            {
                MessageBox.Show("Could not open serial port");
                Close();
                return;
            }
            monitor = new PacketMonitor(serial);
            monitor.DutPacketReceived += Monitor_DutPacketReceived;
            regs = new RegisterInterface(serial, monitor);

            models = new RailModel[]
            {
                new RailModel(0, "Flight MCU"),
                new RailModel(1, "Camera"),
                new RailModel(2, "GNSS Receiver"),
                new RailModel(3, "GNSS LNA"),
                new RailModel(4, "Radio"),
                new RailModel(5, "Backup Radio")
            };

            for (int i = 0; i < Enum.GetNames(typeof(Register)).Length; i++)
            {
                var regModel = new RegisterModel();
                regModel.RegId = ((Register)i);
                regs.AddMonitor(regModel);

                var view = new RegisterView();
                regStack.Children.Add(view);
                view.DataContext = regModel;
            }

            RailView[] views = new RailView[]
            {
                railView1,
                railView2,
                railView3,
                railView4,
                railView5,
                railView6,
            };
            for (int i = 0; i < models.Length; i++)
            {
                var m = models[i];
                m.EnabledChanged += Rail_EnabledChanged;
                regs.AddMonitor(m);
                views[i].DataContext = m;
            }

            requestTimer = new DispatcherTimer();
            requestTimer.Interval = TimeSpan.FromSeconds(1);
            requestTimer.Tick += RequestTimer_Tick;
            requestTimer.Start();

            RequestIntState();
        }

        private void Monitor_DutPacketReceived(PacketMonitor sender, byte[] packet)
        {
            _dutReceivePacketCount++;
            Dispatcher.Invoke(new Action(() => recvDutRun.Text = _dutReceivePacketCount.ToString()));
        }

        private void RequestIntState()
        {
            serial.WritePacket(new byte[] { 0x03, 0x00 });
        }

        private void Serial_PacketSent(SerialLayer serial, byte[] packet)
        {
            _dutTransmitPacketCount++;
            Dispatcher.Invoke(new Action(() => tranDutRun.Text = _dutTransmitPacketCount.ToString()));
            PushToUi("TX", packet);
        }

        private void Serial_PacketReceived(SerialLayer serial, byte[] packet)
        {
            PushToUi("RX", packet);
            byte header = packet[0];
            switch (header)
            {
                case 0x81:
                    PushIntToUi(packet[2]);
                    break;
            }
        }
        private void PushIntToUi(int value)
        {
            Dispatcher.Invoke(new Action(() =>
            {
                intPinRun.Text = value.ToString();
            }));
        }
        private void PushToUi(string dir, byte[] packet)
        {
            Dispatcher.Invoke(new Action(() =>
            {
                // Byte array to hex string from http://stackoverflow.com/a/311382
                var txt = dir + ": " + String.Join(" ", packet.Select(x => x.ToString("X2")));
                receivedDataLst.Items.Insert(0, new ListViewItem
                {
                    Content = txt
                });
            }));
        }

        private void RequestTimer_Tick(object sender, EventArgs e)
        {
            regs.Get(Register.Config, regStack.Children.Count);
        }

        private void Rail_EnabledChanged(RailModel sender, bool value)
        {
            ushort mask = 0;
            foreach(var m in models)
                if (m.Enabled)
                    mask |= (ushort)(1 << m.Id);
            regs.Set(Register.SW_On, mask);
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            int regStart;
            ushort value;
            if (int.TryParse(regTxt.Text, out regStart) && ushort.TryParse(valueTxt.Text, out value))
            {
                if (writeChk.IsChecked.HasValue && writeChk.IsChecked.Value)
                    regs.Set((Register)regStart, value);
                else
                    regs.Get((Register)regStart, value);
            }
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            serial.Dispose();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            regs.Get(Register.Config, regStack.Children.Count);
        }

        public static byte[] StringToByteArray(string hex)
        {
            int NumberChars = hex.Length;
            byte[] bytes = new byte[NumberChars / 2];
            for (int i = 0; i < NumberChars; i += 2)
                bytes[i / 2] = Convert.ToByte(hex.Substring(i, 2), 16);
            return bytes;
        }

        private void SendBtn_Click(object sender, RoutedEventArgs e)
        {
            var textHex = inputTxt.Text.Replace(" ", "");
            if (textHex.Length % 2 != 0)
            {
                MessageBox.Show("Invalid packet");
                return;
            }
            try
            {
                var packet = StringToByteArray(textHex);
                serial.WritePacket(packet);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Invalid packet: {ex}");
                return;
            }
        }
        
        private void receivedDataLst_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            var item = receivedDataLst.SelectedItem as ListViewItem;
            if (item != null)
                inputTxt.Text = (item.Content as string)?.Substring(4);
        }
    }
}
