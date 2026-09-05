#include "stepper.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

#if !defined(__AVR_ATmega2560__) || F_CPU != 16000000UL
#error "Stepper Timer1 configuration requires a 16 MHz ATmega2560."
#endif

namespace {
// Timer1 CTC, prescaler 8: 100 timer counts = 50 us.
// Timer0 (timekeeping), Timer2 (lamp D9), and Timer5 (LED D44) untouched.
// Timer1 is reserved for stepping; PWM on D11/D12 must not be used.
constexpr uint16_t tick_us = 50;
constexpr uint16_t pulse_ticks = 2; // Existing 100 us HIGH.
constexpr uint16_t setup_ticks = 40; // 2 ms direction/enable setup.
volatile bool busy = false;
volatile bool pulse_high = false;
volatile uint16_t ticks_left = 0;
volatile uint16_t period_ticks = 0;
volatile uint16_t steps_left = 0;
volatile uint8_t * volatile step_port = nullptr;
volatile uint8_t step_mask = 0;
}

ISR(TIMER1_COMPA_vect) {
    if (--ticks_left != 0) return;
    if (pulse_high) {
        *step_port &= static_cast<uint8_t>(~step_mask);
        pulse_high = false;
        --steps_left;
        ticks_left = period_ticks - pulse_ticks;
    } else if (steps_left != 0) {
        *step_port |= step_mask;
        pulse_high = true;
        ticks_left = pulse_ticks;
    } else {
        // Complete the final LOW interval before returning to the caller.
        TIMSK1 = 0;
        TCCR1B = 0;
        busy = false;
    }
}

void Stepper::nsteps(int steps, uint8_t direction) {
    if (steps <= 0) return;
    // Preserve the original 200 pulses/revolution speed conversion.
    if (!(rpm > 0.0 && rpm <= 900.0)) return;
    double requested_ticks = 300000.0 / rpm / tick_us;
    if (requested_ticks > 65535.0) return;
    if (!(SREG & _BV(SREG_I))) return; // Avoid deadlock with interrupts off.
    while (busy) {}
    digitalWrite(dir_pin, direction);
    digitalWrite(step_pin, LOW);
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        TCCR1A = 0;
        TCCR1B = 0;
        TIMSK1 = 0;
        TCNT1 = 0;
        OCR1A = 99;
        TIFR1 = _BV(OCF1A) | _BV(OCF1B) | _BV(OCF1C) | _BV(TOV1);
        step_port = portOutputRegister(digitalPinToPort(step_pin));
        step_mask = digitalPinToBitMask(step_pin);
        period_ticks = static_cast<uint16_t>(requested_ticks + 0.5);
        steps_left = static_cast<uint16_t>(steps);
        ticks_left = setup_ticks;
        pulse_high = false;
        busy = true;
        TIMSK1 = _BV(OCIE1A);
        TCCR1B = _BV(WGM12) | _BV(CS11);
    }
    // Synchronous API, interrupt-driven pulses.
    while (busy) {}
}
