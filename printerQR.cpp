#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "/var/www/kodxmcu/printer/drive-download-20230220T060945Z-001/QR-Code-generator-master/cpp/qrcodegen.hpp"
#include <cstddef>  // c++ 17 ile derlemen gerekir.Bu kütüphaneden dolayı 
#include <vector>
#include <sstream>




using namespace std;

/*
std::string base64Encode(const std::string& data)
{
    const std::string base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    result.reserve(((data.size() / 3) + 1) * 4);

    for (size_t i = 0; i < data.size(); i += 3) {
        const uint32_t val = (static_cast<uint32_t>(data[i]) << 16) |
                              ((i + 1 < data.size()) ? (static_cast<uint32_t>(data[i + 1]) << 8) : 0) |
                              ((i + 2 < data.size()) ? (static_cast<uint32_t>(data[i + 2])) : 0);

        result.push_back(base64Chars[(val >> 18) & 0x3F]);
        result.push_back(base64Chars[(val >> 12) & 0x3F]);
        if (i + 1 < data.size()) {
            result.push_back(base64Chars[(val >> 6) & 0x3F]);
        }
        if (i + 2 < data.size()) {
            result.push_back(base64Chars[val & 0x3F]);
        }
    }

    const size_t paddingCount = (3 - (data.size() % 3)) % 3;
    for (size_t i = 0; i < paddingCount; ++i) {
        result.push_back('=');
    }

    return result;
}

*/
std::string stringToHexString(const std::string& data)
{
    static constexpr char hexDigits[] = "0123456789ABCDEF";
    std::string hexString;
    hexString.reserve(data.size() * 2);
    for (unsigned char byte : data) {
        hexString += hexDigits[(byte >> 4) & 0xF];
        hexString += hexDigits[byte & 0xF];
    }
    return hexString;
}



std::string qrAscii(const std::string& qrCodeData) {
    std::stringstream ss;
    for (const char& c : qrCodeData) {
        ss << static_cast<int>(c) << " ";
    }
    return ss.str();
}


std::string generateQRCode(const std::string& text)
{
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text.c_str(), qrcodegen::QrCode::Ecc::LOW);
    std::string qrData;
     int width = qr.getSize() * 2 + 2; // QR kodunun toplam karakter genişliği
    for (int y = 0; y < qr.getSize(); y++) {
         int emptySpace = (width - (qr.getSize() * 2)) / 2; // satır başına eklenecek boşluk sayısı
          qrData += std::string(emptySpace, ' ');  // boşlukları ekle
        for (int x = 0; x < qr.getSize(); x++) {
            qrData += qr.getModule(x, y) ? "\xE2\x96\x88" : " ";
        }
        qrData += "\n";
    }
    
    cout << "QR CİKTİSİ  : " <<endl;
    cout << endl ;

   
   cout << qrData  << endl ;

    return qrData;
}



std::string addHeaderAndFooter(const std::string& data, const std::string& header, const std::string& footer)
{
    std::string result;
    result.reserve(data.size() + header.size() + footer.size());
    result.append(header);
    result.append(data);
    result.append(footer);
    return result;
}


void writeQRCode(const std::string& qrCodeData, std::ofstream& out)
{
    
    
  std::string v1 = stringToHexString(qrCodeData);
    
    // Sıfırlama işlemleri
    std::cout << "Clearing serial port\n";
    std::cout << "Setting printer status to busy\n";
    int writtenLineCount = 0;

    // Başlık ve bitiş baytlarını tanımlama
    std::cout << "Adding header and footer\n";
    std::string header = { 0x02, 0x30, 0x31, 0x50};
    std::string footer;
    if (std::count(v1.begin(), v1.end(), '\n') == 1) {
        footer = { 0x0A, 0x0B, 0x03 , 0x0FA};
    } else {
        footer = { 0x03, 0xFA,};
    }
    std::string message = addHeaderAndFooter(v1, header, footer);
   cout << "message : " << message << endl;
    out.write(message.data(),message.size());
    
    
    
  }
  

int main()
{

    std::ofstream out("/dev/ttyS3");
    std::string qrCodeData = "mert arslan";
    
    writeQRCode(qrCodeData, out);
   
   return 0 ;

}
