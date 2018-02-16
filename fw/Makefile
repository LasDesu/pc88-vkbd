TARGETS=adp_uart.hex
#AVRA=/home/las/programming/avra/src/avra
#AVR_INCLUDES=/home/las/programming/avra/includes

AVRA=avra
AVR_INCLUDES=/usr/include/avr

all: $(TARGETS)

clean:
	rm -f $(TARGETS) *.lst *.eep.hex *.obj *.cof

%.hex: %.asm
	$(AVRA) -I $(AVR_INCLUDES) -l $*.lst $<
