// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
/*******************************************************************************
 *
 * Module Name: nseval - Object evaluation, includes control method execution
 *
 ******************************************************************************/

#include <acpi/acpi.h>
#include "accommon.h"
#include "acparser.h"
#include "acinterp.h"
#include "acnamesp.h"

#define _COMPONENT          ACPI_NAMESPACE
ACPI_MODULE_NAME("nseval")

<<<<<<< HEAD
/* Local prototypes */
static void
acpi_ns_exec_module_code(union acpi_operand_object *method_obj,
			 struct acpi_evaluate_info *info);

=======
>>>>>>> upstream/android-13
/*******************************************************************************
 *
 * FUNCTION:    acpi_ns_evaluate
 *
 * PARAMETERS:  info            - Evaluation info block, contains these fields
 *                                and more:
 *                  prefix_node     - Prefix or Method/Object Node to execute
 *                  relative_path   - Name of method to execute, If NULL, the
 *                                    Node is the object to execute
 *                  parameters      - List of parameters to pass to the method,
 *                                    terminated by NULL. Params itself may be
 *                                    NULL if no parameters are being passed.
 *                  parameter_type  - Type of Parameter list
 *                  return_object   - Where to put method's return value (if
 *                                    any). If NULL, no value is returned.
 *                  flags           - ACPI_IGNORE_RETURN_VALUE to delete return
 *
 * RETURN:      Status
 *
 * DESCRIPTION: Execute a control method or return the current value of an
 *              ACPI namespace object.
 *
 * MUTEX:       Locks interpreter
 *
 ******************************************************************************/
<<<<<<< HEAD

