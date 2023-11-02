#include "lib/lexer.h"

/*

	@Function  : File   *file_init(const char *file_path, MemRes *memory_manager)

	@arguments : const char *file_path  -> (path to the file to open)
	@arguments : MemRes *memory_manager -> (pointer to a memory_manager interface described in lexer.h in the Memory.c section)

	@return    : if successful the function returns a File * object, that is
		     properly initialized. meaning the path was correct, the file was successfuly
		     opened, and the content of the file was loaded into a properly sized buffer.
		     else the function will return (NULL) both file_path and memory_manager needs
		     to be valid objects.
*/

File  
*file_init(const char *file_path, MemRes *memory_manager)
{
	File *self;

	if (!file_path || !memory_manager)
		return (NULL);
	self = memory_manager->alloc(1, sizeof(File));
	if (!self)
		return (NULL);
	self->file_path = file_path;
	self->memory_manager = memory_manager;
	if (!file_fstat(self))
	{
		file_dispose(self);
		return (NULL);
	}
	self->file_content = (char *) memory_manager->alloc(self->content_size + 1, sizeof(char));
	if (!self->file_content)
	{
		file_dispose(self);
		return (NULL);
	}
	if (!file_open(self))
	{
		file_dispose(self);
		return (NULL);
	}
	if (!file_read(self))
	{
		file_dispose(self);
		return (NULL);
	}
	return (self);
}

/*
	@function : bool file_open(File *self)

	@argument : File *self (pointer to a valid file object).

	@return : file_open will return true or false depending of whether it could open
		  a new file according to it's internal logic. (for example the function
		  will refuse to open a new file, il one is already flagged as open) This
		  is to increase the robustness of the program.
*/

bool
file_open(File *self)
{
	if (!self || (self->file_flags & FILE_IS_OPEN))	
		return (false);
	self->file_descriptor = open(self->file_path, O_RDWR);
	if (self->file_descriptor == -1)
	{
		self->file_flags |= FILE_OPEN_FAILED;
		return (false);
	}
	else
	{
		self->file_flags |= FILE_IS_OPEN;
		return (true);
	}
}

/*
	@function : bool file_fstat(File *self)

	@argument : File *self (pointer to a valid file object).

	@return : file_fstat will return true or false depending of whether it could open
		  a new file according to it's internal logic and get a valid length for the openeds file
		  . (for example the function will refuse to open a new file, il one is already
		  flagged as open) This is to increase the robustness of the program. It will also return
		  false in case the value read is incorrect.
*/

bool
file_fstat(File *self)
{
	char	*buffer;
	int	size;

	if (!self || (self->file_flags & FILE_IS_OPEN))	
		return (false);
	if (!file_open(self))
		return (false);
	size = READ_SIZE;
	buffer = (char [READ_SIZE]) {0};
	while (size == READ_SIZE)
	{
		size = read(self->file_descriptor, buffer, READ_SIZE);
		self->content_size += size;
	}
	if (size <= 0)
	{
		self->file_flags |= FILE_READ_FAILED;
		return (false);
	}
	file_close(self);
	return (true);
}

/*
	@function : bool file_read(File *self)

	@argument : File *self (pointer to a valid file object).

	@return : file_read will return true or false depending of whether it could
		  read the opened file descriptor, it will also toggle the bit as a
		  signal for the receiver

	@note : this design of functions using bitwise operations to flag certain
		errors is still experimental and I need more testing to be sure 
		that this is what I want for the program as a whole this might
		become messy for the rest of the program especially as the logics
		get more complex, I might need to crease some sort of general purpose
		error handling processor or state manager, that will get periodically
		the state of the current object and assert that it is in a valid state

	@note : this might create a lot of overhead especially if we want to implemnt
		this for everything in the program.
*/

bool
file_read(File *self)
{
	if (!self || (self->file_flags & FILE_READ_FAILED))
		return (false);
	self->content_size = read(self->file_descriptor, self->file_content, self->content_size);
	if (self->content_size <= 0)
	{
		self->file_flags ^= FILE_READ_FAILED;
		return (false);
	}
	else
	{
		self->file_flags ^= FILE_IS_READ;
		return (true);
	}
}

/*
	@function : bool file_close(File *self)

	@argument : File *self (pointer to a valid File object)

	@return : This function will return true or false depending on the inputs
		  for example if a file is not open it will return false, otherwise
		  it will simply toggle the FILE_IS_OPEN bit in the file_flags variable
*/

bool
file_close(File *self)
{
	if (!(self->file_flags & FILE_IS_OPEN))	
		return (false);
	self->file_descriptor = close(self->file_descriptor);
	self->file_flags ^= FILE_IS_OPEN;

	return (true);
}

/* 	@function : File *file_reuse(File *self, const char *new_path)

	@argument : File *self (pointer to a valid File object (should be used in the context of recycling the memory of an existing File object )) 
	@argument : const char *new_path (pointer to a Cstring of the path of the file to open).

	@return : The function will return NULL in case new_path or self aren't valid pointers or a memory_allocation failed.
	@return : It will return the same object with the new file loaded inside memory (need to do more testing)
	@return : it will return a new object entirely if the file_content pointer is NULL (for example the ownership was passed along to another object).

	@Robusteness (Try to test what happen when the next path is invalid as this might be a potential leak source)


*/

File
*file_reuse(File *self, const char *new_path)
{
	File	*new_self;
	int	current_size;

	if (!new_path || !self)
		return (NULL);
	file_close(self);
	self->file_path = new_path;
	if (self->file_content)
	{
		current_size = self->content_size;
		file_fstat(self);
		if (current_size >= self->content_size)
		{
			finish_initialization :
			self->file_flags ^= self->file_flags;
			if (!file_open(self))
			{
				file_dispose(self);
				return (NULL);
			}
			if(!file_read(self))
			{
				file_dispose(self);
				return (NULL);
			}
			return (self);
		}
		else
		{
			self->memory_manager->dealloc(self->file_content);
			self->file_content = self->memory_manager->alloc(self->content_size + 1, sizeof(char));
			if (!self->file_content)
			{
				file_dispose(self);
				return (NULL);
			}
			else
				goto finish_initialization;
		}
	}
	else 
	{
		new_self = file_init(new_path, self->memory_manager);
		file_dispose(self);
		return (new_self);
	}
	return (self);
}


void
file_dispose(File *self)
{
	MemRes memory_manager;

	if (self)
	{
		memory_manager = *self->memory_manager;
		if (self->file_flags & FILE_IS_OPEN)
			file_close(self);
		if (self->file_content)
			memory_manager.dealloc(self->file_content);
		memory_manager.dealloc(self);
	}
}
