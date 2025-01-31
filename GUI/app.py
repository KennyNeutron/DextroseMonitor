import sys
import serial
import threading
from PyQt5 import QtWidgets, QtCore, QtGui


class SerialWorker(QtCore.QThread):
    data_received = QtCore.pyqtSignal(int, int)  # Signal to send patient weights

    def __init__(self, port="COM3", baudrate=9600):
        super().__init__()
        self.serial_port = serial.Serial(port, baudrate, timeout=1)
        self.running = True

    def run(self):
        while self.running:
            if self.serial_port.in_waiting > 0:
                try:
                    raw_data = self.serial_port.readline().decode("utf-8").strip()
                    if raw_data.startswith("A") and raw_data.endswith("B"):
                        values = raw_data[1:-1].split(
                            ","
                        )  # Remove A and B, split values
                        if len(values) == 2:
                            patient1_weight = int(values[0])
                            patient2_weight = int(values[1])
                            self.data_received.emit(patient1_weight, patient2_weight)
                except Exception as e:
                    print(f"Error reading serial: {e}")

    def stop(self):
        self.running = False
        self.serial_port.close()
        self.quit()


class Ui_MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("DEXTROSE MONITOR")
        self.setGeometry(100, 100, 820, 600)
        self.setStyleSheet(
            "QMainWindow {background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 #000000, stop:0.5 #1C1B1A, stop:1 #383633);}"
        )

        self.centralwidget = QtWidgets.QWidget(self)
        self.setCentralWidget(self.centralwidget)

        # Title
        self.lbl_Title = QtWidgets.QLabel("DEXTROSE MONITOR", self.centralwidget)
        self.lbl_Title.setGeometry(QtCore.QRect(250, 20, 300, 50))
        self.lbl_Title.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Title.setStyleSheet("color: white;")
        self.lbl_Title.setAlignment(QtCore.Qt.AlignCenter)

        # Patient 1 UI Elements
        self.Liquid1 = QtWidgets.QProgressBar(self.centralwidget)
        self.Liquid1.setGeometry(QtCore.QRect(20, 170, 601, 71))
        self.Liquid1.setMaximum(100)
        self.Liquid1.setTextVisible(False)

        self.lbl_Patient1 = QtWidgets.QLabel("Patient 1", self.centralwidget)
        self.lbl_Patient1.setGeometry(QtCore.QRect(20, 70, 201, 50))
        self.lbl_Patient1.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Patient1.setStyleSheet("color: white;")

        self.lbl_Patient1_DextroseWeight = QtWidgets.QLabel("", self.centralwidget)
        self.lbl_Patient1_DextroseWeight.setGeometry(QtCore.QRect(20, 110, 351, 50))
        self.lbl_Patient1_DextroseWeight.setFont(
            QtGui.QFont("Arial", 12, QtGui.QFont.Bold)
        )
        self.lbl_Patient1_DextroseWeight.setStyleSheet("color: white;")

        self.lbl_Patient1_Percent = QtWidgets.QLabel("", self.centralwidget)
        self.lbl_Patient1_Percent.setGeometry(QtCore.QRect(630, 180, 101, 50))
        self.lbl_Patient1_Percent.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Patient1_Percent.setStyleSheet("color: white;")

        # Patient 2 UI Elements
        self.Liquid2 = QtWidgets.QProgressBar(self.centralwidget)
        self.Liquid2.setGeometry(QtCore.QRect(20, 380, 601, 71))
        self.Liquid2.setMaximum(100)
        self.Liquid2.setTextVisible(False)

        self.lbl_Patient2 = QtWidgets.QLabel("Patient 2", self.centralwidget)
        self.lbl_Patient2.setGeometry(QtCore.QRect(20, 280, 201, 50))
        self.lbl_Patient2.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Patient2.setStyleSheet("color: white;")

        self.lbl_Patient2_DextroseWeight = QtWidgets.QLabel("", self.centralwidget)
        self.lbl_Patient2_DextroseWeight.setGeometry(QtCore.QRect(20, 320, 371, 50))
        self.lbl_Patient2_DextroseWeight.setFont(
            QtGui.QFont("Arial", 12, QtGui.QFont.Bold)
        )
        self.lbl_Patient2_DextroseWeight.setStyleSheet("color: white;")

        self.lbl_Patient2_Percent = QtWidgets.QLabel("", self.centralwidget)
        self.lbl_Patient2_Percent.setGeometry(QtCore.QRect(630, 390, 101, 50))
        self.lbl_Patient2_Percent.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Patient2_Percent.setStyleSheet("color: white;")

    def update_patient_data(self, weight1, weight2):
        percent1 = min(max(int((weight1 / 1000) * 100), 0), 100)
        percent2 = min(max(int((weight2 / 1000) * 100), 0), 100)

        color1 = "red" if percent1 < 10 else "orange" if percent1 < 20 else "green"
        color2 = "red" if percent2 < 10 else "orange" if percent2 < 20 else "green"

        self.Liquid1.setValue(percent1)
        self.Liquid1.setStyleSheet(
            f"QProgressBar::chunk {{ background-color: {color1}; }}"
        )
        self.lbl_Patient1_DextroseWeight.setText(f"Dextrose Weight: {weight1}mg")
        self.lbl_Patient1_Percent.setText(f"{percent1}%")

        self.Liquid2.setValue(percent2)
        self.Liquid2.setStyleSheet(
            f"QProgressBar::chunk {{ background-color: {color2}; }}"
        )
        self.lbl_Patient2_DextroseWeight.setText(f"Dextrose Weight: {weight2}mg")
        self.lbl_Patient2_Percent.setText(f"{percent2}%")


class MainApplication(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # Serial Thread
        self.serial_thread = SerialWorker(port="COM3", baudrate=9600)
        self.serial_thread.data_received.connect(self.ui.update_patient_data)
        self.serial_thread.start()

    def closeEvent(self, event):
        self.serial_thread.stop()
        event.accept()


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MainApplication()
    window.show()
    sys.exit(app.exec_())
