using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TravelDecoder 
{
    public class Travel : IComparable<Travel>
    {
        private string name;
        private string key;

        public Travel (string name)
        {
            Name = name;
        }

        public string Name
        {
            get { return name; }
            set
            {
                ConvertNameToKey(value);
                name = value;
            }
        }

        public string Key
        {
            get { return key; }
        }

        public void ConvertNameToKey(string name)
        {
            StringBuilder temp = new StringBuilder();

            foreach(var letter in name)
            {
                if(char.IsLetterOrDigit(letter))
                {
                    temp.Append(Char.ToLower(letter));
                }
            }
            Char[] returnValue = temp.ToString().ToCharArray();
            Array.Sort(returnValue);
            key = new String(returnValue);
        }

        public override int GetHashCode()
        {
            return Key.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            var myTravel = obj as Travel;

            if (myTravel == null)
            {
                return false;
            }

            return CompareTo(myTravel) == 0;
        }

        public int CompareTo(Travel that)
        {
            return this.Name.ToLower().CompareTo(that.Name.ToLower());
        }

        public override string ToString()
        {
            return Name;
        }
    }
}
