from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice

app = QtWidgets.QApplication([])
ui = uic.loadUi("Wind_Port.ui") # #F:/Arduino(sketch)/esp8266_ModeMCU1.0(ESP-12E_Module)/Server_PCmanagment_ESP8266/Python_Files/Window_Server_ESP8266.ui
ui.setWindowTitle("MEGA-2560")



serial = QSerialPort()
serial.setBaudRate(9600) #Швидкість Порту 460800
ports = QSerialPortInfo().availablePorts()
ports_list = []
for pr in ports:
    ports_list.append(pr.portName())
ui.PortBox.addItems(ports_list)




serial.setPortName("COM9")
serial.open(QIODevice.ReadWrite)
print("AUTO Connect Port")
def connectPort():
    print(ui.PortBox.currentText())
    serial.setPortName(str(ui.PortBox.currentText()))
    serial.open(QIODevice.ReadWrite)
    print("Connect Port")
    ui.Statusbar.showMessage("Connect", 4000)
ui.PortConnectButton.clicked.connect(connectPort)


def disconnectPort():
    serial.close()
    print("Disconnect Port")
    ui.Statusbar.showMessage("Disconnect", 4000)
ui.PortDisconectButton.clicked.connect(disconnectPort)



def cursor_curation(xy: list):
    print(xy[0], xy[1])



def parse_input(user_input):
    """ 
    Функція для парсингу команд
    """
    cmd, *args = user_input.split()  # Розбиваю команду
    # cmd = cmd.strip().lower()  # Записую команду в окрему змінну
    return cmd, *args  # Повертаю команду і аргументи

def readPort(): #Читаю порт
    # read_L = str(serial.readLine(), 'utf-8').strip().split(",") #Читаю ESP8266 списком даних
    command, *args = parse_input(str(serial.readLine(), 'utf-8').strip())
    match command:
        case "touch": cursor_curation(args)
        

serial.readyRead.connect(readPort)







ui.show()
app.exec()
