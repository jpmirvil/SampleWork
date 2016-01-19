import java.awt.Color;
import java.awt.Component;
import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JLabel;

import java.awt.Font;
import java.util.ArrayList;
import java.util.Collections;

import javax.swing.SwingConstants;
import javax.swing.JTextField;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.JList;
import javax.swing.AbstractListModel;
import javax.swing.ListSelectionModel;
import javax.swing.JButton;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumnModel;
import javax.swing.table.TableModel;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

import javax.swing.border.EtchedBorder;


public class CarDealerGUI
{

	private JFrame frame;
	private JTable table;
	private JLabel lblSetRange;
	private JLabel lblPrice;
	private JLabel lblMileage;
	private JTextField txtMaxPrice;
	private JTextField txtMaxMiles;
	private JTextField txtMinMiles;
	private JTextField txtMinPrice;
	private JLabel lblMin;
	private JLabel lblMax;
	private JLabel lblSetComparisionMethod;
	public ArrayList<Car> carList = new ArrayList<Car>();
	private JTextField txtMinYear;
	private JTextField txtMaxYear;
	

	/**
	 * Launch the application.
	 */
	public static void main(String[] args)
	{
		
		EventQueue.invokeLater(new Runnable()
		{
			public void run()
			{
				try
				{
					CarDealerGUI window = new CarDealerGUI();
					window.frame.setVisible(true);
				}
				catch (Exception e)
				{
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public CarDealerGUI()
	{
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	@SuppressWarnings({ "unchecked", "serial", "rawtypes" })
	private void initialize()
	{
		TableModel dealerTableModel = new DealerTableModel(carList, "Input.txt");
		
		frame = new JFrame();
		frame.setBounds(100, 100, 860, 482);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setViewportBorder(null);
		scrollPane.getViewport().setBackground(Color.WHITE);
		
		lblSetRange = new JLabel("Set Range");
		lblSetRange.setHorizontalAlignment(SwingConstants.LEFT);
		lblSetRange.setFont(new Font("Calibri", Font.PLAIN, 16));
		
		lblPrice = new JLabel("Price");
		lblPrice.setFont(new Font("Calibri", Font.PLAIN, 14));
		
		lblMileage = new JLabel("Mileage");
		lblMileage.setFont(new Font("Calibri", Font.PLAIN, 14));
		
		txtMaxPrice = new JTextField();
		txtMaxPrice.setToolTipText("Enter a number greater than 0. Must be greater than min price");
		txtMaxPrice.setColumns(10);
		
		txtMaxMiles = new JTextField();
		txtMaxMiles.setToolTipText("Enter a number greater than 0. Must be greater than min mileage");
		txtMaxMiles.setColumns(10);
		
		txtMinMiles = new JTextField();
		txtMinMiles.setToolTipText("Enter a number greater than 0. Must be less than max mileage");
		txtMinMiles.setColumns(10);
		
		txtMinPrice = new JTextField();
		txtMinPrice.setToolTipText("Enter a number greater than 0. Must be less than max price");
		txtMinPrice.setColumns(10);
		
		lblMin = new JLabel("Min");
		lblMin.setFont(new Font("Calibri", Font.PLAIN, 11));
		
		lblMax = new JLabel("Max");
		lblMax.setFont(new Font("Calibri", Font.PLAIN, 11));
		
		lblSetComparisionMethod = new JLabel("Set Comparision Method");
		lblSetComparisionMethod.setHorizontalAlignment(SwingConstants.LEFT);
		lblSetComparisionMethod.setFont(new Font("Calibri", Font.PLAIN, 16));
		
		JList listCompareMethod = new JList();
		listCompareMethod.setBorder(new EtchedBorder(EtchedBorder.LOWERED, null, null));
		listCompareMethod.setToolTipText("Select the item that you would like to compare by. Can only select item per comparision.");
		listCompareMethod.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		listCompareMethod.setModel(new AbstractListModel() {
			String[] values = new String[] {"Mileage", "Price", "Year", "Cylinders", "Fuel Capacity", "Horse Power"};
			public int getSize() {
				return values.length;
			}
			public Object getElementAt(int index) {
				return values[index];
			}
		});
		listCompareMethod.setFont(new Font("Calibri", Font.PLAIN, 14));
		
		JButton btnCompareButton = new JButton("Compare Cars");
		btnCompareButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				Integer minPrice = tryParse(txtMinPrice.getText());
				Integer maxPrice = tryParse(txtMaxPrice.getText());
				Integer minMiles = tryParse(txtMinMiles.getText());
				Integer maxMiles = tryParse(txtMaxMiles.getText());
				Integer minYear = tryParse(txtMinMiles.getText());
				Integer maxYear = tryParse(txtMaxMiles.getText());
				
				Tuple<Integer, Integer> cleanMiles = cleanRange(minMiles, maxMiles);
				Tuple<Integer, Integer> cleanPrice = cleanRange(minPrice, maxPrice);
				Tuple<Integer, Integer> cleanYear = cleanRange(minYear, maxYear);
				minMiles = cleanMiles.valueL;
				maxMiles = cleanMiles.valueR;
				minPrice = cleanPrice.valueL;
				maxPrice = cleanPrice.valueR;
				minYear = cleanYear.valueL;
				maxYear = cleanYear.valueR;

				ArrayList<Car> sortedCars = new ArrayList<Car>(); //= (ArrayList<Car>) carList.clone();
				
				for(Car car : carList ){
					int miles = car.getMileage();
					int price = car.getPrice();
					int year = car.getYear();
					
					if( miles >= minMiles && miles <= maxMiles)  
					{
						if( price >= minPrice && price <= maxPrice)
						{
							if( year >= minYear && year <= maxYear)
							{
								sortedCars.add(car);
							}
						}
					}
				}


				String compareBy = (String)listCompareMethod.getSelectedValue();
				compareBy = compareBy == null ? "NoSort" : compareBy;
				
				switch (compareBy){
					case "Mileage":
						Collections.sort(sortedCars, Car.Comparators.MILES);
						break;
					case "Price":
						Collections.sort(sortedCars, Car.Comparators.PRICE);
						break;
					case "Year":
						Collections.sort(sortedCars, Collections.reverseOrder(Car.Comparators.YEAR));
						break;
					case "Cylinders":
						Collections.sort(sortedCars, Collections.reverseOrder(Car.Comparators.CYLINDERS));
						break;
					case "Fuel Capacity":
						Collections.sort(sortedCars, Collections.reverseOrder(Car.Comparators.FUEL));
						break;
					case "Horse Power":
						Collections.sort(sortedCars, Collections.reverseOrder(Car.Comparators.HORSES));
						break;
					default:
						Collections.sort(sortedCars);
						break;
				}
				TableModel newDealerTableModel = new DealerTableModel(sortedCars, true);
				table.setModel(newDealerTableModel);
				
				DefaultTableCellRenderer centerRenderer = new DefaultTableCellRenderer();
				centerRenderer.setHorizontalAlignment( JLabel.CENTER );
				table.setDefaultRenderer(Integer.class, centerRenderer);
				resizeColumnWidth(table);
				table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
			}
		});
		btnCompareButton.setFont(new Font("Calibri", Font.PLAIN, 16));
		
		JLabel lblYear = new JLabel("Year");
		lblYear.setFont(new Font("Calibri", Font.PLAIN, 14));
		
		txtMinYear = new JTextField();
		txtMinYear.setToolTipText("Enter a number greater than 0. Must be less than max year");
		txtMinYear.setColumns(10);
		
		txtMaxYear = new JTextField();
		txtMaxYear.setToolTipText("Enter a number greater than 0. Must be less than max year");
		txtMaxYear.setColumns(10);
		GroupLayout groupLayout = new GroupLayout(frame.getContentPane());
		groupLayout.setHorizontalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
						.addGroup(groupLayout.createSequentialGroup()
							.addContainerGap()
							.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
								.addGroup(groupLayout.createSequentialGroup()
									.addComponent(lblSetRange, GroupLayout.PREFERRED_SIZE, 164, GroupLayout.PREFERRED_SIZE)
									.addGap(16))
								.addGroup(Alignment.TRAILING, groupLayout.createSequentialGroup()
									.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
										.addComponent(lblPrice)
										.addComponent(lblMileage, GroupLayout.PREFERRED_SIZE, 51, GroupLayout.PREFERRED_SIZE)
										.addComponent(lblYear, GroupLayout.PREFERRED_SIZE, 51, GroupLayout.PREFERRED_SIZE))
									.addPreferredGap(ComponentPlacement.RELATED, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
									.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
										.addGroup(groupLayout.createSequentialGroup()
											.addComponent(txtMinYear, GroupLayout.PREFERRED_SIZE, 53, GroupLayout.PREFERRED_SIZE)
											.addPreferredGap(ComponentPlacement.RELATED)
											.addComponent(txtMaxYear, GroupLayout.PREFERRED_SIZE, 53, GroupLayout.PREFERRED_SIZE))
										.addGroup(groupLayout.createSequentialGroup()
											.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false)
												.addComponent(txtMinMiles, 0, 0, Short.MAX_VALUE)
												.addComponent(txtMinPrice, GroupLayout.DEFAULT_SIZE, 53, Short.MAX_VALUE))
											.addPreferredGap(ComponentPlacement.RELATED)
											.addGroup(groupLayout.createParallelGroup(Alignment.LEADING, false)
												.addComponent(txtMaxMiles, 0, 0, Short.MAX_VALUE)
												.addComponent(txtMaxPrice, GroupLayout.DEFAULT_SIZE, 53, Short.MAX_VALUE))))
									.addGap(13))
								.addGroup(groupLayout.createParallelGroup(Alignment.TRAILING, false)
									.addComponent(btnCompareButton, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
									.addComponent(lblSetComparisionMethod, Alignment.LEADING, GroupLayout.DEFAULT_SIZE, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
									.addComponent(listCompareMethod, Alignment.LEADING, GroupLayout.PREFERRED_SIZE, 170, GroupLayout.PREFERRED_SIZE)))
							.addPreferredGap(ComponentPlacement.UNRELATED)
							.addComponent(scrollPane, GroupLayout.DEFAULT_SIZE, 644, Short.MAX_VALUE))
						.addGroup(groupLayout.createSequentialGroup()
							.addGap(82)
							.addComponent(lblMin)
							.addGap(40)
							.addComponent(lblMax)))
					.addContainerGap())
		);
		groupLayout.setVerticalGroup(
			groupLayout.createParallelGroup(Alignment.LEADING)
				.addGroup(groupLayout.createSequentialGroup()
					.addContainerGap()
					.addGroup(groupLayout.createParallelGroup(Alignment.LEADING)
						.addComponent(scrollPane, GroupLayout.PREFERRED_SIZE, 422, GroupLayout.PREFERRED_SIZE)
						.addGroup(groupLayout.createSequentialGroup()
							.addComponent(lblSetRange)
							.addGap(14)
							.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
								.addComponent(lblMax)
								.addComponent(lblMin))
							.addPreferredGap(ComponentPlacement.RELATED)
							.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
								.addComponent(txtMinPrice, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
								.addComponent(lblPrice)
								.addComponent(txtMaxPrice, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
							.addPreferredGap(ComponentPlacement.UNRELATED)
							.addGroup(groupLayout.createParallelGroup(Alignment.TRAILING)
								.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
									.addComponent(txtMinMiles, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
									.addComponent(txtMaxMiles, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
								.addComponent(lblMileage))
							.addPreferredGap(ComponentPlacement.UNRELATED)
							.addGroup(groupLayout.createParallelGroup(Alignment.BASELINE)
								.addComponent(lblYear, GroupLayout.PREFERRED_SIZE, 17, GroupLayout.PREFERRED_SIZE)
								.addComponent(txtMinYear, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
								.addComponent(txtMaxYear, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
							.addGap(26)
							.addComponent(lblSetComparisionMethod, GroupLayout.PREFERRED_SIZE, 20, GroupLayout.PREFERRED_SIZE)
							.addPreferredGap(ComponentPlacement.RELATED)
							.addComponent(listCompareMethod)
							.addPreferredGap(ComponentPlacement.RELATED)
							.addComponent(btnCompareButton, GroupLayout.PREFERRED_SIZE, 39, GroupLayout.PREFERRED_SIZE)))
					.addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
		);
		
		table = new JTable(dealerTableModel);
		table.setShowHorizontalLines(false);
		table.setShowGrid(false);
		table.setShowVerticalLines(false);
		table.setFont(new Font("Calibri", Font.PLAIN, 16));
		scrollPane.setViewportView(table);
		frame.getContentPane().setLayout(groupLayout);
		
		DefaultTableCellRenderer centerRenderer = new DefaultTableCellRenderer();
		centerRenderer.setHorizontalAlignment( JLabel.CENTER );
		table.setDefaultRenderer(Integer.class, centerRenderer);
		resizeColumnWidth(table);
		table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
		
	}
	
	public void resizeColumnWidth(JTable table) {
	    final TableColumnModel columnModel = table.getColumnModel();
	    
	    for (int col = 0; col < table.getColumnCount(); col++) {
	        int width = 89;
	        
	        for (int row = 0; row < table.getRowCount(); row++) {
	            TableCellRenderer renderer = table.getCellRenderer(row, col);
	            Component tComponent = table.prepareRenderer(renderer, row, col);
	            width = Math.max(tComponent.getPreferredSize().width +1 , width);
	        }
	        columnModel.getColumn(col).setPreferredWidth(width);
	    }
	}
	
	public static Integer tryParse(String text)
	{
		try
		{
			return Integer.parseInt(text);
		} catch (NumberFormatException e)
		{
			return null;
		}
	}
	
	public Tuple<Integer, Integer> cleanRange(Integer min, Integer max )
	{		
		min = min == null || min < 0 ? 0 : min;
		max = max == null || max < 0 ? Integer.MAX_VALUE : max;
		
		if( max < min){
			max = min;
			min = 0;
		}
		return new Tuple<Integer, Integer>(min, max);
	}
	
	public class Tuple<Left, Right>{
		
		public final Left valueL;
		public final Right valueR;

		public Tuple(Left valueL, Right valueR)
		{
			this.valueL = valueL;
			this.valueR = valueR;
		}
	}
}


