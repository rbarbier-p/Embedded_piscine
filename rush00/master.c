#include "TWI.h"

void master_init() {
    TWBR = 72;   // F_SCL = 100kHz
    TWSR = 0x00; // sets prescaler bits TWPS1 and TWPS0 to zero, so TWPS = 0
                 // (prescaler = 1)
                 // F_SCL = F_CPU / (16 + 2 * TWBR * 4^TWPS)
    TWCR = (1 << TWEN) | (1 << TWINT);
}

void master_state_transmit(void) {
    TWDR = DEFAULT_BIT; // Always set a default value first

    switch (game_state) {
    case GAME_WAITING_PLAYER:
        if (button_pressed) {
            is_master_ready = 1;
            light(WHITE);
        }
        if (is_master_ready && is_slave_ready) {
            TWDR = START_GAME_COUNTDOWN_BIT;
            is_master_ready = 0;
            is_slave_ready = 0;
            game_state = GAME_COUNTDOWN;
            call_timer_countdown();
        }
        break;
    case GAME_COUNTDOWN:
        if (button_pressed) {
            TWDR = OPPONENT_LOST_BIT;
            game_state = GAME_OVER;
            call_timer_game_over(LOST);
        }
        break;
    case GAME_STARTED:
        if (button_pressed) {
            DDRB |= (1 << PB0);  // Set direction FIRST
            PORTB |= (1 << PB0); // Then set value
            TWDR = OPPONENT_BUTTON_PRESSED;
            game_state = GAME_OVER;
            call_timer_game_over(WON);
        }
        break;
    }
    button_pressed = 0;
}

void master_state_receive(void) {
    switch (game_state) {
    case GAME_WAITING_PLAYER:
        if (TWDR == PLAYER_READY_BIT) {
            is_slave_ready = 1;
        }
        break;
    case GAME_COUNTDOWN:
        if (TWDR == OPPONENT_LOST_BIT) {
            game_state = GAME_OVER;
            call_timer_game_over(WON);
        }
        break;
    case GAME_STARTED:
        if (TWDR == OPPONENT_BUTTON_PRESSED) {
            game_state = GAME_OVER;
            call_timer_game_over(LOST);
        }
        break;
    default:
        TWDR = DEFAULT_BIT;
    }
}

void master_loop() {
    uint8_t f_game_over = 0;
    playing = 0;
    uint8_t need_to_timer = 1;

    interrupt_init();
    while (1) {
        //	TRANSMITING
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_START)
            ft_error(ERROR_1);

        TWDR = (SLAVE_ADDR << 1) | WRITE;
        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MT_SLA_ACK)
            ft_error(ERROR_2);

        master_state_transmit();

        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
            ft_error(ERROR_3);

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

        _delay_ms(5);

        //	RECIVING
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_START)
            ft_error(ERROR_4);

        TWDR = (SLAVE_ADDR << 1) | READ;
        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MR_SLA_ACK)
            ft_error(ERROR_5);

        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MR_DATA_NACK)
            ft_error(ERROR_6);

        master_state_receive();

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

        _delay_ms(5);
    }
}