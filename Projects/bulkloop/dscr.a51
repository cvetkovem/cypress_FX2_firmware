; Copyright (C) 2009 Ubixum, Inc. 
;
; This library is free software; you can redistribute it and/or
; modify it under the terms of the GNU Lesser General Public
; License as published by the Free Software Foundation; either
; version 2.1 of the License, or (at your option) any later version.
; 
; This library is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
; Lesser General Public License for more details.
; 
; You should have received a copy of the GNU Lesser General Public
; License along with this library; if not, write to the Free Software
; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

; this is a the default 
; full speed and high speed 
; descriptors found in the TRM
; change however you want but leave 
; the descriptor pointers so the setupdat.c file works right
 

.module DEV_DSCR 

; descriptor types
; same as setupdat.h
DSCR_DEVICE_TYPE       = 1
DSCR_CONFIG_TYPE       = 2
DSCR_STRING_TYPE       = 3
DSCR_INTERFACE_TYPE    = 4
DSCR_ENDPOINT_TYPE     = 5
DSCR_DEVQUAL_TYPE      = 6

; for the repeating interfaces
DSCR_INTERFACE_LEN     = 9
DSCR_ENDPOINT_LEN      = 7

; endpoint types
ENDPOINT_TYPE_CONTROL  = 0
ENDPOINT_TYPE_ISO      = 1
ENDPOINT_TYPE_BULK     = 2
ENDPOINT_TYPE_INT      = 3

    .globl	_dev_dscr, _dev_qual_dscr, _highspd_dscr, _fullspd_dscr, _dev_strings, _dev_strings_end
; These need to be in code memory.  If
; they aren't you'll have to manully copy them somewhere
; in code memory otherwise SUDPTRH:L don't work right
    .area	DSCR_AREA	(CODE)

; -----------------------------------------------------------------------------
; Device descriptor
; -----------------------------------------------------------------------------
_dev_dscr:
	.db	dev_dscr_end-_dev_dscr    ; len
	.db	DSCR_DEVICE_TYPE          ; type
	.dw	0x0002      ; USB 2.0
	.db	0xff        ; class (vendor specific)
	.db	0xff        ; subclass (vendor specific)
	.db	0xff        ; protocol (vendor specific)
	.db	64          ; packet size (ep0)
	.dw	0xB404      ; vendor id 
	.dw	0x0410      ; product id
	.dw	0x0100      ; version id
	.db	1           ; manufacturure str idx
	.db	2           ; product str idx	
	.db	0           ; serial str idx 
	.db	1           ; n configurations
dev_dscr_end:

; -----------------------------------------------------------------------------
; Device qualifier (for "other device speed")
; -----------------------------------------------------------------------------
_dev_qual_dscr:
	.db	dev_qualdscr_end - _dev_qual_dscr
	.db	DSCR_DEVQUAL_TYPE
	.dw	0x0002      ; USB 2.0
	.db	0xff        ; Class (vendor specific)
	.db	0xff        ; Subclass (vendor specific)
	.db	0xff        ; Protocol (vendor specific)
	.db	64          ; Max. EP0 packet size
	.db	1           ; Number of configurations
	.db	0           ; Extra reserved byte
dev_qualdscr_end:

; -----------------------------------------------------------------------------
; High-Speed configuration descriptor
; -----------------------------------------------------------------------------
_highspd_dscr:
	.db	highspd_dscr_end - _highspd_dscr
	.db	DSCR_CONFIG_TYPE
	; Total length of the configuration (1st line LSB, 2nd line MSB)
	.db	(highspd_dscr_realend - _highspd_dscr) % 256
	.db	(highspd_dscr_realend - _highspd_dscr) / 256
	.db	1           ; Number of interfaces
	.db	1           ; Configuration number
	.db	0           ; Configuration string (none)
	.db	0x80        ; Attributes (bus powered, no wakeup)
	.db	0x32        ; Max. power (100mA)
highspd_dscr_end:


