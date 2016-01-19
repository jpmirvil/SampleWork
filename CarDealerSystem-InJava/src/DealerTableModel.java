import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

import javax.swing.JOptionPane;
import javax.swing.table.AbstractTableModel;

@SuppressWarnings("serial")
public class DealerTableModel extends AbstractTableModel
{
	private String[] columnHeaders =
	{ "Name", "Mileage", "Price", "Year", "Cylinders", "Fuel Capacity",
			"Horse Power" };
	private Object[][] data;
	private ArrayList<Car> carList;

	public DealerTableModel(ArrayList<Car> carList, String path)
	{
		this.carList = carList;
		initializeCarList(path);
	}

	public DealerTableModel(ArrayList<Car> carList, boolean update)
	{
		setNewSource(carList);
	}

	public void setNewSource(ArrayList<Car> newList)
	{
		carList = newList;
		data = new Object[newList.size()][columnHeaders.length];

		for (int i = 0; i < newList.size(); i++)
		{
			data[i][0] = newList.get(i).getCarName();
			data[i][1] = newList.get(i).getMileage();
			data[i][2] = newList.get(i).getPrice();
			data[i][3] = newList.get(i).getYear();
			data[i][4] = newList.get(i).getCylinders();
			data[i][5] = newList.get(i).getFuelCapacity();
			data[i][6] = newList.get(i).getHorsePower();
		}
	}

	public void initializeCarList(String path)
	{
		try
		{
			Scanner input = new Scanner(new File(path));
			int carCount = Integer.parseInt(input.nextLine());
			data = new Object[carCount][columnHeaders.length];

			for (int i = 0; i < carCount; i++)
			{
				String name;
				while ((name = input.nextLine()) == null
						|| name.trim().isEmpty())
					;
				int miles = Integer.parseInt(input.nextLine());
				int price = Integer.parseInt(input.nextLine());
				int year = Integer.parseInt(input.nextLine());
				int cylinders = Integer.parseInt(input.nextLine());
				int fuelCapacity = Integer.parseInt(input.nextLine());
				int horsePower = Integer.parseInt(input.nextLine());

				Car newCar = new Car(name, miles, price, year, cylinders,
						fuelCapacity, horsePower);
				carList.add(newCar);
				data[i][0] = name;
				data[i][1] = miles;
				data[i][2] = price;
				data[i][3] = year;
				data[i][4] = cylinders;
				data[i][5] = fuelCapacity;
				data[i][6] = horsePower;
			}
			input.close();
		} catch (FileNotFoundException e)
		{
			JOptionPane.showMessageDialog(null, e.getMessage(), "Error Reading File", JOptionPane.ERROR_MESSAGE);
			//System.exit(0);
		}
	}

	public int getColumnCount()
	{
		return columnHeaders.length;
	}

	public int getRowCount()
	{
		return carList.size();
	}

	public String getColumnName(int col)
	{
		return columnHeaders[col];
	}

	public Object getValueAt(int row, int col)
	{
		return data[row][col];
	}

	@SuppressWarnings(
	{ "unchecked", "rawtypes" })
	public Class getColumnClass(int c)
	{
		return getValueAt(0, c).getClass();
	}
}