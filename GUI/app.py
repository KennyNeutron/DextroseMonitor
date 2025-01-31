import sys
import serial
import threading
from PyQt5 import QtWidgets, QtCore, QtGui


class SerialWorker(QtCore.QThread):
    data_received = QtCore.pyqtSignal(int, int)  # Signal to send patient weights
    error_signal = QtCore.pyqtSignal(str)

    def __init__(self, port="COM8", baudrate=9600):
        super().__init__()
        self.mutex = QtCore.QMutex()  # Mutex for thread safety
        try:
            self.serial_port = serial.Serial(port, baudrate, timeout=1)
        except serial.SerialException as e:
            self.error_signal.emit(f"Failed to open serial port: {str(e)}")
            self.running = False
            return
        self.running = True

    def run(self):
        while self.running:
            try:
                self.mutex.lock()
                if self.serial_port.in_waiting > 0:
                    raw_data = self.serial_port.readline().decode("utf-8").strip()
                    print(f"Received: {raw_data}")
                    if raw_data.startswith("A") and raw_data.endswith("B"):
                        values = raw_data[1:-1].split(
                            ","
                        )  # Remove A and B, split values
                        if len(values) == 2:
                            try:
                                patient1_weight = int(values[0])
                                patient2_weight = int(values[1])
                                self.data_received.emit(
                                    patient1_weight, patient2_weight
                                )
                            except ValueError as e:
                                print(f"Error converting values: {e}")
                self.mutex.unlock()
            except Exception as e:
                self.error_signal.emit(f"Serial read error: {str(e)}")
                break

        if self.serial_port.is_open:
            self.serial_port.close()

    def stop(self):
        self.running = False
        if hasattr(self, "serial_port") and self.serial_port.is_open:
            self.serial_port.close()
        self.quit()


class MainApplication(QtWidgets.QMainWindow):
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
        self.lbl_Title.setGeometry(QtCore.QRect(250, 20, 400, 50))
        self.lbl_Title.setFont(QtGui.QFont("Arial", 18, QtGui.QFont.Bold))
        self.lbl_Title.setStyleSheet("color: white;")
        self.lbl_Title.setAlignment(QtCore.Qt.AlignCenter)

        # Patient 1 UI Elements
        self.Liquid1 = QtWidgets.QProgressBar(self.centralwidget)
        self.Liquid1.setGeometry(QtCore.QRect(25, 170, 601, 71))
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

        # Serial Thread
        self.serial_thread = SerialWorker(port="COM8", baudrate=9600)
        self.serial_thread.data_received.connect(self.update_patient_data)
        self.serial_thread.error_signal.connect(self.handle_serial_error)
        self.serial_thread.start()

    def handle_serial_error(self, error_message):
        QtWidgets.QMessageBox.critical(self, "Serial Error", error_message)

    @QtCore.pyqtSlot(int, int)
    def update_patient_data(self, weight1, weight2):
        percent1 = min(max(int((weight1 / 1000) * 100), 0), 100)
        self.Liquid1.setValue(percent1)
        self.lbl_Patient1_DextroseWeight.setText(f"Dextrose Weight: {weight1}mg")
        self.lbl_Patient1_Percent.setText(f"{percent1}%")

    def closeEvent(self, event):
        self.serial_thread.stop()
        self.serial_thread.wait()
        event.accept()


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = MainApplication()
    window.show()
    sys.exit(app.exec_())
