import java.util.Comparator;

public class Car implements Comparable<Car>
{
	private String carName;
	private int mileage;
	private int price;
	private int year;
	private int cylinders;
	private int fuelCapacity;
	private int horsePower;

	public Car(String carName, int mileage, int price, int year, int cylinders,
			int fuelCapacity, int horsePower)
	{
		this.carName = carName;
		this.mileage = mileage;
		this.price = price;
		this.year = year;
		this.cylinders = cylinders;
		this.fuelCapacity = fuelCapacity;
		this.horsePower = horsePower;
	}

	@Override
	public int compareTo(Car that)
	{
		return Comparators.NAME.compare(this, that);
	}

	public static class Comparators
	{

		public static final Comparator<Car> NAME = (Car car1, Car car2) -> car1
				.getCarName().compareTo(car2.getCarName());
		public static final Comparator<Car> MILES = (Car car1, Car car2) -> Integer
				.compare(car1.getMileage(), car2.getMileage());
		public static final Comparator<Car> PRICE = (Car car1, Car car2) -> Integer
				.compare(car1.getPrice(), car2.getPrice());
		public static final Comparator<Car> YEAR = (Car car1, Car car2) -> Integer
				.compare(car1.getYear(), car2.getYear());
		public static final Comparator<Car> CYLINDERS = (Car car1, Car car2) -> Integer
				.compare(car1.getCylinders(), car2.getCylinders());
		public static final Comparator<Car> FUEL = (Car car1, Car car2) -> Integer
				.compare(car1.getFuelCapacity(), car2.getFuelCapacity());
		public static final Comparator<Car> HORSES = (Car car1, Car car2) -> Integer
				.compare(car1.getHorsePower(), car2.getHorsePower());
	}

	// Below are the Getter (Accessors) for each data type

	public String getCarName()
	{
		return carName;
	}

	public int getMileage()
	{
		return mileage;
	}

	public int getPrice()
	{
		return price;
	}

	public int getYear()
	{
		return year;
	}

	public int getCylinders()
	{
		return cylinders;
	}

	public int getFuelCapacity()
	{
		return fuelCapacity;
	}

	public int getHorsePower()
	{
		return horsePower;
	}

}
