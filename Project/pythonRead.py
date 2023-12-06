# Tim Fortune Python problem
# usage is python3 pythonRead.py

import csv

# Replace 'your_data_file.csv' with the path to your CSV file
csv_file_path = 'feed_data.csv'

# Initialize variables to store the temperature data
temperatures = []

# Open the CSV file and read the data
with open(csv_file_path, mode='r') as csv_file:
    csv_reader = csv.reader(csv_file)
    next(csv_reader)  # Skip the header row if there is one
    for row in csv_reader:
        try:
            # Assuming the temperature data is in the first column
            temperature = float(row[1])
            temperatures.append(temperature)
        except ValueError:
            # Skip rows that don't contain float values
            continue

# Calculate the minimum, maximum, and average temperatures
if temperatures:
    min_temp = min(temperatures)
    max_temp = max(temperatures)
    avg_temp = sum(temperatures) / len(temperatures)
    print(f'Minimum Temperature: {min_temp}deg C')
    print(f'Maximum Temperature: {max_temp}deg C')
    print(f'Average Temperature: {avg_temp}deg C')
else:
    print('No temperature data found in the file.')
