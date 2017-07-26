
#ifndef MEMORY_DEFINITIONS_H__
#define MEMORY_DEFINITIONS_H__

#include <stdint.h>

//** Global Variables to access flash memory **//

uint32_t   pg_size;
uint32_t   pg_num;
uint32_t * major_minor_addr;
uint32_t * password_addr;

//********************************************//

/** @brief Function for erasing a page in flash.
 *
 * @param page_address Address of the first word in the page to be erased.
 */
void flash_page_erase(uint32_t * page_address);

/**@brief Function for writing a value into flash memory.
 *
 *
 * @param[in] memory_position  Major value 32 bits.
 * @param[in] value  	   	   Value to save. 16 bits..
 */
void memory_writing(uint32_t * memory_position, uint32_t  value);


#endif  // MEMORY_DEFINITIONS_H__
