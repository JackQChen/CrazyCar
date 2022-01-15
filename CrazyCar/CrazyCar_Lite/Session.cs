using System;
using System.Collections.Generic;
using System.Text;
using MessageLib;

namespace CrazyCar
{
    public class Session : WsSession
    {
        Service service;

        public Session()
        {
        }

        protected override void Initialize()
        {
            this.service = this.Server as Service;
        }

        public override void OnWsConnected(HttpRequest request)
        {
            this.SendTextAsync("[ID=" + this.Id + "] Connected!");
            Console.WriteLine("WebSocket connected.");
        }

        byte[] btCtl = new byte[6];
        public override void OnWsReceived(byte[] buffer, int offset, int size)
        {
            var data = this.service.convert.Deserialize<Dictionary<string, string>>(Encoding.UTF8.GetString(buffer, offset, size));
            if (data.ContainsKey("l"))
            {
                btCtl[0] = 3;
                var bt = BitConverter.GetBytes(Convert.ToInt16(data["l"]));
                btCtl[1] = bt[1];
                btCtl[2] = bt[0];
                bt = BitConverter.GetBytes(Convert.ToInt16(data["r"]));
                btCtl[3] = bt[1];
                btCtl[4] = bt[0];
                btCtl[5] = unchecked((byte)~(btCtl[0] + btCtl[1] + btCtl[2] + btCtl[3] + btCtl[4]));
                this.service.SendToDevice(btCtl);
            }
            else if (data.ContainsKey("motor"))
            {
                var rt = bool.Parse(data["motor"]) ? 1 : 0;
                var bt = new byte[] {
                            1,
                            (byte)rt,
                            unchecked((byte)~(1 + rt))
                        };
                this.service.SendToDevice(bt);
            }
            else if (data.ContainsKey("light"))
            {
            }
        }

    }
}
