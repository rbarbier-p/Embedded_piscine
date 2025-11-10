#include "TWI.h"

uint8_t asign_role() {
    DDRC &= ~((1 << PC4) | (1 << PC5));
    DDRD &= ~(1 << PD4);
    PORTD |= (1 << PD4);
    PORTC |= (1 << PC4) | (1 << PC5);

    slave_init();
    light(GREEN);
    while (1) {
        if (TWCR & (1 << TWINT)) {
            uint8_t status = TWSR & TW_STATUS_MASK;
            if (status == TW_SR_SLA_ACK)
                return SLAVE;
        }
        if (!(PIND & (1 << PD4))) // if button pressed, go master
        {
            return MASTER;
        }
    }
}

void main() {
    uint8_t role = asign_role();
    game_state = GAME_WAITING_PLAYER;

    light(BLACK);

    if (role == MASTER) {
        light(LED4);
        master_init();
        master_loop();
    } else if (role == SLAVE) {
        light(LED3);
        slave_loop();
    }
}