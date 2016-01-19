using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace TravelDecoder
{
    public partial class Form1 : Form
    {
        List<Travel> travelList = new List<Travel>();
        ListView previousListView = new ListView();
        Dictionary<string, List<Travel>> travelDictionary = new Dictionary<string, List<Travel>>();
        ContextMenuStrip travelMenu = new ContextMenuStrip();
        
        public Form1()
        {
            InitializeComponent();
            Util.CheckFile();
            Util.ProcessTextFile("TravelList.txt", travelList, travelDictionary);
            travelMenu.Items.Add("Copy", null, CopyToClipboard);
            travelMenu.Items.Add("Delete", null, deleteButton_Click);
            travelMenu.Items.Add("Edit", null, EditListBoxItem);
            //travelMenu.Items.Add("Sort - Ascending", null, SortListviewAscending);
            //travelMenu.Items.Add("Sort - Descending", null, SortListviewDescending);
            travelListbox.Items.AddRange(travelList.Select(x => new ListViewItem(x.ToString())).ToArray());
        }

        private void searchButton_Click(object sender, EventArgs e)
        {
            if(previousListView.Items.Count < 1)
            {
                previousListView.Items.AddRange((
                    from ListViewItem item in travelListbox.Items 
                    select (ListViewItem)item.Clone()).ToArray());
            }
            
            if (searchButton.Text.Equals("Search"))
            {
                string name = searchBox.Text;
                List<Travel> searchResults = Util.FindTravel(name, travelDictionary);

                if (searchResults == null)
                {
                    MessageBox.Show(
                        "Travel could not be found, please update list and try again",
                        "Travel Not Found!",
                        MessageBoxButtons.OK,
                        MessageBoxIcon.Error);
                }
                else
                {
                    travelListbox.Items.Clear();

                    foreach (var item in searchResults)
                    {
                        travelListbox.Items.Add(item.ToString());
                    }
                    searchButton.Text = "Clear";
                    travelListbox.Items[0].Selected = true;
                    travelListbox.Focus();
                }
                searchBox.Clear();
            }
            else
            {
                travelListbox.Items.Clear();
                travelListbox.Items.AddRange((
                    from ListViewItem item in previousListView.Items
                    select (ListViewItem)item.Clone()).ToArray());
                previousListView.Items.Clear();
                searchButton.Text = "Search";
            }
        }

        private void addButton_Click(object sender, EventArgs e)
        {
            string textInput = Util.CleanName(addTextBox.Text);

            if (!string.IsNullOrEmpty(textInput))
            {
                var size = travelList.Count;
                Util.AddTravelCaller(new Travel(textInput), travelList, travelDictionary);

                if (size < travelList.Count)
                {
                    travelListbox.Items.Add(textInput);
                }
                addTextBox.Clear();
            }
        }

        private void deleteButton_Click(object sender, EventArgs e)
        {
            using (new CenterWinDialog(this))
            {
                DialogResult result = MessageBox.Show(
                    "Are you sure you want to delete?",
                    "Confirmation", MessageBoxButtons.YesNo);

                if (result == DialogResult.Yes && travelListbox.SelectedItems.Count > 0)
                {
                    string selectedTravel = travelListbox.SelectedItems[0].Text;
                    Travel badTravel = new Travel(selectedTravel);
                    travelListbox.SelectedItems[0].Remove();
                    Util.DeleteTravel(badTravel, travelList, travelDictionary);
                }
            }

        }

        private void travelListbox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.C)
            {
                Clipboard.SetText(travelListbox.SelectedItems[0].Text);
                e.SuppressKeyPress = true;
            }
        }

        private void travelListbox_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {

            if (e.Label != null)
            {
                Travel old = new Travel(travelListbox.Items[e.Item].Text);
                string newName = e.Label;
                Util.EditTravel(old, newName, travelList, travelDictionary); 
            }
        }

        private void addTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if(e.KeyCode == Keys.Enter && !string.IsNullOrEmpty(addTextBox.Text))
            {
                addButton_Click(sender, e);
            }
        }

        private void searchBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (searchButton.Text.Equals("Clear"))
            {
                searchButton_Click(sender, e);
            }
            
            if (e.KeyCode == Keys.Enter && !string.IsNullOrEmpty(searchBox.Text))
            {
                searchButton_Click(sender, e);
            }
        }

        private void travelListbox_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                if (travelListbox.FocusedItem.Bounds.Contains(e.Location) == true)
                {
                    travelMenu.Show(Cursor.Position);
                }
            } 
        }

        private void CopyToClipboard(object sender, EventArgs e)
        {
            Clipboard.SetDataObject(travelListbox.SelectedItems[0].Text, true, 5, 200);
 
            //using (new CenterWinDialog(this))
            //{
            //    DialogResult result = MessageBox.Show(
            //        "Travel could not be copied to the clipboard. " +
            //        "Please try and copy again.",
            //        "Copy Error", MessageBoxButtons.OK);          
            //}
        }

        private void importButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog fileSearch = new OpenFileDialog();

            using (new CenterWinDialog(this))
            {
                if (fileSearch.ShowDialog() == DialogResult.OK)
                {
                    Util.ProcessTextFile(fileSearch.FileName, travelList, travelDictionary);
                    travelListbox.Clear();
                    travelListbox.Items.AddRange(travelList.Select(x => new ListViewItem(x.ToString())).ToArray());
                }
            }                
        }

        private void EditListBoxItem(object sender, EventArgs e)
        {
            travelListbox.SelectedItems[0].BeginEdit();
        }

        private void travelListbox_DoubleClick(object sender, EventArgs e)
        {
            EditListBoxItem(sender, e);
        }
        //private void SortListviewAscending(object sender, EventArgs e)
        //{
        //    travelListbox.Sorting = SortOrder.Ascending;
        //    travelListbox.Sort();
        //    //travelListbox.Sorting = SortOrder.Ascending;
        //}

        //private void SortListviewDescending(object sender, EventArgs e)
        //{
        //    travelListbox.Sorting = SortOrder.Descending;
        //    travelListbox.Sort();
        //    //travelListbox.Sorting = SortOrder.Ascending;
        //}
    }
}
