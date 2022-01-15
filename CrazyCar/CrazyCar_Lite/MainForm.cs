using System;
using System.Collections.Concurrent;
using System.IO.Ports;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace CrazyCar
{
    public class MainForm
    {
        Service server;
        SerialPort serialPort;
        BlockingCollection<byte[]> sendQueue = new BlockingCollection<byte[]>();
        Action<string> actLog;
        Action<string> actSendToBrowser;

        int lineCount = 0;

        public MainForm()
        {
            actLog = new Action<string>(log =>
            {
                lineCount++;
                if (lineCount > 100)
                {
                    lineCount = 0;
                    Console.Clear();
                }
                Console.Write(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "\r\n" + log + "\r\n");
            });
            Task.Factory.StartNew(() =>
            {
                while (true)
                {
                    try
                    {
                        var btData = sendQueue.Take();
                        serialPort.Write(btData, 0, btData.Length);
                        Log("send:" + BitConverter.ToString(btData).Replace("-", " "));
                    }
                    catch (Exception ex)
                    {
                        if (ex as ThreadAbortException == null)
                            Log(ex.StackTrace + "\r\n" + ex.Message);
                    }
                }
            });
            Task.Factory.StartNew(() =>
            {
                byte[] btArr = new byte[1024];
                var count = 0;
                var strBuffer = new StringBuilder();
                while (true)
                {
                    try
                    {
                        if (!serialPort.IsOpen)
                        {
                            Thread.Sleep(1000);
                            continue;
                        }
                        count = serialPort.BytesToRead;
                        if (count > 0)
                        {
                            serialPort.Read(btArr, 0, count);
                            strBuffer.Append(Encoding.UTF8.GetString(btArr, 0, count));
                            var strFull = strBuffer.ToString();
                            var lastIndex = strFull.LastIndexOf('\n');
                            if (lastIndex > -1)
                            {
                                foreach (var strLine in strFull.Substring(0, lastIndex).Split("\n".ToCharArray(), StringSplitOptions.RemoveEmptyEntries))
                                {
                                    Log("recv:" + strLine);
                                    if (actSendToBrowser != null)
                                        actSendToBrowser(strLine);
                                }
                                strBuffer.Clear();
                                strBuffer.Append(strFull.Substring(lastIndex + 1));
                            }
                        }
                        else
                            Thread.Sleep(10);
                    }
                    catch (Exception ex)
                    {
                        if (ex as ThreadAbortException == null)
                            Log(ex.StackTrace + "\r\n" + ex.Message);
                    }
                }
            });
            try
            {
                serialPort = new SerialPort("/dev/ttyAMA0", 9600);
                serialPort.Open();
                Log("SerialPort initialization complete");
            }
            catch
            {
                Log("SerialPort init failed");
            }
            server = new Service();
            server.SendToDevice = btArr =>
            {
                SendToDevice(btArr);
            };
            actSendToBrowser = btArr =>
            {
                server.SendToBrowser(btArr);
            };
            server.Endpoint.Port = 9527;
            server.Initialize();
            server.Start();
        }

        internal void MainForm_FormClosing(object sender)
        {
            serialPort.Close();
            server.Stop();
        }

        void Log(string log)
        {
            actLog(log);
        }

        public void SendData(byte[] btData)
        {
            sendQueue.Add(btData);
        }

        public void SendToDevice(byte[] btData)
        {
            SendData(btData);
        }

        // Motor = 1, Light = 2, Control = 3

        private void btnMotor_Click(object sender, EventArgs e)
        {
            var data = new byte[] { 1, 1, unchecked((byte)~(1 + 1)) };
            SendData(data);
        }

        private void btnLight_Click(object sender, EventArgs e)
        {
            var data = new byte[] { 2, 1, unchecked((byte)~(2 + 1)) };
            SendData(data);
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            //var str = this.txtData.Text + '\n';
            //this.txtData.Clear();
            //var btList = str.ToCharArray().Select(s => (byte)s);
            //SendData(btList.ToArray());
        }
    }
}
