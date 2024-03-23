/*
This is an implementation of a waveform generator that generates the waves of our simulation files based on our time stamps of each gate.
**This was created using Qt software interface (including Qt libraries) to visualize our waveforms
*/

#include "simCircuit1.sim"
#include "simCircuit2.sim"
#include "simCircuit3.sim"
#include "simCircuit4.sim"
#include "simCircuit5.sim"
#include <QApplication>
#include <QCustomPlot>
#include <QMainWindow>
#include <QVector>

// Function to read our all gates' timestamps simulation data
QVector<double> readSimulationData() {
  QVector<double> D; // vector D of type data (already declared in our gates file)

  
  // Filling our data vector with our simulation data
  for (int i = 0; i < D.size(); i++) {
    D[i] = get_output(); // reading the timestamps
  }

  return D;
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QMainWindow mainWindow;
  QCustomPlot *customPlot = new QCustomPlot(&mainWindow);
  mainWindow.setCentralWidget(customPlot);

  // Reading our simulation data
  QVector<double> data = readSimulationData();

  customPlot->addGraph();
  customPlot->graph(0)->setData(
      data); // creating a graph and assigning our data vector to it

  // Setup plot
  customPlot->xAxis->setLabel("Time");
  customPlot->yAxis->setLabel("Amplitude");
  customPlot->rescaleAxes();
  customPlot->replot();

  mainWindow.show();
  return app.exec();
}