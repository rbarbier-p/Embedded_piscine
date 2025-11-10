#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void SPI_master_init(void) {
	DDRB = (1 << PB3) | (1 << PB5) | (1 << PB2);
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0); // enable spi, master, set clock rate fck/16
}

void SPI_master_send(uint8_t data) {
	SPDR = data;

	while(!(SPSR & (1<<SPIF)));
}

void SPI_APA102_start(void) {
  PORTB |= (1 << PB2);
  SPI_master_send(0x0);
  SPI_master_send(0x0);
  SPI_master_send(0x0);
  SPI_master_send(0x0);
}
  
void SPI_APA102_frame(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
  SPI_master_send(0xE0 | brightness & 0x1F);
  SPI_master_send(b);
  SPI_master_send(g);
  SPI_master_send(r);
}

void SPI_APA102_stop(void) {
  SPI_master_send(0xFF);
  SPI_master_send(0xFF);
  SPI_master_send(0xFF);
  SPI_master_send(0xFF);
  PORTB &= ~(1 << PB2);
}

void SPI_APA102_set_color()

int main(void)
{
	SPI_master_init();

  while (1) {
    SPI_APA102_start();
    SPI_APA102_frame(0xFF, 0x0, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_stop(); 
    _delay_ms(1000);
    SPI_APA102_start();
    SPI_APA102_frame(0x0, 0xFF, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_stop(); 
    _delay_ms(1000);
    SPI_APA102_start();
    SPI_APA102_frame(0x0, 0x0, 0xFF, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_stop(); 
    _delay_ms(1000);
    SPI_APA102_start();
    SPI_APA102_frame(0xFF, 0xFF, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_stop(); 
    _delay_ms(1000);
    SPI_APA102_start();
    SPI_APA102_frame(0x0, 0xFF, 0xFF, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_stop(); 
    _delay_ms(1000);
    SPI_APA102_start();
    SPI_APA102_frame(0xFF, 0x0, 0xFF, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_stop(); 
    _delay_ms(1000);
    SPI_APA102_start();
    SPI_APA102_frame(0xFF, 0xFF, 0xFF, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_frame(0x0, 0x0, 0x0, 0x01);
    SPI_APA102_stop(); 
    _delay_ms(1000);
  }

	return 0;
}
