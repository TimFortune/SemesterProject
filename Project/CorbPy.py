#!/usr/bin/env python3

# Utilizing the csv module to process CSV files
import csv

# Path to the CSV file containing temperature data
file_path = 'feed_data.csv'

# List to hold temperature readings
temp_readings = []

# Reading the CSV file
with open(file_path, 'r') as file:
    # Creating a CSV reader object
    reader = csv.reader(file)
    # Skipping the first line if it's a header
    next(reader)
    
    # Iterating over each row in the CSV
    for record in reader:
        try:
            # Extracting temperature from the second column
            temp = float(record[1])
            # Adding the temperature to the list
            temp_readings.append(temp)
        except ValueError:
            # Ignoring rows with invalid data
            pass

# Computing the statistical values
if temp_readings:
    lowest_temp = min(temp_readings)
    highest_temp = max(temp_readings)
    average_temp = sum(temp_readings) / len(temp_readings)
    
    # Displaying the results
    print(f'Lowest Recorded Temperature: {lowest_temp}°C')
    print(f'Highest Recorded Temperature: {highest_temp}°C')
    print(f'Average Recorded Temperature: {average_temp}°C')
else:
    print('Temperature data could not be found.')
