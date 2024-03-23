/*
This is an implementation of a waveform generator that generates the waves of our simulation files based on our time stamps of each gate.
**This was created using Qt software interface (including Qt libraries) to visualize our waveforms
*/

#include "circ.h"
#include <QApplication>
#include <QCustomPlot>
#include <QMainWindow>
#include <QVector>

// Function to read simulation data
QVector<double> readSimulationData() {
  QVector<double> data;

  // Fill the data vector with your simulation data

  for (int i = 0; i < data.size(); i++) {
    data[i] = ;
  }

  return data;
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