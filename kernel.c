#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void print_string(const char *str)
{
  volatile char *video_memory = (volatile char *)VGA_ADDRESS;
  int row = 0, col = 0;

  while (*str)
  {
    if (*str == '\n')
    {
      col = 0;
      row++;
      if (row >= VGA_HEIGHT)
      {
        row = VGA_HEIGHT - 1;

        for (int i = 0; i < VGA_HEIGHT - 1; i++)
        {
          for (int j = 0; j < VGA_WIDTH; j++)
          {
            video_memory[(i * VGA_WIDTH + j) * 2] = video_memory[((i + 1) * VGA_WIDTH + j) * 2];
            video_memory[(i * VGA_WIDTH + j) * 2 + 1] = video_memory[((i + 1) * VGA_WIDTH + j) * 2 + 1];
          }
        }

        for (int j = 0; j < VGA_WIDTH; j++)
        {
          video_memory[((VGA_HEIGHT - 1) * VGA_WIDTH + j) * 2] = ' ';
          video_memory[((VGA_HEIGHT - 1) * VGA_WIDTH + j) * 2 + 1] = WHITE_ON_BLACK;
        }
      }
    }
    else
    {
      video_memory[(row * VGA_WIDTH + col) * 2] = *str;
      video_memory[(row * VGA_WIDTH + col) * 2 + 1] = WHITE_ON_BLACK;
      col++;
      if (col >= VGA_WIDTH)
      {
        col = 0;
        row++;
      }
    }
    str++;
  }
  // *video_memory++ = *str++;
  // *video_memory++ = WHITE_ON_BLACK;
}

void kernelmain(void)
{
  print_string("Hello World\nHello My Friend");
  while (1)
  {
  }
}