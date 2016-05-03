/*
 * RF1276.c
 *
 *  Created on: 08/09/2015
 *      Author: cuki
 */

#include "RF1276.h"

#include <string.h>
#include <stdio.h>

uint8_t crc(uint8_t *str, int size) {
	int i;
	int acumulo;

	acumulo = 0;
	for (i = 0; i < size; ++i)
		acumulo += str[i];

	return (uint8_t) acumulo % 256;
}

int make_read_request(uint8_t *request) {

	int cont;
	command_t command;
	uint8_t aux[20];

	command.str.sync_word_hi = 0xAF;
	command.str.sync_word_lo = 0xAF;
	command.str.id_code_hi = 0x00;
	command.str.id_code_lo = 0x00;
	command.str.header = 0xAF;
	command.str.command_xx = 0x80;
	command.str.command_yy = (uint8_t) cmd_read;
	command.str.length = (uint8_t) DATA_SIZE;

	for (cont = 0; cont < DATA_SIZE; ++cont)
		command.str.data[cont] = 0;

	for (cont = 0; cont < 20; ++cont)
		aux[cont] = command.raw[cont];

	command.str.cs = crc(aux, 20);
	command.str.end_code_hi = 0x0D;
	command.str.end_code_lo = 0x0A;

	for (cont = 0; cont < sizeof(command_struct_t); ++cont)
		request[cont] = command.raw[cont];

	return cont;
}

int get_data(uint8_t *request, data_t *data) {

	int cont = -1;
	command_t comando;

	for (cont = 0; cont < sizeof(command_struct_t); ++cont)
		comando.raw[cont] = request[cont];

	for (cont = 0; cont < sizeof(data_str_t); ++cont)
		data->raw[cont] = comando.str.data[cont];

	return cont;
}

int make_write_request(data_t data, uint8_t *request) {

	int cont;
	command_t command;
	uint8_t aux[20];

	command.str.sync_word_hi = 0xAF;
	command.str.sync_word_lo = 0xAF;
	command.str.id_code_hi = 0x00;
	command.str.id_code_lo = 0x00;
	command.str.header = 0xAF;
	command.str.command_xx = 0x80;
	command.str.command_yy = (uint8_t) cmd_write;
	command.str.length = (uint8_t) DATA_SIZE;

	for (cont = 0; cont < sizeof(data_str_t); ++cont)
		command.str.data[cont] = data.raw[cont];

	for (cont = 0; cont < 20; ++cont)
		aux[cont] = command.raw[cont];

	command.str.cs = crc(aux, sizeof(aux));
	command.str.end_code_hi = 0x0D;
	command.str.end_code_lo = 0x0A;

	for (cont = 0; cont < sizeof(command_struct_t); ++cont)
		request[cont] = command.raw[cont];

	return cont;
}

int get_attribute(uint8_t *request, attr_t atributo) {

	int cont, ret = -1;
	command_t comando;
	uint8_t aux;

	for (cont = 0; cont < sizeof(command_struct_t); ++cont)
		comando.raw[cont] = request[cont];

	switch (atributo) {
	case attr_baudrate:
		aux = comando.str.data[0];
		ret = (int) aux;
		break;
	case attr_parity:
		ret = (int) comando.str.data[1];
		break;
	case attr_freq:
		ret = (int) comando.str.data[2];
		ret <<= 8;
		ret |= comando.str.data[3];
		ret <<= 8;
		ret |= comando.str.data[4];
		break;
	case attr_rf_factor:
		ret = (int) comando.str.data[5];
		break;
	case attr_mode:
		ret = (int) comando.str.data[6];
		break;
	case attr_rf_bw:
		ret = (int) comando.str.data[7];
		break;
	case attr_id:
		ret = (int) comando.str.data[8];
		ret <<= 8;
		ret |= comando.str.data[9];
		break;
	case attr_net_id:
		ret = (int) comando.str.data[10];
		break;
	case attr_rf_power:
		ret = (int) comando.str.data[11];
		break;
	}

	return ret;
}

int rtoc(uint8_t *request, command_t *command) {
	int cont;

	for (cont = 0; cont < COMMAND_SIZE; ++cont)
		command->raw[cont] = request[cont];

	return 0;
}

int clear_response(uint8_t *response) {
	int cont;

	for (cont = 0; cont < COMMAND_SIZE; ++cont) {
		response[cont] = 0;
	}

	return 0;
}

float parse_frequencie(uint8_t *response) {
	return (float) get_attribute(response, attr_freq) * 0.000061035;
}

int get_rssi(int fd) {
	int r;
	uint8_t resp[12];

	if (fd <= 0)
		return -1;

	r = make_transaction(fd, rssi, resp, 13, 12);

	if (r != 0)
		return resp[8] - 164;
	else
		return -1;
}
