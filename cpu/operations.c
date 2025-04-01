
#include "operations.h"
#include "../device/def.h"
#include "../device/system.h"
#include "core.h"
#include <stdio.h>

void reset_vector() { do_branch(0, 0xf, 0); }
void wakeup_vector() {
  m_cb = 0;
  do_branch(0, 0, 0);
}
void set_su(u8 su) { m_a = (m_a & ~0x3c0) | (su << 6 & 0x3c0); }

u8 get_su() { return m_a >> 6 & 0xf; }

int get_trs_field() { return trs_field; }

void shift_w() {
  for (int i = 0; i < (m_o_pins - 1); i++)
    m_ox[i] = m_ox[i + 1];
}

void update_segments_state() {
  for (int i = 0; i < m_o_pins; i++) {
    m_o_state[i] = m_o[i];
    m_ox_state[i] = m_ox[i];
  }
}

u8 get_digit() {
  static const u8 lut_digits[0x20] = {0xe, 0x0, 0xc, 0x8, 0x2, 0xa, 0xe, 0x2,
                                      0xe, 0xa, 0x0, 0x0, 0x2, 0xa, 0x2, 0x2,
                                      0xb, 0x9, 0x7, 0xf, 0xd, 0xe, 0xe, 0xb,
                                      0xf, 0xf, 0x4, 0x0, 0xd, 0xe, 0x4, 0x0};

  return lut_digits[m_cn << 4 | m_acc] | (~m_cn & m_mx);
}
void op_lb() {
  m_bm = m_op & 3;
  m_bl = (m_op >> 2 & 3) | ((m_op & 0xc) ? 8 : 0);
}
void op_incb() {
  m_bl = (m_bl + 1) & 0xf;
  m_skip = (m_bl == 8);
}
void op_decb() {
  m_bl = (m_bl - 1) & 0xf;
  m_skip = (m_bl == 0xf);
}
void op_sbm() { m_bm |= 4; }
void op_rbm() { m_bm &= ~4; }
void op_comcb() { m_cb ^= 1; }
void op_rtn0() {
  update_segments_state();
  m_pc = m_a & m_prgmask;
  m_rsub = false;
}
void op_rtn1() {
  op_rtn0();
  m_skip = true;
}
void op_ssr() { set_su(m_op & 0xf); }
void op_tr() {
  m_pc = (m_pc & ~0x3f) | (m_op & 0x3f);
  if (!m_rsub)
    do_branch(m_cb, get_su(), m_pc & 0x3f);
}
void op_trs() {
  if (!m_rsub) {
    m_rsub = true;
    u8 su = get_su();
    m_a = m_pc;
    do_branch(get_trs_field(), 0, m_op & 0x3f);
    if ((m_prev_op & 0xf0) == 0x70)
      do_branch(m_cb, su, m_pc & 0x3f);
  } else
    m_pc = (m_pc & ~0xff) | (m_op << 2 & 0xc0) | (m_op & 0xf);
}
void op_exc() {
  u8 a = m_acc;
  m_acc = ram_r();
  ram_w(a);
  m_bm ^= (m_op & 3);
}
void op_exci() {
  op_exc();
  op_incb();
}
void op_excd() {
  op_exc();
  op_decb();
}
void op_atbp() {
  m_bp = m_acc & 1;
  m_cn = m_acc >> 3 & 1;
}
void op_ptw() {
  m_o[m_o_pins - 1] = m_ox[m_o_pins - 1];
  m_o[m_o_pins - 2] = m_ox[m_o_pins - 2];
}
void op_tw() {
  for (int i = 0; i < m_o_pins; i++)
    m_o[i] = m_ox[i];
}
void op_pdtw() {
  m_ox[m_o_pins - 2] = m_ox[m_o_pins - 1];
  m_ox[m_o_pins - 1] = get_digit();
}
void op_dtw() {
  shift_w();
  m_ox[m_o_pins - 1] = get_digit();
}
void op_wr() {
  shift_w();
  m_ox[m_o_pins - 1] = m_acc & 7;
}
void op_ws() {
  shift_w();
  m_ox[m_o_pins - 1] = m_acc | 8;
}
void op_kta() {
  //  printf("kta ");
  update_segments_state();
  m_acc = readK(m_r_out & 0xf) & 0xf;
}
void op_exksa() {}
void op_exkfa() {}
void op_idiv() { m_div &= 0x3f; }
void op_rmf() {
  m_mx = 0;
  m_acc = 0;
}
void op_smf() { m_mx = 1; }
void op_comcn() { m_cn ^= 1; }
void op_tal() {
  update_segments_state();
  m_skip = (m_read_ba() != 0);
}
void op_tb() {
  update_segments_state();
  m_skip = (m_read_b() != 0);
}

void op_lbl() {
  m_bl = m_param & 0xf;
  m_bm = (m_param & m_datamask) >> 4;
}
void op_exbla() {
  u8 a = m_acc;
  m_acc = m_bl;
  m_bl = a;
}

void op_lda() {
  m_acc = ram_r();
  m_bm ^= (m_op & 3);
}
void op_lax() {
  if ((m_op & ~0xf) != (m_prev_op & ~0xf))
    m_acc = m_op & 0xf;
}
void op_atr() { m_r = m_acc; }
void op_add() { m_acc = (m_acc + ram_r()) & 0xf; }
void op_add11() {
  m_acc += ram_r() + m_c;
  m_c = m_acc >> 4 & 1;
  m_skip = (m_c == 1);
  m_acc &= 0xf;
}
void op_adx() {
  m_acc += (m_op & 0xf);
  m_skip = ((m_op & 0xf) != 10 && (m_acc & 0x10) != 0);
  m_acc &= 0xf;
}
void op_coma() { m_acc ^= 0xf; }
void op_rc() { m_c = 0; }
void op_sc() { m_c = 1; }
void op_tc() { m_skip = !m_c; }
void op_tam() { m_skip = (m_acc == ram_r()); }
void op_tmi() { m_skip = ((ram_r() & bitmask(m_op)) != 0); }
void op_ta0() { m_skip = !m_acc; }
void op_tabl() { m_skip = (m_acc == m_bl); }
void op_tis() {
  m_skip = !m_1s;
  m_1s = false;
}
void op_rm() { ram_w(ram_r() & ~bitmask(m_op)); }
void op_sm() { ram_w(ram_r() | bitmask(m_op)); }
void op_skip() {}
void op_cend() { m_halt = true; }
void op_dta() { m_acc = m_div >> 11 & 0xf; }
void op_illegal() {
  printf("unknown opcode $%02X at $%04X\n", m_op, m_prev_pc);
}
