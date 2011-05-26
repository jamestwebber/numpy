#ifndef _NPY_PRIVATE__DATETIME_H_
#define _NPY_PRIVATE__DATETIME_H_

NPY_NO_EXPORT void
numpy_pydatetime_import();

NPY_NO_EXPORT void
PyArray_DatetimeToDatetimeStruct(npy_datetime val, NPY_DATETIMEUNIT fr,
                                 npy_datetimestruct *result);

NPY_NO_EXPORT void
PyArray_TimedeltaToTimedeltaStruct(npy_timedelta val, NPY_DATETIMEUNIT fr,
                                 npy_timedeltastruct *result);

NPY_NO_EXPORT npy_datetime
PyArray_DatetimeStructToDatetime(NPY_DATETIMEUNIT fr, npy_datetimestruct *d);

NPY_NO_EXPORT npy_datetime
PyArray_TimedeltaStructToTimedelta(NPY_DATETIMEUNIT fr, npy_timedeltastruct *d);

/*
 * This function returns a pointer to the DateTimeMetaData
 * contained within the provided datetime dtype.
 */
NPY_NO_EXPORT PyArray_DatetimeMetaData *
get_datetime_metadata_from_dtype(PyArray_Descr *dtype);

/*
 * Both type1 and type2 must be either NPY_DATETIME or NPY_TIMEDELTA.
 * Applies the type promotion rules between the two types, returning
 * the promoted type.
 */
NPY_NO_EXPORT PyArray_Descr *
datetime_type_promotion(PyArray_Descr *type1, PyArray_Descr *type2);

/*
 * Converts a datetime from a datetimestruct to a datetime based
 * on some metadata.
 */
NPY_NO_EXPORT int
convert_datetimestruct_to_datetime(PyArray_DatetimeMetaData *meta,
                                    const npy_datetimestruct *dts,
                                    npy_datetime *out);

/*
 * This function returns a reference to a capsule
 * which contains the datetime metadata parsed from a metadata
 * string. 'metastr' should be NULL-terminated, and len should
 * contain its string length.
 */
NPY_NO_EXPORT PyObject *
parse_datetime_metacobj_from_metastr(char *metastr, Py_ssize_t len);

/*
 * Converts a datetype dtype string into a dtype descr object.
 * The "type" string should be NULL-terminated, and len should
 * contain its string length.
 */
NPY_NO_EXPORT PyArray_Descr *
parse_dtype_from_datetime_typestr(char *typestr, Py_ssize_t len);

/*
 * Converts a substring given by 'str' and 'len' into
 * a date time unit enum value. The 'metastr' parameter
 * is used for error messages, and may be NULL.
 *
 * Returns 0 on success, -1 on failure.
 */
NPY_NO_EXPORT NPY_DATETIMEUNIT
parse_datetime_unit_from_string(char *str, Py_ssize_t len, char *metastr);

/*
 * Translate divisors into multiples of smaller units.
 * 'metastr' is used for the error message if the divisor doesn't work,
 * and can be NULL if the metadata didn't come from a string.
 *
 * Returns 0 on success, -1 on failure.
 */
NPY_NO_EXPORT int
convert_datetime_divisor_to_multiple(PyArray_DatetimeMetaData *meta,
                                    int den, char *metastr);
/*
 * Computes the GCD of the two date-time metadata values. Raises
 * an exception if there is no reasonable GCD, such as with
 * years and days.
 *
 * Returns a capsule with the GCD metadata.
 */
NPY_NO_EXPORT PyObject *
compute_datetime_metadata_greatest_common_divisor(
                        PyArray_Descr *type1,
                        PyArray_Descr *type2);

/*
 * Given an the capsule datetime metadata object,
 * returns a tuple for pickling and other purposes.
 */
NPY_NO_EXPORT PyObject *
convert_datetime_metadata_to_tuple(PyArray_DatetimeMetaData *meta);

/*
 * Given a tuple representing datetime metadata,
 * returns a capsule datetime metadata object.
 */
NPY_NO_EXPORT PyObject *
convert_datetime_metadata_tuple_to_metacobj(PyObject *tuple);

/*
 * 'ret' is a PyUString containing the datetime string, and this
 * function appends the metadata string to it.
 *
 * This function steals the reference 'ret'
 */
NPY_NO_EXPORT PyObject *
append_metastr_to_datetime_typestr(PyArray_Descr *self, PyObject *ret);

/*
 * Parses (almost) standard ISO 8601 date strings. The differences are:
 *
 * + The date "20100312" is parsed as the year 20100312, not as
 *   equivalent to "2010-03-12". The '-' in the dates are not optional.
 * + Only seconds may have a decimal point, with up to 18 digits after it
 *   (maximum attoseconds precision).
 * + Either a 'T' as in ISO 8601 or a ' ' may be used to separate
 *   the date and the time. Both are treated equivalently.
 *
 * 'str' must be a NULL-terminated string, and 'len' must be its length.
 *
 * Returns 0 on success, -1 on failure.
 */
NPY_NO_EXPORT int
parse_iso_8601_date(char *str, int len, npy_datetimestruct *out);

/*
 * Tests for and converts a Python datetime.datetime or datetime.date
 * object into a NumPy npy_datetimestruct.
 *
 * Returns -1 on error, 0 on success, and 1 (with no error set)
 * if obj doesn't have the neeeded date or datetime attributes.
 */
NPY_NO_EXPORT int
convert_pydatetime_to_datetimestruct(PyObject *obj, npy_datetimestruct *out);

/*
 * Converts a PyObject * into a datetime, in any of the input forms supported.
 *
 * Returns -1 on error, 0 on success.
 */
NPY_NO_EXPORT int
convert_pyobject_to_datetime(PyArray_DatetimeMetaData *meta, PyObject *obj,
                                npy_datetime *out);

/*
 * Converts a datetime into a PyObject *.
 *
 * For days or coarser, returns a datetime.date.
 * For microseconds or coarser, returns a datetime.datetime.
 * For units finer than microseconds, returns an integer.
 */
NPY_NO_EXPORT PyObject *
convert_datetime_to_pyobject(npy_datetime dt, PyArray_DatetimeMetaData *meta);

#endif
