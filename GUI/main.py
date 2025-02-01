from PyQt5 import QtCore, QtGui, QtWidgets
import random
import serial

ser = serial.Serial(port="COM8", baudrate=9600, timeout=0.1)


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.update_values)
        self.timer.start(50)

        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(820, 600)
        MainWindow.setStyleSheet(
            "QMainWindow {\n"
            "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \n"
            "                                stop:0 #000000,  /*Top */\n"
            "                                stop:0.5 #1C1B1A,   /* Center */\n"
            "                                stop:1 #383633);       /* Bottom */\n"
            "}"
        )
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        # Title Label
        self.lbl_Title = QtWidgets.QLabel(self.centralwidget)
        self.lbl_Title.setGeometry(QtCore.QRect(250, 20, 300, 50))
        self.lbl_Title.setFont(QtGui.QFont("Arial", 18, QtGui.QFont.Bold))
        self.lbl_Title.setStyleSheet("color: white;")
        self.lbl_Title.setAlignment(QtCore.Qt.AlignCenter)
        self.lbl_Title.setText("DEXTROSE MONITOR")

        # Patient 1 UI Elements
        self.Liquid1 = QtWidgets.QProgressBar(self.centralwidget)
        self.Liquid1.setGeometry(QtCore.QRect(20, 170, 601, 71))
        self.Liquid1.setMaximum(100)
        self.Liquid1.setTextVisible(False)
        self.Liquid1.setObjectName("Liquid1")

        self.lbl_Patient1 = QtWidgets.QLabel(self.centralwidget)
        self.lbl_Patient1.setGeometry(QtCore.QRect(20, 70, 201, 50))
        self.lbl_Patient1.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Patient1.setStyleSheet("color: white;")
        self.lbl_Patient1.setText("Patient 1")

        self.lbl_Patient1_DextroseWeight = QtWidgets.QLabel(self.centralwidget)
        self.lbl_Patient1_DextroseWeight.setGeometry(QtCore.QRect(20, 110, 351, 50))
        self.lbl_Patient1_DextroseWeight.setFont(
            QtGui.QFont("Arial", 12, QtGui.QFont.Bold)
        )
        self.lbl_Patient1_DextroseWeight.setStyleSheet("color: white;")
        self.lbl_Patient1_DextroseWeight.setObjectName("lbl_Patient1_DextroseWeight")

        self.lbl_Patient1_Percent = QtWidgets.QLabel(self.centralwidget)
        self.lbl_Patient1_Percent.setGeometry(QtCore.QRect(630, 180, 101, 50))
        self.lbl_Patient1_Percent.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Patient1_Percent.setStyleSheet("color: white;")
        self.lbl_Patient1_Percent.setObjectName("lbl_Patient1_Percent")

        # Patient 2 UI Elements
        self.Liquid1_2 = QtWidgets.QProgressBar(self.centralwidget)
        self.Liquid1_2.setGeometry(QtCore.QRect(20, 380, 601, 71))
        self.Liquid1_2.setMaximum(100)
        self.Liquid1_2.setTextVisible(False)
        self.Liquid1_2.setObjectName("Liquid1_2")

        self.lbl_Patient2 = QtWidgets.QLabel(self.centralwidget)
        self.lbl_Patient2.setGeometry(QtCore.QRect(20, 280, 201, 50))
        self.lbl_Patient2.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Patient2.setStyleSheet("color: white;")
        self.lbl_Patient2.setText("Patient 2")

        self.lbl_Patient2_DextroseWeight = QtWidgets.QLabel(self.centralwidget)
        self.lbl_Patient2_DextroseWeight.setGeometry(QtCore.QRect(20, 320, 371, 50))
        self.lbl_Patient2_DextroseWeight.setFont(
            QtGui.QFont("Arial", 12, QtGui.QFont.Bold)
        )
        self.lbl_Patient2_DextroseWeight.setStyleSheet("color: white;")
        self.lbl_Patient2_DextroseWeight.setObjectName("lbl_Patient2_DextroseWeight")

        self.lbl_Patient2_Percent = QtWidgets.QLabel(self.centralwidget)
        self.lbl_Patient2_Percent.setGeometry(QtCore.QRect(630, 390, 101, 50))
        self.lbl_Patient2_Percent.setFont(QtGui.QFont("Arial", 20, QtGui.QFont.Bold))
        self.lbl_Patient2_Percent.setStyleSheet("color: white;")
        self.lbl_Patient2_Percent.setObjectName("lbl_Patient2_Percent")

        MainWindow.setCentralWidget(self.centralwidget)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

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
        self.Liquid1_2.setValue(percent2)
        self.Liquid1_2.setStyleSheet(
            f"QProgressBar::chunk {{ background-color: {color2}; }}"
        )
        self.lbl_Patient2_DextroseWeight.setText(f"Dextrose Weight: {weight2}mg")
        self.lbl_Patient2_Percent.setText(f"{percent2}%")

    def update_values(self):
        # patient1_weight = random.randint(50, 1000)  # Example weight in mg
        # patient2_weight = random.randint(50, 1000)
        # self.update_patient_data(patient1_weight, patient2_weight)

        if ser.in_waiting > 0:
            received_data = (
                ser.readline().decode("utf-8").strip()
            )  # Read a line, decode, and remove extra spaces
            print(f"Received: {received_data}")  # Display the received data
            if received_data.startswith("A") and received_data.endswith("B"):
                print("Valid data received")
                values = received_data[1:-1].split(",")
                if len(values) == 4:
                    print("Valid number of values received")
                    print(f"Values: {values}")
                    print(f"Patient 1 weight: {values[1]}")
                    print(f"Patient 2 weight: {values[2]}")
                    patient1_weight = int(values[1])
                    patient2_weight = int(values[2])
                    self.update_patient_data(patient1_weight, patient2_weight)


if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)

    # Manually setting patient weights
    patient1_weight = 90  # Example weight in mg
    patient2_weight = 600  # Example weight in mg

    ui.update_patient_data(patient1_weight, patient2_weight)

    MainWindow.show()
    sys.exit(app.exec_())
