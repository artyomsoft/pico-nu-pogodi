#ifndef _CORE_H_
#define _CORE_H_
#include "../device/def.h"

extern u16 m_op, m_prev_op;
extern u8 m_param;
extern u16 m_a;
extern u8 m_acc;
extern u8 m_bl;
extern u8 m_bm;
extern u8 m_c;
extern bool m_skip;
extern bool m_k_active;
extern bool m_halt;
extern int m_clk_div;
extern u16 m_div;
extern bool m_1s;
extern u8 m_l;
extern bool m_bc;
extern u8 m_ox[9];
extern u8 m_o[9];
extern u8 m_ox_state[9];
extern u8 m_o_state[9];
extern u8 m_mx;
extern u8 trs_field;

extern int m_datamask;
extern int m_prgmask;
extern u8 ram[128];

extern u16 m_pc, m_prev_pc;
extern u16 m_op, m_prev_op;
extern u8 m_param;
extern int m_icount;
extern u8 m_acc;
extern u8 m_bl;
extern u8 m_bm;
extern u8 m_c;
extern bool m_skip;
extern u8 m_r, m_r_out;
extern bool m_k_active;
extern bool m_halt;
extern int m_clk_div;
extern u16 m_div;
extern bool m_1s;
extern u8 m_bp;
extern bool m_bc;
extern int m_o_pins;
extern u8 m_ox[9];
extern u8 m_o[9];
extern u8 m_ox_state[9];
extern u8 m_o_state[9];
extern u8 m_cn;
extern u8 m_mx;
extern u8 m_cb;
extern bool m_rsub;

void execute_run();
void get_opcode_param();
void div_timer(int nb_inst);
void update_segments_state();
void reset_vector();
void wakeup_vector();
bool wake_me_up();

void shift_w();
u8 get_digit();
void set_su(u8 su);
u8 get_su();
int get_trs_field();

void do_branch(u8 pu, u8 pm, u8 pl);

un8 read_byte_program(un16 rom_address);
void increment_pc();

extern u8 ram_r();
extern void ram_w(u8 data);

extern u8 m_read_ba();
extern u8 m_read_b();

un8 readb(un8 ram_address);
void writeb(un8 ram_address, u8 ram_data);

extern void m_write_r(un8);

u8 bitmask(u16 param);

void device_start();
void device_reset();
void device_run();

#endif
