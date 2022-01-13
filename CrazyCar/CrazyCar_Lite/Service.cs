using System;
using System.Collections.Generic;
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

        byte[] btCtl = new byte[4];
        protected override void OnMessage(WebSocketSharp.MessageEventArgs e)
        {
            var data = convert.Deserialize<Dictionary<string, string>>(e.Data);
            if (data.ContainsKey("l"))
            {
                btCtl[0] = 3;
                btCtl[1] = Convert.ToByte(data["l"]);
                btCtl[2] = Convert.ToByte(data["r"]);
                btCtl[3] = unchecked((byte)~(btCtl[0] + btCtl[1] + btCtl[2]));
                SendToDevice(btCtl);
            }
            else if (data.ContainsKey("motor"))
            {
                var rt = bool.Parse(data["motor"]) ? 1 : 0;
                var bt = new byte[] {
                    1,
                    (byte)rt,
                    unchecked((byte)~(1 + rt))
                };
                SendToDevice(bt);
            }
            else if (data.ContainsKey("light"))
            {
            }

        }
    }
}
