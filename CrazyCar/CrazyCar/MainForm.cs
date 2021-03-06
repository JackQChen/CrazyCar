using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using WebSocketSharp;
using WebSocketSharp.Net;
using WebSocketSharp.Server;

namespace CrazyCar
{
    public partial class MainForm : Form
    {
        HttpServer server;
        SerialPort serialPort;
        BlockingCollection<byte[]> sendQueue = new BlockingCollection<byte[]>();
        Action<string> actLog;
        Action<string> actSendToBrowser;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            actLog = new Action<string>(log =>
            {
                if (this.txtLog.Lines.Length > 100)
                    this.txtLog.Clear();
                this.txtLog.AppendText(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "\r\n" + log + "\r\n");
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
                        Log(ex.StackTrace + "\r\n" + ex.Message);
                    }
                }
            });
            try
            {
                serialPort = new SerialPort("/dev/ttyAMA0", 9600);
                serialPort.Open();
            }
            catch
            {
                Log("SerialPort init failed");
            }
            server = new HttpServer(9527);
            server.DocumentRootPath = AppDomain.CurrentDomain.BaseDirectory;
            server.OnGet += (s, a) =>
            {
                var req = a.Request;
                var res = a.Response;
                var path = req.RawUrl;
                if (path == "/")
                    path += "index.html";
                byte[] contents;
                if (!a.TryReadFile(path, out contents))
                {
                    res.StatusCode = (int)HttpStatusCode.NotFound;
                    return;
                }
                if (path.EndsWith(".html"))
                {
                    res.ContentType = "text/html";
                    res.ContentEncoding = Encoding.UTF8;
                }
                else if (path.EndsWith(".js"))
                {
                    res.ContentType = "application/javascript";
                    res.ContentEncoding = Encoding.UTF8;
                }
                res.ContentLength64 = contents.LongLength;
                res.Close(contents, true);
            };
            server.AddWebSocketService<Service>("/", service =>
            {
                service.SendToDevice = btArr =>
                {
                    SendToDevice(btArr);
                };
                actSendToBrowser = btArr =>
                {
                    service.SendToBrowser(btArr);
                };
                Log("WebSocket connected.");
            });
            server.Start();
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            serialPort.Close();
            server.Stop();
        }

        void Log(string log)
        {
            this.txtLog.Invoke(actLog, log);
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
