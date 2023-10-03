g++ -c -std=c++17 printerQR.cpp /var/www/kodxmcu/printer/drive-download-20230220T060945Z-001/QR-Code-generator-master/cpp/qrcodegen.cpp
g++ printerQR.o qrcodegen.o -o run
./run

