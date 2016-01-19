using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace TravelDecoder
{
    public static class Util
    {
        private static string fileName = "TravelList.txt";

        public static void CheckFile()
        {
            if(!File.Exists(fileName))
            {
                File.Create(fileName).Close();
            }
        }

        public static void EditTravel(Travel oldTravel, string newName, List<Travel> travelList, Dictionary<string, List<Travel>> travelDictionary)
        {
            Travel newTravel = new Travel(CleanName(newName));
            DeleteTravel(oldTravel, travelList, travelDictionary);
            AddTravelCaller(newTravel, travelList, travelDictionary);
        }

        public static void DeleteTravel(Travel oldTravel, List<Travel> travelList, Dictionary<string, List<Travel>> travelDictionary)
        {
            travelList.Remove(oldTravel);
            travelDictionary[oldTravel.Key].Remove(oldTravel);

            if(!travelDictionary[oldTravel.Key].Any())
            {
                travelDictionary.Remove(oldTravel.Key);
            }

            UpdateTextFile(fileName, travelList, false);
        }

        public static string CleanName(string name)
        {
            name = Regex.Replace(name, @"[^\u0000-\u007F]", string.Empty);
            return name.Trim();
        }

        public static List<Travel> FindTravel(string name, Dictionary<string, List<Travel>> travelDictionary)
        {
            List<Travel> possibleTravels = new List<Travel>();
            Travel target = new Travel(CleanName(name));

            if (travelDictionary.ContainsKey(target.Key))
            {
                return travelDictionary[target.Key];
            }
            return null;
        }

        public static void AddTravelCaller(Travel newTravel, List<Travel> travelList, Dictionary<string, List<Travel>> travelDictionary)
        {
            AddTravel(newTravel, travelList, travelDictionary);
            UpdateTextFile(fileName, newTravel);
        }
  
        public static void AddTravel(Travel newTravel, List<Travel> travelList, Dictionary<string, List<Travel>> travelDictionary)
        {
            if(!string.IsNullOrEmpty(newTravel.Name))
            {
                if (!travelList.Contains(newTravel))
                {
                    travelList.Add(newTravel);
                }

                if (travelDictionary.ContainsKey(newTravel.Key))
                {
                    if (!travelDictionary[newTravel.Key].Contains(newTravel))
                    {
                        travelDictionary[newTravel.Key].Add(newTravel);
                    }
                }
                else
                {
                    List<Travel> newTravelList = new List<Travel>();
                    newTravelList.Add(newTravel);
                    travelDictionary.Add(newTravel.Key, newTravelList);
                }
            }
        }

        public static void ProcessTextFile(string input, List<Travel> travelList, Dictionary<string, List<Travel>> travelDictionary)
        {

            try
            {  
                using (StreamReader sr = new StreamReader(@input))
                {

                    while(!sr.EndOfStream)
                    {
                        String travelName = sr.ReadLine();
                        AddTravel(new Travel(CleanName(travelName)), travelList, travelDictionary);
                    }
                }
            }
            catch (Exception e)
            {
                    MessageBox.Show(
                    "Please close the file and try again\n" + e.Message,
                    "Could Not Read " + fileName, 
                    MessageBoxButtons.OK, 
                    MessageBoxIcon.Error);
            }
            UpdateTextFile(fileName, travelList, false);
        }

        public static void UpdateTextFile(string fileName, Travel newTravel)
        {
            List<Travel> temp = new List<Travel>();
            temp.Add(newTravel);
            UpdateTextFile(fileName, temp , true);
        }

        public static void UpdateTextFile(string fileName, List<Travel> travelList, bool append) 
        {
            string newName = fileName.Substring(0, (fileName.Length - 4)) + "_Old.txt";
            File.Copy(fileName, newName, true);

            try
            {
                using (StreamWriter sw = new StreamWriter(fileName, append))
                {
                    foreach(var travel in travelList)
                    {
                        sw.WriteLine(travel.Name);
                    }
                }
            }
            catch (Exception e)
            {
                MessageBox.Show(
                "Please close the file and try again\n" + e.Message,
                "Could Not Write " + fileName,
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);
            }
        }
    }
}
