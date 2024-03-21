using System.IO.Ports;
using System.Windows;
using System.Timers;

namespace NodeView
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private SerialPort serialPort;
        private System.Timers.Timer timer;
        public MainWindow()
        {
            InitializeComponent();
            serialPort = new SerialPort("COM4", 115200);
            serialPort.Open();

            timer = new System.Timers.Timer(100);
            timer.Elapsed += OnTimerElapsed;
            timer.AutoReset = true;
            timer.Start();
        }
        private void OnTimerElapsed(object sender, ElapsedEventArgs e)
        {
            if(serialPort.IsOpen)
            {
                try 
                { 
                    string data = serialPort.ReadExisting();
                    Dispatcher.Invoke(() => ConsoleOutput.AppendText(data));
                }
                catch (Exception ex)
                {
                    Dispatcher.Invoke(() => Console.WriteLine(ex.Message));
                }
            }
        }
        private void SendData(object sender, RoutedEventArgs e)
        {
            string payload = InputTextBox.Text;
            if (serialPort.IsOpen)
            {
                serialPort.Write(payload);
                Console.WriteLine(payload);
            }else
            {
                MessageBox.Show("Serial port is not open.");
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            base.OnClosed(e);
            if(serialPort.IsOpen ) {
                serialPort.Close();
            }
        }
    }


}