=======
>>>>>>> upstream/android-13
acpi_status acpi_ns_evaluate(struct acpi_evaluate_info *info)
{
	acpi_status status;

	ACPI_FUNCTION_TRACE(ns_evaluate);

	if (!info) {
		return_ACPI_STATUS(AE_BAD_PARAMETER);
	}

	if (!info->node) {
		/*
		 * Get the actual namespace node for the target object if we
		 * need to. Handles these cases:
		 *
		 * 1) Null node, valid pathname from root (absolute path)
		 * 2) Node and valid pathname (path relative to Node)
		 * 3) Node, Null pathname
		 */
		status =
		    acpi_ns_get_node(info->prefix_node, info->relative_pathname,
				     ACPI_NS_NO_UPSEARCH, &info->node);
		if (ACPI_FAILURE(status)) {
			return_ACPI_STATUS(status);
		}
	}

	/*
	 * For a method alias, we must grab the actual method node so that
	 * proper scoping context will be established before execution.
	 */
	if (acpi_ns_get_type(info->node) == ACPI_TYPE_LOCAL_METHOD_ALIAS) {
		info->node =
		    ACPI_CAST_PTR(struct acpi_namespace_node,
				  info->node->object);
	}

	/* Complete the info block initialization */

	info->return_object = NULL;
	info->node_flags = info->node->flags;
	info->obj_desc = acpi_ns_get_attached_object(info->node);

	ACPI_DEBUG_PRINT((ACPI_DB_NAMES, "%s [%p] Value %p\n",
			  info->relative_pathname, info->node,
			  acpi_ns_get_attached_object(info->node)));

	/* Get info if we have a predefined name (_HID, etc.) */

	info->predefined =
	    acpi_ut_match_predefined_method(info->node->name.ascii);

	/* Get the full pathname to the object, for use in warning messages */

	info->full_pathname = acpi_ns_get_normalized_pathname(info->node, TRUE);
	if (!info->full_pathname) {
		return_ACPI_STATUS(AE_NO_MEMORY);
	}

<<<<<<< HEAD
=======
	/* Optional object evaluation log */

	ACPI_DEBUG_PRINT_RAW((ACPI_DB_EVALUATION,
			      "%-26s:  %s (%s)\n", "   Enter evaluation",
			      &info->full_pathname[1],
			      acpi_ut_get_type_name(info->node->type)));

>>>>>>> upstream/android-13
	/* Count the number of arguments being passed in */

	info->param_count = 0;
	if (info->parameters) {
		while (info->parameters[info->param_count]) {
			info->param_count++;
		}

		/* Warn on impossible argument count */

		if (info->param_count > ACPI_METHOD_NUM_ARGS) {
			ACPI_WARN_PREDEFINED((AE_INFO, info->full_pathname,
					      ACPI_WARN_ALWAYS,
					      "Excess arguments (%u) - using only %u",
					      info->param_count,
					      ACPI_METHOD_NUM_ARGS));

			info->param_count = ACPI_METHOD_NUM_ARGS;
		}
	}

	/*
	 * For predefined names: Check that the declared argument count
	 * matches the ACPI spec -- otherwise this is a BIOS error.
	 */
	acpi_ns_check_acpi_compliance(info->full_pathname, info->node,
				      info->predefined);

	/*
	 * For all names: Check that the incoming argument count for
	 * this method/object matches the actual ASL/AML definition.
	 */
	acpi_ns_check_argument_count(info->full_pathname, info->node,
				     info->param_count, info->predefined);

	/* For predefined names: Typecheck all incoming arguments */

	acpi_ns_check_argument_types(info);

	/*
	 * Three major evaluation cases:
	 *
	 * 1) Object types that cannot be evaluated by definition
	 * 2) The object is a control method -- execute it
	 * 3) The object is not a method -- just return it's current value
	 */
	switch (acpi_ns_get_type(info->node)) {
	case ACPI_TYPE_ANY:
	case ACPI_TYPE_DEVICE:
	case ACPI_TYPE_EVENT:
	case ACPI_TYPE_MUTEX:
	case ACPI_TYPE_REGION:
	case ACPI_TYPE_THERMAL:
	case ACPI_TYPE_LOCAL_SCOPE:
		/*
		 * 1) Disallow evaluation of these object types. For these,
		 *    object evaluation is undefined.
		 */
		ACPI_ERROR((AE_INFO,
			    "%s: This object type [%s] "
			    "never contains data and cannot be evaluated",
			    info->full_pathname,
			    acpi_ut_get_type_name(info->node->type)));

		status = AE_TYPE;
		goto cleanup;

	case ACPI_TYPE_METHOD:
		/*
		 * 2) Object is a control method - execute it
		 */

		/* Verify that there is a method object associated with this node */

		if (!info->obj_desc) {
			ACPI_ERROR((AE_INFO,
				    "%s: Method has no attached sub-object",
				    info->full_pathname));
			status = AE_NULL_OBJECT;
			goto cleanup;
		}

		ACPI_DEBUG_PRINT((ACPI_DB_EXEC,
				  "**** Execute method [%s] at AML address %p length %X\n",
				  info->full_pathname,
				  info->obj_desc->method.aml_start + 1,
				  info->obj_desc->method.aml_length - 1));

		/*
		 * Any namespace deletion must acquire both the namespace and
		 * interpreter locks to ensure that no thread is using the portion of
		 * the namespace that is being deleted.
		 *
		 * Execute the method via the interpreter. The interpreter is locked
		 * here before calling into the AML parser
		 */
		acpi_ex_enter_interpreter();
		status = acpi_ps_execute_method(info);
		acpi_ex_exit_interpreter();
		break;

	default:
		/*
		 * 3) All other non-method objects -- get the current object value
		 */

		/*
		 * Some objects require additional resolution steps (e.g., the Node
		 * may be a field that must be read, etc.) -- we can't just grab
		 * the object out of the node.
		 *
		 * Use resolve_node_to_value() to get the associated value.
		 *
		 * NOTE: we can get away with passing in NULL for a walk state because
		 * the Node is guaranteed to not be a reference to either a method
		 * local or a method argument (because this interface is never called
		 * from a running method.)
		 *
		 * Even though we do not directly invoke the interpreter for object
		 * resolution, we must lock it because we could access an op_region.
		 * The op_region access code assumes that the interpreter is locked.
		 */
		acpi_ex_enter_interpreter();

		/* TBD: resolve_node_to_value has a strange interface, fix */

		info->return_object =
		    ACPI_CAST_PTR(union acpi_operand_object, info->node);

		status =
		    acpi_ex_resolve_node_to_value(ACPI_CAST_INDIRECT_PTR
						  (struct acpi_namespace_node,
						   &info->return_object), NULL);
		acpi_ex_exit_interpreter();

		if (ACPI_FAILURE(status)) {
			info->return_object = NULL;
			goto cleanup;
		}

		ACPI_DEBUG_PRINT((ACPI_DB_NAMES, "Returned object %p [%s]\n",
				  info->return_object,
				  acpi_ut_get_object_type_name(info->
							       return_object)));

		status = AE_CTRL_RETURN_VALUE;	/* Always has a "return value" */
		break;
	}

	/*
	 * For predefined names, check the return value against the ACPI
	 * specification. Some incorrect return value types are repaired.
	 */
	(void)acpi_ns_check_return_value(info->node, info, info->param_count,
					 status, &info->return_object);

	/* Check if there is a return value that must be dealt with */

	if (status == AE_CTRL_RETURN_VALUE) {

		/* If caller does not want the return value, delete it */

		if (info->flags & ACPI_IGNORE_RETURN_VALUE) {
			acpi_ut_remove_reference(info->return_object);
			info->return_object = NULL;
		}

		/* Map AE_CTRL_RETURN_VALUE to AE_OK, we are done with it */

		status = AE_OK;
	} else if (ACPI_FAILURE(status)) {

		/* If return_object exists, delete it */

		if (info->return_object) {
			acpi_ut_remove_reference(info->return_object);
			info->return_object = NULL;
		}
	}

	ACPI_DEBUG_PRINT((ACPI_DB_NAMES,
			  "*** Completed evaluation of object %s ***\n",
			  info->relative_pathname));

cleanup:
<<<<<<< HEAD
=======
	/* Optional object evaluation log */

	ACPI_DEBUG_PRINT_RAW((ACPI_DB_EVALUATION,
			      "%-26s:  %s\n", "   Exit evaluation",
			      &info->full_pathname[1]));

>>>>>>> upstream/android-13
	/*
	 * Namespace was unlocked by the handling acpi_ns* function, so we
	 * just free the pathname and return
	 */
	ACPI_FREE(info->full_pathname);
	info->full_pathname = NULL;
	return_ACPI_STATUS(status);
}
<<<<<<< HEAD

