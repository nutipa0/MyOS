#define VGA_ADDRESS 0xB8000
#define WHITE_ON_BLACK 0x0F
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
// keyboard
#define KEYBOARD_PORT 0x60
#define KEYBOARD_IRQ 33

void print_char(char c)
{
  volatile char *video_memory = (volatile char *)VGA_ADDRESS;
  static int row = 0, col = 0;

  // Если символ - новая строка, переходим на новую строку
  if (c == '\n')
  {
    col = 0;
    row++;
    if (row >= VGA_HEIGHT)
    {
      row = VGA_HEIGHT - 1;
      // Сдвигаем экран вверх на одну строку
      for (int i = 0; i < VGA_HEIGHT - 1; i++)
      {
        for (int j = 0; j < VGA_WIDTH; j++)
        {
          video_memory[(i * VGA_WIDTH + j) * 2] = video_memory[((i + 1) * VGA_WIDTH + j) * 2];
          video_memory[(i * VGA_WIDTH + j) * 2 + 1] = video_memory[((i + 1) * VGA_WIDTH + j) * 2 + 1];
        }
      }
      // Очищаем последнюю строку экрана
      for (int j = 0; j < VGA_WIDTH; j++)
      {
        video_memory[((VGA_HEIGHT - 1) * VGA_WIDTH + j) * 2] = ' ';
        video_memory[((VGA_HEIGHT - 1) * VGA_WIDTH + j) * 2 + 1] = WHITE_ON_BLACK;
      }
    }
  }
  else
  {
    // Печатаем символ на экране
    video_memory[(row * VGA_WIDTH + col) * 2] = c;
    video_memory[(row * VGA_WIDTH + col) * 2 + 1] = WHITE_ON_BLACK;
    col++;

    // Если мы достигли конца строки, переходим на новую строку
    if (col >= VGA_WIDTH)
    {
      col = 0;
      row++;
    }
  }
}

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

static inline unsigned char inb(unsigned short port)
{
  unsigned char result;
  __asm__ __volatile__("inb %1, %0" : "=a"(result) : "dN"(port));
  return result;
}

void register_interrupt_handler(int irq, void (*handler)(void))
{
  interrupt_table[irq] = handler;
}

void keyboard_interrupt_handler(void)
{
  unsigned char key = inb(KEYBOARD_PORT);
  print_char(key);
}

void setup_keyboard_interrupt()
{
  outb(0x21, 0xFD);
  register_interrupt_handler(KEYBOARD_IRQ, keyboard_interrupt_handler);
}

void kernelmain(void)
{
  print_string("Hello World\nHello My Friend");
  while (1)
  {
  }
}