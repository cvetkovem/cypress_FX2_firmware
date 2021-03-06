#include <fx2regs.h>
#include <fx2macros.h>
#include <delay.h>
#include <setupdat.h>
#include <gpif.h>
#include <fx2lafw.h>
#include <command.h>
#include <gpif-acquisition.h>

/* ... */
volatile __bit got_sud;
BYTE vendor_command;

static void setup_endpoints(void) {
    /* Setup EP2 (IN). */
    EP2CFG = (uint8_t)(1 << 7) |       /* EP is valid/activated */
         (uint8_t)(1 << 6) |       /* EP direction: IN */
         (uint8_t)(1 << 5) | (uint8_t)(0 << 4) |    /* EP Type: bulk */
         (uint8_t)(1 << 3) |       /* EP buffer size: 1024 */
         (uint8_t)(0 << 2) |       /* Reserved. */
         (uint8_t)(0 << 1) | (uint8_t)(0 << 0);     /* EP buffering: quad buffering */
    SYNCDELAY();

    /* Disable all other EPs (EP1, EP4, EP6, and EP8). */
    EP1INCFG &= ~bmVALID;
    SYNCDELAY();
    EP1OUTCFG &= ~bmVALID;
    SYNCDELAY();
    EP4CFG &= ~bmVALID;
    SYNCDELAY();
    EP6CFG &= ~bmVALID;
    SYNCDELAY();
    EP8CFG &= ~bmVALID;
    SYNCDELAY();

    /* EP2: Reset the FIFOs. */
    /* Note: RESETFIFO() gets the EP number WITHOUT bit 7 set/cleared. */
    //RESETFIFO(0x02);
  FIFORESET=0x80;
  SYNCDELAY();
  FIFORESET=0x02;
  SYNCDELAY();
  FIFORESET=0x00;
  SYNCDELAY();
                       
    /* EP2: Enable AUTOIN mode. Set FIFO width to 8bits. */
    EP2FIFOCFG = bmAUTOIN;
    SYNCDELAY();

    /* EP2: Auto-commit 512 (0x200) byte packets (due to AUTOIN = 1). */
    EP2AUTOINLENH = 0x02;
    SYNCDELAY();
    EP2AUTOINLENL = 0x00;
    SYNCDELAY();

    /* EP2: Set the GPIF flag to 'full'. */
    EP2GPIFFLGSEL = (1 << 1) | (0 << 1);
    SYNCDELAY();
}

static void send_fw_version(void) {
    /* Populate the buffer. */
    struct version_info *const vi = (struct version_info *)EP0BUF;
    vi->major = FX2LAFW_VERSION_MAJOR;
    vi->minor = FX2LAFW_VERSION_MINOR;

    /* Send the message. */
    EP0BCH = 0;
    EP0BCL = sizeof(struct version_info);
}

static void send_revid_version(void) {
    uint8_t *p;

    /* Populate the buffer. */
    p = (uint8_t *)EP0BUF;
    *p = REVID;

    /* Send the message. */
    EP0BCH = 0;
    EP0BCL = 1;
}

static void send_ok(void) {
    uint8_t *p;
    /* Populate the buffer. */
    p = (uint8_t *)EP0BUF;
    *p = 0xAA;

    /* Send the message. */
    EP0BCH = 0;
    EP0BCL = 1;
}

void byte2fpga(unsigned char cmd4fpga) {
    /* PA[0] - clock, PA[1] - set ant flag, PA[2,3,4] - mode, PA[5,6,7] - number ant */
    IOA = cmd4fpga;
    IOA |= 0x01;      // clock up
    SYNCDELAY();
    IOA &= 0xFE;      // clock down
}

BOOL handle_vendorcommand(BYTE cmd) {
    /* Protocol implementation */
    switch (cmd) {
    case CMD_START:
        vendor_command = cmd;
        EP0BCL = 0;
        return TRUE;
        break;
        
    case CMD_GET_FW_VERSION:
        send_fw_version();
        return TRUE;
        break;
        
    case CMD_GET_REVID_VERSION:
        send_revid_version();
        return TRUE;
        break;
        
    case CMD_SET_OUT_SOURCE_RX1:
        byte2fpga(0b00000000); // set mode 0 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_OUT_SOURCE_RX2:
        byte2fpga(0b00000100); // set mode 1 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_OUT_CHIRPS:
        byte2fpga(0b00001000); // set mode 2 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_OUT_AUTO_0:
        byte2fpga(0b00001100); // set mode 3 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_ANT_ALL_OFF:
        byte2fpga(0b00000010); // set ant off for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_ANT_A1:
        byte2fpga(0b00100010); // set ant 1 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_ANT_A2:
        byte2fpga(0b01000010); // set ant 2 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_ANT_A3:
        byte2fpga(0b01100010); // set ant 3 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_ANT_A4:
        byte2fpga(0b10000010); // set ant 4 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_ANT_A5:
        byte2fpga(0b10100010); // set ant 5 for fpga;
        send_ok();
        return TRUE;
        break;
        
    case CMD_SET_ANT_A6:
        byte2fpga(0b11000010); // set ant 6 for fpga;
        send_ok();
        return TRUE;
        break;
    }

    return FALSE;
}
	
