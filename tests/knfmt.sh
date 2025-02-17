#/bin/sh

set -e

# commstrip file
#
# Removed the leading block comment from file.
commstrip() {
	awk '!p && !length($0) {p=1; next} p {print}' "$1"
}

_wrkdir="$(mktemp -dt knfmt.XXXXXX)"
trap "rm -rf ${_wrkdir}" 0
_out="${_wrkdir}/out"

case "$1" in
error-*)
	_err=0
	${EXEC:-} ${KNFMT} "$1" >"$_out" 2>&1 || _err="$?"
	if [ "$_err" -ne 1 ]; then
		cat "$_out" 1>&2
		echo "${1}; expected exit 1, got ${_err}" 1>&2
		exit 1
	fi
	;;
*)
	_ok="${1%.c}.ok"
	if [ -e "$_ok" ]; then
		_tmp="${_wrkdir}/tmp"
		commstrip "$1" >"$_tmp"
		if ! ${EXEC:-} "${KNFMT}" -v "$_tmp" 2>&1 |
			diff -u -L "$1" -L "$_ok" "$_ok" - >"$_out" 2>&1
		then
			cat "$_out" 1>&2
			exit 1
		fi
	else
		if ! ${EXEC:-} "${KNFMT}" -dv "$1" >"$_out" 2>&1; then
			cat "$_out" 1>&2
			exit 1
		fi
	fi
	if ! cmp -s /dev/null "$_out"; then
		cat "$_out" 1>&2
		exit 1
	fi
	;;
esac
