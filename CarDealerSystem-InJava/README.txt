*********************************************************************************************
Author, Class, Assignment #
*********************************************************************************************
James Mirvil
Cop 4331
HomeWork 2

*********************************************************************************************
Description of Program
*********************************************************************************************
This is a user driven program which allows user to view the list of cars that the 
dealer has. The user is able to filter and compare change based on selection.

*********************************************************************************************
Running the Application
*********************************************************************************************
Requires Java 8 or higher to run and an input file

In order to run this program, launch "CarDealerSystem.jar" file with the input file
in the same directory. The input file must be named "INPUT.TXT" in order for the 
program to locate the file. The constraints of the are listed in the bottom section.

If there is an error with the input file and the file is changed, the user will have 
to relaunch the program inorder for the changes to appear in the program.

*********************************************************************************************
Using the Application
*********************************************************************************************
Once the program is launched, their are three option that can interact with: 
Range, Comparison Method, and Compare Cars. Input is as follows:
    Set Range - 
        The user can set the range of Price, Mileage, and Year. 
        Accepted input for these fields are integers greater than 0.
        If an invalid value is entered, the min/max value of 0/INTEGER.MAX will be used
        If entered min > max, the min will be set to 0 and max set to the previosu min

    Set Comparison Method -
        The user can select to compare by one type method.
        Supported methods :Mileage, Price, Year, Cylinders, Fuel Capacity, and Horse Power
        If none are selected, entire list will be returned sorting by name

    Compare Cars -
        This user can select to modify the list based on the options above.

*********************************************************************************************
Input File Constraints
*********************************************************************************************
The first line must be a number from 1 – 100 (representing the number of cars)
This is followed by 'n' number of cars in the following format:
    -Car Name: Name of the car for example BMW M4.
    -Mileage: The distance the car has traveled until now in miles.
    -Price: Price of the car in dollars
    -Year: The year the car was built in.
    -Cylinders: Number of cylinders the car engine has for example 6 cylinders.
    -Fuel Capacity: The capacity of gas tank the car has for example 10 gallons.
    -Horse Power: The horse power of the car for example 180 hp.

    **All Values, except name, must be integers**
