.include "m328Pdef.inc"

.equ	KBD_PORT	= PORTB
.equ	KBD_DDR		= DDRB
.equ	KBD_PIN		= PB0

.def	tmp		= r16
.def	tmph	= r17
.def	tmp2	= r18

.def	s_store		= r6
.def	pin_buf		= r7
.def	uart_val0	= r8
.def	uart_val1	= r9
.def	m_tmp		= r10

.equ	sys_freq	= 8000000

.equ	kbd_clk		= 20800
.equ	prescale	= 8
.equ	kbd_div	= sys_freq / kbd_clk / prescale

.equ	uart_clk	= 9600
.equ	uart_div	= sys_freq / 16 / uart_clk - 1

.equ	kbd_buf		= SRAM_START

.CSEG ;	ROM
	.org 0
		rjmp	reset
	
	.org OC0Aaddr
		rjmp	uart_next

	.org INT_VECTORS_SIZE
reset:
		cli
		
		; set stack
		ldi		tmp, low(RAMEND)
		out		SPL, tmp
		ldi		tmp, high(RAMEND)
		out		SPH, tmp
		
		; set I/O
		ser		tmp
		mov		uart_val0, tmp
		mov		uart_val1, tmp
		
		ldi		tmp, (1 << KBD_PIN)
		mov		pin_buf, tmp
		out		KBD_PORT, pin_buf
		
		ldi		tmp, (1 << KBD_PIN)
		out		KBD_DDR, tmp
		
		; setup keyboard timer period
		ldi		tmp, kbd_div
		out		OCR0A, tmp
		; clear on compare match
		ldi		tmp, (2<<WGM00)
		out		TCCR0A, tmp
		; 8x prescaler
		ldi		tmp, (2<<CS00)
		out		TCCR0B, tmp
		; enable overflow interrupt
		ldi		tmp, (1<<OCIE0A)
		sts		TIMSK0, tmp
		
		; set UART baud rate
		ldi		tmp, high(uart_div)
		sts		UBRR0H, tmp
		ldi		tmp, low(uart_div)
		sts		UBRR0L, tmp
		; enable receiver
		ldi		tmp, (1<<RXEN0)
		sts		UCSR0B, tmp
		; set frame format: 8data, 1stop bit
		ldi		tmp, (0<<USBS0)|(3<<UCSZ00)
		sts		UCSR0C, tmp
		
		; fill keyboard buffer
		ldi		XH, high(kbd_buf)
		clr		XL
		
		ser		tmp
		ldi		tmph, 0xE
fill_buf:
		st		X+, tmp
		dec		tmph
		brne	fill_buf
		ldi		tmp, 0x7F
		st		X, tmp
		
		sei

		;rcall	key_event	; debug

main_loop:
		; wait for data and process it
		lds		tmp, UCSR0A
		sbrc	tmp, RXC0
		rcall	key_event
		; repeat
		rjmp	main_loop
		
uart_next:
		in		s_store, SREG

		; update pin state
		bst		uart_val0, 0
		bld		pin_buf, KBD_PIN
		out		KBD_PORT, pin_buf
		
		; shift bits
		sec
		ror		uart_val1
		ror		uart_val0
		
		out		SREG, s_store
		
		reti

key_event:
		lds		m_tmp, UDR0
		
		; get row address
		mov		tmp, m_tmp
		lsl		tmp
		swap	tmp
		andi	tmp, 0xF
		ldi		XH, high(kbd_buf)
		mov		XL, tmp
		
		; get bit mask
		mov		tmp, m_tmp
		andi	tmp, 7
		inc		tmp
		
		clr		tmph
		sec
get_mask:
		rol		tmph
		dec		tmp
		brne	get_mask
		
		; load current state
		ld		tmp, X

		; make or break key
		sbrs	m_tmp, 7
		rjmp	key_make
		
		or		tmp, tmph
		rjmp	store_keys

key_make:
		com		tmph
		and		tmp, tmph
		
store_keys:
		; save new state
		st		X, tmp
		
		; prepare data for transmit
		; row
		mov		tmph, m_tmp
		lsl		tmph
		swap	tmph
		andi	tmph, 0xF
		mov		m_tmp, tmph
		
		; mask
		swap	tmp
		mov		tmph, tmp
		andi	tmph, 0x0F
		andi	tmp, 0xF0
		or		tmp, m_tmp
		
		; calculate parity
		clr 	m_tmp
		ldi		tmp2, 16
		clc
calc_parity:
		ror		tmph
		ror		tmp
		brcc	parity_zero
		inc		m_tmp
parity_zero:
		dec		tmp2
		brne	calc_parity
		; put last bit back 
		ror		tmph
		ror		tmp
		
		;com		m_tmp	; for odd
		bst		m_tmp, 0
		bld		tmph, 12-8
		
		; make start/stop bits
		clc
		rol		tmp
		rol		tmph
		ori		tmph, 0xC0
		
		; set buffer
		cli
		mov		uart_val0, tmp
		mov		uart_val1, tmph
		sei

		ret
