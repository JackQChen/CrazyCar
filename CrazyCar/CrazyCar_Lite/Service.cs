using System;
using System.Linq;
using System.Web.Script.Serialization;
using MessageLib;

namespace CrazyCar
{
    public class Service : WsServer
    {
        internal JavaScriptSerializer convert = new JavaScriptSerializer();
        internal Action<string> SendToBrowser;
        internal Action<byte[]> SendToDevice;

        public Service()
        {
            this.AddStaticContent(AppDomain.CurrentDomain.BaseDirectory);
            SendToBrowser = str =>
            {
                (this.GetSession(this.GetSessionIds().Max()) as Session).SendTextAsync(str);
            };
        }

        protected override TcpSession CreateSession()
        {
            return new Session();
        }

    }
}
