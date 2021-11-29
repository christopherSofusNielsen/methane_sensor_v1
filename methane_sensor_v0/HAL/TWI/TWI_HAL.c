/*
 * TWI_HAL.c
 *
 * Created: 14-10-2021 08:43:55
 *  Author: Christopher S. Nielsen
 */ 

#include "TWI_HAL.h"
#include "../../util/bit_operators.h"
#include <xc.h>
#define twi_read_status(Reg) (Reg & 0xF8)
#define twi_start_transmission (1<<TWINT)|(1<<TWEN)


#define TWI_CODE_START 0x08
#define TWI_CODE_REP_START 0x10
#define TWI_CODE_ADDR_W_TRANS_ACK_REC 0x18
#define TWI_CODE_ADDR_W_TRANS_NACK_REC 0x20
#define TWI_CODE_ADDR_R_TRANS_ACK_REC 0x40
#define TWI_CODE_ADDR_R_TRANS_NACK_REC 0x48
#define TWI_CODE_DATA_TRANS_ACK_REC 0x28
#define TWI_CODE_DATA_TRANS_NACK_REC 0x30




void TWI_HAL_init(){
	TWBR=TWI_TWBR; //Set bit generator
	clear_byte(TWSR, 0b00000011); //set prescale to 1
	
}

uint8_t TWI_HAL_start(uint8_t addr, TWI_HAL_START_TYPE type){
	uint8_t addr_type=(addr<<1) | type;
	uint8_t status;
	
	TWCR=twi_start_transmission |(1<<TWSTA); //Send start condition
	
	while (!get_bit(TWCR, TWINT)); //Wait to finish job
	status=twi_read_status(TWSR);
	
	if(status != TWI_CODE_START) return status; //Check status is success
	
	TWDR=addr_type; //Set Write addr
	TWCR=twi_start_transmission; //Start transmission of addr
	
	while (!get_bit(TWCR, TWINT)); //Wait to finish job
	status=twi_read_status(TWSR);
	
	if(status == TWI_CODE_ADDR_W_TRANS_ACK_REC || status==TWI_CODE_ADDR_R_TRANS_ACK_REC) return TWI_CODE_SUCCESS; //Success
	
	return status;
}

uint8_t TWI_HAL_repeated_start(uint8_t addr){
	uint8_t addr_type=(addr<<1) | 1;
	uint8_t status;
	
	TWCR=twi_start_transmission |(1<<TWSTA); //Send start condition
	
	while (!get_bit(TWCR, TWINT)); //Wait to finish job
	status=twi_read_status(TWSR);
	
	if(status != TWI_CODE_REP_START) return status; //Check status is success
	
	TWDR=addr_type; //Set Write addr
	TWCR=twi_start_transmission; //Start transmission of addr
	
	while (!get_bit(TWCR, TWINT)); //Wait to finish job
	status=twi_read_status(TWSR);
	
	if(status==TWI_CODE_ADDR_R_TRANS_ACK_REC) return TWI_CODE_SUCCESS;
	
	return status;
}

uint8_t TWI_HAL_write_byte(uint8_t data){
	uint8_t status;
	
	TWDR=data;	//Set data
	TWCR=twi_start_transmission;
	
	while (!get_bit(TWCR, TWINT)); 
	status=twi_read_status(TWSR);
	
	if(status==TWI_CODE_DATA_TRANS_ACK_REC) return TWI_CODE_SUCCESS; //Success
	return status;
}

uint8_t TWI_HAL_read_byte_ack(){
	TWCR=twi_start_transmission|(1<<TWEA);
	while (!get_bit(TWCR, TWINT)); 
	
	return TWDR;
}

uint8_t TWI_HAL_read_byte_nack(){
	TWCR=twi_start_transmission;
	while (!get_bit(TWCR, TWINT));
	
	return TWDR;
}

void TWI_HAL_stop(){
	 TWCR=twi_start_transmission|(1<<TWSTO);
	 while(TWCR&(1<<TWSTO));
}

