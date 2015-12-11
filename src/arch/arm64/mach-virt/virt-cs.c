/*
 * virt-cs.c
 *
 * Copyright(c) 2007-2015 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot.h>
#include <arm64.h>
#include <clocksource/clocksource.h>

u64_t arm64_timer_frequecy(void)
{
	u64_t rate = arm64_read_sysreg(cntfrq_el0);
	return (rate != 0) ? rate : 1000000;
}

void arm64_timer_compare(u64_t cnt)
{
	arm64_write_sysreg(cntp_cval_el0, cnt);
}

u64_t arm64_timer_read(void)
{
	return arm64_read_sysreg(cntpct_el0);
}

void arm64_timer_enable(void)
{
	u64_t ctrl;
	ctrl = arm64_read_sysreg(cntp_ctl_el0);
	ctrl |= (1 << 0);
	ctrl &= ~(1 << 1);
	arm64_write_sysreg(cntp_ctl_el0, ctrl);
}

void arm64_timer_disable(void)
{
	u64_t ctrl;
	ctrl = arm64_read_sysreg(cntp_ctl_el0);
	ctrl &= ~(1 << 0);
	arm64_write_sysreg(cntp_ctl_el0, ctrl);
}

static bool_t virt_cs_init(struct clocksource_t * cs)
{
	u64_t rate = arm64_timer_frequecy();
	cs->mult = clocksource_hz2mult(rate, cs->shift);
	arm64_timer_compare(~0ULL);
	arm64_timer_compare(5000);
	arm64_timer_enable();
	return TRUE;
}

static u64_t virt_cs_read(struct clocksource_t * cs)
{
	u64_t val = arm64_timer_read();
	return (u64_t)(val);
}

static struct clocksource_t virt_cs = {
	.name	= "virt-cs",
	.mask	= CLOCKSOURCE_MASK(64),
	.init	= virt_cs_init,
	.read	= virt_cs_read,
};

static __init void virt_clocksource_init(void)
{
	if(register_clocksource(&virt_cs))
		LOG("Register clocksource");
	else
		LOG("Failed to register clocksource");
}
core_initcall(virt_clocksource_init);