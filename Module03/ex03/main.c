#include "inc.h"

int main(void)
{
    char read_byte;
    char buffer[BUFF_SIZE + 1] = {0};

    uart_init();
    init_rgb();
    init_timer();

    uart_print("Enter an HEX RGB color (#RRGGBB): #");

    while (1)
    {
        read_byte = uart_rx();
        
        if (read_byte == DELETE)
            ft_remove_from_buffer(buffer);
        else if (read_byte == ENTER)
        {
            if (ft_check_input(buffer))
                uart_print("\n\r[Input error]");
            else
                set_rgb(hex_to_dec(buffer), hex_to_dec(buffer + 2), hex_to_dec(buffer + 4));
            ft_clear_buffer(buffer);
            uart_print("\n\rEnter an HEX RGB color (#RRGGBB): #");
        }
        else {
            ft_add_to_buffer(read_byte, buffer);
        }
    }
}