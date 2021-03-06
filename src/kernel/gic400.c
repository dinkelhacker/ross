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


#define ALL_OTHER_CORES (1 << 24)
#define ONLY_IF_GRP1	(1 << 15)

int gic400_init(void *interrupt_controller_base)
{
	int error = OK;

	gic400.gicd = (gic400_gicd_t*)(interrupt_controller_base + 0x1000);
	gic400.gicc = (gic400_gicc_t*)(interrupt_controller_base + 0x2000);

	// disable the controller 
	gicd_ctrl(GIC400_CTL_DISABLE);
	gic400.gicd->icfg[6] = 0x00000000;
	gicd_ctrl(GIC400_ENABLE_GRP1);

	return error;
}

void gic400_enir(uint32_t irid, uint32_t group, uint32_t core)
{
	gicd_enableir(irid);
	gicd_groupir(irid, group);
	gicd_irtarget(irid, core);
}

void gic400_ensgi(uint32_t irid, uint32_t group)
{
	gicd_enableir(irid);
	gicd_groupir(irid, group);
}

void gic400_sched_timer()
{
	//volatile uint32_t irid = (1<<24) | (1<<15) | (0xf << 16) | 4;
	gic400.gicd->sgi = ALL_OTHER_CORES | ONLY_IF_GRP1 | IRID_TIMER_SGI;
}

int gic400_enable_cpuif()
{
	gic400.gicc->pm = 0xFF;
	return gicc_ctrl(GIC400_ENABLE_GRP1);
}

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
