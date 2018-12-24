#include "nemu.h"
#include "device/mmio.h"

#define PMEM_SIZE (128 * 1024 * 1024)

#define pmem_rw(addr, type) *(type *)({\
    Assert(addr < PMEM_SIZE, "physical address(0x%08x) is out of bound", addr); \
    guest_to_host(addr); \
    })

uint8_t pmem[PMEM_SIZE];

/* Memory accessing interfaces */

uint32_t paddr_read(paddr_t addr, int len) {
	int map_NO = is_mmio(addr);
	if(map_NO == -1)	//memory
  		return pmem_rw(addr, uint32_t) & (~0u >> ((4 - len) << 3));
  	else {
  		uint32_t data = mmio_read(addr, len, map_NO);
  		//Log("Reading 0x%x from addr 0x%x in divice\n", data, addr);
  		return data;
  	}
}

void paddr_write(paddr_t addr, uint32_t data, int len) {
	int map_NO = is_mmio(addr);
	if(map_NO == -1)	//memory
  		memcpy(guest_to_host(addr), &data, len);
  	else {
  		///Log("Writing data 0x%x to addr 0x%x in divice\n",data, addr);
  		mmio_write(addr, len, data, map_NO);
  	}
  	return;
}

uint32_t vaddr_read(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, uint32_t data, int len) {
  paddr_write(addr, data, len);
}