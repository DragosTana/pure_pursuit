import subprocess
import matplotlib.pyplot as plt

# Compile and run the C++ code using subprocess
process = subprocess.Popen(["/home/dragos/Projects/control_node/pid/pid"], stdout=subprocess.PIPE, text=True)
output, _ = process.communicate()

# Initialize lists to store data
i_values = []
current_values = []
reference = []

for i in range(300):
    if i < 25:
        reference.append(50)
    elif i < 75:
        reference.append(100)
    else:
        reference.append(50)
        
# Parse the output and store the data
for line in output.splitlines():
    parts = line.split(":")
    i_values.append(int(parts[0]))
    current_values.append(float(parts[1]))

# Plot the data
plt.plot(i_values, current_values, label='Current Value')
plt.plot(i_values, reference, label='Reference')
#plt.axhline(y=100, color='r', linestyle='-', label='Setpoint')
plt.xlabel('i')
plt.ylabel('Current Value')
plt.legend()
plt.show()