/*******************************************************************************
 *
 * FUNCTION:    acpi_ns_exec_module_code_list
 *
 * PARAMETERS:  None
 *
 * RETURN:      None. Exceptions during method execution are ignored, since
 *              we cannot abort a table load.
 *
 * DESCRIPTION: Execute all elements of the global module-level code list.
 *              Each element is executed as a single control method.
 *
 * NOTE: With this option enabled, each block of detected executable AML
 * code that is outside of any control method is wrapped with a temporary
 * control method object and placed on a global list. The methods on this
 * list are executed below.
 *
 * This function executes the module-level code for all tables only after
 * all of the tables have been loaded. It is a legacy option and is
 * not compatible with other ACPI implementations. See acpi_ns_load_table.
 *
 * This function will be removed when the legacy option is removed.
 *
 ******************************************************************************/

void acpi_ns_exec_module_code_list(void)
{
	union acpi_operand_object *prev;
	union acpi_operand_object *next;
	struct acpi_evaluate_info *info;
	u32 method_count = 0;

	ACPI_FUNCTION_TRACE(ns_exec_module_code_list);

	/* Exit now if the list is empty */

	next = acpi_gbl_module_code_list;
	if (!next) {
		ACPI_DEBUG_PRINT((ACPI_DB_INIT_NAMES,
				  "Legacy MLC block list is empty\n"));

		return_VOID;
	}

	/* Allocate the evaluation information block */

	info = ACPI_ALLOCATE(sizeof(struct acpi_evaluate_info));
	if (!info) {
		return_VOID;
	}

	/* Walk the list, executing each "method" */

	while (next) {
		prev = next;
		next = next->method.mutex;

		/* Clear the link field and execute the method */

		prev->method.mutex = NULL;
		acpi_ns_exec_module_code(prev, info);
		method_count++;

		/* Delete the (temporary) method object */

		acpi_ut_remove_reference(prev);
	}

	ACPI_INFO(("Executed %u blocks of module-level executable AML code",
		   method_count));

	ACPI_FREE(info);
	acpi_gbl_module_code_list = NULL;
	return_VOID;
}

