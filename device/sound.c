#include "sound.h"
#include "def.h"
#include "hardware/pio.h"
#include "sound.pio.h"
#include <stdio.h>

#include "hardware/dma.h"

volatile uint8_t *current_buff;

uint8_t buffer1[SOUND_BUFFER_SIZE] __attribute__((aligned(SOUND_BUFFER_SIZE)));
uint8_t buffer2[SOUND_BUFFER_SIZE] __attribute__((aligned(SOUND_BUFFER_SIZE)));

#define PIO_TX_PIN 15

int dma_chan1;
int dma_chan2;

volatile bool emulate = false;
volatile bool draw = false;

volatile uint8 *buffer_start = buffer1;

void dma_handler() {
  static uint32 cnt = 0;
  if (dma_hw->ints0 & 1u << dma_chan1) {
    // Clear the interrupt request.
    dma_hw->ints0 = 1u << dma_chan1;
    emulate = true;
    buffer_start = buffer2;
    if (!(cnt++ % 8)) {
      draw = true;
    }
  } else if (dma_hw->ints0 & 1u << dma_chan2) {
    dma_hw->ints0 = 1u << dma_chan2;
    emulate = true;
    buffer_start = buffer1;
    if (!(cnt++ % 8)) {
      draw = true;
    }
  }
}

int init_sound() {

  const uint sm = 0;

  uint offset = pio_add_program(pio0, &beeper_tx_program);

  beeper_tx_program_init(pio0, sm, offset, PIO_TX_PIN, SYS_FREQ);

  dma_chan1 = dma_claim_unused_channel(true);
  dma_chan2 = dma_claim_unused_channel(true);

  dma_channel_config c1 = dma_channel_get_default_config(dma_chan1);
  channel_config_set_transfer_data_size(&c1, DMA_SIZE_8);
  channel_config_set_read_increment(&c1, true);
  channel_config_set_write_increment(&c1, false);
  channel_config_set_dreq(&c1, DREQ_PIO0_TX0);
  channel_config_set_ring(&c1, false, 8);
  channel_config_set_chain_to(&c1, dma_chan2);
  dma_channel_set_irq0_enabled(dma_chan1, true);
  dma_channel_configure(dma_chan1, &c1, &pio0_hw->txf[0], buffer1,
                        SOUND_BUFFER_SIZE, false);

  dma_channel_config c2 = dma_channel_get_default_config(dma_chan2);
  channel_config_set_transfer_data_size(&c2, DMA_SIZE_8);
  channel_config_set_read_increment(&c2, true);
  channel_config_set_write_increment(&c2, false);
  channel_config_set_dreq(&c2, DREQ_PIO0_TX0);
  channel_config_set_ring(&c2, false, 8);
  channel_config_set_chain_to(&c2, dma_chan1);
  dma_channel_set_irq0_enabled(dma_chan2, true);
  dma_channel_configure(dma_chan2, &c2, &pio0_hw->txf[0], buffer2,
                        SOUND_BUFFER_SIZE, false);

  // Configure the processor to run dma_handler() when DMA IRQ 0 is asserted
  irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
  irq_set_enabled(DMA_IRQ_0, true);
  dma_channel_start(dma_chan1);
}
