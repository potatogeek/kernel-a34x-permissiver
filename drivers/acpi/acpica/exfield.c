// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
/******************************************************************************
 *
<<<<<<< HEAD
 * Module Name: exfield - ACPI AML (p-code) execution - field manipulation
 *
 * Copyright (C) 2000 - 2018, Intel Corp.
=======
 * Module Name: exfield - AML execution - field_unit read/write
 *
 * Copyright (C) 2000 - 2021, Intel Corp.
>>>>>>> upstream/android-13
 *
 *****************************************************************************/

#include <acpi/acpi.h>
#include "accommon.h"
#include "acdispat.h"
#include "acinterp.h"
#include "amlcode.h"

#define _COMPONENT          ACPI_EXECUTER
ACPI_MODULE_NAME("exfield")

<<<<<<< HEAD
/* Local prototypes */
static u32
acpi_ex_get_serial_access_length(u32 accessor_type, u32 access_length);

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_get_serial_access_length
 *
 * PARAMETERS:  accessor_type   - The type of the protocol indicated by region
 *                                field access attributes
 *              access_length   - The access length of the region field
 *
 * RETURN:      Decoded access length
=======
/*
 * This table maps the various Attrib protocols to the byte transfer
 * length. Used for the generic serial bus.
 */
#define ACPI_INVALID_PROTOCOL_ID        0x80
#define ACPI_MAX_PROTOCOL_ID            0x0F
static const u8 acpi_protocol_lengths[] = {
	ACPI_INVALID_PROTOCOL_ID,	/* 0 - reserved */
	ACPI_INVALID_PROTOCOL_ID,	/* 1 - reserved */
	0x00,			/* 2 - ATTRIB_QUICK */
	ACPI_INVALID_PROTOCOL_ID,	/* 3 - reserved */
	0x01,			/* 4 - ATTRIB_SEND_RECEIVE */
	ACPI_INVALID_PROTOCOL_ID,	/* 5 - reserved */
	0x01,			/* 6 - ATTRIB_BYTE */
	ACPI_INVALID_PROTOCOL_ID,	/* 7 - reserved */
	0x02,			/* 8 - ATTRIB_WORD */
	ACPI_INVALID_PROTOCOL_ID,	/* 9 - reserved */
	0xFF,			/* A - ATTRIB_BLOCK  */
	0xFF,			/* B - ATTRIB_BYTES */
	0x02,			/* C - ATTRIB_PROCESS_CALL */
	0xFF,			/* D - ATTRIB_BLOCK_PROCESS_CALL */
	0xFF,			/* E - ATTRIB_RAW_BYTES */
	0xFF			/* F - ATTRIB_RAW_PROCESS_BYTES */
};

#define PCC_MASTER_SUBSPACE     3

/*
 * The following macros determine a given offset is a COMD field.
 * According to the specification, generic subspaces (types 0-2) contains a
 * 2-byte COMD field at offset 4 and master subspaces (type 3) contains a 4-byte
 * COMD field starting at offset 12.
 */
#define GENERIC_SUBSPACE_COMMAND(a)     (4 == a || a == 5)
#define MASTER_SUBSPACE_COMMAND(a)      (12 <= a && a <= 15)

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_get_protocol_buffer_length
 *
 * PARAMETERS:  protocol_id     - The type of the protocol indicated by region
 *                                field access attributes
 *              return_length   - Where the protocol byte transfer length is
 *                                returned
 *
 * RETURN:      Status and decoded byte transfer length
>>>>>>> upstream/android-13
 *
 * DESCRIPTION: This routine returns the length of the generic_serial_bus
 *              protocol bytes
 *
 ******************************************************************************/