/*******************************************************************************
 *
 * FUNCTION:    acpi_ns_exec_module_code
 *
 * PARAMETERS:  method_obj          - Object container for the module-level code
 *              info                - Info block for method evaluation
 *
 * RETURN:      None. Exceptions during method execution are ignored, since
 *              we cannot abort a table load.
 *
 * DESCRIPTION: Execute a control method containing a block of module-level
 *              executable AML code. The control method is temporarily
 *              installed to the root node, then evaluated.
 *
 ******************************************************************************/

static void
acpi_ns_exec_module_code(union acpi_operand_object *method_obj,
			 struct acpi_evaluate_info *info)
{
	union acpi_operand_object *parent_obj;
	struct acpi_namespace_node *parent_node;
	acpi_object_type type;
	acpi_status status;

	ACPI_FUNCTION_TRACE(ns_exec_module_code);

	/*
	 * Get the parent node. We cheat by using the next_object field
	 * of the method object descriptor.
	 */
	parent_node =
	    ACPI_CAST_PTR(struct acpi_namespace_node,
				    method_obj->method.next_object);
	type = acpi_ns_get_type(parent_node);

	/*
	 * Get the region handler and save it in the method object. We may need
	 * this if an operation region declaration causes a _REG method to be run.
	 *
	 * We can't do this in acpi_ps_link_module_code because
	 * acpi_gbl_root_node->Object is NULL at PASS1.
	 */
	if ((type == ACPI_TYPE_DEVICE) && parent_node->object) {
		method_obj->method.dispatch.handler =
		    parent_node->object->device.handler;
	}

	/* Must clear next_object (acpi_ns_attach_object needs the field) */

	method_obj->method.next_object = NULL;

	/* Initialize the evaluation information block */

	memset(info, 0, sizeof(struct acpi_evaluate_info));
	info->prefix_node = parent_node;

	/*
	 * Get the currently attached parent object. Add a reference,
	 * because the ref count will be decreased when the method object
	 * is installed to the parent node.
	 */
	parent_obj = acpi_ns_get_attached_object(parent_node);
	if (parent_obj) {
		acpi_ut_add_reference(parent_obj);
	}

	/* Install the method (module-level code) in the parent node */

	status =
	    acpi_ns_attach_object(parent_node, method_obj, ACPI_TYPE_METHOD);
	if (ACPI_FAILURE(status)) {
		goto exit;
	}

	/* Execute the parent node as a control method */

	status = acpi_ns_evaluate(info);

	ACPI_DEBUG_PRINT((ACPI_DB_INIT_NAMES,
			  "Executed module-level code at %p\n",
			  method_obj->method.aml_start));

	/* Delete a possible implicit return value (in slack mode) */

	if (info->return_object) {
		acpi_ut_remove_reference(info->return_object);
	}

	/* Detach the temporary method object */

	acpi_ns_detach_object(parent_node);

	/* Restore the original parent object */

	if (parent_obj) {
		status = acpi_ns_attach_object(parent_node, parent_obj, type);
	} else {
		parent_node->type = (u8)type;
	}

exit:
	if (parent_obj) {
		acpi_ut_remove_reference(parent_obj);
	}
	return_VOID;
}
=======
>>>>>>> upstream/android-13
