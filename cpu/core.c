#include "core.h"
#include "../device/def.h"
#include "../device/rom.h"
#include "../device/system.h"
#include "operations.h"

u8 m_prgwidth = 11;
u8 m_datawidth = 7;
int m_prgmask;
int m_datamask;

un8 ram[128];

u16 m_pc, m_prev_pc, m_a;
u8 m_cb;
u8 m_c;
u16 m_op, m_prev_op;
u8 m_param;
u8 m_acc;
u8 m_bl;
u8 m_bm;
bool m_skip;
u8 m_r, m_r_out;
u8 m_ox[9];
u8 m_o[9];
u8 m_ox_state[9];
u8 m_o_state[9];
bool m_k_active;
bool m_halt;
int m_clk_div;
u16 m_div;
int m_icount;
bool m_1s;
u8 m_bp;
bool m_bc;
int m_o_pins;
u8 m_cn;
u8 m_mx;
u8 trs_field;

bool m_rsub;

un8 readb(un8 ram_address) { return ram[ram_address]; }

void writeb(un8 ram_address, u8 ram_data) { ram[ram_address] = ram_data; }

u8 bitmask(u16 param) { return 1 << (param & 3); }

un8 read_byte_program(un16 rom_address) { return *(program + rom_address); }

void increment_pc() {
  int feed = ((m_pc >> 1 ^ m_pc) & 1) ? 0 : 0x20;
  m_pc = feed | (m_pc >> 1 & 0x1f) | (m_pc & ~0x3f);
}

u8 ram_r() {
  u8 address = (m_bm << 4 | m_bl) & m_datamask;
  if (address > 0x4f)
    address &= 0x4f;
  return readb(address) & 0xf;
}
void ram_w(u8 data) {
  u8 address = (m_bm << 4 | m_bl) & m_datamask;
  if (address > 0x4f)
    address &= 0x4f;
  writeb(address, data & 0xf);
}

void do_branch(u8 pu, u8 pm, u8 pl) {
  m_pc = ((pu << 10) | (pm << 6 & 0x3c0) | (pl & 0x03f)) & m_prgmask;
}

un8 m_read_ba() { return readBA(); }
un8 m_read_b() { return readB(); }
void m_write_r(un8 data) { writeR(data); }

void clock_melody() {
  u8 out = (~m_r & 0xf);
  m_r_out = out;
  m_write_r(m_r_out);
}

bool wake_me_up() {
  if (m_k_active || m_1s) {
    m_halt = false;
    wakeup_vector();
    return true;
  } else
    return false;
}
void div_timer_cb() {
  m_div = (m_div + 1) & 0x7fff;
  if (m_div == 0) {
    m_1s = true;
    if (m_halt)
      update_segments_state();
  }
  clock_melody();
}
void div_timer(int nb_inst) {
  if (nb_inst > 0)
    for (int toctoc = 0; toctoc < m_clk_div * nb_inst; toctoc++)
      div_timer_cb();
}
void get_opcode_param() {
  if (m_op == 0x5e || m_op == 0x5f) {
    m_icount--;
    m_param = read_byte_program(m_pc);
    increment_pc();
  }
}

