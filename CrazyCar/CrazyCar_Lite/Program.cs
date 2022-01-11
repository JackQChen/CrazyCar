using System;

namespace CrazyCar
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            var frm = new MainForm();
            while (true)
            {
                var cmd = Console.ReadLine();
                if (cmd == "q")
                {
                    frm.MainForm_FormClosing(null);
                    break;
                }
            }
        }
    }
}
