#ifndef SCANNER_H_
#define SCANNER_H_

#include "common.h"

/* Private Scanner Implementation */

Scanner *__scanner_init__(FileHandle file, FileInterface interface);
int      __scanner_consume(Scanner *self);
int      __scanner_peek(Scanner *self, int index);
int      __scanner_trim(Scanner *self);
Scanner *__scanner_dispose__(Scanner *self);

/* Public Scanner Interface */

ScannerHandle GetScannerHandle(FileHandle file, FileInterface interface);
int           ScannerConsume(ScannerHandle scanner);
int           ScannerPeek(ScannerHandle scanner);
int           ScannerTrim(ScannerHandle scanner);
void          ReleaseScannerHandle(ScannerHandle scanner);

#endif // scanner.h
