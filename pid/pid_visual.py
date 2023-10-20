import subprocess
import matplotlib.pyplot as plt

# Compile and run the C++ code using subprocess
process = subprocess.Popen(["/home/dragos/Projects/pure_pursuit/pid/pid"], stdout=subprocess.PIPE, text=True)
output, _ = process.communicate()

# Initialize lists to store data
i_values = []
current_values = []

# Parse the output and store the data
for line in output.splitlines():
    parts = line.split(":")
    i_values.append(int(parts[0]))
    current_values.append(float(parts[1]))

# Plot the data
plt.plot(i_values, current_values, label='Current Value')
plt.axhline(y=100, color='r', linestyle='-', label='Setpoint')
plt.xlabel('i')
plt.ylabel('Current Value')
plt.legend()
plt.show()

