#include "get_next_line.h"

char	*cleaner(char **remainder, char *buffer)
{
	if (remainder && *remainder)
	{
		free(*remainder);
		*remainder = NULL;
	}
	if (buffer)
	{
		free(buffer);
		buffer = NULL;
	}
	return (NULL);
}

char	*read_file(int fd, char *buffer, char **remainder)
{
	ssize_t	read_bytes;
	char	*tmp;

	read_bytes = 1;
	while (read_bytes > 0)
	{
		read_bytes = read(fd, buffer, BUFFER_SIZE);
		if (read_bytes < 0)
			return (cleaner(remainder, NULL));
		buffer[read_bytes] = '\0';
		if (read_bytes == 0)
			break ;
		if (*remainder == NULL)
			*remainder = ft_strdup("");
		if (!*remainder)
			return (cleaner(remainder, NULL));
		tmp = ft_strjoin(*remainder, buffer);
		if (!tmp)
			return (cleaner(remainder, NULL));
		cleaner(remainder, NULL);
		*remainder = tmp;
		if (ft_strchr(*remainder, '\n'))
			break ;
	}
	return (*remainder);
}

char	*extract_line(char **remainder)
{
	char *line; 
	char *new_pos;
	char *tmp;

	if (!*remainder || !**remainder)
		return (NULL);
	new_pos = ft_strchr(*remainder, '\n');
	if (new_pos)
	{
		line = ft_substr(*remainder, 0, new_pos - *remainder + 1);
		if (!line)
			return (cleaner(remainder, NULL));
		tmp = ft_strdup(new_pos + 1);
		if (!tmp)
			return (cleaner(remainder, line));
		free(*remainder);
		*remainder = tmp;
	}
	else
	{
		line = ft_strdup(*remainder);
		free(*remainder);
		*remainder = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char *remainder;
	char *saver;
	char *buffer;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd,0,0) < 0)
		return (cleaner(&remainder, NULL));

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (cleaner(&remainder, NULL));

	if (!read_file(fd, buffer, &remainder))
	{
		free(buffer);
		return (NULL);
	}
	free(buffer);
	if (!remainder)
		return (NULL);

	saver = extract_line(&remainder);
	return (saver);
}
