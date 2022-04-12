.section ".text"

.globl mmio_write32
/* void mmio_write32(uint64_t reg, uint32_t val) */
mmio_write32:
	str	w1, [x0]
	ret

.globl mmio_read32
/* uint32_t mmio_read32(uint64_t reg) */
mmio_read32:
	ldr	w0, [x0]
	ret
