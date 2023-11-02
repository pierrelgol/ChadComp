#include "lib/lexer.h"
#include <stdio.h>

Scanner 
*scanner_init(const char *path, unique_ptr *content, int content_size, MemRes *memory_manager)
{
	Scanner *self;

	if (!path || !content || content_size == 0 || !memory_manager)
		return (NULL);
	self = memory_manager->alloc(1, sizeof(Scanner));
	if (!self)
		return (NULL);
	self->memory_manager = memory_manager;
	self->content_path = path;
	self->content_size = content_size;
	self->scanner_content = (char *) content;	
	return (self);
}

int
scanner_skip(Scanner *self, int (*controller)(int ch))
{
	int ch;

	if (!self || !controller)
		return (-1);
	ch = scanner_peek_curr_character(self);
	while (controller(ch))
		ch = scanner_eat_character(self);
	return (ch);
}

int 
scanner_rewind_character(Scanner *self)
{
	self->current_cursor_position -= 1;	
	return (self->scanner_content[self->current_cursor_position]);
}

int 
scanner_eat_character(Scanner *self)
{
	if (self->scanner_content[self->current_cursor_position] == '\n')
	{
		self->current_line += 1;
		self->current_line_begin = self->current_cursor_position;
	}
	self->current_cursor_position += 1;	
	if (self->current_cursor_position >= self->content_size)
		return (EOF);
	return (self->scanner_content[self->current_cursor_position]);
}

int 
scanner_peek_curr_character(Scanner *self)
{
	if (self->current_cursor_position >= self->content_size)
		return (EOF);
	else
		return (self->scanner_content[self->current_cursor_position]);
}

int 
scanner_peek_prev_character(Scanner *self)
{
	if (self->current_cursor_position >= self->content_size)
		return (EOF);
	else
		return (self->scanner_content[self->current_cursor_position - 1]);
}

int 
scanner_peek_next_character(Scanner *self)
{
	if (self->current_cursor_position >= self->content_size)
		return (EOF);
	else
		return (self->scanner_content[self->current_cursor_position + 1]);
}

void
scanner_dispose(Scanner *self)
{
	MemRes *memory_manager;

	if (self)
	{
		memory_manager = self->memory_manager;
		if (self->scanner_content)
		{
			memory_manager->dealloc(self->scanner_content);
		}
		memory_manager->dealloc(self);
	}
}
