from PyQt5 import QtWidgets, uic
from PyQt5.QtSerialPort import QSerialPort, QSerialPortInfo
from PyQt5.QtCore import QIODevice
import pyautogui as pg

app = QtWidgets.QApplication([])
ui = uic.loadUi("Wind_Port.ui") # #F:/Arduino(sketch)/esp8266_ModeMCU1.0(ESP-12E_Module)/Server_PCmanagment_ESP8266/Python_Files/Window_Server_ESP8266.ui
ui.setWindowTitle("MEGA-2560")



serial = QSerialPort() # Створюю порт
serial.setBaudRate(9600) #Швидкість Порту 460800
ports = QSerialPortInfo().availablePorts() # Зберігаю інформацію про порти
ports_list = []
for pr in ports:
    ports_list.append(pr.portName())
ui.PortBox.addItems(ports_list) # Додаю список портів до `PortBox`
serial.setPortName("COM9") # Підключаюсь до порту
serial.open(QIODevice.ReadWrite)
print("-"*50)
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


def serialSend(dataText):
    sdT = str(dataText)
    serial.write(sdT.encode())
    # print(dataText)


def LineSend():
    editLine = ui.lineEditMessage.displayText()
    outpText = f"0:{editLine}"
    serialSend(outpText)
ui.SendMessageButton.clicked.connect(LineSend)




def cursor_curation(xy: list): # Функція керування курсором
    print(xy[0], xy[1])
    x = int(xy[0])
    y = int(xy[1])
    # print(x, y)
    # pg.move(0, 116, 0.8) 
    pg.moveTo(x, y)


def buttons_screen_read(valButt: list): # Функція читання кнопок
    # for vb in valButt:
    #     valB = vb
    print(valButt[0], valButt[1])



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
    # print("Вхідні дані: ", command, args)
    match command:
        case "touch_positions":
            cursor_curation(args) # Приймає позицію натиску на сенсор
            
        case "value_buttons": # Читаю кнопки
            buttons_screen_read(args) 
        

serial.readyRead.connect(readPort)







ui.show()
app.exec()
