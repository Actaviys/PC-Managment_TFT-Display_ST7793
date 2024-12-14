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



def cursor_curation(xy: list): # Приймає позицію натиску на сенсор
    print(xy[0], xy[1])


def buttons_screen(valButt): # Функція читання кнопок
    for vb in valButt:
        valB = vb
    print(valB)



def parse_input(user_input: str) -> list:
    """ 
    Функція для парсингу команд
    """
    cmd, args = user_input.split(":")  # Зрізаю команду
    a = args.split(",") # Розбиваю аргументи після команди
    return cmd, a  # Повертаю команду і аргументи

def readPort(): #Читаю порт
    read_L = str(serial.readLine(), 'utf-8').strip() # Читаю дані з порту
    command, args = parse_input(read_L)
    print("Вхідні дані: ", command, args)
    match command:
        case "touch_positions": cursor_curation(args)
        
        case "value_buttons": buttons_screen(args) # Читаю кнопки
        

serial.readyRead.connect(readPort)







ui.show()
app.exec()
