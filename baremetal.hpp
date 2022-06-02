#ifndef bare_h
#define bare_h

#define CLEAR_BIT(num, index) \
  num = num & (~(1 << index))
#define SET_BIT(num, index) \
  num = num | (1 << index)

#define READ_BIT(num, index) \
  (num >> index) & 0x01;
#define WRITE_BIT(num, index, val) \
  num = (num & ~(1 << index)) | (val << index);

uint8_t digital_read(uint8_t pin)
{
  uint8_t value;

  if (pin <= 7)
  {

    CLEAR_BIT(DDRD, pin);
    value = READ_BIT(PIND, pin);
  }
  else if (pin <= 13)
  {
    pin -= 8;
    CLEAR_BIT(DDRB, pin);
    value = READ_BIT(PINB, pin);
  }
  else
  {
    pin -= 14;
    CLEAR_BIT(DDRC, pin);
    value = READ_BIT(PINC, pin);
  }
  return value;
}

void digital_write(uint8_t pin, uint8_t value)
{
  // uint8_t value;

  if (pin <= 7)
  {

    SET_BIT(DDRD, pin);
    WRITE_BIT(PORTD, pin, value);
  }
  else if (pin <= 13)
  {
    pin -= 8;
    SET_BIT(DDRB, pin);
    WRITE_BIT(PORTB, pin, value);
  }
  else
  {
    pin -= 14;
    SET_BIT(DDRC, pin);
    WRITE_BIT(PORTC, pin, value);
  }
}

void adc_init()
{
  // AREF = AVcc
  ADMUX = (1 << REFS0);

  // ADC Enable and prescaler of 128
  // 16000000/128 = 125000
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t adc_read(uint8_t ch)
{
  // select the corresponding channel 0~5
  // ANDing with ’7′ will always keep the value
  // of ‘ch’ between 0 and 5
  ch &= 0b00000111;            // AND operation with 7
  ADMUX = (ADMUX & 0xF8) | ch; // clears the bottom 3 bits before ORing
  // start single conversion
  // write ’1′ to ADSC
  ADCSRA |= (1 << ADSC);
  // wait for conversion to complete
  // ADSC becomes ’0′ again
  // till then, run loop continuously
  while (ADCSRA & (1 << ADSC))
    ;

  return (ADC);
}

void PWM_OC1A_init(void)
{
  // FAST PWM 10bit, no prescaler,
  // clear timer on compare(non inverting mode) , duty is time high
  TCCR1A |= 1 << COM1A1 | 1 << WGM11 | 1 << WGM10 | 1 << COM1B1;
  TCCR1B |= 1 << WGM12 | 1 << CS10;

  // TCCR1A |= 1<<COM1B1 | 1<<WGM11 | 1<<WGM10;
  // TCCR1B |= 1<<WGM12 | 1<<CS10;

  //  TTCR1B= 1<<COM1B1 | 1<<WGM11 | 1<<WGM10;
}

void PWM_OC1A_set_duty(uint8_t pin, uint8_t duty)
{
  // make pin -8  because we use pin9,10 which are pb1,pb2
  // DDRB |= (1 << (pin - 8));
  SET_BIT(DDRB, (pin - 8));
  if (pin == 9)
  {
    OCR1A = (duty * 4);
  }
  else
  {
    OCR1B = (duty * 4);
  }
}

#endif