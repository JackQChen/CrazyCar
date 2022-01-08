using System;
using System.Web.Script.Serialization;
using WebSocketSharp.Server;

namespace CrazyCar
{
    public class Service : WebSocketBehavior
    {
        JavaScriptSerializer convert = new JavaScriptSerializer();
        internal Action<string> SendToBrowser;
        internal Action<byte[]> SendToDevice;


        public Service()
        {
            this.IgnoreExtensions = true;
            SendToBrowser = str =>
            {
                if (this.ConnectionState == WebSocketSharp.WebSocketState.Open)
                    SendAsync(str, rt => { });
            };
        }

        byte[] btCtl = new byte[3];
        protected override void OnMessage(WebSocketSharp.MessageEventArgs e)
        {
            var data = convert.Deserialize<MessageData>(e.Data);
            btCtl[0] = (byte)data.l;
            btCtl[1] = (byte)data.r;
            btCtl[2] = 0xff;
            SendToDevice(btCtl);
        }
    }
}
