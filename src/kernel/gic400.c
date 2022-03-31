#include "gic400.h"
#include "error.h"
#include "irids.h"
#include <stdint.h>

typedef struct {
		gic400_gicd_t* gicd;
		gic400_gicc_t* gicc;
} gic400_t;

static gic400_t gic400;

void gicd_enableir(uint32_t irid);
int gicd_groupir(uint32_t irid, uint32_t group);
int gicd_irtarget(uint32_t irid, uint32_t cpuid);
int gicd_ctrl(uint32_t mode);
int gicc_ctrl(uint32_t mode);
int gic_ctrl(uint32_t mode, uint32_t* ctl);

int gic400_init(void *interrupt_controller_base)
{
	int error = OK;

	gic400.gicd = (gic400_gicd_t*)(interrupt_controller_base + 0x1000);
	gic400.gicc = (gic400_gicc_t*)(interrupt_controller_base + 0x2000);

	// disable the controller 
	gicd_ctrl(GIC400_CTL_DISABLE);

	// enable system timer interrupt (line 1)
	// strange offset o.O
	gicd_enableir(97);
	gicd_groupir(97,1);
	gicd_irtarget(97, 0);

	gicd_enableir(IRID_GPIO_BANK0);
	gicd_groupir(IRID_GPIO_BANK0, 1);
	gicd_irtarget(IRID_GPIO_BANK0, 0);

	gic400.gicd->icfg[6] = 0x00000000;

	gicd_ctrl(GIC400_ENABLE_GRP1);
	gic400.gicc->pm = 0xFF;
	gicc_ctrl(GIC400_ENABLE_GRP1);


	return error;
}

//void gicd_icfg(uint32_t irid, uint32_t mode){
//  if(mode > 1)
//    return ERROR;
//
//  gicd400.gicd->icfg[irid/16] = 
//}

void gicd_enableir(uint32_t irid)
{
	gic400.gicd->isenable[irid/32] |= 0x1 << (irid % 32);
}

int gicd_groupir(uint32_t irid, uint32_t group)
{
	if(group>1)
			return ERROR;

	uint32_t groupv = (group == 1)? 0x1:0x0;
	gic400.gicd->igroup[irid/32] |= groupv << (irid % 32);

	return OK;
}

int gicd_irtarget(uint32_t irid, uint32_t cpuid)
{
	if(cpuid > 3)
			return ERROR;
	
	gic400.gicd->istargets[irid/4] |= (0x1 << cpuid) << ((irid % 4) * 8);

	return OK;
}

int gicd_ctrl(uint32_t mode)
{
	return gic_ctrl(mode, (uint32_t *) &(gic400.gicd->ctl));
}

int gicc_ctrl(uint32_t mode)
{
	return gic_ctrl(mode, (uint32_t *) &(gic400.gicc->ctl));
}

int gic_ctrl(uint32_t mode, uint32_t* ctl)
{
	uint32_t tmp = *ctl;
	*ctl |= mode;

	// check if we had the right permissions to set the ctl bits
	if((*ctl | mode) == (tmp | mode))
		return ERROR;
	
	return OK;
}

// acknowledge pending irq
uint32_t gicc_apirq()
{
	return gic400.gicc->ia;
}

void gicc_eoi(uint32_t irid)
{
	gic400.gicc->eoi = irid;
}
