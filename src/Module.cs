// Module.cs
using System;
namespace glib.Host
{
    class Module
    {
        public Module() { _s = "Default"; }
        public Module(string s) { _s = s; }

    public void SetString(string s) { _s = s; }
    public String GetString() { return _s; }

    private string _s = "Default";
    }
}