void device_start() {
  memset(ram, 0, sizeof(ram));
  memset(m_ox, 0, sizeof(m_ox));
  memset(m_o, 0, sizeof(m_o));
  memset(m_ox_state, 0, sizeof(m_ox_state));
  memset(m_o_state, 0, sizeof(m_o_state));

  m_pc = 0;
  m_prev_pc = 0;
  m_op = 0;
  m_prev_op = 0;
  m_param = 0;
  m_acc = 0;

  m_bl = 0;
  m_bm = 0;
  m_c = 0;
  m_skip = false;
  m_r = 0;
  m_r_out = 0;
  m_div = 0;
  m_1s = false;
  m_k_active = false;
  m_bp = 0;
  m_bc = false;
  m_halt = false;
  m_clk_div = 2;
  m_icount = 0;

  m_rsub = false;
  m_o_pins = 9;
  m_prgmask = (1 << m_prgwidth) - 1;
  m_datamask = (1 << m_datawidth) - 1;
  trs_field = 1;

  m_cn = 0;
  m_mx = 0;

  m_cb = 0;
}
void device_reset() {

  m_skip = false;
  m_halt = false;
  m_op = m_prev_op = 0;

  m_prev_pc = m_pc;
  m_bp = 1;
  m_bc = false;
  m_r = m_r_out = 0;
  reset_vector();
  m_a = m_pc;
  op_idiv();

  m_1s = true;
  m_cb = 0;
  m_rsub = false;
  m_r = 0xff;
  m_r_out = 0;
}
void execute_one() {
  switch (m_op & 0xf0) {
  case 0x20:
    op_lax();
    break;
  case 0x30:
    op_adx();
    break;
  case 0x40:
    op_lb();
    break;
  case 0x70:
    op_ssr();
    break;
  case 0x80:
  case 0x90:
  case 0xa0:
  case 0xb0:
    op_tr();
    break;
  case 0xc0:
  case 0xd0:
  case 0xe0:
  case 0xf0:
    op_trs();
    break;
  default:
    switch (m_op & 0xfc) {
    case 0x04:
      op_rm();
      break;
    case 0x0c:
      op_sm();
      break;
    case 0x10:
      op_exc();
      break;
    case 0x14:
      op_exci();
      break;
    case 0x18:
      op_lda();
      break;
    case 0x1c:
      op_excd();
      break;
    case 0x54:
      op_tmi();
      break;
    default:
      switch (m_op) {
      case 0x00:
        op_skip();
        break;
      case 0x01:
        op_atr();
        break;
      case 0x02:
        op_sbm(); //?
        break;
      case 0x03:
        op_atbp();
        break;
      case 0x08:
        op_add();
        break;
      case 0x09:
        op_add11(); //?
        break;
      case 0x0a:
        op_coma();
        break;
      case 0x0b:
        op_exbla();
        break;
      case 0x50:
        op_tal();
        break;
      case 0x51:
        op_tb();
        break;
      case 0x52:
        op_tc();
        break;
      case 0x53:
        op_tam();
        break;
      case 0x58:
        op_tis(); //?
        break;
      case 0x59:
        op_ptw();
        break;
      case 0x5a:
        op_ta0();
        break;
      case 0x5b:
        op_tabl();
        break;
      case 0x5c:
        op_tw();
        break;
      case 0x5d:
        op_dtw();
        break;
      case 0x5f:
        op_lbl();
        break;
      case 0x60:
        op_comcn();
        break;
      case 0x61:
        op_pdtw();
        break;
      case 0x62:
        op_wr();
        break;
      case 0x63:
        op_ws();
        break;
      case 0x64:
        op_incb();
        break;
      case 0x65:
        op_idiv();
        break;
      case 0x66:
        op_rc();
        break;
      case 0x67:
        op_sc();
        break;
      case 0x68:
        op_rmf();
        break;
      case 0x69:
        op_smf();
        break;
      case 0x6a:
        op_kta();
        break;
      case 0x6b:
        op_rbm(); //?
        break;
      case 0x6c:
        op_decb();
        break;
      case 0x6d:
        op_comcb();
        break;
      case 0x6e:
        op_rtn0();
        break;
      case 0x6f:
        op_rtn1();
        break;
      case 0x5e:
        m_op = m_op << 8 | m_param;
        switch (m_param) {
        case 0x00:
          op_cend();
          break;
        case 0x04:
          op_dta();
          break;
        default:
          op_illegal();
          break;
        }
        break;
      default:
        op_illegal();
        break;
      }
      break;
    }
    break;
  }
}
void device_run() {
  int remaining_icount = m_icount;

  while (m_icount > 0) {
    m_icount--;

    if (m_halt && !wake_me_up()) {
      div_timer(remaining_icount);
      m_icount = 0;
      return;
    }

    m_prev_op = m_op;
    m_prev_pc = m_pc;
    m_op = read_byte_program(m_pc);

    increment_pc();
    get_opcode_param();

    if (m_skip) {
      m_skip = false;
      m_op = 0;
    } else
      execute_one();

    div_timer(remaining_icount - m_icount);
    remaining_icount = m_icount;
  }
}
