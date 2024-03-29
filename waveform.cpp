/*
 This is an implementation of a waveform generator that generates the waves of
 our simulation files based on our time stamps of each gate. This was created
 using QT software interface (including QT libraries) to visualize our waveforms
 */

/*
  #include "simCircuit1.sim"
  #include "simCircuit2.sim"
  #include "simCircuit3.sim"
  #include "simCircuit4.sim"
  #include "simCircuit5.sim"
*/
#include "stimuli.stim"
#include <QApplication>
#include <QCheckBox>
#include <QCustomPlot>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QStringList>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include <fstream>
#include "simfilename.sim"

class WaveformGenerator : public QWidget {
  Q_OBJECT
public:
  WaveformGenerator(QWidget *parent = nullptr) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Input section
    QLabel *inputLabel = new QLabel("Input Signals:");
    layout->addWidget(inputLabel);

    // Input signal fields
    for (const QString &signalName : signalNames) {
      QCheckBox *checkBox = new QCheckBox(signalName, this);
      signalsCheckboxes.append(checkBox);
      layout->addWidget(checkBox);
    }

    // Control buttons
    addButton = new QPushButton("Add Signal", this);
    connect(addButton, &QPushButton::clicked, this,
            &WaveformGenerator::addSignal);
    layout->addWidget(addButton);

    generateButton = new QPushButton("Generate Waveforms", this);
    connect(generateButton, &QPushButton::clicked, this,
            &WaveformGenerator::generateWaveforms);
    layout->addWidget(generateButton);

    // Waveform display area
    waveformDisplay = new QTextEdit(this);
    layout->addWidget(waveformDisplay);

    setLayout(layout);
  }

private slots:
  void addSignal() {
    QString signalName = "New Signal";
    QCheckBox *checkBox = new QCheckBox(signalName, this);
    signalsCheckboxes.append(checkBox);
    layout()->addWidget(checkBox);
  }

  void generateWaveforms() {
    // Generate waveforms based on the input signals
    QString waveformText;
    for (QCheckBox *checkBox : qAsConst(signalsCheckboxes)) {
      QString signalName = checkBox->text();
      QString signalValue = checkBox->isChecked() ? "1" : "0";
      waveformText += signalName + ": " + signalValue + "\n";
    }
    waveformDisplay->setText(waveformText);
  }

private:
  QList<QCheckBox *> signalsCheckboxes;
  QPushButton *addButton;
  QPushButton *generateButton;
  QTextEdit *waveformDisplay;
  QStringList signalNames = {"Signal A", "Signal B",
                             "Signal C"}; // Example signal names
};

// Function to read our all gates timestamps simulation data
QVector<double> readSimulationData() {
  QVector<double> D;
  QVector<int> logicalValue;
  int i = 0;
  double timestamp = 0.0;
  string name = "";
  int logVal;

  ifstream file("simCircuit1.sim"); // Open stim file for reading

  if (file.is_open()) { // Check if the file is successfully opened
    string line;
    while (getline(file, line)) { // Read each line from the file
      if (line != "=============") {
        stringstream s(line); // Create stringstream object to parse line

        getline(s, timestamp, ','); // read time
        getline(s, name, ',');      // read name of gate
        s >> logVal;                // reading logical value (0 or 1)

        D[i] = timestamp;
        logicalValue[i] = logVal;
        i++;
      }
    }

    file.close(); // Close the file after reading
  } else
    return; // file not found

  return D;
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  WaveformGenerator waveformGenerator;

  //// Create main window and custom plot
  // QMainWindow mainWindow;
  // QCustomPlot* customPlot = new QCustomPlot(&mainWindow);
  // mainWindow.setCentralWidget(customPlot);
  //
  //// Read simulation data
  // QVector<double> data = readSimulationData();
  //
  //// Create graph and assign data to it
  // customPlot->addGraph();
  // customPlot->graph(0)->setData(data);
  //
  //// Setup plot
  // customPlot->xAxis->setLabel("Time"); // time on x-axis
  // customPlot->yAxis->setLabel("Amplitude"); // amplitude (0 or 1) on y-axis
  // customPlot->rescaleAxes();
  // customPlot->replot();
  //
  // mainWindow.show();

  waveformGenerator.show();

  return app.exec();
}

#include "main.moc"