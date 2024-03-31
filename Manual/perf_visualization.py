import matplotlib.pyplot as plt
import subprocess
import time

def generate_flamegraph():
    perf_script = subprocess.Popen(['perf', 'script'], stdout=subprocess.PIPE)
    stack_collapse = subprocess.Popen(['/home/jangoo/FlameGraph/stackcollapse-perf.pl'], stdin=perf_script.stdout, stdout=subprocess.PIPE)
    flamegraph = subprocess.Popen(['/home/jangoo/FlameGraph/flamegraph.pl'], stdin=stack_collapse.stdout, stdout=subprocess.PIPE)

    perf_script.stdout.close()
    stack_collapse.stdout.close()

    return flamegraph.communicate()[0].decode('utf-8')

while True:
    flamegraph_data = generate_flamegraph()

    # Process flamegraph_data to extract x and y data for plotting
    # ...

    # Plotting logic using extracted data
    plt.clf()
    # Plot x and y data
    plt.xlabel('X-axis Label')
    plt.ylabel('Y-axis Label')
    plt.title('Performance Visualization')
    plt.pause(1)  # Adjust pause duration as needed

    time.sleep(1)  # Adjust sleep duration as needed

