/*
 * RF1276.h
 *
 *  Created on: 01/09/2015
 *      Author: cuki
 */

#ifndef SRC_RF1276_H_
#define SRC_RF1276_H_

#include <stdint.h>
#include <serial.h>

#define DATA_SIZE		12
#define COMMAND_SIZE	23

int8_t rssi[14] =
		"0xAF 0xAF 0x00 0x00 0xAF 0x80 0x06 0x02 0x00 0x00 0x95 0x0D 0x0A";

typedef enum cmd_enum {
	cmd_write = 0x01, cmd_read, cmd_standard, cmd_central, cmd_node
} comando_t;

typedef enum baudrate_enum {
	b1200 = 1, b2400, b4800, b9600, b19200, b38400, b57600
} baudrate_t;

typedef enum parity_enum {
	no_parity = 0, odd_parity, even_parity
} parity_t;

typedef enum rf_factor_enum {
	rf128 = 7, rf256, rf512, rf1024, rf2048, rf4096
} rf_factor_t;

typedef enum mode_enum {
	standard_mode = 0, low_power_mode, sleep_mode
} mode_enum_t;

typedef enum rf_bw_enum {
	bw625k = 6, bw125k, bw256k, bw512k
} rf_bw_t;

typedef enum rf_power_enum {
	p4dBm = 1, p7dBm, p10dBm, p13dBm, p14dBm, p17dBm, p20dBm
} rf_power_t;

typedef enum attr_enum {
	attr_baudrate,
	attr_parity,
	attr_freq,
	attr_rf_factor,
	attr_mode,
	attr_rf_bw,
	attr_id,
	attr_net_id,
	attr_rf_power
} attr_t;

typedef struct data_str {
	uint8_t baudrate;
	uint8_t parity;
	uint8_t frequency_HL;
	uint8_t frequency_LH;
	uint8_t frequency_LL;
	uint8_t rf_factor;
	uint8_t mode;
	uint8_t rf_bw;
	uint8_t id_hi;
	uint8_t id_lo;
	uint8_t net_id;
	uint8_t rf_power;
} data_str_t;

typedef struct command_struct {
	uint8_t sync_word_hi;
	uint8_t sync_word_lo;
	uint8_t id_code_hi;
	uint8_t id_code_lo;
	uint8_t header;
	uint8_t command_xx;
	uint8_t command_yy;
	uint8_t length;
	uint8_t data[DATA_SIZE];
	uint8_t cs;
	uint8_t end_code_hi;
	uint8_t end_code_lo;
} command_struct_t;

typedef union data_un {
	data_str_t data_str;
	uint8_t raw[sizeof(data_str_t)];
} data_t;

typedef union command_struct_un {
	command_struct_t str;
	uint8_t raw[sizeof(command_struct_t)];
} command_t;

extern uint8_t crc(uint8_t *str, int size);
extern int make_read_request(uint8_t *request);
extern int get_data(uint8_t *request, data_t *data);
extern int make_write_request(data_t data, uint8_t *request);
extern int get_attribute(uint8_t *request, attr_t atributo);
extern int rtoc(uint8_t *request, command_t *command);
extern int clear_response(uint8_t *response);
extern float parse_frequencie(uint8_t *response);
extern int get_rssi(int fd);

#endif /* SRC_RF1276_H_ */