; all the interfaces next 
; NOTE the default TRM actually has more alt interfaces
; but you can add them back in if you need them.
; here, we just use the default alt setting 1 from the trm
	
  ; Interfaces (only one in our case)
	.db	DSCR_INTERFACE_LEN
	.db	DSCR_INTERFACE_TYPE
	.db	0           ; Interface index
	.db	0           ; Alternate setting index
	.db	1           ; Number of endpoints
	.db	0xff        ; Class (vendor specific)
	.db	0xff        ; Subclass (vendor specific)
	.db	0xff        ; Protocol (vendor specific)
	.db	0           ; String index (none)

  ; Endpoint 2 (IN)
	.db	DSCR_ENDPOINT_LEN
	.db	DSCR_ENDPOINT_TYPE
	.db	0x82        ; EP number (2), direction (IN)
	.db	ENDPOINT_TYPE_BULK	; Endpoint type (bulk)
	.db	0x00        ; Max. packet size, LSB (512 bytes)
	.db	0x02        ; Max. packet size, MSB (512 bytes)
	.db	0x00        ; Polling interval

highspd_dscr_realend:

	.even

; -----------------------------------------------------------------------------
; Full-Speed configuration descriptor
; -----------------------------------------------------------------------------
_fullspd_dscr:
	.db	fullspd_dscr_end - _fullspd_dscr
	.db	DSCR_CONFIG_TYPE
	; Total length of the configuration (1st line LSB, 2nd line MSB)
	.db	(fullspd_dscr_realend - _fullspd_dscr) % 256
	.db	(fullspd_dscr_realend - _fullspd_dscr) / 256
	.db	1           ; Number of interfaces
	.db	1           ; Configuration number
	.db	0           ; Configuration string (none)
	.db	0x80        ; Attributes (bus powered, no wakeup)
	.db	0x32        ; Max. power (100mA)
fullspd_dscr_end:

; all the interfaces next 
; NOTE the default TRM actually has more alt interfaces
; but you can add them back in if you need them.
; here, we just use the default alt setting 1 from the trm
	
  ; Interfaces (only one in our case)
	.db	DSCR_INTERFACE_LEN
	.db	DSCR_INTERFACE_TYPE
	.db	0           ; Interface index
	.db	0           ; Alternate setting index
	.db	1           ; Number of endpoints
	.db	0xff        ; Class (vendor specific)
	.db	0xff        ; Subclass (vendor specific)
	.db	0xff        ; Protocol (vendor specific)
	.db	0           ; String index (none)

	; Endpoint 2 (IN)
	.db	DSCR_ENDPOINT_LEN
	.db	DSCR_ENDPOINT_TYPE
	.db	0x82        ; EP number (2), direction (IN)
	.db	ENDPOINT_TYPE_BULK  ; Endpoint type (bulk)
	.db	0x40        ; Max. packet size, LSB (64 bytes)
	.db	0x00        ; Max. packet size, MSB (64 bytes)
	.db	0x00        ; Polling interval

fullspd_dscr_realend:

	.even

; -----------------------------------------------------------------------------
; Strings
; -----------------------------------------------------------------------------

_dev_strings:

; See http://www.usb.org/developers/docs/USB_LANGIDs.pdf for the full list.
_string0:
	.db	string0end - _string0
	.db	DSCR_STRING_TYPE
	.db	0x09, 0x04  ; Language code 0x0409 (English, US)
string0end:

_string1:
	.db	string1end - _string1
	.db	DSCR_STRING_TYPE
	.ascii	'C'
	.db	0
	.ascii	'y'
	.db	0
	.ascii	'p'
	.db	0
	.ascii	'r'
	.db	0
  .ascii	'e'
	.db	0
  .ascii	's'
	.db	0
  .ascii	's'
	.db	0
string1end:

_string2:
	.db	string2end - _string2
	.db	DSCR_STRING_TYPE
	.ascii	'C'
	.db	0
	.ascii	'Y'
	.db	0
	.ascii	'-'
	.db	0
	.ascii	'S'
	.db	0
  .ascii	't'
	.db	0
  .ascii	'r'
	.db	0
  .ascii	'e'
	.db	0
	.ascii	'a'
	.db	0
	.ascii	'm'
	.db	0
string2end:
    
_dev_strings_end:
    .dw 0x0000    ; just in case someone passes an index higher than the end to the firmware
