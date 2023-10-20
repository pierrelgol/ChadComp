#include "../lib/scanner.h"





/*---------------------- Private Scanner Implementation ---------------------*/

Scanner	*__scanner_init__(FileHandle file, FileInterface interface)
{
	Scanner	*self;

	if (!file || !interface.IsFileValid(file))
		return (NULL);
	self = (Scanner *) calloc(1, sizeof(Scanner));
	if (!self)
		return (NULL);
	self->content = interface.AcquireContent(file);
	if (!self->content)
		return (__scanner_dispose__(self));
	self->size = interface.GetFileSize(file);
	return (self);
}

int __scanner_consume(Scanner *self)
{
	if (!self)
		return (-1);
	if (self->cursor + 1  <= self->size)
		return (self->content[self->cursor++]);
	return (EOF);
}

int __scanner_peek(Scanner *self, int index)
{
	if (!self)
		return (-1);
	if (index >= 0 && index <= self->size)
		return (self->content[index]);
	return (-1);
}

int __scanner_trim(Scanner *self)
{
	int count;

	if (!self)
		return (-1);

	count = 0;
	while (isspace(self->content[self->cursor]))
	{
		if (__scanner_consume(self) == '\n')
			self->line += 1;
		count++;
	}
	return (count);
}

Scanner *__scanner_dispose__(Scanner *self)
{
	if (self)
	{
		if (self->content)
			free(self->content);
		free(self);
	}
	return (NULL);
}

/*---------------------------------------------------------------------------*/





/*------------------------ Public Scanner Interface -------------------------*/

ScannerHandle GetScannerHandle(FileHandle file, FileInterface interface)
{
	return (__scanner_init__(file, interface));
}

int           ScannerConsume(ScannerHandle scanner)
{
	if ((Scanner *)scanner)
		return (__scanner_consume(scanner));
	return (0);
}

int           ScannerPeek(ScannerHandle scanner)
{
	if ((Scanner *)scanner)
		return (__scanner_peek(scanner, ((Scanner *)scanner)->cursor + 1));
	return (0);
}

int           ScannerTrim(ScannerHandle scanner)
{	
	if ((Scanner *)scanner)
		return (__scanner_trim(scanner));
	return (0);
}

void          ReleaseScannerHandle(ScannerHandle scanner)
{
	__scanner_dispose__(scanner);
}

