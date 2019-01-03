nút *, #, B không có tác dụng
nút 1,2,3,4,5,6,7,8,9,0 để nhập số
nút A là OK
nút B là CANCLE
nút D là tắt màn hình

sơ đồ chân
4 chân trái của keypad nối vào lần lượt: PA12-PA11-PA1-PA0

4 chân phải keypad nối vào lần lượt: PB15-PB14-PB13-PB12

GND-VCC của LCD nên nối vào STLink để đủ dòng

SCL - PB6, SDA - PB7

***** IMPORTANT *****

Thiết lập IDE Keil như sau:
Trên toolbar của Keil, chọn Edit >> Configuration >> chọn tab Editor >> ở 3 mục 
C/C++ files; ASM files và Other files >> Nhấn tick vào Insert spaces for tabs ở cả 3 mục này
>> chọn tab size là 4 >> Nhấn OK để lưu config 