<<<<<<< HEAD
static u32
acpi_ex_get_serial_access_length(u32 accessor_type, u32 access_length)
{
	u32 length;

	switch (accessor_type) {
	case AML_FIELD_ATTRIB_QUICK:

		length = 0;
		break;

	case AML_FIELD_ATTRIB_SEND_RCV:
	case AML_FIELD_ATTRIB_BYTE:

		length = 1;
		break;

	case AML_FIELD_ATTRIB_WORD:
	case AML_FIELD_ATTRIB_WORD_CALL:

		length = 2;
		break;

	case AML_FIELD_ATTRIB_MULTIBYTE:
	case AML_FIELD_ATTRIB_RAW_BYTES:
	case AML_FIELD_ATTRIB_RAW_PROCESS:

		length = access_length;
		break;

	case AML_FIELD_ATTRIB_BLOCK:
	case AML_FIELD_ATTRIB_BLOCK_CALL:
	default:

		length = ACPI_GSBUS_BUFFER_SIZE - 2;
		break;
	}

	return (length);
=======
acpi_status
acpi_ex_get_protocol_buffer_length(u32 protocol_id, u32 *return_length)
{

	if ((protocol_id > ACPI_MAX_PROTOCOL_ID) ||
	    (acpi_protocol_lengths[protocol_id] == ACPI_INVALID_PROTOCOL_ID)) {
		ACPI_ERROR((AE_INFO,
			    "Invalid Field/AccessAs protocol ID: 0x%4.4X",
			    protocol_id));

		return (AE_AML_PROTOCOL);
	}

	*return_length = acpi_protocol_lengths[protocol_id];
	return (AE_OK);
>>>>>>> upstream/android-13
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_read_data_from_field
 *
 * PARAMETERS:  walk_state          - Current execution state
 *              obj_desc            - The named field
 *              ret_buffer_desc     - Where the return data object is stored
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Read from a named field. Returns either an Integer or a
<<<<<<< HEAD
 *              Buffer, depending on the size of the field.
=======
 *              Buffer, depending on the size of the field and whether if a
 *              field is created by the create_field() operator.
>>>>>>> upstream/android-13
 *
 ******************************************************************************/

acpi_status
acpi_ex_read_data_from_field(struct acpi_walk_state *walk_state,
			     union acpi_operand_object *obj_desc,
			     union acpi_operand_object **ret_buffer_desc)
{
	acpi_status status;
	union acpi_operand_object *buffer_desc;
<<<<<<< HEAD
	acpi_size length;
	void *buffer;
	u32 function;
	u16 accessor_type;
=======
	void *buffer;
	u32 buffer_length;
>>>>>>> upstream/android-13

	ACPI_FUNCTION_TRACE_PTR(ex_read_data_from_field, obj_desc);

	/* Parameter validation */

	if (!obj_desc) {
		return_ACPI_STATUS(AE_AML_NO_OPERAND);
	}
	if (!ret_buffer_desc) {
		return_ACPI_STATUS(AE_BAD_PARAMETER);
	}

	if (obj_desc->common.type == ACPI_TYPE_BUFFER_FIELD) {
		/*
		 * If the buffer_field arguments have not been previously evaluated,
		 * evaluate them now and save the results.
		 */
		if (!(obj_desc->common.flags & AOPOBJ_DATA_VALID)) {
			status = acpi_ds_get_buffer_field_arguments(obj_desc);
			if (ACPI_FAILURE(status)) {
				return_ACPI_STATUS(status);
			}
		}
	} else if ((obj_desc->common.type == ACPI_TYPE_LOCAL_REGION_FIELD) &&
		   (obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_SMBUS
		    || obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_GSBUS
		    || obj_desc->field.region_obj->region.space_id ==
<<<<<<< HEAD
		    ACPI_ADR_SPACE_IPMI)) {
		/*
		 * This is an SMBus, GSBus or IPMI read. We must create a buffer to
		 * hold the data and then directly access the region handler.
		 *
		 * Note: SMBus and GSBus protocol value is passed in upper 16-bits
		 * of Function
		 */
		if (obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_SMBUS) {
			length = ACPI_SMBUS_BUFFER_SIZE;
			function =
			    ACPI_READ | (obj_desc->field.attribute << 16);
		} else if (obj_desc->field.region_obj->region.space_id ==
			   ACPI_ADR_SPACE_GSBUS) {
			accessor_type = obj_desc->field.attribute;
			length =
			    acpi_ex_get_serial_access_length(accessor_type,
							     obj_desc->field.
							     access_length);

			/*
			 * Add additional 2 bytes for the generic_serial_bus data buffer:
			 *
			 *     Status;    (Byte 0 of the data buffer)
			 *     Length;    (Byte 1 of the data buffer)
			 *     Data[x-1]: (Bytes 2-x of the arbitrary length data buffer)
			 */
			length += 2;
			function = ACPI_READ | (accessor_type << 16);
		} else {	/* IPMI */

			length = ACPI_IPMI_BUFFER_SIZE;
			function = ACPI_READ;
		}

		buffer_desc = acpi_ut_create_buffer_object(length);
		if (!buffer_desc) {
			return_ACPI_STATUS(AE_NO_MEMORY);
		}

		/* Lock entire transaction if requested */

		acpi_ex_acquire_global_lock(obj_desc->common_field.field_flags);

		/* Call the region handler for the read */

		status = acpi_ex_access_region(obj_desc, 0,
					       ACPI_CAST_PTR(u64,
							     buffer_desc->
							     buffer.pointer),
					       function);

		acpi_ex_release_global_lock(obj_desc->common_field.field_flags);
		goto exit;
=======
		    ACPI_ADR_SPACE_IPMI
		    || obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_PLATFORM_RT)) {

		/* SMBus, GSBus, IPMI serial */

		status = acpi_ex_read_serial_bus(obj_desc, ret_buffer_desc);
		return_ACPI_STATUS(status);
>>>>>>> upstream/android-13
	}

	/*
	 * Allocate a buffer for the contents of the field.
	 *
	 * If the field is larger than the current integer width, create
	 * a BUFFER to hold it. Otherwise, use an INTEGER. This allows
	 * the use of arithmetic operators on the returned value if the
	 * field size is equal or smaller than an Integer.
	 *
<<<<<<< HEAD
	 * Note: Field.length is in bits.
	 */
	length =
	    (acpi_size)ACPI_ROUND_BITS_UP_TO_BYTES(obj_desc->field.bit_length);

	if (length > acpi_gbl_integer_byte_width) {

		/* Field is too large for an Integer, create a Buffer instead */

		buffer_desc = acpi_ut_create_buffer_object(length);
=======
	 * However, all buffer fields created by create_field operator needs to
	 * remain as a buffer to match other AML interpreter implementations.
	 *
	 * Note: Field.length is in bits.
	 */
	buffer_length =
	    (acpi_size)ACPI_ROUND_BITS_UP_TO_BYTES(obj_desc->field.bit_length);

	if (buffer_length > acpi_gbl_integer_byte_width ||
	    (obj_desc->common.type == ACPI_TYPE_BUFFER_FIELD &&
	     obj_desc->buffer_field.is_create_field)) {

		/* Field is too large for an Integer, create a Buffer instead */

		buffer_desc = acpi_ut_create_buffer_object(buffer_length);
>>>>>>> upstream/android-13
		if (!buffer_desc) {
			return_ACPI_STATUS(AE_NO_MEMORY);
		}
		buffer = buffer_desc->buffer.pointer;
	} else {
		/* Field will fit within an Integer (normal case) */

		buffer_desc = acpi_ut_create_integer_object((u64) 0);
		if (!buffer_desc) {
			return_ACPI_STATUS(AE_NO_MEMORY);
		}

<<<<<<< HEAD
		length = acpi_gbl_integer_byte_width;
=======
		buffer_length = acpi_gbl_integer_byte_width;
>>>>>>> upstream/android-13
		buffer = &buffer_desc->integer.value;
	}

	if ((obj_desc->common.type == ACPI_TYPE_LOCAL_REGION_FIELD) &&
	    (obj_desc->field.region_obj->region.space_id ==
	     ACPI_ADR_SPACE_GPIO)) {
<<<<<<< HEAD
		/*
		 * For GPIO (general_purpose_io), the Address will be the bit offset
		 * from the previous Connection() operator, making it effectively a
		 * pin number index. The bit_length is the length of the field, which
		 * is thus the number of pins.
		 */
		ACPI_DEBUG_PRINT((ACPI_DB_BFIELD,
				  "GPIO FieldRead [FROM]:  Pin %u Bits %u\n",
				  obj_desc->field.pin_number_index,
				  obj_desc->field.bit_length));

		/* Lock entire transaction if requested */

		acpi_ex_acquire_global_lock(obj_desc->common_field.field_flags);

		/* Perform the write */

		status =
		    acpi_ex_access_region(obj_desc, 0, (u64 *)buffer,
					  ACPI_READ);

		acpi_ex_release_global_lock(obj_desc->common_field.field_flags);
		if (ACPI_FAILURE(status)) {
			acpi_ut_remove_reference(buffer_desc);
		} else {
			*ret_buffer_desc = buffer_desc;
		}
		return_ACPI_STATUS(status);
=======

		/* General Purpose I/O */

		status = acpi_ex_read_gpio(obj_desc, buffer);
		goto exit;
	} else if ((obj_desc->common.type == ACPI_TYPE_LOCAL_REGION_FIELD) &&
		   (obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_PLATFORM_COMM)) {
		/*
		 * Reading from a PCC field unit does not require the handler because
		 * it only requires reading from the internal_pcc_buffer.
		 */
		ACPI_DEBUG_PRINT((ACPI_DB_BFIELD,
				  "PCC FieldRead bits %u\n",
				  obj_desc->field.bit_length));

		memcpy(buffer,
		       obj_desc->field.region_obj->field.internal_pcc_buffer +
		       obj_desc->field.base_byte_offset,
		       (acpi_size)ACPI_ROUND_BITS_UP_TO_BYTES(obj_desc->field.
							      bit_length));

		*ret_buffer_desc = buffer_desc;
		return AE_OK;
>>>>>>> upstream/android-13
	}

	ACPI_DEBUG_PRINT((ACPI_DB_BFIELD,
			  "FieldRead [TO]:   Obj %p, Type %X, Buf %p, ByteLen %X\n",
			  obj_desc, obj_desc->common.type, buffer,
<<<<<<< HEAD
			  (u32) length));
=======
			  buffer_length));
>>>>>>> upstream/android-13
	ACPI_DEBUG_PRINT((ACPI_DB_BFIELD,
			  "FieldRead [FROM]: BitLen %X, BitOff %X, ByteOff %X\n",
			  obj_desc->common_field.bit_length,
			  obj_desc->common_field.start_field_bit_offset,
			  obj_desc->common_field.base_byte_offset));

	/* Lock entire transaction if requested */

	acpi_ex_acquire_global_lock(obj_desc->common_field.field_flags);

	/* Read from the field */

<<<<<<< HEAD
	status = acpi_ex_extract_from_field(obj_desc, buffer, (u32) length);
=======
	status = acpi_ex_extract_from_field(obj_desc, buffer, buffer_length);
>>>>>>> upstream/android-13
	acpi_ex_release_global_lock(obj_desc->common_field.field_flags);

exit:
	if (ACPI_FAILURE(status)) {
		acpi_ut_remove_reference(buffer_desc);
	} else {
		*ret_buffer_desc = buffer_desc;
	}

	return_ACPI_STATUS(status);
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ex_write_data_to_field
 *
 * PARAMETERS:  source_desc         - Contains data to write
 *              obj_desc            - The named field
 *              result_desc         - Where the return value is returned, if any
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Write to a named field
 *
 ******************************************************************************/

acpi_status
acpi_ex_write_data_to_field(union acpi_operand_object *source_desc,
			    union acpi_operand_object *obj_desc,
			    union acpi_operand_object **result_desc)
{
	acpi_status status;
<<<<<<< HEAD
	u32 length;
	void *buffer;
	union acpi_operand_object *buffer_desc;
	u32 function;
	u16 accessor_type;
=======
	u32 buffer_length;
	u32 data_length;
	void *buffer;
>>>>>>> upstream/android-13

	ACPI_FUNCTION_TRACE_PTR(ex_write_data_to_field, obj_desc);

	/* Parameter validation */

	if (!source_desc || !obj_desc) {
		return_ACPI_STATUS(AE_AML_NO_OPERAND);
	}

	if (obj_desc->common.type == ACPI_TYPE_BUFFER_FIELD) {
		/*
		 * If the buffer_field arguments have not been previously evaluated,
		 * evaluate them now and save the results.
		 */
		if (!(obj_desc->common.flags & AOPOBJ_DATA_VALID)) {
			status = acpi_ds_get_buffer_field_arguments(obj_desc);
			if (ACPI_FAILURE(status)) {
				return_ACPI_STATUS(status);
			}
		}
	} else if ((obj_desc->common.type == ACPI_TYPE_LOCAL_REGION_FIELD) &&
		   (obj_desc->field.region_obj->region.space_id ==
<<<<<<< HEAD
=======
		    ACPI_ADR_SPACE_GPIO)) {

		/* General Purpose I/O */

		status = acpi_ex_write_gpio(source_desc, obj_desc, result_desc);
		return_ACPI_STATUS(status);
	} else if ((obj_desc->common.type == ACPI_TYPE_LOCAL_REGION_FIELD) &&
		   (obj_desc->field.region_obj->region.space_id ==
>>>>>>> upstream/android-13
		    ACPI_ADR_SPACE_SMBUS
		    || obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_GSBUS
		    || obj_desc->field.region_obj->region.space_id ==
<<<<<<< HEAD
		    ACPI_ADR_SPACE_IPMI)) {
		/*
		 * This is an SMBus, GSBus or IPMI write. We will bypass the entire
		 * field mechanism and handoff the buffer directly to the handler.
		 * For these address spaces, the buffer is bi-directional; on a
		 * write, return data is returned in the same buffer.
		 *
		 * Source must be a buffer of sufficient size:
		 * ACPI_SMBUS_BUFFER_SIZE, ACPI_GSBUS_BUFFER_SIZE, or
		 * ACPI_IPMI_BUFFER_SIZE.
		 *
		 * Note: SMBus and GSBus protocol type is passed in upper 16-bits
		 * of Function
		 */
		if (source_desc->common.type != ACPI_TYPE_BUFFER) {
			ACPI_ERROR((AE_INFO,
				    "SMBus/IPMI/GenericSerialBus write requires "
				    "Buffer, found type %s",
				    acpi_ut_get_object_type_name(source_desc)));

			return_ACPI_STATUS(AE_AML_OPERAND_TYPE);
		}

		if (obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_SMBUS) {
			length = ACPI_SMBUS_BUFFER_SIZE;
			function =
			    ACPI_WRITE | (obj_desc->field.attribute << 16);
		} else if (obj_desc->field.region_obj->region.space_id ==
			   ACPI_ADR_SPACE_GSBUS) {
			accessor_type = obj_desc->field.attribute;
			length =
			    acpi_ex_get_serial_access_length(accessor_type,
							     obj_desc->field.
							     access_length);

			/*
			 * Add additional 2 bytes for the generic_serial_bus data buffer:
			 *
			 *     Status;    (Byte 0 of the data buffer)
			 *     Length;    (Byte 1 of the data buffer)
			 *     Data[x-1]: (Bytes 2-x of the arbitrary length data buffer)
			 */
			length += 2;
			function = ACPI_WRITE | (accessor_type << 16);
		} else {	/* IPMI */

			length = ACPI_IPMI_BUFFER_SIZE;
			function = ACPI_WRITE;
		}

		if (source_desc->buffer.length < length) {
			ACPI_ERROR((AE_INFO,
				    "SMBus/IPMI/GenericSerialBus write requires "
				    "Buffer of length %u, found length %u",
				    length, source_desc->buffer.length));

			return_ACPI_STATUS(AE_AML_BUFFER_LIMIT);
		}

		/* Create the bi-directional buffer */

		buffer_desc = acpi_ut_create_buffer_object(length);
		if (!buffer_desc) {
			return_ACPI_STATUS(AE_NO_MEMORY);
		}

		buffer = buffer_desc->buffer.pointer;
		memcpy(buffer, source_desc->buffer.pointer, length);

		/* Lock entire transaction if requested */

		acpi_ex_acquire_global_lock(obj_desc->common_field.field_flags);

		/*
		 * Perform the write (returns status and perhaps data in the
		 * same buffer)
		 */
		status =
		    acpi_ex_access_region(obj_desc, 0, (u64 *)buffer, function);
		acpi_ex_release_global_lock(obj_desc->common_field.field_flags);

		*result_desc = buffer_desc;
		return_ACPI_STATUS(status);
	} else if ((obj_desc->common.type == ACPI_TYPE_LOCAL_REGION_FIELD) &&
		   (obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_GPIO)) {
		/*
		 * For GPIO (general_purpose_io), we will bypass the entire field
		 * mechanism and handoff the bit address and bit width directly to
		 * the handler. The Address will be the bit offset
		 * from the previous Connection() operator, making it effectively a
		 * pin number index. The bit_length is the length of the field, which
		 * is thus the number of pins.
		 */
		if (source_desc->common.type != ACPI_TYPE_INTEGER) {
			return_ACPI_STATUS(AE_AML_OPERAND_TYPE);
		}

		ACPI_DEBUG_PRINT((ACPI_DB_BFIELD,
				  "GPIO FieldWrite [FROM]: (%s:%X), Val %.8X  [TO]: Pin %u Bits %u\n",
				  acpi_ut_get_type_name(source_desc->common.
							type),
				  source_desc->common.type,
				  (u32)source_desc->integer.value,
				  obj_desc->field.pin_number_index,
				  obj_desc->field.bit_length));

		buffer = &source_desc->integer.value;

		/* Lock entire transaction if requested */

		acpi_ex_acquire_global_lock(obj_desc->common_field.field_flags);

		/* Perform the write */

		status =
		    acpi_ex_access_region(obj_desc, 0, (u64 *)buffer,
					  ACPI_WRITE);
		acpi_ex_release_global_lock(obj_desc->common_field.field_flags);
		return_ACPI_STATUS(status);
=======
		    ACPI_ADR_SPACE_IPMI
		    || obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_PLATFORM_RT)) {

		/* SMBus, GSBus, IPMI serial */

		status =
		    acpi_ex_write_serial_bus(source_desc, obj_desc,
					     result_desc);
		return_ACPI_STATUS(status);
	} else if ((obj_desc->common.type == ACPI_TYPE_LOCAL_REGION_FIELD) &&
		   (obj_desc->field.region_obj->region.space_id ==
		    ACPI_ADR_SPACE_PLATFORM_COMM)) {
		/*
		 * According to the spec a write to the COMD field will invoke the
		 * region handler. Otherwise, write to the pcc_internal buffer. This
		 * implementation will use the offsets specified rather than the name
		 * of the field. This is considered safer because some firmware tools
		 * are known to obfiscate named objects.
		 */
		data_length =
		    (acpi_size)ACPI_ROUND_BITS_UP_TO_BYTES(obj_desc->field.
							   bit_length);
		memcpy(obj_desc->field.region_obj->field.internal_pcc_buffer +
		       obj_desc->field.base_byte_offset,
		       source_desc->buffer.pointer, data_length);

		if (MASTER_SUBSPACE_COMMAND(obj_desc->field.base_byte_offset)) {

			/* Perform the write */

			ACPI_DEBUG_PRINT((ACPI_DB_BFIELD,
					  "PCC COMD field has been written. Invoking PCC handler now.\n"));

			status =
			    acpi_ex_access_region(obj_desc, 0,
						  (u64 *)obj_desc->field.
						  region_obj->field.
						  internal_pcc_buffer,
						  ACPI_WRITE);
			return_ACPI_STATUS(status);
		}
		return (AE_OK);
>>>>>>> upstream/android-13
	}

	/* Get a pointer to the data to be written */

	switch (source_desc->common.type) {
	case ACPI_TYPE_INTEGER:

		buffer = &source_desc->integer.value;
<<<<<<< HEAD
		length = sizeof(source_desc->integer.value);
=======
		buffer_length = sizeof(source_desc->integer.value);
>>>>>>> upstream/android-13
		break;

	case ACPI_TYPE_BUFFER:

		buffer = source_desc->buffer.pointer;
<<<<<<< HEAD
		length = source_desc->buffer.length;
=======
		buffer_length = source_desc->buffer.length;
>>>>>>> upstream/android-13
		break;

	case ACPI_TYPE_STRING:

		buffer = source_desc->string.pointer;
<<<<<<< HEAD
		length = source_desc->string.length;
		break;

	default:

=======
		buffer_length = source_desc->string.length;
		break;

	default:
>>>>>>> upstream/android-13
		return_ACPI_STATUS(AE_AML_OPERAND_TYPE);
	}

	ACPI_DEBUG_PRINT((ACPI_DB_BFIELD,
			  "FieldWrite [FROM]: Obj %p (%s:%X), Buf %p, ByteLen %X\n",
			  source_desc,
			  acpi_ut_get_type_name(source_desc->common.type),
<<<<<<< HEAD
			  source_desc->common.type, buffer, length));
=======
			  source_desc->common.type, buffer, buffer_length));
>>>>>>> upstream/android-13

	ACPI_DEBUG_PRINT((ACPI_DB_BFIELD,
			  "FieldWrite [TO]:   Obj %p (%s:%X), BitLen %X, BitOff %X, ByteOff %X\n",
			  obj_desc,
			  acpi_ut_get_type_name(obj_desc->common.type),
			  obj_desc->common.type,
			  obj_desc->common_field.bit_length,
			  obj_desc->common_field.start_field_bit_offset,
			  obj_desc->common_field.base_byte_offset));

	/* Lock entire transaction if requested */

	acpi_ex_acquire_global_lock(obj_desc->common_field.field_flags);

	/* Write to the field */

<<<<<<< HEAD
	status = acpi_ex_insert_into_field(obj_desc, buffer, length);
	acpi_ex_release_global_lock(obj_desc->common_field.field_flags);

=======
	status = acpi_ex_insert_into_field(obj_desc, buffer, buffer_length);
	acpi_ex_release_global_lock(obj_desc->common_field.field_flags);
>>>>>>> upstream/android-13
	return_ACPI_STATUS(status);
}