BOOL handle_get_descriptor() {
 return FALSE;
}

BOOL handle_get_interface(BYTE ifc, BYTE *alt_ifc) {
    /* We only support interface 0, alternate interface 0. */
    if (ifc != 0)
        return FALSE;

    *alt_ifc = 0;
    return TRUE;
}

BOOL handle_set_interface(BYTE ifc, BYTE alt_ifc) {
    /* We only support interface 0, alternate interface 0. */
    if (ifc != 0 || alt_ifc != 0)
        return FALSE;

    /* Perform procedure from TRM, section 2.3.7: */

    /* (1) TODO. */

    /* (2) Reset data toggles of the EPs in the interface. */
    /* Note: RESETTOGGLE() gets the EP number WITH bit 7 set/cleared. */
    //RESETTOGGLE(0x82);
    TOGCTL = (0x82&0x0F) + ((0x82&0x80)>>3); TOGCTL |= bmRESETTOGGLE;

    /* (3) Restore EPs to their default conditions. */
    /* Note: RESETFIFO() gets the EP number WITHOUT bit 7 set/cleared. */
    //RESETFIFO(0x02);
    FIFORESET=0x80;
    SYNCDELAY();
    FIFORESET=0x02;
    SYNCDELAY();
    FIFORESET=0x00;
    SYNCDELAY();
    /* TODO */

    /* (4) Clear the HSNAK bit. Not needed, fx2lib does this. */

    return TRUE;
}

BYTE handle_get_configuration(void) {
    /* We only support configuration 1. */
    return 1;
}

BOOL handle_set_configuration(BYTE cfg) {
    /* We only support configuration 1. */
    return (cfg == 1) ? TRUE : FALSE;
}

void sudav_isr(void) __interrupt SUDAV_ISR {
    got_sud = TRUE;
    CLEAR_SUDAV();
}

void sof_isr(void) __interrupt SOF_ISR __using 1 {
    CLEAR_SOF();
}

void usbreset_isr(void) __interrupt USBRESET_ISR {
    handle_hispeed(FALSE);
    CLEAR_USBRESET();
}

void hispeed_isr(void) __interrupt HISPEED_ISR {
    handle_hispeed(TRUE);
    CLEAR_HISPEED();
}

void portA_fpga_init(void) {
    OEA = 0xFF; // make all pins of Port A as output
    IOA = 0x00; // all down
}

void fx2lafw_init(void) {
    /* Set DYN_OUT and ENH_PKT bits, as recommended by the TRM. */
    REVCTL = bmNOAUTOARM | bmSKIPCOMMIT;

    got_sud = FALSE;
    vendor_command = 0;

    /* Renumerate. */
    RENUMERATE_UNCOND();

    SETCPUFREQ(CLK_48M);

    USE_USB_INTS();

    /* TODO: Does the order of the following lines matter? */
    ENABLE_SUDAV();
    ENABLE_SOF();
    ENABLE_HISPEED();
    ENABLE_USBRESET();

    /* Global (8051) interrupt enable. */
    EA = 1;

    /* Setup the endpoints. */
    setup_endpoints();

    /* Put the FX2 into GPIF master mode and setup the GPIF. */
    gpif_init_la();
}

void fx2lafw_poll(void) {
    if (got_sud) {
        handle_setupdata();
        got_sud = FALSE;
    }

    if (vendor_command) {
        switch (vendor_command) {
        case CMD_START:
            if ((EP0CS & bmEPBUSY) != 0)
                break;

            if (EP0BCL == sizeof(struct cmd_start_acquisition)) {
                gpif_acquisition_start(
                 (const struct cmd_start_acquisition *)EP0BUF);
            }

            /* Acknowledge the vendor command. */
            vendor_command = 0;
            break;
        default:
            /* Unimplemented command. */
            vendor_command = 0;
            break;
        }
    }

    gpif_poll();
}

void main(void) {
    fx2lafw_init();
    portA_fpga_init();
    while (1)
        fx2lafw_poll();
}
