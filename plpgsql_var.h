#ifndef PLVAR_H
#define PLVAR_H

#include "postgres.h"

#include "lib/stringinfo.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"
#include "globalbp.h"
#include "utils/array.h"
#include "utils/builtins.h"
#include "utils/syscache.h"
#include "utils/lsyscache.h"
#include "utils/datum.h"
#include "utils/rel.h"
#include "miscadmin.h"
#include "funcapi.h"
#include "executor/spi.h"
#include "parser/parse_type.h"

#if (PG_VERSION_NUM >= 130000)
#include "access/detoast.h"
#elif (PG_VERSION_NUM >= 120000)
#include "access/tuptoaster.h"
#else
#include "access/tuptoaster.h"
#include "access/htup_details.h"
#endif

#define get_eval_mcontext(estate) \
	((estate)->eval_econtext->ecxt_per_tuple_memory)
#define eval_mcontext_alloc(estate, sz) \
	MemoryContextAlloc(get_eval_mcontext(estate), sz)
#define eval_mcontext_alloc0(estate, sz) \
	MemoryContextAllocZero(get_eval_mcontext(estate), sz)

#if INCLUDE_PACKAGE_SUPPORT
#include "spl.h"
#include "catalog/edb_variable.h"
#else
#include "plpgsql.h"
#endif

extern void exec_eval_datum(PLpgSQL_execstate *estate, PLpgSQL_datum *datum, Oid *typeid, int32 *typetypmod, Datum *value, bool *isnull);

extern void print_datum(StringInfo stringInfo, PLpgSQL_execstate *estate, Datum datumValue, Oid oid);

#endif