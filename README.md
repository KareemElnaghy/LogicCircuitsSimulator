# LogicCircuitsSimulator
G3 Kareem Elnaghy, Jana Elfeky, Mark Kyrollos
## Overview
This project is a circuit simulator implemented in C++. It allows users to read circuit descriptions from a file, simulate the behavior of the circuit, and output the results to another file. The simulator supports basic logic gates such as AND, OR, and NOT gates or complicated gates and it can handle complex circuits with multiple inputs and outputs.

## Features
- Read circuit descriptions from a file
- Simulate the behavior of the circuit
- Output simulation results to a file
- Support for basic and complex logic gates
- Event-Driven Simulation
- Error handling for invalid circuit descriptions

## Event-Driven Simulation

This circuit simulator employs an event-driven simulation approach to model the behavior of logic circuits. In event-driven simulation, the simulator maintains a priority queue of events, where each event represents a change in the circuit's state, such as an input change or a gate output change. The events are processed in chronological order based on their timestamp.

### Workflow
1. **Initialization**: The simulator initializes the circuit to default values of logic 0 at time 0 ps. The stimuli file is read and values are pushed into the queue initially.
   
2. **Event Processing**: The simulator continuously processes events from the priority queue until the queue becomes empty. Each event corresponds to a change in the circuit's state, such as an input value change or a gate output change. When an event is processed, the simulator updates the affected gates' outputs and schedules new events if necessary. All events that have been processed will be pushed into a priority queue.

3. **Gate Evaluation**: After processing an event, the simulator evaluates the outputs of all gates affected by the event. If the output of a gate changes as a result of the event, a new event is scheduled for the gate's output with a timestamp reflecting the propagation delay.

4. **Simulation Termination**: The simulation continues until there are no more events to process in the priority queue. At this point, the simulator has simulated the behavior of the circuit over a specific duration.

## How to Use
1. **Compile the Code**: Compile the source code using a C++ compiler. For example:
    ```
    g++ -o logicsim main.cpp gate.cpp circ.cpp
    ```

2. **Run the Program**: Execute the compiled binary and provide the input circuit file name as a command-line argument.
    ```
    ./logicsim library.lib circuit.cir stimuli.stim
    ```

3. **View Simulation Results**: After the program finishes running, check the output file (`circuit.sim`) to view the simulation results.

## Error Handling
The program provides error messages for the following scenarios:
- Unable to open files
- Different files being incorrectly formatted and provided
- Undefined components in the circuit description
- Logical Errors in the circuit i.e. an input also serving as an output

## Contributors
- [Kareem Elnaghy](https://github.com/KareemElnaghy) 
- [Jana ElFeky](https://github.com/JanaElfeky)
- [Mark Kyrollos](https://github.com/MarkKyrollos)
