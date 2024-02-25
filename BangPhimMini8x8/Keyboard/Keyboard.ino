#include <TinyWireS.h>     // Sử dụng thư viện TinyWireS cho chức năng I2C slave
#include "DigiKeyboard.h"  // Sử dụng thư viện DigiKeyboard cho chức năng HID

#define I2C_SLAVE_ADDRESS 0x04  // Định nghĩa địa chỉ I2C slave

void setup() {
  TinyWireS.begin(I2C_SLAVE_ADDRESS);  // Khởi tạo I2C slave với địa chỉ đã định
  // Không cần sử dụng onRequest hoặc onReceive vì chúng ta sẽ kiểm tra bộ đệm trong vòng lặp
}

void loop() {
  DigiKeyboard.sendKeyStroke(0);
  // Kiểm tra xem có dữ liệu từ I2C master để đọc khônG
  if (TinyWireS.available()) {
    // Đọc byte được nhận từ I2C master
    byte dataReceived = TinyWireS.read();
    // Xử lý byte nhận được
    processCommand(dataReceived);
  }
  // Quan trọng là phải cho DigiKeyboard cơ hội xử lý các tác vụ USB
  DigiKeyboard.delay(1);
}

void processCommand(byte command) {
  // Tùy vào lệnh nhận được, thực hiện hành động bàn phím tương ứng
  switch (command) {
    // Mỗi trường hợp sẽ tương ứng với một lệnh bàn phím hoặc hành động
    case 0:
      DigiKeyboard.sendKeyStroke(KEY_A, MOD_CONTROL_LEFT);  // Ctrl+A
      break;
    case 1:
      DigiKeyboard.sendKeyStroke(KEY_C, MOD_CONTROL_LEFT);  // Ctrl+C
      break;
    case 2:
      DigiKeyboard.sendKeyStroke(KEY_V, MOD_CONTROL_LEFT);  // Ctrl+V
      break;
    case 3:
      DigiKeyboard.sendKeyStroke(KEY_X, MOD_CONTROL_LEFT);  // Ctrl+X
      break;
    case 4:
      DigiKeyboard.sendKeyStroke(KEY_Z, MOD_CONTROL_LEFT);  // Ctrl+Z
      break;
    case 5:
      DigiKeyboard.sendKeyStroke(KEY_T, MOD_SHIFT_LEFT | MOD_CONTROL_LEFT);  // Ctrl+Shift+T
      break;
    case 6:
      DigiKeyboard.sendKeyStroke(KEY_F, MOD_ALT_LEFT | MOD_SHIFT_LEFT);  // Alt+Shift+F
      break;
    case 7:
      DigiKeyboard.sendKeyStroke(43, MOD_ALT_LEFT);  // Alt+Tab
      break;
    case 8:
      DigiKeyboard.sendKeyStroke(KEY_D, MOD_GUI_LEFT);  // Win+D
      break;
    case 9:
      DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_GUI_LEFT);  // Win+Space
      break;
    case 10:
      DigiKeyboard.sendKeyStroke(KEY_D, MOD_GUI_LEFT);  // Win+D
      break;
    case 11:
      DigiKeyboard.sendKeyStroke(KEY_SPACE, MOD_GUI_LEFT);  // Win+Space
      break;
    case 15:
      DigiKeyboard.sendKeyStroke(82);  // Assuming Roll_Up as Page Up
      break;
    case 16:
      DigiKeyboard.sendKeyStroke(81);  // Assuming Roll_Down as Page Down
      break;
    // Thêm các trường hợp khác theo yêu cầu
    default:
      // Đối với bất kỳ lệnh nào không xử lý, không làm gì hoặc thêm xử lý mặc định
      break;
  }
}
