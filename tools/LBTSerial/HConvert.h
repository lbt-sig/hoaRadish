#ifndef HCONVERT_H
#define HCONVERT_H
#include <stdint.h>
#include <QString>
#include <QByteArray>

// char 转为 16 进制
int8_t convertCharToHex(char ch);

// QString 转为 16 进制
void convertStringToHex(const QString &str, QByteArray &byteData);

#endif // HCONVERT_H
